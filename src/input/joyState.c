#include <stdio.h>
#include "SDL.h"
#include "global.h"
#include "settings.h"

#include "joyState.h"

keybind keys[PlayerCount][ButtonCount] = DefaultKeybinds;

int nextJoyID = 0;
SDL_Joystick *joystick[MaxJoysticks];
uint8_t joystate[PlayerCount][ButtonCount];

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
            nextJoyID = SDL_JoystickInstanceID(joystick[i]) + 1; // auto-rebind controller
            return 0;
        }
    }
    return 2; // No more open spots
}

void remJoy(int index)
{
    for(int i = 0; i < MaxJoysticks; ++i)
    {
        if(joystick[i] != NULL && SDL_JoystickInstanceID(joystick[i]) == index)
        {
            int joyID = SDL_JoystickInstanceID(joystick[i]);
            SDL_JoystickClose(joystick[i]);
            joystick[i] = NULL;
            // Code for auto-rebind controller. A joystick instance ID is never reused,
            // so this moves all keybinds that use that ID to the next one to be added.
            for(int p = 0; p < PlayerCount; ++p)
            {
                for(int b = 0; b < ButtonCount; ++b)
                {
                    // If the next Joystick instance ID already has matching keybinds,
                    // e.g. if only a single stick is added and then removed and there are
                    // two sets of keybinds mapped by default, bump those up one.
                    if(keys[p][b].type != BIND_KEYBOARD && keys[p][b].joy == nextJoyID)
                        keys[p][b].joy = nextJoyID + 1;
                    // Set it to the next Joystick instance ID that will be added.
                    if(keys[p][b].type != BIND_KEYBOARD && keys[p][b].joy == joyID)
                        keys[p][b].joy = nextJoyID;
                }
            }
            return;
        }
    }
}


int updateStateAxi(SDL_JoyAxisEvent* jaxis)
{
  //  dbgprint("%d: Axis %d on Joystick %d: %d\n", jaxis->timestamp,
  //          jaxis->axis, jaxis->which, jaxis->value);
    for(int p = 0; p < PlayerCount; ++p)
    {
        for(int i = 0; i < ButtonCount; ++i)
        {
            if(keys[p][i].type != BIND_AXIS) continue;
            if(keys[p][i].joy != jaxis->which) continue;
            if(keys[p][i].ind != jaxis->axis) continue;
            if(keys[p][i].threshold >= 0)
            {
                if(jaxis->value >= keys[p][i].threshold)
                    joystate[p][i] = joystate[p][i] == 0 ? 1 : 2;
                else
                    joystate[p][i] = joystate[p][i] == 2 ? 3 : 0;
            }
            else
            {
                if(jaxis->value <= keys[p][i].threshold)
                    joystate[p][i] = joystate[p][i] == 0 ? 1 : 2;
                else
                    joystate[p][i] = joystate[p][i] == 2 ? 3 : 0;
            }
        }
    }
    return 0;
}

int updateStateBut(SDL_JoyButtonEvent* jbutton)
{
    //  dbgprint("%d: Button %d down on Joystick %d: %d\n", jbutton->timestamp,
    //          jbutton->button, jbutton->which, jbutton->state);

    for(int p = 0; p < PlayerCount; ++p)
    {
        for(int i = 0; i < ButtonCount; ++i)
        {
            if(keys[p][i].type != BIND_BUTTON) continue;
            if(keys[p][i].joy != jbutton->which) continue;
            if(keys[p][i].ind != jbutton->button) continue;
            if(jbutton->type == SDL_JOYBUTTONDOWN)
                joystate[p][i] = 1;
            else
                joystate[p][i] = 3;
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
    for(int p = 0; p < PlayerCount; ++p)
    {
        for(int i = 0; i < ButtonCount; ++i)
        {
            if(keys[p][i].type != BIND_HAT) continue;
            if(keys[p][i].joy != jhat->which) continue;
            if(keys[p][i].ind != jhat->hat) continue;
            if((keys[p][i].threshold & jhat->value) > 0)
                joystate[p][i] = joystate[p][i] == 0 ? 1 : 2;
            else
                joystate[p][i] = joystate[p][i] == 2 ? 3 : 0;
        }
    }
    return 0;
}

int updateStateKey(SDL_KeyboardEvent* key)
{
  //  dbgprint("%d: %s key pressed\n", key->timestamp, SDL_GetKeyName(key->keysym.sym));
    for(int p = 0; p < PlayerCount; ++p)
    {
        for(int i = 0; i < ButtonCount; ++i)
        {
            if(keys[p][i].type != BIND_KEYBOARD) continue;
            if(keys[p][i].ind != key->keysym.sym) continue;
            if(key->type == SDL_KEYDOWN)
                joystate[p][i] = 1;
            else
                joystate[p][i] = 3;
        }
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
    memset(joystate, 0, PlayerCount * ButtonCount * sizeof(*joystate));
}

void joyUpdate(void)
{
    // Remove the one-frame button states
    for(int p = 0; p < PlayerCount; ++p)
    {
        for(int i = 0; i < ButtonCount; ++i)
        {
            if(joystate[p][i] % 2 == 1)
                joystate[p][i] = (joystate[p][i] + 1) % 4;
        }
    }

    //dbgprint("[%d %d %d %d %d %d %d %d]\t", joystatep1[0], joystatep1[1], joystatep1[2],
    //        joystatep1[3], joystatep1[4], joystatep1[5], joystatep1[6], joystatep1[7]); 
    //dbgprint("[%d %d %d %d %d %d %d %d]\n", joystatep2[0], joystatep2[1], joystatep2[2],
    //        joystatep2[3], joystatep2[4], joystatep2[5], joystatep2[6], joystatep2[7]); 
}

void joyEvent(SDL_Event* e)
{
    if(e->type == SDL_KEYDOWN || e->type == SDL_KEYUP)
        updateStateKey(&e->key);
    if(e->type == SDL_JOYAXISMOTION)
        updateStateAxi(&e->jaxis);
    if(e->type == SDL_JOYHATMOTION)
        updateStateHat(&e->jhat);
    if(e->type == SDL_JOYBUTTONDOWN || e->type == SDL_JOYBUTTONUP)
        updateStateBut(&e->jbutton);
    if(e->type == SDL_JOYDEVICEADDED || e->type == SDL_JOYDEVICEREMOVED)
        updateStateDev(&e->jdevice);
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

uint8_t* joyState(int player, uint8_t* out)
{
    memcpy(out, joystate[player], ButtonCount * sizeof(*out));
    return out;
}
