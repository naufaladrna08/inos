#ifndef STRING_H
#define STRING_H
#include <types.h>

unsigned short strlen(char* str);
int strcmp(char s1[], char s2[]);
char* strcpy(char* dest, const char* src);
void strjoin(char* dest, char* src);
int atoi(char* str, int base);
int atoui(char* str, int base);
void utoa(unsigned int number, char* asc, int base);
void strrev(char *str);
void itoa(char* buf, int base, int d);
void append(char s[], char n);

#endif /* string.h */