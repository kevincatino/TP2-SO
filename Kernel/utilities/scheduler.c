
#include "../include/naiveConsole.h"
#include "../include/interrupts.h"
#include "../include/time.h"
#include <scheduler.h>

#define QUANTUM 1
#define PRIORITY_QUANTUM 5
#define MAX_WAITING_KEYBOARD 10
#define DEFAULT_PROGRAM_SIZE 4096

typedef struct ListNode
{
  pcb process;
  struct ListNode *next;
} ListNode;

typedef struct WaitingNode
{
  pcb *process;
  struct WaitingNode *next;
} WaitingNode;

typedef struct WaitingKeyboardList
{
  WaitingNode *current;
  WaitingNode *tail;
  uint8_t size;
} WaitingKeyboardList;

typedef struct Scheduler
{
  uint32_t quantum;
  uint32_t priorityQuantum;
  uint8_t fgTaken; // proceso en foreground
  ListNode *currentProcess;
  ListNode *start; // lista ordenada por prioridades, los mas prioritarios primero
} Scheduler;

Scheduler *scheduler;
uint8_t firstProcess;
uint32_t pid;
ListNode *dummy;
WaitingKeyboardList *waitingKBList;

/*
Un poco de background porque no me acordaba de nada: El Kernel en su main, luego de copiar todo el sampleCodeModule a la posicion 0x400000, primero inicializa el scheduler y luego crea el primer proceso que va a correr el main del sampleCodeModule. Este proceso es la shell, y es el unico proceso que se crea con prioridad = 1 dado que es la shell quien lo crea.
*/


static void uselessProcess()
{
  while (1);
}

void initScheduler()
{
  scheduler = (Scheduler *)alloc(sizeof(Scheduler));
  scheduler->currentProcess = NULL;
  scheduler->quantum = QUANTUM - 1;
  scheduler->priorityQuantum = PRIORITY_QUANTUM;
  scheduler->start = NULL;
  scheduler->fgTaken = 0;
  pid = 1;
  firstProcess = 1;

  // aloco memoria para el waitingKBList, y agrego la maxima cantidad de nodos en null a la lista.
  // El ultimo nodo que agrego es un puntero a waitingKBList->current para que sea circular.
  waitingKBList = (WaitingKeyboardList *)alloc(sizeof(WaitingKeyboardList));
  waitingKBList->current = (WaitingNode *)alloc(sizeof(WaitingNode));
  waitingKBList->current->process = NULL;
  waitingKBList->current->next = NULL;
  waitingKBList->tail = waitingKBList->current;
  waitingKBList->size = 0;
  WaitingNode *aux = waitingKBList->current;
  for (int i = 0; i < MAX_WAITING_KEYBOARD; i++)
  {
    WaitingNode *newNode = (WaitingNode *)alloc(sizeof(WaitingNode));
    newNode->process = NULL;
    newNode->next = NULL;
    aux->next = newNode;
    aux = aux->next;
  }
  aux->next = waitingKBList->current;

  uint64_t dummyMemory = (uint64_t)alloc(2048); /// 2048 bytes = 2K

  uint64_t sp = initProcess(dummyMemory + 2048, (uint64_t)&uselessProcess, 0, NULL);
  dummy = (ListNode *)alloc(sizeof(ListNode));
  dummy->process.pid = 0;
  dummy->process.sp = sp;
  dummy->process.processMemory = dummyMemory;
  dummy->process.pstate = 1;
  dummy->process.priority = 1;
  dummy->next = scheduler->start;
}

static char *copyString(char *destination, const char *source)
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

