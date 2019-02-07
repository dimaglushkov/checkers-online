#include "gui.h"


const int SCREEN_WIDTH = 900, SCREEN_HEIGHT = 750;
const char* GAME_TITLE = "Checkers-online";

SDL_Window*  main_window = NULL;
SDL_Surface* main_surface = NULL;
SDL_Surface* checkers_surface = NULL;
SDL_Surface* desk_surface = NULL;
SDL_Rect small_checker_white, small_checker_black,
         checker_black, checker_white,
         big_checker_black, big_checker_white,
         option, selected,
         whos_turn, desk_rect;

int init_create_mains(const char* title);
int init_game_elements();

int draw_picture (int picture_id)
{

    if (main_surface == NULL)
    {
        SDL_Init(SDL_INIT_VIDEO);
        init_create_mains(GAME_TITLE);
    }

    SDL_Surface* loading_surface = NULL;

    switch (picture_id)
    {
        case 0:
            loading_surface = SDL_LoadBMP("../img/draw_init_connecting.bmp");
            break;
        case 1:
            loading_surface = SDL_LoadBMP("../img/draw_waiting.bmp");
            break;
        default:break;
    }

    SDL_BlitSurface(loading_surface, NULL, main_surface, NULL);
    SDL_UpdateWindowSurface(main_window);

    SDL_FreeSurface(loading_surface);
    return 0;

}

int draw_gameplay_base(int player_id)
{
    SDL_Surface* base_surface;

    init_game_elements();

    base_surface = SDL_LoadBMP("../img/draw_gameplay_base.bmp");
    if (!base_surface)
        return -1;

    SDL_Rect status_bar;

    status_bar.x = 380;
    status_bar.y = 663;
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

int draw_desk_checkers(int desk[8][8], int status)
{

    SDL_BlitSurface(desk_surface, NULL, main_surface, &desk_rect);
    if (status == 1)
        SDL_BlitSurface(checkers_surface, &small_checker_white, main_surface, &whos_turn);
    else
        SDL_BlitSurface(checkers_surface, &small_checker_black, main_surface, &whos_turn);

    SDL_Rect checker_on_desk;
    checker_on_desk.x = 150;
    checker_on_desk.y = 50;
    checker_on_desk.w = checker_on_desk.h = 75;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            switch (desk[i][j])
            {
                case (1):
                    SDL_BlitSurface(checkers_surface, &checker_white, main_surface, &checker_on_desk);
                    break;

                case (2):
                    SDL_BlitSurface(checkers_surface, &checker_black, main_surface, &checker_on_desk);
                    break;

                case (4):
                    SDL_BlitSurface(checkers_surface, &big_checker_white, main_surface, &checker_on_desk);
                    break;

                case (5):
                    SDL_BlitSurface(checkers_surface, &big_checker_black, main_surface, &checker_on_desk);
                    break;

                default:;
            }

            checker_on_desk.x += 75;
        }
        checker_on_desk.x = 150;
        checker_on_desk.y += 75;
    }

    SDL_UpdateWindowSurface(main_window);
    return 0;
}

int draw_selected(SDL_Rect draw_at)
{
    SDL_BlitSurface(checkers_surface, &selected, main_surface, &draw_at);
    SDL_UpdateWindowSurface(main_window);
    return 0;
}

int draw_options(int times, SDL_Rect* opt_rect)
{
    for (int i = 0; i < times; i++)
    {
        if (opt_rect[i].x == 0)
            continue;

        SDL_BlitSurface(checkers_surface, &option, main_surface, &opt_rect[i]);
    }
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

void draw_deads(int player_id, int num)
{
    SDL_Rect* selected;
    SDL_Rect draw_at;

    selected = player_id == 1 ? &small_checker_white : &small_checker_black;

    draw_at.x = player_id == 1 ? 10 : 760;
    draw_at.y = 60;
    draw_at.w = 30;
    draw_at.h = 30;

    for (int i = 0; i < num; i++)
    {
        SDL_BlitSurface(checkers_surface, selected, main_surface, &draw_at);
        draw_at.x += 50;
        if ((draw_at.x > 110 && player_id == 1) || (draw_at.x > 860 && player_id == 2))
        {
            draw_at.x =  player_id == 1 ? 10 : 760;
            draw_at.y += 50;
        }
    }
    SDL_UpdateWindowSurface(main_window);
}

void draw_result(int status)
{
    char* result = malloc(200);
    result[0] = '\0';

    switch (status)
    {

        case(6):
            strcat(result, "Белые победили!");
            break;

        case(7):
            strcat(result, "Черные победили!");
            break;

        case(8):
            strcat(result, "Черные победили!\n(Белые вышли)");
            break;

        case(9):
            strcat(result,"Белые победили!\n(Черные вышли)");
            break;

        default:
            strcat(result, "Wrong end_status received");

    }

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                            "Поздравляем!",
                            result,
                            NULL);

    free(result);
}


int init_create_mains(const char* title)
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



int init_game_elements()
{

    checkers_surface = SDL_LoadBMP("../img/draw_checkers.bmp");
    desk_surface = SDL_LoadBMP("../img/draw_desk.bmp");

    small_checker_black.x = 0;      small_checker_white.x = 30;
    small_checker_black.y = 450;    small_checker_white.y = 450;
    small_checker_black.h = 30;     small_checker_white.h = 30;
    small_checker_black.w = 30;     small_checker_white.w = 30;

    checker_black.x = 0;            checker_white.x = 0;
    checker_black.y = 75;           checker_white.y = 0;
    checker_black.w = 75;           checker_white.w = 75;
    checker_black.h = 75;           checker_white.h = 75;

    big_checker_black.x = 0;        big_checker_white.x = 0;
    big_checker_black.y = 225;      big_checker_white.y = 150;
    big_checker_black.w = 75;       big_checker_white.w = 75;
    big_checker_black.h = 75;       big_checker_white.h = 75;

    option.x = 0;             selected.x = 0;
    option.y = 375;           selected.y = 300;
    option.h = 75;            selected.h = 75;
    option.w = 75;            selected.w = 75;

    whos_turn.x = 380;
    whos_turn.y = 695;
    whos_turn.h = whos_turn.w = 1;

    desk_rect.x = 150;
    desk_rect.y = 50;
    desk_rect.w = desk_rect.h = 600;
}

void draw_rules()
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                             "Правила",
                             "Ну это шашки\nя хз че тут писать\nыыыыыыыыыыы",
                             NULL);
}