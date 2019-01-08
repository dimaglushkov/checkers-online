#ifndef GAMEPLAY_H
#define GAMEPLAY_H

typedef struct {
    int col, raw;
} pos_t;

int game_start(int player_id, int opponent_id, int desk[8][8], int status);
int count_checkers_on_desk(int player_id, int desk[8][8]);

#endif