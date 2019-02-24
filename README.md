# AnimeFightGame
A 2D fighting game with engine written in C using SDL. 

## Features

Controller hotswapping
GPU accelerated rendering
Windows, Mac, and Linux support
Animage: A custom image format designed for easy pallete swapping at loadtime
Anpalette:A swappable palette system that works in tandem with animage

## TODO

Stereo audio
Simple sample game running in the engine
Code cleanup

## Building on Linux
Install SDL2 with your package manager.

On Arch based systems:
```
pacman -S sdl2
```

Run `make`.

## Building on Windows
- Download SDL2's MinGW-w64 distribution from the wiki.
- Install MinGW-w64 based tools.
- Copy the SDL2.dll file from the SDL2 distribution into the root directory of the repository.
- Run `mingw32-make SDL_DIR=C:\Path\To\SDL2-2.0.x\x86_64-w64-mingw32`
