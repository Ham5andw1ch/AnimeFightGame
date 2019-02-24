#include <stdio.h>
#include <stdlib.h>

#include "SDL2/SDL.h"

#include "global.h"
#include "settings.h"
#include "joyState.h"
#include "parse.h"
#include "display.h"

#include "animage.h"
Uint64 NOW = 0;
Uint64 LAST = 0;
typedef int (*upfunc_t)(void);

int testScene(void);

upfunc_t upfuncs[] = {
    testScene
};

SDL_Rect P1, P2;

//TODO: SHITTY CODE
drawable_t* back_d;
drawable_t* ryu_d;
drawable_t* ryu2_d;
//END SHITTY CODE

int game_loop(void)
{
    int ret;
    // Input Updates
    if((ret = joyUpdate()))
    {
        return -1;
    }
    parserUpdate();

    // Game Logic
    int funcs = sizeof(upfuncs) / sizeof(upfuncs[0]);
    for(int i = 0; i < funcs; ++i)
    {
        if((ret = (*upfuncs)()))
        {
            errprint("Update Function %d returned %d\n", i, ret);
            return ret;
        }
    }

    // Drawing
    ret = updateViewport(&P1, &P2);
    if(ret)
    {
        errprint("Update Viewport Function returned %d\n", ret);
        return ret;
    }

    uint8_t* sp1 = malloc((ButtonCount + MacroCount + 1) * sizeof(*sp1));
    sp1 = joyState(0, sp1);
    if(sp1[2]==1||sp1[2]==2){
        ryu_d->x-=5;
    }
    if(sp1[3]==1||sp1[3]==2){
        ryu_d->x+=5;
    }
    drawGame();
    drawUI();
    ret = updateWindow();
    if(ret)
    {
        errprint("Update Window Function returned %d\n", ret);
        return ret;
    }

    // Audio
    return 0;
}

int initialize(void)
{
    int ret;
    if((ret = SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_JOYSTICK|SDL_INIT_EVENTS)))
    {
        errprint("Failure initializing SDL: %s\n", SDL_GetError());
        return ret;
    }
    joyInit();
    initQueues();
    SDL_Rect bounds;
    if((ret = SDL_GetDisplayUsableBounds(0, &bounds)))
    {
        errprint("Unable to find display bounds: %s\n", SDL_GetError());
        return ret;
    }
    makeWindow(bounds.w, bounds.h, "Fight Game");
    initDisplay();
    P1.x = 300; P1.y = 300;
    return 0;
}

int teardown(void)
{
    destroyWindow();
    joyRip();
    SDL_Quit();
    return 0;
}

int main(int argc, char** argv)
{
    int ret = 0;
    if((ret = initialize()))
        return ret;

    //TODO: REMOVE THIS SHITTY BLOCK:
    struct sprite *back_spr  = malloc(sizeof(struct sprite));
    struct palette* back_pal = malloc(sizeof(struct palette));
    struct sprite *ryu_spr   = malloc(sizeof(struct sprite));
    struct palette* ryu_pal  = malloc(sizeof(struct palette));

    readSprite(argv[1], back_spr);
    readPalette(argv[2], back_pal);
    readSprite(argv[3], ryu_spr);
    readPalette(argv[4], ryu_pal);

    sprite_t* back = createSprite(back_spr, back_pal, 1);
    sprite_t* ryu = createSprite(ryu_spr, ryu_pal, 1);

    back_d = drawFromSprite(back, 0, 0, 0, 2, NULL, GAME);
    ryu_d = drawFromSprite(ryu, 1300, 900, 1, 0,  NULL, GAME);
    ryu2_d = drawFromSprite(ryu, 900, 900, 1, 1, NULL, GAME);
    //END TODO

    while(ret == 0)
    {
        LAST = SDL_GetPerformanceCounter();
        ret = game_loop();
        NOW = SDL_GetPerformanceCounter();
        printf("%d\n",SDL_GetPerformanceFrequency());
    }
    if(ret != -1)
        return ret;

    //TODO: MORE SHITTY CODE:
    free(back);
    free(ryu);

    free(back_spr);
    free(back_pal);
    free(ryu_spr);
    free(ryu_pal);
    //END SHITTY CODE

    if((ret = teardown()))
        return ret;
}

int testScene(void)
{
    //ryu_d->x++;
    //ryu2_d->y--;
    P1.x = ryu_d->x;
    P1.y = ryu_d->y;
    P1.w = ryu_d->sprite->surface->w / ryu_d->sprite->frames;
    P1.h = ryu_d->sprite->surface->h;
    P2.x = ryu2_d->x;
    P2.y = ryu2_d->y;
    P2.w = ryu2_d->sprite->surface->w / ryu2_d->sprite->frames;
    P2.h = ryu2_d->sprite->surface->h;
    return 0;
}
