#include <stdlib.h>
#include <stdio.h>

#include "include/packer.h"

extern char MODE_DEBUG;
const int MESSAGE_SIZE = 66;
const int INITIAL_MESSAGE_SIZE = 2;
const int KING_CHECKER_OFFSET = 3;

int8_t count_checkers(const char* message, char opp_id)
{
    int8_t checkers = 0;

    for (int i = 1; message[i] != '\0'; i++)
        if (message[i] == opp_id || message[i] == opp_id + KING_CHECKER_OFFSET)
            checkers++;

    return checkers;
}

char* update_message(char* message)
{

    if (MODE_DEBUG)
        printf("[+] Server received message: %s\n", message);

    if (chtoi(message[0]) > 7)
    {
//      surrender
        if (MODE_DEBUG)
            printf("[+] Server sending response: %s\n", message);
        return message;
    }

    int8_t status;
    int8_t cur_id = (int8_t)chtoi(message[0]);
    int8_t opp_id = cur_id == 1 ? 2 : 1;
    int8_t opp_checkers_left = count_checkers(message, itoch(opp_id));
    if (opp_checkers_left == 0)
        status = (int8_t)(cur_id + 5);
    else
        status = opp_id;

    message[0] = itoch(status);

    if(MODE_DEBUG)
        printf("[+] Server sending response: %s\n", message);
    return message;
}

int is_ended(const char* message)
{
    return message[0] > 2 ? 1 : 0;
}

int chtoi(char sym)
{
    return (sym >= '0' && sym <= '9') ? sym - 48 : -1;
}

char itoch(int num)
{
    return (num >= 0 && num <= 9) ? (char)(num + 48) : '\0';
}
