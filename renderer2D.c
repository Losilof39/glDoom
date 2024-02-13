#include "renderer2D.h"
#include "renderer.h"
#include "v_video.h"
#include "z_zone.h"
#include "w_wad.h"
#include "gl_texture.h"
#include "i_system.h"

R2DStorage s_Data;
twodcommand* head_command = NULL;
extern video_t video;
extern RenderInfo s_renderinfo;

void InitRenderer2D()
{
	// configure VAO/VBO
	unsigned int VBO;
	float vertices[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &s_Data.VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(s_Data.VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	s_Data.flatColorShader = Shader_Create("flatcolor", "shader_files/flatcolor.vs", "shader_files/flatcolor.ps");
	s_Data.spriteShader = Shader_Create("sprite", "shader_files/sprite.vs", "shader_files/sprite.ps");

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void R2D_DrawSprite(vec3* position, vec2 size, GLTexData* tex)
{
	twodcommand* newNode = (twodcommand*)malloc(sizeof(twodcommand));

	if (head_command == NULL) {
		// If the list is empty, set the new node as the head
		head_command = newNode;
	}
	else {
		// Traverse to the end of the list and append the new node
		twodcommand* current = head_command;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newNode;
	}

	glm_vec3_copy(position, &newNode->position);
	glm_vec3_copy(size, &newNode->size);

	newNode->glTexture = tex->TexName;
	newNode->light = -1;

	newNode->next = NULL;
}

void R2D_DrawSpriteFromName(vec3* position, vec2 size, const char* name)
{
	twodcommand* newNode = (twodcommand*)malloc(sizeof(twodcommand));
	GLTexData	tex;

	GL_MakeSpriteTexture(W_CacheLumpName(name, PU_CACHE), &tex, false);

	if (head_command == NULL) {
		// If the list is empty, set the new node as the head
		head_command = newNode;
	}
	else {
		// Traverse to the end of the list and append the new node
		twodcommand* current = head_command;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newNode;
	}

	glm_vec3_copy(position, &newNode->position);
	glm_vec3_copy(size, &newNode->size);

	newNode->glTexture = tex.TexName;
	newNode->light = -1;

	newNode->next = NULL;
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
	twodcommand* newNode = (twodcommand*)malloc(sizeof(twodcommand));

	if (head_command == NULL) {
		// If the list is empty, set the new node as the head
		head_command = newNode;
	}
	else {
		// Traverse to the end of the list and append the new node
		twodcommand* current = head_command;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = newNode;
	}

	glm_vec3_copy(position, &newNode->position);
	glm_vec3_copy(size, &newNode->size);

	newNode->glTexture = tex->TexName;
	newNode->light = light;

	newNode->next = NULL;
}

void R2D_StartRendition(void)
{
}

void R2D_StopRendition(void)
{
	glDisable(GL_DEPTH_TEST);

	twodcommand* cur, *nextNode;
	mat4 model;

	cur = head_command;

	Shader_Use(s_Data.spriteShader);

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

	Shader_Unbind();

	cur = head_command;

	// Free each node in the list
	while (cur != NULL) {
		nextNode = cur->next;
		free(cur);
		cur = nextNode;
	}

	// Set the head to NULL to indicate an empty list
	head_command = NULL;
}