#ifndef _LIB_H_
#define _LIB_H_

#include <stdint.h>

#define STDIN ((FileDes) 0)
#define STDOUT ((FileDes) 1)


void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
char * intToHexa(uint64_t num, char * str, int bytes);
void reverse(char *str, int len);
char *cpuVendor(char *result);
uint64_t initProcess(uint64_t stackBase, uint64_t ip, uint32_t argc, char * argv[]); // todo: Implementar funcion en assembler

// Fuerza la ejecucion del scheduler con una llamada explicita a int 20h
void forceScheduler(); // todo: Implementar funcion en assembler

extern uint64_t xchg(uint64_t * lock, uint64_t value);


#endif