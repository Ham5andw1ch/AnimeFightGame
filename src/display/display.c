//This file is frontend to sidestep most of the work handling SDL_CreateWindow boilerplate.
//
//Sprites are handled as a struct containing the number of animation frames and the SDL_Surface created
//  elsewhere.
//
//There is no need for the user to define rectangles to display, but they are required to wash the screen
//  because this class simply handles the blitting.

#include <stdio.h>
#include "SDL2/SDL.h"
#include "global.h"
#include "settings.h"
#include "display.h"
SDL_Window *window;
SDL_Surface *game_surface;
SDL_Surface *ui_surface;

SDL_Window *makeWindow(uint16_t x, uint16_t y, char* name){
    window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, x, y, 0);
    return window;
}

int blitSprite(sprite_t* src, uint16_t x, uint16_t y, uint8_t frame){
    SDL_Rect srcRect, destRect;
    //Grab the frame of the sprite
    srcRect.x = ((src->surface->w)/src->frames)*frame;
    srcRect.y = 0;
    srcRect.w = (src->surface->w)/src->frames;
    srcRect.h = (src->surface->h);

    //Grab the frame of the window
    destRect.x = x;
    destRect.y = y;
    destRect.w = (src->surface->w)/src->frames;
    destRect.h = (src->surface->h);
    
    //Blit the final surface
    return SDL_BlitSurface(src->surface, &srcRect, game_surface, &destRect);
}

int fillRect(uint32_t color){
    return SDL_FillRect(SDL_GetWindowSurface(window), NULL, color);
}

int updateWindow(){
    SDL_Surface window_surface = SDL_GetWindowSurface(window);
    SDL_Rect rect;
    rect.x=0;
    rect.y=0;
    rect.w=window_surface->w;
    rect.h=window_surface->h;
    SDL_BlitSurface(game_surface, &rect, window_surface, &rect);
    SDL_BlitSurface(ui_surface, &rect, window_surface, &rect);
    return SDL_UpdateWindowSurface(window);
}

void destroyWindow(){
   SDL_DestroyWindow(window);
}


