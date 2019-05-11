#ifndef GAMEPLAY_H
#define GAMEPLAY_H


typedef struct {
    int col, raw;
} pos_t;

int game_start(SDL_Window* main_window, SDL_Surface* desk_surface, SDL_Surface* checkers_surface, SDL_Rect* texture_rects, int player_id, int opponent_id, int desk[8][8], int status);
int wait_for_your_turn(char ** message, int socket, int MESSAGE_SIZE);
int count_checkers_on_desk(int player_id, int desk[8][8]);

#endif