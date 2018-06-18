#include <stdio.h>
#include "SDL.h"

void printEventTypes()
{
    printf("SDL_QUIT: %d\n", SDL_QUIT);
    printf("SDL_APP_TERMINATING: %d\n", SDL_APP_TERMINATING);
    printf("SDL_APP_LOWMEMORY: %d\n", SDL_APP_LOWMEMORY);
    printf("SDL_APP_WILLENTERBACKGROUND: %d\n", SDL_APP_WILLENTERBACKGROUND);
    printf("SDL_APP_DIDENTERBACKGROUND: %d\n", SDL_APP_DIDENTERBACKGROUND);
    printf("SDL_APP_WILLENTERFOREGROUND: %d\n", SDL_APP_WILLENTERFOREGROUND);
    printf("SDL_APP_DIDENTERFOREGROUND: %d\n", SDL_APP_DIDENTERFOREGROUND);

    printf("SDL_WINDOWEVENT: %d\n", SDL_WINDOWEVENT);
    printf("SDL_SYSWMEVENT: %d\n", SDL_SYSWMEVENT);

    printf("SDL_KEYDOWN: %d\n", SDL_KEYDOWN);
    printf("SDL_KEYUP: %d\n", SDL_KEYUP);
    printf("SDL_TEXTEDITING: %d\n", SDL_TEXTEDITING);
    printf("SDL_TEXTINPUT: %d\n", SDL_TEXTINPUT);
    printf("SDL_KEYMAPCHANGED: %d\n", SDL_KEYMAPCHANGED);

    printf("SDL_MOUSEMOTION: %d\n", SDL_MOUSEMOTION);
    printf("SDL_MOUSEBUTTONDOWN: %d\n", SDL_MOUSEBUTTONDOWN);
    printf("SDL_MOUSEBUTTONUP: %d\n", SDL_MOUSEBUTTONUP);
    printf("SDL_MOUSEWHEEL: %d\n", SDL_MOUSEWHEEL);

    printf("SDL_JOYAXISMOTION: %d\n", SDL_JOYAXISMOTION);
    printf("SDL_JOYBALLMOTION: %d\n", SDL_JOYBALLMOTION);
    printf("SDL_JOYHATMOTION: %d\n", SDL_JOYHATMOTION);
    printf("SDL_JOYBUTTONDOWN: %d\n", SDL_JOYBUTTONDOWN);
    printf("SDL_JOYBUTTONUP: %d\n", SDL_JOYBUTTONUP);
    printf("SDL_JOYDEVICEADDED: %d\n", SDL_JOYDEVICEADDED);
    printf("SDL_JOYDEVICEREMOVED: %d\n", SDL_JOYDEVICEREMOVED);

    printf("SDL_CONTROLLERAXISMOTION: %d\n", SDL_CONTROLLERAXISMOTION);
    printf("SDL_CONTROLLERBUTTONDOWN: %d\n", SDL_CONTROLLERBUTTONDOWN);
    printf("SDL_CONTROLLERBUTTONUP: %d\n", SDL_CONTROLLERBUTTONUP);
    printf("SDL_CONTROLLERDEVICEADDED: %d\n", SDL_CONTROLLERDEVICEADDED);
    printf("SDL_CONTROLLERDEVICEREMOVED: %d\n", SDL_CONTROLLERDEVICEREMOVED);
    printf("SDL_CONTROLLERDEVICEREMAPPED: %d\n", SDL_CONTROLLERDEVICEREMAPPED);

    printf("SDL_FINGERDOWN: %d\n", SDL_FINGERDOWN);
    printf("SDL_FINGERUP: %d\n", SDL_FINGERUP);
    printf("SDL_FINGERMOTION: %d\n", SDL_FINGERMOTION);

    printf("SDL_DOLLARGESTURE: %d\n", SDL_DOLLARGESTURE);
    printf("SDL_DOLLARRECORD: %d\n", SDL_DOLLARRECORD);
    printf("SDL_MULTIGESTURE: %d\n", SDL_MULTIGESTURE);

    printf("SDL_CLIPBOARDUPDATE: %d\n", SDL_CLIPBOARDUPDATE);

    printf("SDL_DROPFILE: %d\n", SDL_DROPFILE);
    printf("SDL_DROPTEXT: %d\n", SDL_DROPTEXT);
    printf("SDL_DROPBEGIN: %d\n", SDL_DROPBEGIN);
    printf("SDL_DROPCOMPLETE: %d\n", SDL_DROPCOMPLETE);

    printf("SDL_AUDIODEVICEADDED: %d\n", SDL_AUDIODEVICEADDED);
    printf("SDL_AUDIODEVICEREMOVED: %d\n", SDL_AUDIODEVICEREMOVED);

    printf("SDL_RENDER_TARGETS_RESET: %d\n", SDL_RENDER_TARGETS_RESET);
    printf("SDL_RENDER_DEVICE_RESET: %d\n", SDL_RENDER_DEVICE_RESET);

    printf("SDL_USEREVENT: %d\n", SDL_USEREVENT);
    printf("SDL_LASTEVENT: %d\n", SDL_LASTEVENT);
}

