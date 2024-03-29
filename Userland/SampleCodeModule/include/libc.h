#ifndef _LIBC_H_
#define _LIBC_H_

#include <stdint.h>


typedef struct fd * FileDes;


#define FALSE 0
#define TRUE 1
#define MAX_BUFFER 100
#define GENERAL_SCREEN 0
#define GAMES_SCREEN 5
#define GAME_SCREEN_SL 1
#define GAME_SCREEN_SR 2
#define GAME_SCREEN_IL 3
#define GAME_SCREEN_IR 4

#define BLACK 0x00
#define RED 0x04
#define WHITE 0x0F
#define GREEN 0x02
#define YELLOW 0x0E
#define BROWN 0x06
#define LIGHT_BLUE 0x0B

#define READY 1
#define BLOCKED 0

#define STDIN ((FileDes) 0)
#define STDOUT ((FileDes) 1)
#define NULL ((void *) 0)


extern int sys_read(char* buffer, unsigned int buffercount);    
extern int sys_write(char*buffer, unsigned int buffersize); 
extern char sys_get_char();
extern void sys_screen_divition();
extern void sys_screen_clear();
extern void sys_print_user(unsigned int fd, char*buffer, unsigned int buffersize); 
extern void sys_get_screen_area(char* buffer, unsigned int buffersize); 
extern void sys_print_sudoku_numbers(char* line, int position, int orientationX); 
extern void sys_print_in_screen_position(char* line, int position); 
extern void sys_screen_mode(uint16_t mode);
extern uint64_t sys_get_milli_seconds(uint64_t time);
extern void *sys_alloc(unsigned int to_alloc);
extern void sys_free(void *to_free);
extern int sys_create_process(uint64_t ip, int priority, uint64_t argc, char * argv[], FileDes stdin, FileDes stdout);
extern int sys_change_process_priority(uint32_t pid, uint8_t newPriority);
extern int sys_change_process_state(uint32_t pid, int state);
extern void sys_exit();
extern uint64_t sys_get_ticks();

extern void get_regs();

typedef struct Sem_T * Semaphore;

extern Semaphore sys_sem_open(uint32_t id, int initialValue);

extern int sys_sem_close(Semaphore sem);

extern int sys_sem_signal(Semaphore sem);

extern int sys_sem_wait(Semaphore sem);

extern uint32_t sys_pid();

extern void sys_yield();

FileDes sys_createFd();

extern int sys_createPipe(FileDes f0, FileDes f1);

extern int sys_openPipeId(FileDes *fd, uint32_t id, int readwrite);

extern void sys_closeFd(FileDes fd);

extern int sys_pipeWrite(FileDes fd, char *string);

extern int sys_pipeRead(FileDes fd, char *buffer, int limit);

extern void sys_killPid(uint32_t pid);

extern void sys_ps();

extern void sys_printMem();

extern void sys_printPipes();

extern void sys_printSemaphores();


char* strCpy(char* destination, const char* source);




// ----------------------------------------------------------
// strlength: me calcula la longitud
// ----------------------------------------------------------
// Argumentos:
//      1. El puntero del string a calcular
// ----------------------------------------------------------
// Retorna:
//     el length 
// ----------------------------------------------------------
uint64_t strlength(char * string);


// ----------------------------------------------------------
// userPrint: llama a la sys call de imprimir el usuario
// ----------------------------------------------------------
// Argumentos:
//      1. El buffer
// ----------------------------------------------------------
void userPrint(char * buffer);



// ----------------------------------------------------------
// getScreenDivition: limpia la pantalla
// ----------------------------------------------------------
// Argumentos:
//      1. La subpantalla a limpiar
// ----------------------------------------------------------
void clearScreen();


// ----------------------------------------------------------
// getChar: agarra el caracter por entrada estandar
// ----------------------------------------------------------
char getChar();


// ----------------------------------------------------------
// putChar: pone un char en pantalla
// ----------------------------------------------------------
// Argumentos:
//      1. El char a srcolocar
// ----------------------------------------------------------
void putChar(char c);



// ----------------------------------------------------------
// reverse: da vuelta un string
// ----------------------------------------------------------
// Argumentos:
//      1. El string
//		2. La longitud del string
// ----------------------------------------------------------
void reverse(char * str, uint64_t length);


