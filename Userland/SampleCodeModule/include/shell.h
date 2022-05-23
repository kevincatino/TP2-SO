#ifndef _SHELL_H_
#define _SHELL_H_

#include <libc.h>

#define MAX_SIZE 50   
#define MAX_ARGUMENTS 6
#define MAX_GAMES_ARGUMENTS 2



extern void div_zero();
extern void sys_memory_dump();
extern void sys_print_regs();
extern void throwInvalidOpcode();

void memoryManagerTest(uint64_t argc, char * argv[]);
void syncTest(uint64_t argc, char * argv[]);
void loop(uint64_t argc, char * argv[]);
void kill(uint64_t argc, char * argv[]);


void initializeShell();
void initialMessage();
void generalShell();
int menuCommands(char* input);
void helpMenu();
// void infoReg(uint64_t* registers);

void getTime();
void games();
void printUser();

void updateRegs(uint64_t* registers);

#endif