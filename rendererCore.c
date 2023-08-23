#include "renderer.h"
#include "rendererCore.h"

#include "v_video.h"
#include "gl_video.h"

extern video_t video;
extern glmode_t glmode;
extern int gl_premalpha;
extern float glTop, glBottom, glRight, glLeft, SetBack;

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
}

void cSetup(void)
{
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

	Set3D();

	glmode = gl_3d;
}

void cStopRendition(void)
{
	if (glmode == gl_3d)
	{
	}
	else
	{
		Set2D();

	}

	// Swap the rendering buffers...
	SDL_GL_SwapWindow(pWindow);
}

void cSetTexture(unsigned int texID)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
}

void cSetShader(unsigned int shaderID)
{
}

void cRenderEntities(void)
{
}

void cSet2D(void)
{

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
}