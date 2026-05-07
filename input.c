#include "input.h"

uint8_t quit = 0;
uint8_t slow_mode = 0;
int lmb_held = 0;
int rmb_held = 0;
int mmb_held = 0;
SDL_Event e;

int grab_x, grab_y, dest_x, dest_y, str;

void Events_Handle()
{
    
    while (SDL_PollEvent(&e) != 0) 
    {
        
        if (e.type == SDL_QUIT) 
        {
            quit = 1;
        }
        
        if (e.type == SDL_KEYDOWN) 
        {
            if(e.key.keysym.sym == SDLK_q)
            {
                display_mode = mod(display_mode - 1, DISP_MODE_COUNT);
                last_frame = 0;
            }
            if(e.key.keysym.sym == SDLK_w)
            {
                display_mode = mod(display_mode + 1, DISP_MODE_COUNT);
                last_frame = 0;
            }
            if(e.key.keysym.sym == SDLK_r)
            {
                
            }
            if(e.key.keysym.sym == SDLK_t)
            {
                
            }
            if(e.key.keysym.sym == SDLK_p)
            {
                
            }
            if(e.key.keysym.sym == SDLK_s)
            {
                slow_mode = 1 - slow_mode;
            }
            if(e.key.keysym.sym == SDLK_d)
            {
                
            }
            if(e.key.keysym.sym == SDLK_l)
            {
                
            }
            if(e.key.keysym.sym == SDLK_z)
            {
                
            }
            if(e.key.keysym.sym == SDLK_x)
            {
                
            }
            if(e.key.keysym.sym == SDLK_c)
            {
                
            }
            if(e.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = 1;
            }
            if(e.key.keysym.sym == SDLK_SPACE)
            {
                pause = 1 - pause;
            }
        }
        
        if (e.type == SDL_MOUSEBUTTONDOWN) 
        {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            
            int x = mouseX / CELL_SIZE;
            int y = mouseY / CELL_SIZE;
            
            if (e.button.button == SDL_BUTTON_RIGHT) 
            {
                rmb_held = 1;
                
            }
            if (e.button.button == SDL_BUTTON_LEFT) 
            {
                lmb_held = 1;
            }
            if (e.button.button == SDL_BUTTON_MIDDLE) 
            {
                mmb_held = 1;
            }
        }
        if (e.type == SDL_MOUSEBUTTONUP) {
            if (e.button.button == SDL_BUTTON_LEFT) 
            {
                lmb_held = 0;
            }
            if (e.button.button == SDL_BUTTON_RIGHT) 
            {
                rmb_held = 0;
            }
            if (e.button.button == SDL_BUTTON_MIDDLE) 
            {
                mmb_held = 0;
            }
        }
        if (e.type == SDL_MOUSEMOTION) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            
            int x = mouseX / CELL_SIZE;
            int y = mouseY / CELL_SIZE;

            if (rmb_held == 1)
            {
                Grid_Set(x, y, 1);
            }
            if (mmb_held == 1)
            {
                
            }
            if (lmb_held == 1)
            {
                
            }
        }
    }
}
