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

void slowInc(int64_t *p, int64_t inc) {
  int64_t aux = *p;
  aux += inc;
  sys_yield();
  *p = aux;
}

void inc(int argc, char *argv[]) {

  // print(argv[0]);
  // print(argv[1]);
  // print(argv[2]);
  // print(argv[3]);
  int64_t useSem = satoi(argv[1]);
  int64_t value = satoi(argv[2]);
  int64_t N = satoi(argv[3]);
  int64_t i = 0;

  Semaphore sem;

  if (useSem && (sem = sys_sem_open(SEM_ID, 1)) == NULL) {
    print("ERROR OPENING SEM\n");
    sys_exit();
  }

  for (i = 0; i < N; i++) {
    if (useSem && sys_sem_wait(sem) != 0) {
      print("ERROR WAITING SEM\n");
    }
    slowInc(&global, value);
    if (useSem && sys_sem_signal(sem) != 0) {
      print("ERROR POSTING SEM\n");
    }
    sys_yield();
  }

  if (useSem) {
    sys_sem_close(sem);
  }

  print("Final value: ");
  printNum(global);
  print("\n");

  sys_exit();
  
}

void testSync() {
  uint64_t i;

  global = 0;

  print("CREATING PROCESSES...(SEM)\n");

  for (i = 0; i <TOTAL_PAIR_PROCESSES; i++) {
    char *argv1[] = {"inc process with sem", "1", "1", "100"};
    sys_create_process(&inc, 4, 4, argv1, STDIN, STDOUT);
    char *argv2[] = {"inc process with sem", "1", "-1", "100"};
    sys_create_process(&inc, 4, 4, argv2, STDIN, STDOUT);
  }
}

void testNoSync() {
  uint64_t i;

  global = 0;

  print("CREATING PROCESSES...(NO SEM)\n");

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    char *argv1[] = {"inc process without sem", "0", "1", "100"};
    sys_create_process(&inc, 4, 4, argv1, STDIN, STDOUT);
    char *argv2[] = {"inc process without sem", "0", "-1", "100"};
   sys_create_process(&inc, 4, 4, argv2, STDIN, STDOUT);
  }
}

// int64_t global;  //shared memory

// void slowInc(int64_t *p, int64_t inc){
//   uint64_t aux = *p;
//   // my_yield(); //This makes the race condition highly probable
//   aux += inc;
//   *p = aux;
// }

// uint64_t my_process_inc(uint64_t argc, char *argv[]){
//   uint64_t n;
//   uint8_t inc;
//   int8_t use_sem;

//   if (argc != 3) return -1;

//   if ((n = satoi(argv[0])) <= 0) return -1;
//   if ((inc = satoi(argv[1])) == 0) return -1;
//   if ((use_sem = satoi(argv[2])) < 0) return -1;

//   if (use_sem)
//     if (!my_sem_open(SEM_ID, 1)){
//       print("test_sync: ERROR opening semaphore\n");
//       return -1;
//     }

//   uint64_t i;
//   for (i = 0; i < n; i++){
//     if (use_sem) my_sem_wait(SEM_ID);
//     slowInc(&global, inc);
//     if (use_sem) my_sem_post(SEM_ID);
//   }

//   if (use_sem) my_sem_close(SEM_ID);
  
//   return 0;
// }

// uint64_t test_sync(uint64_t argc, char *argv[]){ //{n, use_sem, 0}
//   uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

//   if (argc != 2) return -1;

//   char * argvDec[] = {argv[0], "-1", argv[1], NULL};
//   char * argvInc[] = {argv[0], "1", argv[1], NULL};

//   global = 0;

//   uint64_t i;
//   for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
//     pids[i] = my_create_process("my_process_inc", 3, argvDec);
//     pids[i + TOTAL_PAIR_PROCESSES] = my_create_process("my_process_inc", 3, argvInc);
//   }

//   for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
//     my_wait(pids[i]);
//     my_wait(pids[i + TOTAL_PAIR_PROCESSES]);
//   }

//   // print("Final value: %d\n");

//   return 0;
// }
