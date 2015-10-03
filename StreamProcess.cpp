//
// Created by chloridecull on 9/29/15.
//

#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <sys/un.h>
#include <thread>
#include <fcntl.h>
#include "StreamProcess.hpp"
#include "Util.hpp"

StreamProcess::StreamProcess(int Connection, std::list<StreamProcess*> *ExistingStreams,
                             std::unordered_map<std::string, std::string>* KUMap) {
    connid = Connection;
    streamlists = ExistingStreams;
    keyusermap = KUMap;
    StreamName = "";
}

void StreamProcess::RunStreamProcesses() {
    // Setup master RTMP input
    std::cout << "  Firing up master ffmpeg decoder instance, and eavesdropping for stream key." << std::endl;
    int masterffmpegsocket = socket(AF_UNIX, SOCK_STREAM, 0);
    std::array<int, 3> baseproc = Util::LaunchSubprocess("ffmpeg", {
                                                                 "-rtmp_listen", "1", "-rtmp_live", "1",
                                                                 "-i", "rtmp://203.0.113.13/mediacourier/unknown_stream",
                                                                 "-f", "mpegts", "-strict", "experimental",
                                                                 "-c:a", "aac", "-b:a", "512k",
                                                                 "-c:v", "libx264", "-preset", "ultrafast", "-qp", "0",
                                                                 "pipe://"
                                                         }, {
                                                                 std::make_pair(
                                                                         "MCNETREDIR_SOCKPATH",
                                                                         "/tmp/mediacourier-" + std::to_string(connid)
                                                                 ), std::make_pair(
                                                                         "LD_PRELOAD",
                                                                         "libMCNetRedir.so"
                                                                 )
                                                         });
    sockaddr_un sockpath;
    memset(&sockpath, 0, sizeof(sockaddr_un));
    sockpath.sun_family = AF_UNIX;
    strcpy(sockpath.sun_path, ("/tmp/mediacourier-" + std::to_string(connid)).c_str());
    if (connect(masterffmpegsocket, (sockaddr*)&sockpath, sizeof(sockaddr_un)) == -1)
        throw std::runtime_error("Failed to connect to ffmpeg master! " + std::string(strerror(errno)));
    std::thread([this, masterffmpegsocket](){
        PassMasterStream(connid, masterffmpegsocket);
    });
}

void StreamProcess::PassMasterStream(int outside, int ffmpeg) {
    fcntl(outside, F_SETFL, fcntl(outside, F_GETFL) & O_NONBLOCK);
    fcntl(ffmpeg, F_SETFL, fcntl(ffmpeg, F_GETFL) & O_NONBLOCK);
    char packettmp[4096];
    ssize_t recvpackets = 0;
    while (true) {
        // outside -> ffmpeg
        recvpackets = recv(outside, packettmp, 4096, 0);
        if (recvpackets == -1) {
            if (errno != EWOULDBLOCK)
                throw std::runtime_error("Socket read failed from outside to ffmpeg!" + std::string(strerror(errno)));
        } else if (recvpackets == 0) {
            shutdown(ffmpeg, SHUT_RDWR);
            shutdown(outside, SHUT_RDWR);
            break;
        }
        if (send(ffmpeg, packettmp, recvpackets, 0) == -1)
            throw std::runtime_error("Socket write failed from outside to ffmpeg!" + std::string(strerror(errno)));

        // see if we can find an FCPublish command with a key, if we haven't already
        if (StreamName == "") {
            for (int i = 0; i < recvpackets; ++i) {
                if (memcmp(packettmp + i, "FCPublish", 8) == 0) {
                    std::cout << "  Found FCPublish message!" << std::endl;
                    for (int j = 0; (j < i) && (j < recvpackets); ++j) {
                        if (memcmp(packettmp + j, "KEY-", 4) == 0) {
                            std::cout << "  Found key marker!" << std::endl;
                            std::string KeyStr(packettmp + j + 4, 34);
                            if ((KeyStr[32] != '-') ||
                                (KeyStr[33] != 'K')) {
                                std::cout << "  But the key was invalid." << std::endl;
                                shutdown(ffmpeg, SHUT_RDWR);
                                shutdown(outside, SHUT_RDWR);
                                return;
                            } else {
                                if (keyusermap->count(KeyStr) == 1) {
                                    for (auto streaminspect = streamlists->begin();
                                         streaminspect != streamlists->end(); streaminspect++) {
                                        if ((*streaminspect)->StreamName == (*keyusermap)[KeyStr]) {
                                            std::cout << "  But the user was already streaming." << std::endl;
                                            shutdown(ffmpeg, SHUT_RDWR);
                                            shutdown(outside, SHUT_RDWR);
                                            return;
                                        }
                                    }
                                    std::cout << "  It validated as user " << (*keyusermap)[KeyStr] << "!" << std::endl;
                                    std::cout << "  We stop eavesdropping now." << std::endl;
                                    StreamName = (*keyusermap)[KeyStr];
                                    break;
                                } else {
                                    std::cout << "  But the key wasn't registered." << std::endl;
                                    shutdown(ffmpeg, SHUT_RDWR);
                                    shutdown(outside, SHUT_RDWR);
                                    return;
                                }
                            }
                        }
                    }
                    std::cout << "  But we didn't find a key." << std::endl;
                    shutdown(ffmpeg, SHUT_RDWR);
                    shutdown(outside, SHUT_RDWR);
                    return;
                }
            }
        }

        // ffmpeg -> outside
        recvpackets = recv(ffmpeg, packettmp, 4096, 0);
        if (recvpackets == -1) {
            if (errno != EWOULDBLOCK)
                throw std::runtime_error("Socket read failed from ffmpeg to outside!" + std::string(strerror(errno)));
        } else if (recvpackets == 0) {
            shutdown(ffmpeg, SHUT_RDWR);
            shutdown(outside, SHUT_RDWR);
            break;
        }
        if (send(outside, packettmp, recvpackets, 0) == -1)
            throw std::runtime_error("Socket write failed from ffmpeg to outside!" + std::string(strerror(errno)));
    }
}
