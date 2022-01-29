#include "include/gui.h"

static const int SCREEN_WIDTH = 900, SCREEN_HEIGHT = 750,
        SMALL_WHITE_ID = 0,
        SMALL_BLACK_ID = 3,
        WHITE_ID = 1,
        BLACK_ID = 2,
        KING_WHITE_ID = 4,
        KING_BLACK_ID = 5,
        OPTION_ID = 6,
        SELECTED_ID = 7,
        CURRENT_TURN_ID = 8,
        DESK_ID = 9;
static const char* GAME_TITLE = "Checkers-online";


SDL_Rect create_rect(int x, int y, int w, int h);

int create_window_with_surface(SDL_Window** window, SDL_Surface** surface)
{
    *window = SDL_CreateWindow(GAME_TITLE,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SCREEN_WIDTH,
                                   SCREEN_HEIGHT,
                                   SDL_WINDOW_SHOWN);
    if (!*window)
        return 1;

    *surface = SDL_GetWindowSurface(*window);
    if (!*surface)
        return 1;

    return 0;
}

SDL_Surface* create_surface_from_bmp(const char* path)
{
    SDL_Surface* surface;
    surface = SDL_LoadBMP(path);
    return surface;
}

int draw_image (SDL_Window** main_window, const char* path)
{
    SDL_Surface* main_surface = SDL_GetWindowSurface(*main_window);
    SDL_Surface* loading_surface = SDL_LoadBMP(path);

    SDL_BlitSurface(loading_surface, NULL, main_surface, NULL);
    SDL_UpdateWindowSurface(*main_window);

    SDL_FreeSurface(loading_surface);
    return 0;

}

SDL_Rect create_rect(int x, int y, int w, int h)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    return rect;
}

void create_texture_rects(SDL_Rect* texture_rects)
{
    texture_rects[SMALL_WHITE_ID] = create_rect(30, 450, 30, 30);
    texture_rects[SMALL_BLACK_ID] = create_rect(0, 450, 30, 30);
    texture_rects[WHITE_ID] = create_rect(0, 0, 75, 75);
    texture_rects[BLACK_ID] = create_rect(0, 75, 75, 75);
    texture_rects[KING_WHITE_ID] = create_rect(0, 150, 75, 75);
    texture_rects[KING_BLACK_ID] = create_rect(0, 225, 75, 75);
    texture_rects[OPTION_ID] = create_rect(0, 375, 75, 75);
    texture_rects[SELECTED_ID] = create_rect(0, 300, 75, 75);
    texture_rects[CURRENT_TURN_ID] = create_rect(380, 695, 1, 1);
    texture_rects[DESK_ID] = create_rect(150, 50, 600, 600);
}


void draw_rules()
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                             "Rules",
                             "Something",
                             NULL);
}

int draw_game_background(
        SDL_Window* main_window,
        SDL_Surface* checkers_surface,
        SDL_Rect* texture_rects,
        int player_id)
{
    SDL_Surface* base_surface;
    SDL_Surface* main_surface = SDL_GetWindowSurface(main_window);

    base_surface = SDL_LoadBMP("img/background.bmp");
    if (!base_surface)
        return -1;

    SDL_Rect status_bar;

    status_bar.x = 380;
    status_bar.y = 663;
    status_bar.w = status_bar.h = 1;

    if (player_id == 1)
        SDL_BlitSurface(checkers_surface, &texture_rects[SMALL_WHITE_ID], base_surface, &status_bar);
    else
        SDL_BlitSurface(checkers_surface, &texture_rects[SMALL_BLACK_ID], base_surface, &status_bar);

    SDL_BlitSurface(base_surface, NULL, main_surface, NULL);
    SDL_UpdateWindowSurface(main_window);

    SDL_FreeSurface(base_surface);
    return 0;
}

