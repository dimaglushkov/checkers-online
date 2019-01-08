#include <stdio.h>
#include <SDL2/SDL_timer.h>
#include "gui.h"
#include "gameplay.h"
#include "network.h"




//TODO: переделать архитектуру
//      Добавить возможность остановить игру

//TODO: убрать ход назад
//TODO: добавить отрисовку по бокам
//TODO: сделать кнопку выхода
//TODO: добавить возможность стать дамкой


const char* ADDRESS = "127.0.0.1";
const int PORT = 2510;


int main(int argc, char* args[])
{

    int status = -1, socket, player_id, opponents_id;
    char* message;
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

    draw_gameplay_base(player_id, status);
    draw_desk_checkers(player_id, desk, status);


    while (1)
    {


        if (status == player_id)
        {

            draw_desk_checkers(player_id, desk, status);
            draw_deads(player_id, 12 - count_checkers_on_desk(player_id, desk));

            if (game_start(player_id, opponents_id, desk, status) > 2)
                break;
            status == 1 ? status++ : status--;

            draw_desk_checkers(player_id, desk, status);


            if (message != NULL)
                free(message);
            message = create_message(player_id, desk);
            send_message(socket, message);
        }
        else
        {
            if (message != NULL)
                free(message);
            message = receive_message(socket);
            status = parse_message(message, &desk[0][0]);
            if (status > 2)
                break;
        }

    }

    //отрисовка окна в зависимости от статуса
    draw_result(status);

    free(message);
    draw_destroy();

    return 0;
}

