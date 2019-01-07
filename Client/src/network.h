#ifndef NETWORK_H

#define NETWORK_H

int create_connection(const char* ADDRESS, int PORT);
char* create_message(int index, int desk[8][8]);
int send_message(int socket, char* );
int receive_player_id(int socket);
char* receive_message(int socket);
int parse_message(char* message, int* desk);

#endif