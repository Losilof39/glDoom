//#include <windows.h>

// Console functions
void con_clear(void);
//void con_printf( char* message, ... );
void con_setup(int width, int height);
void con_shutdown(void);
void con_SetupOpenGL(void);

#define con_printf(...) printf(__VA_ARGS__)