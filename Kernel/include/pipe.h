#include "./scheduler.h"

#define DATA_SIZE 512
#define PIPES_MAXQTY 20
#define WRITE 1
#define READ 0


typedef struct fd * FileDes;

FileDes createFd();

int createPipe(FileDes f0, FileDes f1);

int openPipeId(FileDes fd, uint32_t id, int readwrite);

void closeFd(FileDes fd);

int pipeWrite(FileDes fd, char *string);

int pipeRead(FileDes fd, char *buffer, int limit);

void deleteProcessFromPipes(uint32_t pid);

void printPipes();

