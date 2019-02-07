#ifndef GUI_H

#define GUI_H
#include <SDL2/SDL.h>




int create_window_with_surface(SDL_Window** window, SDL_Surface** surface);

void create_texture_rects(SDL_Rect* texture_rects);

SDL_Surface* create_surface_from_bmp(const char* path);

int draw_image (SDL_Window** main_window, SDL_Surface** main_surface, const char* path);

int draw_game_background(
        SDL_Window* main_window,
        SDL_Surface* main_surface,
        SDL_Surface* checkers_surface,
        SDL_Rect* texture_rects,
        int player_id);

int draw_checkers_on_desk(
        SDL_Window* main_window,
        SDL_Surface* main_surface,
        SDL_Surface* desk_surface,
        SDL_Surface* checkers_surface,
        SDL_Rect* texture_rects,
        int desk[8][8],
        int status);

void draw_selected(
        SDL_Window* main_window,
        SDL_Surface* main_surface,
        SDL_Surface* checkers_surface,
        SDL_Rect* texture_rects,
        SDL_Rect draw_at);

int draw_options(
        SDL_Window* main_window,
        SDL_Surface* main_surface,
        SDL_Surface* checkers_surface,
        SDL_Rect* texture_rects,
        int number_of_options,
        SDL_Rect* opt_rect);

int free_window_surfaces(
        SDL_Window* main_window,
        SDL_Surface* main_surface,
        SDL_Surface* checkers_surface,
        SDL_Surface* desk_surface);

void draw_deads(
        SDL_Window* main_window,
        SDL_Surface* main_surface,
        SDL_Surface* checkers_surface,
        SDL_Rect* texture_rects,
        int player_id,
        int number_of_deads);

void draw_result(int status);
void draw_rules();

#endif