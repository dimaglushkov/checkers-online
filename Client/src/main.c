#include <stdio.h>
#include <SDL2/SDL_timer.h>
#include "gui.h"
#include "network.h"


const char* ADDRESS = "127.0.0.1";
const int PORT = 2510;


int main(int argc, char* args[])
{

    int status = -1, socket;
    char* message;
    int desk[8][8];

    draw_intro(status);

    do
    {
        socket = create_connection(ADDRESS, PORT);
    }
    while(socket < 0);

    draw_intro(0);

    message = receive_message(socket);
    status = parse_message(message, &desk[0][0]);

    printf ("Received index: %d\n", status);

    SDL_Delay(5000);
    draw_destroy_mains();
    return 0;
}

