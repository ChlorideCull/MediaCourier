//
// Created by chloridecull on 9/29/15.
//

#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <sys/un.h>
#include <thread>
#include <chrono>
#include <fcntl.h>
#include <unistd.h>
#include "StreamProcess.hpp"
#include "Util.hpp"
#include "DBGUtils.hpp"

#define CHILDPASSBUFSIZE 4*1024*1024
#define MAINPASSBUFSIZE 1*1024*1024

StreamProcess::StreamProcess(int Connection, std::list<StreamProcess*> *ExistingStreams,
                             std::unordered_map<std::string, std::string>* KUMap) {
    connid = Connection;
    // TODO: Just ask the Hive if the user is streaming already, instead.
    streamlists = ExistingStreams;
    // TODO: Look into replacing the KeyUser map, instead querying the Hive for the corresponding user when we get a key
    keyusermap = KUMap;
    StreamName = "";
    killThread = false;
}

void StreamProcess::RunStreamProcesses() {
    // Setup master RTMP input
    std::string connsuffix = std::to_string(connid) + "_" + std::to_string(rand());
    std::cout << "  Firing up master ffmpeg decoder instance, and eavesdropping for stream key." << std::endl;
    int masterffmpegsocket = socket(AF_UNIX, SOCK_STREAM, 0);
    std::array<int, 3> baseproc = Util::LaunchSubprocess("ffmpeg", { "ffmpeg", "-v", "debug",
                                                                 "-rtmp_listen", "1", "-rtmp_live", "1",
                                                                 "-i", "rtmp://203.0.113.13/mediacourier/unknown_stream",
                                                                 "-f", "mpegts", "-strict", "experimental",
                                                                 "-c:a", "aac", "-b:a", "512k",
                                                                 "-c:v", "libx264", "-preset", "ultrafast", "-qp", "0",
                                                                 "pipe://"
                                                         }, {
                                                                 "MCNETREDIR_SOCKPATH=/tmp/mediacourier-" + connsuffix,
                                                                 std::string("LD_PRELOAD=") + BINDIR + "/libMCNetRedir.so"
                                                         });
    // Connect to the ffmpeg RTMP unix socket
    sockaddr_un sockpath;
    memset(&sockpath, 0, sizeof(sockaddr_un));
    sockpath.sun_family = AF_UNIX;
    strcpy(sockpath.sun_path, ("/tmp/mediacourier-" + connsuffix).c_str());
    char stderrbuf[2048];
    while (true) {
        std::cerr << std::string(stderrbuf, read(baseproc[2], stderrbuf, 2047));
        if (connect(masterffmpegsocket, (sockaddr*)&sockpath, sizeof(sockaddr_un)) == -1) {
            if (errno != ENOENT)
                throw std::runtime_error("Failed to connect to ffmpeg master! " + std::string(strerror(errno)));
        } else {
            break;
        }
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(50));
    }

    std::thread* childProc = new std::thread([&](){
        char childdatabuf[CHILDPASSBUFSIZE];
        while (!killThread) {
            ssize_t dataread = read(baseproc[1], childdatabuf, CHILDPASSBUFSIZE);
            for (auto subproc = subprocesses.begin(); subproc != subprocesses.end(); subproc++) {
                // TODO: Pass the data to child processes initialized with the arguments from the config we got from the hive when we authenticated the streamer.
            }
            DBGPRINT("Swallowed " + std::to_string(dataread) + " bytes.");
        }
    });
    PassMasterStream(connid, masterffmpegsocket, baseproc[2]);
    killThread = true;
    if (childProc->joinable())
        childProc->join();
    delete childProc;
}

ssize_t passSocket(char* packettmp, std::string sfrom, int from, std::string sto, int to) {
    ssize_t recvpackets = recv(from, packettmp, MAINPASSBUFSIZE, 0);
    if (recvpackets == -1) {
        if ((errno != EWOULDBLOCK) && (errno != EAGAIN))
            throw std::runtime_error("Socket read failed from " + sfrom + " to " + sto + "! " + std::string(strerror(errno)));
        else
            return 0;
    } else if (recvpackets == 0) {
        DBGPRINT("Connection to " + sfrom + " was cut.");
        shutdown(to, SHUT_RDWR);
        shutdown(from, SHUT_RDWR);
        return -1;
    } else {
        DBGPRINT("Got " + std::to_string(recvpackets) + " packets from " + sfrom + ".");
        ssize_t sentpackets = -1;
        while (sentpackets == -1) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            sentpackets = send(to, packettmp, recvpackets, 0);
            if ((sentpackets == -1) && (errno != EWOULDBLOCK) && (errno != EAGAIN))
                throw std::runtime_error("Socket write failed from " + sfrom + " to " + sto + "! " + std::string(strerror(errno)) + "(" + std::to_string(errno) + ")");
        }
        DBGPRINT("Sent " + std::to_string(recvpackets) + " packets to " + sto + ".");
    }
    return recvpackets;
}

void StreamProcess::PassMasterStream(const int outside, const int ffmpeg, const int ffmpegstderr) {
    DBGPRINT("Beginning to pass stream " + std::to_string(outside) + " from outside to " + std::to_string(ffmpeg) + ".");
    fcntl(outside, F_SETFL, fcntl(outside, F_GETFL) | O_NONBLOCK);
    fcntl(ffmpeg, F_SETFL, fcntl(ffmpeg, F_GETFL) | O_NONBLOCK);
    fcntl(ffmpegstderr, F_SETFL, fcntl(ffmpegstderr, F_GETFL) | O_NONBLOCK);
    char packettmp[MAINPASSBUFSIZE];
    char stderrbuf[2048];
    while (true) {
        ssize_t errreadlen = read(ffmpegstderr, stderrbuf, 2047);
        if (errreadlen == -1) {
            if ((errno != EWOULDBLOCK) && (errno != EAGAIN))
                throw std::runtime_error("File read failed from ffmpeg stderr! " + std::string(strerror(errno)));
        } else {
            std::cerr << std::string(stderrbuf, errreadlen);
        }
        // outside -> ffmpeg
        ssize_t recvpackets = passSocket(packettmp, "outside", outside, "ffmpeg", ffmpeg);
        if (recvpackets == -1)
            break;

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

                    if (StreamName == "") {
                        std::cout << "  But we didn't find a key." << std::endl;
                        shutdown(ffmpeg, SHUT_RDWR);
                        shutdown(outside, SHUT_RDWR);
                        return;
                    }
                }
            }
        }

        // ffmpeg -> outside
        recvpackets = passSocket(packettmp, "ffmpeg", ffmpeg, "outside", outside);
        if (recvpackets == -1)
            break;
    }
}
