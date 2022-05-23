
#include "../include/naiveConsole.h"
#include "../include/interrupts.h"
#include "../include/time.h"
#include "../include/semaphore.h"
#include <scheduler.h>

#define QUANTUM 2
#define PRIORITY_QUANTUM 5
#define MAX_WAITING_KEYBOARD 10
#define DEFAULT_PROGRAM_SIZE 4096

typedef struct ListNode
{
  pcb process;
  struct ListNode *next;
} ListNode;

typedef struct Scheduler
{
  ListNode *currentProcess;
  ListNode *start; // lista ordenada por prioridades, los mas prioritarios primero
} Scheduler;

// start: cursorProcess -> shellProcess -> process3

typedef struct KBNode
{
  pcb *process;
  struct KBNode *next;
} KBNode;

typedef struct WaitingForKBList
{
  KBNode *list;
  KBNode *last;
  uint8_t size;
} WaitingForKBList;

Scheduler *scheduler;
uint8_t firstProcess;
uint32_t pid;
ListNode *dummy;
WaitingForKBList *kbList;

/*
Un poco de background porque no me acordaba de nada: El Kernel en su main, luego de copiar todo el sampleCodeModule a la posicion 0x400000, primero inicializa el scheduler y luego crea el primer proceso que va a correr el main del sampleCodeModule. Este proceso es la shell, y es el unico proceso que se crea con prioridad = 1 dado que es el kernel quien lo crea.
*/

static void cursorProcess()
{
  while (1)
  {
    if (ticks_elapsed() % 5 == 0)
    {
      displayCursor();
      _hlt();
    }
  }
}

static int getQuantum(uint8_t priority)
{
  switch (priority)
  {
  case 1:
    return 5;
  case 2:
    return 4;
  case 3:
    return 3;
  case 4:
    return 2;
  case 5:
    return 1;
  }
  return 1;
}

static char *stringCopy(char *destination, const char *source)
{
  if (destination == NULL)
    return NULL;

  char *ptr = destination;

  while (*source != '\0')
  {
    *destination = *source;
    destination++;
    source++;
  }

  *destination = '\0';
  return ptr;
}

static void setBlockPriority(pcb *block, uint8_t newPriority)
{
  block->priority = newPriority;
  block->quantum = getQuantum(newPriority);
}

static int8_t userValidPid(int8_t pid)
{
  return pid >= 2;
}

static int8_t userValidPriority(int8_t priority)
{
  return priority >= 2 && priority <= 5;
}

void initScheduler()
{
  scheduler = (Scheduler *)allocMemory(sizeof(Scheduler));
  scheduler->currentProcess = NULL;
  scheduler->start = NULL;
  pid = 1;
  firstProcess = 1;

  kbList = (WaitingForKBList *)allocMemory(sizeof(WaitingForKBList));
  kbList->size = 0;

  uint64_t dummyMemory = (uint64_t)allocMemory(2048); /// 2048 bytes = 2K

  uint64_t sp = initProcess(dummyMemory + 2048, (uint64_t)&cursorProcess, 0, NULL);
  dummy = (ListNode *)allocMemory(sizeof(ListNode));

  scheduler->start = dummy; // Arrancamos con el proceso dummy

  dummy->process.pid = 0;
  dummy->process.sp = sp;
  stringCopy(dummy->process.args[0], "cursor");
  dummy->process.processMemory = dummyMemory;
  dummy->process.pstate = 1;
  setBlockPriority(&dummy->process, 2);
  dummy->next = NULL;
}

