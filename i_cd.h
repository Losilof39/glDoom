/*#ifndef CDDEF
   #define CDDEF 1

#include "doomtype.h"

// Global define needed for calling these functions

typedef enum { cd_stop, cd_play, cd_pause, cd_empty, cd_unavail } cdstate;

typedef struct
   {
    dboolean CDMedia;
    char     CDCode[18];
    DWORD    CDPosition;
    DWORD    CDDevice;
    DWORD    CDTrack;
    DWORD    CDTrackCount;
    DWORD    CDStatus;
    DWORD    CDTrackList[32];
    //MCIERROR CDError;
   }CD_Data_t;

dboolean  CDOpen( CD_Data_t *cdd);
dboolean  CDCheck(CD_Data_t *cdd);
dboolean  CDMediaIdentity(CD_Data_t *cdd, char *szIdent);
dboolean  CDTrackCount(CD_Data_t *cdd);
dboolean  CDTrackType(CD_Data_t *cdd);
dboolean  CDTrackPlay(CD_Data_t *cdd);
dboolean  CDPause(    CD_Data_t *cdd);
dboolean  CDResume(   CD_Data_t *cdd);
void  CDStop(CD_Data_t *cdd);
void  CDClose(CD_Data_t *cdd);

#endif

*/