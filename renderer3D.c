#include "renderer.h"
#include "renderer3D.h"
#include "renderer2D.h"
#include "v_video.h"

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

threedcommand* head = NULL;

// indices used only for things rendering
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

    glGenVertexArrays(1, &s_threeData.thingVAO);
    glGenBuffers(1, &s_threeData.thingVBO);

    glBindBuffer(GL_ARRAY_BUFFER, s_threeData.thingVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(s_threeData.thingVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	s_threeData.shader = Shader_Create("polygon", "shader_files/polygon.vs", "shader_files/polygon.ps");
    s_threeData.thingShader = Shader_Create("thing", "shader_files/thing.vs", "shader_files/thing.ps");
	glm_perspective(glm_rad(video.fovy), (float)video.width / (float)video.height, 0.1f, 5000.0f, s_threeData.cam.projection);

    Shader_Use(s_threeData.shader);
    Shader_SetMat4(s_threeData.shader, "proj", s_threeData.cam.projection);

    Shader_Use(s_threeData.thingShader);
    Shader_SetMat4(s_threeData.thingShader, "proj", s_threeData.cam.projection);
    Shader_SetInt(s_threeData.thingShader, "tex", 0);

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

    Shader_Use(s_threeData.shader);
    Shader_SetMat4(s_threeData.shader, "view", s_threeData.cam.view);

    Shader_Use(s_threeData.thingShader);
    Shader_SetMat4(s_threeData.thingShader, "view", s_threeData.cam.view);

    Shader_Unbind();
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
    threedcommand* newNode = (threedcommand*)malloc(sizeof(threedcommand));
    mat4 model;

    glm_mat4_identity(model);

    if (head == NULL) {
        // If the list is empty, set the new node as the head
        head = newNode;
    }
    else {
        // Traverse to the end of the list and append the new node
        threedcommand* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

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

    newNode->next = NULL;
}

void R3D_RenderCeil(DW_FloorCeil* ceil, unsigned int* tex, float light)
{
    threedcommand* newNode = (threedcommand*)malloc(sizeof(threedcommand));

    if (head == NULL) {
        // If the list is empty, set the new node as the head
        head = newNode;
    }
    else {
        // Traverse to the end of the list and append the new node
        threedcommand* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

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
    newNode->next = NULL;
}

void R3D_RenderFloor(DW_FloorCeil* floor, unsigned int* tex, float light)
{
    threedcommand* newNode = (threedcommand*)malloc(sizeof(threedcommand));

    if (head == NULL) {
        // If the list is empty, set the new node as the head
        head = newNode;
    }
    else {
        // Traverse to the end of the list and append the new node
        threedcommand* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

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
    newNode->next = NULL;
}

void R3D_RenderThing(vec3 pos, GLTexData* tex, float light, float angle, int mirror)
{
    threedcommand* newNode = (threedcommand*)malloc(sizeof(threedcommand));
    mat4 model;
    vec2 size = { tex->glWidth,  tex->glHeight };

    glm_mat4_identity(model);

    if (head == NULL) {
        // If the list is empty, set the new node as the head
        head = newNode;
    }
    else {
        // Traverse to the end of the list and append the new node
        threedcommand* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

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

    newNode->next = NULL;
}

void R3D_StartRendition(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void R3D_StopRendition(void)
{
    threedcommand* cur, * nextNode;
    mat4 model;

    glm_mat4_identity(model);

    cur = head;

    // setup shader used for wall, ceil and floor rendering
    Shader_Use(s_threeData.shader);
    Shader_SetInt(s_threeData.shader, "tex", 0);
   
    glEnable(GL_DEPTH_TEST);

    // iterate through all 3D draw commands
    while (cur != NULL)
    {
        // it's wall
        if (cur->polygon)
        {
            Shader_Use(s_threeData.shader);
            Shader_SetFloat(s_threeData.shader, "light", cur->light);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, *cur->polygon->tex);

            glBindVertexArray(cur->polygon->VAO);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        // it's a flat, which could contain a floor, a ceiling or both
        else if (cur->flat)
        {
            Shader_Use(s_threeData.shader);
            Shader_SetFloat(s_threeData.shader, "light", cur->light);

            // it's ceiling
            if (cur->flat->ceilVAO > 0)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, *cur->flat->texCeil);

                glBindVertexArray(cur->flat->ceilVAO);

                glDrawArrays(GL_TRIANGLE_FAN, 0, cur->flat->PCount);

                glBindVertexArray(0);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            
            // it's floor
            if (cur->flat->floorVAO > 0)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, *cur->flat->texFloor);

                glBindVertexArray(cur->flat->floorVAO);

                glDrawArrays(GL_TRIANGLE_FAN, 0, cur->flat->PCount);

                glBindVertexArray(0);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }
        // draw thing / masked
        else
        {
            Shader_Use(s_threeData.thingShader);
            Shader_SetMat4(s_threeData.thingShader, "model", cur->model);
            Shader_SetFloat(s_threeData.thingShader, "light", cur->light);
            Shader_SetInt(s_threeData.thingShader, "mirror", cur->mirror);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, cur->tex->TexName);

            glBindVertexArray(s_threeData.thingVAO);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        cur = cur->next;
    }

    Shader_Unbind();

    cur = head;

    // Free each node in the list
    while (cur != NULL) {
        nextNode = cur->next;
        free(cur);
        cur = nextNode;
    }

    // Set the head to NULL to indicate an empty list
    head = NULL;
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