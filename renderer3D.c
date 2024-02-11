#include "renderer3D.h"
#include "renderer2D.h"
#include "v_video.h"

extern video_t video;
extern RenderInfo s_renderinfo;

R3DStorage s_threeData;

void InitRenderer3D()
{
	glm_perspective(video.fovy, (float)s_renderinfo.virtualWidth / (float)s_renderinfo.virtualHeight, 0.1f, 100.0f, s_threeData.cam.projection);
	s_threeData.shader = Shader_Create("wall", "shader_files/wall.vs", "shader_files/wall.ps");
}

void R3D_StartRendition(void)
{
}

void R3D_StopRendition(void)
{
}

void R3D_UpdateCamera(vec3 position, vec3 dir)
{
}

void R3D_RenderWall(DW_Polygon* wall, unsigned int* tex)
{
}
