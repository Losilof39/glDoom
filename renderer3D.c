#include "renderer.h"
#include "renderer3D.h"
#include "renderer2D.h"
#include "v_video.h"
#include "z_zone.h"

extern video_t video;
extern RenderInfo s_renderinfo;

extern sector_plane_t**    sorted_flats;
extern int                 sorted_flats_count;
extern DW_Polygon          **sorted_walls;
extern int                 sorted_walls_count;
extern sector_plane_t*     planes;
extern sector_t*           sectors;
extern player_t            players[MAXPLAYERS];
extern int                 displayplayer;

threedcommand* render_types[R3D_RENDER_TYPE_COUNT] = { NULL };

// indices used only for quad rendering
unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };

R3DStorage s_threeData;

void InitRenderer3D()
{
    float vertices[] = {
        // pos      // tex
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
    };

    glm_mat4_identity(s_threeData.ortho);

    glGenVertexArrays(1, &s_threeData.quadVAO);
    glGenBuffers(1, &s_threeData.quadVBO);

    glBindBuffer(GL_ARRAY_BUFFER, s_threeData.quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(s_threeData.quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	s_threeData.polygonShader = Shader_Create("polygon", "shader_files/polygon.vs", "shader_files/polygon.ps");
    s_threeData.thingShader = Shader_Create("thing", "shader_files/thing.vs", "shader_files/thing.ps");
    s_threeData.skyShader = Shader_Create("sky", "shader_files/sky.vs", "shader_files/sky.ps");

	glm_perspective(glm_rad(video.fovy), (float)video.width / (float)video.height, 0.1f, 5000.0f, s_threeData.cam.projection);

    glm_ortho(0.0f,
        (float)s_renderinfo.virtualWidth,
        (float)s_renderinfo.virtualHeight,
        0.0f,
        -1.0f,
        1.0f,
        s_threeData.ortho);


    Shader_Use(s_threeData.polygonShader);
    Shader_SetMat4(s_threeData.polygonShader, "proj", s_threeData.cam.projection);

    Shader_Use(s_threeData.thingShader);
    Shader_SetMat4(s_threeData.thingShader, "proj", s_threeData.cam.projection);
    Shader_SetInt(s_threeData.thingShader, "tex", 0);

    Shader_Use(s_threeData.skyShader);
    Shader_SetInt(s_threeData.skyShader, "image", 0);
    Shader_SetMat4(s_threeData.skyShader, "ortho", s_threeData.ortho);


    Shader_Unbind();
}

void R3D_UpdateCamera(vec3* position, vec3 viewangle)
{
    vec3 center;
    vec3 dir = { 0.0f, 0.0f, -1.0f };

    glm_vec3_rotate(dir, glm_rad(viewangle[1]), (vec3) { 0.0f, 1.0f, 0.0f });
    glm_vec3_rotate(dir, glm_rad(viewangle[0]), (vec3) { 1.0f, 0.0f, 0.0f });

    glm_vec3_add(position, dir, center);
    glm_lookat(position, center, (vec3) { 0.0f, 1.0f, 0.0f }, s_threeData.cam.view);

    Shader_Use(s_threeData.polygonShader);
    Shader_SetMat4(s_threeData.polygonShader, "view", s_threeData.cam.view);

    Shader_Use(s_threeData.thingShader);
    Shader_SetMat4(s_threeData.thingShader, "view", s_threeData.cam.view);

    Shader_Unbind();
}

// push back 3D draw command and return its pointer
threedcommand* R3D_AddDrawCmd(threedcommand** list, R3D_RENDER_TYPE type)
{
    threedcommand* newNode = (threedcommand*)Z_Malloc(sizeof(threedcommand), PU_LEVEL, NULL);

    if (render_types[type] == NULL) {
        // If the list is empty, set the new node as the head
        render_types[type] = newNode;
    }
    else {
        // Traverse to the end of the list and append the new node
        threedcommand* current = render_types[type];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    newNode->next = NULL;

    return newNode;
}

void R3D_FlushCommandList(threedcommand** cmd)
{
    threedcommand* cur, *nextNode;

    for (unsigned int type = 0; type < R3D_RENDER_TYPE_COUNT; type++)
    {
        cur = render_types[type];

        // Free each node in the list
        while (cur != NULL) {
            nextNode = cur->next;
            Z_Free(cur);
            cur = nextNode;
        }

        // Set the head to NULL to indicate an empty list
        render_types[type] = NULL;
    }
}

void R3D_RecalcPoly(DW_Polygon* wall)
{
    if (wall->VAO >= 0)
    {
        glBindVertexArray(wall->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, wall->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, (void*)0, sizeof(wall->Point), wall->Point);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void R3D_RecalcCeil(DW_FloorCeil* ceil)
{
    if (ceil->ceilVAO >= 0)
    {
        glBindVertexArray(ceil->ceilVAO);
        glBindBuffer(GL_ARRAY_BUFFER, ceil->ceilVBO);
        glBufferSubData(GL_ARRAY_BUFFER, (void*)0, sizeof(DW_Vertex3Dv) * ceil->PCount, ceil->Point);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void R3D_RecalcFloor(DW_FloorCeil* floor)
{
    if (floor->floorVAO >= 0)
    {
        glBindVertexArray(floor->floorVAO);
        glBindBuffer(GL_ARRAY_BUFFER, floor->floorVBO);
        glBufferSubData(GL_ARRAY_BUFFER, (void*)0, sizeof(DW_Vertex3Dv) * floor->PCount, floor->Point);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void R3D_RenderWall(DW_Polygon* wall, unsigned int* tex, float light)
{
    threedcommand* newNode = R3D_AddDrawCmd(render_types, R3D_RENDER_WALL);
    mat4 model;

    glm_mat4_identity(model);

    // build vao and vbo if not already
    if (wall->VAO < 0)
    {
        glGenVertexArrays(1, &wall->VAO);
        glGenBuffers(1, &wall->VBO);

        glBindBuffer(GL_ARRAY_BUFFER, wall->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(wall->Point), wall->Point, GL_DYNAMIC_DRAW);

        glBindVertexArray(wall->VAO);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DW_Vertex3Dv), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DW_Vertex3Dv), (void*)(3 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    R3D_RecalcPoly(wall);

    wall->tex = tex;

    newNode->polygon = wall;
    newNode->light = light;
    newNode->flat = NULL;

    /*newNode->next = NULL;*/
}

void R3D_RenderCeil(DW_FloorCeil* ceil, unsigned int* tex, float light)
{
    threedcommand* newNode = R3D_AddDrawCmd(render_types, R3D_RENDER_CEIL);

    if (ceil->ceilVAO < 0)
    {
        glGenVertexArrays(1, &ceil->ceilVAO);
        glGenBuffers(1, &ceil->ceilVBO);

        glBindBuffer(GL_ARRAY_BUFFER, ceil->ceilVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(DW_Vertex3Dv) * ceil->PCount, ceil->Point, GL_DYNAMIC_DRAW);

        glBindVertexArray(ceil->ceilVAO);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DW_Vertex3Dv), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DW_Vertex3Dv), (void*)(3 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ceil->texCeil = tex;

    R3D_RecalcCeil(ceil);

    newNode->polygon = NULL;
    newNode->flat = ceil;
    newNode->light = light;
    /*newNode->next = NULL;*/
}

void R3D_RenderFloor(DW_FloorCeil* floor, unsigned int* tex, float light)
{
    threedcommand* newNode = R3D_AddDrawCmd(render_types, R3D_RENDER_FLOOR);

    if (floor->floorVAO < 0)
    {
        glGenVertexArrays(1, &floor->floorVAO);
        glGenBuffers(1, &floor->floorVBO);

        glBindBuffer(GL_ARRAY_BUFFER, floor->floorVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(DW_Vertex3Dv) * floor->PCount, floor->Point, GL_DYNAMIC_DRAW);

        glBindVertexArray(floor->floorVAO);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DW_Vertex3Dv), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(DW_Vertex3Dv), (void*)(3 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    R3D_RecalcFloor(floor);

    floor->texFloor = tex;

    newNode->polygon = NULL;
    newNode->flat = floor;
    newNode->light = light;
    /*newNode->next = NULL;*/
}

void R3D_RenderThing(vec3 pos, GLTexData* tex, float light, float angle, int mirror)
{
    threedcommand* newNode = R3D_AddDrawCmd(render_types, R3D_RENDER_BILLBOARD);
    mat4 model;
    vec2 size = { tex->glWidth,  tex->glHeight };

    glm_mat4_identity(model);

    glm_translate(model, pos);

    glm_translate(model, (vec3){ 0.0f, size[1], 0.0f });
    glm_rotate(model, glm_rad(angle), (vec3) { 0.0f, 1.0f, 0.0f });
    glm_translate(model, (vec3) { -0.5f * size[0], -size[1], 0.0f });

    glm_scale(model, size);

    newNode->tex = tex;
    newNode->light = light;
    glm_mat4_copy(model, newNode->model);
    newNode->flat = NULL;
    newNode->polygon = NULL;
    newNode->mirror = mirror;

    /*newNode->next = NULL;*/
}

void R3D_RenderSky(float angle, GLTexData* tex)
{
    threedcommand* newNode = R3D_AddDrawCmd(render_types, R3D_RENDER_SKY);
    mat4 model;
    vec2 size = { tex->glWidth, tex->glHeight };

    glm_mat4_identity(model);

    glm_scale(model, size);

    newNode->tex = tex;
    newNode->angle = glm_rad(angle) / (M_PI) * 2;
    glm_mat4_copy(model, newNode->model);
}

void R3D_StartRendition(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void R3D_StopRendition(void)
{
    threedcommand* cur;
    mat4 model;

    glm_mat4_identity(model);

    // setup shader used for wall, ceil and floor rendering
    Shader_Use(s_threeData.polygonShader);
    Shader_SetInt(s_threeData.polygonShader, "tex", 0);
   
    glEnable(GL_DEPTH_TEST);

    // loop through all render types
    for (unsigned int type = 0; type < R3D_RENDER_TYPE_COUNT; type++)
    {
        cur = render_types[type];
        
        if (cur == NULL)
            continue;

        switch (type)
        {
            case R3D_RENDER_SKY:
            {
                glDisable(GL_DEPTH_TEST);

                Shader_Use(s_threeData.skyShader);
                Shader_SetMat4(s_threeData.skyShader, "model", cur->model);
                Shader_SetFloat(s_threeData.skyShader, "angle", cur->angle);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, cur->tex->TexName);

                glBindVertexArray(s_threeData.quadVAO);

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

                glBindVertexArray(0);
                glBindTexture(GL_TEXTURE_2D, 0);

                glEnable(GL_DEPTH_TEST);

            }break;


            case R3D_RENDER_WALL:
            {
                Shader_Use(s_threeData.polygonShader);

                while (cur != NULL)
                {
                    
                    Shader_SetFloat(s_threeData.polygonShader, "light", cur->light);

                    if (cur->polygon->Position)
                    {
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, *cur->polygon->tex);

                        glBindVertexArray(cur->polygon->VAO);

                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

                        glBindVertexArray(0);
                        glBindTexture(GL_TEXTURE_2D, 0);
                    }
                    else
                    {
                        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, *cur->polygon->tex);

                        glBindVertexArray(cur->polygon->VAO);

                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

                        glBindVertexArray(0);
                        glBindTexture(GL_TEXTURE_2D, 0);

                        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                    }
                    

                    cur = cur->next;
                }
                
            }break;

            case R3D_RENDER_CEIL:
            {
                Shader_Use(s_threeData.polygonShader);

                while (cur != NULL)
                {
                    Shader_SetFloat(s_threeData.polygonShader, "light", cur->light);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, *cur->flat->texCeil);

                    glBindVertexArray(cur->flat->ceilVAO);

                    glDrawArrays(GL_TRIANGLE_FAN, 0, cur->flat->PCount);

                    glBindVertexArray(0);
                    glBindTexture(GL_TEXTURE_2D, 0);

                    cur = cur->next;
                }
            }break;

            case R3D_RENDER_FLOOR:
            {
                Shader_Use(s_threeData.polygonShader);

                while (cur != NULL)
                {
                    Shader_SetFloat(s_threeData.polygonShader, "light", cur->light);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, *cur->flat->texFloor);

                    glBindVertexArray(cur->flat->floorVAO);

                    glDrawArrays(GL_TRIANGLE_FAN, 0, cur->flat->PCount);

                    glBindVertexArray(0);
                    glBindTexture(GL_TEXTURE_2D, 0);

                    cur = cur->next;
                }
            }break;

            case R3D_RENDER_BILLBOARD:
            {
                Shader_Use(s_threeData.thingShader);
                
                while (cur != NULL)
                {
                    Shader_SetMat4(s_threeData.thingShader, "model", cur->model);
                    Shader_SetFloat(s_threeData.thingShader, "light", cur->light);
                    Shader_SetInt(s_threeData.thingShader, "mirror", cur->mirror);

                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, cur->tex->TexName);

                    glBindVertexArray(s_threeData.quadVAO);

                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

                    glBindVertexArray(0);
                    glBindTexture(GL_TEXTURE_2D, 0);

                    cur = cur->next;
                }
            }break;

        default:
            break;
        }
    }

    Shader_Unbind();


    R3D_FlushCommandList(render_types);
}

void R3D_DestroyRenderObjects(void)
{
    DW_Polygon* curWall;
    DW_FloorCeil* curFlat;
    sector_t* psector;
    sector_plane_t* pplane;
    int sector;

    for (unsigned int wall = 0; wall < sorted_walls_count; wall++)
    {
        curWall = sorted_walls[wall];


        while(curWall != NULL)
        {
            if (curWall->VAO >= 0)
            {
                glDeleteVertexArrays(1, &curWall->VAO);
                glDeleteBuffers(1, &curWall->VBO);

                curWall->VAO = -1;
                curWall->VBO = -1;
            }

            curWall = curWall->Next;
        }
        
    }

    for (unsigned int flat = 0; flat < sorted_flats_count; flat++)
    {
        pplane = sorted_flats[flat];
        sector = pplane - planes;
        psector = sectors + sector;

        for (unsigned int subsector = 0; subsector < pplane->ss_count; subsector++)
        {
            curFlat = pplane->subsectors[subsector];

            if (curFlat != NULL)
            {
                if (curFlat->ceilVAO >= 0)
                {
                    glDeleteVertexArrays(1, &curFlat->ceilVAO);
                    glDeleteBuffers(1, &curFlat->ceilVBO);

                    curFlat->ceilVAO = -1;
                    curFlat->ceilVBO = -1;
                }

                if (curFlat->floorVAO >= 0)
                {
                    glDeleteVertexArrays(1, &curFlat->floorVAO);
                    glDeleteBuffers(1, &curFlat->floorVBO);

                    curFlat->floorVAO = -1;
                    curFlat->floorVBO = -1;
                }
            }
            
        }
    }
}