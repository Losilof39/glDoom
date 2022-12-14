///*
//   gconsole.c - graphical console support functions
//   by Bruce Lewis
//*/
//
////#include <windows.h>
//#include <stdio.h>
//#include <io.h>
//#include <fcntl.h>
//#include <stdarg.h>
////#include <gl/gl.h>
////#include <gl/glu.h>
//#include <glad/glad.h>
//#include "gldefs.h"
//#include "v_video.h"
//
//void InitGLPalette(int red, int green, int blue);
//void lfprintf(char *message, ... );
//
//#define MAX_MESSAGE  256              // size of message loop
//#define MSG_MASK     0xFF             // mask for recycling the write line
//
//extern unsigned char  TexRaw[512*512];
//extern unsigned char  TexAlpha[512*512];
//extern int            TexWide, TexHigh;
//
//extern video_t        video;
//
////extern HDC            hGDC;
//GLubyte               TexRGB[512];
//
//dboolean              glReady = false;
//
//typedef struct
//   {
//    unsigned char r;
//    unsigned char g;
//    unsigned char b;
//   }MY_PAL;
//
//extern MY_PAL  statpal[256];
//
////int ConFont[2];
//int ConFont[2][96];
//int ConBack;
//
//typedef struct
//   {
//    float  ls;    // Left 's' texture coordinate
//    float  tt;    // Top 't' texture coordinate
//    float  rs;    // Right 's' texture coordinate
//    float  bt;    // Bottom 't' texture coordinate
//   }glfont_t;
//
//glfont_t  GLFont[2][96];  // One yellow font and one white font
//
//int MakeRGBATexture(dboolean clamp, dboolean smooth, int dw, int dh);
//int MakeRGBTexture(int dw, int dh);
//
//#define GLD_TRANSPARENT 0
//#define GLD_COLORED     1
//
//typedef struct
//   {
//    int            cxChar;
//    int            cxCaps;
//    int            cyChar;
//    int            cyLines;
//    int            cxChars;
//    unsigned char *console;
//    int            width;           // console window width
//    int            height;          // console window height
//    //HWND           hWnd;            // Window handle of main window
//    //HDC            hDC;             // screen DC
//    //HBITMAP        hBitmap;         // console bitmap for GDI
//    //HDC            hMemDC;          // console memory DC
//    //BITMAPINFO     bmi;
//    //HBITMAP        hFontBMP;
//   }condata_t;
//
//condata_t            ConData;         // console font information
//
//dboolean             con_surface = false;  // is console initialized yet?
////HPEN                 ltgray_pen;           // for drawing same color border with rectangle
//
//char                 msgloop[MAX_MESSAGE][256];  // message loop buffer
//int                  ml_head = 0, ml_draw = 0;   // current message loop draw line
//int                  ccp = 0;                    // current message loop write character position
//
//unsigned char FontData[128*128];
//unsigned char ConInterpic[256*128];
//unsigned char ConPalette[768];
//
//void con_printf( char *message, ... );
//
//void con_SetupGLFont(int font)
//   {
//    int   y, x, i;
//    float top, bottom, left, right;
//
//    top = 1.0f;
//    for (y = 0, i = 0; y < 6; y++)
//       {
//        bottom = top - 0.1171875f;
//        left = 0.0f;
//        for (x = 0; x < 16; x++, i++)
//           {
//            right = left + 0.0625f;
//            // left
//            GLFont[font][i].ls = left;
//            // right
//            GLFont[font][i].rs = right;
//            left = right;
//            // top
//            GLFont[font][i].tt = top;
//            // bottom
//            GLFont[font][i].bt = bottom;
//           }
//        top -= 0.1171875f;
//       }
//   }
//
//void con_MakeGLFont(int font, int red, int green, int blue)
//   {
//    int      TexNumb = 0;
//    int      i, c, x, y, cx, cy, topleft, charoff, srcoff, corner;
//    int      row, col, d, h, t, m, n, TempTexName;
//
//    // This is the top/left of each line of characters - 6 lines of characters
//    for (y = 0, c = 0, topleft = 0; y < 6; y++, topleft += (128*15))
//       {
//        // This is the top/left of each character - 16 characters per line
//        for (x = 0, corner = topleft; x < 16; x++, c++, corner += 8)
//           { 
//            for (i = 0; i < (8*16); i++)
//               {
//                TexRaw[i] = 0;
//                TexAlpha[i] = GLD_TRANSPARENT;
//               }
//            // This is loop through the scan lines of the character - 15 scan lines
//            for (cy = 0, i = 0, charoff = corner; cy < 15; cy++, charoff += 128)
//               {
//                // This is loop through the bytes of the character scan line - 8 texels wide
//                for (cx = 0, srcoff = charoff; cx < 8; cx++, i++, srcoff++)
//                   {
//                    if (FontData[srcoff] == 0xFF)
//                       {
//                        TexRaw[i] = 0xFF;
//                        TexAlpha[i] = GLD_COLORED;
//                       }
//                   }
//               }
//            TexWide = 8;
//            TexHigh = 16;
//
//            t = (TexHigh - 1);
//            for (row = 0; row < TexHigh; row++)
//               {
//                h = t - row;
//                m = (row * TexWide);
//                for (col = 0; col < TexWide; col++)
//                   {
//                    n = m + col;
//                    d = ((h * TexWide) * 4) + (col * 4);
//                    TexRGB[d]   = red;
//                    TexRGB[d+1] = green;
//                    TexRGB[d+2] = blue;
//                    if (TexAlpha[n] == GLD_COLORED)
//                       {
//                        TexRGB[d+3] = 255;
//                       }
//                    else
//                       {
//                        TexRGB[d+3] = 0;
//                       }
//                   }
//               }
//
//            glGenTextures(1, &TempTexName);
//            glBindTexture(GL_TEXTURE_2D, TempTexName);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//            // Define the 2D texture image.
//            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	// Force 4-byte alignment
//            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
//            glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
//            glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
//
//            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TexWide, TexHigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, TexRGB);
//            glBindTexture(GL_TEXTURE_2D, 0);
//            ConFont[font][c] = TempTexName;
//           }
//       }
///*
//    for (i = 0; i < (128*128); i++)
//       {
//        if (FontData[i] == 0xFF)
//           {
//            TexRaw[i] = 0xFF;
//            TexAlpha[i] = GLD_COLORED;
//           }
//        else
//           {
//            TexRaw[i] = 0;
//            TexAlpha[i] = GLD_TRANSPARENT;
//           }
//       }
//    ConFont[font] = MakeRGBATexture(true, true, 128, 128);
//*/
//   }
//
//
void con_SetupOpenGL()
   {
//    con_SetupGLFont(0);
//    con_SetupGLFont(1);
//    con_MakeGLFont(0, 255, 255, 255);  // A white font
//    con_MakeGLFont(1, 255, 255, 0);    // A yellow font
//
//    memcpy(statpal, ConPalette, 768);
//    TexWide = 256;
//    TexHigh = 128;
//    memcpy(TexRaw, ConInterpic, (256*128));
//    ConBack = MakeRGBTexture(256,128);
//    ConData.cyLines = ConData.height/16;
//    ConData.cxChars = ConData.width/8;
//    glReady = true;
//    con_printf("OpenGL Console - Startup\n");
   }
