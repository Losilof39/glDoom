// Emacs style mode select   -*- C -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//	Refresh of things, i.e. objects represented by sprites.
//
//-----------------------------------------------------------------------------


static const char
rcsid[] = "$Id: r_things.c,v 1.5 1997/02/03 16:47:56 b1 Exp $";


#include <stdio.h>
#include <stdlib.h>

#include "doomdef.h"
#include "m_swap.h"

#include "i_system.h"
#include "z_zone.h"
#include "w_wad.h"

#include "r_local.h"

#include "doomstat.h"
#include <glad/glad.h>
#include "gldefs.h"
#include "renderer2D.h"

#define MINZ				(FRACUNIT*4)
//#define BASEYCENTER			(SCREENHEIGHT/2)
#define BASEYCENTER			(100-((SCREENHEIGHT-200)/12))
//void R_DrawColumn (void);
//void R_DrawFuzzColumn (void);

typedef struct
{
    int		x1;
    int		x2;
	
    int		column;
    int		topclip;
    int		bottomclip;

} maskdraw_t;



//
// Sprite rotation 0 is facing the viewer,
//  rotation 1 is one angle turn CLOCKWISE around the axis.
// This is not the same as the angle,
//  which increases counter clockwise (protractor).
// There was a lot of stuff grabbed wrong, so I changed it...
//
fixed_t		pspritescale;
fixed_t		pspriteiscale;

lighttable_t**	spritelights;

// constant arrays
//  used for psprite clipping and initializing clipping
short      *negonearray;
short      *screenheightarray;
//short		negonearray[SCREENWIDTH];
//short		screenheightarray[SCREENWIDTH];


//
// INITIALIZATION FUNCTIONS
//

// variables used to look up
//  and range check thing_t sprites patches
spritedef_t*	sprites;
int		numsprites;

spriteframe_t	sprtemp[29];
int		        maxframe;
char           *spritename;

extern int      gl_fog;
extern int      gl_modeltest;
extern int      gl_alphatest;
extern int      gl_premalpha;

//
// R_InstallSpriteLump
// Local function for R_InitSprites.
//
void R_InstallSpriteLump( int lump, unsigned frame, unsigned rotation, dboolean flipped )
   {
    int		r;
	
    if (frame >= 29 || rotation > 8)
        I_Error("R_InstallSpriteLump: Bad frame characters in lump %i", lump);
	
    if ((int)frame > maxframe)
        maxframe = frame;
		
    if (rotation == 0)
       {
        // the lump should be used for all rotations
        if (sprtemp[frame].rotate == false)
            I_Error ("R_InitSprites: Sprite %s frame %c has multip rot=0 lump", spritename, 'A'+frame);

        if (sprtemp[frame].rotate == true)
            I_Error ("R_InitSprites: Sprite %s frame %c has rotations and a rot=0 lump", spritename, 'A'+frame);
			
        sprtemp[frame].rotate = false;
        for (r = 0; r < 8; r++)
           {
            sprtemp[frame].lump[r] = lump - firstspritelump;
            sprtemp[frame].flip[r] = (dbyte)flipped;
           }
        return;
       }
	
    // the lump is only used for one rotation
    if (sprtemp[frame].rotate == false)
        I_Error ("R_InitSprites: Sprite %s frame %c has rotations and a rot=0 lump", spritename, 'A'+frame);
		
    sprtemp[frame].rotate = true;

    // make 0 based
    rotation--;		
    if (sprtemp[frame].lump[rotation] != -1)
        I_Error ("R_InitSprites: Sprite %s : %c : %c has two lumps mapped to it", spritename, 'A'+frame, '1'+rotation);
		
    sprtemp[frame].lump[rotation] = lump - firstspritelump;
    sprtemp[frame].flip[rotation] = (dbyte)flipped;
   }

int        NumSpriteLumps;
int       *SpriteLumps;
GLTexData *SprData;
/*
    "TROO","SHTG","PUNG","PISG","PISF","SHTF","SHT2","CHGG","CHGF","MISG",
    "MISF","SAWG","PLSG","PLSF","BFGG","BFGF","BLUD","PUFF","BAL1","BAL2",
    "PLSS","PLSE","MISL","BFS1","BFE1","BFE2","TFOG","IFOG","PLAY","POSS",
    "SPOS","VILE","FIRE","FATB","FBXP","SKEL","MANF","FATT","CPOS","SARG",
    "HEAD","BAL7","BOSS","BOS2","SKUL","SPID","BSPI","APLS","APBX","CYBR",
    "PAIN","SSWV","KEEN","BBRN","BOSF","ARM1","ARM2","BAR1","BEXP","FCAN",
    "BON1","BON2","BKEY","RKEY","YKEY","BSKU","RSKU","YSKU","STIM","MEDI",
    "SOUL","PINV","PSTR","PINS","MEGA","SUIT","PMAP","PVIS","CLIP","AMMO",
    "ROCK","BROK","CELL","CELP","SHEL","SBOX","BPAK","BFUG","MGUN","CSAW",
    "LAUN","PLAS","SHOT","SGN2","COLU","SMT2","GOR1","POL2","POL5","POL4",
    "POL3","POL1","POL6","GOR2","GOR3","GOR4","GOR5","SMIT","COL1","COL2",
    "COL3","COL4","CAND","CBRA","COL6","TRE1","TRE2","ELEC","CEYE","FSKU",
    "COL5","TBLU","TGRN","TRED","SMBT","SMGT","SMRT","HDB1","HDB2","HDB3",
    "HDB4","HDB5","HDB6","POB1","POB2","BRS1","TLMP","TLP2" };
*/
int AlphaValue[] = {
    255,    255,   255,   255,   255,   255,   255,   255,   255,   255,
    255,    255,   255,   255,   255,   255,   255,   128,   128,   128,
    128,    128,   128,   128,   128,   128,   128,   128,   255,   255,
    255,    255,   128,   255,   255,   255,   128,   255,   255,   319,
    255,    255,   255,   255,   128,   255,   255,   128,   128,   255,
    255,    255,   255,   255,   255,   255,   255,   255,   255,   255,
    160,    255,   255,   255,   255,   255,   255,   255,   255,   255,
    128,    128,   255,   128,   255,   255,   255,   255,   255,   255,
    255,    255,   255,   255,   255,   255,   255,   255,   255,   255,
    255,    255,   255,   255,   255,   255,   255,   255,   255,   255,
    255,    255,   255,   255,   255,   255,   255,   255,   255,   255,
    255,    255,   255,   255,   255,   255,   255,   255,   255,   255,
    255,    255,   255,   255,   255,   255,   255,   255,   255,   255,
    255,    255,   255,   255,   255,   255,   255,   255 };