// ----------------------------------------------------------
// print: Imprime el String que le pase
//        Utiliza: strlength()   
// ----------------------------------------------------------
// Argumentos:
//      1. El string a imprimir
// ----------------------------------------------------------
void print(char * string);


// ----------------------------------------------------------
// numToStr: Me convierte de numero a String 
// ----------------------------------------------------------
// Argumentos:
//      1. El numero a convertir
//      2. La posicion(desde donde quiero convertir el numero)
//      3. El buffer
// ----------------------------------------------------------
// Retorna:
//      El indice siguiente al fin del numero
// ----------------------------------------------------------
int numToStr(int num, int pos, char*buff);


// ----------------------------------------------------------
// isDigit: Me dice si es digito o no
// ----------------------------------------------------------
// Argumentos:
//      1. El caracter
// ----------------------------------------------------------
// Retorna:
//      1 si es un digito o -1 si no
// ----------------------------------------------------------
int isDigit(char c);



// ----------------------------------------------------------
// toLower: Me pasa a minusculas
// ----------------------------------------------------------
// Argumentos:
//      1. El caracter a transformar
// ----------------------------------------------------------
// Retorna:
//      El caracter en minuscula
// ----------------------------------------------------------
char toLower(char c);


// ----------------------------------------------------------
// toUpper: Me pasa a mayuscula
// ----------------------------------------------------------
// Argumentos:
//      1. El caracter a transformar
// ----------------------------------------------------------
// Retorna:
//      El caracter en mayuscula
// ----------------------------------------------------------
char toUpper(char c);


// ----------------------------------------------------------
// isWhiteSpace: me dice si es un espacio en blanco
// ----------------------------------------------------------
// Argumentos:
//      1. El String a verificar
// ----------------------------------------------------------
// Retorna:
//      1 si es un espacio, 0 si no
// ----------------------------------------------------------
int isWhiteSpace(char c);


// ----------------------------------------------------------
// isAlpha: chequa si el char c es una letra
// ----------------------------------------------------------
// Argumentos:
//      1. El char a verificar
// ----------------------------------------------------------
// Retorna:
//      1 si es una letra, 0 si no
// ----------------------------------------------------------
int isAlpha(char c);



// ----------------------------------------------------------
// strCmp: compara dos string a ver si son iguales o no
// ----------------------------------------------------------
// Argumentos:
//      1. El primer string
// 		2. El segundo string
// ----------------------------------------------------------
// Retorna:
//     0 si son iguales, otro numero sino
// ----------------------------------------------------------
int strCmp(char * s1, char * s2);


// ----------------------------------------------------------
// printNum: imprime un int
// ----------------------------------------------------------
// Argumentos:
//      1. El numero a imprimir
// ----------------------------------------------------------
void printNum(int num);


// Dada por catedra
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);


// ----------------------------------------------------------
// hexaToInt: convierte de hexa a int
// ----------------------------------------------------------
// Argumentos:
//      1. Char a convertir
// ----------------------------------------------------------
// Retorna:
//     el int
// ----------------------------------------------------------
uint64_t hexaToInt(char* hexaAddress, int strBase);



//https://www.geeksforgeeks.org/program-decimal-hexadecimal-conversion/
char * intToHexa(uint64_t num, char * str, int bytes);


// ----------------------------------------------------------
// strtok: almacena en el array las palabras del string 
//         separadas por un delimitador
// ----------------------------------------------------------
// Argumentos:
//      1. El string
//		2. El delimitador
// 		3. El array a donde copiar
// 		4. El maximo arraySize
// ----------------------------------------------------------
// Retorna:
//     el arrayIndex donde se dejo de copiar
// ----------------------------------------------------------
int strtok(char *s, char * delim, char *array[], int arraySize);


// ----------------------------------------------------------
// readInput: lee lo que el usuario escribe
// ----------------------------------------------------------
// Argumentos:
//      1. El char del buffer
// 		2. EL size
// 		3. Flag 1 para shell, 0 para general
// ----------------------------------------------------------
void readInput(char * inputBuffer, int maxSize, int flag);


void sleep(uint64_t seconds);


#endif


