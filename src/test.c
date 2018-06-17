#include <stdio.h>
#include "SDL.h"

int main()
{
    if(SDL_Init(SDL_INIT_GAMECONTROLLER))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    while(1)
    {
        printf("%d\n", SDL_NumJoysticks());
        SDL_Event e;
        if(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
                break;
            if(e.type == SDL_KEYDOWN)
            {
                SDL_Keycode key = e.key.keysym.sym;
                if(key == SDLK_ESCAPE)
                {
                    SDL_Event ex;
                    ex.type = SDL_QUIT;
                    SDL_PushEvent(&exit);
                }
            }
        }
    }

    SDL_Quit();
}
