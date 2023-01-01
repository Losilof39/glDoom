# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=GLDOOM - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to GLDOOM - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "GLDOOM - Win32 Release" && "$(CFG)" != "GLDOOM - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "GLDOOM.mak" CFG="GLDOOM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GLDOOM - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GLDOOM - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "GLDOOM - Win32 Debug"
RSC=rc.exe
MTL=mktyplib.exe
CPP=cl.exe

!IF  "$(CFG)" == "GLDOOM - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\GLDOOM.exe" "$(OUTDIR)\GLDOOM.bsc"

CLEAN : 
	-@erase "$(INTDIR)\am_map.obj"
	-@erase "$(INTDIR)\am_map.sbr"
	-@erase "$(INTDIR)\d_console.obj"
	-@erase "$(INTDIR)\d_console.sbr"
	-@erase "$(INTDIR)\d_items.obj"
	-@erase "$(INTDIR)\d_items.sbr"
	-@erase "$(INTDIR)\d_main.obj"
	-@erase "$(INTDIR)\d_main.sbr"
	-@erase "$(INTDIR)\d_net.obj"
	-@erase "$(INTDIR)\d_net.sbr"
	-@erase "$(INTDIR)\doomdef.obj"
	-@erase "$(INTDIR)\doomdef.sbr"
	-@erase "$(INTDIR)\doomstat.obj"
	-@erase "$(INTDIR)\doomstat.sbr"
	-@erase "$(INTDIR)\dstrings.obj"
	-@erase "$(INTDIR)\dstrings.sbr"
	-@erase "$(INTDIR)\dxerr.obj"
	-@erase "$(INTDIR)\dxerr.sbr"
	-@erase "$(INTDIR)\f_finale.obj"
	-@erase "$(INTDIR)\f_finale.sbr"
	-@erase "$(INTDIR)\f_wipe.obj"
	-@erase "$(INTDIR)\f_wipe.sbr"
	-@erase "$(INTDIR)\g_game.obj"
	-@erase "$(INTDIR)\g_game.sbr"
	-@erase "$(INTDIR)\gconsole.obj"
	-@erase "$(INTDIR)\gconsole.sbr"
	-@erase "$(INTDIR)\GL_utils.obj"
	-@erase "$(INTDIR)\GL_utils.sbr"
	-@erase "$(INTDIR)\gl_video.obj"
	-@erase "$(INTDIR)\gl_video.sbr"
	-@erase "$(INTDIR)\GLdoom.res"
	-@erase "$(INTDIR)\glsbar.obj"
	-@erase "$(INTDIR)\glsbar.sbr"
	-@erase "$(INTDIR)\hu_lib.obj"
	-@erase "$(INTDIR)\hu_lib.sbr"
	-@erase "$(INTDIR)\hu_stuff.obj"
	-@erase "$(INTDIR)\hu_stuff.sbr"
	-@erase "$(INTDIR)\i_cd.obj"
	-@erase "$(INTDIR)\i_cd.sbr"
	-@erase "$(INTDIR)\i_main.obj"
	-@erase "$(INTDIR)\i_main.sbr"
	-@erase "$(INTDIR)\i_midi.obj"
	-@erase "$(INTDIR)\i_midi.sbr"
	-@erase "$(INTDIR)\i_net.obj"
	-@erase "$(INTDIR)\i_net.sbr"
	-@erase "$(INTDIR)\i_system.obj"
	-@erase "$(INTDIR)\i_system.sbr"
	-@erase "$(INTDIR)\info.obj"
	-@erase "$(INTDIR)\info.sbr"
	-@erase "$(INTDIR)\lights.obj"
	-@erase "$(INTDIR)\lights.sbr"
	-@erase "$(INTDIR)\m_argv.obj"
	-@erase "$(INTDIR)\m_argv.sbr"
	-@erase "$(INTDIR)\m_bbox.obj"
	-@erase "$(INTDIR)\m_bbox.sbr"
	-@erase "$(INTDIR)\m_cheat.obj"
	-@erase "$(INTDIR)\m_cheat.sbr"
	-@erase "$(INTDIR)\m_fixed.obj"
	-@erase "$(INTDIR)\m_fixed.sbr"
	-@erase "$(INTDIR)\m_menu.obj"
	-@erase "$(INTDIR)\m_menu.sbr"
	-@erase "$(INTDIR)\m_misc.obj"
	-@erase "$(INTDIR)\m_misc.sbr"
	-@erase "$(INTDIR)\m_music.obj"
	-@erase "$(INTDIR)\m_music.sbr"
	-@erase "$(INTDIR)\m_random.obj"
	-@erase "$(INTDIR)\m_random.sbr"
	-@erase "$(INTDIR)\m_swap.obj"
	-@erase "$(INTDIR)\m_swap.sbr"
	-@erase "$(INTDIR)\newlumps.obj"
	-@erase "$(INTDIR)\newlumps.sbr"
	-@erase "$(INTDIR)\p_ceilng.obj"
	-@erase "$(INTDIR)\p_ceilng.sbr"
	-@erase "$(INTDIR)\p_doors.obj"
	-@erase "$(INTDIR)\p_doors.sbr"
	-@erase "$(INTDIR)\p_enemy.obj"
	-@erase "$(INTDIR)\p_enemy.sbr"
	-@erase "$(INTDIR)\p_floor.obj"
	-@erase "$(INTDIR)\p_floor.sbr"
	-@erase "$(INTDIR)\p_inter.obj"
	-@erase "$(INTDIR)\p_inter.sbr"
	-@erase "$(INTDIR)\p_lights.obj"
	-@erase "$(INTDIR)\p_lights.sbr"
	-@erase "$(INTDIR)\p_map.obj"
	-@erase "$(INTDIR)\p_map.sbr"
	-@erase "$(INTDIR)\p_maputl.obj"
	-@erase "$(INTDIR)\p_maputl.sbr"
	-@erase "$(INTDIR)\p_mobj.obj"
	-@erase "$(INTDIR)\p_mobj.sbr"
	-@erase "$(INTDIR)\p_plats.obj"
	-@erase "$(INTDIR)\p_plats.sbr"
	-@erase "$(INTDIR)\p_pspr.obj"
	-@erase "$(INTDIR)\p_pspr.sbr"
	-@erase "$(INTDIR)\p_saveg.obj"
	-@erase "$(INTDIR)\p_saveg.sbr"
	-@erase "$(INTDIR)\p_setup.obj"
	-@erase "$(INTDIR)\p_setup.sbr"
	-@erase "$(INTDIR)\p_sight.obj"
	-@erase "$(INTDIR)\p_sight.sbr"
	-@erase "$(INTDIR)\p_spec.obj"
	-@erase "$(INTDIR)\p_spec.sbr"
	-@erase "$(INTDIR)\p_switch.obj"
	-@erase "$(INTDIR)\p_switch.sbr"
	-@erase "$(INTDIR)\p_telept.obj"
	-@erase "$(INTDIR)\p_telept.sbr"
	-@erase "$(INTDIR)\p_tick.obj"
	-@erase "$(INTDIR)\p_tick.sbr"
	-@erase "$(INTDIR)\p_user.obj"
	-@erase "$(INTDIR)\p_user.sbr"
	-@erase "$(INTDIR)\r_bsp.obj"
	-@erase "$(INTDIR)\r_bsp.sbr"
	-@erase "$(INTDIR)\r_data.obj"
	-@erase "$(INTDIR)\r_data.sbr"
	-@erase "$(INTDIR)\r_draw.obj"
	-@erase "$(INTDIR)\r_draw.sbr"
	-@erase "$(INTDIR)\r_main.obj"
	-@erase "$(INTDIR)\r_main.sbr"
	-@erase "$(INTDIR)\r_plane.obj"
	-@erase "$(INTDIR)\r_plane.sbr"
	-@erase "$(INTDIR)\r_segs.obj"
	-@erase "$(INTDIR)\r_segs.sbr"
	-@erase "$(INTDIR)\r_sky.obj"
	-@erase "$(INTDIR)\r_sky.sbr"
	-@erase "$(INTDIR)\r_things.obj"
	-@erase "$(INTDIR)\r_things.sbr"
	-@erase "$(INTDIR)\s_sound.obj"
	-@erase "$(INTDIR)\s_sound.sbr"
	-@erase "$(INTDIR)\savepic.obj"
	-@erase "$(INTDIR)\savepic.sbr"
	-@erase "$(INTDIR)\snd_dx.obj"
	-@erase "$(INTDIR)\snd_dx.sbr"
	-@erase "$(INTDIR)\sounds.obj"
	-@erase "$(INTDIR)\sounds.sbr"
	-@erase "$(INTDIR)\st_lib.obj"
	-@erase "$(INTDIR)\st_lib.sbr"
	-@erase "$(INTDIR)\st_stuff.obj"
	-@erase "$(INTDIR)\st_stuff.sbr"
	-@erase "$(INTDIR)\sys_win.obj"
	-@erase "$(INTDIR)\sys_win.sbr"
	-@erase "$(INTDIR)\tables.obj"
	-@erase "$(INTDIR)\tables.sbr"
	-@erase "$(INTDIR)\v_video.obj"
	-@erase "$(INTDIR)\v_video.sbr"
	-@erase "$(INTDIR)\w_wad.obj"
	-@erase "$(INTDIR)\w_wad.sbr"
	-@erase "$(INTDIR)\wi_stuff.obj"
	-@erase "$(INTDIR)\wi_stuff.sbr"
	-@erase "$(INTDIR)\win_inpt.obj"
	-@erase "$(INTDIR)\win_inpt.sbr"
	-@erase "$(INTDIR)\win_joy.obj"
	-@erase "$(INTDIR)\win_joy.sbr"
	-@erase "$(INTDIR)\win_kbrd.obj"
	-@erase "$(INTDIR)\win_kbrd.sbr"
	-@erase "$(INTDIR)\win_mous.obj"
	-@erase "$(INTDIR)\win_mous.sbr"
	-@erase "$(INTDIR)\win_video.obj"
	-@erase "$(INTDIR)\win_video.sbr"
	-@erase "$(INTDIR)\z_zone.obj"
	-@erase "$(INTDIR)\z_zone.sbr"
	-@erase "$(OUTDIR)\GLDOOM.bsc"
	-@erase "$(OUTDIR)\GLDOOM.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /FR /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /ML /W3 /GX /D "NDEBUG" /D "WIN32" /D "_WINDOWS"\
 /FR"$(INTDIR)/" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\Release/
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/GLdoom.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/GLDOOM.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\am_map.sbr" \
	"$(INTDIR)\d_console.sbr" \
	"$(INTDIR)\d_items.sbr" \
	"$(INTDIR)\d_main.sbr" \
	"$(INTDIR)\d_net.sbr" \
	"$(INTDIR)\doomdef.sbr" \
	"$(INTDIR)\doomstat.sbr" \
	"$(INTDIR)\dstrings.sbr" \
	"$(INTDIR)\dxerr.sbr" \
	"$(INTDIR)\f_finale.sbr" \
	"$(INTDIR)\f_wipe.sbr" \
	"$(INTDIR)\g_game.sbr" \
	"$(INTDIR)\gconsole.sbr" \
	"$(INTDIR)\GL_utils.sbr" \
	"$(INTDIR)\gl_video.sbr" \
	"$(INTDIR)\glsbar.sbr" \
	"$(INTDIR)\hu_lib.sbr" \
	"$(INTDIR)\hu_stuff.sbr" \
	"$(INTDIR)\i_cd.sbr" \
	"$(INTDIR)\i_main.sbr" \
	"$(INTDIR)\i_midi.sbr" \
	"$(INTDIR)\i_net.sbr" \
	"$(INTDIR)\i_system.sbr" \
	"$(INTDIR)\info.sbr" \
	"$(INTDIR)\lights.sbr" \
	"$(INTDIR)\m_argv.sbr" \
	"$(INTDIR)\m_bbox.sbr" \
	"$(INTDIR)\m_cheat.sbr" \
	"$(INTDIR)\m_fixed.sbr" \
	"$(INTDIR)\m_menu.sbr" \
	"$(INTDIR)\m_misc.sbr" \
	"$(INTDIR)\m_music.sbr" \
	"$(INTDIR)\m_random.sbr" \
	"$(INTDIR)\m_swap.sbr" \
	"$(INTDIR)\newlumps.sbr" \
	"$(INTDIR)\p_ceilng.sbr" \
	"$(INTDIR)\p_doors.sbr" \
	"$(INTDIR)\p_enemy.sbr" \
	"$(INTDIR)\p_floor.sbr" \
	"$(INTDIR)\p_inter.sbr" \
	"$(INTDIR)\p_lights.sbr" \
	"$(INTDIR)\p_map.sbr" \
	"$(INTDIR)\p_maputl.sbr" \
	"$(INTDIR)\p_mobj.sbr" \
	"$(INTDIR)\p_plats.sbr" \
	"$(INTDIR)\p_pspr.sbr" \
	"$(INTDIR)\p_saveg.sbr" \
	"$(INTDIR)\p_setup.sbr" \
	"$(INTDIR)\p_sight.sbr" \
	"$(INTDIR)\p_spec.sbr" \
	"$(INTDIR)\p_switch.sbr" \
	"$(INTDIR)\p_telept.sbr" \
	"$(INTDIR)\p_tick.sbr" \
	"$(INTDIR)\p_user.sbr" \
	"$(INTDIR)\r_bsp.sbr" \
	"$(INTDIR)\r_data.sbr" \
	"$(INTDIR)\r_draw.sbr" \
	"$(INTDIR)\r_main.sbr" \
	"$(INTDIR)\r_plane.sbr" \
	"$(INTDIR)\r_segs.sbr" \
	"$(INTDIR)\r_sky.sbr" \
	"$(INTDIR)\r_things.sbr" \
	"$(INTDIR)\s_sound.sbr" \
	"$(INTDIR)\savepic.sbr" \
	"$(INTDIR)\snd_dx.sbr" \
	"$(INTDIR)\sounds.sbr" \
	"$(INTDIR)\st_lib.sbr" \
	"$(INTDIR)\st_stuff.sbr" \
	"$(INTDIR)\sys_win.sbr" \
	"$(INTDIR)\tables.sbr" \
	"$(INTDIR)\v_video.sbr" \
	"$(INTDIR)\w_wad.sbr" \
	"$(INTDIR)\wi_stuff.sbr" \
	"$(INTDIR)\win_inpt.sbr" \
	"$(INTDIR)\win_joy.sbr" \
	"$(INTDIR)\win_kbrd.sbr" \
	"$(INTDIR)\win_mous.sbr" \
	"$(INTDIR)\win_video.sbr" \
	"$(INTDIR)\z_zone.sbr"

