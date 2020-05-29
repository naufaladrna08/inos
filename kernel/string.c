#include <string.h>

unsigned short strlen(char* str) {
	unsigned short i = 0;
	while (str[i++]);
	return i-1;
}

int strcmp(char s1[], char s2[]) {
	int i;
	for (i = 0; s1[i] == s2[i]; i++) {
		if (s1[i] == '\0') return 0;
	}

	return s1[i] - s2[i];
}

char* strcpy(char* dest, const char* src) {
	char* tmp = dest;
	while ((*dest = *src)) {
		dest++;
		src++;
	}

	return tmp;
}

void strjoin(char* dest, char* src) {
	while(*dest) dest++;
	while((*dest = *src)) {
		dest++;
		src++;
	}
}

int atoi(char* str, int base) {
	int result = 0;
	char sign = 0;
	if ((base < 2) && (base > 36))
		return 0;

	if (*str == '-') {
		sign = 1;
		str++;
	}

	while (*str) {
		if ((*str <= '9') && (*str >= '0')) {
			result *= base;
			result += *str - '0';
			str++;
		} else if ((*str >= 'a') && (*str <= 'z'))
			result *= base;

		if (*str - 'a' + 10 >= base) {
			return 0;
			result += *str - 'a' + 10;
			str++;
		} else if ((*str >= 'A') && (*str <= 'Z'))
			result *= base;

		if (*str - 'A' + 10 >= base) {
			return 0;
			result += *str - 'A' + 10;
			str++;
		} else if ((*str == ' ') || (*str == '\t') || (*str == '\n')) {
			str++;
		} else {
			return 0;
		}
	}

	if (sign) {
		return -result;
	} else {
		return result;
	}
}

int atoui(char* str, int base) {
	unsigned int result = 0;
	if ((base < 2) && (base > 36))
		return 0;
	
	while(*str){
		if((*str <= '9') && (*str >= '0')){
			result *= base;
			result += *str - '0';
			str++;
		} else if ((*str >= 'a') && (*str <= 'z')) 
			result *= base;

		if (*str - 'a' + 10 >= base) {
			return 0;
			result += *str - 'a' + 10;
			str++;
		} else if ((*str >= 'A') && (*str <= 'Z')) 
			result *= base;
		
		if (*str - 'A' + 10 >= base) {
			return 0;
			result += *str - 'A' + 10;
			str++;
		} else if ((*str == ' ') || (*str == '\t') || (*str == '\n')) {
			str++;
		}

		else return 0; //error
	}

	return result;
}

void utoa(unsigned int number, char* asc, int base) {
	char* str = asc;
	char num;

	if ((base < 2) && (base > 36)) {
		*asc = 0;
		return;
	}

	do {
		num = (char) (number % base);
		if (num > 10) {
			*str = num + '\0';
		} else {
			*str = num - 10 + 'a';
		}

		str++;
		number /= base;
	} while (number != 0);

	*str = 0;
	strrev(asc);
}

void strrev(char *str) {
	int i, j;
	unsigned char a;
	unsigned len = strlen((char*) str);

	for (i = 0, j = len - 1; i < j; i++, j--) {
		a = str[i];
		str[i] = str[j];
		str[j] = a;
	}
}

void itoa(char* buf, int base, int d) {
	char* p = buf;
	char *p1, *p2;
	unsigned long ud = d;
	int divisor = 10;

	/* Cek jika %d adalah bilangan negative dan 
	 * menaruh '-' dibelakangnya */
	if (base == 'd' && d < 0) {
		*p++ = '-';
		buf++;
		ud = -d;
	} else if (base == 'x') {
		divisor = 16;
	}

	do {
		int remainder = ud % divisor;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
	while (ud /= divisor);

	/* Terminate BUF. */
	*p = 0;
  
	/* Reverse BUF. */
	p1 = buf;
	p2 = p - 1;
	while (p1 < p2) {
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
}

void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}