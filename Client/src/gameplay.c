#include <SDL2/SDL_events.h>
#include "gameplay.h"
#include "gui.h"

SDL_Rect pos_t_to_rect(pos_t);
pos_t* find_options(int player_id, int opponent_id, int* checker_type, int desk[8][8], pos_t cur_pos);
pos_t* find_options_big(int opponent_id, int* checker_type, int desk[8][8], pos_t cur_pos);
pos_t* find_options_after(int opponent_id, int desk[8][8], pos_t cur_pos);
void set_option(pos_t* where_to, int x, int y);
pos_t make_move(int desk[8][8], pos_t cur_pos, pos_t new);
int count_options(pos_t options_pos[]);
int select_checker(
        SDL_Window* main_window,
        SDL_Surface* main_surface,
        SDL_Surface* checkers_surface,
        SDL_Rect* texture_rects,
        int checker_type,
        pos_t *cur_pos,
        pos_t **options_pos,
        SDL_Rect** options_rect);

void check_growing(int player_id, pos_t cur_pos, int desk[8][8]);

int game_start(
        SDL_Window* main_window,
        SDL_Surface* main_surface,
        SDL_Surface* desk_surface,
        SDL_Surface* checkers_surface,
        SDL_Rect* texture_rects,
        int player_id,
        int opponent_id,
        int desk[8][8],
        int status)
{
    pos_t cur_pos = {0,0};
    SDL_Event event;
    int is_selected = 0, is_moved = 0;
    pos_t *options_pos = NULL;
    SDL_Rect * options_rect = NULL;
    int num_of_checkers_alive = count_checkers_on_desk(opponent_id, desk);
    int checker_type = 0; // 0 for usual 1 for big

    while(SDL_WaitEvent(&event))
    {
        if (event.type == SDL_MOUSEBUTTONDOWN )
        {
            //handling mouse press on exit button
            if (event.button.x > 700 && event.button.x < 876
                        && event.button.y > 675 && event.button.y < 726)
                return player_id + 7;


            //options
            if (event.button.x > 499 && event.button.x < 675
                && event.button.y > 675 && event.button.y < 726)
                draw_rules();


            //handling mouse down on desk
            if (event.button.x > 150 && event.button.x < 750
                && event.button.y > 50 && event.button.y < 650)
            {

                pos_t selected_pos;

                selected_pos.col = (event.button.x - 150) / 75;
                selected_pos.raw = (event.button.y - 50) / 75;

                if (!is_moved)
                {
                    //if selected is checker
                    if((desk[selected_pos.raw][selected_pos.col] == player_id
                    || desk[selected_pos.raw][selected_pos.col] == (player_id + 3)))
                    {
    
                        if (is_selected)
                            draw_checkers_on_desk(main_window,
                                    main_surface,
                                    desk_surface,
                                    checkers_surface,
                                    texture_rects,
                                    desk,
                                    status);
    
                        cur_pos.raw = selected_pos.raw;
                        cur_pos.col = selected_pos.col;
    
    
                        desk[cur_pos.raw][cur_pos.col] == (player_id + 3) ?

                        (options_pos = find_options_big( opponent_id, &checker_type, desk, cur_pos))
                        :
                        (options_pos = find_options(player_id, opponent_id, &checker_type, desk, cur_pos));
    
                        is_selected = select_checker(
                                main_window,
                                main_surface,
                                checkers_surface,
                                texture_rects,
                                checker_type,
                                &cur_pos,
                                &options_pos,
                                &options_rect);

                        continue;
    
                    }
    
    
    
                    if (is_selected)
                    {
                        for (int i = 0; i < (checker_type ? 13 : 4); i++)
                        {
    
                            if (selected_pos.raw == options_pos[i].raw
                                && selected_pos.col == options_pos[i].col)
                            {
    
                                cur_pos = make_move(desk, cur_pos, options_pos[i]);
                                check_growing(player_id, cur_pos, desk);
                                draw_checkers_on_desk(main_window,
                                        main_surface,
                                        desk_surface,
                                        checkers_surface,
                                        texture_rects,
                                        desk,
                                        status);

                                free(options_pos);
                                free(options_rect);


                                int new_num_of_checkers_alive = count_checkers_on_desk(opponent_id, desk);
    
                                if (new_num_of_checkers_alive != num_of_checkers_alive)
                                {

                                    draw_deads(
                                            main_window,
                                            main_surface,
                                            checkers_surface,
                                            texture_rects,
                                            opponent_id,
                                            12 - new_num_of_checkers_alive);
    
                                    options_pos = find_options_after(opponent_id, desk, cur_pos);
    
                                    if (count_options(options_pos) > 0)
                                    {

                                        select_checker(
                                                main_window,
                                                main_surface,
                                                checkers_surface,
                                                texture_rects,
                                                0,
                                                &cur_pos,
                                                &options_pos,
                                                &options_rect);
                                        is_moved++;
                                        break;
                                    }
                                    else
                                    {
                                        free(options_pos);
                                        return opponent_id;
                                    }
                                }
                                else
                                    return opponent_id;
                            }
                        }
                        continue;
                    }
                }

                else
                {

                    for (int i = 0; i < 4; i++)
                    {

                        if (selected_pos.raw == options_pos[i].raw
                            && selected_pos.col == options_pos[i].col)
                        {

                            cur_pos = make_move(desk, cur_pos, options_pos[i]);
                            check_growing(player_id, cur_pos, desk);
                            draw_checkers_on_desk(main_window,
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
                                    opponent_id,
                                    12 - count_checkers_on_desk(opponent_id, desk));

                            free(options_pos);
                            free(options_rect);


                            options_pos = find_options_after(opponent_id, desk, cur_pos);

                            if (count_options(options_pos) > 0)
                            {

                                select_checker(
                                        main_window,
                                        main_surface,
                                        checkers_surface,
                                        texture_rects,
                                        0,
                                        &cur_pos,
                                        &options_pos,
                                        &options_rect);
                                break;
                            }
                            else
                            {
                                free(options_pos);
                                return opponent_id;
                            }
                        }
                    }
                    continue;
                }

            }
        }

    }

}



