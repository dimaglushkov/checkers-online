#ifndef PACKER_H
#define PACKER_H

#include <stdlib.h>

extern const int MESSAGE_SIZE;
extern const int INITIAL_MESSAGE_SIZE;

char itoch(int num);
int chtoi(char sym);
char* create_message(int index, int desk[8][8]);
int parse_message(char* message, int desk[8][8]);
int parse_initial_message(char* message, int* player_id, int desk[8][8]);

#endif