"$(OUTDIR)\GLDOOM.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib dinput.lib dsound.lib wsock32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /nodefaultlib
LINK32_FLAGS=opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib dinput.lib dsound.lib\
 wsock32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/GLDOOM.pdb" /machine:I386 /out:"$(OUTDIR)/GLDOOM.exe" 
LINK32_OBJS= \
	"$(INTDIR)\am_map.obj" \
	"$(INTDIR)\d_console.obj" \
	"$(INTDIR)\d_items.obj" \
	"$(INTDIR)\d_main.obj" \
	"$(INTDIR)\d_net.obj" \
	"$(INTDIR)\doomdef.obj" \
	"$(INTDIR)\doomstat.obj" \
	"$(INTDIR)\dstrings.obj" \
	"$(INTDIR)\dxerr.obj" \
	"$(INTDIR)\f_finale.obj" \
	"$(INTDIR)\f_wipe.obj" \
	"$(INTDIR)\g_game.obj" \
	"$(INTDIR)\gconsole.obj" \
	"$(INTDIR)\GL_utils.obj" \
	"$(INTDIR)\gl_video.obj" \
	"$(INTDIR)\GLdoom.res" \
	"$(INTDIR)\glsbar.obj" \
	"$(INTDIR)\hu_lib.obj" \
	"$(INTDIR)\hu_stuff.obj" \
	"$(INTDIR)\i_cd.obj" \
	"$(INTDIR)\i_main.obj" \
	"$(INTDIR)\i_midi.obj" \
	"$(INTDIR)\i_net.obj" \
	"$(INTDIR)\i_system.obj" \
	"$(INTDIR)\info.obj" \
	"$(INTDIR)\lights.obj" \
	"$(INTDIR)\m_argv.obj" \
	"$(INTDIR)\m_bbox.obj" \
	"$(INTDIR)\m_cheat.obj" \
	"$(INTDIR)\m_fixed.obj" \
	"$(INTDIR)\m_menu.obj" \
	"$(INTDIR)\m_misc.obj" \
	"$(INTDIR)\m_music.obj" \
	"$(INTDIR)\m_random.obj" \
	"$(INTDIR)\m_swap.obj" \
	"$(INTDIR)\newlumps.obj" \
	"$(INTDIR)\p_ceilng.obj" \
	"$(INTDIR)\p_doors.obj" \
	"$(INTDIR)\p_enemy.obj" \
	"$(INTDIR)\p_floor.obj" \
	"$(INTDIR)\p_inter.obj" \
	"$(INTDIR)\p_lights.obj" \
	"$(INTDIR)\p_map.obj" \
	"$(INTDIR)\p_maputl.obj" \
	"$(INTDIR)\p_mobj.obj" \
	"$(INTDIR)\p_plats.obj" \
	"$(INTDIR)\p_pspr.obj" \
	"$(INTDIR)\p_saveg.obj" \
	"$(INTDIR)\p_setup.obj" \
	"$(INTDIR)\p_sight.obj" \
	"$(INTDIR)\p_spec.obj" \
	"$(INTDIR)\p_switch.obj" \
	"$(INTDIR)\p_telept.obj" \
	"$(INTDIR)\p_tick.obj" \
	"$(INTDIR)\p_user.obj" \
	"$(INTDIR)\r_bsp.obj" \
	"$(INTDIR)\r_data.obj" \
	"$(INTDIR)\r_draw.obj" \
	"$(INTDIR)\r_main.obj" \
	"$(INTDIR)\r_plane.obj" \
	"$(INTDIR)\r_segs.obj" \
	"$(INTDIR)\r_sky.obj" \
	"$(INTDIR)\r_things.obj" \
	"$(INTDIR)\s_sound.obj" \
	"$(INTDIR)\savepic.obj" \
	"$(INTDIR)\snd_dx.obj" \
	"$(INTDIR)\sounds.obj" \
	"$(INTDIR)\st_lib.obj" \
	"$(INTDIR)\st_stuff.obj" \
	"$(INTDIR)\sys_win.obj" \
	"$(INTDIR)\tables.obj" \
	"$(INTDIR)\v_video.obj" \
	"$(INTDIR)\w_wad.obj" \
	"$(INTDIR)\wi_stuff.obj" \
	"$(INTDIR)\win_inpt.obj" \
	"$(INTDIR)\win_joy.obj" \
	"$(INTDIR)\win_kbrd.obj" \
	"$(INTDIR)\win_mous.obj" \
	"$(INTDIR)\win_video.obj" \
	"$(INTDIR)\z_zone.obj"

