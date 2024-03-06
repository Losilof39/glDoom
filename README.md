# glDoom Resurrected (again!)
This repository contains improvements for the glDoom port by Bruce Lewis and by other authors, including: REDPOWAR, Adam Bilbrough (Gibbon), André Guilherme (Wolf3s) and Losilof39.

### Fixes (original from REDPOWAR)
* new floor/ceiling generation code (similar to code from Doom Legacy port)
* aspect ratio fix (correct rendering on 16:9 displays)
* fixed invisible area culling (behind walls in sectors without ceiling)
* few bugfixes for buffer overflows & invalid memory pointers

### Fixes from Gibbon
* 64bit cleanups to allow it to build and run on 64bit systems
* Cleanups for modern types (``` uintptr_t ```, ``` intptr_t ``` and MSVC specific functions like ``` _access ``` etc..)
* Add SIGIL support.
* Add vsync
* Add NRTL support

### Fixes from Losilof39
* Cleaned up all Win32 code and added a SDL2 backend
* Ported to Linux but it's not stable yet
* Mouse support
* Audio rewritten using SDL2 from chocolate doom
* Brand new 2D and 3D renderer using modern OpenGL!

### Fixes from Wolf3s
* Added a wrapper for encapsulate underscore newlib functions
* Removal of unused declarations
* Code cleanup
* Remove type defintions 
* Add ANSI C Implementations in some parts
* Fixed win32 build
* Add GetTicks macro
* Remove msvc level 3 and a little of level 4 warnings
* Replaced `_filelength` function to `fstat`  
* Created a bitmap loader based on windows api
* Ported Win32 api to posix

### Current Problems
* No vertical mouse movement
* Clipping issue due to changing fov
* Fullscreen option not working
* Resolution option not working
* New renderer still in prototype
* Only the legacy renderer works (for now!)
* Multiple errors when switching to OpenGL core profile
* SIGIL and NRFTL support works, but the levels doesn't show up

### TO-DO List
I am planning for this port, the following (in order of priority)

1. Remove all unused code and make code more readable
   
2. OpenGL renderer:
   -   Unload all VAO, VBO and texture data before loading new level
   -   Add dynamic point lights
     
3. Increasing the vanilla limits and adding support for No Rest For The Living and SIGIL
   - The Sigil compat version works fine however the 5th episode never gets loaded and always goes to the 4th episode(gibbon)

Plus more!

## Compiling

### Windows
Use the Visual Studio 2022 project and build for your desired architecture.

### Linux
Run ``` make all ``` in the commandline to build.

### Game data
Before playing DOOM, you will need to also create the ```gldoom.wad ``` file by using the wadbuild tool (wadbuild.exe gldoom.lst) in the resources directory.
Be sure that ```gldoom.wad ``` and your IWAD of choice (DOOM, DOOM 2, Ultimate Doom, ...) are in the same folder as the executable!
In order to hear the music in-game you should download [this](https://archive.org/download/free-soundfonts-sf2-2019-04/SC-55.sf2) soundfont, rename it to ``` soundfont.sf2``` and place it in the game directory. You could use other `.sf2` files but I would recommend to use the SC-55, which is the soundcard originally used to create the DOOM music.

### Why maintain glDoom?
The answer is simple. These older source ports are special, they paved the way for what we have now, an actual piece of Doom history. While glDoom will never be as popular as GZDoom, PRBoom, PRBoom+ and dsda-doom, his history and legacy it's an important part of the Doom Community and rather than be forgotten, they should be maintained for the next generation to use. Also for me (Losilof39), Wolf3s and other contribuitors is a great way to learn how a large project like Doom is organized, coded and we're honored to maintain this great work of art alive and bring new features to it!