//fish
SDL_Joystick *joysticks[50];
int nextJoy = 0;

int joystickWatch(void* userdata, SDL_Event* event)
{
    switch(event->type)
    {
    case SDL_JOYAXISMOTION:
        printf("%d: Axis %d on Joystick %d: %d\n", event->jaxis.timestamp,
                event->jaxis.axis, event->jaxis.which, event->jaxis.value);
        break;
    case SDL_JOYBALLMOTION:
        printf("%d: Trackball %d on Joystick %d: (%d, %d)\n", event->jball.timestamp,
                event->jball.ball, event->jball.which, event->jball.xrel,
                event->jball.yrel);
        break;
    case SDL_JOYHATMOTION:
        printf("%d: POV Hat %d on Joystick %d: %d\n", event->jhat.timestamp,
                event->jhat.hat, event->jhat.which, event->jhat.value);
        break;
    case SDL_JOYBUTTONDOWN:
        printf("%d: Button %d down on Joystick %d: %d\n", event->jbutton.timestamp,
                event->jbutton.button, event->jbutton.which, event->jbutton.state);
        break;
    case SDL_JOYBUTTONUP:
        printf("%d: Button %d up on Joystick %d: %d\n", event->jbutton.timestamp,
                event->jbutton.button, event->jbutton.which, event->jbutton.state);
        break;
    case SDL_JOYDEVICEADDED:
        printf("Joystick %d Added\n", event->jdevice.which);
        joysticks[nextJoy++] = SDL_JoystickOpen(event->jdevice.which);
        break;
    case SDL_JOYDEVICEREMOVED:
        printf("Joystick %d Removed\n", event->jdevice.which);
        SDL_JoystickClose(SDL_JoystickFromInstanceID(event->jdevice.which));
        joysticks[event->jdevice.which] = NULL;
        break;
    }
}

int main()
{
    if(SDL_Init(SDL_INIT_GAMECONTROLLER))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    for(int i = 0; i < SDL_NumJoysticks(); ++i)
    {
        printf("Joystick %d Added\n", i);
        if(!(joysticks[nextJoy++] = SDL_JoystickOpen(i)))
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't open Joystick %d: %s\n", i,
                    SDL_GetError());
        }
    }

    SDL_AddEventWatch(joystickWatch, NULL);

//  printEventTypes();

    while(1)
    {
        SDL_Event e;
        if(SDL_PollEvent(&e))
        {
//          printf("Event Type %d\n", e.type);
            if(e.type == SDL_QUIT)
                break;

        }
    }

    for(int i = 0; i < nextJoy; ++i)
    {
        if(joysticks[i] != NULL && SDL_JoystickGetAttached(joysticks[i]))
        {
            printf("Joystick %d Removed\n", SDL_JoystickInstanceID(joysticks[i]));
            SDL_JoystickClose(joysticks[i]);
        }
    }

    SDL_Quit();
}
