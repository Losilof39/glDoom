#ifndef RENDERER2D_H
#define RENDERER2D_H

#include <glad/glad.h>
#include <cglm/cglm.h>
#include "r_shader.h"
#include "gldefs.h"

typedef struct s_R2DStorage
{
	GLuint VAO;
	GLuint screenVAO;
	Shader flatColorShader;
	Shader spriteShader;
	Shader fbShader;
	mat4 camOrtho;
}R2DStorage;

// 2D draw command
typedef struct s_twodcommand
{
	unsigned int glTexture;
	vec3 position;
	vec2 size;
	float light;
	struct twodcommand* next;
}twodcommand;

void InitRenderer2D();

void R2D_StartRendition(void);
void R2D_StopRendition(void);
void R2D_DrawSprite(vec3* position, vec2 size, GLTexData* tex);
void R2D_DrawSpriteFromName(vec3* position, vec2 size, const char* name);
void R2D_DrawColoredQuad(vec3* position, vec3* size, vec3* color);
void R2D_DrawLightSprite(vec3* position, vec2 size, GLTexData* tex, float light);		// sprite influenced by sector lighting
#endif

