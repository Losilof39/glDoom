#if 0
// Global define needed for calling these functions
//#include <windows.h>
#include <stdio.h>
//#include <mmsystem.h>
#include <fcntl.h>
#ifdef _WIN32
#include <io.h>
#else
#include <inttypes.h>
#include <unistd.h>
#endif

#include "i_midi.h"

void lfprintf(char *message, ... );

// Local define needed only for error message capture and display
#define     MCIMSGLEN  128
char        MidiMsg[MCIMSGLEN+1];
void lfprintf(char *message, ... );

dboolean MidiPlay(MIDI_Data_t *mdd)
   {
    //MCI_OPEN_PARMS     open;
    //MCI_PLAY_PARMS     play;
    //MCIERROR           MCI_Error;
    //UINT               uDeviceId;

    //if (_access(mdd->szMidiFile, 4) != 0)
    //   {
    //    mdd->MidiStatus = midi_nofile;
    //    return(false);
    //   }

    //open.dwCallback       = (DWORD)hWnd;
    //open.lpstrDeviceType  = "sequencer";
    //open.lpstrElementName = mdd->szMidiFile;

    //MCI_Error = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)&open);

    //if (MCI_Error)
    //   {
    //    mciGetErrorString(MCI_Error, MidiMsg, MCIMSGLEN);
    //    MessageBox(hWnd, MidiMsg, NULL, MB_ICONSTOP | MB_OK);
    //    strcat(MidiMsg, "\n");
    //    lfprintf(MidiMsg);
    //    mdd->MidiStatus = midi_nofile;
    //    mdd->MidiError = MCI_Error;
    //    return(false);
    //   }

    //// play back midi file
    ////......................

    //uDeviceId = open.wDeviceID;
    //play.dwCallback = (DWORD)hWnd;
    //                       
    //MCI_Error = mciSendCommand(uDeviceId, MCI_PLAY, MCI_NOTIFY, (DWORD)&play);
    //if (MCI_Error)
    //   {
    //    mciGetErrorString(MCI_Error, MidiMsg, MCIMSGLEN);
    //    MessageBox(hWnd, MidiMsg, NULL, MB_ICONSTOP | MB_OK);
    //    mciSendCommand(uDeviceId, MCI_CLOSE, 0, (DWORD)NULL);
    //    mdd->MidiError = MCI_Error;
    //    return(false);
    //   }
    //mdd->MidiDevice = uDeviceId;
    //mdd->MidiStatus = midi_play;
    //return(true);
   }

dboolean MidiReplay(MIDI_Data_t *mdd)
   {
    /*MCI_PLAY_PARMS     play;
    MCIERROR           MCI_Error;

    play.dwCallback = (DWORD)hWnd;
    play.dwFrom = 0;
                           
    MCI_Error = mciSendCommand(mdd->MidiDevice, MCI_PLAY, MCI_NOTIFY | MCI_FROM, (DWORD)&play);
    if (MCI_Error)
       {
        mciGetErrorString(MCI_Error, MidiMsg, MCIMSGLEN);
        MessageBox(hWnd, MidiMsg, NULL, MB_ICONSTOP | MB_OK);
        return(false);
       }
    return(true);*/
   }

dboolean MidiPause(MIDI_Data_t *mdd)
   {
    /*MCI_SET_PARMS      set;
    MCI_STATUS_PARMS   status;
    MCIERROR           MCI_Error;

    mdd->MidiError = 0;
    set.dwTimeFormat   = MCI_FORMAT_MILLISECONDS;
    mciSendCommand(mdd->MidiDevice, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID)&set);

    status.dwCallback  = (DWORD)hWnd;
    status.dwItem = MCI_STATUS_POSITION;

    MCI_Error = mciSendCommand(mdd->MidiDevice, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)&status);
    if (MCI_Error)
       {
        mciGetErrorString(MCI_Error, MidiMsg, MCIMSGLEN);
        strcat(MidiMsg, "\nMidiPause");
        MessageBox(hWnd, MidiMsg, NULL, MB_ICONSTOP | MB_OK);
        mdd->MidiError = MCI_Error;
        return(false);
       }
    mdd->MidiPosition = status.dwReturn;
    mciSendCommand(mdd->MidiDevice, MCI_STOP, 0, (DWORD)NULL);
    mdd->MidiStatus = midi_pause;
    return(true);*/
   }