void lfprintf(char *message, ... );

typedef struct
   {
    char  name[10];
    unsigned char  red;
    unsigned char  green;
    unsigned char  blue;
    unsigned char  alpha;
    int   intensity;
   }sprite_info_t;

FILE *sd;
char tline[1024];

sprite_info_t sprite_data[1024];
int           info_count = 0;
char *D_PrepTextLine( char *text );

int D_GetByteValue(char *text)
   {
    int i, tval;

    if (text == NULL)
       {
        return 0;
       }
    if (strlen(text) > 3)
       {
        return 0;
       }
    if (strlen(text) == 0)
       {
        return 0;
       }
    i = 0;
    while ((text[i] == ' ') || (text[i] == '\t'))
       {
        i++;
       }
    tval = 0;
    while ((text[i] >= '0') && (text[i] <= '9') && (tval < 256))
       {
        tval *= 10;
        tval += text[i] - '0';
        i++;
       }
    if (tval > 255)
       {
        return 0;
       }
    return tval;
   }

int D_GetIntValue(char *text)
   {
    int i, tval;

    if (text == NULL)
       {
        return 0;
       }
    if (strlen(text) > 5)
       {
        return 0;
       }
    if (strlen(text) == 0)
       {
        return 0;
       }
    i = 0;
    while ((text[i] == ' ') || (text[i] == '\t'))
       {
        i++;
       }
    tval = 0;
    while ((text[i] >= '0') && (text[i] <= '9') && (tval < 65536))
       {
        tval *= 10;
        tval = text[i] - '0';
        i++;
       }
    if (tval > 65535)
       {
        return 0;
       }
    return tval;
   }

void LoadSpriteData()
   {
    char *tc, *ls;
    if ((sd = fopen("sprtdata", "r")) != NULL)
       {
        fgets(tline, 256, sd);
        while (fgets(tline, 256, sd) != NULL)
           { 
            tc = D_PrepTextLine(tline);
            if (strlen(tc) > 0)
               {
                ls = strtok(tc, " ");
                if (ls != NULL)
                   {
                    if (strlen(ls) > 8)
                       {
                        continue;
                       }
                    strcpy(sprite_data[info_count].name, ls);
                    ls = strtok(NULL, " ");
                    if (ls == NULL)
                       {
                        continue;
                       }
                    sprite_data[info_count].red = D_GetByteValue(ls);
                    ls = strtok(NULL, " ");
                    if (ls == NULL)
                       {
                        continue;
                       }
                    sprite_data[info_count].green = D_GetByteValue(ls);
                    ls = strtok(NULL, " ");
                    if (ls == NULL)
                       {
                        continue;
                       }
                    sprite_data[info_count].blue = D_GetByteValue(ls);
                    ls = strtok(NULL, " ");
                    if (ls == NULL)
                       {
                        continue;
                       }
                    sprite_data[info_count].alpha = D_GetByteValue(ls);
                    ls = strtok(NULL, " ");
                    if (ls == NULL)
                       {
                        continue;
                       }
                    sprite_data[info_count].intensity = D_GetByteValue(ls);
                    lfprintf("Sprite %-8s, R:%d G:%d B:%d A:%d I:%d\n",
                       sprite_data[info_count].name,
                       sprite_data[info_count].red,
                       sprite_data[info_count].green,
                       sprite_data[info_count].blue,
                       sprite_data[info_count].alpha,
                       sprite_data[info_count].intensity);
                    info_count++;
                   }
               }
           }
        fclose(sd);
       }
   }

int D_FindSpriteData(char *tname)
   {
    int i;

    for (i = 0; i < info_count; i++)
       {
        if (strncmp(sprite_data[i].name, tname, 8) == 0)
           {
            return i;
           }
       }
    return -1;
   }

