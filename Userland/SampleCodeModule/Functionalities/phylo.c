
//  basado en https://www.geeksforgeeks.org/dining-philosopher-problem-using-semaphores/

#include "../include/libc.h"
#include "phylo.h"
#include "test_util.h"

#define MAX_PH 20
#define EAT_SECONDS 1

#define THINKING 0
#define EATING 1
#define HUNGRY 2


typedef struct
{
  int state;
  int pid;
} Phylo;

static int deletePhylo(int index);

static void printState();

static void phyloProcess(int argc, char * argv[]);

static void takeChopsticks(int index);

static void putChopsticks(int index);

static void checkAvailability(int index);

static int left(int index);

static int right(int index);

static void finishEating();

static Phylo phylos[MAX_PH]; 
static int phyloQty; 
Semaphore mutexSem;  
Semaphore phyloSem[MAX_PH];

static int addPhylo(int index) {
  if(index >= MAX_PH)
    return -1;
  
  sys_sem_wait(mutexSem);


  phylos[index].state = THINKING;
  phyloSem[index] = sys_sem_open(index+100, 1);
  if(phyloSem[index] == NULL)
    return -1;

  char seatIdx[3];
  uintToBase(index, seatIdx, 10);
  char * argv[] = {"phylo", seatIdx, "&"};
  
  phylos[index].pid = sys_create_process((uint64_t)&phyloProcess, 2, 3, argv, STDIN, STDOUT);
  phyloQty++;
  sys_sem_signal(mutexSem);
  return 0;
}

static void phyloProcess(int argc, char * argv[]) {
  int seat = satoi((char *)argv[1]);
  while (1)
  {
    sleep(EAT_SECONDS);
    takeChopsticks(seat);
    sleep(EAT_SECONDS);
    putChopsticks(seat);
  }
}


static void takeChopsticks(int index) {
  sys_sem_wait(mutexSem);

  phylos[index].state = HUNGRY;
  checkAvailability(index);

  sys_sem_signal(mutexSem);
  sys_sem_wait(phyloSem[index]);

}


static void putChopsticks(int index) {
  sys_sem_wait(mutexSem);

  phylos[index].state = THINKING;

  checkAvailability(left(index));
  checkAvailability(right(index));

  sys_sem_signal(mutexSem);
}

static void checkAvailability(int index) {
  if(phylos[index].state == HUNGRY && phylos[left(index)].state != EATING &&  phylos[right(index)].state != EATING) {
    phylos[index].state = EATING;
    printState();

    sys_sem_signal(phyloSem[index]);
  }

}

static int left(int index) {
  return (index == 0) ? phyloQty - 1 : index - 1;
}

static int right(int index) {
  return (index == phyloQty -1) ? 0 : index + 1;
}


static int deletePhylo(int index) {
  if(index < 0 || phyloQty == 0)
    return -1;

  sys_sem_wait(mutexSem);

  int eating = (phylos[index].state == EATING);

  if (sys_sem_close(phyloSem[index]) == -1) {
    print("Error closing phylo's sem");
    return -1;
  }

  sys_killPid(phylos[index].pid);
  phyloQty--;

  if(eating) {
    checkAvailability(index - 1);
    checkAvailability(0);
  }

  sys_sem_signal(mutexSem);
  return 0;
}

static void finishEating() {
  while (phyloQty > 0) 
  {
    if (sys_sem_close(phyloSem[phyloQty - 1]) == -1)
      print("Error closing phylo's sem");

    sys_killPid(phylos[phyloQty - 1].pid);

    phyloQty--;
  }
}

static void printState() {
  for (int i = 0; i < phyloQty; i++)
  {
    phylos[i].state == EATING ? print(" E ") : print(" . ");
  }
  print("\n");
}

void phylo(int argc, char * argv[]) {
  print("Bienvenidos al juego de los filosofos!\n");
  mutexSem = sys_sem_open(30, 1);

  phyloQty = 0;

  print(" 'a' para agregar - 'r' para remover filosofos - 'q' para salir\n");
  char c;
  while( (c = getChar()) != 0 ) 
  {
    switch (c)
    {
    case 'a':
      if(addPhylo(phyloQty) == -1)
        print("Error agregando filosofo\n");
      print("\n");
      break;
    case 'r':
      if(deletePhylo(phyloQty - 1) == -1)
        print("Error borrando filosofo\n");
      print("\n");
      break;
    case 'q':
      print("\n");
      finishEating();
      sys_sem_close(mutexSem);
      sys_exit();
      return;
    }
  }
  print("Bye\n");
  finishEating();
  sys_sem_close(mutexSem);
  sys_exit();
}