dboolean MidiResume(MIDI_Data_t *mdd)
   {
    /*MCI_SET_PARMS      set;
    MCI_PLAY_PARMS     play;
    MCIERROR           MCI_Error;

    mdd->MidiError = 0;
    set.dwTimeFormat   = MCI_FORMAT_MILLISECONDS;
    mciSendCommand(mdd->MidiDevice, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD)(LPVOID) &set);

    play.dwCallback = (DWORD)hWnd;
    play.dwTo = 0;
    play.dwFrom = mdd->MidiPosition;
    MCI_Error = mciSendCommand(mdd->MidiDevice, MCI_PLAY, MCI_NOTIFY | MCI_FROM, (DWORD)(LPVOID)&play);
    if (MCI_Error)
       {
        mciGetErrorString(MCI_Error, MidiMsg, MCIMSGLEN);
        strcat(MidiMsg, "\nMidiResume");
        MessageBox(hWnd, MidiMsg, NULL, MB_ICONSTOP | MB_OK);
        mdd->MidiError = MCI_Error;
        return(false);
       }
    mdd->MidiStatus = midi_play;
    return(true);*/
   }

void MidiStop(MIDI_Data_t *mdd)
   {
    /*mciSendCommand(mdd->MidiDevice, MCI_STOP, 0, (DWORD)NULL);
    mciSendCommand(mdd->MidiDevice, MCI_CLOSE, 0, (DWORD)NULL);
    mdd->MidiStatus = midi_stop;*/
   }


void MidiVolumeUp(MIDI_Data_t *mdd, DWORD dwVolume)
{
	/*MMRESULT mmResult;
	HMIDIOUT hmo;

	mmResult = midiOutSetVolume(mdd->MidiDevice, dwVolume );*/
}

void MidiVolumeDown()
{
}

/*
   Quick MUS to Midi converter.
   (C) 1995,96 Sebastien Bacquet  ( bacquet@iie.cnam.fr )
*/
// Now part of i_midi.h
//
//#include "musmid.h"

#define __LITTLE_ENDIAN__

// Not needed with big endian.
#ifdef __LITTLE_ENDIAN__

/*
// Swap 16bit, that is, MSB and LSB byte.
unsigned short SwapSHORT(unsigned short x)
{
    // No masking with 0xFF should be necessary. 
    return (x>>8) | (x<<8);
}

// Swapping 32bit.
unsigned long long SwapLONG( unsigned long long x)
{
    return
	(x>>24)
	| ((x>>8) & 0xff00)
	| ((x<<8) & 0xff0000)
	| (x<<24);
}
*/
short	SwapSHORT(short);
long long	SwapLONG(long long);

// Endianess handling.
// WAD files are stored little endian.

#define SHORT(x)	((short)SwapSHORT((unsigned short) (x)))
#define LONG(x)         ((long long)SwapLONG((unsigned long long) (x)))

#else

#define SHORT(x)	(x)
#define LONG(x)         (x)

#endif

void Close_Midi(void);

unsigned char  musname[10] = "DOOMSONG";
unsigned char *musdata;
int            musptr;

int            bufflen;

int file_mid = -1;

unsigned int TRACKBUFFERSIZE = 65536L;  /* 64 Kb */

void mread(void *d, int size)
   {
    /*memcpy(d, &musdata[musptr], size);
    musptr = musptr + size;*/
   }

void FreeTracks( struct Track track[] )
   {
    /*int i;

    for ( i = 0; i < 16; i++ )
       {
        if ( track[i].data )
           {
            free( track[i].data );
           }
       }*/
   }

void TWriteByte( char MIDItrack, unsigned char byte, struct Track track[] )
   {
    /*unsigned int pos;

    pos = track[MIDItrack].current;
    if ( pos < TRACKBUFFERSIZE )
       {
        track[MIDItrack].data[pos] = byte;
       }
    else
       {
        lfprintf("ERROR: Track buffer full.\n"
               "Increase the track buffer size (option -size).\n" );
        FreeTracks( track );
        Close();
        exit( EXIT_FAILURE );
       }
    track[MIDItrack].current++;*/
   }


