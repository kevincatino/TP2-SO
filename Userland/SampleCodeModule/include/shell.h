#ifndef _SHELL_H_
#define _SHELL_H_

#include <libc.h>
#include <clock.h>
#include <hangman.h>
#include <sudoku.h>
#include <chronometer.h>

#define MAX_SIZE 50   
#define MAX_ARGUMENTS 3
#define MAX_GAMES_ARGUMENTS 2



extern void div_zero();
extern void sys_memory_dump();
extern void sys_print_regs();
extern void throwInvalidOpcode();


void initializeShell();
void initialMessage();
void generalShell();
int menuCommands(char* input);
void helpMenu();
void zeroDivExc();
void invOpCodeExc();
void infoReg();
// void infoReg(uint64_t* registers);

void getTime();
void games();
void globalExit();
void printMem(char *args);
void printUser();

void updateRegs(uint64_t* registers);

#endif