//
// R_InitSpriteDefs
// Pass a null terminated list of sprite names
//  (4 chars exactly) to be used.
// Builds the sprite rotation matrixes to account
//  for horizontally flipped sprites.
// Will report an error if the lumps are inconsistant. 
// Only called at startup.
//
// Sprite lump names are 4 characters for the actor,
//  a letter for the frame, and a number for the rotation.
// A sprite that is flippable will have an additional
//  letter/number appended.
// The rotation character can be 0 to signify no rotations.
//
void R_InitSpriteDefs (char** namelist) 
   { 
    char**	check;
    int		i;
    int		l, info;
    int		intname;
    int		frame;
    int		rotation;
    int		start;
    int		end;
    int		patched;
    char    tname[10];
		
    // count the number of sprite names
    check = namelist;
    while (*check != NULL)
        check++;

    numsprites = check-namelist;
	
    if (!numsprites)
       return;
		
    printf("Loading sprite data...\n");
    LoadSpriteData();
    printf("Loaded info for %d sprites...\n", info_count);

    sprites = Z_Malloc(numsprites *sizeof(*sprites), PU_STATIC, NULL);
	
    start = firstspritelump-1;
    end = lastspritelump+1;

    NumSpriteLumps = end - start;
    SpriteLumps = (int *)malloc(sizeof(int)*NumSpriteLumps);
    SprData = (GLTexData *)malloc(sizeof(GLTexData)*NumSpriteLumps);
    for (i = 0; i < NumSpriteLumps; i++)
       {
        SpriteLumps[i] = 0;
        SprData[i].Permanent = false;
        SprData[i].Translucent = 255;
        SprData[i].Intensity = 0;
        SprData[i].Translucent = AlphaValue[i];
       }
	
    // scan all the lump names for each of the names,
    //  noting the highest frame letter.
    // Just compare 4 characters as ints
    for (i = 0; i < numsprites; i++)
       {
        spritename = namelist[i];
        memset (sprtemp,-1, sizeof(sprtemp));
		
        maxframe = -1;
        intname = *(int *)namelist[i];

        // scan the lumps,
        //  filling in the frames for whatever is found
        for (l = start+1; l < end; l++)
           {
            if (*(int *)lumpinfo[l].name == intname)
               {
                strncpy(tname, lumpinfo[l].name, 8);
                tname[8] = '\0';
                if ((info = D_FindSpriteData(tname)) != -1)
                   {
                    SprData[l-(start+1)].Translucent = sprite_data[info].alpha;
                    SprData[l-(start+1)].Intensity = sprite_data[info].intensity;
                   }

                frame = lumpinfo[l].name[4] - 'A';
                rotation = lumpinfo[l].name[5] - '0';
                if (modifiedgame)
                    patched = W_GetNumForName(lumpinfo[l].name);
                else
                    patched = l;

                SprData[l-(start+1)].Translucent = AlphaValue[i];

                R_InstallSpriteLump (patched, frame, rotation, false);

                if (lumpinfo[l].name[6])
                   {
                    frame = lumpinfo[l].name[6] - 'A';
                    rotation = lumpinfo[l].name[7] - '0';
                    R_InstallSpriteLump (l, frame, rotation, true);
                   }
               }
           }
	
        // check the frames that were found for completeness
        if (maxframe == -1)
           {
            sprites[i].numframes = 0;
            continue;
           }
		
        maxframe++;
	
        for (frame = 0 ; frame < maxframe; frame++)
           {
            switch ((int)sprtemp[frame].rotate)
               {
                case -1:
                     // no rotations were found for that frame at all
                     I_Error ("R_InitSprites: No patches found for %s frame %c", namelist[i], frame+'A');
                     break;
	
                case 0:
                     // only the first rotation is needed
                     break;
		
                case 1:
                     // must have all 8 frames
                     for (rotation = 0; rotation < 8; rotation++)
                     if (sprtemp[frame].lump[rotation] == -1)
                         I_Error ("R_InitSprites: Sprite %s frame %c is missing rotations", namelist[i], frame+'A');
                     break;
               }
	       }
	
        // allocate space for the frames present and copy sprtemp to it
        sprites[i].numframes = maxframe;
        sprites[i].spriteframes = Z_Malloc (maxframe * sizeof(spriteframe_t), PU_STATIC, NULL);
        memcpy (sprites[i].spriteframes, sprtemp, maxframe*sizeof(spriteframe_t));
       }
   }


//
// GAME FUNCTIONS
//
vissprite_t	vissprites[MAXVISSPRITES];
vissprite_t*	vissprite_p;
int		newvissprite;

//
// R_InitSprites
// Called at program start.
//
void R_InitSprites (char** namelist)
{
    int		i;
	
    xtoviewangle = (angle_t *)calloc(SCREENWIDTH+1,sizeof(angle_t));
    negonearray = (short *)calloc(SCREENWIDTH,sizeof(short));
    screenheightarray = (short *)calloc(SCREENWIDTH,sizeof(short));
    for (i=0 ; i<SCREENWIDTH ; i++)
    {
	negonearray[i] = -1;
    }

    R_InitSpriteDefs (namelist);
   }



//
// R_ClearSprites
// Called at frame start.
//
void R_ClearSprites (void)
{
    vissprite_p = vissprites;
}


//
// R_NewVisSprite
//
vissprite_t	overflowsprite;

vissprite_t* R_NewVisSprite (void)
{
    if (vissprite_p == &vissprites[MAXVISSPRITES])
	return &overflowsprite;
    
    vissprite_p++;
    return vissprite_p-1;
}



//
// R_DrawMaskedColumn
// Used for sprites and masked mid textures.
// Masked means: partly transparent, i.e. stored
//  in posts/runs of opaque pixels.
//
short*		mfloorclip;
short*		mceilingclip;

fixed_t		spryscale;
fixed_t		sprtopscreen;

void R_DrawMaskedColumn (column_t* column)
{
 //   int		topscreen;
 //   int 	bottomscreen;
 //   fixed_t	basetexturemid;
	//
 //   basetexturemid = dc_texturemid;
	//
 //   for ( ; column->topdelta != 0xff ; ) 
 //   {
	//// calculate unclipped screen coordinates
	////  for post
	//topscreen = sprtopscreen + spryscale*column->topdelta;
	//bottomscreen = topscreen + spryscale*column->length;

	//dc_yl = (topscreen+FRACUNIT-1)>>FRACBITS;
	//dc_yh = (bottomscreen-1)>>FRACBITS;
	//	
	//if (dc_yh >= mfloorclip[dc_x])
	//    dc_yh = mfloorclip[dc_x]-1;
	//if (dc_yl <= mceilingclip[dc_x])
	//    dc_yl = mceilingclip[dc_x]+1;

	//if (dc_yl <= dc_yh)
	//{
	//    dc_source = (byte *)column + 3;
	//    dc_texturemid = basetexturemid - (column->topdelta<<FRACBITS);
	//    // dc_source = (byte *)column + 3 - column->topdelta;

	//    // Drawn by either R_DrawColumn
	//    //  or (SHADOW) R_DrawFuzzColumn.
	//    colfunc ();	
	//}
	//column = (column_t *)(  (byte *)column + column->length + 4);
 //   }
	//
 //   dc_texturemid = basetexturemid;
}



