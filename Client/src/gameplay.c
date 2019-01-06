#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_events.h>
#include "gameplay.h"
#include "gui.h"

//TODO: optimize changing current checker algs

// game_start - тут вызываются почти все оставльные функции для единого контекста
// (1)game_init - все что нужно сделать при получении ответа от сервера
// (2)game_change_pos - переключение на другую шашку
// (3)int* game_get_options - определить возможные ходы, возвращает массив в положениями куда можно сходить
// (4)game_move - совершить ход
// (5)game_become_big - стать дамкой

int* game_start(int status, int desk[8][8])
{
    pos cur;
    SDL_Event event;
    int is_chosen = 0;

    cur = game_init(status, desk);


    while(SDL_WaitEvent(&event))
    {
        if (event.type == SDL_KEYDOWN  && !is_chosen)
        {
            if (event.key.keysym.sym == SDLK_UP) {
                cur = game_change_pos_up(status, desk, cur);

            }

            if (event.key.keysym.sym == SDLK_DOWN) {
                cur = game_change_pos_down(status, desk, cur);

            }

            if (event.key.keysym.sym == SDLK_LEFT) {
                cur = game_change_pos_left(status, desk, cur);

            }

            if (event.key.keysym.sym == SDLK_RIGHT) {
                cur = game_change_pos_right(status, desk, cur);

            }

            if (event.key.keysym.sym == SDLK_RETURN) {
                is_chosen = 1;
                //draw
            }
        }
        if (event.type == SDL_KEYDOWN  && is_chosen)
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
                is_chosen = 0;

            }

        }
    }


}

/*
pos* game_get_options (int status, int desk[8][8], pos cur)
{
    pos* opts = (pos*) malloc(sizeof(pos) * 4);

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

pos game_init(int status, int desk[8][8])
{
    pos current;

    current.col = 0;
    current.raw = 0;

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            if (desk[i][j] == status)
            {
                current.raw = i;
                current.col = j;
                return current;
            }

    return current;
}





