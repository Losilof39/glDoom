#ifndef RENDERERCORE_H
#define RENDERERCORE_H

#include "renderer.h"

void InitRendererCore(sRenderer* renderer);

void cSetup(void);
void cSet3D(void);
void cSetColor(float r, float g, float b);
void cStartRendition(void);
void cStopRendition(void);
void cSetTexture(unsigned int);
void cSetShader(unsigned int);
void cRenderEntities(void);
void cSet2D(void);
void cRenderString(vec3* vertices, GLuint* indices, GLuint numIndices);
void cGetColorBuffer(GLubyte* data);
void cRenderSprite(float* v, float* uv, GLuint* indices, GLuint numIndices);

#endif
