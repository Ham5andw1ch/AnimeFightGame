#include <stdio.h>
#include "SDL.h"
#include "global.h"
#include "settings.h"

#include "joyState.h"

keybind keysP1[buttonCount] = {
    { BIND_HAT, 0, 0, 1 },
    { BIND_HAT, 0, 0, 4 },
    { BIND_HAT, 0, 0, 8 },
    { BIND_HAT, 0, 0, 2 },
//  { BIND_AXIS, 0, 1, -2000},
//  { BIND_AXIS, 0, 1, 2000},
//  { BIND_AXIS, 0, 0, -2000},
//  { BIND_AXIS, 0, 0, 2000},
    { BIND_BUTTON, 0, 2, 0 },
    { BIND_BUTTON, 0, 3, 0 },
    { BIND_BUTTON, 0, 5, 0 },
    { BIND_BUTTON, 0, 0, 0 },
};

keybind keysP2[buttonCount] = {
    { BIND_HAT, 1, 0, 1 },
    { BIND_HAT, 1, 0, 4 },
    { BIND_HAT, 1, 0, 8 },
    { BIND_HAT, 1, 0, 2 },
//  { BIND_AXIS, 1, 1, -2000},
//  { BIND_AXIS, 1, 1, 2000},
//  { BIND_AXIS, 1, 0, -2000},
//  { BIND_AXIS, 1, 0, 2000},
    { BIND_BUTTON, 1, 2, 0 },
    { BIND_BUTTON, 1, 3, 0 },
    { BIND_BUTTON, 1, 5, 0 },
    { BIND_BUTTON, 1, 0, 0 },
};

SDL_Joystick *joystick[MaxJoysticks];
uint8_t joystatep1[buttonCount];
uint8_t joystatep2[buttonCount];

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
        if(joystick[i] != NULL && SDL_JoystickInstanceID(joystick[i]))
        {
            SDL_JoystickClose(joystick[i]);
            joystick[i] = NULL;
            return;
        }
    }
}


int updateStateAxi(SDL_JoyAxisEvent* jaxis)
{
  //  dbgprint("%d: Axis %d on Joystick %d: %d\n", jaxis->timestamp,
  //          jaxis->axis, jaxis->which, jaxis->value);
    for(int i = 0; i < buttonCount; ++i)
    {
        if(keysP1[i].type != BIND_AXIS) continue;
        if(keysP1[i].joy != jaxis->which) continue;
        if(keysP1[i].ind != jaxis->axis) continue;
        if(keysP1[i].threshold >= 0)
        {
            if(jaxis->value >= keysP1[i].threshold)
                joystatep1[i] = joystatep1[i] == 0 ? 1 : 2;
            else
                joystatep1[i] = joystatep1[i] == 2 ? 3 : 0;
        }
        else
        {
            if(jaxis->value <= keysP1[i].threshold)
                joystatep1[i] = joystatep1[i] == 0 ? 1 : 2;
            else
                joystatep1[i] = joystatep1[i] == 2 ? 3 : 0;
        }
    }

    for(int i = 0; i < buttonCount; ++i)
    {
        if(keysP2[i].type != BIND_AXIS) continue;
        if(keysP2[i].joy != jaxis->which) continue;
        if(keysP2[i].ind != jaxis->axis) continue;
        if(keysP2[i].threshold >= 0)
        {
            if(jaxis->value >= keysP1[i].threshold)
                joystatep2[i] = joystatep2[i] == 0 ? 1 : 2;
            else
                joystatep2[i] = joystatep2[i] == 2 ? 3 : 0;
        }
        else
        {
            if(jaxis->value <= keysP1[i].threshold)
                joystatep2[i] = joystatep2[i] == 0 ? 1 : 2;
            else
                joystatep2[i] = joystatep2[i] == 2 ? 3 : 0;
        }
    }
    return 0;
}

