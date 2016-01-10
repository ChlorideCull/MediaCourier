//
// Created by chloridecull on 9/23/15.
//
#define _GNU_SOURCE
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

typedef int (*sockcalldef)(int domain, int type, int protocol);
typedef int (*bindcalldef)(int socket, const struct sockaddr *address, socklen_t address_len);

void* sockcallptr;
void* bindcallptr;

// Socket construction prototype
int socket(int domain, int type, int protocol) {
    if (sockcallptr == NULL) {
        fputs("Loading net redirect.\n", stderr);
        sockcallptr = dlsym(RTLD_NEXT, "socket");
        bindcallptr = dlsym(RTLD_NEXT, "bind");
    }
    fprintf(stderr, "Initialized socket as UNIX socket (app wanted %i %i %i, created %i %i %i)\n",
            domain, type, protocol,
            AF_UNIX, SOCK_STREAM, 0);
    return ((sockcalldef)sockcallptr)(AF_UNIX, SOCK_STREAM, 0);
}

// Socket bind prototype
int bind(int socket, const struct sockaddr *address, socklen_t address_len) {
    char* targetpath = getenv("MCNETREDIR_SOCKPATH");
    if (targetpath == NULL)
        return -1;
    fprintf(stderr, "Binding UNIX socket on %s\n", targetpath);
    struct sockaddr_un *newaddr = malloc(sizeof(struct sockaddr_un));
    memset(newaddr, 0, sizeof(struct sockaddr_un));
    newaddr->sun_family = AF_UNIX;
    strcpy(newaddr->sun_path, targetpath);
    return ((bindcalldef)bindcallptr)(socket, (struct sockaddr*)newaddr, sizeof(struct sockaddr_un));
}