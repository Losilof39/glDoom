################################################################
#
# $Id:$
#
# $Log:$
#

ifeq ($(V),1)
	VB=''
else
	VB=@
endif

CC=gcc
CFLAGS+= -I./thirdparty/glad/include
#LIBS+= `pkg-config --libs sdl2` `pkg-config --libs libpng` `pkg-config --libs gl` `pkg-config --libs glu` `pkg-config --libs SDL2_net` -lm
LIBS+=-lOpenGL -lSDL2_mixer -lSDL2 -lm -lGL -lGLU -lcglm
OBJDIR=.
OUTPUT=glDoom-Re
WADBUILD=wadbuild/wadbuild
SRC_GLDOOM = /thirdparty/glad/src/glad.c am_map.c co_console.c co_utils.c d_bitmap.c d_items.c d_main.c d_net.c doomdef.c doomlib.c doomstat.c dstrings.c f_finale.c f_wipe.c g_game.c gl_texture.c gl_video.c \
gl_wipe.c glsbar.c hu_lib.c hu_stuff.c i_net.c i_sdlmusic.c i_sdlsound.c i_sound.c i_system.c info.c m_argv.c m_bbox.c m_cheat.c m_fixed.c m_menu.c m_misc.c m_random.c m_swap.c mathlib.c \
mmus2mid.c p_ceilng.c p_doors.c p_enemy.c p_floor.c p_inter.c p_lights.c p_map.c p_maputl.c p_mobj.c p_plats.c p_pspr.c p_saveg.c p_setup.c p_sight.c p_spec.c p_switch.c \
p_telept.c p_tick.c p_user.c r_bsp.c r_data.c r_draw.c r_main.c r_plane.c r_segs.c r_shader.c r_sky.c r_things.c renderer.c rendererCore.c rendererFixed.c s_sound.c savepic.c sdl_input.c \
sdl_kbrd.c sdl_video.c sounds.c st_lib.c st_stuff.c sys_sdl.c tables.c v_video.c w_wad.c wi_stuff.c z_zone.c 

SRC_WADBUILD = wadbuild/wadbuild.c

GL_OBJS += $(addprefix $(OBJDIR)/, $(SRC_GLDOOM))

TOOL_OBJS += $(addprefix $(OBJDIR)/, $(SRC_WADBUILD))

all:	 $(OUTPUT)

clean:
	rm -f $(OUTPUT)
	rm -f $(OUTPUT).gdb
	rm -f $(OUTPUT).map

tool:	 $(WADBUILD)

tool-clean:
	rm -f $(WADBUILD)
	rm -f $(WADBUILD).gdb
	rm -f $(WADBUILD).map

$(OUTPUT):	$(OBJDIR)
	$(VB)$(CC) $(CFLAGS) $(GL_OBJS) \
	-o $(OUTPUT) $(LIBS)

$(WADBUILD):	$(OBJDIR)
	$(VB)$(CC) $(CFLAGS) $(TOOL_OBJS) \
	-o $(WADBUILD) $(LIBS)

$(OBJDIR)/%.o:	%.c
	$(VB)$(CC) $(CFLAGS) -c $< -o $@


