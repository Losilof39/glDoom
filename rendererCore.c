#include "renderer.h"
#include "rendererCore.h"

#include "v_video.h"
#include "gl_video.h"
#include "r_shader.h"

extern video_t video;
extern glmode_t glmode;
extern int gl_premalpha;
extern float glTop, glBottom, glRight, glLeft, SetBack;

GLuint indices[6] = {0, 1, 2, 0, 2, 3};
GLuint EBO, VBO, texVBO;

Shader shader;
mat4 ortho;

void InitRendererCore(sRenderer* renderer)
{
	renderer->Setup = cSetup;
	renderer->Set3D = cSet3D;
	renderer->SetColor = cSetColor;
	renderer->StartRendition = cStartRendition;
	renderer->StopRendition = cStopRendition;
	renderer->SetTexture = cSetTexture;
	renderer->SetShader = cSetShader;
	renderer->RenderEntities = cRenderEntities;
	renderer->Set2D = cSet2D;
	renderer->RenderString = cRenderString;
	renderer->GetColorBuffer = cGetColorBuffer;
	renderer->RenderSprite = cRenderSprite;

	glm_ortho(0.0f,
		video.width,
		video.height,
		0.0f,
		-1.0f,
		1.0f,
		renderer->matOrtho);

	glm_perspective(video.fovy,
		(float)video.width / (float)video.height,
		video.nearclip,
		video.farclip,
		renderer->matProj);
}

void cSetup(void)
{
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &texVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	shader = Shader_Create("UI", "shader_files//ui_vert.glsl", "shader_files//ui_frag.glsl");

	glm_ortho(0.0f,
		video.width,
		video.height,
		0.0f,
		-1.0f,
		1.0f,
		ortho);
}

void cw3sgluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
	GLdouble top, bottom, left, right;
	double pi180 = 0.017453292519943295769236907684886;
	top = zNear * tan(pi180 * fovy / 2);
	bottom = -top;
	right = aspect * top;
	left = -right;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(left, right, bottom, top, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
}

void cSet3D(void)
{
}

void cSetColor(float r, float g, float b)
{
}

void cStartRendition(void)
{
	// er?
	// Will put the OpenGL frame start code in here...
	glClear(GL_DEPTH_BUFFER_BIT);

	cSet3D();

	glmode = gl_3d;
}

void cStopRendition(void)
{
	if (glmode == gl_3d)
	{
	}
	else
	{
		cSet2D();
	}

	// Swap the rendering buffers...
	SDL_GL_SwapWindow(pWindow);
}

void cSetTexture(unsigned int texID)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	glActiveTexture(GL_TEXTURE0);
}

void cSetShader(unsigned int shaderID)
{
}

void cRenderEntities(void)
{
}

void cSet2D(void)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDisable(GL_DEPTH_TEST);
}

void cRenderString(vec3* vertices, GLuint* indices, GLuint numIndices)
{
}

void cGetColorBuffer(GLubyte* data)
{
}

void cRenderSprite(float* v, float* uv, GLuint* indices, GLuint numIndices)
{
	float verts[] = { v[0], v[2], SetBack, v[0], v[3], SetBack, v[1], v[3], SetBack, v[1], v[2], SetBack };
	float coords[] = { 0.0f, 1.0f, 0.0f, uv[1], uv[0], uv[1], uv[0], 1.0f };

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)(0));
	glEnableVertexAttribArray(1);

	Shader_SetInt(shader, "texture", 0);
	Shader_SetMat4(shader, "ortho", ortho);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
}