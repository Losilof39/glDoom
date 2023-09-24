#include <glad/glad.h>
#include "r_shader.h"
#include <string.h>

char* Shader_GetSource(const char* fileName)
{
    FILE* fp;
    long size = 0;
    char* shaderContent;

    /* Read File to get size */
    fp = fopen(fileName, "rb");
    if (fp == NULL) {
        printf("Failed to read shader: %s\n", fileName);
        return "";
    }
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp) + 1;
    fclose(fp);

    /* Read File for Content */
    fp = fopen(fileName, "r");
    shaderContent = memset(malloc(size), '\0', size);
    fread(shaderContent, 1, size - 1, fp);
    fclose(fp);

    return shaderContent;
}

Shader Shader_Create(const char* name, const char* pVertPath, const char* pFragPath)
{
    GLuint mVertShader, mFragShader;
    const char* VertSource;
    const char* FragSource;
    char* InfoLog[512] = { 0 };
    Shader shader = { 0 };
    int success;

    shader.name = name;

    VertSource = Shader_GetSource(pVertPath);
    FragSource = Shader_GetSource(pFragPath);


    // create shaders and program
    mVertShader = glCreateShader(GL_VERTEX_SHADER);
    mFragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(mVertShader, 1, (const GLchar**)&VertSource, NULL);
    glShaderSource(mFragShader, 1, (const GLchar**)&FragSource, NULL);

    glCompileShader(mVertShader);
    glCompileShader(mFragShader);

    // Vertex Shader
    glGetShaderiv(mVertShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(mVertShader, 512, NULL, InfoLog);
        printf("[ERROR] Failed to compile vertex shader: %s", InfoLog);
    }

    // Fragment Shader
    glGetShaderiv(mFragShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(mFragShader, 512, NULL, InfoLog);
        printf("[ERROR] Failed to compile fragment shader: %s", InfoLog);
    }

    shader.programID = glCreateProgram();
    glAttachShader(shader.programID, mVertShader);
    glAttachShader(shader.programID, mFragShader);

    glLinkProgram(shader.programID);

    glDeleteShader(mVertShader);
    glDeleteShader(mFragShader);

    // check for compiler errors in compiling shaders
    glGetShaderiv(shader.programID, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader.programID, 512, NULL, InfoLog);
        printf("[ERROR] Failed to compile shaders: %s", InfoLog);
    }

    return shader;
}

void Shader_Use(Shader shader)
{
    glUseProgram(shader.programID);
}

void Shader_Unbind()
{
    glUseProgram(0);
}

void Shader_SetInt(Shader shader, const char* uniform, int value)
{
    glUniform1i(glGetUniformLocation(shader.programID, uniform), value);
}

void Shader_SetBoolean(Shader shader, const char* uniform, dboolean value)
{
    glUniform1i(glGetUniformLocation(shader.programID, uniform), (GLint)value);
}

void Shader_SetFloat(Shader shader, const char* uniform, float value)
{
    glUniform1f(glGetUniformLocation(shader.programID, uniform), value);
}

void Shader_SetMat4(Shader shader, const char* uniform, mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.programID, uniform), 1, GL_FALSE, value[0]);
}

void Shader_SetVec3(Shader shader, const char* uniform, vec3 value)
{
    glUniform3fv(glGetUniformLocation(shader.programID, uniform), 1, value);
}

void Shader_SetVec4(Shader shader, const char* uniform, vec4 value)
{
    glUniform4fv(glGetUniformLocation(shader.programID, uniform), 1, value);
}