// Funcion auxiliar recursiva que crea y encola un proceso de prioridad 'priority'
static ListNode *loadProcess(ListNode *node, uint32_t pid, uint8_t priority, int argc, char args[6][21], uint64_t ip)
{

  // Si la lista de procesos esta vacia, creo el nodo, inicio el proceso y retorno el nodo como inicio de la lista
  if (node == NULL)
  {
    ListNode *newNode = (ListNode *)alloc(sizeof(ListNode));
    newNode->process.pid = pid;
    newNode->process.pstate = 1;
    newNode->process.priority = priority;
    for (int i = 0; i < argc; i++)
      copyString(newNode->process.args[i], args[i]);
    uint64_t processMemory = (uint64_t)alloc(DEFAULT_PROGRAM_SIZE);

    // initProcess es una funcion de assembler que inicia el proceso y devuelve el nuevo stackpointer. Los procesos por defecto son de 4K de tamanio.
    // todo: Usar y probar si anda forma de inicializar en una sola linea: ej newNode->p = {.sp = sp, etc.}
    uint64_t sp = initProcess(processMemory + DEFAULT_PROGRAM_SIZE, ip, argc, newNode->process.args);
    newNode->process.sp = sp;
    newNode->process.bp = processMemory + DEFAULT_PROGRAM_SIZE - 1;
    newNode->process.processMemory = processMemory;
    return newNode;
  }

  // Llamada recursiva: si estoy en un nodo de mayor prioridad que 'priority', tengo que encolar el proceso mas adelante
  if (node->next != NULL && priority >= node->next->process.priority)
  {
    node->next = loadProcess(node->next, pid, priority, argc, args, ip);
    return node;
  }

  // O bien estoy en el final de la lista, o llegue a un nodo con menor prioridad, en ambos casos tengo que crear el proceso y encolarlo en este punto

  ListNode *newNode = (ListNode *)alloc(sizeof(ListNode));

  newNode->next = node->next;
  node->next = newNode;

  newNode->process.pid = pid;
  newNode->process.pstate = 1; // por defecto pstate = 1 = ready
  newNode->process.priority = priority;
  // Copio los argumentos al nodo creado
  for (int i = 0; i < argc; i++)
    copyString(newNode->process.args[i], args[i]);
  uint64_t processMemory = (uint64_t)alloc(DEFAULT_PROGRAM_SIZE);
  uint64_t sp = initProcess(processMemory + DEFAULT_PROGRAM_SIZE, ip, argc, newNode->process.args);
  newNode->process.sp = sp;
  newNode->process.bp = processMemory + DEFAULT_PROGRAM_SIZE - 1;
  newNode->process.processMemory = processMemory;

  return node;
}

// Crea un proceso de prioridad 'priority' con ciertos argumentos
int createProcess(uint64_t ip, uint8_t priority, uint64_t argc, char argv[6][21])
{
  // El scheduler pasa a obtener el foreground si la prioridad del nuevo proceso es maxima y tengo mas procesos ademas de la shell
  if (priority == 1 && pid > 1)
    scheduler->fgTaken = 1;
  int thisPid = pid;
  scheduler->start = loadProcess(scheduler->start, pid++, priority, argc, argv, ip);
  return thisPid;
}

int createProcessWrapper(uint64_t ip, uint8_t priority, uint64_t argc, char *argv)
{
  if (priority <= 1 || priority > 20)
    return -1;

  int i = 0, j = 0;
  char args[6][21];
  while (i < argc)
  {
    int k = 0;
    while (argv[j])
    {
      args[i][k] = argv[j];
      j++;
      k++;
    }
    args[i][k] = 0;
    j++;
    i++;
  }

  return createProcess(ip, priority, argc, args);
}

// Es la funcion llamada desde assembler cada vez que ocurre una interrupcion de cualquier tipo (incluyendo timer tick). Recibe el stack pointer para saber desde donde retomar luego el contexto
uint64_t switchProcess(uint64_t sp)
{

  // Para el primer proceso no actualizamos el sp, usamos el hardcodeado
  if (firstProcess)
  {
    firstProcess = 0;
    scheduler->currentProcess = scheduler->start;
    return scheduler->start->process.sp;
  }
  // Si faltan ticks por correr, disminuimos el quantum y seguimos en el mismo proceso
  if (scheduler->quantum > 0)
  {
    scheduler->quantum--;
    return 0;
  }

  // Si llegue aca, el proceso ya termino de ejecutar su quantum completo y tengo que elegir un nuevo proceso a correr

  // Como ya terminó su quantum, lo reiniciamos
  scheduler->quantum = QUANTUM - 1;

  // guardar el sp del proceso actual en su PCB. Al hacer context switch tiene que reanudar desde ese mismo punto
  scheduler->currentProcess->process.sp = sp;

  if (scheduler->currentProcess->next == NULL)
    scheduler->currentProcess = scheduler->start;
  else
    scheduler->currentProcess = scheduler->currentProcess->next;


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
    free((void *)node->process.processMemory);
    free((void *)node);
    return aux;
  }

  node->next = deleteProcess(node->next, pid);
  return node;
}