"$(OUTDIR)\GLDOOM.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "GLDOOM - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\GLDOOM.exe" "$(OUTDIR)\GLDOOM.bsc"

CLEAN : 
	-@erase "$(INTDIR)\am_map.obj"
	-@erase "$(INTDIR)\am_map.sbr"
	-@erase "$(INTDIR)\d_console.obj"
	-@erase "$(INTDIR)\d_console.sbr"
	-@erase "$(INTDIR)\d_items.obj"
	-@erase "$(INTDIR)\d_items.sbr"
	-@erase "$(INTDIR)\d_main.obj"
	-@erase "$(INTDIR)\d_main.sbr"
	-@erase "$(INTDIR)\d_net.obj"
	-@erase "$(INTDIR)\d_net.sbr"
	-@erase "$(INTDIR)\doomdef.obj"
	-@erase "$(INTDIR)\doomdef.sbr"
	-@erase "$(INTDIR)\doomstat.obj"
	-@erase "$(INTDIR)\doomstat.sbr"
	-@erase "$(INTDIR)\dstrings.obj"
	-@erase "$(INTDIR)\dstrings.sbr"
	-@erase "$(INTDIR)\dxerr.obj"
	-@erase "$(INTDIR)\dxerr.sbr"
	-@erase "$(INTDIR)\f_finale.obj"
	-@erase "$(INTDIR)\f_finale.sbr"
	-@erase "$(INTDIR)\f_wipe.obj"
	-@erase "$(INTDIR)\f_wipe.sbr"
	-@erase "$(INTDIR)\g_game.obj"
	-@erase "$(INTDIR)\g_game.sbr"
	-@erase "$(INTDIR)\gconsole.obj"
	-@erase "$(INTDIR)\gconsole.sbr"
	-@erase "$(INTDIR)\GL_utils.obj"
	-@erase "$(INTDIR)\GL_utils.sbr"
	-@erase "$(INTDIR)\gl_video.obj"
	-@erase "$(INTDIR)\gl_video.sbr"
	-@erase "$(INTDIR)\GLdoom.res"
	-@erase "$(INTDIR)\glsbar.obj"
	-@erase "$(INTDIR)\glsbar.sbr"
	-@erase "$(INTDIR)\hu_lib.obj"
	-@erase "$(INTDIR)\hu_lib.sbr"
	-@erase "$(INTDIR)\hu_stuff.obj"
	-@erase "$(INTDIR)\hu_stuff.sbr"
	-@erase "$(INTDIR)\i_cd.obj"
	-@erase "$(INTDIR)\i_cd.sbr"
	-@erase "$(INTDIR)\i_main.obj"
	-@erase "$(INTDIR)\i_main.sbr"
	-@erase "$(INTDIR)\i_midi.obj"
	-@erase "$(INTDIR)\i_midi.sbr"
	-@erase "$(INTDIR)\i_net.obj"
	-@erase "$(INTDIR)\i_net.sbr"
	-@erase "$(INTDIR)\i_system.obj"
	-@erase "$(INTDIR)\i_system.sbr"
	-@erase "$(INTDIR)\info.obj"
	-@erase "$(INTDIR)\info.sbr"
	-@erase "$(INTDIR)\lights.obj"
	-@erase "$(INTDIR)\lights.sbr"
	-@erase "$(INTDIR)\m_argv.obj"
	-@erase "$(INTDIR)\m_argv.sbr"
	-@erase "$(INTDIR)\m_bbox.obj"
	-@erase "$(INTDIR)\m_bbox.sbr"
	-@erase "$(INTDIR)\m_cheat.obj"
	-@erase "$(INTDIR)\m_cheat.sbr"
	-@erase "$(INTDIR)\m_fixed.obj"
	-@erase "$(INTDIR)\m_fixed.sbr"
	-@erase "$(INTDIR)\m_menu.obj"
	-@erase "$(INTDIR)\m_menu.sbr"
	-@erase "$(INTDIR)\m_misc.obj"
	-@erase "$(INTDIR)\m_misc.sbr"
	-@erase "$(INTDIR)\m_music.obj"
	-@erase "$(INTDIR)\m_music.sbr"
	-@erase "$(INTDIR)\m_random.obj"
	-@erase "$(INTDIR)\m_random.sbr"
	-@erase "$(INTDIR)\m_swap.obj"
	-@erase "$(INTDIR)\m_swap.sbr"
	-@erase "$(INTDIR)\newlumps.obj"
	-@erase "$(INTDIR)\newlumps.sbr"
	-@erase "$(INTDIR)\p_ceilng.obj"
	-@erase "$(INTDIR)\p_ceilng.sbr"
	-@erase "$(INTDIR)\p_doors.obj"
	-@erase "$(INTDIR)\p_doors.sbr"
	-@erase "$(INTDIR)\p_enemy.obj"
	-@erase "$(INTDIR)\p_enemy.sbr"
	-@erase "$(INTDIR)\p_floor.obj"
	-@erase "$(INTDIR)\p_floor.sbr"
	-@erase "$(INTDIR)\p_inter.obj"
	-@erase "$(INTDIR)\p_inter.sbr"
	-@erase "$(INTDIR)\p_lights.obj"
	-@erase "$(INTDIR)\p_lights.sbr"
	-@erase "$(INTDIR)\p_map.obj"
	-@erase "$(INTDIR)\p_map.sbr"
	-@erase "$(INTDIR)\p_maputl.obj"
	-@erase "$(INTDIR)\p_maputl.sbr"
	-@erase "$(INTDIR)\p_mobj.obj"
	-@erase "$(INTDIR)\p_mobj.sbr"
	-@erase "$(INTDIR)\p_plats.obj"
	-@erase "$(INTDIR)\p_plats.sbr"
	-@erase "$(INTDIR)\p_pspr.obj"
	-@erase "$(INTDIR)\p_pspr.sbr"
	-@erase "$(INTDIR)\p_saveg.obj"
	-@erase "$(INTDIR)\p_saveg.sbr"
	-@erase "$(INTDIR)\p_setup.obj"
	-@erase "$(INTDIR)\p_setup.sbr"
	-@erase "$(INTDIR)\p_sight.obj"
	-@erase "$(INTDIR)\p_sight.sbr"
	-@erase "$(INTDIR)\p_spec.obj"
	-@erase "$(INTDIR)\p_spec.sbr"
	-@erase "$(INTDIR)\p_switch.obj"
	-@erase "$(INTDIR)\p_switch.sbr"
	-@erase "$(INTDIR)\p_telept.obj"
	-@erase "$(INTDIR)\p_telept.sbr"
	-@erase "$(INTDIR)\p_tick.obj"
	-@erase "$(INTDIR)\p_tick.sbr"
	-@erase "$(INTDIR)\p_user.obj"
	-@erase "$(INTDIR)\p_user.sbr"
	-@erase "$(INTDIR)\r_bsp.obj"
	-@erase "$(INTDIR)\r_bsp.sbr"
	-@erase "$(INTDIR)\r_data.obj"
	-@erase "$(INTDIR)\r_data.sbr"
	-@erase "$(INTDIR)\r_draw.obj"
	-@erase "$(INTDIR)\r_draw.sbr"
	-@erase "$(INTDIR)\r_main.obj"
	-@erase "$(INTDIR)\r_main.sbr"
	-@erase "$(INTDIR)\r_plane.obj"
	-@erase "$(INTDIR)\r_plane.sbr"
	-@erase "$(INTDIR)\r_segs.obj"
	-@erase "$(INTDIR)\r_segs.sbr"
	-@erase "$(INTDIR)\r_sky.obj"
	-@erase "$(INTDIR)\r_sky.sbr"
	-@erase "$(INTDIR)\r_things.obj"
	-@erase "$(INTDIR)\r_things.sbr"
	-@erase "$(INTDIR)\s_sound.obj"
	-@erase "$(INTDIR)\s_sound.sbr"
	-@erase "$(INTDIR)\savepic.obj"
	-@erase "$(INTDIR)\savepic.sbr"
	-@erase "$(INTDIR)\snd_dx.obj"
	-@erase "$(INTDIR)\snd_dx.sbr"
	-@erase "$(INTDIR)\sounds.obj"
	-@erase "$(INTDIR)\sounds.sbr"
	-@erase "$(INTDIR)\st_lib.obj"
	-@erase "$(INTDIR)\st_lib.sbr"
	-@erase "$(INTDIR)\st_stuff.obj"
	-@erase "$(INTDIR)\st_stuff.sbr"
	-@erase "$(INTDIR)\sys_win.obj"
	-@erase "$(INTDIR)\sys_win.sbr"
	-@erase "$(INTDIR)\tables.obj"
	-@erase "$(INTDIR)\tables.sbr"
	-@erase "$(INTDIR)\v_video.obj"
	-@erase "$(INTDIR)\v_video.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\w_wad.obj"
	-@erase "$(INTDIR)\w_wad.sbr"
	-@erase "$(INTDIR)\wi_stuff.obj"
	-@erase "$(INTDIR)\wi_stuff.sbr"
	-@erase "$(INTDIR)\win_inpt.obj"
	-@erase "$(INTDIR)\win_inpt.sbr"
	-@erase "$(INTDIR)\win_joy.obj"
	-@erase "$(INTDIR)\win_joy.sbr"
	-@erase "$(INTDIR)\win_kbrd.obj"
	-@erase "$(INTDIR)\win_kbrd.sbr"
	-@erase "$(INTDIR)\win_mous.obj"
	-@erase "$(INTDIR)\win_mous.sbr"
	-@erase "$(INTDIR)\win_video.obj"
	-@erase "$(INTDIR)\win_video.sbr"
	-@erase "$(INTDIR)\z_zone.obj"
	-@erase "$(INTDIR)\z_zone.sbr"
	-@erase "$(OUTDIR)\GLDOOM.bsc"
	-@erase "$(OUTDIR)\GLDOOM.exe"
	-@erase "$(OUTDIR)\GLDOOM.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /FR /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /MLd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /FR"$(INTDIR)/" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/GLdoom.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/GLDOOM.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\am_map.sbr" \
	"$(INTDIR)\d_console.sbr" \
	"$(INTDIR)\d_items.sbr" \
	"$(INTDIR)\d_main.sbr" \
	"$(INTDIR)\d_net.sbr" \
	"$(INTDIR)\doomdef.sbr" \
	"$(INTDIR)\doomstat.sbr" \
	"$(INTDIR)\dstrings.sbr" \
	"$(INTDIR)\dxerr.sbr" \
	"$(INTDIR)\f_finale.sbr" \
	"$(INTDIR)\f_wipe.sbr" \
	"$(INTDIR)\g_game.sbr" \
	"$(INTDIR)\gconsole.sbr" \
	"$(INTDIR)\GL_utils.sbr" \
	"$(INTDIR)\gl_video.sbr" \
	"$(INTDIR)\glsbar.sbr" \
	"$(INTDIR)\hu_lib.sbr" \
	"$(INTDIR)\hu_stuff.sbr" \
	"$(INTDIR)\i_cd.sbr" \
	"$(INTDIR)\i_main.sbr" \
	"$(INTDIR)\i_midi.sbr" \
	"$(INTDIR)\i_net.sbr" \
	"$(INTDIR)\i_system.sbr" \
	"$(INTDIR)\info.sbr" \
	"$(INTDIR)\lights.sbr" \
	"$(INTDIR)\m_argv.sbr" \
	"$(INTDIR)\m_bbox.sbr" \
	"$(INTDIR)\m_cheat.sbr" \
	"$(INTDIR)\m_fixed.sbr" \
	"$(INTDIR)\m_menu.sbr" \
	"$(INTDIR)\m_misc.sbr" \
	"$(INTDIR)\m_music.sbr" \
	"$(INTDIR)\m_random.sbr" \
	"$(INTDIR)\m_swap.sbr" \
	"$(INTDIR)\newlumps.sbr" \
	"$(INTDIR)\p_ceilng.sbr" \
	"$(INTDIR)\p_doors.sbr" \
	"$(INTDIR)\p_enemy.sbr" \
	"$(INTDIR)\p_floor.sbr" \
	"$(INTDIR)\p_inter.sbr" \
	"$(INTDIR)\p_lights.sbr" \
	"$(INTDIR)\p_map.sbr" \
	"$(INTDIR)\p_maputl.sbr" \
	"$(INTDIR)\p_mobj.sbr" \
	"$(INTDIR)\p_plats.sbr" \
	"$(INTDIR)\p_pspr.sbr" \
	"$(INTDIR)\p_saveg.sbr" \
	"$(INTDIR)\p_setup.sbr" \
	"$(INTDIR)\p_sight.sbr" \
	"$(INTDIR)\p_spec.sbr" \
	"$(INTDIR)\p_switch.sbr" \
	"$(INTDIR)\p_telept.sbr" \
	"$(INTDIR)\p_tick.sbr" \
	"$(INTDIR)\p_user.sbr" \
	"$(INTDIR)\r_bsp.sbr" \
	"$(INTDIR)\r_data.sbr" \
	"$(INTDIR)\r_draw.sbr" \
	"$(INTDIR)\r_main.sbr" \
	"$(INTDIR)\r_plane.sbr" \
	"$(INTDIR)\r_segs.sbr" \
	"$(INTDIR)\r_sky.sbr" \
	"$(INTDIR)\r_things.sbr" \
	"$(INTDIR)\s_sound.sbr" \
	"$(INTDIR)\savepic.sbr" \
	"$(INTDIR)\snd_dx.sbr" \
	"$(INTDIR)\sounds.sbr" \
	"$(INTDIR)\st_lib.sbr" \
	"$(INTDIR)\st_stuff.sbr" \
	"$(INTDIR)\sys_win.sbr" \
	"$(INTDIR)\tables.sbr" \
	"$(INTDIR)\v_video.sbr" \
	"$(INTDIR)\w_wad.sbr" \
	"$(INTDIR)\wi_stuff.sbr" \
	"$(INTDIR)\win_inpt.sbr" \
	"$(INTDIR)\win_joy.sbr" \
	"$(INTDIR)\win_kbrd.sbr" \
	"$(INTDIR)\win_mous.sbr" \
	"$(INTDIR)\win_video.sbr" \
	"$(INTDIR)\z_zone.sbr"

