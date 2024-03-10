#ifndef RENDERER2D_H
#define RENDERER2D_H

#include <glad/glad.h>
#include <cglm/cglm.h>
#include "r_shader.h"
#include "gldefs.h"

typedef struct s_R2DStorage
{
	GLuint VAO, lineVAO, lineVBO;
	GLuint screenVAO;
	Shader flatColorShader;
	Shader spriteShader;
	Shader fbShader;
	Shader lineShader;
	mat4 camOrtho;
}R2DStorage;

typedef enum R2D_RENDER_TYPE
{
	R2D_RENDER_PATCH = 0,
	R2D_RENDER_LINE,
	R2D_RENDER_TYPE_COUNT

}R2D_RENDER_TYPE;

// 2D draw command
typedef struct s_twodcommand
{
	unsigned int glTexture;
	GLint* plineVBO;
	vec3 position;
	vec2 size;
	float light;
	vec3 color;
	struct twodcommand* next;
}twodcommand;

void InitRenderer2D();

void R2D_StartRendition(void);
void R2D_StopRendition(void);
void R2D_FlushCommandList(twodcommand* head);
twodcommand* R2D_AddDrawCmd(twodcommand* head, R2D_RENDER_TYPE render_type);
void R2D_DrawSprite(vec3* position, vec2 size, GLTexData* tex);
void R2D_DrawSpriteFromName(vec3* position, vec2 size, const char* name);
void R2D_DrawColoredQuad(vec3* position, vec3* size, vec3* color);
void R2D_DrawLightSprite(vec3* position, vec2 size, GLTexData* tex, float light);		// sprite influenced by sector lighting
void R2D_DrawLine(float* points, vec3 color);
#endif

