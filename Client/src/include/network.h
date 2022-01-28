#ifndef NETWORK_H

#define NETWORK_H

int create_connection(const char* ADDRESS, int PORT);
void close_connection(int socket);
int send_message(int socket, char* message);
char* receive_message(int socket, size_t str_size, int FLAG);

#endif