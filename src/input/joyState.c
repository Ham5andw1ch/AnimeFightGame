#include <stdio.h>
#include "SDL.h"
#include "global.h"

SDL_Joystick *joystick[MaxJoysticks];
int addJoy(int index)
{
    for(int i = 0; i < MaxJoysticks; ++i)
    {
        if(joystick[i] == NULL)
        {
            joystick[i] = SDL_JoystickOpen(index);
            if(!joystick[i])
                errprint("Couldn't open Joystick: %s\n", SDL_GetError());
        }
    }
}

int updateStateAxi(SDL_JoyAxisEvent* jaxis)
{
    dbgprint("%d: Axis %d on Joystick %d: %d\n", jaxis->timestamp,
            jaxis->axis, jaxis->which, jaxis->value);
    return 0;
}

int updateStateBal(SDL_JoyBallEvent* jball)
{
    dbgprint("%d: Trackball %d on Joystick %d: (%d, %d)\n", jball->timestamp,
            jball->ball, jball->which, jball->xrel,
            jball->yrel);
    return 0;
}

int updateStateBut(SDL_JoyButtonEvent* jbutton)
{
    if(jbutton->type == SDL_JOYBUTTONDOWN)
    {
        dbgprint("%d: Button %d down on Joystick %d: %d\n", jbutton->timestamp,
                jbutton->button, jbutton->which, jbutton->state);
    }
    else
    {
        dbgprint("%d: Button %d up on Joystick %d: %d\n", jbutton->timestamp,
                jbutton->button, jbutton->which, jbutton->state);
    }
    return 0;
}

int updateStateDev(SDL_JoyDeviceEvent* jdevice)
{
    if(jdevice->type == SDL_JOYDEVICEADDED)
    {
        dbgprint("%d: Joystick %d Added\n", jdevice->timestamp, jdevice->which);
    }
    else
    {
        dbgprint("%d: Joystick %d Removed\n", jdevice->timestamp, jdevice->which);
    }
    return 0;
}

int updateStateHat(SDL_JoyHatEvent* jhat)
{
    dbgprint("%d: POV Hat %d on Joystick %d: %d\n", jhat->timestamp,
            jhat->hat, jhat->which, jhat->value);
    return 0;
}

int updateStateKey(SDL_KeyboardEvent* key)
{
    if(key->type == SDL_KEYDOWN)
    {
        dbgprint("%d: %s key pressed\n", key->timestamp, SDL_GetKeyName(key->keysym.sym));
    }
    else
    {
        dbgprint("%d: %s key released\n", key->timestamp, SDL_GetKeyName(key->keysym.sym));
    }
    return 0;
}
