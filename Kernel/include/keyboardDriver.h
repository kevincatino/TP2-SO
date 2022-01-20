#ifndef _KEYBOARDDRIVER_H_
#define _KEYBOARDDRIVER_H_
#include <stdint.h>

#define TAB 15
#define ENTER 10
#define ESC 27
#define SPACE 32
#define SHIFTL 42
#define SHIFTR 54
#define CAPSLOCKS 58
#define BACKSPACE 127
#define PRESS_RELESED_OFFSET 128
#define SHIFTL_RELESED (SHIFTL + PRESS_RELESED_OFFSET)
#define SHIFTR_RELESED (SHIFTR + PRESS_RELESED_OFFSET)

#define KEYS 58
#define MAX_SIZE 10

// Deveulve el scanCode del teclado
extern int kbScanCode();

// Detecta si hay algo en el buffer
extern int kbDetection();


extern uint64_t* get_regs();


void keyboard_handler();

// Me convierte del scanCode al char que corresponde
char scanToChar(int scancode);

// Retorna el buffer
unsigned char* getBuffer();


// Me coloca la tecla en el buffer
void putCharInBuffer(char sc);


// Retorna la tecal correspondiente en el buffer y la elimina del mismo
char getChar();


uint64_t* getRegs();

#endif