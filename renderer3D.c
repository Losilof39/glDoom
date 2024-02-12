#include "renderer.h"
#include "renderer3D.h"
#include "renderer2D.h"
#include "v_video.h"

extern video_t video;
extern RenderInfo s_renderinfo;

R3DStorage s_threeData;

void InitRenderer3D()
{
	s_threeData.shader = Shader_Create("wall", "shader_files/wall.vs", "shader_files/wall.ps");
	glm_perspective(glm_rad(video.fovy), (float)video.width / (float)video.height, 0.1f, 5000.0f, s_threeData.cam.projection);
    Shader_Use(s_threeData.shader);
    Shader_SetMat4(s_threeData.shader, "proj", s_threeData.cam.projection);
    Shader_Unbind();
}

void R3D_StartRendition(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void R3D_StopRendition(void)
{
}

void R3D_UpdateCamera(vec3* position, vec3 viewangle)
{
    vec3 up = { 0.0f, 1.0f, 0.0f };
    vec3 center;
    vec3 dir = { 0.0f, 0.0f, -1.0f };
    vec3 y_axis = { 0.0f, 1.0f, 0.0f };
    vec3 x_axis = { 1.0f, 0.0f, 0.0f };

    glm_vec3_rotate(dir, glm_rad(viewangle[1]), y_axis);
    glm_vec3_rotate(dir, glm_rad(viewangle[0]), x_axis);

    glm_vec3_add(position, dir, center);
    glm_lookat(position, center, up, s_threeData.cam.view);

    Shader_Use(s_threeData.shader);
    Shader_SetMat4(s_threeData.shader, "view", s_threeData.cam.view);
    Shader_Unbind();
}

void R3D_RenderWall(DW_Polygon* wall, unsigned int* tex)
{
	unsigned int indices[6] = {0, 1, 2, 2, 3, 0};

    if (wall->VAO < 0)
    {
        unsigned int VBO;

        glGenVertexArrays(1, &wall->VAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(wall->Point), wall->Point, GL_STATIC_DRAW);

        glBindVertexArray(wall->VAO);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DW_Vertex3Dv), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DW_Vertex3Dv), (void*)(3 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    Shader_Use(s_threeData.shader);
    Shader_SetInt(s_threeData.shader, "tex", 0);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *tex);
    
    glBindVertexArray(wall->VAO);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    Shader_Unbind();
}

void R3D_RenderCeil(DW_FloorCeil* ceil, unsigned int* tex)
{
    if (ceil->ceilVAO < 0)
    {
        unsigned int VBO;

        glGenVertexArrays(1, &ceil->ceilVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(DW_Vertex3Dv) * ceil->PCount, ceil->Point, GL_DYNAMIC_DRAW);

        glBindVertexArray(ceil->ceilVAO);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DW_Vertex3Dv), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DW_Vertex3Dv), (void*)(3 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    Shader_Use(s_threeData.shader);
    Shader_SetInt(s_threeData.shader, "tex", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *tex);

    glBindVertexArray(ceil->ceilVAO);

    glDrawArrays(GL_TRIANGLE_FAN, 0, ceil->PCount);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    Shader_Unbind();
}

void R3D_RenderFloor(DW_FloorCeil* floor, unsigned int* tex)
{
    if (floor->floorVAO < 0)
    {
        unsigned int VBO;

        glGenVertexArrays(1, &floor->floorVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(DW_Vertex3Dv) * floor->PCount, floor->Point, GL_DYNAMIC_DRAW);

        glBindVertexArray(floor->floorVAO);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DW_Vertex3Dv), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DW_Vertex3Dv), (void*)(3 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    Shader_Use(s_threeData.shader);
    Shader_SetInt(s_threeData.shader, "tex", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *tex);

    glBindVertexArray(floor->floorVAO);

    glDrawArrays(GL_TRIANGLE_FAN, 0, floor->PCount);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    Shader_Unbind();
}