//
//void glDrawText(int x, int y, char *text)
//   {
//    int c, ch;
//    int left, right, top, bottom;
//
//    left = (x * 8) + 1;
//    top = ConData.height-(y * 16);
//    bottom = top-16;
//
//    //glBindTexture(GL_TEXTURE_2D, ConFont[0]);
//    for (c = 0; text[c] != '\0'; c++)
//       {
//        right = left + 8;
//        if (text[c] != ' ')
//           {
//            ch = text[c] - 33;
//            glBindTexture(GL_TEXTURE_2D, ConFont[0][ch]);
//            glBegin(GL_QUADS);
//               //glTexCoord2f(GLFont[0][ch].ls, GLFont[0][ch].tt);
//               glTexCoord2f(0.0f, 1.0f);
//               glVertex2i(left, top);
//               //glTexCoord2f(GLFont[0][ch].ls, GLFont[0][ch].bt);
//               glTexCoord2f(0.0f, 0.0f);
//               glVertex2i(left, bottom);
//               //glTexCoord2f(GLFont[0][ch].rs, GLFont[0][ch].bt);
//               glTexCoord2f(1.0f, 0.0f);
//               glVertex2i(right, bottom);
//               //glTexCoord2f(GLFont[0][ch].rs, GLFont[0][ch].tt);
//               glTexCoord2f(1.0f, 1.0f);
//               glVertex2i(right, top);
//            glEnd();
//           }
//        left += 8;
//       }
//    //glEnd();
//   }
//
//void con_draw()
//   {
//    int          i, lines;
//    int          ml_tail;
//    RECT         drect;
//
//    if ((!glReady) && (!con_surface))
//       {
//        return;
//       }
//
//    if (ml_head < ConData.cyLines)
//       {
//        lines = ml_head;
//       }
//    else
//       {
//        lines = ConData.cyLines;
//       }
//    if (!glReady)
//       {
//        /*SelectObject(ConData.hMemDC, ConData.hBitmap);
//        SelectObject(ConData.hMemDC, GetStockObject(GRAY_BRUSH));
//        SelectObject(ConData.hMemDC, ltgray_pen);
//        SelectObject(ConData.hMemDC, GetStockObject(SYSTEM_FIXED_FONT));
//        Rectangle(ConData.hMemDC, 0, 0, ConData.width, ConData.height);
//        SetBkColor(ConData.hMemDC, RGB(128, 128, 128));
//        SetTextColor(ConData.hMemDC, RGB(255, 255, 255));
//        drect.right = ConData.width;
//        drect.left = 0;
//        for (ml_tail = ml_head-lines, i = 0; ml_tail < ml_head; ml_tail++, i++)
//           {
//            drect.top = ConData.cyChar*i;
//            drect.bottom = drect.top + ConData.cyChar;
//            DrawText(ConData.hMemDC, msgloop[ml_tail], strlen(msgloop[ml_tail]), &drect, DT_LEFT | DT_NOCLIP);
//           }
//        InvalidateRect(NULL, NULL, false);
//        if (BitBlt(ConData.hDC, 0, 0, ConData.width, ConData.height, ConData.hMemDC, 0, 0, SRCCOPY) == false)
//           {
//            MessageBox(NULL, "BitBlt of \"console\" failed...", "Information", MB_OK);
//           }*/
//       }
//    else
//       {
//        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//
//        // Define the modelview transformation.
//        glMatrixMode(GL_MODELVIEW);
//        glLoadIdentity();
//
//        glTranslatef( 0.0f, 0.0f, 0.0f );
//
//        glEnable(GL_TEXTURE_2D);
//        glBindTexture(GL_TEXTURE_2D, ConBack);
//        glBegin(GL_QUADS);
//          glTexCoord2f( 0.0f, 1.0f );
//          glVertex2i( 0, ConData.height );
//          glTexCoord2f( 0.0f, 0.0f );
//          glVertex2i( 0, 0 );
//          glTexCoord2f( 1.0f, 0.0f );
//          glVertex2i( ConData.width, 0 );
//          glTexCoord2f( 1.0f, 1.0f );
//          glVertex2i( ConData.width, ConData.height );
//        glEnd();
//
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//        glColor4f( 1.0f, 1.0f, 1.0f, 1.0f);
//        //glEnable(GL_ALPHA_TEST);
//        //glAlphaFunc(GL_GEQUAL, 0.1f);
//
//        for (ml_tail = (ml_head-lines)+1, i = 0; ml_tail <= ml_head; ml_tail++, i++)
//           {
//            glDrawText(1, i, msgloop[ml_tail]);
//           }
//
//        // set up a projection matrix to fill the client window
//        glMatrixMode( GL_PROJECTION );
//        glLoadIdentity();
//
//        glDisable(GL_BLEND);
//        //glDisable(GL_ALPHA_TEST);
//
//        glOrtho(0, ConData.width, 0, ConData.height, 0.5, 10.0);
//        glViewport( 0, 0, video.width, video.height);
//
//        // a perspective-view matrix...
//        //glRotatef( Camera.Orient.y, 0.0f, 1.0f, 0.0f );
//        glTranslatef( 0.0f, 0.0f, -5.0f );
//
//        // Swap the rendering buffers...
//        //glFlush();
//        //SwapBuffers(hGDC);
//       }
//   }
//
//void con_printf( char *message, ... )
 //{
