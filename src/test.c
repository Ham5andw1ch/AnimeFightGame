#include <stdio.h>
#include "SDL.h"
#include "global.h"
#include "joyState.h"
#include "parser.h"
#include "settings.h"

void printEventTypes()
{
    dbgprint("SDL_QUIT: %d\n", SDL_QUIT);
    dbgprint("SDL_APP_TERMINATING: %d\n", SDL_APP_TERMINATING);
    dbgprint("SDL_APP_LOWMEMORY: %d\n", SDL_APP_LOWMEMORY);
    dbgprint("SDL_APP_WILLENTERBACKGROUND: %d\n", SDL_APP_WILLENTERBACKGROUND);
    dbgprint("SDL_APP_DIDENTERBACKGROUND: %d\n", SDL_APP_DIDENTERBACKGROUND);
    dbgprint("SDL_APP_WILLENTERFOREGROUND: %d\n", SDL_APP_WILLENTERFOREGROUND);
    dbgprint("SDL_APP_DIDENTERFOREGROUND: %d\n", SDL_APP_DIDENTERFOREGROUND);

    dbgprint("SDL_WINDOWEVENT: %d\n", SDL_WINDOWEVENT);
    dbgprint("SDL_SYSWMEVENT: %d\n", SDL_SYSWMEVENT);

    dbgprint("SDL_KEYDOWN: %d\n", SDL_KEYDOWN);
    dbgprint("SDL_KEYUP: %d\n", SDL_KEYUP);
    dbgprint("SDL_TEXTEDITING: %d\n", SDL_TEXTEDITING);
    dbgprint("SDL_TEXTINPUT: %d\n", SDL_TEXTINPUT);
    dbgprint("SDL_KEYMAPCHANGED: %d\n", SDL_KEYMAPCHANGED);

    dbgprint("SDL_MOUSEMOTION: %d\n", SDL_MOUSEMOTION);
    dbgprint("SDL_MOUSEBUTTONDOWN: %d\n", SDL_MOUSEBUTTONDOWN);
    dbgprint("SDL_MOUSEBUTTONUP: %d\n", SDL_MOUSEBUTTONUP);
    dbgprint("SDL_MOUSEWHEEL: %d\n", SDL_MOUSEWHEEL);

    dbgprint("SDL_JOYAXISMOTION: %d\n", SDL_JOYAXISMOTION);
    dbgprint("SDL_JOYBALLMOTION: %d\n", SDL_JOYBALLMOTION);
    dbgprint("SDL_JOYHATMOTION: %d\n", SDL_JOYHATMOTION);
    dbgprint("SDL_JOYBUTTONDOWN: %d\n", SDL_JOYBUTTONDOWN);
    dbgprint("SDL_JOYBUTTONUP: %d\n", SDL_JOYBUTTONUP);
    dbgprint("SDL_JOYDEVICEADDED: %d\n", SDL_JOYDEVICEADDED);
    dbgprint("SDL_JOYDEVICEREMOVED: %d\n", SDL_JOYDEVICEREMOVED);

    dbgprint("SDL_CONTROLLERAXISMOTION: %d\n", SDL_CONTROLLERAXISMOTION);
    dbgprint("SDL_CONTROLLERBUTTONDOWN: %d\n", SDL_CONTROLLERBUTTONDOWN);
    dbgprint("SDL_CONTROLLERBUTTONUP: %d\n", SDL_CONTROLLERBUTTONUP);
    dbgprint("SDL_CONTROLLERDEVICEADDED: %d\n", SDL_CONTROLLERDEVICEADDED);
    dbgprint("SDL_CONTROLLERDEVICEREMOVED: %d\n", SDL_CONTROLLERDEVICEREMOVED);
    dbgprint("SDL_CONTROLLERDEVICEREMAPPED: %d\n", SDL_CONTROLLERDEVICEREMAPPED);

    dbgprint("SDL_FINGERDOWN: %d\n", SDL_FINGERDOWN);
    dbgprint("SDL_FINGERUP: %d\n", SDL_FINGERUP);
    dbgprint("SDL_FINGERMOTION: %d\n", SDL_FINGERMOTION);

    dbgprint("SDL_DOLLARGESTURE: %d\n", SDL_DOLLARGESTURE);
    dbgprint("SDL_DOLLARRECORD: %d\n", SDL_DOLLARRECORD);
    dbgprint("SDL_MULTIGESTURE: %d\n", SDL_MULTIGESTURE);

    dbgprint("SDL_CLIPBOARDUPDATE: %d\n", SDL_CLIPBOARDUPDATE);

    dbgprint("SDL_DROPFILE: %d\n", SDL_DROPFILE);
    dbgprint("SDL_DROPTEXT: %d\n", SDL_DROPTEXT);
    dbgprint("SDL_DROPBEGIN: %d\n", SDL_DROPBEGIN);
    dbgprint("SDL_DROPCOMPLETE: %d\n", SDL_DROPCOMPLETE);

    dbgprint("SDL_AUDIODEVICEADDED: %d\n", SDL_AUDIODEVICEADDED);
    dbgprint("SDL_AUDIODEVICEREMOVED: %d\n", SDL_AUDIODEVICEREMOVED);

    dbgprint("SDL_RENDER_TARGETS_RESET: %d\n", SDL_RENDER_TARGETS_RESET);
    dbgprint("SDL_RENDER_DEVICE_RESET: %d\n", SDL_RENDER_DEVICE_RESET);

    dbgprint("SDL_USEREVENT: %d\n", SDL_USEREVENT);
    dbgprint("SDL_LASTEVENT: %d\n", SDL_LASTEVENT);
}

