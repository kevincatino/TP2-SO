#include <test_util.h>
#include <libc.h>
#include <test_lib.h>

#define MAX_MEMORY 209715200

uint64_t test_mm(uint64_t argc, char *argv[])
{

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;

  if (argc != 2)
  {
    print("The percentage of heap used is neeced as parameter\n");
    return -1;
  }

  max_memory = (uint64_t)(satoi(argv[1])) * (MAX_MEMORY) / 100;

  if ((max_memory) <= 0)
  {
    print("Memory value is invalid\n");
    return -1;
  }

  print("Testing memory manager with max size = ");
  print(argv[1]);
  print("\%\n");
  // return;
  // while (1){
  rq = 0;
  total = 0;

  // Request as many blocks as we can

  while (rq < MAX_BLOCKS && total < max_memory)
  {
    mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
    void *aux = sys_alloc(mm_rqs[rq].size);
    mm_rqs[rq].address = aux;

    if (aux == NULL)
    {
      print("Obtained NULL pointer. Aborting\n");
      int i;
      for (i = 0; i < rq; i++)
        if (mm_rqs[i].address)
          sys_free(mm_rqs[i].address);
      return 1;
    }

    if (mm_rqs[rq].address)
    {
      total += mm_rqs[rq].size;
      rq++;
    }
  }

  // Set
  uint32_t i;
  for (i = 0; i < rq; i++)
    if (mm_rqs[i].address)
      memset(mm_rqs[i].address, i, mm_rqs[i].size);

  // Check
  for (i = 0; i < rq; i++)
    if (mm_rqs[i].address)
      if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
      {
        print("test_mm ERROR\n");
        return -1;
      }

  // Free
  for (i = 0; i < rq; i++)
    if (mm_rqs[i].address)
      sys_free(mm_rqs[i].address);

  // }
  print("Tests passed!\n");
  return 0;
}

enum State
{
  RUNNING,
  BLOCK,
  KILLED
};

typedef struct P_rq
{
  int32_t pid;
  enum State state;
} p_rq;

int64_t test_processes(uint64_t argc, char *argv[])
{
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;
  char *argvAux[] = {0};

  if (argc != 2) {
    print("The number of processes is needed as parameter\n");
    return -1;
  }

  if ((max_processes = satoi(argv[1])) <= 0)
    return -1;

  p_rq p_rqs[max_processes];

  // while (1){

  // Create max_processes processes
  for (rq = 0; rq < max_processes; rq++)
  {
    char *args[] = {"testLoop", "3"};
    p_rqs[rq].pid = sys_create_process(loop, 3, 2, args, STDIN, STDOUT);
    print("Created process: ");
    printNum(p_rqs[rq].pid);
    print("\n");

    if (p_rqs[rq].pid == -1)
    {
      print("test_processes: ERROR creating process\n");
      return -1;
    }
    else
    {
      p_rqs[rq].state = RUNNING;
      alive++;
    }
  }

  // Randomly kills, blocks or unblocks processes until every one has been killed
  while (alive > 0)
  {

    for (rq = 0; rq < max_processes; rq++)
    {
      action = GetUniform(100) % 2;

      switch (action)
      {
      case 0:
        if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCK)
        {
          print("Killing process: ");
          printNum(p_rqs[rq].pid);
          print("\n");
          sys_killPid(p_rqs[rq].pid);
          p_rqs[rq].state = KILLED;
          alive--;
        }
        break;

      case 1:
        if (p_rqs[rq].state == RUNNING)
        {
          if (sys_change_process_state(p_rqs[rq].pid, BLOCKED) == -1)
          {
            print("test_processes: ERROR blocking process\n");
            return -1;
          }
          print("Blocking process ");
          printNum(p_rqs[rq].pid);
          print("\n");
          p_rqs[rq].state = BLOCK;
        }
        break;
      }
    }

    // Randomly unblocks processes
    for (rq = 0; rq < max_processes; rq++)
      if (p_rqs[rq].state == BLOCK && GetUniform(100) % 2)
      {
        if (sys_change_process_state(p_rqs[rq].pid, READY) == -1)
        {
          print("test_processes: ERROR unblocking process\n");
          return -1;
        }
        p_rqs[rq].state = RUNNING;
      }
  }
  print("Following table should not have processes called 'testLoop'\n");
  sys_ps();
  // }
  return 0;
}

