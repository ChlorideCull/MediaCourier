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
        puts("Loading net redirect.\n");
        sockcallptr = dlsym(RTLD_NEXT, "socket");
        bindcallptr = dlsym(RTLD_NEXT, "bind");
    }
    puts("Initialized socket as UNIX socket.");
    return ((sockcalldef)sockcallptr)(AF_UNIX, type, PF_UNIX);
}

// Socket bind prototype
int bind(int socket, const struct sockaddr *address, socklen_t address_len) {
    puts("Binding UNIX socket.");
    char* targetpath = getenv("MCNETREDIR_SOCKPATH");
    if (targetpath == NULL)
        return -1;
    struct sockaddr_un *newaddr = malloc(sizeof(struct sockaddr_un));
    memset(newaddr, 0, sizeof(struct sockaddr_un));
    newaddr->sun_family = AF_UNIX;
    strcpy(newaddr->sun_path, targetpath);
    return ((bindcalldef)bindcallptr)(socket, (struct sockaddr*)newaddr, sizeof(struct sockaddr_un));
}