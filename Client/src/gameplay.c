#include <SDL2/SDL_events.h>
#include "gameplay.h"
#include "network.h"
#include "gui.h"


pos_t rect_to_pos_t(SDL_Rect);
SDL_Rect pos_t_to_rect(pos_t);
pos_t* find_options(int player_id, int opponent_id, int desk[8][8], pos_t cur_pos);
pos_t* find_options_big(int player_id, int desk[8][8], pos_t cur_pos);
pos_t* find_options_after(int player_id, int opponent_id, int desk[8][8], pos_t cur_pos);
void set_option(pos_t* where_to, int x, int y);
int handle_selected_option(SDL_Rect* opt_rect, int x, int y);
pos_t make_move(int desk[8][8], pos_t cur_pos, pos_t new);
int count_options(pos_t options_pos[]);
int select_checker(int player_id, pos_t *cur_pos, int desk[8][8], pos_t **options_pos, SDL_Rect** options_rect);



int game_start(int player_id, int opponent_id, int desk[8][8], int status)
{
    pos_t cur_pos;
    SDL_Event event;
    int is_selected = 0, is_moved = 0;
    pos_t *options_pos = NULL;
    SDL_Rect * options_rect = NULL;
    int num_of_checkers_alive = count_checkers_on_desk(opponent_id, desk);

    while(SDL_WaitEvent(&event))
    {
        if (event.type == SDL_MOUSEBUTTONDOWN )
        {
            //handling mouse press on menu button
            /*if (event.button.x > 700 && event.button.x < 876
                        && event.button.y > 675 && event.button.y < 726)
            {
                puts ("Menu pressed");
                return -1;
            }*/

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
                    || desk[selected_pos.raw][selected_pos.col] == (player_id + 5)))
                    {
    
                        if (is_selected)
                            draw_desk_checkers(player_id, desk, status);
    
                        cur_pos.raw = selected_pos.raw;
                        cur_pos.col = selected_pos.col;
    
    
                        desk[cur_pos.raw][cur_pos.col] == (player_id + 5) ?
                        (options_pos = find_options_big(player_id, desk, cur_pos))
                        :
                        (options_pos = find_options(player_id, opponent_id, desk, cur_pos));
    
                        is_selected = select_checker(player_id, &cur_pos, desk, &options_pos, &options_rect);

                        continue;
    
                    }
    
    
    
                    if (is_selected)
                    {
                        for (int i = 0; i < 4; i++)
                        {
    
                            if (selected_pos.raw == options_pos[i].raw
                                && selected_pos.col == options_pos[i].col)
                            {
                                // TODO: is it equals to i ???
                                int needed_id = handle_selected_option(options_rect, event.button.x, event.button.y);
    
                                cur_pos = make_move(desk, cur_pos, options_pos[needed_id]);
                                draw_desk_checkers(player_id, desk, status);
    
                                if (options_pos != NULL)
                                    free(options_pos);
                                if (options_rect != NULL)
                                    free(options_rect);
    
    
    
                                int new_num_of_checkers_alive = count_checkers_on_desk(opponent_id, desk);
    
                                if (new_num_of_checkers_alive != num_of_checkers_alive)
                                {
    
                                    draw_deads(opponent_id, 12 - new_num_of_checkers_alive);
    
                                    options_pos = find_options_after(player_id, opponent_id, desk, cur_pos);
    
                                    if (count_options(options_pos) > 0)
                                    {

                                        select_checker(player_id, &cur_pos, desk, &options_pos, &options_rect);
                                        is_moved++;
                                        break;
                                    }
                                    else
                                        return opponent_id;
    
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
                            //TODO: IS IT EQUALS TO i?
                            int needed_id = handle_selected_option(options_rect, event.button.x, event.button.y);

                            cur_pos = make_move(desk, cur_pos, options_pos[needed_id]);
                            draw_desk_checkers(player_id, desk, status);

                            if (options_pos != NULL)
                                free(options_pos);
                            if (options_rect != NULL)
                                free(options_rect);

                            options_pos = find_options_after(player_id, opponent_id, desk, cur_pos);

                            if (count_options(options_pos) > 0)
                            {

                                select_checker(player_id, &cur_pos, desk, &options_pos, &options_rect);
                                break;
                            }
                            else
                                return opponent_id;

                        }
                    }
                    continue;
                }

            }
        }

    }

}



int select_checker(int player_id, pos_t *cur_pos, int desk[8][8], pos_t **options_pos, SDL_Rect** options_rect)
{

    draw_selected(pos_t_to_rect(*cur_pos));

    *options_rect = (SDL_Rect *) malloc(sizeof(SDL_Rect) * 4);
    for (int k = 0; k < 4; k++)
        (*options_rect)[k] = pos_t_to_rect((*options_pos)[k]);

    draw_options(*options_rect);
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

pos_t make_move(int desk[8][8], pos_t cur_pos, pos_t new)
{
    desk[new.raw][new.col] = desk[cur_pos.raw][cur_pos.col];
    desk[cur_pos.raw][cur_pos.col] = 0;

    if (abs(cur_pos.col - new.col) > 1)
        desk[(cur_pos.raw + new.raw) / 2][(cur_pos.col + new.col) / 2] = 0;


    return new;
}

int handle_selected_option(SDL_Rect* opt_rect, int x, int y)
{

    for (int i = 0; i < 4; i++)
    {
        if (x > opt_rect[i].x
        && x < (opt_rect[i].x + 75)
        && y < (opt_rect[i].y + 75)
        && y < (opt_rect[i].y + 75))

            return i;
    }


}

pos_t rect_to_pos_t(SDL_Rect rect)
{
    pos_t position;
    position.col = (rect.x - 150 ) / 75;
    position.raw = (rect.y - 50 )  / 75;

    return position;
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

pos_t* find_options(int player_id, int opponent_id, int desk[8][8], pos_t cur_pos)
{

    pos_t* options = (pos_t*) malloc(sizeof(pos_t) * 4);


    int nx, ny;


    //up+left
    ny = cur_pos.raw - 1;
    nx = cur_pos.col - 1;

    if (nx > -1 && ny > -1)
    {
        if ((desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 5)
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
        if ((desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 5)
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
        if ((desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 5)
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
        if ((desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 5)
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

    return options;
}

pos_t* find_options_big(int player_id, int desk[8][8], pos_t cur_pos)
{
    pos_t* a = NULL;
    return a;
}

pos_t* find_options_after(int player_id, int opponent_id, int desk[8][8], pos_t cur_pos)
{
    pos_t* options = (pos_t*) malloc(sizeof(pos_t) * 4);

    int nx, ny;

    //up+left
    ny = cur_pos.raw - 1;
    nx = cur_pos.col - 1;

    if (nx > -1 && ny > -1
        &&(desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 5)
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
        &&(desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 5)
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
        && (desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 5)
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
        &&(desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 5)
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
                || desk[i][j] == player_id + 5)
                num++;

    return num;
}