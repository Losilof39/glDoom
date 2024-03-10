#include "renderer2D.h"
#include "renderer.h"
#include "v_video.h"
#include "z_zone.h"
#include "w_wad.h"
#include "gl_texture.h"
#include "i_system.h"

R2DStorage s_Data;
twodcommand* headCmd2D[R2D_RENDER_TYPE_COUNT] = { NULL };
extern video_t video;
extern RenderInfo s_renderinfo;

#define MAXLINES 128

GLint lineVBO[MAXLINES] = { 0 };
unsigned int lineCountVBO = 0;

void InitRenderer2D()
{
	// configure VAO/VBO
	unsigned int quadVBO;
	float vertices[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	float line[4] = { 0.0f };

	glGenVertexArrays(1, &s_Data.VAO);
	glGenBuffers(1, &quadVBO);

	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(s_Data.VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &s_Data.lineVAO);
	glGenBuffers(1, &s_Data.lineVBO);

	glBindBuffer(GL_ARRAY_BUFFER, s_Data.lineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_DYNAMIC_DRAW);

	glBindVertexArray(s_Data.lineVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	s_Data.flatColorShader = Shader_Create("flatcolor", "shader_files/flatcolor.vs", "shader_files/flatcolor.ps");
	s_Data.spriteShader = Shader_Create("sprite", "shader_files/sprite.vs", "shader_files/sprite.ps");
	s_Data.lineShader = Shader_Create("line", "shader_files/line.vs", "shader_files/line.ps");

	glm_ortho(0.0f,
		(float)s_renderinfo.virtualWidth,
		(float)s_renderinfo.virtualHeight,
		0.0f,
		-1.0f,
		1.0f,
		s_Data.camOrtho);

	Shader_Use(s_Data.spriteShader);
	Shader_SetMat4(s_Data.spriteShader, "u_Ortho", s_Data.camOrtho);
	Shader_SetInt(s_Data.spriteShader, "image", 0);
	Shader_Unbind();

	Shader_Use(s_Data.flatColorShader);
	Shader_SetMat4(s_Data.flatColorShader, "u_Ortho", s_Data.camOrtho);
	Shader_Unbind();

	Shader_Use(s_Data.lineShader);
	Shader_SetMat4(s_Data.lineShader, "u_Ortho", s_Data.camOrtho);
	Shader_Unbind();

}

// every frame flush command list
void R2D_FlushCommandList(twodcommand* head)
{
	twodcommand* cur, * nextNode;

	for (unsigned int type = 0; type < R2D_RENDER_TYPE_COUNT; type++)
	{
		cur = headCmd2D[type];

		// Free each node in the list
		while (cur != NULL) {
			nextNode = cur->next;
			Z_Free(cur);
			cur = nextNode;
		}

		// Set the head to NULL to indicate an empty list
		headCmd2D[type] = NULL;
	}
}

// push back 2D draw command and return its pointer
twodcommand* R2D_AddDrawCmd(twodcommand* head, R2D_RENDER_TYPE render_type)
{
	twodcommand* newNode = (twodcommand*)Z_Malloc(sizeof(twodcommand), PU_STATIC, NULL);

	if (headCmd2D[render_type] == NULL) {
		// If the list is empty, set the new node as the head
		headCmd2D[render_type] = newNode;
	}
	else {
		// Traverse to the end of the list and append the new node
		twodcommand* current = headCmd2D[render_type];
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newNode;
	}

	newNode->next = NULL;

	return newNode;
}

void R2D_DrawSprite(vec3* position, vec2 size, GLTexData* tex)
{
	twodcommand* newNode = R2D_AddDrawCmd(headCmd2D, R2D_RENDER_PATCH);

	glm_vec3_copy(position, &newNode->position);
	glm_vec3_copy(size, &newNode->size);

	newNode->glTexture = tex->TexName;
	newNode->light = -1;

	//newNode->next = NULL;
}

void R2D_DrawSpriteFromName(vec3* position, vec2 size, const char* name)
{
	twodcommand* newNode = R2D_AddDrawCmd(headCmd2D, R2D_RENDER_PATCH);
	GLTexData	tex;

	GL_MakeSpriteTexture(W_CacheLumpName(name, PU_CACHE), &tex, false);

	glm_vec3_copy(position, &newNode->position);
	glm_vec3_copy(size, &newNode->size);

	newNode->glTexture = tex.TexName;
	newNode->light = -1;

	//newNode->next = NULL;
}

void R2D_DrawColoredQuad(vec3* position, vec3* size, vec3* color)
{
	/*mat4 model, translate, scale;

	glm_mat4_identity(model);
	glm_mat4_identity(translate);
	glm_mat4_identity(scale);

	glm_scale(scale, size);
	glm_translate(translate, position);
	glm_mat4_mul(translate, scale, model);

	Shader_Use(s_Data.flatColorShader);
	Shader_SetVec3(s_Data.flatColorShader, "u_Color", color);
	Shader_SetMat4(s_Data.flatColorShader, "u_Model", model);

	glBindVertexArray(s_Data.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


	Shader_Unbind();*/
}

void R2D_DrawLightSprite(vec3* position, vec2 size, GLTexData* tex, float light)
{
	twodcommand* newNode = R2D_AddDrawCmd(headCmd2D, R2D_RENDER_PATCH);

	glm_vec3_copy(position, &newNode->position);
	glm_vec3_copy(size, &newNode->size);

	newNode->glTexture = tex->TexName;
	newNode->light = light;

	//newNode->next = NULL;
}

void R2D_DrawLine(float* points, vec3 color)
{
	twodcommand* newNode = R2D_AddDrawCmd(headCmd2D, R2D_RENDER_PATCH);
	GLint vbo = -1;

	glm_vec3_copy(color, newNode->color);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_DYNAMIC_DRAW);

	lineVBO[lineCountVBO] = vbo;

	lineCountVBO = (lineCountVBO + 1) % MAXLINES;

	newNode->plineVBO = &lineVBO[lineCountVBO];

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void R2D_StartRendition(void)
{
}

void R2D_StopRendition(void)
{
	glDisable(GL_DEPTH_TEST);

	twodcommand* cur;
	mat4 model;


	Shader_Use(s_Data.spriteShader);

	for (unsigned int type = 0; type < R2D_RENDER_TYPE_COUNT; type++)
	{
		cur = headCmd2D[type];

		switch(type)
		{
			case R2D_RENDER_PATCH:
			{
				while (cur != NULL)
				{
					glm_mat4_identity(model);

					glm_translate(model, cur->position);
					glm_scale(model, cur->size);

					Shader_SetMat4(s_Data.spriteShader, "u_Model", model);
					Shader_SetFloat(s_Data.spriteShader, "light", cur->light);

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, cur->glTexture);

					glBindVertexArray(s_Data.VAO);
					glDrawArrays(GL_TRIANGLES, 0, 6);

					glBindVertexArray(0);
					glBindTexture(GL_TEXTURE_2D, 0);

					cur = cur->next;
				}
			}break;

			/*case R2D_RENDER_LINE:
			{
				glBindVertexArray(s_Data.VAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				glBindVertexArray(0);
				glBindTexture(GL_TEXTURE_2D, 0);

				cur = cur->next;
			}*/
		}
	}

	Shader_Unbind();

	R2D_FlushCommandList(headCmd2D);
}
