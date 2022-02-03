#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32                    // Windows 95 and above
    #include "winsock2.h"
    #include "Ws2tcpip.h"
    #include "stdint.h"         // for uint16_t
    #ifndef socklen_t
        typedef int socklen_t;
    #endif
#else                           // UNIX/Linux
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <fcntl.h>
#endif



int try_create_connection(char* ip_addr, int port);
void close_connection(int socket);
int send_message(int socket, char* message);
char* receive_message(int socket, size_t str_size, int blocking);
int resolve_host_name(char * hostname , char* ip);


#endif