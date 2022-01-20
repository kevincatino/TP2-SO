#ifndef _TIME_H_
#define _TIME_H_

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();

// ----------------------------------------------------------
// milli_seconds_elapsed: Retorna la cantidad de milisegundos
// ----------------------------------------------------------
// Retorna:
//     Retorna la cantidad de milisegundos
// ----------------------------------------------------------
int milli_seconds_elapsed();

#endif
