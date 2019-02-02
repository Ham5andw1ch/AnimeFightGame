//This file is frontend to sidestep most of the work handling SDL_CreateWindow boilerplate.
//
//Sprites are handled as a struct containing the number of animation frames and the SDL_Surface created
//  elsewhere.
//
//There is no need for the user to define rectangles to display, but they are required to wash the screen
//  because this class simply handles the blitting.

#include <stdio.h>
#include <math.h>
#include "SDL2/SDL.h"
#include "global.h"
#include "settings.h"
#include "display.h"
#include "animage.h"

SDL_Window *window;
SDL_Surface *game_surface;
SDL_Surface *ui_surface;

SDL_Rect viewport;
// used as a base case for recursive relative position calculation
drawable_t treeRoot = {
    0, // x
    0, // y
    0, // z_index
    0, // current_frame
    0, // cached_x
    0, // cached_y
    1, // calculated
    NULL, // sprite
    NULL, // parent
    GAME  // layer
};

int initDisplay()
{
    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
    game_surface = SDL_CreateRGBSurface(0, Game_W, Game_H, 32, format->Rmask, format->Gmask, format->Bmask, format->Amask);
    int win_w, win_h;
    SDL_GetWindowSize(window, &win_w, &win_h);
    ui_surface = SDL_CreateRGBSurface(0, win_w, win_h, 32, format->Rmask, format->Gmask, format->Bmask, format->Amask);
    return 0;
}

SDL_Window *makeWindow(uint16_t x, uint16_t y, char* name){
    window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, x, y, SDL_WINDOW_FULLSCREEN);
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
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);
    SDL_Rect rect;
    rect.x=0;
    rect.y=0;
    rect.w=window_surface->w;
    rect.h=window_surface->h;
    if(SDL_BlitSurface(game_surface, &rect, window_surface, &rect))
        errprint("%s\n", SDL_GetError());
    if(SDL_BlitSurface(ui_surface, &rect, window_surface, &rect))
        errprint("%s\n", SDL_GetError());
    return SDL_UpdateWindowSurface(window);
}

sprite_t* createSprite(struct sprite* spr, struct palette* pal, int num_frames){
    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
    uint8_t* pixels = malloc(4* spr->height*spr->width * sizeof(*pixels));

    int depth, pitch;
    depth = 32;
    pitch = 4*spr->width;
    
    int i =0;
    //Fill pixels with the actual sprite data
    for(int row = 0; row < spr->height; ++row)
    {	
        for(int col = 0; col < spr->width; ++col)
        {
            uint8_t r,g,b,a;
            r = pal->srgb[spr->colors[row * spr->width + col].color].red;
            g = pal->srgb[spr->colors[row * spr->width + col].color].green;
            b = pal->srgb[spr->colors[row * spr->width + col].color].blue;
            a = spr->colors[row * spr->width + col].alpha;
            pixels[i] =r;
            pixels[i+1] =g;
            pixels[i+2] =b;
            pixels[i+3] =a;
//          printf("%i %i %i %i\n", pixels[i], pixels[i+1], pixels[i+2], pixels[i+3]);
	    i+=4;
        }
    }
    
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, spr->width, spr->height,depth, pitch, format->Rmask, format->Gmask, format->Bmask, format->Amask);
    sprite_t* sprite = malloc(sizeof(*sprite));
    sprite->frames = num_frames;
    sprite->surface = surface;
    free(pixels);
    SDL_FreeFormat(format);
    return sprite;
}


drawable_t* drawFromSprite(sprite_t* spr, int x, int y, int z_ind, drawable_t* parent, int layer){
    drawable_t* ret = malloc(sizeof(*ret));
    ret -> x = x;
    ret -> y = y;
    ret -> z_index = z_ind;
    ret -> current_frame = 0;
    ret -> sprite = spr;
    if(layer == UI){
        ret -> parent = NULL;
        insertDispNode(ret, &ui_displayables);
    }else if(layer == GAME){
        if(parent == NULL)
            ret -> parent = &treeRoot;
        else
            ret -> parent = parent;
        insertDispNode(ret, &game_displayables);
    } else {
        return NULL;
    }
    return ret;
}

void freeSprite(sprite_t* sprite){
    SDL_FreeSurface(sprite->surface);
    free(sprite);
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

void absolutePos(drawable_t* drawable, int* x, int* y)
{
    if(drawable->calculated)
    {
        *x = drawable->cached_x;
        *y = drawable->cached_y;
        return;
    }

    absolutePos(drawable->parent, x, y);
    *x += drawable->x;
    *y += drawable->y;
    drawable->cached_x = *x;
    drawable->cached_y = *y;
    drawable->calculated = 1;
}

void drawGame()
{
    dbgprint("Entered drawGame()\n");
    // iterate through the game displayables, calculating their absolute position and blitting them
    // to the game surface
    for(disp_node_t* node = game_displayables; node != NULL; node = node->next)
    {
        int x, y;
        absolutePos(node->drawable, &x, &y);
        if(blitSprite(node->drawable->sprite, x, y, node->drawable->current_frame))
            errprint("%s\n", SDL_GetError());
        dbgprint("Draw sprite at (%d, %d) on frame %d\n", x, y, node->drawable->current_frame);
    }

    // reset the calculated bit for the next frame
    for(disp_node_t* node = game_displayables; node != NULL; node = node->next)
    {
        node->drawable->calculated = 0;
    }
}

void drawUI()
{
    for(disp_node_t* node = ui_displayables; node != NULL; node = node->next)
    {
        SDL_Rect pos;
        pos.x = node->drawable->x;
        pos.y = node->drawable->y;
        SDL_BlitSurface(node->drawable->sprite->surface, NULL, ui_surface, &pos);
    }
}

// Returns the id of the inserted drawable node
int insertDispNode(drawable_t* draw, disp_node_t** list){
    dbgprint("Entered insertDispNode()\n");
    disp_node_t** pointer = list;
    disp_node_t* node = malloc(sizeof(*node));
    node->id = id_acc++;
    node->drawable = draw;
    node->next = NULL;
    uint_fast8_t done = 0;
    while(!done){
        if(*pointer == NULL){
            *pointer = node;
            done = 1;
        }else if(node->drawable->z_index <= (*pointer)->drawable->z_index){
            node->next = (*pointer);
            (*pointer) = node;
            done = 1;
        }else{
            pointer = &(*pointer)->next;
        }
    }
    return node->id;
}

// remove the node with the id
drawable_t* removeDispNode(uint64_t id, disp_node_t** list){
    disp_node_t** pointer = list;
    while(true){
        if(*pointer == NULL){
            return NULL;
        }else if((*pointer)->id == id){
            disp_node_t* old= (*pointer);
            (*pointer) = (*pointer)->next;
            drawable_t* out = old->drawable;
            free(old);
            return out;
        }else{
            pointer = &(*pointer)->next;
        }
    }
}

void destroyWindow(){
   SDL_DestroyWindow(window);
}
