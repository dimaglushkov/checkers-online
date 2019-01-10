#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>


#include "packer.h"
#include "network.h"

#define PORT 2510
#define MAX_CLIENTS 2

int main(int argc , char *argv[])
{
    int master_socket, client_socket[MAX_CLIENTS],
        cur_sd, max_sd;
    char *message;

    socklen_t address_len;
    struct sockaddr_in address;
    fd_set read_fds;


    for (uint8_t i = 0; i < MAX_CLIENTS; i++)
        client_socket[i] = 0;
    master_socket = create_socket(AF_INET , SOCK_STREAM , 0);
    init_address(&address, PORT);
    init_socket(master_socket, &address, PORT);
    address_len = sizeof(address);

    while(1)
    {

        FD_ZERO(&read_fds);
        FD_SET(master_socket, &read_fds);
        max_sd = master_socket;
        set_fd(&max_sd, MAX_CLIENTS, client_socket, &read_fds);
        select_fds(max_sd, &read_fds);

        if (FD_ISSET(master_socket, &read_fds))
        {

            if ((cur_sd = accept_socket(master_socket, &address, &address_len)) < 0)
                return 0;

            for (uint8_t i = 0; i < MAX_CLIENTS; i++)
            {
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = cur_sd;
                    printf("[+] Adding to list of sockets as %d\n", i);

                    if (i % 2 == 1)
                        start_game(client_socket, i);

                    break;
                }
            }
        }

        for (uint8_t i = 0; i < MAX_CLIENTS; i++)
        {
            cur_sd = client_socket[i];

            if (FD_ISSET(cur_sd, &read_fds))
            {
                message = malloc(STRING_SIZE);

                if (recv(cur_sd, message, STRING_SIZE, 0) == 0)
                    clear_socket(cur_sd, &client_socket[i], &address, &address_len);
                else
                {
                    int16_t to_send;

                    to_send = (int16_t)(i % 2 == 0? 1 : 0);
                    cur_sd = client_socket[to_send];

                    message[STRING_SIZE - 1] = '\0';
                    message = update_message(message);
                    send(cur_sd, message, strlen(message), 0);

                    if (is_ended(message))
                    {
                        to_send == 0 ? to_send++ : to_send--;
                        cur_sd = client_socket[to_send];
                        send(cur_sd, message, strlen(message), 0);
                    }
                }
                
                free(message);
            }
        }
    }
}


