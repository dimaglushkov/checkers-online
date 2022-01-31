#include "include/network.h"
#include "include/packer.h"


extern char MODE_DEBUG;
const uint8_t MAX_CLIENTS = 2;

void set_mode_debug()
{
    MODE_DEBUG = 1;
}

int create_socket(int* sock, struct sockaddr_in* address, uint16_t port)
{
    #ifdef WIN32                  
    WSADATA wsaData;
    int err;
    char opt = 1;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    if ((err = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0) 
    {
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(port);

    if ((*sock = socket(address->sin_family, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) 
    {
        printf("[!] Error: socket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    if ((err = setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) != 0) {
        printf("[!] Error: setsockopt for SO_REUSEADDR failed with error: %d\n", WSAGetLastError());
        return 1;
    } 

    if ((err = bind(*sock, (struct sockaddr *) address, sizeof(*address))) == SOCKET_ERROR) {
        printf("[!] Error: bind failed with error: %d\n", WSAGetLastError());
        closesocket(*sock);
        WSACleanup();
        return 1;
    }

    if ((err = listen(*sock, MAX_CLIENTS)) == SOCKET_ERROR) {
        printf("[!] Error: listen failed with error: %d\n", WSAGetLastError());
        closesocket(*sock);
        WSACleanup();
        return 1;
    }

    #else
    int opt = 1;
    signal(SIGPIPE, SIG_IGN);

    address -> sin_family = AF_INET;
    address -> sin_addr.s_addr = INADDR_ANY;
    address -> sin_port = htons(port);

    if((*sock = socket(address -> sin_family, SOCK_STREAM, 0)) == 0)
    {
        perror("[!] Error: creating socket failed");
        return 1;
    }

    if(setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("[!] Error: setting socket options failed");
        return 1;
    }

    if(bind(*sock, (struct sockaddr *) address, sizeof(*address)) < 0)
    {
        perror("[!] Error: binding failed");
        return 1;
    }

    if(listen(*sock, MAX_CLIENTS) < 0)
    {
        perror("[!] Error: listen() failed");
        return 1;
    }

    #endif

    if (MODE_DEBUG)
        printf("[+] Listener on port %d \n[+] Waiting for connections...\n", port);
    
    return 0;
}

int accept_socket(int master_socket, struct sockaddr_in * address, socklen_t* address_len)
{
    int sock;
    if ((sock = accept(master_socket, (struct sockaddr*) address, address_len)) < 0)
    {
        perror("[!] Error: accepting socket failed");
    }
    else if (MODE_DEBUG)
        printf("[+] New connection - ip: %s port: %d.\n",
               inet_ntoa(address -> sin_addr),
               ntohs(address -> sin_port));

    return sock;
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
    if (MODE_DEBUG)
        printf("[-] Host disconnected - ip: %s port: %d.\n",
               inet_ntoa(address -> sin_addr),
               ntohs(address -> sin_port));

    #ifdef WIN32
    closesocket(cur_sd);
    #else
    close(cur_sd);
    #endif
    *client_socket = 0;
}

void start_game(int* client_socket, int second_id)
{
    //sending initial messages to start game
    char init_message[INITIAL_MESSAGE_SIZE];
    strcpy(init_message, INITIAL_MESSAGE);


    init_message[0] = '1';
    send(client_socket[second_id - 1], init_message, INITIAL_MESSAGE_SIZE, 0);

    init_message[0] = '2';
    send(client_socket[second_id], init_message, INITIAL_MESSAGE_SIZE, 0);
}

void set_fd(int* max_sd, uint8_t MAX_CLIENTS, const int* client_socket, fd_set* read_fds)
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