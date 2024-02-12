#include "renderer2D.h"
#include "renderer.h"
#include "v_video.h"
#include "z_zone.h"
#include "w_wad.h"
#include "gl_texture.h"
#include "i_system.h"

R2DStorage s_Data;
twodcommand* draw_command;
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

	draw_command = (twodcommand*)malloc(sizeof(twodcommand));

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

void R2D_StartRendition(void)
{
}

void R2D_StopRendition(void)
{
}

void R2D_RenderString(vec3* position, const char* text)
{
}

void R2D_DrawSprite(vec3* position, vec2 size, GLTexData* tex)
{
	mat4 model, translate, scale;

	glm_mat4_identity(model);
	glm_mat4_identity(translate);
	glm_mat4_identity(scale);

	glm_scale(scale, size);
	glm_translate(translate, position);
	glm_mat4_mul(translate, scale, model);

	Shader_Use(s_Data.spriteShader);
	Shader_SetMat4(s_Data.spriteShader, "u_Model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex->TexName);

	glBindVertexArray(s_Data.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	Shader_Unbind();
}

void R2D_DrawSpriteFromName(vec3* position, vec2 size, const char* name)
{
	GLTexData	tex;
	mat4		model, translate, scale;

	GL_MakeSpriteTexture(W_CacheLumpName(name, PU_CACHE), &tex, false);

	glm_mat4_identity(model);
	glm_mat4_identity(translate);
	glm_mat4_identity(scale);

	glm_scale(scale, size);
	glm_translate(translate, position);
	glm_mat4_mul(translate, scale, model);

	Shader_Use(s_Data.spriteShader);
	Shader_SetMat4(s_Data.spriteShader, "u_Model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex.TexName);

	glBindVertexArray(s_Data.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	Shader_Unbind();
}

void R2D_DrawColoredQuad(vec3* position, vec3* size, vec3* color)
{
	mat4 model, translate, scale;

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


	Shader_Unbind();
}
