#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <cglm/cglm.h>
#include "gldefs.h"

typedef struct sRenderer
{
	int bBlending;
	int gl_mode;
	GLuint vao;
	GLuint ebo;
	mat4 matOrtho;
	mat4 matProj;

	void (*Setup)(void);
	void (*Set3D)(void);
	void (*SetColor)(float r,float g,float b);
	void (*StartRendition)(void);
	void (*StopRendition)(void);
	void (*SetTexture)(unsigned int);
	void (*SetShader)(unsigned int);
	void (*RenderEntities)(void);
	void (*Set2D)(void);
	void (*RenderString)(vec3* vertices, GLuint* indices, GLuint numIndices);
	void (*GetColorBuffer)(GLubyte* data);
	void (*RenderSprite)(float* v, GLTexData* tex);
}sRenderer;

#endif