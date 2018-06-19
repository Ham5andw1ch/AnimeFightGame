#include <stdio.h>
#include "SDL.h"
#include "global.h"

SDL_Joystick *joystick[10];
int nextJoy = 0;

int joystickWatch(void* userdata, SDL_Event* event)
{
    // userdata will be something about controller mappings

    SDL_EventType type;
    switch(type = event->type)
    {
    default:
        dbgprint("stuff\n");
    }
}
