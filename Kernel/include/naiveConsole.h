#ifndef _NAIVECONSOLE_H_
#define _NAIVECONSOLE_H_

#include <stdint.h>

#define RED 0x04
#define WHITE 0x0F
#define GREEN 0x02
#define YELLOW 0x0E
#define BROWN 0x06
#define LIGHT_BLUE 0x0B

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncClear();
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void displayCursor();





// ----------------------------------------------------------
// ncPrintCharColour: imprime un char en color en pantalla 
//                    en el valor acutal de current position
// ----------------------------------------------------------
// Argumentos:
//      1. El char
//      2. El color
// ----------------------------------------------------------
void ncPrintCharColour(char c, int colour);


// ----------------------------------------------------------
// ncPrintStringColour: imprime un string en color en  
//                      pantalla en el valor acutal de 
//                      current position
// ----------------------------------------------------------
// Argumentos:
//      1. El string
//      2. El color
// ----------------------------------------------------------

void ncPrintStringColour(char * message, int colour);   


// ----------------------------------------------------------
// ncScreenDivition: divide la pantalla con las posiciones
//                   predeterminadas
// ----------------------------------------------------------
void ncScreenDivition();


// ----------------------------------------------------------
// ncPrintHeight: imprime un caracter en predeterminado
//                a lo largo de la pantalla
// ----------------------------------------------------------
void ncPrintHeight();


// ----------------------------------------------------------
// ncPrintWidth: imprime un caracter en predeterminado
//                a lo ancho de la pantalla
// ----------------------------------------------------------
void ncPrintWidth();


// ----------------------------------------------------------
// ncPrintSudokuNumbers: imprime la linea con numeros 
//                       del sudoku en la posiion y 
//                       orientacin determinada
// ----------------------------------------------------------
// Argumentos:
//      1. La buffer con los numeros a imprimir
//      2. La posicion en donde se imprime
//      3. La orientacion con la que se imprime
// ----------------------------------------------------------
void ncPrintSudokuNumbers(char* line, uint64_t position, uint64_t orientationX);


// ----------------------------------------------------------
// ncPrintInScreenPosition: imprime un string en una
//                          posision determinada
// ----------------------------------------------------------
// Argumentos:
//      1. La buffer con los numeros a imprimir
//      2. La posicion en donde se imprime
// ----------------------------------------------------------
void ncPrintInScreenPosition(char* message, uint64_t position);


// ----------------------------------------------------------
// ncClearScreen: limpia la pantalla determinada
// ----------------------------------------------------------
// Argumentos:
//      1. La pantalla a limpiar
// ----------------------------------------------------------
void ncClearScreen(uint64_t mode);


// ----------------------------------------------------------
// ncScrollUp: mueve para arriba la pantalla
// ----------------------------------------------------------
// Argumentos:
//      1. La pantalla a mover
// ----------------------------------------------------------
void ncScrollUp(uint64_t screen);


// ----------------------------------------------------------
// ncCheckTerminalPosition: chequea si tengo lugar para  
//             escrbir, sino llama a scrollUp
// ----------------------------------------------------------
void ncCheckTerminalPosition();


// ----------------------------------------------------------
// ncDelete: chequea puedo o no borrar letras en la pantalla
// ----------------------------------------------------------
// Argumentos:
//      1. La pantalla a mover
// ----------------------------------------------------------
void ncDelete();


// ----------------------------------------------------------
// ncChangeScreen: cambio de pantalla
// ----------------------------------------------------------
// Argumentos:
//      1. La pantalla a cambiar
// ----------------------------------------------------------
void ncChangeScreen(int screenMode);

#endif