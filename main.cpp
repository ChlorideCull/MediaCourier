#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unordered_map>
#include <string>

std::unordered_map<std::string, void*> streammap;

int main() {
    std::cout << "Loading MediaCourier Server Node..." << std::endl;
    std::cout << " Creating RTMP listener..." << std::endl;

    int rtmpsock = socket(AF_INET6, SOCK_STREAM, PF_INET6);
    sockaddr_in6 anyaddr;
    anyaddr.sin6_addr = in6addr_any;
    anyaddr.sin6_port = htons(1935);
    anyaddr.sin6_family = AF_INET6;
    anyaddr.sin6_flowinfo = 0;
    anyaddr.sin6_scope_id = 0;
    bind(rtmpsock, (sockaddr*)&anyaddr, sizeof(sockaddr_in6));

    std::cout << " RTMP is now listening on any IPv6 address, port 1935." << std::endl;
    std::cout << "Server node ready." << std::endl;

    bool shutdown = false;
    int clisock = -1;
    while (!shutdown) {
        sockaddr_in6 cliaddr;
        socklen_t cliaddrlen = 0;
        clisock = accept(rtmpsock, (sockaddr*)&cliaddr, &cliaddrlen);
        if (cliaddrlen != sizeof(sockaddr_in6)) {
            std::cout << " New connection from the unknown (address info of incorrect size)" << std::endl;
        } else {
            char prettyaddr[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &cliaddr, prettyaddr, cliaddrlen);
            std::cout << " New connection from " << prettyaddr << " on port " << ntohs(cliaddr.sin6_port) << std::endl;
        }
        /*TODO: Check if pointer in streammap is null, if so...
         *      Create StreamProcess instance with relevant config, add to streammap, run CreateProcesses on separate
         *      thread. If the pointer isn't null, close the attempted connection.
         */
    }
    return 0;
}