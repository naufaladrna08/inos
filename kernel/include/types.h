#ifndef TYPES_H
#define TYPES_H

/* Standard C Type definition */
#define NULL ((void *)0UL)

typedef char s8;
typedef unsigned char u8;

typedef short s16;
typedef unsigned short u16;

typedef int s32;
typedef unsigned int u32;

typedef long s64;
typedef unsigned long u64;

/* 
	Boolean (by default, we don't have bool in gcc. 
	must include stdbool) 
#ifndef bool
	#define bool 	int
	#define true 	1
	#define false	0
#endif
*/

#endif /* types.h */