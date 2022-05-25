#ifndef _SHELL_H_
#define _SHELL_H_

#include <libc.h>

#define MAX_SIZE 50   
#define MAX_ARGUMENTS 6


void memoryManagerTest(uint64_t argc, char * argv[]);
void syncTest(uint64_t argc, char * argv[]);
void loop(uint64_t argc, char * argv[]);
void kill(uint64_t argc, char * argv[]);
void cat(uint64_t argc, char * argv[]);
void filter(uint64_t argc,char * argv[]);
void wc(uint64_t argc, char * argv[]);
void block(uint64_t argc, char * argv[]);
void clean(uint64_t argc, char *argv[]);
void ps(uint64_t argc, char * argv[]);
void memStatus(uint64_t argc, char *argv[]);
void nice(uint64_t argc, char *argv[]);
void pipe(uint64_t argc, char * argv[]);
void sem(uint64_t argc, char * argv[]);
void prioTest(uint64_t argc, char *argv[]);
void procTest(uint64_t argc, char *argv[]);


void initializeShell();
void initialMessage();
void generalShell();
int menuCommands(char* input);
void helpMenu(uint64_t argc, char *argv[]);

void printUser();

#endif