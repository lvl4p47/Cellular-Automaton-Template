#include "grid.h"

Tile **grid_array = NULL;
uint16_t grid_width = 0;
uint16_t grid_height = 0;

uint8_t global_time = 0;
uint8_t border = 0;

uint32_t mutation_rarity = 100000;

uint8_t rule_set[MAX_STATES];

void Grid_Init(uint16_t w, uint16_t h)
{
    grid_width = w;
    grid_height = h;
    
    grid_array = (Tile**)malloc(h * sizeof(Tile*));
    
    for(int i = 0; i < h; i++)
    {
        grid_array[i] = (Tile*)malloc(w * sizeof(Tile));
    }
    
    Grid_Reset(0);
}

void Grid_Quit()
{
    for(int i = 0; i < grid_height; i++)
    {
        free(grid_array[i]);
    }
    
    free(grid_array);
}

void Grid_Reset(uint8_t state)
{
    Tile *tile;
    
    for(int i = 0; i < grid_height; i++)
    {
        for(int j = 0; j < grid_width; j++)
        {
            Grid_Set(j, i, state);
        }
    }
}

void Grid_Set(int16_t x, int16_t y, uint8_t state)
{
    uint16_t x1 = mod(x, grid_width);
    uint16_t y1 = mod(y, grid_height);
    
    grid_array[y1][x1].state = state;
    grid_array[y1][x1].buf_state = state;
}

void Grid_Update()
{
    #pragma omp parallel for
    for(uint16_t y = 0; y < grid_height; y++)
    {
        #pragma omp parallel for
        for(uint16_t x = 0; x < grid_width; x++)
        {
            Grid_Update_Stage_1(x, y);
        }
    }
    #pragma omp parallel for
    for(uint16_t y = 0; y < grid_height; y++)
    {
        #pragma omp parallel for
        for(uint16_t x = 0; x < grid_width; x++)
        {
            Grid_Update_Stage_2(x, y);
        }
    }
}

void Grid_Update_Stage_1(uint16_t x, uint16_t y)
{   
    Tile *itself;
    
    itself = Grid_Get(x, y);
    
    itself->state = itself->buf_state;
}

void Grid_Update_Stage_2(uint16_t x, uint16_t y)
{
    Tile *itself, *neighbor;
    int8_t dx, dy;
    uint8_t n_states[MAX_STATES];
    uint8_t mask;
    
    itself = Grid_Get(x, y);
    
    for(int state = 0; state < MAX_STATES; state++)
    {
        n_states[state] = 0;
    }
    
    for(int dir = 1; dir < 8; dir += 2)
    {
        dx = dir_to_coords[dir][0];
        dy = dir_to_coords[dir][1];
        mask = (uint8_t)1 << dir;
        
        neighbor = Grid_Get(x + dx, y + dy);
        
        n_states[neighbor->state] |= mask;
    }
    
    for(int rot = 0; rot < 4; rot++)
    {
        switch (itself->state)
        {
        case 0:
            if(n_states[2] & 0b00000010
            && 
            (
               n_states[1] & 0b00001000
            || n_states[1] & 0b10000000
            )
            )
            {
                itself->buf_state = 2;
                return;
            }
            for(int state = 4; state < MAX_STATES; state += 2)
            {
                if(n_states[state] & 0b00100000)
                {
                    itself->buf_state = 1;
                    return;
                }
            }
            for(int state = 5; state < MAX_STATES; state += 2)
            {
                if(n_states[state] & 0b00000010
                && n_states[1] & 0b10001000
                )
                {
                    itself->buf_state = state;
                    return;
                }
            }
            break;
        case 1:
            for(int state = 4; state < MAX_STATES; state += 2)
            {
                if(n_states[state] & 0b00000010
                && n_states[0] == 0b10001000
                )
                {
                    itself->buf_state = 2;
                    return;
                }
                if(n_states[state] & 0b00000010
                && n_states[2] & 0b10001000
                )
                {
                    itself->buf_state = state + 1;
                    return;
                }
            }
            for(int state = 5; state < MAX_STATES; state += 2)
            {
                if(n_states[state] == 0b00000010
                )
                {
                    itself->buf_state = state;
                    return;
                }
                if(n_states[state] == 0b00100010
                )
                {
                    itself->buf_state = state - 1;
                    return;
                }
            }
            
            if(n_states[3] == 0b10001000
            )
            {
                itself->buf_state = 0;
                return;
            }
            
            break;
        case 2:
            itself->buf_state = 3;
            // for(int state = 3; state < MAX_STATES; state += 2)
            // {
            //     if(n_states[state])
            //     {
            //         itself->buf_state = 2;
            //         return;
            //     }
            // }
            break;
        case 3:
            itself->buf_state = 0;
            // for(int state = 3; state < MAX_STATES; state += 2)
            // {
            //     if(n_states[state])
            //     {
            //         itself->buf_state = 2;
            //         return;
            //     }
            // }
            break;
        
        default:
            if(itself->state % 2 == 1)
            {
                itself->buf_state = 0;
            }
            if(itself->state % 2 == 0)
            {
                if(n_states[itself->state + 1] == 0b10001000
                && n_states[0] == 0b00100000
                )
                {
                    itself->buf_state = 3;
                    return;
                }
            }
            // if(n_states[1])
            // {
            //     itself->buf_state = 0;
            //     return;
            // }
            break;
        }
        
        for(int state = 0; state < MAX_STATES; state++)
        {
        
            n_states[state] = Bit_Rotate(n_states[state], 2);
        }
    }
}