// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <naiveConsole.h>
#include <scheduler.h>
#include <the_memory_manager.h>
#include "../include/semaphore.h"
#include <lib.h>

typedef struct Sem_T
{
    uint32_t id;
    uint64_t mutex;
    int value;
    pcb * waitingProcs[MAX_PROCESSES];
    uint32_t waitingProcQty;
} Sem_T;


Sem_T * sems[MAX_SEMS];
uint64_t privSem = 0;
uint32_t qty = 0;

// retorna NULL si superamos la cantidad maxima de semaforos o si lo inicializamos en < 0.
// Si ya esta opened, retornamos el semaforo ya existente
Semaphore my_sem_open(uint32_t id, int initialValue)
{
    while (xchg(&privSem, 1) != 0);

    if (qty == MAX_SEMS || initialValue < 0)
        return NULL;

    int semIdx = 0;
    while (semIdx < qty && sems[semIdx]->id != id)
        semIdx++;

    if (semIdx == qty)
    {
        // creamos un sem nuevo
        sems[qty] = (Sem_T *) allocMemory(sizeof(Sem_T));
        (*sems[qty]) = (Sem_T){.id = id, .value = initialValue, .waitingProcQty = 0, .mutex = 0};

        xchg(&privSem, 0);
        return sems[qty++];
    }

    xchg(&privSem, 0);
    return sems[semIdx];
}

// devuelve -1 sin cerrar el semaforo cuando hay procesos en la lista de espera por el post o no se encuentra el semaforo
int my_sem_close(Semaphore sem)
{
    while (xchg(&privSem, 1) != 0);

    if (sem->waitingProcQty > 0)
        return -1;

    int semIdx = 0;
    while (semIdx < qty && sem->id != sems[semIdx]->id)
        semIdx++;

    if (semIdx == qty)
    {
        // error, no existe el semaforo
        xchg(&privSem, 0);
        return -1;
    }

    // corremos todo a la izquierda
    // [semPrev] -> [sem2] -> [sem3]
    while (semIdx < qty - 1)
    {
        sems[semIdx] = sems[semIdx + 1];
        semIdx++;
    }

    qty--;

    freeMemory(sem);

    xchg(&privSem, 0);
    return 0;
}

static void removeFirstWaitingProc(Semaphore sem)
{
    for (int i = 0; i < sem->waitingProcQty; i++)
        sem->waitingProcs[i] = sem->waitingProcs[i + 1];

    sem->waitingProcQty--;
}

// P1 {pstate = 0}, P2 {pstate = 1}

int my_signal(Sem_T *sem)
{
    while (xchg(&sem->mutex, 1) != 0);

    sem->value++;

    if (sem->waitingProcQty > 0)
    {
        sem->waitingProcs[0]->pstate = READY;

        removeFirstWaitingProc(sem);
    }

    xchg(&sem->mutex, 0);
    return 0;
}

// retorna -1 sin hacer nada si se supera la cantidad de maxima de procesos esperando
int my_wait(Semaphore sem)
{
    while (xchg(&sem->mutex, 1) != 0)
        ;

    while (sem->value <= 0)
    {
        // el semaforo está tomado

        if (sem->waitingProcQty == MAX_PROCESSES)
            return -1;

        sem->waitingProcs[sem->waitingProcQty++] = blockCurrentProcess();
        xchg(&sem->mutex, 0);
        forceScheduler(); 

        while (xchg(&sem->mutex, 1) != 0)
            ;
    }

    sem->value--;

    xchg(&sem->mutex, 0);
    return 0;
}

void printSemaphores() {
  uint32_t index = 0;
      ncPrintStringColour("ID      Value       Blocked Processes\n", WHITE);
  while(index < qty) {

    ncPrintDec(sems[index]->id);
    ncPrintStringColour("         ", WHITE);
    ncPrintDec(sems[index]->value);
    ncPrintStringColour("               ", WHITE);
    if(sems[index]->waitingProcQty > 0) {
      ncPrintDec(sems[index]->waitingProcs[0]->pid);
      ncPrintStringColour("\n", WHITE);
      for(int i = 1; i < sems[index]->waitingProcQty; i++) {
       ncPrintStringColour("                          ", WHITE);
        ncPrintDec(sems[index]->waitingProcs[i]->pid);
       ncPrintStringColour("\n", WHITE);
      }
    } else {
     ncPrintStringColour("None", WHITE);
      ncPrintStringColour("\n", WHITE);
    }
    index++;
  }
}

static void deleteProcessFromSemaphore(Semaphore sem, uint32_t pid)
{
    for (int j = 0; j < sem->waitingProcQty; j++)
    {
        if (sem->waitingProcs[j]->pid == pid)
        {
            for (int k = 0; k < sem->waitingProcQty; k++)
                sem->waitingProcs[k] = sem->waitingProcs[k + 1];
            sem->waitingProcQty--;
            return;
        }
    }
}

void deleteProcessFromSemaphores(uint32_t pid)
{
    for (int i = 0; i < qty; i++)
        deleteProcessFromSemaphore(sems[i], pid);
}