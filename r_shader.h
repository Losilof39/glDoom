#ifndef R_SHADER_H
#define R_SHADER_H


#include <cglm/cglm.h>
#include "doomtype.h"

typedef struct
{
	const char* name;
	GLuint programID;
} Shader;

char* Shader_GetSource(const char* fileName);
Shader Shader_Create(const char* name, const char* pVertPath, const char* pFragPath);
void Shader_Use(Shader shader);
void Shader_SetInt(Shader shader, const char* uniform, int value);
void Shader_SetBoolean(Shader shader, const char* uniform, dboolean value);
void Shader_SetFloat(Shader shader, const char* uniform, float value);
void Shader_SetMat4(Shader shader, const char* uniform, mat4 value);
void Shader_SetVec3(Shader shader, const char* uniform, vec3 value);

#endif