//
// R_DrawVisSprite
//  mfloorclip and mceilingclip should also be set.
//
void
R_DrawVisSprite
( vissprite_t*		vis,
  int			x1,
  int			x2 )
{
//    column_t*		column;
//    int			texturecolumn;
//    fixed_t		frac;
//    patch_t*		patch;
//	
//	
//    patch = W_CacheLumpNum (vis->patch+firstspritelump, PU_CACHE);
//
//    dc_colormap = vis->colormap;
//    
//    if (!dc_colormap)
//    {
//	// NULL colormap = shadow draw
//	colfunc = fuzzcolfunc;
//    }
//    else if (vis->mobjflags & MF_TRANSLATION)
//    {
//	colfunc = R_DrawTranslatedColumn;
//	dc_translation = translationtables - 256 +
//	    ( (vis->mobjflags & MF_TRANSLATION) >> (MF_TRANSSHIFT-8) );
//    }
//	
//    dc_iscale = abs(vis->xiscale)>>detailshift;
//    dc_texturemid = vis->texturemid;
//    frac = vis->startfrac;
//    spryscale = vis->scale;
//    sprtopscreen = centeryfrac - FixedMul(dc_texturemid,spryscale);
//	
//    for (dc_x=vis->x1 ; dc_x<=vis->x2 ; dc_x++, frac += vis->xiscale)
//    {
//	texturecolumn = frac>>FRACBITS;
//#ifdef RANGECHECK
//	if (texturecolumn < 0 || texturecolumn >= SHORT(patch->width))
//	    I_Error ("R_DrawSpriteRange: bad texturecolumn");
//#endif
//	column = (column_t *) ((byte *)patch +
//			       LONG(patch->columnofs[texturecolumn]));
//	R_DrawMaskedColumn (column);
//    }
//
//    colfunc = basecolfunc;
}


int			lightnum;

//
// R_ProjectSprite
// Generates a vissprite for a thing
//  if it might be visible.
//
void R_ProjectSprite (mobj_t* thing)
{
    fixed_t		tr_x;
    fixed_t		tr_y;
    
    fixed_t		gxt;
    fixed_t		gyt;
    
    fixed_t		tx;
    fixed_t		tz;

    fixed_t		xscale;
    
    int			x1;
    int			x2;

    spritedef_t*	sprdef;
    spriteframe_t*	sprframe;
    int			lump;
    
    unsigned		rot;
    dboolean		flip;
    
    int			index;

    vissprite_t*	vis;
    
    angle_t		ang;
    fixed_t		iscale;
    
    // transform the origin point
    tr_x = thing->x - viewx;
    tr_y = thing->y - viewy;
	
    gxt = FixedMul(tr_x,viewcos); 
    gyt = -FixedMul(tr_y,viewsin);
    
    tz = gxt-gyt; 

    // thing is behind view plane?
    if (tz < MINZ)
	return;
    
    xscale = FixedDiv(projection, tz);
	
    gxt = -FixedMul(tr_x,viewsin); 
    gyt = FixedMul(tr_y,viewcos); 
    tx = -(gyt+gxt); 

    // too far off the side?
    if (abs(tx)>(tz<<2))
	return;
    
    // decide which patch to use for sprite relative to player
#ifdef RANGECHECK
    if ((unsigned)thing->sprite >= (unsigned)numsprites)
	I_Error ("R_ProjectSprite: invalid sprite number %i ",
		 thing->sprite);
#endif
    sprdef = &sprites[thing->sprite];
#ifdef RANGECHECK
    if ( (thing->frame&FF_FRAMEMASK) >= sprdef->numframes )
	I_Error ("R_ProjectSprite: invalid sprite frame %i : %i ",
		 thing->sprite, thing->frame);
#endif
    sprframe = &sprdef->spriteframes[ thing->frame & FF_FRAMEMASK];

    if (sprframe->rotate)
    {
	// choose a different rotation based on player view
	ang = R_PointToAngle (thing->x, thing->y);
	rot = (ang-thing->angle+(unsigned)(ANG45/2)*9)>>29;
	lump = sprframe->lump[rot];
	flip = (dboolean)sprframe->flip[rot];
    }
    else
    {
	// use single rotation for all views
	lump = sprframe->lump[0];
	flip = (dboolean)sprframe->flip[0];
    }
    
    // calculate edges of the shape
    tx -= spriteoffset[lump];	
    x1 = (centerxfrac + FixedMul (tx,xscale) ) >>FRACBITS;

    // off the right side?
    if (x1 > viewwidth)
	return;
    
    tx +=  spritewidth[lump];
    x2 = ((centerxfrac + FixedMul (tx,xscale) ) >>FRACBITS) - 1;

    // off the left side
    if (x2 < 0)
	return;
    
    // store information in a vissprite
    vis = R_NewVisSprite ();
    vis->mobjflags = thing->flags;
    vis->scale = xscale<<detailshift;
    vis->gx = thing->x;
    vis->gy = thing->y;
    vis->gz = thing->z;
    vis->gzt = thing->z + spritetopoffset[lump];
    vis->texturemid = vis->gzt - viewz;
    vis->x1 = x1 < 0 ? 0 : x1;
    vis->x2 = x2 >= viewwidth ? viewwidth-1 : x2;	
    iscale = FixedDiv (FRACUNIT, xscale);

    if (flip)
    {
	vis->startfrac = spritewidth[lump]-1;
	vis->xiscale = -iscale;
    }
    else
    {
	vis->startfrac = 0;
	vis->xiscale = iscale;
    }

    if (vis->x1 > x1)
	vis->startfrac += vis->xiscale*(vis->x1-x1);
    vis->patch = lump;
    
    // get light level
    if (thing->flags & MF_SHADOW)
    {
	// shadow draw
	vis->colormap = NULL;
    //vis->light = -1;
    vis->light = 0 - lightnum;
    }
    else if (fixedcolormap)
    {
	// fixed map
	vis->colormap = fixedcolormap;
    vis->light = 16; // until I get this fixed...
    }
    else if (thing->frame & FF_FULLBRIGHT)
    {
	// full bright
	vis->colormap = colormaps;
    vis->light = 16;
    }
    
    else
    {
	// diminished light
	index = xscale>>(LIGHTSCALESHIFT-detailshift);

	if (index >= MAXLIGHTSCALE) 
	    index = MAXLIGHTSCALE-1;

	vis->colormap = spritelights[index];
    vis->light = lightnum;
    }	
}




