#include <stdio.h>
#include "SDL2/SDL.h"
#include "global.h"
#include "joyState.h"
#include "parse.h"
#include "settings.h"
#include "animage.h"
Uint64 NOW = 0;
Uint64 LAST = 0;
double deltaTime = 0.0;



//Main Game Loop
int main(int argc, char **argv)
{
     //Check if we have the right number of args
    if(argc < 5)
    {
        fprintf(stderr, "Usage: %s [animage sprite] [animage palette] [Num Frames] [animage palette]\n", argv[0]);
        return 1;
    }   
    
    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }	
	

    //Animage initialization
    struct sprite* spr = malloc(sizeof(*spr));
    struct palette* pal = malloc(sizeof(*pal));
    struct palette* pal1 = malloc(sizeof(*pal1));
    int ret;
    if((ret = readSprite(argv[1], spr)))
        return ret;
    if((ret = readPalette(argv[2], pal)))
        return ret;
    if((ret = readPalette(argv[4], pal1)))
        return ret;
    //SDL Boilerplate
    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
    
    //Pixel pointers for our images
    uint8_t* pixelsA = malloc(4* spr->height*spr->width * sizeof(*pixelsA));
    uint8_t* pixelsB = malloc(4* spr->height*spr->width * sizeof(*pixelsB));
    

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
            pixelsA[i] =r;
            pixelsA[i+1] =g;
            pixelsA[i+2] =b;
            pixelsA[i+3] =a;
//          printf("%i %i %i %i\n", pixels[i], pixels[i+1], pixels[i+2], pixels[i+3]);
	    i+=4;
        }
    }
    i = 0;
    //Fill pixels with the actual sprite data
    for(int row = 0; row < spr->height; ++row)
    {	
        for(int col = 0; col < spr->width; ++col)
        {
            uint8_t r,g,b,a;
            r = pal1->srgb[spr->colors[row * spr->width + col].color].red;
            g = pal1->srgb[spr->colors[row * spr->width + col].color].green;
            b = pal1->srgb[spr->colors[row * spr->width + col].color].blue;
            a = spr->colors[row * spr->width + col].alpha;
            pixelsB[i] =r;
            pixelsB[i+1] =g;
            pixelsB[i+2] =b;
            pixelsB[i+3] =a;
//          printf("%i %i %i %i\n", pixels[i], pixels[i+1], pixels[i+2], pixels[i+3]);
	    i+=4;
        }
    }
    int num = atoi(argv[3]);
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixelsA, spr->width, spr->height,depth, pitch, format->Rmask, format->Gmask, format->Bmask, format->Amask);
    SDL_Surface* surface1 = SDL_CreateRGBSurfaceFrom(pixelsB, spr->width, spr->height,depth, pitch, format->Rmask, format->Gmask, format->Bmask, format->Amask);



    SDL_Window *window = SDL_CreateWindow("Anime Fight Game Test Main", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    
    if(window == NULL)
    {
        errprint("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    joyInit();
    initQueues();
    int y = 0;
    int x = 0;
    int button = 1;
    int flag = 0;
    i = 0;
    while(1)
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = deltaTime + (double) ((NOW-LAST)*1000/(double)SDL_GetPerformanceFrequency());
        if( deltaTime * .001 > (double)1/(double) FPS){
            deltaTime = 0;
            joyUpdate();
            parserUpdate();
            uint8_t* sp1 = malloc((ButtonCount + MacroCount + 1) * sizeof(*sp1));
            sp1 = joyState(0, sp1);
            if(flag!=1){
                i = 0;
                uint8_t input[] = {3,2,3,6};
                if(sp1[5]==1){
                    button = 2;
                    flag = searchInputHelper(&input,0,0);
                }else if(sp1[4]==1)
                {
                    button = 1;
                    flag = searchInputHelper(&input,0,0);
                }
            }
            dbgprint("[%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d]\n", x, y, flag, i, sp1[0], sp1[1],
                sp1[2], sp1[3], sp1[4], sp1[5],
                sp1[6], sp1[7], sp1[8], sp1[9],
                sp1[10], sp1[11]);
 
            if(sp1[1]==1||sp1[1]==2){
                y++;
            }
            if(sp1[0]==1||sp1[0]==2){
                y--;
            }
            if(sp1[2]==1||sp1[2]==2){
                x--;
            }
            if(sp1[3]==1||sp1[3]==2){
                x++;
            }
            
            SDL_Rect dest;
            dest.x =x;
            dest.y =y;
            dest.w = spr->width;
            dest.h = spr->height;
    
            
            SDL_Rect src;
            src.x =(spr->width/num)*i;
            src.y =0;
            src.w = spr->width/num;
            src.h = spr->height;

            SDL_FillRect(SDL_GetWindowSurface(window), NULL, 0xED9564FF);
            if(button == 1)
                SDL_BlitSurface(surface, &src, SDL_GetWindowSurface(window), &dest);
            if(button == 2)
                SDL_BlitSurface(surface1, &src, SDL_GetWindowSurface(window), &dest);
            //  SDL_RenderPresent(rend);
            SDL_UpdateWindowSurface(window);
            i++;
            if(i ==num){
                i=0;
                flag = 0;
            }
        }
            
            
        SDL_Event e;
        if(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
                break;
            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r)
                rebind(0, DIR_UP);
        }
        
    }
    
    joyRip();

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
