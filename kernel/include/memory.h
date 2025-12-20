#ifndef MEMORY_H
#define MEMORY_H
#include <types.h>

/* 
 * memset - Fills a block of memory with the specified value.
  * 
  * @ptr: Pointer to the block of memory to fill.
  * @value: Value to be set.
  * @num: Number of bytes to be set to the value.
 */
void* memset(void* ptr, int value, size_t num);

#endif