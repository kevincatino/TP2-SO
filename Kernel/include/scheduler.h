#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../include/lib.h"
#include "../include/the_memory_manager.h"

#define READY 1
#define BLOCKED 0
#define ARG_LENGTH 25
#define NULL ((void*) 0)

typedef struct pcb {
  char args[6][25];
  char * argv[6];
  uint32_t pid; // pid = 1 -> Proceso de la shell
  uint8_t pstate; // por defecto es 1 que significa 'ready'
  uint8_t priority; // mas bajo -> mayor prioridad. La shell es el unico con prioridad = 1. El user tiene un rango de 2 a 10.
  uint64_t sp; // stack pointer
  uint64_t bp; // base pointer
  uint8_t quantum;
  uint64_t processMemory;
} pcb;


void initScheduler();

uint64_t switchProcess(uint64_t sp);

uint32_t  createProcess(uint64_t ip, uint8_t priority, uint64_t argc, char args[6][ARG_LENGTH]);

uint32_t createProcessWrapper(uint64_t ip, uint8_t priority, uint64_t argc, char * argv[]);

uint32_t createProcessForUser(uint64_t ip, uint8_t priority, uint64_t argc, char *argv[]);

void exitCurrentProcess();

void printProcessList();

void killPid(uint32_t pid);

void changeProcessPriority(uint32_t pid, uint8_t newPriority);

void changeProcessState(uint32_t pid, uint8_t state);

void changeProcessPriorityForUser(uint32_t pid, uint8_t newPriority);

void changeProcessStateForUser(uint32_t pid, uint8_t state);

uint32_t getCurrentPid();

pcb *getCurrentProcess();

void awakeProcessFromKBQueue();

void getProcessIntoKBQueue();

pcb *blockCurrentProcess();

#endif