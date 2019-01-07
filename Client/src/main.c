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

    //initial messages
    player_id = receive_player_id(socket);
    message = receive_message(socket);
    status = parse_message(message, &desk[0][0]);

    draw_gameplay_base(player_id, status);
    draw_desk_checkers(player_id, desk, status);

    game_start(player_id, desk);


    //SDL_Delay(2000);


    while (1)
    {
        message = receive_message(socket);
        status = parse_message(message, &desk[0][0]);

        if (status > 2)
            break;

        draw_desk_checkers(player_id, desk, status);

        game_start(player_id, desk);
        status == 1 ? status++ : status--;

        draw_desk_checkers(player_id, desk, status);

        message = create_message(player_id, desk);
        send_message(socket, message);

    }

    //отрисовка окна в зависимости от статуса
    puts("game ended");
    SDL_Delay(3000);


    free(message);
    draw_destroy();
    return 0;
}

