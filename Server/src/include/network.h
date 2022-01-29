#ifndef SERVER_NETWORK_H
#define SERVER_NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#ifdef WIN32                    // Windows 95 and above
    #include "winsock2.h"
    #include "Ws2tcpip.h"

    #ifndef socklen_t
        typedef int socklen_t;
    #endif
#else                           // UNIX/Linux
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/types.h>
    #include <errno.h>
#endif

int create_socket(int, int, int);
int accept_socket(int master_socket, struct sockaddr_in * address, socklen_t* address_len);
void init_socket(int sockfd, struct sockaddr_in* address, uint16_t PORT);
void init_address(struct sockaddr_in * address, uint16_t PORT);
int select_fds(int max_sd, fd_set* readfds);
void clear_socket(int cur_sd, int* client_socket, struct sockaddr_in * address,  socklen_t* address_len);
void start_game(int* client_socket, int second_id);
void set_fd(int* max_sd, uint8_t MAX_CLIENTS, const int* client_socket, fd_set* read_fds);
void set_mode_debug();
#endif
