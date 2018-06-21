#ifndef JOYSTATE_H_
#define JOYSTATE_H_

#include "settings.h"

enum bindType
{
    BIND_KEYBOARD, BIND_AXIS, BIND_BUTTON, BIND_HAT
};

typedef struct
{
    enum bindType type; //keyboard, axis, button, etc.
    int joy;  //if not keyboard, the joystick
    int ind;  //index of the axis, button, etc. or keycode
    int threshold; //if an axis, the threshold to count as pushed, takes sign
} keybind;

void joyInit();
void joyUpdate();
void joyEvent(SDL_Event);
void joyRip();

#endif