int select_checker(
        SDL_Window* main_window,
        SDL_Surface* main_surface,
        SDL_Surface* checkers_surface,
        SDL_Rect* texture_rects,
        int checker_type,
        pos_t *cur_pos,
        pos_t **options_pos,
        SDL_Rect** options_rect)
{

    draw_selected(
            main_window,
            main_surface,
            checkers_surface,
            texture_rects,
            pos_t_to_rect(*cur_pos));

    int times = (checker_type ? 13 : 4);

    *options_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect) * times);
    for (int k = 0; k < times; k++)
        (*options_rect)[k] = pos_t_to_rect((*options_pos)[k]);

    draw_options(
            main_window,
            main_surface,
            checkers_surface,
            texture_rects,
            times,
            *options_rect);
    return 1;

}

int count_options(pos_t options_pos[])
{
    int num = 0;
    for(int i = 0; i < 4; i++)
        if(options_pos[i].raw != -1)
            num++;

    return num;
}

pos_t make_move(int desk[8][8], pos_t cur_pos, pos_t new_pos)
{
    desk[new_pos.raw][new_pos.col] = desk[cur_pos.raw][cur_pos.col];
    desk[cur_pos.raw][cur_pos.col] = 0;

    pos_t diff;
    diff.raw = new_pos.raw - cur_pos.raw;
    diff.col = new_pos.col - cur_pos.col;

    if (diff.raw < 0 && diff.col < 0)
        desk[new_pos.raw + 1][new_pos.col + 1] = 0;

    if (diff.raw < 0 && diff.col > 0)
        desk[new_pos.raw + 1][new_pos.col - 1] = 0;

    if (diff.raw > 0 && diff.col < 0)
        desk[new_pos.raw - 1][new_pos.col + 1] = 0;

    if (diff.raw > 0 && diff.col > 0)
        desk[new_pos.raw - 1][new_pos.col - 1] = 0;

    return new_pos;
}

SDL_Rect pos_t_to_rect(pos_t position)
{
    SDL_Rect rect;

    if(position.col == -1)
    {

        rect.x = 0;
        rect.y = 0;
        rect.h = 0;
        rect.w = 0;

    }
    else
    {

        rect.x = (position.col * 75) + 150;
        rect.y = (position.raw * 75)+ 50;
        rect.h = 75;
        rect.w = 75;

    }
    return rect;
}

void set_option(pos_t* where_to, int x, int y)
{
    where_to -> col = x;
    where_to -> raw = y;
}

