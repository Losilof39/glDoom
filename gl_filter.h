/*
** File: gl_filter.h
** Project: glDoom Re
** Author: André Guilherme
** Creation Date: 08/31/2023
** Description:
** File created for create a utility that manages the 
** texture and filter used
** License: Public domain/ID License 
*/

#include "gldefs.h"

/********************************************
*********************************************
** Every typedef is based on GLAD/OpenGL
** specificaion including the hexadecimal 
** values for create the textures.
** See glad.h/gl.h to see more details!
*********************************************
*********************************************/
typedef enum
{
	gl_texture_1d = 0x0DE0,
	gl_texture_2d = 0x0DE1,
	gl_texture_width = 0x1000,
	gl_texture_height = 0x1001,
	gl_texture_border_color = 0x1004
} gltexture_t;
#ifdef NOT_YET
typedef enum
{
	gl_unpack_swap_bytes = 0x0CF0,
	gl_unpack_lsb_first = 0x0CF1,
	gl_unpack_row_length = 0x0CF2,
	gl_unpack_skip_rows = 0x0CF3,
	gl_unpack_skip_pixels = 0x0CF4,
	gl_unpack_alignment = 0x0CF5, 
} gltexture_packs_t;
#endif
void GL_GenTextures(unsigned int* texture, int texture_size, gltexture_t texture_target);
#ifdef NOT_YET
void GL_PixelStorePacks(gltexture_packs_t pack, unsigned int parameter);
#endif
dboolean GL_CreateDefaultTexture(unsigned int texture, gltexture_t texture_target, int width, int height, const void *pixels, dboolean enable_tex, dboolean copy, dboolean use_rgba, dboolean activated);
dboolean GL_CreateClampTexture(gltexture_t texture_target, int width, int height, const void* pixels, dboolean activated);