void Close_Midi()
   {
//	lfprintf("Closing files...\n");
    /*if (file_mid != -1)
       {
        close(file_mid);
        file_mid = -1;
       }*/
   }

void TWriteVarLen( int tracknum, register unsigned int value, struct Track track[] )
   {
    /*register unsigned int buffer;

    buffer = value & 0x7f;
    while ( (value >>= 7) )
       {
        buffer <<= 8;
        buffer |= 0x80;
        buffer += (value & 0x7f);
       }
    while ( 1 )
       {
        TWriteByte( (char)tracknum, (unsigned char)buffer, track );
         if ( buffer & 0x80 )
            {
             buffer >>= 8;
            }
         else
            {
             break;
            }
       }*/
   }


int ReadMUSheader( MUSheader *MUSh )
   {
    //mread(MUSh->ID, (sizeof(MUSheader)-2));
    //if ( strncmp( MUSh->ID, MUSMAGIC, 4 ) )
    //   {
    //    return NOTMUSFILE;
    //   }
    //MUSh->instruments = (unsigned short *)calloc(MUSh->InstrCnt, sizeof(unsigned short)*MUSh->InstrCnt);
    //mread(MUSh->instruments, (2*MUSh->InstrCnt));
    //free( MUSh->instruments ); /* suppress this line if you want to display instruments later */
    //return 0;
   }


int WriteMIDheader( int file, unsigned short ntrks, unsigned short division )
   {
    /*unsigned short  tvalue;

    _write( file, MIDIMAGIC , 10 );
    tvalue = SHORT(ntrks);
    _write( file, &tvalue, 2);
    tvalue = SHORT(division);
    _write( file, &tvalue, 2 );

    return 0;*/
   }

        /* maybe for ms-dog too ? */ /* Yes, why not ?... */
#define last(e)         ((unsigned char)(e & 0x80))
#define event_type(e)   ((unsigned char)((e & 0x7F) >> 4))
#define channel(e)      ((unsigned char)(e & 0x0F))

void TWriteString( char tracknum, const char *string, int length, struct Track track[] )
   {
    /*register int i;

    for ( i = 0; i < length; i++ )
       {
        TWriteByte( tracknum, string[i], track );
       }*/
   }


void WriteTrack( int tracknum, int file, struct Track track[] )
   {
    //unsigned int  size, tsize;
    //size_t        quot, rem;

    ///* Do we risk overflow here ? */
    //size = (unsigned int)track[tracknum].current+4;
    //if (size > 32767)
    //   {
    //    lfprintf("ERROR: track size overflow...\n");
    //   }
    //_write( file, "MTrk", 4 );
    //if ( !tracknum )
    //   {
    //    size += 33;
    //   }

    //tsize = LONG(size);
    //_write(file, &tsize, 4);
    //if ( !tracknum)
    //   {
    //    _write( file, TRACKMAGIC1 "Quick MUS->MID ! by S.Bacquet", 33 );
    //   }
    //quot = (size_t) (track[tracknum].current / 4096);
    //rem = (size_t) (track[tracknum].current - quot*4096);
    //_write( file, track[tracknum].data, (4096*quot) );
    //_write( file, ((const unsigned char *)(track[tracknum].data)+4096*quot), rem );
    //_write( file, TRACKMAGIC2, 4 );
   }


void WriteFirstTrack( int file )
   {
    /*unsigned int           size;

    _write( file, "MTrk", 4 );
    size = LONG(43);
    _write( file, &size, 4 );
    _write( file, TRACKMAGIC3 , 4 );
    _write( file, "QMUS2MID (C) S.Bacquet", 22 );
    _write( file, TRACKMAGIC4, 6 );
    _write( file, TRACKMAGIC5, 7 );
    _write( file, TRACKMAGIC6, 4 );*/
   }

