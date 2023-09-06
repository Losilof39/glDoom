#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "cimgui.h"
#include "cimgui_impl.h"
#include<SDL.h>

void SetupIMGUI(SDL_Window* window, SDL_GLContext glcontext);
void ShutdownIMGUI();

#endif