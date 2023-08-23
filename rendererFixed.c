#include "renderer.h"
#include "rendererFixed.h"

#include "v_video.h"
#include "gl_video.h"

extern video_t video;
extern glmode_t glmode;
extern int gl_premalpha;
extern float glTop, glBottom, glRight, glLeft, SetBack;

void InitRendererFixed(sRenderer* renderer)
{
	renderer->Setup = Setup;
	renderer->Set3D = Set3D;
	renderer->SetColor = SetColor;
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

void w3sgluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
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

void SetColor(float r, float g, float b)
{
	glColor3f(r, g, b);
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
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
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

void RenderSprite(float* v, float* uv, GLuint* indices, GLuint numIndices)
{
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	glEnable(GL_BLEND);
	if (gl_premalpha)
	   {
		glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
	   }
	else
	   {
	   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	  }

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(v[0], v[2], SetBack);

	glTexCoord2f(0.0f, uv[1]);
	glVertex3f(v[0], v[3], SetBack);

	glTexCoord2f(uv[0], uv[1]);
	glVertex3f(v[1], v[3], SetBack);

	glTexCoord2f(uv[0], 1.0f);
	glVertex3f(v[1], v[2], SetBack);
	glEnd();

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
}