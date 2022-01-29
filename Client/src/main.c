#include <stdio.h>
#include "include/gui.h"
#include "include/gameplay.h"
#include "include/network.h"
#include "include/packer.h"

char* get_host_addr(int argc, char *argv[]);
int   get_host_port(int argc, char *argv[]);

const char *CONNECTING_BMP = "img/connecting.bmp",
           *WAITING_BMP = "img/waiting.bmp",
           *CHECKERS_BMP = "img/checkers.bmp",
           *DESK_BMP = "img/desk.bmp";

int main(int argc, char* argv[])
{

    int socket, host_port;
    char* host_addr;
    char host_ip[16];

    int status = 0, player_id, opponents_id;
    char* message = NULL;
    int desk[8][8];

    SDL_Window*  main_window = NULL;
    SDL_Surface* main_surface = NULL;
    SDL_Surface* checkers_surface = NULL;
    SDL_Surface* desk_surface = NULL;
    SDL_Rect texture_rects[10];

    host_addr = get_host_addr(argc, argv);
    if (resolve_host_name(host_addr, host_ip)){
        return 1;
    }
    host_port = get_host_port(argc, argv);

    create_window_with_surface(&main_window, &main_surface);
    checkers_surface = create_surface_from_bmp(CHECKERS_BMP);
    desk_surface = create_surface_from_bmp(DESK_BMP);
    create_texture_rects(texture_rects);

    draw_image(&main_window, CONNECTING_BMP);
    socket = wait_for_connection(host_ip, host_port);
    if (socket == 0) {
        free_window_surfaces(main_window, checkers_surface, desk_surface);
        return 0;
    }

    draw_image(&main_window, WAITING_BMP);

    if (wait_for_response(&message, socket, INITIAL_MESSAGE_SIZE)) {
        free_window_surfaces(main_window, checkers_surface, desk_surface);
        close_connection(socket);
        return 0;
    }

    status = parse_initial_message(message, &player_id, desk);
    opponents_id = player_id == 1 ? 2 : 1;

    draw_game_background(main_window, checkers_surface, texture_rects, player_id);
    draw_checkers_on_desk(main_window, desk_surface, checkers_surface, texture_rects, desk, status);


    while (1)
    {
        if (status == player_id)
        {
            draw_checkers_on_desk(main_window, desk_surface, checkers_surface, texture_rects, desk, status);

            draw_deads(main_window, checkers_surface, texture_rects, player_id, 12 - count_checkers_on_desk(player_id, desk));

            status = game_start(main_window, desk_surface, checkers_surface, texture_rects, player_id, opponents_id, desk, status);

            if(status > 2)
            {
                free(message);
                message = create_message(status, desk);
                send_message(socket, message);
                break;
            }

            draw_checkers_on_desk(main_window, desk_surface, checkers_surface, texture_rects, desk, status);

            free(message);
            message = create_message(player_id, desk);
            send_message(socket, message);
        }
        else
        {
            char * new_msg;
            if (wait_for_response(&new_msg, socket, MESSAGE_SIZE))
            {
                status = player_id + PLAYER_CONCEDE_OFFSET;
                free(message);
                message = create_message(status, desk);
                send_message(socket, message);
                break;
            }
            status = parse_message(new_msg, desk);
            free(new_msg);
            if (status > 2)
            {
                draw_checkers_on_desk(main_window, desk_surface, checkers_surface, texture_rects, desk, status);
                break;
            }
        }

    }

    draw_result(status);

    free(message);
    free_window_surfaces(main_window, checkers_surface, desk_surface);
    close_connection(socket);
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
