#ifndef RENDERER_H
#define RENDERER_H

#include <cglm/cglm.h>
#include <glad/glad.h>
#include "r_shader.h"

typedef struct s_Renderer
{
	unsigned int screenVAO;
	Shader fbShader;
}Renderer;

typedef struct s_RenderInfo
{
	unsigned int virtualWidth, virtualHeight;
	float virtualRatio;							// i.e. aspect ratio of virtual screen size
}RenderInfo;

typedef struct s_FrameBuffer
{
	GLuint fb, rbo, texColorBuffer;
}FrameBuffer;

void R_InitRenderer();
void R_StartRendition();
void R_StopRendition();
void R_BindFramebuffer();
void R_BlitFramebuffer();

#endif