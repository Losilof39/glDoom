#include "imgui_layer.h"

ImGuiIO* io;

void SetupIMGUI(SDL_Window* window, SDL_GLContext glcontext)
{
    const char* glsl_version = "#version 330";

    // Setup cimgui context
    igCreateContext(NULL);
    io = igGetIO(); (void)io;

    // Setup Platform/Renderer backends
    //TODO:
    //ImGui_ImplSDL2_InitForOpenGL(window, glcontext);
    //ImGui_ImplOpenGL3_Init(glsl_version);

    igStyleColorsDark(NULL);
}

void ShutdownIMGUI()
{
    //ImGui_ImplOpenGL3_Shutdown();
    //ImGui_ImplSDL2_Shutdown();
    igDestroyContext(NULL);
}
