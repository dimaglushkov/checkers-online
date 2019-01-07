#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

#define STRING_SIZE 132


int create_connection(const char* ADDRESS, int PORT)
{
    int sock;
    struct sockaddr_in address, server_address;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        puts("[-]Error: Can't create socket.");
        return -1;
    }

    memset(&server_address, '\0', sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons((uint16_t)PORT);

    if(inet_pton(AF_INET, ADDRESS, &server_address.sin_addr)<=0)
    {
        puts("[-]Error: Invalid address.");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        puts("[-]Error: Connection failed.");
        return -1;
    }

    return sock;
}

char* create_message(int index, int desk[8][8])
{
    int k = 2;
    char* message = (char*) malloc (sizeof(char) * STRING_SIZE);

    message[0] = (char) (index + '0');
    message[1] = ' ';

    for(int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            message[k] = (char)(desk[i][j] + '0');
            message[k + 1] = ' ';
            k += 2;
        }

    message[k] = '\0';

    return message;
}

int send_message(int socket, char* message)
{
    return (int) send(socket, message, strlen(message) , 0 );
}

char* receive_message(int socket)
{
    char* message;
    message = (char*) malloc(STRING_SIZE * sizeof(char));
    read(socket, message, STRING_SIZE);
    puts("[=]Received message:");
    printf("[=]%s\n", message);
    puts("[==========]");
    return message;
}

int receive_player_id(int socket)
{
    char* message;
    message = (char*) malloc(2 * sizeof(char));
    read(socket, message, 2);
    puts("[=]Received id message:");
    printf("[=]%s\n", message);
    puts("[==========]");
    int res = message[0] - '0';
    free(message);
    return res;

}

int parse_message(char* message, int* desk)
{
    int index = (int) message[0] - '0';
    message += 2;
    int i = 0;
    while(*message)
    {
        if (*(message) != ' ')
        {
            *(desk + i) = *message - '0';
            i++;
        }
        message++;
    }
    return index;
}