//    va_list	argptr;
//    int     i, bc, tlen, mline = 0, mask = MSG_MASK;
//    char    tstr[1024];
//
//    va_start(argptr,message);
//    vsprintf (tstr,message,argptr);
//    va_end(argptr);
//
//    lfprintf(tstr);
//
//    if (!con_surface)
//       {
//        return;
//       }
//
//    if (ml_head < 0)
//       {
//        ml_head = 0;
//       }
//
//    tlen = (int)strlen(tstr);
//
//    if (tlen > ConData.cxChars)
//       {
//        bc = ConData.cxChars;
//        while ((tstr[bc--] != ' ') && (bc > 0));
//        if (bc == 0)
//           {
//            bc = ConData.cxChars;
//           }
//       }
//    else
//       {
//        bc = 0xFFFF;
//       }
//
//    for (i = 0; i < tlen; i++)
//       {
//        switch(tstr[i])
//           {
//            case '\n': // handle "newline" chars
//                 //msgloop[ml_head&MSG_MASK][ccp] = '\0';
//                 ml_head++;
//                 ccp = 0;
//                 if ((int)strlen(&tstr[i]) > ConData.cxChars)
//                    {
//                     bc = i+ConData.cxChars;
//                     while ((tstr[bc--] != ' ') && (bc > i));
//                     if (bc == i)
//                        {
//                         bc = i+ConData.cxChars;
//                        }
//                    }
//                 else
//                    {
//                     bc = 0xFFFF;
//                    }
//                 memset(msgloop[ml_head&MSG_MASK], 0, 256);
//                 break;
//            case '\b': // handle "back-space" chars
//                 if (ccp > 0)
//                    {
//                     ccp--;
//                    }
//                 break;
//            default:
//                 if (bc <= i)
//                    {
//                     ml_head++;
//                     ccp = 0;
//                     if ((int)strlen(&tstr[i]) > ConData.cxChars)
//                        {
//                         bc = i+ConData.cxChars;
//                         while ((tstr[bc--] != ' ') && (bc > i));
//                         if (bc == i)
//                            {
//                             bc = i+ConData.cxChars;
//                            }
//                        }
//                     else
//                        {
//                         bc = 0xFFFF;
//                        }
//                    }
//                 mline = ml_head & MSG_MASK;
//                 msgloop[mline][ccp] = tstr[i];
//                 ccp++;
//                 break;
//           }
//       }
//    if (con_surface)
//       {
//        con_draw();
//       }
 // }
