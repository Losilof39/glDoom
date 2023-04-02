// doomlib.c
//
// This source file contains functions that are not supported by the
// ANSI 'C' specification but which appear in some form in most
// implementations.  These are provided to make the software more
// portable by using only ANSI standard library functions.


char *D_strupper (char *str)
   {
    char *c, t;

    t = 'a' - 'A';

    c = str;
    while (*c)
       {
        *c = ((*c >= 'a') && (*c <= 'z')) ? *c - t : *c;
        c++;
       }
    return str;
   }

char* D_strlower(char* str)
{
    char* c, t;

    t = 'A' - 'a';

    c = str;

    while (*c)
    {
        *c = ((*c >= 'A') && (*c <= 'Z')) ? *c - t : *c;
        c++;
    }
    return str;
}

#ifdef ANSI_C
int strncasecmp(char *s1, char *s2, int count)
   {
    unsigned char c1, c2, c3;
	
    c3 = 'a' - 'A';

    while (1)
       {
        c1 = (unsigned char)*s1++;
        c2 = (unsigned char)*s2++;

        if (!count--)
            return 0;		// strings are equal until end point
		
		if (c1 != c2)
           {
            if (c1 >= 'a' && c1 <= 'z')
                c1 -= c3;
            if (c2 >= 'a' && c2 <= 'z')
                c2 -= c3;
            if (c1 != c2)
                return (int)(c1 - c2);  // strings not equal
           }
        if (!c1)
            return 0;   // strings are equal
       }
	return -1;
   }

int strcasecmp(char *s1, char *s2)
   {
    return(D_strncasecmp(s1, s2, 99999));
   }
#endif