#ifndef RENDERER3D_H
#define RENDERER3D_H

#include "doomdata.h"
#include "gldefs.h"
#include "r_shader.h"
#include <cglm/cglm.h>

typedef struct s_Camera
{
	mat4 projection;
	mat4 view;
	vec3 position;
}Camera;

typedef struct s_R3DStorage
{
	GLuint VAO;
	Shader shader;
	Camera cam;
}R3DStorage;

// 3D draw command
typedef struct s_threedcommand
{
	DW_Polygon* wall;
	DW_FloorCeil* flat;
	float light;
	struct threedcommand* next;
}threedcommand;

void InitRenderer3D();
void R3D_StartRendition(void);
void R3D_StopRendition(void);
void R3D_UpdateCamera(vec3* position, vec3 viewangle);
void R3D_RenderWall(DW_Polygon* wall, unsigned int* tex, float light);
void R3D_RenderCeil(DW_FloorCeil* ceil, unsigned int* tex, float light);
void R3D_RenderFloor(DW_FloorCeil* floor, unsigned int* tex, float light);
void R3D_RenderThing(vec3* position, vec2 size, GLTexData* tex, float light);
void R3D_RecalcWall(DW_Polygon* wall);
void R3D_RecalcCeil(DW_FloorCeil* ceil);
void R3D_RecalcFloor(DW_FloorCeil* floor);
void R3D_DestroyRenderObjects(void);

#endif