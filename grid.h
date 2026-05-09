#ifndef GRID_H
#define GRID_H

#include "utility.h"

#define MAX_PHEROMONES 8
#define MAX_STATES 8

extern uint8_t global_time;

typedef struct {
    uint8_t state, buf_state;
} Tile;

extern uint16_t grid_width;
extern uint16_t grid_height;

extern Tile **grid_array;
extern int lmb_held, rmb_held, mmb_held;
extern uint16_t cycles;
extern uint32_t total_cycles;
extern uint8_t max_states;

void Grid_Init(uint16_t w, uint16_t h);
void Grid_Quit();
void Grid_Reset(uint8_t type);

static inline Tile* Grid_Get(int16_t x, int16_t y)
{
    uint16_t x1 = mod(x, grid_width);
    uint16_t y1 = mod(y, grid_height);
    return &grid_array[y1][x1];
}

void Grid_Set(int16_t x, int16_t y, uint8_t state);
void Grid_Update();
void Grid_Update_Stage_1(uint16_t x, uint16_t y);
void Grid_Update_Stage_2(uint16_t x, uint16_t y);

#endif