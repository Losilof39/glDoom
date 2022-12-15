/////////////////////////////////////////////////////////////////////////////////////
// Windows Includes...
/////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <io.h>
#else
#include <inttypes.h>
#include <unistd.h>
#endif

#include <fcntl.h>
#include "sys_sdl.h"

//#include "i_cd.h"      // My own cdaudio "library"
#include "i_midi.h"    // My own midi "library"

//extern windata_t WinData;
void lfprintf(char *message, ... );

//CD_Data_t   CDData;
//MIDI_Data_t MidiData;

#if 0
void GetCDInfo()
   {
    /*int  i;
    char cdName[32];

    if (CDData.CDDevice == 0)
       {
        if (CDOpen(&CDData))
           {
            lfprintf("CDDevice ID = %X\n", CDData.CDDevice);
           }
        else
           {
            CDData.CDTrack = 0;
            CDData.CDTrackCount = 0;
            for (i = 0; i < 32; i++)
                CDData.CDTrackList[i] = 0;
            CDData.CDStatus = cd_unavail;
            CDData.CDMedia = false;
            lfprintf("CD Player is unavailable...\n");
            return;
           }
       }
    CDCheck(&CDData);
    if (CDData.CDMedia == false)
       {
        lfprintf("CD Player is empty...\n");
        CDData.CDTrack = 0;
        CDData.CDTrackCount = 0;
        for (i = 0; i < 32; i++)
           CDData.CDTrackList[i] = 0;
        CDData.CDStatus = cd_empty;
       }
    else
       {
        lfprintf("CD Player has a CD in it...\n");
        CDMediaIdentity(&CDData, cdName);
        cdName[16] = '\0';
        lfprintf("CD Hex ID = %s\n", cdName);
        if (strcmp(CDData.CDCode, cdName) != 0)
           {
            strcpy(CDData.CDCode, cdName);
            CDData.CDStatus = cd_stop;
            CDTrackCount(&CDData);
            lfprintf("Number of Tracks = %d\n", CDData.CDTrackCount);
            for (CDData.CDTrack = 0; CDData.CDTrack < CDData.CDTrackCount; CDData.CDTrack++)
               {
                CDTrackType(&CDData);
                if (CDData.CDTrackList[CDData.CDTrack] == MCI_CDA_TRACK_AUDIO)
                   lfprintf("Track %d : Audio\n", CDData.CDTrack, CDData.CDTrackList[CDData.CDTrack]);
                else
                   lfprintf("Track %d : Data\n", CDData.CDTrack, CDData.CDTrackList[CDData.CDTrack]);
               }
            if (CDData.CDTrackList[0] == MCI_CDA_TRACK_AUDIO)
               CDData.CDTrack = 0;
            else
               CDData.CDTrack = 1;
            if (CDData.CDTrack == CDData.CDTrackCount)
               {
                CDData.CDTrackCount = 0;
                CDData.CDTrack = 0;
                CDData.CDStatus = cd_empty;
               }
           }
       }*/
   }

void PlayCDMusic()
   {
    /*if ((CDData.CDStatus != cd_play) && (CDData.CDStatus != cd_pause))
       {
        if ((CDData.CDStatus == cd_empty) || (CDData.CDStatus == cd_unavail))
           {
            GetCDInfo();
           }
        else
           {
            CDCheck(&CDData);
            if (CDData.CDMedia == false)
                GetCDInfo();
           }
        if ((CDData.CDStatus != cd_empty) && (CDData.CDStatus != cd_unavail))
           {
            if (MidiData.MidiStatus != midi_stop)
               {
                if (MidiData.MidiStatus != midi_nofile)
                   {
                    MidiStop(&MidiData);
                   }
                CDTrackPlay(&CDData);
                CDData.CDStatus = cd_play;
               }
           }
       }*/
   }

void PlayCDTrack(int track)
   {
    /*if ((MidiData.MidiStatus != midi_play) && (MidiData.MidiStatus != midi_pause))
       {
        if ((track < 0) || (track > CDData.CDTrackCount))
           return;
        if (CDData.CDTrackList[track] == MCI_CDA_TRACK_AUDIO)
           {
            CDData.CDTrack = track;
            CDTrackPlay(&CDData);
           }
       }*/
   }

