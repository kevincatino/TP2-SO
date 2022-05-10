#ifndef _LIBC_H_
#define _LIBC_H_

#include <stdint.h>



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


extern int sys_read(unsigned int fd, char* buffer, unsigned int buffercount);    
extern int sys_write(unsigned int fd, char*buffer, unsigned int buffersize); 
extern char sys_get_char();
extern void sys_screen_divition();
extern void sys_screen_clear(uint64_t mode);
extern void sys_print_user(unsigned int fd, char*buffer, unsigned int buffersize); 
extern void sys_get_screen_area(char* buffer, unsigned int buffersize); 
extern void sys_print_sudoku_numbers(char* line, int position, int orientationX); 
extern void sys_print_in_screen_position(char* line, int position); 
extern void sys_screen_mode(uint16_t mode);
extern uint64_t sys_get_milli_seconds(uint64_t time);
extern void *sys_alloc(unsigned int to_alloc);
extern void sys_free(void *to_free);
extern int sys_create_process(uint64_t ip, uint8_t priority, uint64_t argc, char *argv);
extern int sys_change_process_priority(uint32_t pid, uint8_t newPriority);
extern int sys_change_process_state(uint32_t pid);

extern void get_regs();




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
// getScreenDivition: llama a la sys call de division de 
// 						pantalla
// ----------------------------------------------------------
void getScreenDivition();


// ----------------------------------------------------------
// getScreenDivition: limpia la pantalla
// ----------------------------------------------------------
// Argumentos:
//      1. La subpantalla a limpiar
// ----------------------------------------------------------
void clearScreen(uint64_t mode);


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
// printSudokuNumbers: pone un char en pantalla
// ----------------------------------------------------------
// Argumentos:
//      1. El char a srcolocar
// ----------------------------------------------------------
void printSudokuNumbers(char* line, uint64_t position, uint64_t orientationX);


// ----------------------------------------------------------
// printInScreenPosition: imprime en la posicion que se le
// 							indique, en la pantalla de juegos
// ----------------------------------------------------------
// Argumentos:
//      1. El mensaje a escrbir
//		2. La posicion a escrbir
// ----------------------------------------------------------
void printInScreenPosition(char* message, uint64_t position);


// ----------------------------------------------------------
// changeScreen: cambia la pantalla donde "quiero estar" 
// ----------------------------------------------------------
// Argumentos:
//      1. El numero de pantalla que quiero estar
// ----------------------------------------------------------
void changeScreen(uint64_t mode);


// ----------------------------------------------------------
// getMilliSeconds: retorna los milisegundos
// ----------------------------------------------------------
// Argumentos:
//      1. El tiempo que le indico
// ----------------------------------------------------------
// Retorna:
//     retorna milisegundos
// ----------------------------------------------------------
uint64_t getMilliSeconds(uint64_t time);


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
// charToDigit: Me pasa el valor a digito
// 				Utiliza: isDigit
// ----------------------------------------------------------
// Argumentos:
//      1. El caracter
// ----------------------------------------------------------
// Retorna:
//      El valor del digito entero si el char es un numero 
// 		o -1 sino.
// ----------------------------------------------------------
int charToDigit(char* buffer);


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
// stringToIntArray: guarda un numero y avanza hasta el  
// 					 siguiente saltando espacios
// ----------------------------------------------------------
// Argumentos:
//      1. El char a guardar
// 		2. El array donde guardarlo
// ----------------------------------------------------------
// Retorna:
//      1 si no hubo errores
//		0 si hubo errores
// ----------------------------------------------------------
void stringToIntArray(char* buffer[], int* positions, int max); 


// ----------------------------------------------------------
// clearBuffer: limpia el buffer
// ----------------------------------------------------------
// Argumentos:
//      1. -
// ----------------------------------------------------------
void clearBuffer();


// ----------------------------------------------------------
// toStr: convierte un numero a string
// ----------------------------------------------------------
// Argumentos:
//      1. El numero a convertir
// 		2. El retorno
// ----------------------------------------------------------
void toStr(int num, char* ret);


// ----------------------------------------------------------
// putInBuffer: coloca en el buffer el char 
// ----------------------------------------------------------
// Argumentos:
//      1. El char a colocar en el buffer
// ----------------------------------------------------------
void putInBuffer(char c);


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
// power: Hace la potencia
// ----------------------------------------------------------
// Argumentos:
//      1. El numero
//      2. La potencia de 10. Ejemplo: (5 ^ p)
// ----------------------------------------------------------
// Retorna:
//      El numero final
// ----------------------------------------------------------
int power(int num, int p);


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
int strtok(char *s, char delim, char *array[], int arraySize);


// ----------------------------------------------------------
// readInput: lee lo que el usuario escribe
// ----------------------------------------------------------
// Argumentos:
//      1. El char del buffer
// 		2. EL size
// 		3. Flag 1 para shell, 0 para general
// ----------------------------------------------------------
void readInput(char * inputBuffer, int maxSize, int flag);


int checkTabCount();


// ----------------------------------------------------------
// intToString: convierte de int a string
// ----------------------------------------------------------
// Argumentos:
//      1. El numero a convertir
// 		2. El string resultante
// ----------------------------------------------------------
void intToString(int num, char *str);


#endif


