#include <SDL2/SDL_events.h>
#include "gameplay.h"
#include "gui.h"


pos rect_to_pos(SDL_Rect);
SDL_Rect pos_to_rect(pos);
pos* find_options(int player_id, int desk[8][8], pos cur);
pos* find_options_big(int player_id, int desk[8][8], pos cur);
void set_option(pos* where_to, int x, int y);
int handle_selected_option(SDL_Rect*);
pos make_move(int desk[8][8], pos cur, pos new);


int game_start(int player_id, int desk[8][8])
{
    pos cur;
    SDL_Event event;

    while(SDL_WaitEvent(&event))
    {
        if (event.type == SDL_MOUSEBUTTONDOWN )
        {

            //handling mouse press on menu button
            if (event.button.x > 700 && event.button.x < 876
                        && event.button.y > 675 && event.button.y < 726)
            {
                //TODO: pop up menu
                puts ("Menu pressed");
                return 0;
            }

            //handling mouse down on desk
            if (event.button.x > 150 && event.button.x < 750
                        && event.button.y > 50 && event.button.y < 650)
            {
                //col equals x
                //raw equals y
                pos new_cur;

                new_cur.col = (event.button.x - 150) / 75;
                new_cur.raw = (event.button.y - 50) / 75;

                printf("selected: [%d; %d]\n", new_cur.raw, new_cur.col);

                //if selected is checker
                if(desk[new_cur.raw][new_cur.col] == player_id
                    || desk[new_cur.raw][new_cur.col] == (player_id + 5))
                {
                    cur.raw= new_cur.raw;
                    cur.col = new_cur.col;

                    draw_selected(pos_to_rect(cur));


                    pos *options;
                    desk[new_cur.raw][new_cur.col] == (player_id + 5) ?
                        (options = find_options_big(player_id, desk, cur))
                        :
                        (options = find_options(player_id, desk, cur));

                    SDL_Rect * options_rect;
                    options_rect = (SDL_Rect*) malloc(sizeof(SDL_Rect) * 4);
                    for (int k = 0; k < 4; k++)
                        if (options[k].col != -1)
                            options_rect[k] = pos_to_rect(options[k]);
                        else
                            options_rect[k].x = 0;

                        
                    draw_options(options_rect);
                    // return index of selected option handle_selected_option(options_rect);

                    cur = make_move(desk, cur, options[handle_selected_option(options_rect)]);

                    free(options);
                    free(options_rect);
                    
                    return 0;
                }
            }

        }

    }


}


pos make_move(int desk[8][8], pos cur, pos new)
{
    desk[new.raw][new.col] = desk[cur.raw][cur.col];
    desk[cur.raw][cur.col] = 0;

    if (abs(cur.col - new.col) > 1)
        desk[(cur.raw + new.raw) / 2][(cur.col + new.col) / 2] = 0;

    cur.col = new.col;
    cur.raw = new.raw;
}


int handle_selected_option(SDL_Rect* opt_rect)
{
    SDL_Event event;

    while(SDL_WaitEvent(&event))
    {
        if (event.type == SDL_MOUSEBUTTONDOWN )
        {
            for (int i = 0; i < 4; i++)
            {
                if (event.button.x > opt_rect[i].x 
                && event.button.x < (opt_rect[i].x + 75)
                && event.button.y < (opt_rect[i].y + 75)
                && event.button.y < (opt_rect[i].y + 75))
                    
                    return i;
            }
        }
    }
}

pos rect_to_pos(SDL_Rect rect)
{
    pos position;
    position.col = (rect.x - 150 ) / 75;
    position.raw = (rect.y - 50 )  / 75;

    return position;
}

SDL_Rect pos_to_rect(pos position)
{
    SDL_Rect rect;
    rect.x = (position.col * 75) + 150;
    rect.y = (position.raw * 75)+ 50;
    rect.h = rect.w = 75;

    return rect;
}

void set_option(pos* where_to, int x, int y)
{
    where_to -> col = x;
    where_to -> raw = y;
}

pos* find_options(int player_id, int desk[8][8], pos cur)
{

    pos* options = (pos*) malloc(sizeof(pos) * 4);

    int opponent_id;

    opponent_id = player_id == 1 ? 2 : 1;

    int nx, ny;


    //up+left
    ny = cur.raw - 1;
    nx = cur.col - 1;

    if (nx > -1 && ny > -1)
    {
        if ((desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 5)
        && (nx - 1) > -1
        && (ny - 1) > -1
        && desk[ny - 1][nx - 1] == 0)
        {
            set_option(&options[0], nx - 1, ny - 1);
        }
        else if (desk[ny][nx] == 0)
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
    ny = cur.raw - 1;
    nx = cur.col + 1;

    if (nx < 8 && ny > -1)
    {
        if ((desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 5)
            && (nx + 1) < 8
            && (ny - 1) > -1
            && desk[ny - 1][nx + 1] == 0)
        {
            set_option(&options[1], nx + 1, ny - 1);
        }
        else if (desk[ny][nx] == 0)
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
    ny = cur.raw + 1;
    nx = cur.col - 1;

    if (nx > -1 && ny < 8)
    {
        if ((desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 5)
            && (nx - 1) > -1
            && (ny + 1) < 8
            && desk[ny + 1][nx - 1] == 0)
        {
            set_option(&options[2], nx - 1, ny + 1);
        }
        else if (desk[ny][nx] == 0)
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
    ny = cur.raw + 1;
    nx = cur.col + 1;

    if (nx < 8 && ny < 8)
    {
        if ((desk[ny][nx] == opponent_id || desk[ny][nx] == opponent_id + 5)
            && (nx + 1) < 8
            && (ny + 1) < 8
            && desk[ny + 1][nx + 1] == 0)
        {
            set_option(&options[3], nx + 1, ny + 1);
        }
        else if (desk[ny][nx] == 0)
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

pos* find_options_big(int player_id, int desk[8][8], pos cur)
{
    pos* a = NULL;
    return a;
}
