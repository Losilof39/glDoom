#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <cglm.h>

typedef struct sRenderer
{
	int bBlending;
	GLuint vao;
	GLuint ebo;
	mat4 matOrtho;
	mat4 matProj;

	void (*Setup)(void);
	void (*Set3D)(void);
	void (*StartRendition)(void);
	void (*StopRendition)(void);
	void (*SetTexture)(unsigned int);
	void (*SetShader)(unsigned int);
	void (*RenderEntities)(void);
	void (*Set2D)(void);
	void (*RenderString)(vec3* vertices, GLuint* indices, GLuint numIndices);
	void (*GetColorBuffer)(GLubyte* data);
	void (*RenderSprite)(GLuint textureId, vec3* vertices, GLuint* indices, GLuint numIndices);
}sRenderer;

#endif