"$(OUTDIR)\GLDOOM.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib dinput.lib dsound.lib wsock32.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386
# SUBTRACT LINK32 /map /nodefaultlib
LINK32_FLAGS=opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib dinput.lib dsound.lib\
 wsock32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/GLDOOM.pdb" /debug /machine:I386 /out:"$(OUTDIR)/GLDOOM.exe" 
LINK32_OBJS= \
	"$(INTDIR)\am_map.obj" \
	"$(INTDIR)\d_console.obj" \
	"$(INTDIR)\d_items.obj" \
	"$(INTDIR)\d_main.obj" \
	"$(INTDIR)\d_net.obj" \
	"$(INTDIR)\doomdef.obj" \
	"$(INTDIR)\doomstat.obj" \
	"$(INTDIR)\dstrings.obj" \
	"$(INTDIR)\dxerr.obj" \
	"$(INTDIR)\f_finale.obj" \
	"$(INTDIR)\f_wipe.obj" \
	"$(INTDIR)\g_game.obj" \
	"$(INTDIR)\gconsole.obj" \
	"$(INTDIR)\GL_utils.obj" \
	"$(INTDIR)\gl_video.obj" \
	"$(INTDIR)\GLdoom.res" \
	"$(INTDIR)\glsbar.obj" \
	"$(INTDIR)\hu_lib.obj" \
	"$(INTDIR)\hu_stuff.obj" \
	"$(INTDIR)\i_cd.obj" \
	"$(INTDIR)\i_main.obj" \
	"$(INTDIR)\i_midi.obj" \
	"$(INTDIR)\i_net.obj" \
	"$(INTDIR)\i_system.obj" \
	"$(INTDIR)\info.obj" \
	"$(INTDIR)\lights.obj" \
	"$(INTDIR)\m_argv.obj" \
	"$(INTDIR)\m_bbox.obj" \
	"$(INTDIR)\m_cheat.obj" \
	"$(INTDIR)\m_fixed.obj" \
	"$(INTDIR)\m_menu.obj" \
	"$(INTDIR)\m_misc.obj" \
	"$(INTDIR)\m_music.obj" \
	"$(INTDIR)\m_random.obj" \
	"$(INTDIR)\m_swap.obj" \
	"$(INTDIR)\newlumps.obj" \
	"$(INTDIR)\p_ceilng.obj" \
	"$(INTDIR)\p_doors.obj" \
	"$(INTDIR)\p_enemy.obj" \
	"$(INTDIR)\p_floor.obj" \
	"$(INTDIR)\p_inter.obj" \
	"$(INTDIR)\p_lights.obj" \
	"$(INTDIR)\p_map.obj" \
	"$(INTDIR)\p_maputl.obj" \
	"$(INTDIR)\p_mobj.obj" \
	"$(INTDIR)\p_plats.obj" \
	"$(INTDIR)\p_pspr.obj" \
	"$(INTDIR)\p_saveg.obj" \
	"$(INTDIR)\p_setup.obj" \
	"$(INTDIR)\p_sight.obj" \
	"$(INTDIR)\p_spec.obj" \
	"$(INTDIR)\p_switch.obj" \
	"$(INTDIR)\p_telept.obj" \
	"$(INTDIR)\p_tick.obj" \
	"$(INTDIR)\p_user.obj" \
	"$(INTDIR)\r_bsp.obj" \
	"$(INTDIR)\r_data.obj" \
	"$(INTDIR)\r_draw.obj" \
	"$(INTDIR)\r_main.obj" \
	"$(INTDIR)\r_plane.obj" \
	"$(INTDIR)\r_segs.obj" \
	"$(INTDIR)\r_sky.obj" \
	"$(INTDIR)\r_things.obj" \
	"$(INTDIR)\s_sound.obj" \
	"$(INTDIR)\savepic.obj" \
	"$(INTDIR)\snd_dx.obj" \
	"$(INTDIR)\sounds.obj" \
	"$(INTDIR)\st_lib.obj" \
	"$(INTDIR)\st_stuff.obj" \
	"$(INTDIR)\sys_win.obj" \
	"$(INTDIR)\tables.obj" \
	"$(INTDIR)\v_video.obj" \
	"$(INTDIR)\w_wad.obj" \
	"$(INTDIR)\wi_stuff.obj" \
	"$(INTDIR)\win_inpt.obj" \
	"$(INTDIR)\win_joy.obj" \
	"$(INTDIR)\win_kbrd.obj" \
	"$(INTDIR)\win_mous.obj" \
	"$(INTDIR)\win_video.obj" \
	"$(INTDIR)\z_zone.obj"

