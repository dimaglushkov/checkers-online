#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "packer.h"

extern char MODE_DEBUG;

void count_checker(const char* req, int8_t* checkers, char index)
{

    checkers[0] = 0;
    checkers[1] = 0;

    for (int i = 2; req[i] != '\0'; i++)
    {
        if (req[i] == index)
            checkers[0]++;
        else if (req[i] == index + 3)
            checkers[1]++;
    }
}

char* update_message(char* message)
{

    printf("[+] Server received message: %s\n", message);

    if (message[0] > '2')
    {
        printf("[+] Server sending response: %s\n", message);
        return message;
    }

    int8_t checkers[2];
    int8_t index = (int8_t)(message[0] - '0');

    char opponent = (char) (index == 1 ? '2' : '1');
    count_checker(message, checkers, opponent);

    if (checkers[0] == 0 && checkers[1] == 0)
        index += 5;
    else
        index = (int8_t) (opponent - '0');

    message[0] = (char) (index + '0');

    printf("[+] Server sending response: %s\n", message);
    return message;

}

char* init_message()
{

    const int buffer_size = 128;
    char* desk_string = (char*) malloc (sizeof(char) * STRING_SIZE);
    FILE * desk_file = fopen("../desk/desk.txt", "r");
    char* buffer = malloc(buffer_size);


    desk_string[0] = (char) ('1');
    desk_string[1] = ' ';
    desk_string[2] = '\0';


    if (desk_file)
    {
        if (fgets(buffer, buffer_size, desk_file) == NULL)
            perror("[!] Error: fgets returned NULL");
        strcat(desk_string, buffer);
    }
    else
        perror("[!] Error: can't open desk file");

    free(buffer);
    return desk_string;
}

int is_ended(const char* message)
{
    return message[0] > 2 ? 1 : 0;
}

