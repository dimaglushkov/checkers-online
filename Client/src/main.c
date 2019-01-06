#include <stdio.h>
#include <SDL2/SDL_timer.h>
#include "gui.h"
#include "network.h"
#include "gameplay.h"


const char* ADDRESS = "127.0.0.1";
const int PORT = 2510;


int main(int argc, char* args[])
{

    int status = -1, socket, player_id;
    char* message;
    int desk[8][8];

    draw_intro(status);

    do
    {
        socket = create_connection(ADDRESS, PORT);
    }
    while(socket < 0);

    draw_intro(0);

    player_id = receive_player_id(socket);

    message = receive_message(socket);
    status = parse_message(message, &desk[0][0]);

    draw_gameplay_base(player_id, status);
    draw_desk_checkers(player_id, desk, status);

    SDL_Delay(10000);
    free(message);
    draw_destroy();
    return 0;
}

