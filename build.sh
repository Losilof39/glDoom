gcc -g -o gldoom *.c thirdparty/glad/src/glad.c -I./thirdparty/glad/include -I./thirdparty/SDL2/include -lOpenGL -lSDL2 -lSDL2_mixer -lm -lGL -lGLU -lglut
