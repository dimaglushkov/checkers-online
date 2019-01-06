int create_connection(const char* ADDRESS, int PORT);
int create_message(int index, int desk[8][8]);
int send_message(int socket, char* );
char* receive_message(int socket);
int parse_message(char* message, int* desk);