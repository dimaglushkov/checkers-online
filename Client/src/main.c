#include <stdio.h>
#include <SDL2/SDL_timer.h>
#include "ui.h"


int main(int argc, char* args[])
{
    draw_init(-1);
    SDL_Delay(2000);
    draw_init(0);
    SDL_Delay(2000);

    draw_destroy_mains();
    return 0;
}
