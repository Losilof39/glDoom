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

## Current Problems
1. Sounds fx like Imps' screams or shotgun do not work (for now only MIDI works)

### TO-DO List
I am planning for this port, the following (in order of priority)

1. Reorganize the code better with current SDL2 backend
2. Replace current sound/audio subsystem with SDL2 (bye bye DirectSound)
3. Fix palette updates when picking up items
4. Allowing the user to select the resolution and fullscreen/window within glDoom's menu
5. Cleaning up the OpenGL code and moving some things to shaders (MAYBE IN THE FUTURE)
6. Increasing the vanilla limits and adding support for No Rest For The Living and SIGIL (MAYBE IN THE FUTURE)

### Note - 16.05.2022
Current I am adding SDL2 Mixer support to stream audio (ported from various SDL2-based source ports).

Plus more!

### Why
The answer is simple. These older source ports are special, they paved the way for what we have now, an actual piece of Doom history. While glDoom will never be as popular as GZDoom or PRBoom, it's history and legacy is an important part of the Doom Community and rather than be forgotten, they should be maintained for the next generation to use.

### Compiling
Use the Visual Studio project and build for your architecture.
Now it compiles on x64.

To use it, you will need to also create the gldoom.wad file by using the wadbuild tool (wadbuild.exe gldoom.lst) in the resources directory.
