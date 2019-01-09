#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <errno.h>

#define PORT 2510
#define STRING_SIZE 132

char* update (char* );
char* init (int index);
int is_ended(const char* message);

int main(int argc , char *argv[])
{
    int opt = 1,
        master_socket,
        addrlen,
        new_socket,
        client_socket[30],
        max_clients = 2,
        activity,
        i,
        valread,
        sd,
        max_sd;

    struct sockaddr_in address;

    char buffer[STRING_SIZE];
    fd_set readfds;

    puts("[+] Starting server...");

    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("master socket");
        puts("[-] Error: can't create master socket.");
        exit(EXIT_FAILURE);
    }

    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        puts("[-] Error: setcockopt() failed.");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind");
        puts("[-] Error: bind() failed.");
        exit(EXIT_FAILURE);
    }

    printf("[+] Listener on port %d. \n", PORT);

    if (listen(master_socket, 2) < 0)
    {
        perror("listen");
        puts("[-] Error: listen() failed.");
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(address);
    puts("[+] Waiting for connections...");

    while(1)
    {
        FD_ZERO(&readfds);

        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        for ( i = 0 ; i < max_clients ; i++)
        {
            sd = client_socket[i];

            if(sd > 0)
                FD_SET( sd , &readfds);

            if(sd > max_sd)
                max_sd = sd;
        }

        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))
        {
            puts("[-] Error: select() failed.");
        }

        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                puts("[-] Error: accept() failed.");
                exit(EXIT_FAILURE);
            }

            printf("[+] New connection - ip: %s port: %d.\n",
                    inet_ntoa(address.sin_addr),
                    ntohs(address.sin_port));


            for (i = 0; i < max_clients; i++)
            {
                if( client_socket[i] == 0 )
                {
                    //when somebody connected
                    client_socket[i] = new_socket;
                    printf("[+] Adding to list of sockets as %d\n", i);

                    //both players connected
                    if (i == 1)
                    {
                        //sending id
                        send(client_socket[0], "1", 2, 0);
                        send(client_socket[1], "2", 2, 0);

                        //sending initial messages to start game
                        char * initial_message = init(1);
                        send(client_socket[0], initial_message, STRING_SIZE, 0);
                        send(client_socket[1], initial_message, STRING_SIZE, 0);
                        free(initial_message);
                    }
                    break;
                }
            }
        }

        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET(sd, &readfds))
            {
                if ((valread =  (int) read(sd, buffer, 1024)) == 0)
                {
                    getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                    printf("[-] Host disconnected - ip: %s port: %d.\n",
                            inet_ntoa(address.sin_addr),
                            ntohs(address.sin_port));

                    close(sd);
                    client_socket[i] = 0;
                }

                else
                {
                    char selected;
                    if (sd == client_socket[0])
                    {
                        sd = client_socket[1];
                        selected = 1;
                    }
                    else
                    {
                        sd = client_socket[0];
                        selected = 0;
                    }
                    buffer[valread] = '\0';
                    char* response = update(buffer);
                    send(sd, response, strlen(buffer), 0);

                    if (is_ended(response))
                    {
                        selected == 0 ? selected++ : selected--;
                        sd = client_socket[selected];
                        send(sd, response, strlen(buffer), 0);
                    }

                    free(response);
                }
            }
        }
    }
}




char* update(char* req)
{

    int checkers[2];
    int index = (char)(req[0] - '0');
    char* response;
    response = (char*) malloc (sizeof(char) * STRING_SIZE);

    if (req[0] > '2')
    {
        response[0] = req[0];
        response[1] = '\0';
        printf("[!] Server sending response: %s\n", response);
        return response;
    }

    char opponent = (char) (index == 1 ? '2' : '1');

    printf("[!] Server received message:%s\n", req);

    checkers[0] = 0;
    checkers[1] = 0;

    for (int i = 2; req[i] != '\0'; i++)
    {
        if (req[i] == opponent)
            checkers[0]++;
        else if (req[i] == opponent + 3)
            checkers[1]++;
    }

    if (checkers[0] == 0 && checkers[1] == 0)
        index += 5;
    else
        index = opponent - '0';

    response[0] = (char) (index + '0');
    response[1] = ' ';
    response[2] = '\0';

    strcpy(&response[2], &req[2]);

    printf("[!] Server sending response: %s\n", response);
    return response;
}

char* init(int index)
{
    const int buffer_size = 128;
    char* desk_string = (char*) malloc (sizeof(char) * STRING_SIZE);
    FILE * desk_file = fopen("../desk/desk.txt", "r");
    char* buffer = malloc(buffer_size);


    desk_string[0] = (char) (index + '0');
    desk_string[1] = ' ';
    desk_string[2] = '\0';


    if (desk_file)
    {
        fgets(buffer, buffer_size, desk_file);
        strcat(desk_string, buffer);
    }
    else
    {
        puts("[+] Error: can't open desk file.");
    }

    free(buffer);
    return desk_string;
}

int is_ended(const char* message)
{
    return message[0] > 2 ? 1 : 0;
}


