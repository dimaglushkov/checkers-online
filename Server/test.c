#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct{
	int col, raw;
} pos;

pos game_change_pos_right(int status, int desk[8][8], pos current);
pos game_change_pos_left(int status, int desk[8][8], pos current);
pos game_change_pos_up(int status, int desk[8][8], pos current);
pos game_change_pos_down(int status, int desk[8][8], pos current);

int main ()
{	
	FILE * file = fopen("desk/desk.txt", "r");
	int desk[8][8];
	
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			fscanf(file, "%d", &desk[i][j]);
	
	
	desk[0][1] = 1;
	desk[2][3] = 1;
	desk[1][6] = 1;
	desk[3][2] = 1;
	desk[5][5] = 0;
	desk[5][7] = 0;
	
	puts ("Matrix");
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
			printf ("%d ", desk[i][j]);
		puts("");
	}
	
	int status = 1;
	
	
	pos current; 
	current.raw = 0; 
	current.col = 1;
	
	
	
	while(1)
	{
		char c;
		scanf("%c", &c);
		switch(c)
		{
			case('d'): current = game_change_pos_right(status, desk, current);
					   break;
					  
			case('a'): current = game_change_pos_left(status, desk, current);
					   break;
					   
			case('w'): current = game_change_pos_up(status, desk, current);
					   break;
					   
			case('s'): current = game_change_pos_down(status, desk, current);
					   break;
		}
		printf("raw = %d  col = %d\n", current.raw, current.col);
	}

}

//TODO: create an enum
// 1 eq UP
// 2 eq RIGHT
// 3 eq DOWN
// 4 eq LEFT
//pos game_change_pos(int status, int desk[8][8], pos current, int direction)
//{
//	for 

//}

/*
pos game_change_pos_right(int status, int desk[8][8], pos cur)
{
	for (int i = 0; i < 8; i++)
	{
		if (cur.raw - i > -1)
			for (int j = cur.col + 1; j < 8; j++)
				if (desk[i][j] == status)
				{
					pos new_pos;
					new_pos.raw = i;
					new_pos.col = j;
					return new_pos;
				}
		if (cur.raw + i < 8)
			for (int j = cur.col + 1; j < 8; j++)
				if (desk[i][j] == status)
				{
					pos new_pos;
					new_pos.raw = i;
					new_pos.col = j;
					return new_pos;
				}
	}
	return cur;
}*/


pos game_change_pos_right(int status, int desk[8][8], pos current)
{
	for (int i = current.raw; i < 8; i++)
		for (int j = current.col + 1; j < 8; j++)
			if (desk[i][j] == status)
			{
				pos new_pos;
				new_pos.raw = i;
				new_pos.col = j;
				return new_pos;
			}
	for (int i = current.raw - 1; i > -1; i--)
		for (int j = current.col + 1; j < 8; j++)
			if (desk[i][j] == status)
			{
				pos new_pos;
				new_pos.raw = i;
				new_pos.col = j;
				return new_pos;
			}
	return current;
}


pos game_change_pos_left(int status, int desk[8][8], pos current)
{
	for (int i = current.raw; i < 8; i++)
		for (int j = current.col - 1; j > -1; j--)
			if (desk[i][j] == status)
			{
				pos new_pos;
				new_pos.raw = i;
				new_pos.col = j;
				return new_pos;
			}
	for (int i = current.raw - 1; i > -1; i--)
		for (int j = current.col - 1; j > -1; j--)
			if (desk[i][j] == status)
			{
				pos new_pos;
				new_pos.raw = i;
				new_pos.col = j;
				return new_pos;
			}
	return current;
}


pos game_change_pos_up(int status, int desk[8][8], pos current)
{
	for (int i = current.raw - 1; i > -1; i--)
		for (int j = current.col; j < 8; j++)
			if (desk[i][j] == status)
			{
				pos new_pos;
				new_pos.raw = i;
				new_pos.col = j;
				return new_pos;
			}	
	for (int i = current.raw - 1; i > -1; i--)
		for (int j = current.col - 1; j > -1; j--)
			if (desk[i][j] == status)
			{
				pos new_pos;
				new_pos.raw = i;
				new_pos.col = j;
				return new_pos;
			}
	return current;
}


pos game_change_pos_down(int status, int desk[8][8], pos current)
{
	for (int i = current.raw + 1; i < 8; i++)
		for (int j = current.col; j < 8; j++)
			if (desk[i][j] == status)
			{
				pos new_pos;
				new_pos.raw = i;
				new_pos.col = j;
				return new_pos;
			}
	for (int i = current.raw + 1; i < 8; i++)
		for (int j = current.col - 1; j > -1; j--)
			if (desk[i][j] == status)
			{
				pos new_pos;
				new_pos.raw = i;
				new_pos.col = j;
				return new_pos;
			}
	return current;
}










