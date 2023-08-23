#ifndef R_GLCORE_H
#define R_GLCORE_H

#include <glad/glad.h>
#include <cglm.h>
#include "r_shader.h"

typedef struct DrawableQuad
{
	GLuint VBO;
	GLuint textureID;
	void* color;
}DrawableQuad;

typedef struct RenderState
{
	GLuint VAO;
	GLuint EBO;
	Shader shaderBound;
	mat4 matOrtho;
	mat4 matProj;
	int UICount;
}RenderState;

//void R_RenderTexturedQuad(float* pBuffer, GLTexData* texture_id)
//{
//    /*unsigned int vbo;
//
//    glGenBuffers(1, &vbo);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(pBuffer), pBuffer, GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderstate.EBO);
//
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(0));
//
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(3 * sizeof(float)));
//
//    DrawableQuad temp = { 0 };
//    temp.VBO = vbo;
//    temp.textureID = texture_id->TexName;
//
//    gameUI[Renderstate.UICount] = temp;
//
//    Renderstate.UICount++;*/
//
//    /*glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    Shader_Use(Renderstate.shaderBound);
//    Shader_SetInt(Renderstate.shaderBound, "texture", 0);
//
//    glBindTexture(GL_TEXTURE_2D, texture_id);
//    glActiveTexture(GL_TEXTURE0);
//
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);*/
//}
//
//void R_RenderColoredQuad(void* pBuffer, void* color)
//{
//}

#endif