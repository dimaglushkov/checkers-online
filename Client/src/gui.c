#include <SDL2/SDL.h>
#include <string.h>

const int SCREEN_WIDTH = 900, SCREEN_HEIGHT = 750;
const char* GAME_TITLE = "Checkers-online(ver. 0.1)";

SDL_Window*  main_window = NULL;
SDL_Surface* main_surface = NULL;
SDL_Surface* checkers_surface = NULL;
SDL_Surface* desk_surface = NULL;
SDL_Rect small_checker_white, small_checker_black, checker, big_checker, not_selected, selected, whos_turn, desk_rect;

int init_create_mains(char* title);
int init_game_elements(int status);

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


int draw_gameplay_base(int player_id, int status)
{
    SDL_Surface* base_surface;
    SDL_Rect from, to;

    init_game_elements(player_id);

    base_surface = SDL_LoadBMP("../img/draw_gameplay_base.bmp");
    if (!base_surface)
        return -1;

    SDL_Rect status_bar;

    status_bar.x = 550;
    status_bar.y = 7;
    status_bar.w = status_bar.h = 1;

    if (player_id == 1)
        SDL_BlitSurface(checkers_surface, &small_checker_white, base_surface, &status_bar);
    else
        SDL_BlitSurface(checkers_surface, &small_checker_black, base_surface, &status_bar);

    SDL_BlitSurface(base_surface, NULL, main_surface, NULL);
    SDL_UpdateWindowSurface(main_window);

    SDL_FreeSurface(base_surface);
    return 0;
}


int draw_gameplay(int player_id, int desk[8][8], char status)
{

    SDL_BlitSurface(desk_surface, NULL, main_surface, &desk_rect);
    if (status)





    SDL_UpdateWindowSurface(main_window);
    return 0;
}

int draw_destroy()
{
    SDL_FreeSurface( main_surface );
    SDL_FreeSurface(checkers_surface);
    SDL_FreeSurface(desk_surface);
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

int init_game_elements(int status)
{

    checkers_surface = SDL_LoadBMP("../img/draw_checkers.bmp");
    desk_surface = SDL_LoadBMP("../img/desk.bmp");

    int i;
    status == 1 ? (i = 0) : (i = 75) ;

    small_checker_black.x = 0; small_checker_white.x = 30;
    small_checker_black.y =    small_checker_white.y = 450;
    small_checker_black.h =    small_checker_white.h = 30;
    small_checker_black.w =    small_checker_white.w = 30;

    checker.x = 0;
    checker.y = i;
    checker.w = 75;
    checker.h = 75;

    big_checker.x = 0;
    big_checker.y = i + 150;
    big_checker.w = 75;
    big_checker.h = 75;

    not_selected.x = 0;
    not_selected.y = 300;
    not_selected.h = 75;
    not_selected.w = 75;

    selected.x = 0;
    selected.y = 375;
    selected.h = 75;
    selected.w = 75;

    whos_turn.x = 240;
    whos_turn.y = 500;
    whos_turn.h = whos_turn.w = 1;

    desk_rect.x = 150;
    desk_rect.y = 50;
    desk_rect.w = desk_rect.h = 600;
}