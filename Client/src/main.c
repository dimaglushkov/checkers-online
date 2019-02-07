#include <stdio.h>
#include <SDL2/SDL_timer.h>
#include "gui.h"
#include "gameplay.h"
#include "network.h"


const char* ADDRESS = "127.0.0.1";
const int PORT = 2510;


int main(int argc, char* args[])
{

    int status = -1, socket, player_id, opponents_id;
    char* message = NULL;
    int desk[8][8];

    draw_intro(status);

    do
    {
        SDL_Delay(500);
        socket = create_connection(ADDRESS, PORT);
    }
    while(socket < 0);

    draw_intro(0);

    //initial messages
    player_id = receive_player_id(socket);
    opponents_id = player_id == 1? 2 : 1;
    message = receive_message(socket);
    status = parse_message(message, &desk[0][0]);

    draw_gameplay_base(player_id);
    draw_desk_checkers(desk, status);


    while (1)
    {
        if (status == player_id)
        {
            draw_desk_checkers(desk, status);
            draw_deads(player_id, 12 - count_checkers_on_desk(player_id, desk));

            status = game_start(player_id, opponents_id, desk, status);

            if(status > 2)
            {
                free(message);
                message = create_message(status, desk);
                send_message(socket, message);
                break;
            }

            draw_desk_checkers(desk, status);

            free(message);
            message = create_message(player_id, desk);
            send_message(socket, message);
        }
        else
        {

            free(message);
            message = receive_message(socket);
            status = parse_message(message, &desk[0][0]);
            if (status > 2)
            {
                draw_desk_checkers(desk, player_id);
                break;
            }
        }

    }

    draw_result(status);

    free(message);
    draw_destroy();

    return 0;
}

