#include "renderer2D.h"
#include "v_video.h"
#include "z_zone.h"
#include "w_wad.h"
#include "gl_texture.h"
#include "i_system.h"

R2DStorage s_Data;
FrameBuffer s_framebuffer;
RenderInfo s_renderinfo;
extern video_t video;

void InitRenderer2D()
{
	// configure VAO/VBO
	unsigned int VBO, quadVBO;
	float vertices[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	s_renderinfo.virtualWidth = 320;
	s_renderinfo.virtualHeight = 200;
	s_renderinfo.virtualRatio = (float)s_renderinfo.virtualWidth / (float)s_renderinfo.virtualHeight;

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
	s_Data.fbShader = Shader_Create("framebuffer", "shader_files/framebuffer.vs", "shader_files/framebuffer.ps");

	Shader_Use(s_Data.fbShader);
	Shader_SetInt(s_Data.fbShader, "screenTexture", 0);
	Shader_Unbind();

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// framebuffer configuration
	// -------------------------
	glGenFramebuffers(1, &s_framebuffer.fb);
	glBindFramebuffer(GL_FRAMEBUFFER, s_framebuffer.fb);


	// create a color attachment texture
	glGenTextures(1, &s_framebuffer.texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, s_framebuffer.texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, s_renderinfo.virtualWidth, s_renderinfo.virtualHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, s_framebuffer.texColorBuffer, 0);


	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &s_framebuffer.rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, s_framebuffer.rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, s_renderinfo.virtualWidth, s_renderinfo.virtualHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, s_framebuffer.rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		I_Error("Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// screen quad VAO
	glGenVertexArrays(1, &s_Data.screenVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(s_Data.screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);

}

void R2D_StartRendition(void)
{
	// bind to framebuffer and draw scene as we normally would to color texture 
	glBindFramebuffer(GL_FRAMEBUFFER, s_framebuffer.fb);
	glViewport(0, 0, s_renderinfo.virtualWidth, s_renderinfo.virtualHeight);
	//glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	// make sure we clear the framebuffer's content
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


}

void R2D_StopRendition(void)
{
	// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	int width = video.width;
	int height = (int)(width / s_renderinfo.virtualRatio + 0.5f);

	if (height > video.height)
	{
		//It doesn't fit our height, we must switch to pillarbox then
		height = video.height;
		width = (int)(height * s_renderinfo.virtualRatio + 0.5f);
	}

	// set up the new viewport centered in the backbuffer
	int vp_x = (video.width / 2) - (width / 2);
	int vp_y = (video.height / 2) - (height / 2);

	glViewport(vp_x, vp_y, width, height);
	//glViewport(0, 0, video.width, video.height);


	// clear all relevant buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set clear color to black
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(s_Data.screenVAO);

	Shader_Use(s_Data.fbShader);

	glBindTexture(GL_TEXTURE_2D, s_framebuffer.texColorBuffer);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
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
	Shader_SetMat4(s_Data.spriteShader, "u_Ortho", s_Data.camOrtho);
	Shader_SetInt(s_Data.spriteShader, "image", 0);

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
	Shader_SetMat4(s_Data.flatColorShader, "u_Ortho", s_Data.camOrtho);

	glBindVertexArray(s_Data.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


	Shader_Unbind();
}
