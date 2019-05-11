#include <stdlib.h>

const int STRING_SIZE = 132;

int char_to_int(char sym);

char* create_message(int index, int desk[8][8])
{
    int k = 2;
    char* message = (char*) malloc (sizeof(char) * STRING_SIZE);

    message[0] = (char) (index + '0');
    message[1] = ' ';

    for(int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            message[k] = (char)(desk[i][j] + 48);
            message[k + 1] = ' ';
            k += 2;
        }

    message[k] = '\0';

    return message;
}

int parse_message(char* message, int* desk)
{
    int index = char_to_int(message[0]);
    message += 2;
    int i = 0;
    while(*message)
    {
        if (*(message) != ' ')
        {
            *(desk + i) = char_to_int(*message);
            i++;
        }
        message++;
    }
    return index;
}

int parse_initial_message(char* message)
{
    int status;
    status = char_to_int(message[0]);
    return status;
}

int char_to_int(char sym)
{
    return (sym >= '0' && sym <= '9') ? sym - 48 : -1;
}
