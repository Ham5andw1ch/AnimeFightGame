#include <stdio.h>
#include "SDL.h"
#include "global.h"
#include "settings.h"

#include "joyState.h"

keybind testKeys[buttonCount] = {
//  { BIND_HAT, 0, 0, 1 },
//  { BIND_HAT, 0, 0, 4 },
//  { BIND_HAT, 0, 0, 8 },
//  { BIND_HAT, 0, 0, 2 },
    { BIND_AXIS, 0, 1, -2000},
    { BIND_AXIS, 0, 1, 2000},
    { BIND_AXIS, 0, 0, -2000},
    { BIND_AXIS, 0, 0, 2000},
    { BIND_BUTTON, 0, 2, 0 },
    { BIND_BUTTON, 0, 3, 0 },
    { BIND_BUTTON, 0, 5, 0 },
    { BIND_BUTTON, 0, 0, 0 },
};

SDL_Joystick *joystick[MaxJoysticks];
uint8_t joystate[buttonCount];

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
                return 1; // SDL error
            }
            return 0;
        }
    }
    return 2; // No more open spots
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
    for(int i = 0; i < buttonCount; ++i)
    {
        if(testKeys[i].type != BIND_AXIS) continue;
        if(testKeys[i].joy != jaxis->which) continue;
        if(testKeys[i].ind != jaxis->axis) continue;
        if(testKeys[i].threshold >= 0)
        {
            if(jaxis->value >= testKeys[i].threshold)
                joystate[i] = joystate[i] == 0 ? 1 : 2;
            else
                joystate[i] = joystate[i] == 2 ? 3 : 0;
        }
        else
        {
            if(jaxis->value <= testKeys[i].threshold)
                joystate[i] = joystate[i] == 0 ? 1 : 2;
            else
                joystate[i] = joystate[i] == 2 ? 3 : 0;
        }
    }
    return 0;
}

int updateStateBut(SDL_JoyButtonEvent* jbutton)
{
    if(jbutton->type == SDL_JOYBUTTONDOWN)
    {
        dbgprint("%d: Button %d down on Joystick %d: %d\n", jbutton->timestamp,
                jbutton->button, jbutton->which, jbutton->state);

        for(int i = 0; i < buttonCount; ++i)
        {
            if(testKeys[i].type != BIND_BUTTON) continue;
            if(testKeys[i].joy != jbutton->which) continue;
            if(testKeys[i].ind != jbutton->button) continue;
            joystate[i] = 1; break;
        }
    }
    else
    {
        dbgprint("%d: Button %d up on Joystick %d: %d\n", jbutton->timestamp,
                jbutton->button, jbutton->which, jbutton->state);

        for(int i = 0; i < buttonCount; ++i)
        {
            if(testKeys[i].type != BIND_BUTTON) continue;
            if(testKeys[i].joy != jbutton->which) continue;
            if(testKeys[i].ind != jbutton->button) continue;
            joystate[i] = 3; break;
        }
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
    for(int i = 0; i < buttonCount; ++i)
    {
        if(testKeys[i].type != BIND_HAT) continue;
        if(testKeys[i].joy != jhat->which) continue;
        if(testKeys[i].ind != jhat->hat) continue;
        if((testKeys[i].threshold & jhat->value) > 0)
            joystate[i] = joystate[i] == 0 ? 1 : 2;
        else
            joystate[i] = joystate[i] == 2 ? 3 : 0;
    }
    return 0;
}

int updateStateKey(SDL_KeyboardEvent* key)
{
    if(key->type == SDL_KEYDOWN)
    {
        dbgprint("%d: %s key pressed\n", key->timestamp, SDL_GetKeyName(key->keysym.sym));
        for(int i = 0; i < buttonCount; ++i)
        {
            if(testKeys[i].type != BIND_KEYBOARD) continue;
            if(testKeys[i].ind != key->keysym.sym) continue;
            joystate[i] = 1; break;
        }
    }
    else
    {
        dbgprint("%d: %s key released\n", key->timestamp, SDL_GetKeyName(key->keysym.sym));
        for(int i = 0; i < buttonCount; ++i)
        {
            if(testKeys[i].type != BIND_KEYBOARD) continue;
            if(testKeys[i].ind != key->keysym.sym) continue;
            joystate[i] = 3; break;
        }
    }
    return 0;
}

void joyInit()
{
//  for(int i = 0; i < SDL_NumJoysticks() && i < MaxJoysticks; ++i)
//  {
//      dbgprint("Joystick %d Added\n", i);
//      addJoy(i);
//  }

    memset(joystick, (size_t)NULL, MaxJoysticks * sizeof(*joystick));
    memset(joystate, 0, buttonCount * sizeof(*joystate));
}

void joyUpdate()
{
    // Remove the one-frame button states
    for(int i = 0; i < buttonCount; ++i)
    {
        if(joystate[i] % 2 == 1)
            joystate[i] = (joystate[i] + 1) % 4;
    }

    dbgprint("[%d %d %d %d %d %d %d %d]\n", joystate[0], joystate[1], joystate[2],
            joystate[3], joystate[4], joystate[5], joystate[6], joystate[7]); 
}

void joyEvent(SDL_Event e)
{
    if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
        updateStateKey(&e.key);
    if(e.type == SDL_JOYAXISMOTION)
        updateStateAxi(&e.jaxis);
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
