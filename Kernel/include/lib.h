#ifndef _LIB_H_
#define _LIB_H_

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
char * intToHexa(uint64_t num, char * str, int bytes);
void reverse(char *str, int len);
char *cpuVendor(char *result);

#endif