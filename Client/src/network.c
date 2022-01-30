#include "include/network.h"
#include "include/packer.h"

int try_create_connection(const char* ip_addr, int port)
{
    int sock;
    struct sockaddr_in server_address;
#ifdef WIN32
    WSADATA wsaData;
    u_long mode = 1;

    if (WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR){
        printf("Error at WSAStartup()\n");
        return 0;
    }
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 0;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip_addr);
    server_address.sin_port = htons(port);


    if (connect(sock, (SOCKADDR *) & server_address, sizeof (server_address))) {
        wprintf(L"connect function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 0;
    }

    if(ioctlsocket(sock, FIONBIO, &mode) != NO_ERROR) {
        WSACleanup();
        return 0;
    }
#else
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return 0;

    memset(&server_address, '\0', sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons((uint16_t)port);

    if(inet_pton(AF_INET, ip_addr, &server_address.sin_addr) <= 0)
        return 0;

    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        return 0;

    if(fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) < 0)
        return 0;
#endif
    return sock;
}

int send_message(int socket, char* message)
{
    return (int) send(socket, message, MESSAGE_SIZE, 0 );
}

char* receive_message(int socket, size_t str_size, int blocking)
{
    char* message = NULL;
    message = (char*) malloc(str_size * sizeof(char));
    size_t n = 0;
    if (blocking) {
        do {
            n = recv(socket, message, str_size, 0);
        }
        while (n <= str_size - 1 || message == NULL);
        return message;
    }
    else {
        n = recv(socket, message, str_size, 0);
        if (n < str_size - 1 || n == -1 || message == NULL) {
            free(message);
            return NULL;
        }
        return message;
    }
}

void close_connection(int socket) {
    close(socket);
}

int resolve_host_name(char * hostname , char* ip){
    struct addrinfo hints, *serv_info, *p;
    struct sockaddr_in *h;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo( hostname , "http" , &hints , &serv_info)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = serv_info; p != NULL; p = p->ai_next)
    {
        h = (struct sockaddr_in *) p->ai_addr;
        strcpy(ip , inet_ntoa( h->sin_addr ) );
    }
    freeaddrinfo(serv_info);
    return 0;
}
