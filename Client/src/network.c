#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "include/network.h"
#include "include/packer.h"

const int SLEEP_TIME = 1;

int create_connection(const char* ADDRESS, int PORT)
{
    int sock;
    struct sockaddr_in server_address;

    do
    {
        sleep(SLEEP_TIME);
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("Creating socket");
            continue;
        }

        memset(&server_address, '\0', sizeof(server_address));

        server_address.sin_family = AF_INET;
        server_address.sin_port = htons((uint16_t)PORT);

        if(inet_pton(AF_INET, ADDRESS, &server_address.sin_addr)<=0)
        {
            perror("Converting addresses");
            continue;
        }

        if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        {
            perror("Connecting");
            continue;
        }

        break;
    }
    while (1);

    return sock;
}

int send_message(int socket, char* message)
{
    return (int) send(socket, message, MESSAGE_SIZE, 0 );
}

char* receive_message(int socket, size_t str_size, int FLAG)
{
    char* message = NULL;
    message = (char*) malloc(str_size * sizeof(char));
    int n = recv(socket, message, str_size, FLAG);
    if (n < str_size - 1 || n == -1 || message == NULL)
    {
        free(message);
        return NULL;
    }
    return message;
}

void close_connection(int socket) {
    shutdown(socket, SHUT_WR);
    close(socket);
}

