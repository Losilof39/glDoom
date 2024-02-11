#ifndef RENDERER3D_H
#define RENDERER3D_H

#include "doomdata.h"
#include "gldefs.h"
#include <cglm/cglm.h>

void InitRenderer3D();
void R3D_Set3D(void);
void R3D_SetColor(float r, float g, float b);
void R3D_StartRendition(void);
void R3D_StopRendition(void);
void R3D_UpdateUniforms(void);
void R3D_SetMaterial(const int* tex);
void R3D_RenderEntities(void);
void R3D_RenderWall(DW_Polygon* wall);
void R3D_RenderFlat(DW_FloorCeil* flat);
void R3D_RenderThing(vec3* position, vec2 size, GLTexData* tex);

#endif