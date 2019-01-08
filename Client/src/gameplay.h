#ifndef GAMEPLAY_H
#define GAMEPLAY_H

typedef struct {
    int col, raw;
} pos_t;

int game_start(int player_id, int desk[8][8], int status);

#endif