//fish
//SDL_Joystick *joysticks[50];
//int nextJoy = 0;
//
//int joystickWatch(void* userdata, SDL_Event* event)
//{
//    switch(event->type)
//    {
//    case SDL_JOYAXISMOTION:
//        dbgprint("%d: Axis %d on Joystick %d: %d\n", event->jaxis.timestamp,
//                event->jaxis.axis, event->jaxis.which, event->jaxis.value);
//        break;
//    case SDL_JOYBALLMOTION:
//        dbgprint("%d: Trackball %d on Joystick %d: (%d, %d)\n", event->jball.timestamp,
//                event->jball.ball, event->jball.which, event->jball.xrel,
//                event->jball.yrel);
//        break;
//    case SDL_JOYHATMOTION:
//        dbgprint("%d: POV Hat %d on Joystick %d: %d\n", event->jhat.timestamp,
//                event->jhat.hat, event->jhat.which, event->jhat.value);
//        break;
//    case SDL_JOYBUTTONDOWN:
//        dbgprint("%d: Button %d down on Joystick %d: %d\n", event->jbutton.timestamp,
//                event->jbutton.button, event->jbutton.which, event->jbutton.state);
//        break;
//    case SDL_JOYBUTTONUP:
//        dbgprint("%d: Button %d up on Joystick %d: %d\n", event->jbutton.timestamp,
//                event->jbutton.button, event->jbutton.which, event->jbutton.state);
//        break;
//    case SDL_JOYDEVICEADDED:
//        dbgprint("Joystick %d Added\n", event->jdevice.which);
//        joysticks[nextJoy++] = SDL_JoystickOpen(event->jdevice.which);
//        break;
//    case SDL_JOYDEVICEREMOVED:
//        dbgprint("Joystick %d Removed\n", event->jdevice.which);
//        SDL_JoystickClose(SDL_JoystickFromInstanceID(event->jdevice.which));
//        joysticks[event->jdevice.which] = NULL;
//        break;
//    }
//}

Uint64 NOW = 0;
Uint64 LAST = 0;
double deltaTime = 0.0;
int main(int argc, char **argv)
{
    if(SDL_Init(SDL_INIT_JOYSTICK))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    joyInit();
    initQueues();

//  for(int i = 0; i < SDL_NumJoysticks(); ++i)
//  {
//      dbgprint("Joystick %d Added\n", i);
//      if(!(joysticks[nextJoy++] = SDL_JoystickOpen(i)))
//      {
//          SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't open Joystick %d: %s\n", i,
//                  SDL_GetError());
//      }
//  }

//  SDL_AddEventWatch(joystickWatch, NULL);

//  printEventTypes();

    while(1)
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = deltaTime + (double) ((NOW-LAST)*1000/(double)SDL_GetPerformanceFrequency());
        //dbgprint("%d\t%d\t%f\n", LAST, NOW, deltaTime*.001);
        if( deltaTime * .001 > (double)1/(double) FPS){
        deltaTime = 0;
        joyUpdate();
        parserUpdate();
        SDL_Event e;
        if(SDL_PollEvent(&e))
        {
//          dbgprint("Event Type %d\n", e.type);
            if(e.type == SDL_QUIT)
                break;
            joyEvent(&e);
        }
        }
    }

    joyRip();

//  for(int i = 0; i < nextJoy; ++i)
//  {
//      if(joysticks[i] != NULL && SDL_JoystickGetAttached(joysticks[i]))
//      {
//          dbgprint("Joystick %d Removed\n", SDL_JoystickInstanceID(joysticks[i]));
//          SDL_JoystickClose(joysticks[i]);
//      }
//  }

    SDL_Quit();
    return 0;
}
