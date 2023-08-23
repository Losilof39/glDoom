#include "renderer.h"
#include "rendererFixed.h"

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

}

void StartRendition(void)
{
}

void StopRendition(void)
{
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