//
// R_AddSprites
// During BSP traversal, this adds sprites by sector.
//
void R_AddSprites (sector_t* sec)
{
    mobj_t*		thing;

    // BSP is traversed by subsector.
    // A sector might have been split into several
    //  subsectors during BSP building.
    // Thus we check whether its already added.
    if (sec->validcount == validcount)
	return;		

    // Well, now it will be done.
    sec->validcount = validcount;
	
    lightnum = (sec->lightlevel >> LIGHTSEGSHIFT)+extralight;

    if (lightnum < 0)		
	spritelights = scalelight[0];
    else if (lightnum >= LIGHTLEVELS)
	spritelights = scalelight[LIGHTLEVELS-1];
    else
	spritelights = scalelight[lightnum];

    // Handle all things in sector.
    for (thing = sec->thinglist ; thing ; thing = thing->snext)
	R_ProjectSprite (thing);
}


//
// R_DrawPSprite
//
void R_DrawPSprite (pspdef_t* psp)
   {
    fixed_t		tx;
    int			x1;
    int			x2;
    spritedef_t*	sprdef;
    spriteframe_t*	sprframe;
    int			lump;
    dboolean		flip;
    vissprite_t*	vis;
    vissprite_t		avis;
    static int  csprite = -1;
    
    // decide which patch to use
#ifdef RANGECHECK
    if ((unsigned)psp->state->sprite >= (unsigned)numsprites)
        I_Error ("R_ProjectSprite: invalid sprite number %i ",psp->state->sprite);
#endif
    sprdef = &sprites[psp->state->sprite];
#ifdef RANGECHECK
    if ((psp->state->frame & FF_FRAMEMASK) >= sprdef->numframes)
        I_Error ("R_ProjectSprite: invalid sprite frame %i : %i ",psp->state->sprite, psp->state->frame);
#endif
    sprframe = &sprdef->spriteframes[ psp->state->frame & FF_FRAMEMASK ];

    lump = sprframe->lump[0];
    flip = (dboolean)sprframe->flip[0];
    
    // calculate edges of the shape
    tx = psp->sx-160*FRACUNIT;
	
    tx -= spriteoffset[lump];	
    x1 = (centerxfrac + FixedMul (tx,pspritescale) ) >>FRACBITS;

    // off the right side
    if (x1 > viewwidth)
        return;		

    tx +=  spritewidth[lump];
    x2 = ((centerxfrac + FixedMul (tx, pspritescale) ) >>FRACBITS) - 1;

    // off the left side
    if (x2 < 0)
        return;
    
    // store information in a vissprite
    vis = &avis;
    vis->mobjflags = 0;
    vis->texturemid = (BASEYCENTER<<FRACBITS)+FRACUNIT/2-(psp->sy-spritetopoffset[lump]);
    vis->x1 = x1 < 0 ? 0 : x1;
    vis->x2 = x2 >= viewwidth ? viewwidth-1 : x2;	
    vis->scale = pspritescale<<detailshift;

    if (flip)
       {
        vis->xiscale = -pspriteiscale;
        vis->startfrac = spritewidth[lump]-1;
       }
    else
       {
        vis->xiscale = pspriteiscale;
        vis->startfrac = 0;
       }
    
    if (vis->x1 > x1)
        vis->startfrac += vis->xiscale*(vis->x1-x1);

    vis->patch = lump;

    if (viewplayer->powers[pw_invisibility] > 4*32 || viewplayer->powers[pw_invisibility] & 8)
       {
        // shadow draw
        vis->colormap = NULL;
       }
    else
    if (fixedcolormap)
       {
        // fixed color
        vis->colormap = fixedcolormap;
       }
    else
    if (psp->state->frame & FF_FULLBRIGHT)
       {
        // full bright
        vis->colormap = colormaps;
       }
    else
       {
        // local light
        vis->colormap = spritelights[MAXLIGHTSCALE-1];
       }

//    if (csprite != psp->state->sprite)
//       {
//        csprite = psp->state->sprite;
//       }
    R_DrawVisSprite (vis, vis->x1, vis->x2);
   }

extern int hudmode;

