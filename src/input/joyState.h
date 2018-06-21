#ifndef JOYSTATE_H_
#define JOYSTATE_H_

int updateStateAxi(SDL_JoyAxisEvent*);
int updateStateBal(SDL_JoyBallEvent*);
int updateStateBut(SDL_JoyButtonEvent*);
int updateStateDev(SDL_JoyDeviceEvent*);
int updateStateHat(SDL_JoyHatEvent*);
int updateStateKey(SDL_KeyboardEvent*);

#endif
