#ifndef RENDERER_H
#define RENDERER_H

#include <cglm/cglm.h>

typedef struct s_Renderer
{
	mat4 orthoCam;
	mat4 projCam;
}Renderer;

void R_StartRendition();
void R_StopRendition();

#endif