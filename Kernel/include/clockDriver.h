#ifndef _CLOCKDRIVER_H_
#define _CLOCKDRIVER_H_

extern int getRTC(int code);

// ----------------------------------------------------------
// getSeconds(): devuelve los segundos actuales
// ----------------------------------------------------------
// Retorna:
//     -Los segundos
// ----------------------------------------------------------
unsigned int getSeconds();

// ----------------------------------------------------------
// getTimeFormat(): devuelve el tiempo en el formato 
//                  correspondiente
// ----------------------------------------------------------
// Argumentos:
//    - El numero a convertir
// ----------------------------------------------------------
// Retorna:
//     -El tiempo en el formato correcto, no en RTC
// ----------------------------------------------------------
unsigned int getTimeFormat(unsigned int num);

// ----------------------------------------------------------
// sleep(): suspende el programa 
// ----------------------------------------------------------
// Argumentos:
//    - Cantidad de segundos que se suspende el programa
// ----------------------------------------------------------
void sleep(int seconds);

#endif