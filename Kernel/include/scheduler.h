#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../include/lib.h"
#include "../include/the_memory_manager.h"

typedef struct pcb {
  char args[6][21];
  uint32_t pid; // pid = 1 -> Proceso de la shell
  uint8_t pstate; // por defecto es 1 que significa 'ready'
  uint8_t priority; // mas bajo -> mayor prioridad. La shell es el unico con prioridad = 1. El user tiene un rango de 2 a 10.
  uint64_t sp; // stack pointer
  uint64_t bp; // base pointer
  uint64_t processMemory;
} pcb;


void initScheduler();

uint64_t switchProcess(uint64_t sp);

int createProcess(uint64_t ip, uint8_t priority, uint64_t argc, char args[6][21]);

int createProcessWrapper(uint64_t ip, uint8_t priority, uint64_t argc, char * argv);

void exitCurrentProcess();

void printProcessList();

void killPid(uint32_t pid);

void changeProcessPriority(uint32_t pid, uint8_t newPriority);

void changeProcessState(uint32_t pid);

pcb *blockCurrentProcess();

uint32_t getCurrentPid();

pcb *getCurrentProcess();

#endif