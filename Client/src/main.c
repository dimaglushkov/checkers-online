#include <stdio.h>
#include "include/gui.h"
#include "include/gameplay.h"
#include "include/network.h"
#include "include/packer.h"

char* get_host_addr(int argc, char *argv[]);
int   get_host_port(int argc, char *argv[]);

const char *CONNECTING_BMP = "../img/connecting.bmp",
           *WAITING_BMP = "../img/waiting.bmp",
           *CHECKERS_BMP = "../img/checkers.bmp",
           *DESK_BMP = "../img/desk.bmp";

const int INITIAL_MESSAGE_SIZE = 2,
          MESSAGE_SIZE = 132;

int main(int argc, char* argv[])
{

    int socket, host_port;
    char* host_addr;

    int status = 0, player_id, opponents_id;
    char* message = NULL;
    int desk[8][8];

    SDL_Window*  main_window = NULL;
    SDL_Surface* main_surface = NULL;
    SDL_Surface* checkers_surface = NULL;
    SDL_Surface* desk_surface = NULL;
    SDL_Rect texture_rects[10];

    host_addr = get_host_addr(argc, argv);
    host_port = get_host_port(argc, argv);

    create_window_with_surface(&main_window, &main_surface);
    checkers_surface = create_surface_from_bmp(CHECKERS_BMP);
    desk_surface = create_surface_from_bmp(DESK_BMP);
    create_texture_rects(texture_rects);


    draw_image(&main_window, &main_surface, CONNECTING_BMP);
    socket = create_connection(host_addr, host_port);
    draw_image(&main_window, &main_surface, WAITING_BMP);

    message = receive_message(socket, INITIAL_MESSAGE_SIZE, 0);
    player_id = parse_initial_message(message);
    free(message);
    opponents_id = player_id == 1 ? 2 : 1;

    message = receive_message(socket, MESSAGE_SIZE, 0);
    status = parse_message(message, &desk[0][0]);

    draw_game_background(main_window,
            main_surface,
            checkers_surface,
            texture_rects,
            player_id);

    draw_checkers_on_desk(main_window,
            main_surface,
            desk_surface,
            checkers_surface,
            texture_rects,
            desk,
            status);


    while (1)
    {
        if (status == player_id)
        {
            draw_checkers_on_desk(
                    main_window,
                    main_surface,
                    desk_surface,
                    checkers_surface,
                    texture_rects,
                    desk,
                    status);

            draw_deads(
                    main_window,
                    main_surface,
                    checkers_surface,
                    texture_rects,
                    player_id,
                    12 - count_checkers_on_desk(player_id, desk));

            //here
            status = game_start(
                         main_window,
                         main_surface,
                         desk_surface,
                         checkers_surface,
                         texture_rects,
                         player_id,
                         opponents_id,
                         desk,
                         status);

            if(status > 2)
            {
                free(message);
                message = create_message(status, desk);
                send_message(socket, message);
                break;
            }

            draw_checkers_on_desk(main_window,
                    main_surface,
                    desk_surface,
                    checkers_surface,
                    texture_rects,
                    desk,
                    status);

            free(message);
            message = create_message(player_id, desk);
            send_message(socket, message);
        }
        else
        {

            free(message);

            // wait for your turn
            // message = receive_message(socket, MESSAGE_SIZE);
            if (wait_for_your_turn(main_window, &message, socket, MESSAGE_SIZE))
            {
                status = player_id + 7;
                message = create_message(status, desk);
                puts ("sending ending message");
                puts (message);
                send_message(socket, message);
                break;
            }
            status = parse_message(message, &desk[0][0]);
            if (status > 2)
            {
                draw_checkers_on_desk(main_window,
                        main_surface,
                        desk_surface,
                        checkers_surface,
                        texture_rects,
                        desk,
                        status);
                break;
            }
        }

    }

    draw_result(status);

    free(message);
    free_window_surfaces(
            main_window,
            main_surface,
            checkers_surface,
            desk_surface);

    return 0;
}

char* get_host_addr(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0 && argv[i + 1])
            return argv[i + 1];
    }
    return "127.0.0.1";
}

int get_host_port(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-p") == 0 && argv[i + 1])
            return atoi(argv[i + 1]);
    }
    return 2510;
}