// Funcion auxiliar recursiva que crea y encola un proceso de prioridad 'priority'
static ListNode *loadProcess(ListNode *node, uint32_t pid, uint8_t priority, int argc, char args[6][ARG_LENGTH], uint64_t ip, fd * stdin, fd * stdout)
{

  // Si la lista de procesos esta vacia, creo el nodo, inicio el proceso y retorno el nodo como inicio de la lista
  if (node == NULL)
  {

    ListNode *newNode = (ListNode *)allocMemory(sizeof(ListNode));
    newNode->process.pid = pid;
    newNode->process.pstate = 1;
    newNode->process.priority = priority;
    newNode->process.quantum = getQuantum(priority);
    for (int i = 0; i < argc; i++) {
      stringCopy(newNode->process.args[i], args[i]);
      newNode->process.argv[i] = newNode->process.args[i];
    }
      
    uint64_t processMemory = (uint64_t)allocMemory(DEFAULT_PROGRAM_SIZE);

    // initProcess es una funcion de assembler que inicia el proceso y devuelve el nuevo stackpointer. Los procesos por defecto son de 4K de tamanio.
    // todo: Usar y probar si anda forma de inicializar en una sola linea: ej newNode->p = {.sp = sp, etc.}
    uint64_t sp = initProcess(processMemory + DEFAULT_PROGRAM_SIZE, ip, argc, newNode->process.argv);
    newNode->process.sp = sp;
    newNode->process.bp = processMemory + DEFAULT_PROGRAM_SIZE - 1;
    newNode->process.processMemory = processMemory;
    newNode->process.stdin = stdin;
    newNode->process.stdout = stdout;

    // ncPrint("stdout es ");
    // ncPrintDec(stdout);

    return newNode;
  }

  // Llamada recursiva: si estoy en un nodo de mayor prioridad que 'priority', tengo que encolar el proceso mas adelante
  if (node->next != NULL)
  {
    node->next = loadProcess(node->next, pid, priority, argc, args, ip, stdin, stdout);
    return node;
  }



  // O bien estoy en el final de la lista, o llegue a un nodo con menor prioridad, en ambos casos tengo que crear el proceso y encolarlo en este punto

  ListNode *newNode = (ListNode *)allocMemory(sizeof(ListNode));

  newNode->next = node->next;
  node->next = newNode;

  newNode->process.pid = pid;
  newNode->process.pstate = 1; // por defecto pstate = 1 = ready
  newNode->process.priority = priority;
  newNode->process.quantum = getQuantum(priority);
  // Copio los argumentos al nodo creado
  for (int i = 0; i < argc; i++) {
    stringCopy(newNode->process.args[i], args[i]);
    newNode->process.argv[i] = newNode->process.args[i];
  }
  

  uint64_t processMemory = (uint64_t)allocMemory(DEFAULT_PROGRAM_SIZE);
  uint64_t sp = initProcess(processMemory + DEFAULT_PROGRAM_SIZE, ip, argc, newNode->process.argv);
  newNode->process.sp = sp;
  newNode->process.bp = processMemory + DEFAULT_PROGRAM_SIZE - 1;
  newNode->process.processMemory = processMemory;
      newNode->process.stdin = stdin;
    newNode->process.stdout = stdout;

  return node;
}



// Crea un proceso de prioridad 'priority' con ciertos argumentos
uint32_t createProcess(uint64_t ip, uint8_t priority, uint64_t argc, char argv[6][ARG_LENGTH], fd * stdin, fd * stdout)
{
  // El scheduler pasa a obtener el foreground si la prioridad del nuevo proceso es maxima y tengo mas procesos ademas de la shell
  int thisPid = pid;

  scheduler->start = loadProcess(scheduler->start, pid++, priority, argc, argv, ip, stdin, stdout);

  return thisPid;
}

uint32_t createProcessForUser(uint64_t ip, uint8_t priority, uint64_t argc, char *argv[], fd * stdin, fd * stdout)
{
  if (!userValidPriority(priority))
    return -1; // el usuario no puede crear procesos con prioridad menor a 2 o mayor a 5

  return createProcessWrapper(ip, priority, argc, argv, stdin, stdout);
}

// Funcion auxiliar que copia el vector de argumentos a un arreglo (no hacemos malloc, hay limitaciones en la longitud y cantidad de argumentos)
uint32_t createProcessWrapper(uint64_t ip, uint8_t priority, uint64_t argc, char *argv[], fd * stdin, fd * stdout)
{
  int i = 0;
  char args[6][ARG_LENGTH];

  while (i < argc)
  {
    // ncPrint(argv[i]);
    stringCopy(args[i], argv[i]);
    i++;
  }

  return createProcess(ip, priority, argc, args, stdin, stdout);
}

// Es la funcion llamada desde assembler cada vez que ocurre una interrupcion de cualquier tipo (incluyendo timer tick). Recibe el stack pointer para saber desde donde retomar luego el contexto
uint64_t switchProcess(uint64_t sp)
{

  if (scheduler->start == NULL)
    return 0;

  // Para el primer proceso usamos el stack pointer hardcodeado porque no hay punto del cual resumir
  if (firstProcess)
  {
    firstProcess = 0;
    scheduler->currentProcess = scheduler->start;
    return scheduler->start->process.sp;
  }

  // ncPrintDec(scheduler->currentProcess->process.pid);

  // Si faltan ticks por correr, disminuimos el quantum y seguimos en el mismo proceso
  if (scheduler->currentProcess->process.quantum > 0 && scheduler->currentProcess->process.pstate == 1)
  {
    scheduler->currentProcess->process.quantum--;
    return 0;
  }

  // Si llegue aca, el proceso ya termino de ejecutar su quantum completo y tengo que elegir un nuevo proceso a correr

  // Como ya terminó su quantum, lo reiniciamos
  scheduler->currentProcess->process.quantum = getQuantum(scheduler->currentProcess->process.priority);

  // guardar el sp del proceso actual en su PCB. Al hacer context switch tiene que reanudar desde ese mismo punto

  scheduler->currentProcess->process.sp = sp;

  int foundNext = 0;

  // Buscamos el proximo proceso ready (si el siguiente es la shell o el cursor, ni siquiera chequeamos si esta ready o no, siempre lo estan)
  while (!foundNext)
  {
    if (scheduler->currentProcess->next == NULL)
    {
      scheduler->currentProcess = scheduler->start;
    }
    else
    {
      scheduler->currentProcess = scheduler->currentProcess->next;
    }

    if (scheduler->currentProcess->process.pid < 1 || scheduler->currentProcess->process.pstate == 1)
      foundNext = 1;
  }

  return scheduler->currentProcess->process.sp;
}