int draw_checkers_on_desk(
        SDL_Window* main_window,
        SDL_Surface* desk_surface,
        SDL_Surface* checkers_surface,
        SDL_Rect* texture_rects,
        int desk[8][8],
        int status)
{

    const int x_on_surface_start = 150,
              y_on_surface_start = 50,
              step_on_surface = 75;
    SDL_Surface* main_surface = SDL_GetWindowSurface(main_window);
    SDL_BlitSurface(desk_surface, NULL, main_surface, &texture_rects[DESK_ID]);
    if (status == 1)
        SDL_BlitSurface(checkers_surface,
                &texture_rects[SMALL_WHITE_ID],
                main_surface,
                &texture_rects[CURRENT_TURN_ID]);
    else
        SDL_BlitSurface(checkers_surface,
                &texture_rects[SMALL_BLACK_ID],
                main_surface,
                &texture_rects[CURRENT_TURN_ID]);

    SDL_Rect checker_on_desk;
    checker_on_desk.x = x_on_surface_start;
    checker_on_desk.y = y_on_surface_start;
    checker_on_desk.w = checker_on_desk.h = step_on_surface;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (desk[i][j])
                SDL_BlitSurface(checkers_surface,
                            &texture_rects[desk[i][j]],
                            main_surface,
                            &checker_on_desk);

            checker_on_desk.x += step_on_surface;
        }
        checker_on_desk.x = x_on_surface_start;
        checker_on_desk.y += step_on_surface;
    }

    SDL_UpdateWindowSurface(main_window);
    return 0;
}

void draw_selected(
        SDL_Window* main_window,
        SDL_Surface* checkers_surface,
        SDL_Rect* texture_rects,
        SDL_Rect draw_at)
{
    SDL_Surface* main_surface = SDL_GetWindowSurface(main_window);
    SDL_BlitSurface(checkers_surface, &texture_rects[SELECTED_ID], main_surface, &draw_at);
    SDL_UpdateWindowSurface(main_window);
}

int draw_options(
        SDL_Window* main_window,
        SDL_Surface* checkers_surface,
        SDL_Rect* texture_rects,
        int number_of_options,
        SDL_Rect* opt_rect)
{
    SDL_Surface* main_surface = SDL_GetWindowSurface(main_window);
    for (int i = 0; i < number_of_options; i++)
    {
        if (opt_rect[i].x == 0)
            continue;

        SDL_BlitSurface(checkers_surface, &texture_rects[OPTION_ID], main_surface, &opt_rect[i]);
    }
    SDL_UpdateWindowSurface(main_window);

    return 0;
}

int free_window_surfaces(
        SDL_Window* main_window,
        SDL_Surface* checkers_surface,
        SDL_Surface* desk_surface)
{
    SDL_Surface* main_surface = SDL_GetWindowSurface(main_window);
    SDL_FreeSurface(main_surface);
    SDL_FreeSurface(checkers_surface);
    SDL_FreeSurface(desk_surface);
    SDL_DestroyWindow(main_window);
    SDL_Quit();
    return 0;
}



void draw_deads(
        SDL_Window* main_window,
        SDL_Surface* checkers_surface,
        SDL_Rect* texture_rects,
        int player_id,
        int number_of_deads)
{
    SDL_Surface* main_surface = SDL_GetWindowSurface(main_window);
    SDL_Rect* current_players_color;
    SDL_Rect draw_at;

    current_players_color = player_id == 1 ? &texture_rects[SMALL_WHITE_ID] : &texture_rects[SMALL_BLACK_ID];

    draw_at.x = player_id == 1 ? 10 : 760;
    draw_at.y = 60;
    draw_at.w = 30;
    draw_at.h = 30;

    for (int i = 0; i < number_of_deads; i++)
    {
        SDL_BlitSurface(checkers_surface, current_players_color, main_surface, &draw_at);
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
    char* results[] = {
            "Whites won!",
            "Blacks won!",
            "Blacks won! (Whites left)",
            "Whites won! (Blacks left)",
            "Wrong end status received"
            };

    if (status >= 6 && status <= 9)
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                 "Congratulations!",
                                 results[status - 6],
                                 NULL);
    else
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                 "Whoops!",
                                 results[4],
                                 NULL);
}