extern float SetBack;
//
// R_DrawPSprite
//
void GL_DrawPSprite (pspdef_t* psp, int sptype, int iLight)
   {
    fixed_t		tx;
    int			x1;
    int			x2;
    float       sx, sy;
    static float tsy = 0.0f, tsx = 0.0f;
    spritedef_t*	sprdef;
    spriteframe_t*	sprframe;
    int			lump;
    dboolean		flip;
    vissprite_t*	vis;
    vissprite_t		avis;
    static int  csprite = -1;
    static dboolean invisible;
    float       fLight;
    static int  tLight = 0;
    
    // decide which patch to use
    sprdef = &sprites[psp->state->sprite];
    sprframe = &sprdef->spriteframes[ psp->state->frame & FF_FRAMEMASK ];

    lump = sprframe->lump[0];
    flip = (dboolean)sprframe->flip[0];
    
    // calculate edges of the shape
    tx = psp->sx-160*FRACUNIT;
	
    tx -= spriteoffset[lump];	
    x1 = (centerxfrac + FixedMul (tx,pspritescale) ) >>FRACBITS;

    tx +=  spritewidth[lump];
    x2 = ((centerxfrac + FixedMul (tx, pspritescale) ) >>FRACBITS) - 1;

    
    // store information in a vissprite
    vis = &avis;
    vis->mobjflags = 0;
    vis->texturemid = (BASEYCENTER<<FRACBITS)+FRACUNIT/2-(psp->sy-spritetopoffset[lump]);
    vis->x1 = x1 < 0 ? 0 : x1;
    vis->x2 = x2 >= viewwidth ? viewwidth-1 : x2;	
    vis->scale = pspritescale<<detailshift;

    if (flip)
       {
        vis->xiscale = -pspriteiscale;
        vis->startfrac = spritewidth[lump]-1;
       }
    else
       {
        vis->xiscale = pspriteiscale;
        vis->startfrac = 0;
       }
    
    if (vis->x1 > x1)
        vis->startfrac += vis->xiscale*(vis->x1-x1);

    vis->patch = lump;

    if (viewplayer->powers[pw_invisibility] > 4*32 || viewplayer->powers[pw_invisibility] & 8)
       {
        // shadow draw
        vis->colormap = NULL;
        invisible = true;
       }
    else
       {
        invisible = false;
    if (fixedcolormap)
       {
        // fixed color
        vis->colormap = fixedcolormap;
       }
    else
    if (psp->state->frame & FF_FULLBRIGHT)
       {
        // full bright
        vis->colormap = colormaps;
       }
    else
       {
        // local light
        vis->colormap = spritelights[MAXLIGHTSCALE-1];
       }
       }

    sx = (psp->sx >> FRACBITS) - SprData[lump].LeftOff;
    sy = (psp->sy >> FRACBITS) - SprData[lump].TopOff - 22.0f;

/*
    if ((csprite != psp->state->sprite) || (tsy != sy))
       {
        csprite = psp->state->sprite;
        tsy = sy;
        tsx = psp->sx;
       }
*/
    if (SprData[lump].TexName != 0)
       {
        if (invisible == true)
           {// FIXME - not coming out transparent...
            //glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            //glBlendFunc( GL_ONE, GL_ONE );
           }
        else
        if ((SprData[lump].Translucent != 255) && (sptype == 1))
           {
//            glEnable(GL_BLEND);
            //glBlendFunc( GL_ONE, GL_ONE );
//            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
           }
        //glBindTexture(GL_TEXTURE_2D, SprData[lump].TexName);
        if ((psp->state->frame & FF_FULLBRIGHT) || (players[consoleplayer].powers[pw_infrared] > 0))
           {
            // full bright
            //glColor3f( 1.0f, 1.0f, 1.0f );
           }
        else
           {
            // local light
            fLight = ((float)(iLight * 16) / 255.0f);
            //glColor3f( fLight, fLight, fLight );
           }
        if (tLight != iLight)
           {
            tLight = iLight;
           }
        if (hudmode != 1)
           {
            sy += 22.0f;
           }

        vec3 pos = { sx, sy, 0 };
        vec2 size = { SprData[lump].Width, SprData[lump].Height };

        R2D_DrawSprite(pos, size, &SprData[lump].TexName);

////        if (((SprData[lump].Translucent != 255) && (sptype == 1)) || (invisible == true))
////           {
//            glDisable(GL_BLEND);
////           }
//        glDisable(GL_TEXTURE_2D);
       }
   }

//
// R_DrawPlayerSprites
//
void R_DrawPlayerSprites (void)
   {
    int		i;
    pspdef_t*	psp;
    
    // get light level
    lightnum = (viewplayer->mo->subsector->sector->lightlevel >> LIGHTSEGSHIFT)+extralight;

    if (lightnum < 0)		
        spritelights = scalelight[0];
    else
    if (lightnum >= LIGHTLEVELS)
        spritelights = scalelight[LIGHTLEVELS-1];
    else
        spritelights = scalelight[lightnum];
    
    // clip to screen bounds
    mfloorclip = screenheightarray;
    mceilingclip = negonearray;
    
    // add all active psprites
    for (i = 0, psp = viewplayer->psprites; i < NUMPSPRITES; i++,psp++)
       {
        if (psp->state)
           {
            R_DrawPSprite(psp);
           }
       }
   }

//
// R_DrawPlayerSprites
//
void GL_DrawPlayerSprites (void)
   {
    int		i;
    pspdef_t*	psp;
    
    // get light level
    lightnum = (viewplayer->mo->subsector->sector->lightlevel >> LIGHTSEGSHIFT)+extralight;

    if (lightnum < 0)		
        spritelights = scalelight[0];
    else
    if (lightnum >= LIGHTLEVELS)
        spritelights = scalelight[LIGHTLEVELS-1];
    else
        spritelights = scalelight[lightnum];

/*    
    // clip to screen bounds
    mfloorclip = screenheightarray;
    mceilingclip = negonearray;
*/    
    // add all active psprites
    for (i = 0, psp = viewplayer->psprites; i < NUMPSPRITES; i++,psp++)
       {
        if (psp->state)
           {
            GL_DrawPSprite(psp, i, lightnum);
           }
       }
   }




//
// R_SortVisSprites
//
vissprite_t	vsprsortedhead;