//
//void con_clear()
//   {
//    SelectObject(ConData.hMemDC, ConData.hBitmap);
//    SelectObject(ConData.hMemDC, GetStockObject(LTGRAY_BRUSH));
//    SelectObject(ConData.hMemDC, ltgray_pen);
//    Rectangle(ConData.hMemDC, 0, 0, ConData.width, ConData.height);
//    BitBlt(ConData.hDC, 0, 0, ConData.width, ConData.height, ConData.hMemDC, 0, 0, SRCCOPY);
//   }
//
//void con_setup(int width, int height)
//   {
////    HDC             hdc;
////    TEXTMETRIC      tm;
////    int             y, srcoff, dstoff;
////    HRSRC           hRsrc = 0;
////    HGLOBAL         hGlobal = 0;
////    unsigned char  *dibits = 0;
////
////    ConData.hWnd = hwnd;
////    ConData.width = width;
////    ConData.height = height;
////    hdc = GetDC (hwnd);
////    ConData.hDC = hdc;
////    GetTextMetrics(hdc, &tm);
////    GetStockObject(SYSTEM_FIXED_FONT);
////    ConData.cxChar = tm.tmAveCharWidth;
////    ConData.cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * ConData.cxChar / 2;
////    ConData.cyChar = tm.tmHeight + tm.tmExternalLeading;
////    ConData.cyLines = ConData.height/ConData.cyChar;
////    ConData.cxChars = ConData.width/ConData.cxChar;
////    ConData.hBitmap = CreateCompatibleBitmap(hdc, ConData.width, ConData.height);
////    ConData.hMemDC = CreateCompatibleDC(hdc);
////    ltgray_pen = CreatePen(PS_SOLID, 1, RGB(128,128,128));
////    con_surface = true;
////    for (y = 0; y < MAX_MESSAGE; y++)
////       {
////        memset(msgloop[y], 0, 256);
////       }
////    con_printf("GDI Console startup\n");
////
////    // Windows is SUCH a pig...
////    // This copies the font data from an internal resource
////    // to a bitmap where it can be accessed to make a "bitmap"
////    // font for OpenGL
////    // Filename with the data in it is: gldfont.bin
////    hRsrc = FindResource(NULL, "GLDFONT", "RAWIMAGE");
////    if (hRsrc != 0)
////       {
////        hGlobal = LoadResource(NULL, hRsrc);
////        if (hGlobal != 0)
////           {
////            dibits = LockResource(hGlobal);
////            if (dibits != 0)
////               {
////                srcoff = 127*128;
////                dstoff = 0;
////                for (y = 0; y < 128; y++)
////                   {
////                    memcpy(&FontData[dstoff], &dibits[srcoff], 128);
////                    srcoff -= 128;
////                    dstoff += 128;
////                   }
////               }
////           }
////       }
////    hRsrc = FindResource(NULL, "PLAYPAL", "RAWIMAGE");
////    if (hRsrc != 0)
////       {
////        hGlobal = LoadResource(NULL, hRsrc);
////        if (hGlobal != 0)
////           {
////            dibits = LockResource(hGlobal);
////            if (dibits != 0)
////               {
////                memcpy(ConPalette, dibits, 768);
////               }
////           }
////       }
////    hRsrc = FindResource(NULL, "INTERPIC", "RAWIMAGE");
////    if (hRsrc != 0)
////       {
////        hGlobal = LoadResource(NULL, hRsrc);
////        if (hGlobal != 0)
////           {
////            dibits = LockResource(hGlobal);
////            if (dibits != 0)
////               {
////                memcpy(ConInterpic, dibits, 256*128);
/////*
////                srcoff = 127*256;
////                dstoff = 0;
////                for (y = 0; y < 128; y++)
////                   {
////                    memcpy(&ConInterpic[dstoff], &dibits[srcoff], 256);
////                    srcoff -= 256;
////                    dstoff += 256;
////                   }
////*/
////               }
////           }
////       }
//   }
//
void con_shutdown()
   {
//    if (ltgray_pen != NULL)
//       {
//        SelectObject(ConData.hMemDC, GetStockObject(BLACK_PEN));
//        DeleteObject(ltgray_pen);
//        ltgray_pen = NULL;
//       }
//    ReleaseDC (ConData.hWnd, ConData.hDC);
   }
