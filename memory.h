

#ifndef __MEMORY__
#define __MEMORY__

#include <malloc.h>

#define FREE(x)  {if((x) != NULL) free(x);}
#define ZFREE(x) {if((x) != NULL){free(x); (x) = NULL;}}

#endif
