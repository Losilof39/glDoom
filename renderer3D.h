#ifndef RENDERER3D_H
#define RENDERER3D_H

void InitRenderer3D();
void R3D_Set3D(void);
void R3D_SetColor(float r, float g, float b);
void R3D_StartRendition (void);
void R3D_StopRendition(void);
void R3D_UpdateUniforms(void);
//void R3D_SetMaterial(material* mat);
void R3D_RenderEntities(void);
void R3D_RenderWall(void);
void R3D_RenderFlat(void);

#endif