int ReadTime()
   {
    /*int           time;
    unsigned char timebyte;

    time = 0;
    do
      {
       mread(&timebyte, 1);
       if ( musptr < bufflen )
          {
           time = (time << 7) + (timebyte & 0x7F);
          }
      }while ( ( musptr < bufflen ) && (timebyte & 0x80) );

    return time;*/
   }

char FirstChannelAvailable( signed char MUS2MIDchannel[] )
   {
    /*int i;
    signed char old15 = MUS2MIDchannel[15], max = -1;

    MUS2MIDchannel[15] = -1;
    for ( i = 0; i < 16; i++ )
       {
        if ( MUS2MIDchannel[i] > max )
           {
            max = MUS2MIDchannel[i];
           }
       }
    MUS2MIDchannel[15] = old15;

    return (max == 8 ? 10: max+1);*/
   }


int qmus2mid( const char *mid, int nodisplay, unsigned short division, int BufferSize, int nocomp )
   {
//    struct           Track  track[16];
//    unsigned short   TrackCnt = 0;
//    unsigned char    et, MUSchannel, MIDIchannel, MIDItrack, NewEvent, data, event;
//    int              i, r;
//    static MUSheader MUSh;
//    unsigned int     DeltaTime, TotalTime = 0, time, min, n = 0;
//    unsigned char    MUS2MIDcontrol[15] = {
//       0,                          /* Program change - not a MIDI control change */
//       0x00,                       /* Bank select */
//       0x01,                       /* Modulation pot */
//       0x07,                       /* Volume */
//       0x0A,                       /* Pan pot */
//       0x0B,                       /* Expression pot */
//       0x5B,                       /* Reverb depth */
//       0x5D,                       /* Chorus depth */
//       0x40,                       /* Sustain pedal */
//       0x43,                       /* Soft pedal */
//       0x78,                       /* All sounds off */
//       0x7B,                       /* All notes off */
//       0x7E,                       /* Mono */
//       0x7F,                       /* Poly */
//       0x79                        /* Reset all controllers */
//      }, MIDIchan2track[16];
//    signed char MUS2MIDchannel[16];
//    char ouch = 0, sec;
//
//    if ( (file_mid = _open( mid, O_CREAT | O_RDWR | O_BINARY | O_TRUNC, 0666 )) == -1 )
//       {
//        return CWMIDFILE;
//       }
//
//    r = ReadMUSheader( &MUSh );
//    if ( r )
//       {
//        Close();
//        return r;
//       }
//    musptr = MUSh.ScoreStart;
////    lfprintf("Score starts at offset %d\n", MUSh.ScoreStart);
//    if ((musptr > bufflen) || (musptr < 0))
//       {
//        Close();
//        return MUSFILECOR;
//       }
//    if ( !nodisplay )
//       {
//        lfprintf( "%s (%ld bytes) contains %d melodic channel%s.\n", musname,
//               (unsigned int) bufflen, MUSh.channels,
//               MUSh.channels >= 2 ? "s": "" );
//       }
//    if ( MUSh.channels > 15 )      /* <=> MUSchannels+drums > 16 */
//       {
//        Close();
//        return TOOMCHAN;
//       }
//
//    for ( i = 0; i < 16; i++ )
//       {
//        MUS2MIDchannel[i] = -1;
//        track[i].current = 0;
//        track[i].vel = 64;
//        track[i].DeltaTime = 0;
//        track[i].LastEvent = 0;
//        track[i].data = NULL;
//       }
//    if ( BufferSize )
//       {
//        TRACKBUFFERSIZE = ((unsigned int)BufferSize) << 10;
//        if ( !nodisplay )
//           {
//            lfprintf( "Track buffer size set to %d KB.\n", BufferSize );
//           }
//       }
//
//    if ( !nodisplay )
//       {
//        lfprintf( "Converting..." );
//       }
//    mread(&event, 1);
//    et = event_type( event );
//    MUSchannel = channel( event );
////    lfprintf("\nStart MUS channel %d\n", MUSchannel);
//    while ( (et != 6) && (musptr < bufflen) )
//       {
//        //lfprintf("MUSchannel = %d\n", MUSchannel);
//        if ( MUS2MIDchannel[MUSchannel] == -1 )
//           {
//            MIDIchannel = MUS2MIDchannel[MUSchannel ] =
//               (MUSchannel == 15 ? 9: FirstChannelAvailable( MUS2MIDchannel));
//            if ((MIDIchannel < 0) || (MIDIchannel > 15))
//               {
//                lfprintf("ERROR: Invalid MIDI channel value %d\n", MIDIchannel);
//                FreeTracks( track );
//                Close();
//                return TOOMCHAN;
//               }
//            MIDItrack   = MIDIchan2track[MIDIchannel] = (unsigned char) (TrackCnt++);
//            if ((MIDItrack < 0) || (MIDItrack > 15))
//               {
//                lfprintf("ERROR: Invalid MIDI track value %d\n", MIDItrack);
//                FreeTracks( track );
//                Close();
//                return TOOMCHAN;
//               }
//            if ( !(track[MIDItrack].data = (char *) malloc( TRACKBUFFERSIZE )) )
//               {
//                FreeTracks( track );
//                Close();
//                return MEMALLOC;
//               }
//           }
//        else
//           {
//            MIDIchannel = MUS2MIDchannel[MUSchannel];
//            MIDItrack   = MIDIchan2track [MIDIchannel];
//           }
//        //lfprintf("MIDI channel %d\n", MIDIchannel);
//        //lfprintf("MIDI track %d\n", MIDItrack);
//        TWriteVarLen( MIDItrack, track[MIDItrack].DeltaTime, track );
//        track[MIDItrack].DeltaTime = 0;
//        switch( et )
//           {
//            case 0:                /* release note */
//                 NewEvent = 0x90 | MIDIchannel;
//                 if ( (NewEvent != track[MIDItrack].LastEvent) || nocomp )
//                    {
//                     TWriteByte( MIDItrack, NewEvent, track );
//                     track[MIDItrack].LastEvent = NewEvent;
//                    }
//                 else
//                    {
//                     n++;
//                    }
//                 mread(&data, 1);
//                 TWriteByte( MIDItrack, data, track );
//                 TWriteByte( MIDItrack, 0, track );
//                 //lfprintf("Release note...%02X\n", data);
//                 break;
//            case 1:
//                 NewEvent = 0x90 | MIDIchannel;
//                 if ( (NewEvent != track[MIDItrack].LastEvent) || nocomp )
//                    {
//                     TWriteByte( MIDItrack, NewEvent, track );
//                     track[MIDItrack].LastEvent = NewEvent;
//                    }
//                 else
//                    {
//                     n++;
//                    }
//                 mread(&data, 1);
//                 TWriteByte( MIDItrack, (unsigned char)(data & 0x7F), track );
//                 if ( data & 0x80 )
//                    {
//                     mread(&track[MIDItrack].vel, 1);
//                    }
//                 TWriteByte( MIDItrack, track[MIDItrack].vel, track );
//                 //lfprintf("Play note...%02X vel %02X\n", data & 0x7F, track[MIDItrack].vel);
//                 break;
//            case 2:
//                 NewEvent = 0xE0 | MIDIchannel;
//                 if ( (NewEvent != track[MIDItrack].LastEvent) || nocomp )
//                    {
//                     TWriteByte( MIDItrack, NewEvent, track );
//                     track[MIDItrack].LastEvent = NewEvent;
//                    }
//                 else
//                    {
//                     n++;
//                    }
//                 mread(&data, 1);
//                 TWriteByte( MIDItrack, (unsigned char)((data & 1) << 6), track );
//                 TWriteByte( MIDItrack, (unsigned char)(data >> 1), track );
//                 //lfprintf("Pitch bend...%02X\n", (data & 1)<<6);
//                 break;
//            case 3:
//                 NewEvent = 0xB0 | MIDIchannel;
//                 if ( (NewEvent != track[MIDItrack].LastEvent) || nocomp )
//                    {
//                     TWriteByte( MIDItrack, NewEvent, track );
//                     track[MIDItrack].LastEvent = NewEvent;
//                    }
//                 else
//                    {
//                     n++;
//                    }
//                 mread(&data, 1);
//                 TWriteByte( MIDItrack, MUS2MIDcontrol[data], track );
//                 if ( data == 12 )
//                    {
//                     TWriteByte( MIDItrack, (unsigned char)(MUSh.channels+1), track );
//                    }
//                 else
//                    {
//                     TWriteByte( MIDItrack, (unsigned char)0, track );
//                    }
//                 //lfprintf("MIDI Control...%02X\n", data);
//                 break;
//            case 4:
//                 mread(&data, 1);
//                 if ( data )
//                    {
//                     NewEvent = 0xB0 | MIDIchannel;
//                     if ( (NewEvent != track[MIDItrack].LastEvent) || nocomp )
//                        {
//                         TWriteByte( MIDItrack, NewEvent, track );
//                         track[MIDItrack].LastEvent = NewEvent;
//                        }
//                     else
//                        {
//                         n++;
//                        }
//                     TWriteByte( MIDItrack, MUS2MIDcontrol[data], track );
//                    }
//                 else
//                    {
//                     NewEvent = 0xC0 | MIDIchannel;
//                     if ( (NewEvent != track[MIDItrack].LastEvent) || nocomp )
//                        {
//                         TWriteByte( MIDItrack, NewEvent, track );
//                         track[MIDItrack].LastEvent = NewEvent;
//                        }
//                     else
//                        {
//                         n++;
//                        }
//                    }
//                 mread(&data, 1);
//                 TWriteByte( MIDItrack, data, track );
//                 //lfprintf("Controller change...%02X\n", data);
//                 break;
////            case 6:
////                 lfprintf("End of tune detected.\n");
////                 break;
//            case 5:
//            case 7:
//                FreeTracks( track );
//                Close();
//                return MUSFILECOR;
//            default:
//                break;
//           }
//        if ( last( event ) )
//	       {
//            DeltaTime = ReadTime();
//            TotalTime += DeltaTime;
//	        for ( i = 0; i < (int) TrackCnt; i++ )
//               {
//	            track[i].DeltaTime += DeltaTime;
//               }
//           }
//        mread(&event, 1);
//        if ( musptr <= bufflen )
//           {
//            et = event_type( event );
//            MUSchannel = channel( event );
//           }
//        else
//           {
//            ouch = 1;
//           }
//       }
//    if ( !nodisplay )
//       {
//        lfprintf( "done !\n" );
//       }
//    if ( ouch )
//       {
//        lfprintf("WARNING: There are bytes missing at the end of %s.\n          "
//               "The end of the MIDI file might not fit the original one.\n", musname );
//       }
//    if ( !division )
//       {
//        division = 89;
//       }
//    else
//       {
//        if ( !nodisplay )
//           {
//            lfprintf( "Ticks per quarter note set to %d.\n", division );
//           }
//       }
//    if ( !nodisplay )
//       {
//        if ( division != 89 )
//           {
//            time = TotalTime / 140;
//            min = time / 60;
//            sec = (char) (time - min*60);
//            lfprintf( "Playing time of the MUS file: %u'%.2u''.\n", min, sec );
//           }
//        time = (TotalTime * 89) / (140 * division);
//        min = time / 60;
//        sec = (char) (time - min*60);
//        if ( division != 89 )
//           {
//            lfprintf( "                    MID file" );
//           }
//        else
//           {
//            lfprintf( "Playing time" );
//           }
//        lfprintf( ": %u'%.2u''.\n", min, sec );
//       }
//    if ( !nodisplay )
//       {
//        lfprintf( "Writing..." );
//       }
//    WriteMIDheader( file_mid, (unsigned short)(TrackCnt+1), division );
//    WriteFirstTrack( file_mid );
//    for( i = 0; i < (int) TrackCnt; i++ )
//       {
//        WriteTrack( i, file_mid, track );
//       }
//    if ( !nodisplay )
//       {
//        lfprintf( "done !\n" );
//       }
//    if ( !nodisplay && !nocomp )
//       {
//        lfprintf( "Compression: %u%%.\n",(100 * n) / (n+ (unsigned int) _tell( file_mid )) );
//       }
//
//    FreeTracks( track );
//    Close();
//
//    return 0;
   }


