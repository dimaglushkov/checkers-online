#include <SDL2/SDL.h>
#include <string.h>


//int draw_gameplay(char desk[8][8], char game_status);


const int SCREEN_WIDTH = 900, SCREEN_HEIGHT = 750;
const char* GAME_TITLE = "Checkers-online(ver. 0.1)";


SDL_Window*  main_window = NULL;
SDL_Surface* main_surface = NULL;


int init_create_mains(char* title);

//Draws image that pops while both players loading
int draw_intro (int connection_status)
{

    if (main_surface == NULL)
    {
        SDL_Init(SDL_INIT_VIDEO);
        init_create_mains(GAME_TITLE);
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

    SDL_FreeSurface(loading_surface);
    return 0;

}

//Draws desk and other adjective info
int draw_gameplay(char desk[8][8], char game_status)
{
    //TODO: take base drawing to another func and call it only once

    SDL_Surface* base_gameplay_surface = NULL;
    SDL_Surface* checkers_surface = NULL;
    SDL_Surface* desk_surface = NULL;
    SDL_Surface* menu_surface = NULL;
    SDL_Surface* small_checkers_surface = NULL;

    base_gameplay_surface = SDL_LoadBMP("../img/draw_gameplay_base");
    checkers_surface = SDL_LoadBMP("../img/draw_checkers.bmp");
    desk_surface = SDL_LoadBMP("../img/desk.bmp");

    SDL_Rect white_checker, black_checker, current_pos, optional_pos, white_big, black_big;
    white_checker.x = 0;

    









    SDL_BlitSurface(base_gameplay_surface, NULL, main_surface, NULL);




    SDL_UpdateWindowSurface(main_window);

    return 0;
}


int draw_destroy_mains()
{
    SDL_FreeSurface( main_surface );
    SDL_DestroyWindow( main_window);
    SDL_Quit();
    return 0;
}




int init_create_mains(char* title)
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