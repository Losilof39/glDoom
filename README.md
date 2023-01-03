# glDoom Resurrected (again!)
This repository contains improvements for glDoom port

### Fixes (original from REDPOWAR)
* new floor/ceiling generation code (similar to code from Doom Legacy port)
* aspect ratio fix (correct rendering on 16:9 displays)
* fixed invisible area culling (behind walls in sectors without ceiling)
* few bugfixes for buffer overflows & invalid memory pointers

### Fixes from Myself
* 64bit cleanups to allow it to build and run on 64bit systems
* Cleanups for modern types (uintptr_t, intptr_t and MSVC specific functions like (underscore)access etc..)
* Addition of the score system code from WINDOOM
* Cleaned up all Win32 code and added a SDL2 backend
* Ported to Linux but it's not stable
* Now supporting mouse (still a bit janky)

## Current Problems
* Joystick is not supported (for now)
* No vertical mouse look movement
* 
* Multiplayer doesn't work
* Crash when 2nd demo starts playing
* Sound FX stutters

### TO-DO List
I am planning for this port, the following (in order of priority)

1. Remove all unused code and make code more readable
2. Fix palette updates when picking up items
3. Allowing the user to select the resolution and fullscreen/window within glDoom's menu
4. Write new OpenGL renderer that will use modern features from 4.5 and replace the Legacy one
5. Increasing the vanilla limits and adding support for No Rest For The Living and SIGIL

Plus more!

### Why
The answer is simple. These older source ports are special, they paved the way for what we have now, an actual piece of Doom history. While glDoom will never be as popular as GZDoom or PRBoom, it's history and legacy is an important part of the Doom Community and rather than be forgotten, they should be maintained for the next generation to use. Also for me (Losilof39) is a great way to learn how a large project like Doom is organized, coded and I'm honored to maintain this great work of art alive and bring new feature ( in the future ;) ).

## Compiling

## Windows
Use the Visual Studio project and build for your architecture.
Now it compiles on x64.

## Linux
Run ``` sh build.sh ``` in the commandline to build.

## Game data
Before playing DOOM, you will need to also create the gldoom.wad file by using the wadbuild tool (wadbuild.exe gldoom.lst) in the resources directory.
Be sure that gldoom.wad and your IWAD of choice (DOOM, DOOM 2, Ultimate Doom, ...) are in the same folder as the executable!
In order to hear the music in-game you should download [this](https://archive.org/download/free-soundfonts-sf2-2019-04/SC-55.sf2) soundfont, rename it to "soundfont.sf2" and place it in the game directory. You could use other sf2 files but I would recommend to use the SC-55, which is the soundcard originally used to create the DOOM music.