void R_SortVisSprites (void)
{
    int			i;
    int			count;
    vissprite_t*	ds;
    vissprite_t*	best;
    vissprite_t		unsorted;
    fixed_t		bestscale;

    count = vissprite_p - vissprites;
	
    unsorted.next = unsorted.prev = &unsorted;

    if (!count)
	return;
		
    for (ds=vissprites ; ds<vissprite_p ; ds++)
    {
	ds->next = ds+1;
	ds->prev = ds-1;
    }
    
    vissprites[0].prev = &unsorted;
    unsorted.next = &vissprites[0];
    (vissprite_p-1)->next = &unsorted;
    unsorted.prev = vissprite_p-1;
    
    // pull the vissprites out by scale
    //best = 0;		// shut up the compiler warning
    vsprsortedhead.next = vsprsortedhead.prev = &vsprsortedhead;
    for (i=0 ; i<count ; i++)
    {
	bestscale = DMAXINT;
	for (ds=unsorted.next ; ds!= &unsorted ; ds=ds->next)
	{
	    if (ds->scale < bestscale)
	    {
		bestscale = ds->scale;
		best = ds;
	    }
	}
	best->next->prev = best->prev;
	best->prev->next = best->next;
	best->next = &vsprsortedhead;
	best->prev = vsprsortedhead.prev;
	vsprsortedhead.prev->next = best;
	vsprsortedhead.prev = best;
    }
}

//
// R_DrawSprite
//
void R_DrawSprite (vissprite_t* spr)
{
    drawseg_t*		ds;
    short		clipbot[1280];
    short		cliptop[1280];
    //short		clipbot[SCREENWIDTH];
    //short		cliptop[SCREENWIDTH];
    int			x;
    int			r1;
    int			r2;
    fixed_t		scale;
    fixed_t		lowscale;
    int			silhouette;
		
    for (x = spr->x1 ; x<=spr->x2 ; x++)
	clipbot[x] = cliptop[x] = -2;
    
    // Scan drawsegs from end to start for obscuring segs.
    // The first drawseg that has a greater scale
    //  is the clip seg.
    for (ds=ds_p-1 ; ds >= drawsegs ; ds--)
    {
	// determine if the drawseg obscures the sprite
	if (ds->x1 > spr->x2
	    || ds->x2 < spr->x1
	    || (!ds->silhouette
		&& !ds->maskedtexturecol) )
	{
	    // does not cover sprite
	    continue;
	}
			
	r1 = ds->x1 < spr->x1 ? spr->x1 : ds->x1;
	r2 = ds->x2 > spr->x2 ? spr->x2 : ds->x2;

	if (ds->scale1 > ds->scale2)
	{
	    lowscale = ds->scale2;
	    scale = ds->scale1;
	}
	else
	{
	    lowscale = ds->scale1;
	    scale = ds->scale2;
	}
		
	if (scale < spr->scale
	    || ( lowscale < spr->scale
		 && !R_PointOnSegSide (spr->gx, spr->gy, ds->curline) ) )
	{
	    // masked mid texture?
	    if (ds->maskedtexturecol)	
		R_RenderMaskedSegRange (ds, r1, r2);
	    // seg is behind sprite
	    continue;			
	}

	
	// clip this piece of the sprite
	silhouette = ds->silhouette;
	
	if (spr->gz >= ds->bsilheight)
	    silhouette &= ~SIL_BOTTOM;

	if (spr->gzt <= ds->tsilheight)
	    silhouette &= ~SIL_TOP;
			
	if (silhouette == 1)
	{
	    // bottom sil
	    for (x=r1 ; x<=r2 ; x++)
		if (clipbot[x] == -2)
		    clipbot[x] = ds->sprbottomclip[x];
	}
	else if (silhouette == 2)
	{
	    // top sil
	    for (x=r1 ; x<=r2 ; x++)
		if (cliptop[x] == -2)
		    cliptop[x] = ds->sprtopclip[x];
	}
	else if (silhouette == 3)
	{
	    // both
	    for (x=r1 ; x<=r2 ; x++)
	    {
		if (clipbot[x] == -2)
		    clipbot[x] = ds->sprbottomclip[x];
		if (cliptop[x] == -2)
		    cliptop[x] = ds->sprtopclip[x];
	    }
	}
		
    }
    
    // all clipping has been performed, so draw the sprite

    // check for unclipped columns
    for (x = spr->x1 ; x<=spr->x2 ; x++)
    {
	if (clipbot[x] == -2)		
	    clipbot[x] = viewheight;

	if (cliptop[x] == -2)
	    cliptop[x] = -1;
    }
		
    mfloorclip = clipbot;
    mceilingclip = cliptop;
    R_DrawVisSprite (spr, spr->x1, spr->x2);
}




//
// R_DrawMasked
//
void R_DrawMasked (void)
{
    vissprite_t*	spr;
    drawseg_t*		ds;
	
    R_SortVisSprites ();

    if (vissprite_p > vissprites)
    {
	// draw all vissprites back to front
	for (spr = vsprsortedhead.next ;
	     spr != &vsprsortedhead ;
	     spr=spr->next)
	{
	    
	    R_DrawSprite (spr);
	}
    }
    
    // render any remaining masked mid textures
    for (ds=ds_p-1 ; ds >= drawsegs ; ds--)
	if (ds->maskedtexturecol)
	    R_RenderMaskedSegRange (ds, ds->x1, ds->x2);
    
    // draw the psprites on top of everything
    //  but does not draw on side views
    if (!viewangleoffset)
       {
	R_DrawPlayerSprites ();
       }
}

/*
    fixed_t		gx;
    fixed_t		gy;		

    // global bottom / top for silhouette clipping
    fixed_t		gz;
    fixed_t		gzt;
*/

extern camera_t camera;
extern float    foglight;
extern double   pfactor;

