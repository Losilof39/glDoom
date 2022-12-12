#ifndef MIDIDEF
   #define MIDIDEF 1

#include "doomtype.h"

typedef enum { midi_nofile, midi_stop, midi_play, midi_pause } midi_state;

typedef struct
   {
    DWORD    MidiPosition;
    DWORD    MidiDevice;
    DWORD    MidiStatus;
    //MCIERROR MidiError;
    char     szMidiFile[128];
   }MIDI_Data_t;

dboolean  MidiPlay(  MIDI_Data_t *mdd);
dboolean  MidiReplay(MIDI_Data_t *mdd);
dboolean  MidiPause( MIDI_Data_t *mdd);
dboolean  MidiResume(MIDI_Data_t *mdd);
void  MidiStop(MIDI_Data_t *mdd);

#endif

#ifndef QMUS2MID_H
#define QMUS2MID_H

#define NOTMUSFILE      1       /* Not a MUS file */
#define COMUSFILE       2       /* Can't open MUS file */
#define COTMPFILE       3       /* Can't open TMP file */
#define CWMIDFILE       4       /* Can't write MID file */
#define MUSFILECOR      5       /* MUS file corrupted */
#define TOOMCHAN        6       /* Too many channels */
#define MEMALLOC        7       /* Memory allocation error */

/* some (old) compilers mistake the "MUS\x1A" construct (interpreting
   it as "MUSx1A")      */

#define MUSMAGIC     "MUS\032"                    /* this seems to work */
#define MIDIMAGIC    "MThd\000\000\000\006\000\001"
#define TRACKMAGIC1  "\000\377\003\035"
#define TRACKMAGIC2  "\000\377\057\000"
#define TRACKMAGIC3  "\000\377\002\026"
#define TRACKMAGIC4  "\000\377\131\002\000\000"
#define TRACKMAGIC5  "\000\377\121\003\011\243\032"
#define TRACKMAGIC6  "\000\377\057\000"


typedef struct
   {
    char            ID[4];            /* identifier "MUS" 0x1A */
    unsigned short  ScoreLength;
    unsigned short  ScoreStart;
    unsigned short  channels;         /* count of primary channels */
    unsigned short  SecChannels;      /* count of secondary channels (?) */
    unsigned short  InstrCnt;
    unsigned short  dummy;
    /* variable-length part starts here */
    unsigned short *instruments;
   }MUSheader;

struct Track
   {
    unsigned int   current;
    unsigned char   vel;
    int            DeltaTime;
    unsigned char   LastEvent;
    char           *data;            /* Primary data */
   };


int qmus2mid( const char *mid, int nodisplay, unsigned short division, int BufferSize, int nocomp );

#endif


