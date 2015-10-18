#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unordered_map>
#include <string>
#include <thread>
#include <string.h>
#include <errno.h>
#include <list>
#include "StreamProcess.hpp"

#ifdef USE_IPV6
#define IPFAMILY "IPv6"
#define ADDRSTRUCT sockaddr_in6
#define ADDR_ANY_CONST in6addr_any
#define STRUCT_ADDR sin6_addr
#define STRUCT_PORT sin6_port
#define STRUCT_FAMILY sin6_family
#define FAMILY_CONST AF_INET6
#else
#define IPFAMILY "IPv4"
#define ADDRSTRUCT sockaddr_in
#define ADDR_ANY_CONST {INADDR_ANY}
#define STRUCT_ADDR sin_addr
#define STRUCT_PORT sin_port
#define STRUCT_FAMILY sin_family
#define FAMILY_CONST AF_INET
#endif

std::unordered_map<std::string, std::string> validkeyusermap;
std::list<StreamProcess*> streams;
std::list<std::thread*> threads;

int main() {
    std::cout << "Loading MediaCourier Server Node..." << std::endl;
#ifdef DEBUG
    std::cout << " Debug build! Do not run in production, ever, ever, EVER!" << std::endl;
    std::cout << "  Adding KEY-PONKAPONKAPONKAPONKAPONKAPONKAPO-K as a valid key." << std::endl;
    validkeyusermap["PONKAPONKAPONKAPONKAPONKAPONKAPO-K"] = "Pinkie Pie";
#endif
    std::cout << " Creating RTMP listener..." << std::endl;

    srand(time(nullptr));

    int rtmpsock = socket(FAMILY_CONST, SOCK_STREAM, 0);
    if (rtmpsock == -1) {
        throw std::runtime_error("Failed to create socket! " + std::string(strerror(errno)));
    }
    int whyapointertothis = 1;
    setsockopt(rtmpsock, SOL_SOCKET, SO_REUSEADDR, &whyapointertothis, sizeof(int));
    ADDRSTRUCT anyaddr;
    anyaddr.STRUCT_ADDR = ADDR_ANY_CONST;
    anyaddr.STRUCT_PORT = htons(1935);
    anyaddr.STRUCT_FAMILY = FAMILY_CONST;
#ifdef USE_IPV6
    anyaddr.sin6_flowinfo = 0;
    anyaddr.sin6_scope_id = 0;
#endif
    if (bind(rtmpsock, (sockaddr*)&anyaddr, sizeof(ADDRSTRUCT)) == -1)
        throw std::runtime_error("RTMP socket binding error! " + std::string(strerror(errno)));
    if (listen(rtmpsock, 5) == -1)
        throw std::runtime_error("RTMP socket listen error! " + std::string(strerror(errno)));

    std::cout << " RTMP is now listening on any " << IPFAMILY << " address, port 1935." << std::endl;
    std::cout << "Server node ready." << std::endl;

    bool shouldShutdown = false;
    int clisock = -1;
    while (!shouldShutdown) {
        ADDRSTRUCT* cliaddr = new ADDRSTRUCT;
        memset(cliaddr, 0, sizeof(ADDRSTRUCT));
        socklen_t cliaddrlen = sizeof(ADDRSTRUCT);
        clisock = accept(rtmpsock, (sockaddr*)cliaddr, &cliaddrlen);
        if (clisock == -1) {
            throw std::runtime_error("RTMP Socket error! " + std::string(strerror(errno)));
        }
        if (cliaddrlen != sizeof(ADDRSTRUCT)) {
            std::cout << " New connection from the unknown (address info of incorrect size) - killing connection." << std::endl;
            std::cout << " Size was " << cliaddrlen << " but " << sizeof(ADDRSTRUCT) << " was expected." << std::endl;
            shutdown(clisock, SHUT_RDWR);
            continue;
        } else {
            char prettyaddr[INET6_ADDRSTRLEN];
            inet_ntop(FAMILY_CONST, &cliaddr->STRUCT_ADDR, prettyaddr, cliaddrlen);
            std::cout << " New connection from " << prettyaddr << " on port " << ntohs(cliaddr->STRUCT_PORT) << std::endl;
        }
        threads.insert(threads.cend(), new std::thread([&]() {
            StreamProcess* proc = new StreamProcess(clisock, &streams, &validkeyusermap);
            streams.insert(streams.end(), proc);
            proc->RunStreamProcesses();
            streams.remove(proc);
            delete proc;
        }));
        /*TODO: Check if pointer in streammap is null, if so...
         *      Create StreamProcess instance with relevant config, add to streammap, run CreateProcesses on separate
         *      thread. If the pointer isn't null, close the attempted connection.
         */
    }
    return 0;
}