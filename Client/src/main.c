#include <stdio.h>
#include <SDL2/SDL_timer.h>
#include "ui.h"


int main(int argc, char* args[])
{
    FILE* file_desk = fopen("../desk/desk.txt", "r");
    char desk[8][8];
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        fscanf(file_desk, "%d", &desk[i][j]);

    /*draw_intro(-1);
    SDL_Delay(2000);
    draw_intro(0);
    SDL_Delay(2000);
*/
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
            printf("%d ", desk[i][j]);
        puts("");
    }


    draw_destroy_mains();
    return 0;
}

