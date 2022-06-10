// win_video.h
// Function and type declarations for win_video.c

#define OSR2_BUILD_NUMBER 1111

typedef struct
   {
    DWORD dwCurrWidth;
    DWORD dwCurrHeight;
    DWORD dwCurrBPP;
   }video_info_t;

typedef struct
   {
    int  width;
    int  height;
    int  bpp;
   }devinfo_t;

void GetVideoInfo(void);
void GetModeList(char *dbgname);
BOOL SetVideoMode();
void ResetVideoMode(void);

