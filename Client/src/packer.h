#ifndef PACKER_H
#define PACKER_H

char* create_message(int index, int desk[8][8]);
int parse_message(char* message, int* desk);
int parse_initial_message(char* message);


#endif