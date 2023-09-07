#include "renderer.h"
#include "renderer2D.h"
#include "renderer3D.h"

#include <glad/glad.h>
#include <SDL.h>

extern SDL_Window* pWindow;
Renderer renderer;

void R_StartRendition()
{
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	R2D_StartRendition();
}

void R_StopRendition()
{
	R2D_StopRendition();
	SDL_GL_SwapWindow(pWindow);
}