int updateStateBut(SDL_JoyButtonEvent* jbutton)
{
    {
      //  dbgprint("%d: Button %d down on Joystick %d: %d\n", jbutton->timestamp,
      //          jbutton->button, jbutton->which, jbutton->state);

        for(int i = 0; i < buttonCount; ++i)
        {
            if(keysP1[i].type != BIND_BUTTON) continue;
            if(keysP1[i].joy != jbutton->which) continue;
            if(keysP1[i].ind != jbutton->button) continue;
            if(jbutton->type == SDL_JOYBUTTONDOWN)
                joystatep1[i] = 1;
            else
                joystatep1[i] = 3;
        }
        for(int i = 0; i < buttonCount; ++i)
        {
            if(keysP2[i].type != BIND_BUTTON) continue;
            if(keysP2[i].joy != jbutton->which) continue;
            if(keysP2[i].ind != jbutton->button) continue;
            if(jbutton->type == SDL_JOYBUTTONDOWN)
                joystatep2[i] = 1;
            else
                joystatep2[i] = 3;
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
 //   dbgprint("%d: POV Hat %d on Joystick %d: %d\n", jhat->timestamp,
   //         jhat->hat, jhat->which, jhat->value);
    for(int i = 0; i < buttonCount; ++i)
    {
        if(keysP1[i].type != BIND_HAT) continue;
        if(keysP1[i].joy != jhat->which) continue;
        if(keysP1[i].ind != jhat->hat) continue;
        if((keysP1[i].threshold & jhat->value) > 0)
            joystatep1[i] = joystatep1[i] == 0 ? 1 : 2;
        else
            joystatep1[i] = joystatep1[i] == 2 ? 3 : 0;
    }

    for(int i = 0; i < buttonCount; ++i)
    {
        if(keysP2[i].type != BIND_HAT) continue;
        if(keysP2[i].joy != jhat->which) continue;
        if(keysP2[i].ind != jhat->hat) continue;
        if((keysP2[i].threshold & jhat->value) > 0)
            joystatep2[i] = joystatep2[i] == 0 ? 1 : 2;
        else
            joystatep2[i] = joystatep2[i] == 2 ? 3 : 0;
    }
    return 0;
}

int updateStateKey(SDL_KeyboardEvent* key)
{
  //  dbgprint("%d: %s key pressed\n", key->timestamp, SDL_GetKeyName(key->keysym.sym));
    for(int i = 0; i < buttonCount; ++i)
    {
        if(keysP1[i].type != BIND_KEYBOARD) continue;
        if(keysP1[i].ind != key->keysym.sym) continue;
        if(key->type == SDL_KEYDOWN)
            joystatep1[i] = 1;
        else
            joystatep1[i] = 3;
    }

    for(int i = 0; i < buttonCount; ++i)
    {
        if(keysP2[i].type != BIND_KEYBOARD) continue;
        if(keysP2[i].ind != key->keysym.sym) continue;
        if(key->type == SDL_KEYDOWN)
            joystatep2[i] = 1;
        else
            joystatep2[i] = 3;
    }
    return 0;
}

void joyInit(void)
{
//  for(int i = 0; i < SDL_NumJoysticks() && i < MaxJoysticks; ++i)
//  {
//      dbgprint("Joystick %d Added\n", i);
//      addJoy(i);
//  }

    memset(joystick, (size_t)NULL, MaxJoysticks * sizeof(*joystick));
    memset(joystatep1, 0, buttonCount * sizeof(*joystatep1));
    memset(joystatep2, 0, buttonCount * sizeof(*joystatep2));
}

void joyUpdate(void)
{
    // Remove the one-frame button states
    for(int i = 0; i < buttonCount; ++i)
    {
        if(joystatep1[i] % 2 == 1)
            joystatep1[i] = (joystatep1[i] + 1) % 4;
        if(joystatep2[i] % 2 == 1)
            joystatep2[i] = (joystatep2[i] + 1) % 4;
    }

    //dbgprint("[%d %d %d %d %d %d %d %d]\t", joystatep1[0], joystatep1[1], joystatep1[2],
    //        joystatep1[3], joystatep1[4], joystatep1[5], joystatep1[6], joystatep1[7]); 
    //dbgprint("[%d %d %d %d %d %d %d %d]\n", joystatep2[0], joystatep2[1], joystatep2[2],
    //        joystatep2[3], joystatep2[4], joystatep2[5], joystatep2[6], joystatep2[7]); 
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

void joyRip(void)
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

uint8_t* joyStatep1(uint8_t* out)
{
    memcpy(out, joystatep1, buttonCount * sizeof(*out));
    return out;
}

uint8_t* joyStatep2(uint8_t* out)
{
    memcpy(out, joystatep2, buttonCount * sizeof(*out));
    return out;
}
