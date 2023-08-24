#ifndef RENDERERFIXED_H
#define RENDERERFIXED_H

#include "renderer.h"

void InitRendererFixed(sRenderer* renderer);

void Setup(void);
void Set3D(void);
void SetColor(float r, float g, float b);
void StartRendition (void);
void StopRendition(void);
void SetTexture(unsigned int);
void SetShader(unsigned int);
void RenderEntities(void);
void Set2D(void);
void RenderString(vec3* vertices, GLuint* indices, GLuint numIndices);
void GetColorBuffer(GLubyte* data);
#ifdef NOT_YET
void RenderSprite(float* v, float* uv, GLuint* indices, GLuint numIndices);
#else
void RenderSprite(float* v, GLTexData* tex);
#endif

#endif