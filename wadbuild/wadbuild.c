// wadbuild.c
// reads a wadname.lst file and builds wad file with same
// name using the lump files listed in the lst file.
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>

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
    if ((namelist = fopen(argv[1], "r")) == NULL)
       {
        fprintf(stderr, "Error: Can't open input list file %s\n", argv[1]);
        exit(-1);
       }

    str = strtok(argv[1], ".");
    strcpy(wadname, str);
    strcat(wadname, ".wad");

    wadfile = _open(wadname, O_RDWR | O_CREAT | O_TRUNC | O_BINARY, 666);
    wadhead.offset = sizeof(wadhead_t);
    _write(wadfile, &wadhead, sizeof(wadhead_t));
    while (fgets(readline, 256, namelist ) != NULL)
       {
        if (strlen(readline) == 0)
           {
            continue;
           }
        if ((str = strchr(readline, '#')) != NULL)
           {
            str[0] = '\0';
           }
        if (strlen(readline) == 0)
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
        _strupr(tstr);
        if ((tch = strrchr(tstr, '/')) == NULL)
           {
            if ((tch = strrchr(tstr, '\\')) != NULL)
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
        if ((ne = strrchr(resname, '.')) != NULL)
           {
            ne[0] = '\0';
            strcpy(fext, &ne[1]);
           }
        printf("Entry %d: %s -> %s ", wadhead.entries, str, resname);
        pad = 0;
        if ((resource = _open(str, O_RDONLY | O_BINARY)) == -1)
           {
            // Just a place holder... MARKER FILE
            waddir[wadhead.entries].length = 0;
           }
        else
           {
            flength = _filelength(resource);
            databuff = (unsigned char *)realloc(databuff, flength+4);
            _read(resource, databuff, flength);
            _close(resource);
            if ((flength%4) != 0)
               {
                pad = (4-(flength % 4));
               }
            _write(wadfile, databuff, flength+pad);
            waddir[wadhead.entries].length = flength;
           }
        printf("%ld bytes.", flength);
        memcpy(waddir[wadhead.entries].name, resname, 8);
        waddir[wadhead.entries].offset = wadhead.offset;
        wadhead.offset += (waddir[wadhead.entries].length + pad);
        wadhead.entries++;
        printf("\n");
       }
    _write(wadfile, waddir, sizeof(waddir_t)*wadhead.entries);
    _lseek(wadfile, 0, SEEK_SET);
    _write(wadfile, &wadhead, sizeof(wadhead_t));
    _close(wadfile);
    fclose(namelist);
   }

