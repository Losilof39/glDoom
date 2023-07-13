// mathlib.h

#ifndef MATHLIB
#define MATHLIB

#include "tables.h"

#ifndef M_PI
#define M_PI    3.141592657f
#endif
#define DEG2RAD(x) ((x)*(M_PI/180.0f))
#define RAD2DEG(x) ((x)*(180.0f/M_PI))

#define BAM2DEG(x) ((float)x*90.0f/(float)ANG90)
#define DEG2BAM(x) ((angle_t)((float)x*(float)ANG90/90.0f))

#define YAW   1
#define PITCH 0
#define ROLL  2

typedef double ml_vec_t;

typedef ml_vec_t ml_vec3_t[3];

typedef ml_vec_t ml_vert2_t[2];
typedef ml_vec_t ml_vert3_t[3];


void ml_VectorCopy( ml_vec3_t v1, ml_vec3_t v2 );
void ml_VectorAdd( ml_vec3_t v1, ml_vec3_t v2, ml_vec3_t v3 );
void ml_VectorSub( ml_vec3_t v1, ml_vec3_t v2, ml_vec3_t v3 );
void ml_VectorRotate( ml_vec3_t angle, ml_vec3_t vector, ml_vec3_t newvec);
void ml_VectorNormalise( ml_vec3_t v );
void ml_VectorScale(ml_vec3_t vect, float scale, ml_vec3_t result);
ml_vec_t ml_DotProduct (ml_vec3_t v1, ml_vec3_t v2);
void ml_CrossProduct (ml_vec3_t v1, ml_vec3_t v2, ml_vec3_t result);
float ml_MakeFovY(float fovX, float aspect);

#endif