#include "renderer.h"
#include "renderer2D.h"
#include "renderer3D.h"

#include <glad/glad.h>
#include <SDL.h>

extern SDL_Window* pWindow;
Renderer renderer;

void R_StartRendition()
{
	R2D_StartRendition();
}

void R_StopRendition()
{
	R2D_StopRendition();
	SDL_GL_SwapWindow(pWindow);
}
