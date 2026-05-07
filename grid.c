#include "grid.h"

Tile **grid_array = NULL;
uint16_t grid_width = 0;
uint16_t grid_height = 0;

uint8_t global_time = 0;
uint8_t border = 0;

uint8_t max_states = 4;

uint32_t mutation_rarity = 100000;

void Grid_Init(uint16_t w, uint16_t h)
{
    grid_width = w;
    grid_height = h;
    
    grid_array = (Tile**)malloc(h * sizeof(Tile*));
    
    for(int i = 0; i < h; i++)
    {
        grid_array[i] = (Tile*)malloc(w * sizeof(Tile));
    }
    
    Grid_Reset(0, 1000);
}

void Grid_Quit()
{
    for(int i = 0; i < grid_height; i++)
    {
        free(grid_array[i]);
    }
    
    free(grid_array);
}

void Grid_Reset(uint8_t state, uint16_t chance)
{
    Tile *tile;
    
    for(int i = 0; i < grid_height; i++)
    {
        for(int j = 0; j < grid_width; j++)
        {
            if(rnd() % 1000 < chance)
            {   
                Grid_Set(j, i, state);
            }
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
    
    itself = Grid_Get(x, y);
    
    for(int dir = 1; dir < 8; dir += 2)
    {
        dx = dir_to_coords[dir][0];
        dy = dir_to_coords[dir][1];
        
        neighbor = Grid_Get(x + dx, y + dy);
    }
}