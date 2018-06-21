#include <stdio.h>
#include "SDL.h"
#include "global.h"
#include "settings.h"

SDL_Joystick *joystick[MaxJoysticks];

int addJoy(int index)
{
    for(int i = 0; i < MaxJoysticks; ++i)
    {
        if(joystick[i] == NULL)
        {
            joystick[i] = SDL_JoystickOpen(index);
            if(!joystick[i])
            {
                errprint("Couldn't open Joystick: %s\n", SDL_GetError());
                SDL_ClearError();
                return 1;
            }
            return 0;
        }
    }
}

void remJoy(int index)
{
    for(int i = 0; i < MaxJoysticks; ++i)
    {
        if(joystick[i] == NULL && SDL_JoystickInstanceID(joystick[i]))
        {
            SDL_JoystickClose(joystick[i]);
            joystick[i] = NULL;
            return;
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
        addJoy(jdevice->which);
    }
    else
    {
        dbgprint("%d: Joystick %d Removed\n", jdevice->timestamp, jdevice->which);
        remJoy(jdevice->which);
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

void joyInit()
{
    for(int i = 0; i < SDL_NumJoysticks() && i < MaxJoysticks; ++i)
    {
        dbgprint("Joystick %d Added\n", i);
        addJoy(i);
    }
}

void joyUpdate(SDL_Event e)
{
    if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
        updateStateKey(&e.key);
    if(e.type == SDL_JOYAXISMOTION)
        updateStateAxi(&e.jaxis);
    if(e.type == SDL_JOYBALLMOTION)
        updateStateBal(&e.jball);
    if(e.type == SDL_JOYHATMOTION)
        updateStateHat(&e.jhat);
    if(e.type == SDL_JOYBUTTONDOWN || e.type == SDL_JOYBUTTONUP)
        updateStateBut(&e.jbutton);
    if(e.type == SDL_JOYDEVICEADDED || e.type == SDL_JOYDEVICEREMOVED)
        updateStateDev(&e.jdevice);
}

void joyRip()
{
    for(int i = 0; i < MaxJoysticks; ++i)
    {
        if(joystick[i] != NULL && SDL_JoystickGetAttached(joystick[i]))
        {
            dbgprint("Joystick %d Removed\n", SDL_JoystickInstanceID(joystick[i]));
            SDL_JoystickClose(joystick[i]);
        }
    }
}