pos_t* find_options(int player_id, int opponent_id, int* checker_type, int desk[8][8], pos_t cur_pos)
{

    pos_t* options = (pos_t*) malloc(sizeof(pos_t) * 4);


    int nx, ny;


    //up+left
    ny = cur_pos.raw - 1;
    nx = cur_pos.col - 1;

    if (nx > -1 && ny > -1)
    {
        if ((desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 3)
        && (nx - 1) > -1
        && (ny - 1) > -1
        && desk[ny - 1][nx - 1] == 0)
        {
            set_option(&options[0], nx - 1, ny - 1);
        }
        else if (desk[ny][nx] == 0 && player_id == 1)
        {
            set_option(&options[0], nx, ny);
        }
        else
        {
            set_option(&options[0], -1, -1);

        }
    }
    else
    {
        set_option(&options[0], -1, -1);
    }



    //up+right
    ny = cur_pos.raw - 1;
    nx = cur_pos.col + 1;

    if (nx < 8 && ny > -1)
    {
        if ((desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 3)
            && (nx + 1) < 8
            && (ny - 1) > -1
            && desk[ny - 1][nx + 1] == 0)
        {
            set_option(&options[1], nx + 1, ny - 1);
        }
        else if (desk[ny][nx] == 0 && player_id == 1)
        {
            set_option(&options[1], nx, ny);
        }
        else
        {
            set_option(&options[1], -1, -1);

        }
    }
    else
    {
        set_option(&options[1], -1, -1);
    }


    //down+left
    ny = cur_pos.raw + 1;
    nx = cur_pos.col - 1;

    if (nx > -1 && ny < 8)
    {
        if ((desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 3)
            && (nx - 1) > -1
            && (ny + 1) < 8
            && desk[ny + 1][nx - 1] == 0)
        {
            set_option(&options[2], nx - 1, ny + 1);
        }
        else if (desk[ny][nx] == 0 && player_id == 2)
        {
            set_option(&options[2], nx, ny);
        }
        else
        {
            set_option(&options[2], -1, -1);

        }
    }
    else
    {
        set_option(&options[2], -1, -1);
    }


    //down + right
    ny = cur_pos.raw + 1;
    nx = cur_pos.col + 1;

    if (nx < 8 && ny < 8)
    {
        if ((desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 3)
            && (nx + 1) < 8
            && (ny + 1) < 8
            && desk[ny + 1][nx + 1] == 0)
        {
            set_option(&options[3], nx + 1, ny + 1);
        }
        else if (desk[ny][nx] == 0 && player_id == 2)
        {
            set_option(&options[3], nx, ny);
        }
        else
        {
            set_option(&options[3], -1, -1);

        }
    }
    else
    {
        set_option(&options[3], -1, -1);
    }

    *checker_type = 0;
    return options;
}

