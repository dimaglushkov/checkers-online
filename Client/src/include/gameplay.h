#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include <SDL.h>
#include <SDL_events.h>
#include <sys/socket.h>

extern const int PLAYER_CONCEDE_OFFSET;
extern const int CHECKER_KING_OFFSET;


typedef struct {
    int col, raw;
} pos_t;

int game_start(SDL_Window* main_window, SDL_Surface* desk_surface, SDL_Surface* checkers_surface, SDL_Rect* texture_rects, int player_id, int opponent_id, int desk[8][8], int status);
int wait_for_response(char ** message, int socket, int message_size);
int wait_for_connection(char *host_addr, int host_port);
int count_checkers_on_desk(int player_id, int desk[8][8]);

#endif