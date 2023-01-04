

#ifndef __MEMORY__
#define __MEMORY__

#include <malloc.h>


#define FREE(x)  {if((x) != NULL) free(x);}
#define ZFREE(x) {if((x) != NULL){free(x); (x) = NULL;}}

#ifndef ZeroMemory
#define ZeroMemory(x, size) memset(x, 0, size)
#endif

#ifndef CopyMemory
#define CopyMemory(dest, source, size) memcpy(dest, source, size)
#endif


#endif
