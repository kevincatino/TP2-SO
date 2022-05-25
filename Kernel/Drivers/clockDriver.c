// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <clockDriver.h>

#define SECONDS 0x00
#define TRUE 1


unsigned int getSeconds() {
    return getRTC(SECONDS);
}


unsigned int getTimeFormat(unsigned int num) {
    int dec = num & 240;
    dec = dec >> 4;
    int units = num & 15;
    return dec * 10 + units;
}


void sleep(int seconds) { 
	int cantSeconds = getTimeFormat(getSeconds());
	while(TRUE){
		if(getTimeFormat(getSeconds()) - cantSeconds >= seconds) // tiempo actual - tiempo inicial >= tiempo tramscurrido deseado
			return;
	}
}