int MidiConvert( const char *mid, int nodisplay, int div, int size, int nocomp, dboolean overwrite, void *MusBuff, int bufflength )
   {
    //int         file;
    //int         error;
    //static char buffer[512];

    ////strncpy(musname, directory[curlump].name, 8);
    ////musname[8] = '\0';

    //if ((!access( mid, 0 ) ) && (overwrite == false))
    //   {
    //    lfprintf( "ERROR: %s already exists.\n", mid );
    //    return 1;
    //   }

    //musdata = MusBuff;
    //musptr  = 0;

    //bufflen = bufflength;

    //error = qmus2mid( mid, nodisplay, (unsigned short)div, size, nocomp );

    //if ( error )
    //   {
    //    lfprintf( "ERROR: " );
    //    switch( error )
    //       {
    //        case NOTMUSFILE:
    //             lfprintf( "%s is not a MUS file.\n", musname );
    //             break;
    //        case COMUSFILE:
    //             lfprintf( "Can't open %s for read.\n", musname );
    //             break;
    //        case COTMPFILE:
    //             lfprintf( "Can't open temp file.\n" );
    //             break ;
    //        case CWMIDFILE:
    //             lfprintf( "Can't write %s (?).\n", mid );
    //             break;
    //        case MUSFILECOR:
    //             lfprintf( "%s is corrupted.\n", musname );
    //             break;
    //        case TOOMCHAN:
    //             lfprintf( "%s contains more than 16 channels.\n", musname );
    //             break;
    //        case MEMALLOC:
    //             lfprintf( "Not enough memory.\n" );
    //             break;
    //        default:
    //             break;
    //       }
    //    return 4;
    //   }

    //if ( !nodisplay )
    //   {
    //    lfprintf( "%s converted successfully.\n", musname );
    //    if ( (file = _open( mid, O_RDWR | O_BINARY )) != -1 )
    //       {
    //        sprintf( buffer, ": %ld bytes", (unsigned int) _filelength(file) );
    //        _close( file );
    //       }
    //    lfprintf( "%s (%scompressed) written%s.\n", mid, nocomp ? "NOT ": "",file ? buffer: ""  );
    //   }

    //return 0;
   }

