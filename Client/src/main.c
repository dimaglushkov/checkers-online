#include <stdio.h>
#include <SDL2/SDL_timer.h>
#include "gui.h"
#include "gameplay.h"
#include "network.h"
#include "packer.h"

char* get_host_addr(int argc, char *argv[]);
int   get_host_port(int argc, char *argv[]);

const int SDL_DELAY_ON_LOAD = 500,
          CONNECTING_PIC_ID = 0,
          WAITING_PIC_ID = 1;


int main(int argc, char* argv[])
{

    int status, socket, player_id, opponents_id;
    char* message = NULL;
    char* host_addr;
    int desk[8][8], host_port;

    host_addr = get_host_addr(argc, argv);
    host_port = get_host_port(argc, argv);

    draw_picture(CONNECTING_PIC_ID);

    do
    {
        SDL_Delay(SDL_DELAY_ON_LOAD);
        socket = create_connection(host_addr, host_port);
    }
    while(socket < 0);

    draw_picture(WAITING_PIC_ID);

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

char* get_host_addr(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-host") == 0 && argv[i + 1])
            return argv[i + 1];
    }
    return "127.0.0.1";
}

int get_host_port(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-port") == 0 && argv[i + 1])
            return atoi(argv[i + 1]);
    }
    return 2510;
}
