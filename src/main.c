#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "settings.h"
#include "joyState.h"
#include "parse.h"
#include "display.h"

typedef int (*upfunc_t)(void);

upfunc_t upfuncs[] = {

};

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
    ret = updateViewport(&(SDL_Rect){ 0,0,0,0 }, &(SDL_Rect){ 0, 0,0,0});
    if(ret)
    {
        errprint("Update Viewport Function returned %d\n", ret);
        return ret;
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
}

int teardown(void)
{
    joyRip();
    SDL_Quit();
}

int main(int argc, char** argv)
{
    int ret = 0;
    if((ret = initialize()))
        return ret;

    while(ret == 0)
    {
        ret = game_loop();
    }
    if(ret != -1)
        return ret;
    
    if((ret = teardown()))
        return ret;
}
