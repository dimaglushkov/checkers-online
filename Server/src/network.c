#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <errno.h>

#include "packer.h"

int create_socket (int domain, int type, int protocol)
{
    int sockfd;

    if( (sockfd = socket(domain, type, protocol)) == 0)
    {
        perror("[!] Error: creating socket failed");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

int accept_socket(int master_socket, struct sockaddr_in * address, socklen_t* address_len)
{
    int sock;
    if ((sock = accept(master_socket, (struct sockaddr*) address, address_len)) < 0)
    {
        perror("[!] Error: accepting socket failed");
    }
    else
        printf("[+] New connection - ip: %s port: %d.\n",
                inet_ntoa(address -> sin_addr),
                ntohs(address -> sin_port));

    return sock;
}

void init_address(struct sockaddr_in * address, uint16_t PORT)
{
    address -> sin_family = AF_INET;
    address -> sin_addr.s_addr = INADDR_ANY;
    address -> sin_port = htons( PORT );
}

void init_socket(int sockfd, struct sockaddr_in* address, uint16_t PORT)
{

    int opt = 1;


    if( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0 )
    {
        perror("[!] Error: setting socket options failed");
        exit(EXIT_FAILURE);
    }

    if (bind(sockfd, (struct sockaddr *) address, sizeof(*address))<0)
    {
        perror("[!] Error: binding failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 2) < 0)
    {
        perror("[!] Error: listen() failed");
        exit(EXIT_FAILURE);
    }

    printf("[+] Listener on port %d \n", PORT);
    puts("[+] Waiting for connections...");
}

int select_fds(int max_sd, fd_set* readfds)
{

    int activity;
    activity = select(max_sd + 1, readfds, NULL, NULL, NULL);

    if ((activity < 0) && (errno!=EINTR))
    {
        perror("[!] Error: select() failed");
    }

    return activity;
}

void clear_socket(int cur_sd, int* client_socket, struct sockaddr_in * address,  socklen_t* address_len)
{
    getpeername(cur_sd, (struct sockaddr*) address, address_len);

    printf("[-] Host disconnected - ip: %s port: %d.\n",
           inet_ntoa(address -> sin_addr),
           ntohs(address -> sin_port));

    close(cur_sd);
    *client_socket = 0;
}

void start_game(int* client_socket, int second_id)
{
    //sending ids
    send(client_socket[second_id - 1], "1", 2, 0);
    send(client_socket[second_id], "2", 2, 0);

    //sending initial messages to start game
    char * initial_message = init_message();
    send(client_socket[second_id - 1], initial_message, STRING_SIZE, 0);
    send(client_socket[second_id], initial_message, STRING_SIZE, 0);
    free(initial_message);
}

void set_fd(int* max_sd, unsigned int MAX_CLIENTS, const int* client_socket, fd_set* read_fds)
{

    int cur_sd;

    for (uint8_t i = 0 ; i < MAX_CLIENTS ; i++)
    {
        cur_sd = client_socket[i];

        if(cur_sd > 0)
            FD_SET(cur_sd, read_fds);

        if(cur_sd > *max_sd)
            *max_sd = cur_sd;
    }
}