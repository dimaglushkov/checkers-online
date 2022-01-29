#include "include/network.h"
#include "include/packer.h"

int try_create_connection(const char* ADDRESS, int PORT)
{
    int sock;
    struct sockaddr_in server_address;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return 0;

    memset(&server_address, '\0', sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons((uint16_t)PORT);

    if(inet_pton(AF_INET, ADDRESS, &server_address.sin_addr)<=0)
        return 0;

    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        return 0;


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

int resolve_host_name(char * hostname , char* ip){
    struct hostent *he;
    struct in_addr **addr_list;
    int i = 0;

    if ((he = gethostbyname(hostname )) == NULL)
    {
        herror("gethostbyname");
        return 1;
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    if (addr_list[i] != NULL){
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }

    return 1;
}
