/* 
   wadbuild.c
   reads a wadname.lst file and builds wad file with same
   name using the lump files listed in the lst file.
*/
#include <stdio.h>
#include <stdlib.h>
#ifdef _MSC_VER
#include <io.h>
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include <string.h>

#ifdef _MSC_VER
#pragma warning(disable:6031)
#pragma warning(disable:6053)
#pragma warning(disable:6308)
#pragma warning(disable:28183)
#endif

#ifndef _MAX_PATH
#define _MAX_PATH   260 /* max. length of full pathname */
#endif

#if defined(_MSC_VER)
#define Open _open
#define Close _close
#define Read _read
#define LSeek _lseek
#define Write _write
#define Strupr _strupr
#define Filelength _filelength
#else
#define Open open
#define Close close
#define Read read
#define LSeek lseek
#define Write write
#define Strupr strupr
#define Filelength filelength
#endif

typedef struct
   {
    int   offset;
    int   length;
    char  name[8];
   }waddir_t;

typedef struct
   {
    char  id[4];
    int   entries;
    int   offset;
   }wadhead_t;

wadhead_t wadhead = { 'P', 'W', 'A', 'D', 0, 0 };

waddir_t  waddir[4096];

char  readline[1024], *str, *tch, *ne, fext[4];
char  wadname[_MAX_PATH], resfile[_MAX_PATH], tstr[_MAX_PATH], resname[10];

FILE *namelist;

int   wadfile, resource, flength, pad;

unsigned char *databuff = NULL;

void main(int argc, char *argv[])
   {
    int i;

    if (argc < 2)
       {
        fprintf(stderr, "Usage:\n\t%s wadname.lst\nWhere: wadname is the name of the wad you want to create.\nLST is the file extension required.\n",
           argv[0]);
        exit(-1);
       }
    namelist = fopen(argv[1], "r");
    if (namelist == NULL)
       {
        fprintf(stderr, "Error: Can't open input list file %s\n", argv[1]);
        exit(-1);
       }

    str = strtok(argv[1], ".");
    strcpy(wadname, str);
    strcat(wadname, ".wad");

    wadfile = Open(wadname, O_RDWR | O_CREAT | O_TRUNC | O_BINARY, 666);
    wadhead.offset = sizeof(wadhead_t);
    Write(wadfile, &wadhead, sizeof(wadhead_t));
    while (fgets(readline, 256, namelist ) != NULL)
       {
        size_t len = strlen(readline);
        if (len == 0)
           {
            continue;
           }
        str = strchr(readline, '#');
        if (str != NULL)
           {
            str[0] = '\0';
           }
        if (len == 0)
           {
            continue;
           }
        str = strtok(readline, " \t\n");
        if (str == NULL)
           {
            continue;
           }

        for (i = 0; ((str[i] != ' ') && (str[i] != '\0')); i++);
        if (str[i] == ' ')
           {
            str[i] = '\0';
           }
        strcpy(tstr, str);
        Strupr(tstr);
        tch = strrchr(tstr, '/');
        if (tch == NULL)
           {
            tch = strrchr(tstr, '\\');
            if (tch != NULL)
               {
                tch++;
               }
            else
               {
                tch = tstr;
               }
           }
        else
           {
            tch++;
           }
        memset(resname, 0, 10);
        strncpy(resname, tch, 8);
        ne = strrchr(resname, '.');
        if (ne != NULL)
           {
            ne[0] = '\0';
            strcpy(fext, &ne[1]);
           }
        printf("Entry %d: %s -> %s ", wadhead.entries, str, resname);
        pad = 0;
        resource = Open(str, O_RDONLY | O_BINARY);
        if (resource == -1)
           {
            /* Just a place holder... MARKER FILE */
            waddir[wadhead.entries].length = 0;
           }
        else
           {
            flength = Filelength(resource);
            databuff = (unsigned char *)realloc(databuff, flength+4);
            Read(resource, databuff, flength);
            Close(resource);
            if ((flength%4) != 0)
               {
                pad = (4-(flength % 4));
               }
            Write(wadfile, databuff, flength+pad);
            waddir[wadhead.entries].length = flength;
           }
        printf("%ld bytes.", flength);
        memcpy(waddir[wadhead.entries].name, resname, 8);
        waddir[wadhead.entries].offset = wadhead.offset;
        wadhead.offset += (waddir[wadhead.entries].length + pad);
        wadhead.entries++;
        printf("\n");
       }
    Write(wadfile, waddir, sizeof(waddir_t)*wadhead.entries);
    LSeek(wadfile, 0, SEEK_SET);
    Write(wadfile, &wadhead, sizeof(wadhead_t));
    Close(wadfile);
    fclose(namelist);
   }