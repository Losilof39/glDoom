// save pic
//
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <io.h>
#else
#include <inttypes.h>
#include <unistd.h>
#endif
#include <fcntl.h>
#include <malloc.h>

#include "doomtype.h"

int           columns;
unsigned char *image;

void SavePic(int x, int y, unsigned char *texels, char *filename)
{
    unsigned short w, h, vo, ho, run;
    int            wide, high, fn, col = 0, row = 0, start = 0;
    int          *columns, isize = 0;
    unsigned char *image, *p;
    unsigned char  colbuff[128];

    wide = x;
    high = y;
    w = x;
    h = y;

    // allocate space for columns offsets
    columns = (int *)malloc(sizeof(int)*x);
    memset(columns, 0, sizeof(int)*x);

    // Worst case scenario is if every other texel
    // is 0 (blank). This means that every column
    // has y/2 runs of 1 texels. It also means that
    // we have a column offset and a length for each
    // of those runs.  Making it three bytes for each
    // texel painted or 50% larger.
    image = (unsigned char *)malloc(x*y*4);
    memset(image, 0, (x*y*4));

    p = image;
    columns[0] = 8;
    for (col = 0; col < x; col++)
       {
        run = 0;
        start = 0;
        columns[col] = ((sizeof(int)*x)+8)+(p - image);
        for (row = 0; row < y; row++)
           {
            if (texels[(row*x)+col] == 0)
               {
                if (run != 0)
                   {
                    *p = start;
                    p++;
                    *p = (unsigned char)run;
                    p += 2;
                    memcpy(p, colbuff, run);
                    p += (run + 1);
                   }
                run = 0;
                start = row + 1;
               }
            else
               {
                colbuff[run] = texels[(row*x)+col];
                run++;
               }
            vo = run;
           }
        if (run != 0)
           {
            *p = start;
            p++;
            *p = (unsigned char)run;
            p += 2;
            memcpy(p, colbuff, run);
            p += (run + 1);
           }
        *p = 0xff;
        p++;
       }

    // set the vertical and horizontal offsets to 0
    vo = ho = 0;

    // open the file to save the "lump" in
    fn = Open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
    Write(fn, &w, sizeof(short));
    Write(fn, &h, sizeof(short));
    Write(fn, &vo, sizeof(short));
    Write(fn, &ho, sizeof(short));
    Write(fn, columns, sizeof(int)*x);
    Write(fn, image, ((p-image)+1));
    // close the output file
    Close(fn);

    // free the output image
    free(image);
    // free the memory used to hold the column offsets
    free(columns);
}