pos_t* find_options_big(int opponent_id, int* checker_type, int desk[8][8], pos_t cur_pos)
{
    pos_t* options = (pos_t*) malloc(sizeof(pos_t) * 13);

    for(int i = 0; i < 13; i++)
    {
        set_option(&options[i], -1, -1);
    }

    int ind = 0;
    pos_t temp;

    //up + left
    for(temp.raw = cur_pos.raw - 1, temp.col = cur_pos.col - 1;
        temp.raw > -1 && temp.col > -1;
        temp.raw--, temp.col--)
    {
        if (desk[temp.raw][temp.col] == 0)
        {
            set_option(&options[ind], temp.col, temp.raw);
            ind++;
            continue;
        }
        else if ((desk[temp.raw][temp.col] == opponent_id || desk[temp.raw][temp.col] == (opponent_id + 3))
                 && desk[temp.raw - 1][temp.col - 1] == 0
                 && temp.raw - 1 > -1
                 && temp.col - 1 > -1)
        {
            set_option(&options[ind], temp.col - 1, temp.raw - 1);
            ind++;
            break;
        }
        else break;
    }


    //up + right
    for(temp.raw = cur_pos.raw - 1, temp.col = cur_pos.col + 1;
        temp.raw > -1 && temp.col < 8;
        temp.raw--, temp.col++)
    {
        if (desk[temp.raw][temp.col] == 0)
        {
            set_option(&options[ind], temp.col, temp.raw);
            ind++;
            continue;
        }
        else if ((desk[temp.raw][temp.col] == opponent_id || desk[temp.raw][temp.col] == (opponent_id + 3))
                 && desk[temp.raw - 1][temp.col + 1] == 0
                 && temp.raw - 1 > -1
                 && temp.col + 1 < 8)
        {
            set_option(&options[ind], temp.col + 1, temp.raw - 1);
            ind++;
            break;
        }
        else break;
    }


    //down + left
    for(temp.raw = cur_pos.raw + 1, temp.col = cur_pos.col - 1;
        temp.raw < 8 && temp.col > -1;
        temp.raw++, temp.col--)
    {
        if (desk[temp.raw][temp.col] == 0)
        {
            set_option(&options[ind], temp.col, temp.raw);
            ind++;
            continue;
        }
        else if ((desk[temp.raw][temp.col] == opponent_id || desk[temp.raw][temp.col] == (opponent_id + 3))
                 && desk[temp.raw + 1][temp.col - 1] == 0
                 && temp.raw + 1 < 8
                 && temp.col - 1 > -1)
        {
            set_option(&options[ind], temp.col - 1, temp.raw + 1);
            ind++;
            break;
        }
        else break;
    }

    //down + right
    for(temp.raw = cur_pos.raw + 1, temp.col = cur_pos.col + 1;
        temp.raw < 8 && temp.col < 8;
        temp.raw++, temp.col++)
    {
        if (desk[temp.raw][temp.col] == 0)
        {
            set_option(&options[ind], temp.col, temp.raw);
            ind++;
            continue;
        }
        else if ((desk[temp.raw][temp.col] == opponent_id || desk[temp.raw][temp.col] == (opponent_id + 3))
                 && desk[temp.raw + 1][temp.col + 1] == 0
                 && temp.raw + 1 < 8
                 && temp.col + 1 < 8)
        {
            set_option(&options[ind], temp.col + 1, temp.raw + 1);
            break;
        }
        else break;
    }


    *checker_type = 1;

    return options;
}

pos_t* find_options_after(int opponent_id, int desk[8][8], pos_t cur_pos)
{
    pos_t* options = (pos_t*) malloc(sizeof(pos_t) * 4);

    int nx, ny;

    //up+left
    ny = cur_pos.raw - 1;
    nx = cur_pos.col - 1;

    if (nx > -1 && ny > -1
        &&(desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 3)
        && (nx - 1) > -1
        && (ny - 1) > -1
        && desk[ny - 1][nx - 1] == 0)
    {
        set_option(&options[0], nx - 1, ny - 1);
    }
    else
    {
        set_option(&options[0], -1, -1);

    }


    //up+right
    ny = cur_pos.raw - 1;
    nx = cur_pos.col + 1;

    if (nx < 8 && ny > -1
        &&(desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 3)
        && (nx + 1) < 8
        && (ny - 1) > -1
        && desk[ny - 1][nx + 1] == 0)
    {
        set_option(&options[1], nx + 1, ny - 1);
    }
    else
    {
        set_option(&options[1], -1, -1);

    }


    //down+left
    ny = cur_pos.raw + 1;
    nx = cur_pos.col - 1;

    if (nx > -1 && ny < 8
        && (desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 3)
        && (nx - 1) > -1
        && (ny + 1) < 8
        && desk[ny + 1][nx - 1] == 0)
    {
        set_option(&options[2], nx - 1, ny + 1);
    }
    else
    {
        set_option(&options[2], -1, -1);

    }


    //down + right
    ny = cur_pos.raw + 1;
    nx = cur_pos.col + 1;

    if (nx < 8 && ny < 8
        &&(desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 3)
        && (nx + 1) < 8
        && (ny + 1) < 8
        && desk[ny + 1][nx + 1] == 0)
    {
        set_option(&options[3], nx + 1, ny + 1);
    }
    else
    {
        set_option(&options[3], -1, -1);

    }


    return options;
}

int count_checkers_on_desk(int player_id, int desk[8][8])
{
    int num = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (desk[i][j] == player_id
                || desk[i][j] == player_id + 3)
                num++;

    return num;
}

void check_growing(int player_id, pos_t cur_pos, int desk[8][8])
{

    int side = player_id == 1 ? 0 : 7;

    if (cur_pos.raw == side && desk[cur_pos.raw][cur_pos.col] == player_id)
        desk[side][cur_pos.col] += 3;

}