// mathlib.c
// routines for handling vectors, etc.
//
#include <math.h>
#include "mathlib.h"
#include <string.h>

ml_vec3_t identity[3] = { 1.0, 0.0, 0.0,
                          0.0, 1.0, 0.0,
                          0.0, 0.0, 1.0 };

void ml_VectorCopy( ml_vec3_t v1, ml_vec3_t v2)
   {
    v2[0] = v1[0];
    v2[1] = v1[1];
    v2[2] = v1[2];
   }

void ml_VectorAdd( ml_vec3_t v1, ml_vec3_t v2, ml_vec3_t v3)
   {
    v3[0] = v1[0] + v2[0];
    v3[1] = v1[1] + v2[1];
    v3[2] = v1[2] + v2[2];
   }

void ml_VectorSub( ml_vec3_t v1, ml_vec3_t v2, ml_vec3_t v3)
   {
    v3[0] = v1[0] - v2[0];
    v3[1] = v1[1] - v2[1];
    v3[2] = v1[2] - v2[2];
   }

void ml_MakeIdentity( ml_vec3_t *mat )
   {
    memcpy(mat, identity, sizeof(ml_vec3_t)*3);
   }

// apply rotations contained in angles to vector and put into newvec
void ml_VectorRotate(ml_vec3_t vector, ml_vec3_t angles, ml_vec3_t newvec)
   {
    double      angle;
    double      sp, cp, sy, cy;
#ifdef VIEWROLL
    double cr, sr;
#endif

    ml_vec3_t xresult;
#ifdef VIEWROLL
    ml_vec3_t yresult;
#endif
    ml_vec3_t  xmatrix[3], ymatrix[3];
#ifdef VIEWROLL
    ml_vec3_t  zmatrix[3];
#endif

    ml_MakeIdentity(xmatrix);
    ml_MakeIdentity(ymatrix);

    angle = DEG2RAD(angles[PITCH]);
	sp = sin(angle);
	cp = cos(angle);
    xmatrix[1][1] = cp;
    xmatrix[2][2] = cp;
    xmatrix[2][1] = -sp;
    xmatrix[1][2] = sp;

	angle = DEG2RAD(angles[YAW]);
	sy = sin(angle);
	cy = cos(angle);
    ymatrix[0][0] = cy;
    ymatrix[0][2] = -sy;
    ymatrix[2][0] = sy;
    ymatrix[2][2] = cy;

#ifdef VIEWROLL
    ml_MakeIdentity(zmatrix);
	angle = DEG2RAD(angles[ROLL]);
	sr = sin(angle);
	cr = cos(angle);
    zmatrix[0][0] = cr;
    zmatrix[1][1] = cr;
    zmatrix[0][1] = sr;
    zmatrix[1][0] = -sr;
#endif

/*
    xresult[0] = vector[0]*xmatrix[0][0] + vector[1]*xmatrix[1][0] + vector[2]*xmatrix[2][0];
    xresult[1] = vector[0]*xmatrix[0][1] + vector[1]*xmatrix[1][1] + vector[2]*xmatrix[2][1];
    xresult[2] = vector[0]*xmatrix[0][2] + vector[1]*xmatrix[1][2] + vector[2]*xmatrix[2][2];
    
#ifdef VIEWROLL
    yresult[0] = xresult[0]*ymatrix[0][0] + xresult[1]*ymatrix[1][0] + xresult[2]*ymatrix[2][0];
    yresult[1] = xresult[0]*ymatrix[0][1] + xresult[1]*ymatrix[1][1] + xresult[2]*ymatrix[2][1];
    yresult[2] = xresult[0]*ymatrix[0][2] + xresult[1]*ymatrix[1][2] + xresult[2]*ymatrix[2][2];

	newvec[0] = yresult[0]*zmatrix[0][0] + yresult[1]*zmatrix[1][0] + yresult[2]*zmatrix[2][0];
	newvec[1] = yresult[0]*zmatrix[0][1] + yresult[1]*zmatrix[1][1] + yresult[2]*zmatrix[2][1];
	newvec[2] = yresult[0]*zmatrix[0][2] + yresult[1]*zmatrix[1][2] + yresult[2]*zmatrix[2][2];
#else
    newvec[0] = xresult[0]*ymatrix[0][0] + xresult[1]*ymatrix[1][0] + xresult[2]*ymatrix[2][0];
    newvec[1] = xresult[0]*ymatrix[0][1] + xresult[1]*ymatrix[1][1] + xresult[2]*ymatrix[2][1];
    newvec[2] = xresult[0]*ymatrix[0][2] + xresult[1]*ymatrix[1][2] + xresult[2]*ymatrix[2][2];
#endif
*/

// optimize by taking out obvious calculations
    xresult[0] = vector[0];
    xresult[1] = vector[1]*xmatrix[1][1] + vector[2]*xmatrix[2][1];
    xresult[2] = vector[1]*xmatrix[1][2] + vector[2]*xmatrix[2][2];
    
#ifdef VIEWROLL
    yresult[0] = xresult[0]*ymatrix[0][0] +                            xresult[2]*ymatrix[2][0];
    yresult[1] =                            xresult[1];
    yresult[2] = xresult[0]*ymatrix[0][2] +                            xresult[2]*ymatrix[2][2];

	newvec[0] = yresult[0]*zmatrix[0][0] + yresult[1]*zmatrix[1][0] + yresult[2]*zmatrix[2][0];
	newvec[1] = yresult[0]*zmatrix[0][1] + yresult[1]*zmatrix[1][1] + yresult[2]*zmatrix[2][1];
	newvec[2] = yresult[0]*zmatrix[0][2] + yresult[1]*zmatrix[1][2] + yresult[2]*zmatrix[2][2];
#else
    newvec[0] = xresult[0]*ymatrix[0][0] +                            xresult[2]*ymatrix[2][0];
    newvec[1] =                            xresult[1];
    newvec[2] = xresult[0]*ymatrix[0][2] +                            xresult[2]*ymatrix[2][2];
#endif
   }

// "de-scale" vector into a unit vector
void ml_VectorNormalise(ml_vec3_t v)
   {
    double length;

    length = sqrt((v[0]*v[0]) + (v[1]*v[1]) + (v[2]*v[2]));
    if (length != 0.0)
       {
        v[0] /= length;
        v[1] /= length;
        v[2] /= length;
       }
   }

ml_vec_t ml_DotProduct (ml_vec3_t v1, ml_vec3_t v2)
   {
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
   }

void ml_VectorScale(ml_vec3_t vect, float scale, ml_vec3_t result)
   {
    result[0] = vect[1] * scale;
    result[1] = vect[2] * scale;
    result[2] = vect[0] * scale;
   }

void ml_CrossProduct (ml_vec3_t v1, ml_vec3_t v2, ml_vec3_t result)
   {
    result[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    result[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    result[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
   }


float ml_MakeFovY(float fovX, float aspect)
{
    float rad = atanf(tanf(DEG2RAD(fovX * 0.5f)) / aspect);

    return RAD2DEG(rad) * 2.0f;
}
