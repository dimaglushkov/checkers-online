#include <SDL2/SDL.h>
#include <string.h>


//int draw_init (int connection_status);
//int draw_waiting_for_enemy ();
//int draw_gameplay(char desk[8][8], char game_status);


//creates main window and main surface
//returns 0 if everything is ok, 1 otherwise
int draw_create_mains(char* title);

//destroys main window and main surface
//return 0 if everything is ok, 1 otherwise

const int SCREEN_WIDTH = 900, SCREEN_HEIGHT = 750;
const char* GAME_TITLE = "Checkers-online(ver. 0.1)";

SDL_Window*  main_window = NULL;
SDL_Surface* main_surface = NULL;


int draw_init (int connection_status)
{

    if (main_surface == NULL)
    {
        SDL_Init(SDL_INIT_VIDEO);
        draw_create_mains(GAME_TITLE);
    }
    SDL_Surface* loading_surface = NULL;

    switch (connection_status)
    {
        case -1:
            loading_surface = SDL_LoadBMP("../img/draw_init_connecting.bmp");
            break;
        case 0:
            loading_surface = SDL_LoadBMP("../img/draw_waiting.bmp");
            break;
    }

    SDL_BlitSurface(loading_surface, NULL, main_surface, NULL);
    SDL_UpdateWindowSurface(main_window);

    return 0;

}






int draw_create_mains(char* title)
{
    main_window = SDL_CreateWindow(title,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SCREEN_WIDTH,
                                   SCREEN_HEIGHT,
                                   SDL_WINDOW_SHOWN);
    if (!main_window)
        return 1;

    main_surface = SDL_GetWindowSurface(main_window);

    if (!main_surface)
        return 1;

    return 0;
}


int draw_destroy_mains()
{
    SDL_FreeSurface( main_surface );
    SDL_DestroyWindow( main_window);
    SDL_Quit();
    return 0;
}