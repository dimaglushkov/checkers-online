#ifndef GUI_H
#define GUI_H

int draw_destroy();
int draw_intro (int connection_status);
int draw_gameplay(int player_id, int desk[8][8], char game_status);
int draw_gameplay_base(int player_id, int status);

#endif