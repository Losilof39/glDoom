//// win_joy.c
//// These are the Windows joystick support functions
//#include <windows.h>
//#include <mmsystem.h>
//
//#include "doomtype.h"
//#include "d_main.h"
//#include "d_event.h"
//
//#define JOYMAX  2  // Doom only really supports 1 joystick, but what the hell...
//#define JOYBMAX 4  // Doom only currently supports 4 buttons
//
//#define NEUTRAL 2500
//
//int joycount;
//
//DWORD joyaxis[JOYMAX][6];
//DWORD joycenter[JOYMAX][6];
//DWORD joybutton[JOYMAX];
//int   joyPOV[JOYMAX];
//
//extern int joyb1, joyb2, joyb3, joyb4, joyb5, joyb6, joyb7, joyb8, joydead;
//
//DWORD joymask[] =  { 0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080,
//                     0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000,
//                     0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000,
//                     0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000 };
//
//JOYINFOEX ji[JOYMAX];
//
//void lfprintf(char *message, ... );
//
//dboolean I_SetupJoysticks()
//   {
//    int       i, j;
//    JOYINFOEX ji;
//
//    // Check to see how many joysticks driver supports
//    j = joyGetNumDevs();
//
//    // Looks like we didn't find a driver...
//    // (Win95 ALWAYS supports at least 2 if a game port is found)
//    if (j == 0)
//       {
//        joycount = 0;
//        return false;
//       }
//    else
//       {
//        // Initialize the data for the 
//        for (j = 0, joycount = 0; j < JOYMAX; j++)
//           {
//            memset(&ji, 0, sizeof(JOYINFOEX));
//            ji.dwSize = sizeof(JOYINFOEX);
//            ji.dwFlags = JOY_RETURNALL;
//            if (joyGetPosEx(j, &ji) == JOYERR_NOERROR)
//               {
//                joycount++;
//                joyaxis[j][0] = ji.dwXpos;   // left/right axis
//                joyaxis[j][1] = ji.dwYpos;   // forward/backward axis
//                joyaxis[j][2] = ji.dwZpos;   // generally throttle
//                joyaxis[j][3] = ji.dwRpos;   // rudder pedals (normally)
//                joyaxis[j][4] = ji.dwUpos;   // usually second x axis
//                joyaxis[j][5] = ji.dwVpos;   // usually second y axis
//                joybutton[j]  = ji.dwButtons;// button status bitfields
//                joyPOV[j]     = ji.dwPOV;    // point of view (hat) position
//
//                joycenter[j][0] = 0;         // capture the "center" value
//                joycenter[j][1] = 0;         // capture the "center" value
//                for (i = 0; i < 50; i++)
//                   {
//                    joycenter[j][0] += ji.dwXpos; // capture the "center" value
//                    joycenter[j][1] += ji.dwYpos; // capture the "center" value
//                   }
//                joycenter[j][0] /= 50;
//                joycenter[j][1] /= 50;
//                //joySetThreshold(j, NEUTRAL);
//               }
//           }
//       }
//
//    if (joycount == 0)
//       {
//        return false;
//       }
//    else
//       {
//        return true;
//       }
//   }
//
//void I_CheckJoysticks(int stick)
//   {
//    int               j;
//    static event_t    ev[2][JOYBMAX+1];
//    static JOYINFOEX  ji, tji;
//
//    for (j = 0; j < joycount; j++)
//       {
//        memset(&ji, 0, sizeof(JOYINFOEX));
//        ji.dwSize = sizeof(JOYINFOEX);
//        ji.dwFlags = JOY_RETURNALL;
//        if (joyGetPosEx(j, &ji) == JOYERR_NOERROR)
//           {
//            if ((joybutton[j] != ji.dwButtons) ||
//                (joyaxis[j][0] != ji.dwXpos) ||
//                (joyaxis[j][1] != ji.dwYpos))
//               {
//                ev[j][JOYBMAX].type = ev_joystick;
//                ev[j][JOYBMAX].data1 = 0;
//                if (joyb1 == 0)
//                    ev[j][JOYBMAX].data1 |= (ji.dwButtons & 1);
//                else
//                   {
//                    if ((joybutton[j] & 1) != (ji.dwButtons & 1))
//                       {
//                        if (ji.dwButtons & 1)
//                           {
//                            ev[j][0].type = ev_keydown;
//                           }
//                        else
//                           {
//                            ev[j][0].type = ev_keyup;
//                           }
//                        ev[j][0].data1 = joyb1;
//                        D_PostEvent(&ev[j][0]);
//                       }
//                   }
//                if (joyb2 == 0)
//                    ev[j][JOYBMAX].data1 |= (ji.dwButtons & 2);
//                else
//                   {
//                    if ((joybutton[j] & 2) != (ji.dwButtons & 2))
//                       {
//                        if (ji.dwButtons & 2)
//                           {
//                            ev[j][1].type = ev_keydown;
//                           }
//                        else
//                           {
//                            ev[j][1].type = ev_keyup;
//                           }
//                        ev[j][1].data1 = joyb2;
//                        D_PostEvent(&ev[j][1]);
//                       }
//                   }
//                if (joyb3 == 0)
//                    ev[j][JOYBMAX].data1 |= (ji.dwButtons & 4);
//                else
//                   {
//                    if ((joybutton[j] & 4) != (ji.dwButtons & 4))
//                       {
//                        if (ji.dwButtons & 4)
//                           {
//                            ev[j][2].type = ev_keydown;
//                           }
//                        else
//                           {
//                            ev[j][2].type = ev_keyup;
//                           }
//                        ev[j][2].data1 = joyb3;
//                        D_PostEvent(&ev[j][3]);
//                       }
//                   }
//                if (joyb4 == 0)
//                    ev[j][JOYBMAX].data1 |= (ji.dwButtons & 8);
//                else
//                   {
//                    if ((joybutton[j] & 8) != (ji.dwButtons & 8))
//                       {
//                        if (ji.dwButtons & 8)
//                           {
//                            ev[j][3].type = ev_keydown;
//                           }
//                        else
//                           {
//                            ev[j][3].type = ev_keyup;
//                           }
//                        ev[j][3].data1 = joyb4;
//                        D_PostEvent(&ev[j][3]);
//                       }
//                   }
//                if (joyb5 == 0)
//                    ev[j][JOYBMAX].data1 |= (ji.dwButtons & 0x10);
//                else
//                   {
//                    if ((joybutton[j] & 0x10) != (ji.dwButtons & 0x10))
//                       {
//                        if (ji.dwButtons & 0x10)
//                           {
//                            ev[j][3].type = ev_keydown;
//                           }
//                        else
//                           {
//                            ev[j][3].type = ev_keyup;
//                           }
//                        ev[j][3].data1 = joyb5;
//                        D_PostEvent(&ev[j][3]);
//                       }
//                   }
//                if (joyb6 == 0)
//                    ev[j][JOYBMAX].data1 |= (ji.dwButtons & 0x20);
//                else
//                   {
//                    if ((joybutton[j] & 0x20) != (ji.dwButtons & 0x20))
//                       {
//                        if (ji.dwButtons & 0x20)
//                           {
//                            ev[j][3].type = ev_keydown;
//                           }
//                        else
//                           {
//                            ev[j][3].type = ev_keyup;
//                           }
//                        ev[j][3].data1 = joyb6;
//                        D_PostEvent(&ev[j][3]);
//                       }
//                   }
//                if (joyb7 == 0)
//                    ev[j][JOYBMAX].data1 |= (ji.dwButtons & 0x40);
//                else
//                   {
//                    if ((joybutton[j] & 0x40) != (ji.dwButtons & 0x40))
//                       {
//                        if (ji.dwButtons & 0x40)
//                           {
//                            ev[j][3].type = ev_keydown;
//                           }
//                        else
//                           {
//                            ev[j][3].type = ev_keyup;
//                           }
//                        ev[j][3].data1 = joyb7;
//                        D_PostEvent(&ev[j][3]);
//                       }
//                   }
//                if (joyb8 == 0)
//                    ev[j][JOYBMAX].data1 |= (ji.dwButtons & 0x80);
//                else
//                   {
//                    if ((joybutton[j] & 0x80) != (ji.dwButtons & 0x80))
//                       {
//                        if (ji.dwButtons & 0x80)
//                           {
//                            ev[j][3].type = ev_keydown;
//                           }
//                        else
//                           {
//                            ev[j][3].type = ev_keyup;
//                           }
//                        ev[j][3].data1 = joyb8;
//                        D_PostEvent(&ev[j][3]);
//                       }
//                   }
//                if (ji.dwXpos < (joycenter[j][0] - joydead))
//                   {
//                    ev[j][JOYBMAX].data2 = -1;
//                   }
//                else
//                if (ji.dwXpos > (joycenter[j][0] + joydead))
//                   {
//                    ev[j][JOYBMAX].data2 = 1;
//                   }
//                else
//                   {
//                    ev[j][JOYBMAX].data2 = 0;
//                   }
//                if (ji.dwYpos < (joycenter[j][1] - joydead))
//                   {
//                    ev[j][JOYBMAX].data3 = -1;
//                   }
//                else
//                if (ji.dwYpos > (joycenter[j][1] + joydead))
//                   {
//                    ev[j][JOYBMAX].data3 = 1;
//                   }
//                else
//                   {
//                    ev[j][JOYBMAX].data3 = 0;
//                   }
//                D_PostEvent(&ev[j][JOYBMAX]);
//               }
//            joyaxis[j][0] = ji.dwXpos;   // left/right axis
//            joyaxis[j][1] = ji.dwYpos;   // forward/backward axis
//            joyaxis[j][2] = ji.dwZpos;   // generally throttle
//            joyaxis[j][3] = ji.dwRpos;   // rudder pedals (normally)
//            joyaxis[j][4] = ji.dwUpos;   // usually second x axis
//            joyaxis[j][5] = ji.dwVpos;   // usually second y axis
//            joybutton[j]  = ji.dwButtons;// button status bitfields
//            joyPOV[j]     = ji.dwPOV;    // point of view (hat) position
//           }
//       }
//   }
