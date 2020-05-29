#include <stdlib.h>

int memcmp(void* mem1, void* mem2, unsigned int size) {
	char* ptr1 = (char*)mem1;
	char* ptr2 = (char*)mem2;
	
	while(size){
		if(*ptr1 != *ptr2){
			if(*ptr1 < *ptr2) 
				return -1;
			else 
				return 1;
		}
		
		ptr1++;
		ptr2++;
		size--;
	}

	return 0;
}

void memcpy(void* dest, void* src, unsigned int count) {
	char *pdest = dest, *psrc = src;
	while(count) {
		*pdest = *psrc;
		pdest++;
		psrc++;
		count--;
	}
}

void memfill(void* dest, char c, unsigned int count) {
	char *pdest = dest;
	while(count){
		*pdest = c;
		pdest++;
		count--;
	}
}