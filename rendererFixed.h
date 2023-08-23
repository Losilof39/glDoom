#ifndef RENDERERFIXED_H
#define RENDERERFIXED_H

#include "renderer.h"

void InitRendererFixed(sRenderer* renderer);

void Setup(void);
void Set3D(void);
void StartRendition (void);
void StopRendition(void);
void SetTexture(unsigned int);
void SetShader(unsigned int);
void RenderEntities(void);
void Set2D(void);
void RenderString(vec3* vertices, GLuint* indices, GLuint numIndices);
void GetColorBuffer(GLubyte* data);
void RenderSprite(GLuint textureId, vec3* vertices, vec2* texCoords, GLuint* indices, GLuint numIndices);

#endif