"$(OUTDIR)\GLDOOM.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "GLDOOM - Win32 Release"
# Name "GLDOOM - Win32 Debug"

!IF  "$(CFG)" == "GLDOOM - Win32 Release"

!ELSEIF  "$(CFG)" == "GLDOOM - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\z_zone.c
DEP_CPP_Z_ZON=\
	".\d_event.h"\
	".\d_ticcmd.h"\
	".\doomdef.h"\
	".\doomtype.h"\
	".\i_system.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\z_zone.obj" : $(SOURCE) $(DEP_CPP_Z_ZON) "$(INTDIR)"

"$(INTDIR)\z_zone.sbr" : $(SOURCE) $(DEP_CPP_Z_ZON) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\d_items.c
DEP_CPP_D_ITE=\
	".\d_items.h"\
	".\d_think.h"\
	".\doomdef.h"\
	".\info.h"\
	

"$(INTDIR)\d_items.obj" : $(SOURCE) $(DEP_CPP_D_ITE) "$(INTDIR)"

"$(INTDIR)\d_items.sbr" : $(SOURCE) $(DEP_CPP_D_ITE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\d_main.c

!IF  "$(CFG)" == "GLDOOM - Win32 Release"

DEP_CPP_D_MAI=\
	".\am_map.h"\
	".\d_console.h"\
	".\d_englsh.h"\
	".\d_event.h"\
	".\d_french.h"\
	".\d_items.h"\
	".\d_main.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\dstrings.h"\
	".\f_finale.h"\
	".\f_wipe.h"\
	".\g_game.h"\
	".\gconsole.h"\
	".\gl_video.h"\
	".\gldefs.h"\
	".\hu_stuff.h"\
	".\i_sound.h"\
	".\i_system.h"\
	".\i_video.h"\
	".\info.h"\
	".\m_argv.h"\
	".\m_fixed.h"\
	".\m_menu.h"\
	".\m_misc.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_setup.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\st_stuff.h"\
	".\tables.h"\
	".\v_video.h"\
	".\w_wad.h"\
	".\wi_stuff.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\gl\Gl.h"\
	{$(INCLUDE)}"\sys\Stat.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\unistd.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\d_main.obj" : $(SOURCE) $(DEP_CPP_D_MAI) "$(INTDIR)"

"$(INTDIR)\d_main.sbr" : $(SOURCE) $(DEP_CPP_D_MAI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GLDOOM - Win32 Debug"

DEP_CPP_D_MAI=\
	".\am_map.h"\
	".\d_console.h"\
	".\d_englsh.h"\
	".\d_event.h"\
	".\d_french.h"\
	".\d_items.h"\
	".\d_main.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\dstrings.h"\
	".\f_finale.h"\
	".\f_wipe.h"\
	".\g_game.h"\
	".\gconsole.h"\
	".\gl_video.h"\
	".\gldefs.h"\
	".\hu_stuff.h"\
	".\i_sound.h"\
	".\i_system.h"\
	".\i_video.h"\
	".\info.h"\
	".\m_argv.h"\
	".\m_fixed.h"\
	".\m_menu.h"\
	".\m_misc.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_setup.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\st_stuff.h"\
	".\tables.h"\
	".\v_video.h"\
	".\w_wad.h"\
	".\wi_stuff.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\gl\Gl.h"\
	{$(INCLUDE)}"\sys\Stat.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\unistd.h"\
	{$(INCLUDE)}"\values.h"\
	
NODEP_CPP_D_MAI=\
	".\-nomonsters"\
	

"$(INTDIR)\d_main.obj" : $(SOURCE) $(DEP_CPP_D_MAI) "$(INTDIR)"

"$(INTDIR)\d_main.sbr" : $(SOURCE) $(DEP_CPP_D_MAI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\d_net.c
DEP_CPP_D_NET=\
	".\d_console.h"\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\g_game.h"\
	".\gconsole.h"\
	".\i_net.h"\
	".\i_system.h"\
	".\i_video.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\m_menu.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\tables.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\d_net.obj" : $(SOURCE) $(DEP_CPP_D_NET) "$(INTDIR)"

"$(INTDIR)\d_net.sbr" : $(SOURCE) $(DEP_CPP_D_NET) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\doomdef.c
DEP_CPP_DOOMD=\
	".\doomdef.h"\
	

"$(INTDIR)\doomdef.obj" : $(SOURCE) $(DEP_CPP_DOOMD) "$(INTDIR)"

"$(INTDIR)\doomdef.sbr" : $(SOURCE) $(DEP_CPP_DOOMD) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\doomstat.c
DEP_CPP_DOOMS=\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\tables.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\doomstat.obj" : $(SOURCE) $(DEP_CPP_DOOMS) "$(INTDIR)"

"$(INTDIR)\doomstat.sbr" : $(SOURCE) $(DEP_CPP_DOOMS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dstrings.c
DEP_CPP_DSTRI=\
	".\d_englsh.h"\
	".\d_french.h"\
	".\dstrings.h"\
	

"$(INTDIR)\dstrings.obj" : $(SOURCE) $(DEP_CPP_DSTRI) "$(INTDIR)"

"$(INTDIR)\dstrings.sbr" : $(SOURCE) $(DEP_CPP_DSTRI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\f_finale.c
DEP_CPP_F_FIN=\
	".\d_englsh.h"\
	".\d_event.h"\
	".\d_french.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\dstrings.h"\
	".\gl_utils.h"\
	".\gldefs.h"\
	".\hu_stuff.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\m_swap.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_state.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\sys_win.h"\
	".\tables.h"\
	".\v_video.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\gl\Gl.h"\
	{$(INCLUDE)}"\gl\Glu.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\f_finale.obj" : $(SOURCE) $(DEP_CPP_F_FIN) "$(INTDIR)"

"$(INTDIR)\f_finale.sbr" : $(SOURCE) $(DEP_CPP_F_FIN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\f_wipe.c
DEP_CPP_F_WIP=\
	".\d_items.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomtype.h"\
	".\f_wipe.h"\
	".\i_video.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\m_random.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_state.h"\
	".\tables.h"\
	".\v_video.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\f_wipe.obj" : $(SOURCE) $(DEP_CPP_F_WIP) "$(INTDIR)"

"$(INTDIR)\f_wipe.sbr" : $(SOURCE) $(DEP_CPP_F_WIP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\g_game.c
DEP_CPP_G_GAM=\
	".\am_map.h"\
	".\d_englsh.h"\
	".\d_event.h"\
	".\d_french.h"\
	".\d_items.h"\
	".\d_main.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\dstrings.h"\
	".\f_finale.h"\
	".\g_game.h"\
	".\gconsole.h"\
	".\hu_stuff.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_argv.h"\
	".\m_fixed.h"\
	".\m_menu.h"\
	".\m_misc.h"\
	".\m_random.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_saveg.h"\
	".\p_setup.h"\
	".\p_spec.h"\
	".\p_tick.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_sky.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\st_stuff.h"\
	".\tables.h"\
	".\v_video.h"\
	".\w_wad.h"\
	".\wi_stuff.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\g_game.obj" : $(SOURCE) $(DEP_CPP_G_GAM) "$(INTDIR)"

"$(INTDIR)\g_game.sbr" : $(SOURCE) $(DEP_CPP_G_GAM) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\hu_lib.c
DEP_CPP_HU_LI=\
	".\d_items.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomtype.h"\
	".\hu_lib.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\m_swap.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\tables.h"\
	".\v_video.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\hu_lib.obj" : $(SOURCE) $(DEP_CPP_HU_LI) "$(INTDIR)"

"$(INTDIR)\hu_lib.sbr" : $(SOURCE) $(DEP_CPP_HU_LI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\hu_stuff.c
DEP_CPP_HU_ST=\
	".\d_console.h"\
	".\d_englsh.h"\
	".\d_event.h"\
	".\d_french.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\dstrings.h"\
	".\gl_utils.h"\
	".\gldefs.h"\
	".\hu_lib.h"\
	".\hu_stuff.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\m_swap.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_defs.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\tables.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\hu_stuff.obj" : $(SOURCE) $(DEP_CPP_HU_ST) "$(INTDIR)"

"$(INTDIR)\hu_stuff.sbr" : $(SOURCE) $(DEP_CPP_HU_ST) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\i_main.c
DEP_CPP_I_MAI=\
	".\d_event.h"\
	".\d_main.h"\
	".\doomdef.h"\
	".\doomtype.h"\
	".\m_argv.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\i_main.obj" : $(SOURCE) $(DEP_CPP_I_MAI) "$(INTDIR)"

"$(INTDIR)\i_main.sbr" : $(SOURCE) $(DEP_CPP_I_MAI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\i_system.c
DEP_CPP_I_SYS=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\g_game.h"\
	".\i_sound.h"\
	".\i_system.h"\
	".\i_video.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\m_misc.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\sounds.h"\
	".\tables.h"\
	".\win_inpt.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\i_system.obj" : $(SOURCE) $(DEP_CPP_I_SYS) "$(INTDIR)"

"$(INTDIR)\i_system.sbr" : $(SOURCE) $(DEP_CPP_I_SYS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\info.c
DEP_CPP_INFO_=\
	".\d_think.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomtype.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\p_mobj.h"\
	".\sounds.h"\
	".\tables.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\info.obj" : $(SOURCE) $(DEP_CPP_INFO_) "$(INTDIR)"

"$(INTDIR)\info.sbr" : $(SOURCE) $(DEP_CPP_INFO_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\m_argv.c

"$(INTDIR)\m_argv.obj" : $(SOURCE) "$(INTDIR)"

"$(INTDIR)\m_argv.sbr" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\m_bbox.c
DEP_CPP_M_BBO=\
	".\m_bbox.h"\
	".\m_fixed.h"\
	

"$(INTDIR)\m_bbox.obj" : $(SOURCE) $(DEP_CPP_M_BBO) "$(INTDIR)"

"$(INTDIR)\m_bbox.sbr" : $(SOURCE) $(DEP_CPP_M_BBO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\m_cheat.c
DEP_CPP_M_CHE=\
	".\m_cheat.h"\
	

"$(INTDIR)\m_cheat.obj" : $(SOURCE) $(DEP_CPP_M_CHE) "$(INTDIR)"

"$(INTDIR)\m_cheat.sbr" : $(SOURCE) $(DEP_CPP_M_CHE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\m_fixed.c
DEP_CPP_M_FIX=\
	".\d_event.h"\
	".\d_ticcmd.h"\
	".\doomtype.h"\
	".\i_system.h"\
	".\m_fixed.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\m_fixed.obj" : $(SOURCE) $(DEP_CPP_M_FIX) "$(INTDIR)"

"$(INTDIR)\m_fixed.sbr" : $(SOURCE) $(DEP_CPP_M_FIX) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\m_menu.c
DEP_CPP_M_MEN=\
	".\d_console.h"\
	".\d_englsh.h"\
	".\d_event.h"\
	".\d_french.h"\
	".\d_items.h"\
	".\d_main.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\dstrings.h"\
	".\g_game.h"\
	".\gl_utils.h"\
	".\gldefs.h"\
	".\hu_stuff.h"\
	".\i_system.h"\
	".\i_video.h"\
	".\info.h"\
	".\m_argv.h"\
	".\m_fixed.h"\
	".\m_menu.h"\
	".\m_swap.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\sys_win.h"\
	".\tables.h"\
	".\v_video.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\gl\Gl.h"\
	{$(INCLUDE)}"\sys\Stat.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\m_menu.obj" : $(SOURCE) $(DEP_CPP_M_MEN) "$(INTDIR)"

"$(INTDIR)\m_menu.sbr" : $(SOURCE) $(DEP_CPP_M_MEN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\m_misc.c
DEP_CPP_M_MIS=\
	".\d_englsh.h"\
	".\d_event.h"\
	".\d_french.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\dstrings.h"\
	".\gldefs.h"\
	".\hu_stuff.h"\
	".\i_system.h"\
	".\i_video.h"\
	".\info.h"\
	".\m_argv.h"\
	".\m_fixed.h"\
	".\m_misc.h"\
	".\m_swap.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_state.h"\
	".\tables.h"\
	".\v_video.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\gl\Gl.h"\
	{$(INCLUDE)}"\sys\Stat.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\m_misc.obj" : $(SOURCE) $(DEP_CPP_M_MIS) "$(INTDIR)"

"$(INTDIR)\m_misc.sbr" : $(SOURCE) $(DEP_CPP_M_MIS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\m_random.c

"$(INTDIR)\m_random.obj" : $(SOURCE) "$(INTDIR)"

"$(INTDIR)\m_random.sbr" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\m_swap.c
DEP_CPP_M_SWA=\
	".\m_swap.h"\
	

"$(INTDIR)\m_swap.obj" : $(SOURCE) $(DEP_CPP_M_SWA) "$(INTDIR)"

"$(INTDIR)\m_swap.sbr" : $(SOURCE) $(DEP_CPP_M_SWA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_ceilng.c
DEP_CPP_P_CEI=\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\tables.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_ceilng.obj" : $(SOURCE) $(DEP_CPP_P_CEI) "$(INTDIR)"

"$(INTDIR)\p_ceilng.sbr" : $(SOURCE) $(DEP_CPP_P_CEI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_doors.c
DEP_CPP_P_DOO=\
	".\d_englsh.h"\
	".\d_french.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\dstrings.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\tables.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_doors.obj" : $(SOURCE) $(DEP_CPP_P_DOO) "$(INTDIR)"

"$(INTDIR)\p_doors.sbr" : $(SOURCE) $(DEP_CPP_P_DOO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_enemy.c
DEP_CPP_P_ENE=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\g_game.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\m_random.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\tables.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_enemy.obj" : $(SOURCE) $(DEP_CPP_P_ENE) "$(INTDIR)"

"$(INTDIR)\p_enemy.sbr" : $(SOURCE) $(DEP_CPP_P_ENE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_floor.c
DEP_CPP_P_FLO=\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\tables.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_floor.obj" : $(SOURCE) $(DEP_CPP_P_FLO) "$(INTDIR)"

"$(INTDIR)\p_floor.sbr" : $(SOURCE) $(DEP_CPP_P_FLO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_inter.c
DEP_CPP_P_INT=\
	".\am_map.h"\
	".\d_englsh.h"\
	".\d_event.h"\
	".\d_french.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\dstrings.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\m_random.h"\
	".\p_inter.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\tables.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_inter.obj" : $(SOURCE) $(DEP_CPP_P_INT) "$(INTDIR)"

"$(INTDIR)\p_inter.sbr" : $(SOURCE) $(DEP_CPP_P_INT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_lights.c
DEP_CPP_P_LIG=\
	".\d_items.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomtype.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\m_random.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\tables.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_lights.obj" : $(SOURCE) $(DEP_CPP_P_LIG) "$(INTDIR)"

"$(INTDIR)\p_lights.sbr" : $(SOURCE) $(DEP_CPP_P_LIG) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_map.c
DEP_CPP_P_MAP=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_bbox.h"\
	".\m_fixed.h"\
	".\m_random.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\tables.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_map.obj" : $(SOURCE) $(DEP_CPP_P_MAP) "$(INTDIR)"

"$(INTDIR)\p_map.sbr" : $(SOURCE) $(DEP_CPP_P_MAP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_maputl.c
DEP_CPP_P_MAPU=\
	".\d_items.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomtype.h"\
	".\info.h"\
	".\m_bbox.h"\
	".\m_fixed.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\tables.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_maputl.obj" : $(SOURCE) $(DEP_CPP_P_MAPU) "$(INTDIR)"

"$(INTDIR)\p_maputl.sbr" : $(SOURCE) $(DEP_CPP_P_MAPU) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_mobj.c
DEP_CPP_P_MOB=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\hu_stuff.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\m_random.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\st_stuff.h"\
	".\tables.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_mobj.obj" : $(SOURCE) $(DEP_CPP_P_MOB) "$(INTDIR)"

"$(INTDIR)\p_mobj.sbr" : $(SOURCE) $(DEP_CPP_P_MOB) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_plats.c
DEP_CPP_P_PLA=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\m_random.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\tables.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_plats.obj" : $(SOURCE) $(DEP_CPP_P_PLA) "$(INTDIR)"

"$(INTDIR)\p_plats.sbr" : $(SOURCE) $(DEP_CPP_P_PLA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_pspr.c
DEP_CPP_P_PSP=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\m_random.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\tables.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_pspr.obj" : $(SOURCE) $(DEP_CPP_P_PSP) "$(INTDIR)"

"$(INTDIR)\p_pspr.sbr" : $(SOURCE) $(DEP_CPP_P_PSP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_saveg.c
DEP_CPP_P_SAV=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\tables.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_saveg.obj" : $(SOURCE) $(DEP_CPP_P_SAV) "$(INTDIR)"

"$(INTDIR)\p_saveg.sbr" : $(SOURCE) $(DEP_CPP_P_SAV) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_setup.c
DEP_CPP_P_SET=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\g_game.h"\
	".\gconsole.h"\
	".\gldefs.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_bbox.h"\
	".\m_fixed.h"\
	".\m_swap.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sys_win.h"\
	".\tables.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\gl\Gl.h"\
	{$(INCLUDE)}"\gl\Glu.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_setup.obj" : $(SOURCE) $(DEP_CPP_P_SET) "$(INTDIR)"

"$(INTDIR)\p_setup.sbr" : $(SOURCE) $(DEP_CPP_P_SET) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_sight.c
DEP_CPP_P_SIG=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomtype.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\tables.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_sight.obj" : $(SOURCE) $(DEP_CPP_P_SIG) "$(INTDIR)"

"$(INTDIR)\p_sight.sbr" : $(SOURCE) $(DEP_CPP_P_SIG) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_spec.c
DEP_CPP_P_SPE=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\g_game.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_argv.h"\
	".\m_fixed.h"\
	".\m_random.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\tables.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_spec.obj" : $(SOURCE) $(DEP_CPP_P_SPE) "$(INTDIR)"

"$(INTDIR)\p_spec.sbr" : $(SOURCE) $(DEP_CPP_P_SPE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_switch.c
DEP_CPP_P_SWI=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\g_game.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\tables.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_switch.obj" : $(SOURCE) $(DEP_CPP_P_SWI) "$(INTDIR)"

"$(INTDIR)\p_switch.sbr" : $(SOURCE) $(DEP_CPP_P_SWI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_telept.c
DEP_CPP_P_TEL=\
	".\d_items.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomtype.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\tables.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_telept.obj" : $(SOURCE) $(DEP_CPP_P_TEL) "$(INTDIR)"

"$(INTDIR)\p_telept.sbr" : $(SOURCE) $(DEP_CPP_P_TEL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_tick.c
DEP_CPP_P_TIC=\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\tables.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_tick.obj" : $(SOURCE) $(DEP_CPP_P_TIC) "$(INTDIR)"

"$(INTDIR)\p_tick.sbr" : $(SOURCE) $(DEP_CPP_P_TIC) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\p_user.c
DEP_CPP_P_USE=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\tables.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\p_user.obj" : $(SOURCE) $(DEP_CPP_P_USE) "$(INTDIR)"

"$(INTDIR)\p_user.sbr" : $(SOURCE) $(DEP_CPP_P_USE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\r_bsp.c
DEP_CPP_R_BSP=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_bbox.h"\
	".\m_fixed.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\sys_win.h"\
	".\tables.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\r_bsp.obj" : $(SOURCE) $(DEP_CPP_R_BSP) "$(INTDIR)"

"$(INTDIR)\r_bsp.sbr" : $(SOURCE) $(DEP_CPP_R_BSP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\r_data.c
DEP_CPP_R_DAT=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\gconsole.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\m_swap.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_sky.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\tables.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\r_data.obj" : $(SOURCE) $(DEP_CPP_R_DAT) "$(INTDIR)"

"$(INTDIR)\r_data.sbr" : $(SOURCE) $(DEP_CPP_R_DAT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\r_draw.c
DEP_CPP_R_DRA=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\tables.h"\
	".\v_video.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\r_draw.obj" : $(SOURCE) $(DEP_CPP_R_DRA) "$(INTDIR)"

"$(INTDIR)\r_draw.sbr" : $(SOURCE) $(DEP_CPP_R_DRA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\r_main.c
DEP_CPP_R_MAI=\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomtype.h"\
	".\info.h"\
	".\m_bbox.h"\
	".\m_fixed.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_sky.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\sys_win.h"\
	".\tables.h"\
	{$(INCLUDE)}"\gl\Gl.h"\
	{$(INCLUDE)}"\gl\Glu.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\r_main.obj" : $(SOURCE) $(DEP_CPP_R_MAI) "$(INTDIR)"

"$(INTDIR)\r_main.sbr" : $(SOURCE) $(DEP_CPP_R_MAI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\r_plane.c
DEP_CPP_R_PLA=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_sky.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\tables.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\r_plane.obj" : $(SOURCE) $(DEP_CPP_R_PLA) "$(INTDIR)"

"$(INTDIR)\r_plane.sbr" : $(SOURCE) $(DEP_CPP_R_PLA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\r_segs.c
DEP_CPP_R_SEG=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_sky.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\tables.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\r_segs.obj" : $(SOURCE) $(DEP_CPP_R_SEG) "$(INTDIR)"

"$(INTDIR)\r_segs.sbr" : $(SOURCE) $(DEP_CPP_R_SEG) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\r_sky.c
DEP_CPP_R_SKY=\
	".\d_items.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomtype.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_sky.h"\
	".\r_state.h"\
	".\tables.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\r_sky.obj" : $(SOURCE) $(DEP_CPP_R_SKY) "$(INTDIR)"

"$(INTDIR)\r_sky.sbr" : $(SOURCE) $(DEP_CPP_R_SKY) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\r_things.c
DEP_CPP_R_THI=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\gldefs.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\m_swap.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\tables.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\gl\Gl.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\r_things.obj" : $(SOURCE) $(DEP_CPP_R_THI) "$(INTDIR)"

"$(INTDIR)\r_things.sbr" : $(SOURCE) $(DEP_CPP_R_THI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\s_sound.c
DEP_CPP_S_SOU=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\i_sound.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\m_random.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\tables.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\s_sound.obj" : $(SOURCE) $(DEP_CPP_S_SOU) "$(INTDIR)"

"$(INTDIR)\s_sound.sbr" : $(SOURCE) $(DEP_CPP_S_SOU) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\sounds.c
DEP_CPP_SOUND=\
	".\doomtype.h"\
	".\sounds.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\sounds.obj" : $(SOURCE) $(DEP_CPP_SOUND) "$(INTDIR)"

"$(INTDIR)\sounds.sbr" : $(SOURCE) $(DEP_CPP_SOUND) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\st_lib.c
DEP_CPP_ST_LI=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomtype.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\m_swap.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\st_lib.h"\
	".\st_stuff.h"\
	".\tables.h"\
	".\v_video.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\st_lib.obj" : $(SOURCE) $(DEP_CPP_ST_LI) "$(INTDIR)"

"$(INTDIR)\st_lib.sbr" : $(SOURCE) $(DEP_CPP_ST_LI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\st_stuff.c
DEP_CPP_ST_ST=\
	".\am_map.h"\
	".\d_englsh.h"\
	".\d_event.h"\
	".\d_french.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\dstrings.h"\
	".\g_game.h"\
	".\gl_utils.h"\
	".\gldefs.h"\
	".\glsbar.h"\
	".\i_system.h"\
	".\i_video.h"\
	".\info.h"\
	".\m_cheat.h"\
	".\m_fixed.h"\
	".\m_random.h"\
	".\p_inter.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\st_lib.h"\
	".\st_stuff.h"\
	".\tables.h"\
	".\v_video.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\gl\Gl.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\st_stuff.obj" : $(SOURCE) $(DEP_CPP_ST_ST) "$(INTDIR)"

"$(INTDIR)\st_stuff.sbr" : $(SOURCE) $(DEP_CPP_ST_ST) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\tables.c
DEP_CPP_TABLE=\
	".\m_fixed.h"\
	".\tables.h"\
	

"$(INTDIR)\tables.obj" : $(SOURCE) $(DEP_CPP_TABLE) "$(INTDIR)"

"$(INTDIR)\tables.sbr" : $(SOURCE) $(DEP_CPP_TABLE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\v_video.c
DEP_CPP_V_VID=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomtype.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_bbox.h"\
	".\m_fixed.h"\
	".\m_swap.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\tables.h"\
	".\v_video.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\v_video.obj" : $(SOURCE) $(DEP_CPP_V_VID) "$(INTDIR)"

"$(INTDIR)\v_video.sbr" : $(SOURCE) $(DEP_CPP_V_VID) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\w_wad.c
DEP_CPP_W_WAD=\
	".\d_event.h"\
	".\d_ticcmd.h"\
	".\doomtype.h"\
	".\gconsole.h"\
	".\i_system.h"\
	".\m_swap.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\sys\Stat.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\unistd.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\w_wad.obj" : $(SOURCE) $(DEP_CPP_W_WAD) "$(INTDIR)"

"$(INTDIR)\w_wad.sbr" : $(SOURCE) $(DEP_CPP_W_WAD) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\wi_stuff.c
DEP_CPP_WI_ST=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\g_game.h"\
	".\gl_utils.h"\
	".\gldefs.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\m_random.h"\
	".\m_swap.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\tables.h"\
	".\v_video.h"\
	".\w_wad.h"\
	".\wi_stuff.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\gl\Gl.h"\
	{$(INCLUDE)}"\gl\Glu.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\wi_stuff.obj" : $(SOURCE) $(DEP_CPP_WI_ST) "$(INTDIR)"

"$(INTDIR)\wi_stuff.sbr" : $(SOURCE) $(DEP_CPP_WI_ST) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\am_map.c
DEP_CPP_AM_MA=\
	".\am_map.h"\
	".\d_englsh.h"\
	".\d_event.h"\
	".\d_french.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\dstrings.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_cheat.h"\
	".\m_fixed.h"\
	".\p_local.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\p_spec.h"\
	".\r_bsp.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_draw.h"\
	".\r_local.h"\
	".\r_main.h"\
	".\r_plane.h"\
	".\r_segs.h"\
	".\r_state.h"\
	".\r_things.h"\
	".\st_stuff.h"\
	".\tables.h"\
	".\v_video.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\gl\Gl.h"\
	{$(INCLUDE)}"\gl\Glu.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\am_map.obj" : $(SOURCE) $(DEP_CPP_AM_MA) "$(INTDIR)"

"$(INTDIR)\am_map.sbr" : $(SOURCE) $(DEP_CPP_AM_MA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\i_net.c
DEP_CPP_I_NET=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\gconsole.h"\
	".\i_net.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_argv.h"\
	".\m_fixed.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\tables.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\i_net.obj" : $(SOURCE) $(DEP_CPP_I_NET) "$(INTDIR)"

"$(INTDIR)\i_net.sbr" : $(SOURCE) $(DEP_CPP_I_NET) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\d_console.c
DEP_CPP_D_CON=\
	".\d_englsh.h"\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\g_game.h"\
	".\gl_utils.h"\
	".\gldefs.h"\
	".\hu_stuff.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_cheat.h"\
	".\m_fixed.h"\
	".\m_music.h"\
	".\p_inter.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_state.h"\
	".\s_sound.h"\
	".\sounds.h"\
	".\tables.h"\
	".\v_video.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\gl\Gl.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\d_console.obj" : $(SOURCE) $(DEP_CPP_D_CON) "$(INTDIR)"

"$(INTDIR)\d_console.sbr" : $(SOURCE) $(DEP_CPP_D_CON) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\i_cd.c
DEP_CPP_I_CD_=\
	".\i_cd.h"\
	

"$(INTDIR)\i_cd.obj" : $(SOURCE) $(DEP_CPP_I_CD_) "$(INTDIR)"

"$(INTDIR)\i_cd.sbr" : $(SOURCE) $(DEP_CPP_I_CD_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\m_music.c
DEP_CPP_M_MUS=\
	".\i_cd.h"\
	".\i_midi.h"\
	".\sys_win.h"\
	

"$(INTDIR)\m_music.obj" : $(SOURCE) $(DEP_CPP_M_MUS) "$(INTDIR)"

"$(INTDIR)\m_music.sbr" : $(SOURCE) $(DEP_CPP_M_MUS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\i_midi.c
DEP_CPP_I_MID=\
	".\i_midi.h"\
	

"$(INTDIR)\i_midi.obj" : $(SOURCE) $(DEP_CPP_I_MID) "$(INTDIR)"

"$(INTDIR)\i_midi.sbr" : $(SOURCE) $(DEP_CPP_I_MID) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dxerr.c
DEP_CPP_DXERR=\
	{$(INCLUDE)}"\dinput.h"\
	

"$(INTDIR)\dxerr.obj" : $(SOURCE) $(DEP_CPP_DXERR) "$(INTDIR)"

"$(INTDIR)\dxerr.sbr" : $(SOURCE) $(DEP_CPP_DXERR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\GLdoom.rc
DEP_RSC_GLDOO=\
	".\gldfont.bin"\
	".\INTERPIC.bin"\
	".\Playpal.bin"\
	

"$(INTDIR)\GLdoom.res" : $(SOURCE) $(DEP_RSC_GLDOO) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\GL_utils.c
DEP_CPP_GL_UT=\
	".\d_items.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomtype.h"\
	".\gldefs.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_state.h"\
	".\sys_win.h"\
	".\tables.h"\
	".\v_video.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\gl\Gl.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\GL_utils.obj" : $(SOURCE) $(DEP_CPP_GL_UT) "$(INTDIR)"

"$(INTDIR)\GL_utils.sbr" : $(SOURCE) $(DEP_CPP_GL_UT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\glsbar.c
DEP_CPP_GLSBA=\
	".\d_items.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomtype.h"\
	".\gl_utils.h"\
	".\gldefs.h"\
	".\info.h"\
	".\m_fixed.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_state.h"\
	".\tables.h"\
	".\v_video.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\gl\Gl.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\glsbar.obj" : $(SOURCE) $(DEP_CPP_GLSBA) "$(INTDIR)"

"$(INTDIR)\glsbar.sbr" : $(SOURCE) $(DEP_CPP_GLSBA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\gconsole.c
DEP_CPP_GCONS=\
	".\gldefs.h"\
	{$(INCLUDE)}"\gl\Gl.h"\
	{$(INCLUDE)}"\gl\Glu.h"\
	

"$(INTDIR)\gconsole.obj" : $(SOURCE) $(DEP_CPP_GCONS) "$(INTDIR)"

"$(INTDIR)\gconsole.sbr" : $(SOURCE) $(DEP_CPP_GCONS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\lights.c
DEP_CPP_LIGHT=\
	".\sys_win.h"\
	

"$(INTDIR)\lights.obj" : $(SOURCE) $(DEP_CPP_LIGHT) "$(INTDIR)"

"$(INTDIR)\lights.sbr" : $(SOURCE) $(DEP_CPP_LIGHT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\gl_video.c
DEP_CPP_GL_VI=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_main.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\gconsole.h"\
	".\gl_video.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_argv.h"\
	".\m_fixed.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_state.h"\
	".\sys_win.h"\
	".\tables.h"\
	".\v_video.h"\
	".\w_wad.h"\
	".\win_inpt.h"\
	".\win_video.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\gl\Gl.h"\
	{$(INCLUDE)}"\gl\Glu.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\gl_video.obj" : $(SOURCE) $(DEP_CPP_GL_VI) "$(INTDIR)"

"$(INTDIR)\gl_video.sbr" : $(SOURCE) $(DEP_CPP_GL_VI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\win_video.c
DEP_CPP_WIN_V=\
	".\sys_win.h"\
	".\win_video.h"\
	

"$(INTDIR)\win_video.obj" : $(SOURCE) $(DEP_CPP_WIN_V) "$(INTDIR)"

"$(INTDIR)\win_video.sbr" : $(SOURCE) $(DEP_CPP_WIN_V) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\win_mous.c
DEP_CPP_WIN_M=\
	".\d_event.h"\
	".\d_main.h"\
	".\doomtype.h"\
	".\dxerr.h"\
	".\sys_win.h"\
	{$(INCLUDE)}"\dinput.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\win_mous.obj" : $(SOURCE) $(DEP_CPP_WIN_M) "$(INTDIR)"

"$(INTDIR)\win_mous.sbr" : $(SOURCE) $(DEP_CPP_WIN_M) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\win_joy.c
DEP_CPP_WIN_J=\
	".\d_event.h"\
	".\d_main.h"\
	".\doomtype.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\win_joy.obj" : $(SOURCE) $(DEP_CPP_WIN_J) "$(INTDIR)"

"$(INTDIR)\win_joy.sbr" : $(SOURCE) $(DEP_CPP_WIN_J) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\win_kbrd.c
DEP_CPP_WIN_K=\
	".\d_event.h"\
	".\d_main.h"\
	".\doomtype.h"\
	".\dxerr.h"\
	".\sys_win.h"\
	{$(INCLUDE)}"\dinput.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\win_kbrd.obj" : $(SOURCE) $(DEP_CPP_WIN_K) "$(INTDIR)"

"$(INTDIR)\win_kbrd.sbr" : $(SOURCE) $(DEP_CPP_WIN_K) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\win_inpt.c
DEP_CPP_WIN_I=\
	".\gconsole.h"\
	".\sys_win.h"\
	".\win_joy.h"\
	".\win_kbrd.h"\
	".\win_mous.h"\
	

"$(INTDIR)\win_inpt.obj" : $(SOURCE) $(DEP_CPP_WIN_I) "$(INTDIR)"

"$(INTDIR)\win_inpt.sbr" : $(SOURCE) $(DEP_CPP_WIN_I) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\newlumps.c

"$(INTDIR)\newlumps.obj" : $(SOURCE) "$(INTDIR)"

"$(INTDIR)\newlumps.sbr" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\savepic.c

"$(INTDIR)\savepic.obj" : $(SOURCE) "$(INTDIR)"

"$(INTDIR)\savepic.sbr" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\sys_win.c
DEP_CPP_SYS_W=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_main.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\dxerr.h"\
	".\g_game.h"\
	".\gconsole.h"\
	".\gl_video.h"\
	".\gldefs.h"\
	".\i_cd.h"\
	".\i_midi.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_argv.h"\
	".\m_fixed.h"\
	".\m_music.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\r_data.h"\
	".\r_defs.h"\
	".\r_state.h"\
	".\st_lib.h"\
	".\sys_win.h"\
	".\tables.h"\
	".\v_video.h"\
	".\win_inpt.h"\
	".\win_video.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\dinput.h"\
	{$(INCLUDE)}"\gl\Gl.h"\
	{$(INCLUDE)}"\gl\Glaux.h"\
	{$(INCLUDE)}"\gl\Glu.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\sys_win.obj" : $(SOURCE) $(DEP_CPP_SYS_W) "$(INTDIR)"

"$(INTDIR)\sys_win.sbr" : $(SOURCE) $(DEP_CPP_SYS_W) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\snd_dx.c
DEP_CPP_SND_D=\
	".\d_event.h"\
	".\d_items.h"\
	".\d_net.h"\
	".\d_player.h"\
	".\d_think.h"\
	".\d_ticcmd.h"\
	".\doomdata.h"\
	".\doomdef.h"\
	".\doomstat.h"\
	".\doomtype.h"\
	".\gconsole.h"\
	".\i_sound.h"\
	".\i_system.h"\
	".\info.h"\
	".\m_argv.h"\
	".\m_fixed.h"\
	".\m_misc.h"\
	".\m_music.h"\
	".\p_mobj.h"\
	".\p_pspr.h"\
	".\sounds.h"\
	".\sys_win.h"\
	".\tables.h"\
	".\w_wad.h"\
	".\z_zone.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	{$(INCLUDE)}"\values.h"\
	

"$(INTDIR)\snd_dx.obj" : $(SOURCE) $(DEP_CPP_SND_D) "$(INTDIR)"

"$(INTDIR)\snd_dx.sbr" : $(SOURCE) $(DEP_CPP_SND_D) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
