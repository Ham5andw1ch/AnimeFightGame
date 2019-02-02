#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "settings.h"
#include "joyState.h"
#include "parse.h"
#include "display.h"

#include "animage.h"

typedef int (*upfunc_t)(void);

int testScene(void);

upfunc_t upfuncs[] = {
    testScene
};

SDL_Rect P1, P2;

//TODO: SHITTY CODE
drawable_t* back_d;
drawable_t* ryu_d;
drawable_t* fire_d;
drawable_t* ui_d;
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
    ret = updateViewport(&P1, &P1);
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
    struct sprite *fire_spr  = malloc(sizeof(struct sprite));
    struct palette* fire_pal = malloc(sizeof(struct palette));
    struct sprite *ui_spr    = malloc(sizeof(struct sprite));
    struct palette* ui_pal   = malloc(sizeof(struct palette));

    readSprite(argv[1], back_spr);
    readPalette(argv[2], back_pal);
    readSprite(argv[3], ryu_spr);
    readPalette(argv[4], ryu_pal);
    readSprite(argv[5], fire_spr);
    readPalette(argv[6], fire_pal);
    readSprite(argv[7], ui_spr);
    readPalette(argv[8], ui_pal);

    sprite_t* back = createSprite(back_spr, back_pal, 1);
    sprite_t* ryu = createSprite(ryu_spr, ryu_pal, 10);
    sprite_t* fire = createSprite(fire_spr, fire_pal, 39);
    sprite_t* ui = createSprite(ui_spr, ui_pal, 1);

    back_d = drawFromSprite(back, 0, 0, 0, NULL, GAME);
    ryu_d = drawFromSprite(ryu, 100, 100, 1, NULL, GAME);
    fire_d = drawFromSprite(fire, 300, 200, 2, ryu_d, GAME);
    ui_d = drawFromSprite(ui, 100, 100, 0, NULL, UI);
    //END TODO

    while(ret == 0)
    {
        ret = game_loop();
    }
    if(ret != -1)
        return ret;

    //TODO: MORE SHITTY CODE:
    free(back);
    free(ryu);
    free(fire);
    free(ui);

    free(back_spr);
    free(back_pal);
    free(ryu_spr);
    free(ryu_pal);
    free(fire_spr);
    free(fire_pal);
    free(ui_spr);
    free(ui_pal);
    //END SHITTY CODE
    
    if((ret = teardown()))
        return ret;
}

int testScene(void)
{
    P1.x = ryu_d->x;
    P1.y = ryu_d->y;
    return 0;
}
