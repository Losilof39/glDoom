#include "renderer.h"
#include "rendererFixed.h"

#include "v_video.h"
#include "gl_video.h"

extern video_t video;
extern glmode_t glmode;
extern float glTop, glBottom, glRight, glLeft, SetBack;

void InitRendererFixed(sRenderer* renderer)
{
	renderer->Setup = Setup;
	renderer->Set3D = Set3D;
	renderer->StartRendition = StartRendition;
	renderer->StopRendition = StopRendition;
	renderer->SetTexture = SetTexture;
	renderer->SetShader = SetShader;
	renderer->RenderEntities = RenderEntities;
	renderer->Set2D = Set2D;
	renderer->RenderString = RenderString;
	renderer->GetColorBuffer = GetColorBuffer;
	renderer->RenderSprite = RenderSprite;
}

void Setup(void)
{
}

void Set3D(void)
{
	float glFovY = video.fovy;
	float glAspect = (float)video.width / (float)video.height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	w3sgluPerspective((double)glFovY, (double)glAspect, (double)video.nearclip, (double)video.farclip);
	glViewport(0, 0, video.width, video.height);

	glTranslatef(0.0f, 0.0f, 2.0f);

	// Define the modelview transformation.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, 0.0f);

	SetBack = -120.0f / tanf((float)DEG2RAD(glFovY * 0.5f));
	SetBack -= 2.0f;

	glTop = 120.0f;
	glBottom = -glTop;

	glRight = glTop * glAspect;
	glLeft = -glRight;
}

void StartRendition(void)
{
	// er?
  // Will put the OpenGL frame start code in here...
	glClear(GL_DEPTH_BUFFER_BIT);

	Set3D();

	glmode = gl_3d;
}

void StopRendition(void)
{
	if (glmode == gl_3d)
	{
		glPopMatrix();
	}
	else
	{
		Set2D();

	}

	// Swap the rendering buffers...
	SDL_GL_SwapWindow(pWindow);
}

void SetTexture(unsigned int texID)
{
}

void SetShader(unsigned int shaderID)
{
}

void RenderEntities(void)
{
}

void Set2D(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(
		(GLdouble)0,
		(GLdouble)video.width,
		(GLdouble)video.height,
		(GLdouble)0,
		(GLdouble)-1.0,
		(GLdouble)1.0
	);
	glDisable(GL_DEPTH_TEST);
}

void RenderString(vec3* vertices, GLuint* indices, GLuint numIndices)
{
}

void GetColorBuffer(GLubyte* data)
{
}

void RenderSprite(GLuint textureId, vec3* vertices, GLuint* indices, GLuint numIndices)
{
}