void GL_DrawThings(void)
   {
    vissprite_t*	spr;
    int             lump;
    float           sx, sy, sz, Orient, x1, x2, tLight;
    float           fLight, fTop, fBottom, fOffset;
	
    Orient =  360.0f+(camera.oy*-1.0f);
    Orient +=  180.0f;
    if (Orient > 360.0f)
       Orient -= 360.0f;

    if (vissprite_p > vissprites)
       {
/*
        if (vis->mobjflags & MF_TRANSLATION)
           {
	        colfunc = R_DrawTranslatedColumn;
	        dc_translation = translationtables - 256 + ((vis->mobjflags & MF_TRANSLATION) >> (MF_TRANSSHIFT-8));
    }
*/
	// draw all vissprites back to front
	    for (spr = vsprsortedhead.next; spr != &vsprsortedhead; spr = spr->next)
           {
            lump = spr->patch;
            if (SprData[lump].TexName != 0)
               {
                sx = (float)spr->gx * (float)pfactor;
                sy = (float)spr->gz * (float)pfactor;
//    vis->gzt = thing->z + spritetopoffset[lump];
                sz = 0.0f - ((float)spr->gy * (float)pfactor);

                //glPushMatrix();
                ////glLoadIdentity();

                //glTranslatef( sx, sy, sz );
                //glRotatef(Orient, 0.0f, 1.0f, 0.0f );

                if ((spr->xiscale >> FRACBITS) < 0)
                   {
                    x1 = 0.0f;
                    x2 = SprData[lump].XDisp;
                   }
                else
                   {
                    x1 = SprData[lump].XDisp;
                    x2 = 0.0f;
                   }

                /*glEnable(GL_ALPHA_TEST);
                glAlphaFunc(GL_GREATER, 0.0f);
                glBindTexture(GL_TEXTURE_2D, SprData[lump].TexName);*/
                // local light
                fLight = ((float)(spr->light * 16) / 255.0f);
                /*glEnable(GL_BLEND);
                if (gl_premalpha)
                   {
                    glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
                   }
                else
                   {
                    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
                   }
                glColor4f( fLight, fLight, fLight, 1.0f);*/
                if ((fLight <= 0.0f) && (SprData[lump].Translucent > 255))
                   {
                    if (gl_alphatest)
                       {
                        fLight = 1.0f - fLight;
                        /*glColor4f( fLight, fLight, fLight, fLight);
                        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );*/
                       }
                    else
                       {
                        fLight = 0.25f;
                        //glColor4f( fLight, fLight, fLight, (SprData[lump].Translucent - 255)/255.0f);
                        /*glEnable(GL_BLEND);
                        glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
                        glColor4f( fLight, fLight, fLight, 0.2f);*/
                       }
                   }
                else
                   {
                    if (SprData[lump].Translucent < 255)
                       {
                        if (SprData[lump].Intensity == 0)
                           {
                            /*glColor4f( fLight, fLight, fLight, (float)SprData[lump].Translucent/255.0f);
                            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );*/
                           }
                        else
                           {
                            if (SprData[lump].Intensity >= 255)
                               {
                                fLight = 1.0f;
                               }
                            else
                               {
                                tLight = SprData[lump].Intensity / 255.0f;
                                if (tLight > fLight)
                                   {
                                    fLight = tLight;
                                   }
                               }
                            /*glColor4f( fLight, fLight, fLight, 1.0f);
                            glBlendFunc( GL_ONE, GL_ONE );*/
                           }
                       }
                   }
                //glColor3f( fLight, fLight, fLight );
                if ((fLight >= foglight) && (gl_fog == 1))
                   {
                    //glDisable(GL_FOG);
                   }
                if (SprData[lump].Height != SprData[lump].TopOff)
                   {
                    fOffset = ((SprData[lump].Height - (SprData[lump].TopOff)) / 2.0f) - 3.0f;
                    //fOffset = ((SprData[lump].Height - (SprData[lump].TopOff)) / 2.0f);
                    if (fOffset < 0.0f)
                       fOffset = 0.0f;
                   }
                else
                   fOffset = 0.0f;
                fTop = SprData[lump].Height - fOffset;
                fBottom = fTop - SprData[lump].Height;
                //glColor3f( 1.0f, 1.0f, 1.0f );
                //glBegin(GL_QUADS);
/*
                   glTexCoord2f( x1,  1.0f );
                   glVertex3f( sx-(SprData[lump].Width/2.0f),  sy+SprData[lump].Height, sz );
                   glTexCoord2f( x1,  SprData[lump].YDisp );
                   glVertex3f( sx-(SprData[lump].Width/2.0f), sy, sz );
                   glTexCoord2f( x2,  SprData[lump].YDisp );
                   glVertex3f(  sx+(SprData[lump].Width/2.0f), sy, sz );
                   glTexCoord2f( x2,  1.0f );
                   glVertex3f(  sx+(SprData[lump].Width/2.0f),  sy+SprData[lump].Height, sz );
*/
                   /*glTexCoord2f( x1,  1.0f );
                   glVertex3f( 0.0f-(SprData[lump].Width/2.0f),  fTop, 0.0f );
                   glTexCoord2f( x1,  SprData[lump].YDisp );
                   glVertex3f( 0.0f-(SprData[lump].Width/2.0f), fBottom, 0.0f );
                   glTexCoord2f( x2,  SprData[lump].YDisp );
                   glVertex3f(  0.0f+(SprData[lump].Width/2.0f), fBottom, 0.0f );
                   glTexCoord2f( x2,  1.0f );
                   glVertex3f(  0.0f+(SprData[lump].Width/2.0f),  fTop, 0.0f );

                glEnd();*/
                /*if ((fLight >= foglight) && (gl_fog == 1))
                   {
                    glEnable(GL_FOG);
                   }
                glBindTexture(GL_TEXTURE_2D, 0);
                glDisable(GL_ALPHA_TEST);
                glDisable(GL_BLEND);

                glPopMatrix();*/
               }
           }
       }
   }



