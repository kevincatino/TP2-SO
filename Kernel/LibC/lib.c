#include <lib.h>

#define TRUE 1
#define SECONDS 0x00


void * memset(void * destination, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--) {
		dst[length] = chr;
	}

	return destination;
}


void * memcpy(void * destination, const void * source, uint64_t length) {
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 && (uint64_t)source % sizeof(uint32_t) == 0 && length % sizeof(uint32_t) == 0) {
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++) {
			d[i] = s[i];
		}	
	} else {
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++){
			d[i] = s[i];
		}
	}

	return destination;
}


//https://www.geeksforgeeks.org/program-decimal-hexadecimal-conversion/
char* intToHexa(uint64_t num, char * str, int bytes) { 
    // counter for hexadecimal number array
    int i = 0;
    while (i < bytes*2 && num != 0) {
 
        // storing remainder in temp variable.
        int temp = num % 16;
 
        // check if temp < 10
        if (temp < 10)
            str[i++] = temp + '0';
        else
            str[i++] = temp - 10 + 'A';
 
        num = num / 16;
    }

    while (i < bytes*2)
        str[i++] = '0';

    str[i++] = 'x';
    str[i++] = '0';
    str[i] = '\0';
  
    // Reverse the string 
    reverse(str, i); 
    return str;
} 


void reverse(char *str, int len) {
	int temp;
	for(int i=0; i<len/2; i++){
		temp = str[i];
		str[i] = str[len-i-1];
		str[len-i-1]=temp;
	}
}

uint64_t strlength(char * string) {
	uint64_t i=0;
	while(string[i]!=0) {
		i++;
	}
	return i;
}



