#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define address "asd"
#define STRING_SIZE 132

char* create_message(int index, int* desk);
int parse_message(char* message, int* desk);


int main ()
{
	FILE * file = fopen("desk/desk.txt", "r");
	int desk[8][8];
	for (int i = 0; i < 8; i++)
		for(int j =0; j < 8; j++)
			fscanf(file, "%d", &desk[i][j]);
	
	puts("Матрица:");
	printf("1 ");
	for (int i = 0; i < 8; i++)
	{
		for (int j =0; j < 8; j++)
			printf("%d ", desk[i][j]);
	}
	
	puts("");
	puts("Строка");
	
	char* result;
	result = create_message(1, &desk[0][0]);
	puts(result);
	
	//result stores message
	
	int n_index;
	int n_desk[8][8];
	
	n_index = parse_message(result, &n_desk[0][0]);
	
	puts("Новая матрица:");
	printf("%d ", n_index);
	for (int i = 0; i < 8; i++)
	{
		for (int j =0; j < 8; j++)
			printf("%d ", n_desk[i][j]);
	}
	
	free(result);
	return 0;
}

char* create_message(int index, int* desk)
{
	int k = 2;
    char* message = (char*) malloc (sizeof(char) * STRING_SIZE);

    message[0] = (char) (index + '0');
    message[1] = ' ';
	
	for(int i = 0; i < 64; i++)
	{
		message[k] = *(desk + i) + '0';
		message[k + 1] = ' ';
		k += 2;
	}
	
    message[k] = '\0';
	
    return message;
}


int parse_message(char* message, int* desk)
{
    int index = (int) message[0] - '0';
    message += 2;
	int i = 0;
    while(*message)
    {
        if (*(message) != ' ')
		{
            *(desk + i) = *message - '0';
			i++;
		}
		message++;
    }
	return index;
}


