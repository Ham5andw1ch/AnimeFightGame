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

SDL_Rect viewport;

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

int updateViewport(SDL_Rect* p1, SDL_Rect* p2)
{
    int window_w, window_h;   
    SDL_GetWindowSize(window, &window_w, &window_h);
    double k = (double)window_w / (double)window_h;
    int dx = abs(p1->x - p2->x);
    int dy = abs(p1->y - p2->y);
    int mx = ((2 * p1->x + p1->w) + (2 * p2->x + p2->w)) / 4;
    int my = ((2 * p1->y + p1->h) + (2 * p2->y + p2->y)) / 4;
    int w = min(min(max(max(dx + 2 * CamOffset, k * (dy + 2 * CamOffset)), Min_W), Max_W), k * Max_H);
    int h = min(min(max(max(dy + 2 * CamOffset, (1.0 / k) * (dx + 2 * CamOffset)), Min_H), Max_H), (1.0 / k) * Max_W);

    int y = 0;
    if(FollowPriority == FOLLOW_LOW)
        y = min(max(p1->y + p1->h, p2->y + p2->h), FloorY) + CamOffset - h;
    else
        y = max(min(p1->y, p2->y), 0);

    int x = 0;
    if(2 * mx - w < 2 * (LeftWall - CamOffset))
        x = LeftWall - CamOffset;
    else if(2 * mx + w > 2 * (RightWall + CamOffset))
        x = RightWall + CamOffset - w;
    else
        x = mx - (w / 2);
    
    dbgprint("Real Aspect Ratio: %lf, Calculated Aspect Ratio: %lf\n", k, (double)w / (double)h);
    viewport.x = x;
    viewport.y = y;
    viewport.w = w;
    viewport.h = h;
    return 0;
}

void destroyWindow(){
   SDL_DestroyWindow(window);
}


