cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(glDoom VERSION 1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} CACHE INTERNAL "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(SOURCES
am_map.c
co_console.c
co_utils.c
d_bitmap.c
d_items.c
d_main.c
d_net.c
doomdef.c
doomlib.c
doomstat.c
dstrings.c
f_finale.c
f_wipe.c
g_game.c
gl_texture.c
gl_wipe.c
glsbar.c
hu_lib.c
hu_stuff.c
i_net.c
i_sdlmusic.c
i_sdlsound.c
i_sound.c
i_system.c
info.c
m_argv.c
m_bbox.c
m_cheat.c
m_fixed.c
m_menu.c
m_misc.c
m_random.c
m_swap.c
mathlib.c
mmus2mid.c
p_ceilng.c
p_doors.c
p_enemy.c
p_floor.c
p_inter.c
p_lights.c
p_map.c
p_maputl.c
p_mobj.c
p_plats.c
p_pspr.c
p_saveg.c
p_setup.c
p_sight.c
p_spec.c
p_switch.c
p_telept.c
p_tick.c
p_user.c
r_bsp.c
r_data.c
r_draw.c
r_main.c
r_plane.c
r_segs.c
r_shader.c
r_things.c
renderer.c
rendererCore.c
rendererFixed.c
s_sound.c
savepic.c
sdl_input.c
sdl_kbrd.c
sdl_video.c
sounds.c
st_lib.c
st_stuff.c
sys_sdl.c
tables.c
v_video.c
w_wad.c
wi_stuff.c
z_zone.c
thirdparty/glad/src/glad.c)

add_executable(glDoom ${SOURCES})

# Link only prebuilt versions of libraries to avoid unnecessary rebuilds
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(CGLM_LIB thirdparty/cglm/lib/x64/cglm)
	set(SDL_LIB thirdparty/SDL2/lib/x64/SDL2)
	set(SDL_MIXER_LIB thirdparty/SDL2_mixer)
else()
	set(CGLM_LIB thirdparty/cglm/lib/x86/cglm)
	set(SDL_LIB thirdparty/SDL2/lib/x86/SDL2)
	set(SDL_MIXER_LIB thirdparty/SDL2/lib/x86/SDL2_mixer)
endif()

target_include_directories(glDoom PRIVATE src/engine)
target_include_directories(glDoom PRIVATE thirdparty/glad/include)
target_include_directories(glDoom PRIVATE thirdparty/SDL2/include)
target_include_directories(glDoom PRIVATE thirdparty/cglm/include/cglm)
target_include_directories(glDoom PRIVATE thirdparty/cglm/include)
target_include_directories(glDoom PRIVATE thirdparty/freetype/include)

target_link_libraries(glDoom PRIVATE ${CGLM_LIB} ${SDL_LIB} ${SDL_MIXER_LIB})

add_custom_command(TARGET glDoom POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SDL_DLL}
        ${SDL_MIXER_DLL}
        ${CGLM_DLL}
        $<TARGET_FILE_DIR:glDoom>
)