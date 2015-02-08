


#ifndef __YDIRECTENV_H__
#define __YDIRECTENV_H__

/* Direct interface */

#include "devextras.h"

/* XXX U-BOOT XXX */
#if 0
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "assert.h"
#endif
#include "yaffs_malloc.h"

/* XXX U-BOOT XXX */
#if 0
#define YBUG() assert(1)
#endif

#define YCHAR char
#define YUCHAR unsigned char
#define _Y(x) x
#define yaffs_strcpy(a,b)    strcpy(a,b)
#define yaffs_strncpy(a,b,c) strncpy(a,b,c)
#define yaffs_strncmp(a,b,c) strncmp(a,b,c)
#define yaffs_strlen(s)	     strlen(s)
#define yaffs_sprintf	     sprintf
#define yaffs_toupper(a)     toupper(a)

#ifdef NO_Y_INLINE
#define Y_INLINE
#else
#define Y_INLINE inline
#endif

#define YMALLOC(x) yaffs_malloc(x)
#define YFREE(x)   free(x)
#define YMALLOC_ALT(x) yaffs_malloc(x)
#define YFREE_ALT(x)   free(x)

#define YMALLOC_DMA(x) yaffs_malloc(x)

#define YYIELD()  do {} while(0)



//#define YINFO(s) YPRINTF(( __FILE__ " %d %s\n",__LINE__,s))
//#define YALERT(s) YINFO(s)


#define TENDSTR "\n"
#define TSTR(x) x
#define TOUT(p) printf p


#define YAFFS_LOSTNFOUND_NAME		"lost+found"
#define YAFFS_LOSTNFOUND_PREFIX		"obj"
//#define YPRINTF(x) printf x

#include "yaffscfg.h"

#define Y_CURRENT_TIME yaffsfs_CurrentTime()
#define Y_TIME_CONVERT(x) x

#define YAFFS_ROOT_MODE				0666
#define YAFFS_LOSTNFOUND_MODE		0666

#define yaffs_SumCompare(x,y) ((x) == (y))
#define yaffs_strcmp(a,b) strcmp(a,b)

#endif