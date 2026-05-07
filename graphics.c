#include "graphics.h"
#include <SDL2/SDL_image.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint8_t display_mode = STATES;

void Graphics_Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return ;
    }

    window = SDL_CreateWindow("templateSDL", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
    SDL_WINDOW_BORDERLESS);
    if (window == NULL) {
        fprintf(stderr, "Window could not be created! SDL Error: %s\n", SDL_GetError());
        SDL_Quit();
        return ;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return ;
    }
}

void Graphics_Quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Screen_Clear()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Screen_Draw()
{
    Grid_Draw();
    SDL_RenderPresent(renderer);
}

void Grid_Draw()
{
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = CELL_SIZE;
    rect.h = CELL_SIZE;
    
    SDL_Rect dot;
    dot.x = 0;
    dot.y = 0;
    dot.w = CELL_SIZE / 2;
    dot.h = CELL_SIZE / 2;
    
    Tile *tile;
    
    int16_t x, y;
    
    switch (display_mode)
    {
    case STATES:
        for(int i = 0; i < grid_height; i++)
        {
            for(int j = 0; j < grid_width; j++)
            {
                tile = Grid_Get(j, i);
                uint8_t state = tile->state;
                
                int r = 0, g = 0, b = 0;
                
                switch(state)
                {
                case 0:
                    break;
                
                default:
                    r = 127, g = 127, b = 127;
                    break;
                }
                
                rect.x = j * CELL_SIZE;
                rect.y = i * CELL_SIZE;
                
                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
        break;
    default:
        break;
    }
    
}