void exitCurrentProcess()
{
  if (scheduler->currentProcess->process.priority == 1)
    scheduler->fgTaken = 0;

  scheduler->start = deleteProcess(scheduler->start, scheduler->currentProcess->process.pid);
}

void killPid(uint32_t pid)
{
  if (pid > 1)
    scheduler->start = deleteProcess(scheduler->start, pid);
}

void printProcessList()
{
  ncPrint("Name    PID    Priority     SP          BP         Type         State\n");
  ListNode *aux = scheduler->start;
  while (aux != NULL)
  {
    ncPrint(aux->process.args[0]);
    for (int i = ncStrlen(aux->process.args[0]); i < 9; i++)
      ncPrint(" ");
    ncPrintDec(aux->process.pid);
    ncPrint("        ");
    ncPrintDec(aux->process.priority);
    ncPrint("        ");
    ncPrintHex(aux->process.sp);
    ncPrint("     ");
    ncPrintHex(aux->process.bp);
    ncPrint("     ");
    ncPrint(aux->process.priority == 1 ? "Foreground" : "Background");
    ncPrint("     ");
    ncPrint(aux->process.pstate ? "Ready" : "Blocked");
    ncNewline();
    aux = aux->next;
  }
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

// Cambia la prioridad de un proceso y lo reordena en la lista de prioridades
void changeProcessPriority(uint32_t pid, uint8_t newPriority)
{
  if (pid <= 1)
  {
    ncPrint("Cannot change shell priority\n");
    return;
  }
  if (newPriority <= 1 || newPriority > 10)
  {
    ncPrint("Priority must be between 2 and 10");
    return;
  }

  pcb *pidPCB = getPCB(scheduler->start, pid);
  if (pidPCB == NULL)
    return;

  pidPCB->priority = newPriority;

  ListNode *headPID = scheduler->start;
  while (headPID->next->process.pid != pid)
    headPID = headPID->next;

  ListNode *process = headPID->next;
  headPID->next = process->next;

  headPID = scheduler->start;
  while (headPID->next != NULL && newPriority >= headPID->next->process.priority)
    headPID = headPID->next;

  ListNode *aux = headPID->next;
  headPID->next = process;
  process->next = aux;
}

// Cambia el estado del proceso de ready a sleep o viceversa
void changeProcessState(uint32_t pid)
{
  if (pid <= 1)
    return;

  pcb *pidPCB = getPCB(scheduler->start, pid);
  if (pidPCB == NULL)
    return;

  if (pidPCB->pstate == 1)
  {
    pidPCB->pstate = 0;
    if (pid == scheduler->currentProcess->process.pid)
      forceScheduler();
  }
  else if (pidPCB->pstate == 0)
    pidPCB->pstate = 1;
}

void waitForKeyboard()
{
  scheduler->currentProcess->process.pstate = 0;
  waitingKBList->tail->process = &scheduler->currentProcess->process;
  waitingKBList->tail = waitingKBList->tail->next;
  waitingKBList->size++;
  forceScheduler();
}

void awakeKeyboardQueue()
{
  if (waitingKBList->size == 0)
    return;
  waitingKBList->size--;
  waitingKBList->current->process->pstate = 1;
  waitingKBList->current = waitingKBList->current->next;
}

pcb *blockCurrentProcess()
{
  scheduler->currentProcess->process.pstate = 0;
  return &scheduler->currentProcess->process;
}

uint32_t getCurrentPid()
{
  return scheduler->currentProcess->process.pid;
}

pcb *getCurrentProcess()
{
  return &scheduler->currentProcess->process;
}