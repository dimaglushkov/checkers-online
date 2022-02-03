#include <stdio.h>
#include "include/gui.h"
#include "include/gameplay.h"
#include "include/network.h"
#include "include/packer.h"

char* get_conf_file(int argc, char *argv[]);
int   load_conf(char** host_addr, int* host_port, char* conf_file);

const char *CONNECTING_BMP = "img/connecting.bmp",
           *WAITING_BMP = "img/waiting.bmp",
           *CHECKERS_BMP = "img/checkers.bmp",
           *DESK_BMP = "img/desk.bmp";

int main(int argc, char* argv[])
{

    int socket, host_port;
    char *host_addr, *conf_file;
    char host_ip[16];

    int status = 0, player_id, opponents_id;
    char* message = NULL;
    int desk[8][8];

    SDL_Window*  main_window = NULL;
    SDL_Surface* main_surface = NULL;
    SDL_Surface* checkers_surface = NULL;
    SDL_Surface* desk_surface = NULL;
    SDL_Rect texture_rects[10];


    conf_file = get_conf_file(argc, argv);
    if (load_conf(&host_addr, &host_port, conf_file))
        return 1;

    create_window_with_surface(&main_window, &main_surface);
    checkers_surface = create_surface_from_bmp(CHECKERS_BMP);
    desk_surface = create_surface_from_bmp(DESK_BMP);
    create_texture_rects(texture_rects);

    draw_image(&main_window, CONNECTING_BMP);
    socket = wait_for_connection(host_addr, host_port);
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
    free(host_addr);
    free_window_surfaces(main_window, checkers_surface, desk_surface);
    close_connection(socket);
    return 0;
}

int load_conf(char** host_addr, int* host_port, char* conf_file)
{
    const int line_lim = 100;
    char line[line_lim], addr_buffer[line_lim];
    FILE* file;

    memset(line, '\0', line_lim);
    memset(addr_buffer, '\0', line_lim);

    if ((file = fopen(conf_file, "r")) == NULL)
        return 1;

    if(fgets(line, 512, file) == NULL)
    {
        fclose(file);  /* close the file */
        return 1;
    }
    sscanf(line, "%s %d", addr_buffer, host_port);
    *host_addr = (char*) malloc(strlen(addr_buffer) * sizeof(char));
    strcpy(*host_addr, addr_buffer);
    fclose(file);
    return 0;
}


char* get_conf_file(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-c") == 0 && argv[i + 1])
            return argv[i + 1];
    }
    return "conf/con-config.txt";
}