static ListNode *deleteProcess(ListNode *node, uint32_t pid)
{

  if (node == NULL)
    return node;

  if (node->process.pid == pid)
  {
    node->process.pstate = 2;
    ListNode *aux = node->next;
    deleteProcessFromSemaphores(pid);
    deleteProcessFromPipes(pid);
    closeFd(node->process.stdin);
    closeFd(node->process.stdout);
    freeMemory((void *)node->process.processMemory);
    freeMemory((void *)node);

    return aux;
  }

  node->next = deleteProcess(node->next, pid);
  return node;
}

void exitCurrentProcess()
{
  killPid(scheduler->currentProcess->process.pid);
}

void killPid(uint32_t pid)
{
  if (pid > 1) {
    uint32_t currentPid = scheduler->currentProcess->process.pid;
    scheduler->start = deleteProcess(scheduler->start, pid);

      if (currentPid == pid) {
        forceScheduler();
    }
  }
    
}

void printProcessList()
{
  // todo: implementar
}

// Retorna el contexto de un proceso a partir de su pid
static pcb *getPCB(ListNode *node, uint32_t pid)
{
  if (node == NULL)
    return NULL;

  if (node->process.pid == pid)
    return &node->process;

  return getPCB(node->next, pid);
}

// Cambia el estado del proceso de ready a sleep o viceversa
void changeProcessState(uint32_t pid, int state)
{
  if (pid <= 1)
    return;

  pcb *pidPCB = getPCB(scheduler->start, pid);

  if (pidPCB == NULL)
    return;

  if (pidPCB->pstate == 1 && (state == 0 || state == -1))
  {
    pidPCB->pstate = 0;
    if (pid == scheduler->currentProcess->process.pid)
      forceScheduler();
  }
  else if (pidPCB->pstate == 0 && (state == 1 || state == -1))
    pidPCB->pstate = 1;
}

// Cambia la prioridad de un proceso y lo reordena en la lista de prioridades
void changeProcessPriority(uint32_t pid, uint8_t newPriority)
{
  pcb *block = getPCB(scheduler->start, pid);
  setBlockPriority(block, newPriority);
}

void changeProcessPriorityForUser(uint32_t pid, uint8_t newPriority)
{
  if (!userValidPriority(newPriority))
    return;

  changeProcessPriority(pid, newPriority);
}

void changeProcessStateForUser(uint32_t pid, int state)
{
  if (!userValidPid(pid) || (state != READY && state != BLOCKED && state != -1))
    return;

  changeProcessState(pid, state);
}

uint32_t getCurrentPid()
{
  return scheduler->currentProcess->process.pid;
}

pcb *getCurrentProcess()
{
  return &scheduler->currentProcess->process;
}

void getProcessIntoKBQueue()
{
  scheduler->currentProcess->process.pstate = 0;

  KBNode *aux = (KBNode *)allocMemory(sizeof(KBNode));
  aux->process = &scheduler->currentProcess->process;
  aux->next = NULL;

  if (kbList->size == 0)
  {
    kbList->list = aux;
    kbList->last = aux;
  }
  else
  {
    kbList->last->next = aux;
    kbList->last = aux;
  }
  kbList->size++;
  forceScheduler();
}

void awakeProcessFromKBQueue()
{
  if (kbList->size == 0)
    return;
  // ncPrintDec(kbList->size);
  // ncPrint("Awakening ");

  kbList->size--;
  kbList->list->process->pstate = READY;
  KBNode * aux = kbList->list;
  kbList->list = kbList->list->next;
  freeMemory(aux);
}

pcb *blockCurrentProcess()
{
  scheduler->currentProcess->process.pstate = BLOCKED;
  return &scheduler->currentProcess->process;
}

fd *getStdin()
{
  return scheduler->currentProcess->process.stdin;
}

fd *getStdout()
{
  return scheduler->currentProcess->process.stdout;
}