/* 08 Mar, 1999 - Midi stream like Ramdu Heit's Z-Doom

typedef struct midihdr_tag {
  LPSTR              lpData;
  DWORD              dwBufferLength;
  DWORD              dwBytesRecorded;
  DWORD_PTR          dwUser;
  DWORD              dwFlags;
  struct midihdr_tag *lpNext;
  DWORD_PTR          reserved;
  DWORD              dwOffset;
  DWORD_PTR          dwReserved[4];
} MIDIHDR, *LPMIDIHDR;

typedef struct {
  DWORD dwDeltaTime;
  DWORD dwStreamID;
  DWORD dwEvent;
  DWORD dwParms[];
} MIDIEVENT;

HMIDISTRM hMidiStream, *lphMidiStream;
UINT      DeviceID, *DeviceID;
DWORD     cMidi;
DWORD     dwCallback;
DWORD     fdwOpen;
MIDIHDR   MidiHdr;
UINT      cbMidiHdr;
BYTE      propdata;
DWORD     dwProperty;

lphMidiStream = &hMidiStream;
lpDeviceID = &DeviceID;

cbMidiHdr = sizeof(MidiHdr); // ? size right ?


midiStreamOpen  Opens a MIDI stream and retrieves a handle. 
MMRESULT midiStreamOpen( LPHMIDISTRM lphStream, LPUINT puDeviceID, DWORD cMidi, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen );

midiStreamClose  Closes a MIDI stream. 
MMRESULT midiStreamClose( HMIDISTRM hMidiStream );

midiStreamOut  Plays or queues a stream (buffer) of MIDI data to a MIDI output device.
MMRESULT midiStreamOut( HMIDISTRM hMidiStream, LPMIDIHDR lpMidiHdr, UINT cbMidiHdr );

midiStreamPause  Pauses playback of a specified MIDI stream.
MMRESULT midiStreamPause( HMIDISTRM hMidiStream );

midiStreamPosition  Retrieves the current position in a MIDI stream.
MMRESULT midiStreamPosition( HMIDISTRM hms, LPMMTIME pmmt, UINT cbmmt );

midiStreamProperty  Sets and retrieves stream properties. 
MMRESULT midiStreamProperty( HMIDISTRM hMidiStream, LPBYTE lppropdata, DWORD dwProperty );

midiStreamRestart  Restarts playback of a paused MIDI stream.
MMRESULT midiStreamRestart( HMIDISTRM hMidiStream );

midiStreamStop  Turns off all notes on all MIDI channels for the specified MIDI stream. 
MMRESULT midiStreamStop( HMIDISTRM hMidiStream );

//MSDN is my friend... :o)

	// midi event that plays music and specifies a callback event
    MIDIEVENT me;
    me.dwDeltaTime = 48;  // play 48 ticks after preceding event
    me.dwStreamID = 0;    // must be 0
    me.dwEvent = MEVT_F_SHORT | MEVT_F_CALLBACK | // flags
				(((DWORD)MEVT_SHORTMSG) << 24) |  // event code
				(DWORD) 0x90 |                    // MIDI note-on status byte
				(((DWORD) 0x3C) << 8) |           // middle C
				(((DWORD) 0x7F) << 16);           // maximum key velocity

	// callback only midi event -- plays no music				
    MIDIEVENT me;
    me.dwDeltaTime = 48;
    me.dwStreamID = 0;
    me.dwEvent = MEVT_F_SHORT | MEVT_F_CALLBACK | (((DWORD)MEVT_NOP) << 24) |
                 (DWORD) 0x90 |(((DWORD) 0x3C) << 8) | (((DWORD) 0x7F) << 16);

// This function acts as a midiStream callback and handles the callback events ;o)

void CALLBACK MidiStreamProc( 
	HMIDIOUT hmo,     // handle of MIDI output device
	UINT uMsg,        // MIDI message
	DWORD dwInstance, // callback instance data
	DWORD dwParam1,   // address of MIDI header
	DWORD dwParam2)   // unused parameter
{
	LPMIDIHDR lpMIDIHeader;
	MIDIEVENT *lpMIDIEvent;

	if (!dwParam1)
	{
        return;    // nothing to do
	}

    switch (uMsg)
    {
        case MOM_POSITIONCB:
             // assign address of MIDIHDR
             lpMIDIHeader = (LPMIDIHDR)dwParam1;

             // get address of event data
             lpMIDIEvent = (MIDIEVENT *)&(lpMIDIHeader->lpData[lpMIDIHeader->dwOffset]);

             // do something here with event data

             break;

             // handle these messages if desired
        case MOM_OPEN:
        case MOM_DONE:
        case MOM_CLOSE:
             break;
    }
}





// midiOut functions probably will need a LOT of reading...
// Apparently, either no one uses this or all their code is proprietary -- I can't find any samples
HMIDIOUT   hMidiOut, *lphMidiOut;
MIDIHDR    MidiOutHdr, *lpMidiOutHdr;
UINT       cbMidiOutHdr;
DWORD	   dwFlags;
DWORD     *dwCallback; // callback function name?
DWORD     *dwCallbackInstance; // _should_ be this program instance, right?

lphMidiOut = &hMidiOut;
lpMidiOutHdr; = &MidiOutHdr;

cbMidiOutHdr = sizeof(MidiOutHdr); // ? size right ?

midiOutPrepareHeader
MMRESULT midiOutPrepareHeader( HMIDIOUT hMidiOut, LPMIDIHDR lpMidiOutHdr, UINT cbMidiOutHdr );
midiOutUnPrepareHeader
MMRESULT midiOutUnprepareHeader( HMIDIOUT hMidiOut, LPMIDIHDR lpMidiOutHdr, UINT cbMidiOutHdr );

MMRESULT midiOutOpen( LPHMIDIOUT lphMidiOut, UINT_PTR uDeviceID, DWORD_PTR dwCallback, DWORD_PTR dwCallbackInstance, DWORD dwFlags );

MMRESULT midiOutClose( HMIDIOUT hMidiOut );

*/
 
#endif