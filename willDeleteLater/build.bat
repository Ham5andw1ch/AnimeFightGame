@echo off
mkdir .\build
pushd .\build
SETLOCAL

SET    sdldir=..\SDL2\include
SET LIBRARIES=%sdldir%

SET      sdir=..\..\src
SET  inputsrc=%sdir%\input\joystate.c %sdir%\input\parser.c 
SET   SOURCES=%sdir%\test.c %inputsrc%

cl /Zi %SOURCES% -I %sdldir% -I %sdir% -I %sdir%\input /EHa
ENDLOCAL
popd        