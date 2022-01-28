#ifndef PACKER_H
#define PACKER_H

extern const int MESSAGE_SIZE;
extern const int INITIAL_MESSAGE_SIZE;

char* update_message (char* );
int is_ended(const char* message);
int chtoi(char sym);
char itoch(int num);
#endif
