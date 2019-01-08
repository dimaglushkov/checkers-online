#include <stdio.h>
#include <SDL2/SDL_timer.h>
#include "gui.h"
#include "gameplay.h"
#include "network.h"




//TODO: переделать архитектуру
//      Добавить возможность остановить игру

//TODO: убрать баг с выбором клик выше
//TODO: убрать ход назад
//TODO: добавить отрисовку по бокам
//TODO: сделать кнопку выхода
//TODO: добавить возможность стать дамкой


const char* ADDRESS = "127.0.0.1";
const int PORT = 2510;


int main(int argc, char* args[])
{

    int status = -1, socket, player_id, end_status;
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
    message = receive_message(socket);
    status = parse_message(message, &desk[0][0]);

    draw_gameplay_base(player_id, status);
    draw_desk_checkers(player_id, desk, status);


    while (1)
    {



        if (status == player_id)
        {

            draw_desk_checkers(player_id, desk, status);

            if ((end_status = game_start(player_id, desk, status)) > 2)
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

    switch (end_status)
    {

        case(6):
            puts("Whites won!");
            break;

        case(7):
            puts("Blacks won!");
            break;

        case(8):
            puts("Whites won coz blacks left");
            break;

        case(9):
            puts("Whites won coz blacks left");
            break;

        default:
            puts("Wrong end_status received");

    }

    free(message);
    draw_destroy();

    return 0;
}

