#include "include/packer.h"

const int MESSAGE_SIZE = 66;
const int INITIAL_MESSAGE_SIZE = 67;

char* create_message(int index, int desk[8][8])
{
    char* message = (char*) malloc (sizeof(char) * MESSAGE_SIZE);
    message[0] = itoch(index);

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            message[i*8+j+1] = itoch(desk[i][j]);

    return message;
}

int parse_message(char* message, int desk[8][8])
{
    int index = chtoi(message[0]);

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            desk[i][j] = chtoi(message[i * 8 + j + 1]);

    return index;
}

int parse_initial_message(char* message, int* player_id, int desk[8][8])
{
    int status;
    *player_id = chtoi(message[0]);
    status = parse_message(&message[1], desk);
    return status;
}

int chtoi(char sym)
{
    return (sym >= '0' && sym <= '9') ? sym - 48 : -1;
}

char itoch(int num)
{
    return (num >= 0 && num <= 9) ? (char)(num + 48) : '\0';
}
