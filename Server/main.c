#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#define PORT 2510
#define STRING_SIZE 132

//TODO: make this thing looks nice and AT LEAST readable

//
//0 2 0 2 0 2 0 2
//2 0 2 0 2 0 2 0
//0 2 0 2 0 2 0 2
//0 0 0 0 0 0 0 0
//0 0 0 0 0 0 0 0
//0 1 0 1 0 1 0 1
//1 0 1 0 1 0 1 0
//0 1 0 1 0 1 0 1
//


char* update (char* );
char* init (int index);


int main(int argc , char *argv[])
{
    int opt = 1;
    int master_socket , addrlen , new_socket , client_socket[30] ,
            max_clients = 2 , activity, i , valread , sd;
    int max_sd;
    struct sockaddr_in address;

    char buffer[STRING_SIZE];
    fd_set readfds;


    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("[+]Listener on port %d \n", PORT);

    if (listen(master_socket, 2) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(address);
    puts("[+]Waiting for connections ...");

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
            printf("Error: error while select().");
        }

        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            printf("[+]New connection - ip: %s port: %d  \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));



            for (i = 0; i < max_clients; i++)
            {
                if( client_socket[i] == 0 )
                {
                    //when somebody connected
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);

                    //both players connected
                    if (i == 1)
                    {
                        send(client_socket[0], init(1), STRING_SIZE, 0);
                        send(client_socket[1], init(2), STRING_SIZE, 0);
                    }
                    break;
                }
            }
        }

        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET( sd , &readfds))
            {
                if ((valread =  (int) read( sd , buffer, 1024)) == 0)
                {
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    close( sd );
                    client_socket[i] = 0;
                }

                else
                {
                    if (sd == client_socket[0])
                        sd = client_socket[1];
                    else
                        sd = client_socket[0];

                    buffer[valread] = '\0';
                    char* response = update(buffer);
                    send(sd , response , strlen(buffer) , 0 );
                    free(response);
                }
            }
        }
    }
}




char* update(char* req)
{
    int checkers[2];
    char index = (char) strtol(&req[0], NULL, 10);

        puts("[+]INCOMING MESSAGE:");
        puts ("[=====================]");
        puts(req);
        printf ("current index: %d\n", index);

    checkers[0] = 0;
    checkers[1] = 0;

    for (int i = 2; req[i] != '\0'; i++)
    {
        if (index == '1')
            switch (req[i])
            {
                case ('2'): checkers[0]++;
                            break;

                case ('5'): checkers[1]++;
                            break;

                default: ;
            }
        else
            switch (req[i])
            {
                case ('1'): checkers[0]++;
                        break;

                case ('4'): checkers[1]++;
                        break;

                default: ;
            }
    }
        printf("usual checkers: %d, big checkers: %d\n",checkers[0], checkers[1]);
    if (checkers[0] == 0 && checkers[1] == 0)
        index += 5;
    else
        index == 1 ? index++ : index--;

    char* answer;
    answer = (char*) malloc (sizeof(char) * STRING_SIZE); // sizeof is optional
    answer[0] = (char) (index + '0');
    answer[1] = ' ';
    answer[2] = '\0';

    strcpy(&answer[2], &req[2]);

    puts ("[=====================]");
    puts (answer);

    return answer;
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
        puts("creating initial string");
        fgets(buffer, buffer_size, desk_file);
        puts (buffer);
        strcat(desk_string, buffer);
        puts("initial string created");
    }
    else
    {
        puts("Error while opening file");
    }

    free(buffer);
    return desk_string;
}