#include <stdint.h>
#include <stdio.h>
#include "test_util.h"
#include "syscall.h"
#include <libc.h>
#include <stdint.h>

#define TOTAL_PAIR_PROCESSES 2

void testNoSync();
void testSync();

#define SEM_ID 10

int64_t global;
int64_t iterations = 0;

void slowInc(int64_t *p, int64_t inc)
{
  int64_t aux = *p;
  aux += inc;
  sys_yield();
  *p = aux;
}

void inc(int argc, char *argv[])
{
  int64_t useSem = satoi(argv[1]);
  int64_t value = satoi(argv[2]);
  int64_t N = satoi(argv[3]);
  int64_t i = 0;

  Semaphore sem;

  if (useSem && (sem = sys_sem_open(SEM_ID, 1)) == NULL)
  {
    print("ERROR OPENING SEM\n");
    sys_exit();
  }

  for (i = 0; i < N; i++)
  {
    if (useSem && sys_sem_wait(sem) != 0)
    {
      print("ERROR WAITING SEM\n");
    }
    slowInc(&global, value);
    if (useSem && sys_sem_signal(sem) != 0)
    {
      print("ERROR POSTING SEM\n");
    }
    sys_yield();
  }

  if (useSem)
  {
    sys_sem_close(sem);
  }

  print("Final value: ");
  printNum(global);
  print("\n");

  sys_exit();
}

void testSync()
{
  uint64_t i;

  global = 0;

  print("CREATING PROCESSES...(SEM)\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    char *argv1[] = {"inc process with sem", "1", "1", "100"};
    sys_create_process(&inc, 4, 4, argv1, STDIN, STDOUT);
    char *argv2[] = {"inc process with sem", "1", "-1", "100"};
    sys_create_process(&inc, 4, 4, argv2, STDIN, STDOUT);
  }
}

void testNoSync()
{
  uint64_t i;

  global = 0;

  print("CREATING PROCESSES...(NO SEM)\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
  {
    char *argv1[] = {"inc process without sem", "0", "1", "100"};
    sys_create_process(&inc, 4, 4, argv1, STDIN, STDOUT);
    char *argv2[] = {"inc process without sem", "0", "-1", "100"};
    sys_create_process(&inc, 4, 4, argv2, STDIN, STDOUT);
  }
}

#include <stdint.h>
#include <stdio.h>
#include "test_util.h"
#include "syscall.h"

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 100000000000  // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 5  // TODO: Change as required
#define MEDIUM 3  // TODO: Change as required
#define HIGHEST 2 // TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void customLoop(int argc, char *argv[])
{
  uint64_t ticks = sys_get_ticks();

  while (1)
  {
    uint64_t newTicks = sys_get_ticks();
    if (newTicks > ticks)
    {
      print(argv[1]);
      ticks = newTicks;
    }
  }
}

void test_prio(uint64_t argc, char * argv[])
{
  int64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  char buf[TOTAL_PROCESSES][10] = {0};
  char *args[TOTAL_PROCESSES][2] = {0};


  for (i = 0; i < TOTAL_PROCESSES; i++)
  {
    numToStr(i, 0, buf[i]);
    // print(buf[i]);
    args[i][0] = "testLoop";
    args[i][1] = buf[i];

    pids[i] = sys_create_process((uint64_t)customLoop, LOWEST, 2, args[i], STDIN, STDOUT);
  }

  sleep(5);
  print("\nCHANGING PRIORITIES TO ESCALATING VALUES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_change_process_priority(pids[i], prio[i % 3]);

  sleep(5);

  print("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_change_process_state(pids[i], BLOCKED);

  print("CHANGING PRIORITIES TO SAME VALUE WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_change_process_priority(pids[i], MEDIUM);

  print("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_change_process_state(pids[i], READY);

  sleep(5);

  print("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_killPid(pids[i]);

  print("Success!\n");

  sys_exit();
}
