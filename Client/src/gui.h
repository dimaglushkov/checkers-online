#ifndef GUI_H
#define GUI_H
#include <SDL2/SDL.h>

int draw_destroy();
int draw_intro (int connection_status);
int draw_desk_checkers(int player_id, int desk[8][8], int game_status);
int draw_gameplay_base(int player_id, int status);
int draw_selected(SDL_Rect);
int draw_options(SDL_Rect*);
void draw_result(int status);
void draw_deads(int player_id, int number);

#endif