void PlayMidiFile(char *song)
   {
    /*strcpy(MidiData.szMidiFile, song);
    MidiPlay(&MidiData);*/
   }

void PlayMidiMusic()
   {
    /*if ((CDData.CDStatus != cd_stop) && (CDData.CDStatus != cd_empty))
       {
        CDStop(&CDData);
        CDData.CDStatus = cd_stop;
       }
    if ((MidiData.MidiStatus != midi_play) && (MidiData.MidiStatus != midi_pause))
       {
        MidiPlay(&MidiData);
       }*/
   }


void PauseResumeMusic()
   {
    /*switch(MidiData.MidiStatus)
       {
        case midi_play:
             MidiPause(&MidiData);
             break;
        case midi_pause:
             MidiResume(&MidiData);
             break;
       }
    if ((CDData.CDStatus == cd_play) || (CDData.CDStatus == cd_pause))
       {
        if ((CDData.CDStatus == cd_empty) || (CDData.CDStatus == cd_unavail))
           {
            GetCDInfo();
           }
        else
           {
            CDCheck(&CDData);
            if (CDData.CDMedia == false)
               {
                GetCDInfo();
                CDData.CDTrack = CDData.CDTrackCount;
               }
           }
       }
    switch(CDData.CDStatus)
       {
        case cd_play:
             CDPause(&CDData);
             CDData.CDStatus = cd_pause;
             break;
        case cd_pause:
             CDResume(&CDData);
             CDData.CDStatus = cd_play;
             break;
       }*/
   }

void PlayNextSong()
   {
    /*if ((MidiData.MidiStatus != midi_play) && (MidiData.MidiStatus != midi_pause))
       {
        if ((CDData.CDStatus == cd_empty) || (CDData.CDStatus == cd_unavail))
           {
            GetCDInfo();
           }
        else
           {
            CDCheck(&CDData);
            if (CDData.CDMedia == false)
               {
                GetCDInfo();
                CDData.CDTrack = CDData.CDTrackCount;
               }
           }
        if (CDData.CDTrackCount != 0)
           {
            if (CDData.CDStatus != cd_stop)
                CDStop(&CDData);
            CDData.CDTrack++;
            if (CDData.CDTrack >= CDData.CDTrackCount)
                CDData.CDTrack = 0;
            while(CDData.CDTrackList[CDData.CDTrack] != MCI_CDA_TRACK_AUDIO)
               {
                CDData.CDTrack++;
                if (CDData.CDTrack >= CDData.CDTrackCount)
                    CDData.CDTrack = 0;
               }
            CDTrackPlay(&CDData);
            CDData.CDStatus = cd_play;
           }
       }*/
   }

void PlayPrevSong()
   {
    /*if ((MidiData.MidiStatus != midi_play) && (MidiData.MidiStatus != midi_pause))
       {
        if ((CDData.CDStatus == cd_empty) || (CDData.CDStatus == cd_unavail))
           {
            GetCDInfo();
           }
        else
           {
            CDCheck(&CDData);
            if (CDData.CDMedia == false)
               {
                GetCDInfo();
                CDData.CDTrack = CDData.CDTrackCount;
               }
           }
        if (CDData.CDTrackCount != 0)
           {
            if (CDData.CDStatus != cd_stop)
                CDStop(&CDData);
            CDData.CDTrack--;
            if (CDData.CDTrack <= 0)
                CDData.CDTrack = CDData.CDTrackCount;
            while(CDData.CDTrackList[CDData.CDTrack] != MCI_CDA_TRACK_AUDIO)
               {
                CDData.CDTrack--;
                if (CDData.CDTrack < 0 )
                    CDData.CDTrack = CDData.CDTrackCount;
               }
            CDTrackPlay(&CDData);
            CDData.CDStatus = cd_play;
           }
       }*/
   }

void StopMusic()
   {
    /*if ((CDData.CDStatus != cd_stop) && (CDData.CDStatus != cd_empty) && (CDData.CDStatus != cd_unavail))
       {
        CDStop(&CDData);
        CDClose(&CDData);
       }
    if ((MidiData.MidiStatus != midi_stop) && (MidiData.MidiStatus != midi_nofile))
       {
        MidiStop(&MidiData);
       }*/
   }

#endif