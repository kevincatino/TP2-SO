// #include "../include/pipe.h"
#include "../include/naiveConsole.h"
#include <scheduler.h>





pipeData *pipes[PIPES_MAXQTY];
uint32_t pipesQty = 0;
uint8_t internalId = 0;

fd *createFd()
{
    return (fd *)allocMemory(sizeof(fd));
}

// Tanto fd1 como fd2 deben ser distintos de NULL y se genera un id interno para el pipe
int createPipe(fd *f0, fd *f1)
{
    if (pipesQty == PIPES_MAXQTY || f0 == NULL || f1 == NULL)
        return -1;


        internalId--;
 

    pipes[pipesQty] = (pipeData *)allocMemory(sizeof(pipeData));
    (*pipes[pipesQty]) = (pipeData){.readIdx = 0, .writeIdx = 0, .reading = 1, .writing = 1, .readableBytes = 0, .id = internalId, .waitingPCB = NULL};


    //   pipes[pipeAmount]->readIdx = 0;
    //   pipes[pipeAmount]->writeIdx = 0;
    //   pipes[pipeAmount]->reading = 1;
    //   pipes[pipeAmount]->writing = 1;
    //   pipes[pipeAmount]->readableBytes = 0;
    //   pipes[pipeAmount]->id = PIPES_MAXQTY + pipeIds++;
    //   pipes[pipeAmount]->waitingPCB = NULL;

    
        f0->pipe = pipes[pipesQty];
        f0->readwrite = READ;
    

    
        f1->pipe = pipes[pipesQty];
        f1->readwrite = WRITE;
    
    //   f0->pipe = pipes[pipeAmount];
    //   f0->readable = 1;
    //   f0->writable = 0;
    //   f1->pipe = pipes[pipeAmount];
    //   f1->readable = 0;
    //   f1->writable = 1;
    pipesQty++;
    return 0;
}

// Se abre un pipe existente o se crea en caso de que no exista. El id debe ser >= 0. Se retorna 0 si se abrio uno existente y 1 si se creo uno nuevo
int openPipeId(fd *fd, uint32_t id, int readwrite)
{
    if (id < 0)
        return -1;

    int i = 0;
    while (i < pipesQty && pipes[i]->id != id)
        i++;

    fd->readwrite = readwrite;

    if (i == pipesQty)
    {

        // Si no se encuentra el id en los pipes ya creados, creamos uno nuevo

        if (pipesQty == PIPES_MAXQTY)
            return -1;

               pipes[pipesQty] = (pipeData *)allocMemory(sizeof(pipeData));
    (*pipes[pipesQty++]) = (pipeData){.readIdx = 0, .writeIdx = 0, .reading = (readwrite == READ), .writing = (readwrite == WRITE), .readableBytes = 0, .id = id, .waitingPCB = NULL};

        return 1;

        // pipes[pipesQty] = (pipeData *) allocMemory(sizeof(pipeData));
        // fd->pipe = pipes[pipesQty++];
        // fd->pipe->readableBytes = 0;
        // fd->pipe->readIdx = 0;
        // fd->pipe->writeIdx = 0;
        // fd->pipe->reading = reader ? 1 : 0;
        // fd->pipe->writing = reader ? 0 : 1;
        // fd->pipe->id = id;
        // return 0;
    }

    if ((readwrite == READ && pipes[i]->reading) || (readwrite == WRITE && pipes[i]->writing))
        return -1;

    if (readwrite == READ)
        fd->pipe->reading = 1;
    else
        fd->pipe->writing = 1;

    fd->pipe = pipes[i];
    
    return 0;
}

static void deletePipe(pipeData * p)
{
    int i = 0;
    while (i < pipesQty && pipes[i]->id != p->id)
        i++;

    if (i == pipesQty)
        return;

    while (i < pipesQty - 1)
    {
        pipes[i] = pipes[i + 1];
        i++;
    }
    pipesQty--;
}

void closeFd(fd *fd)
{
    if (fd == STDIN || fd == STDOUT)
        return;

    if (fd->readwrite == READ)
        fd->pipe->reading = 0;

    if (fd->readwrite == WRITE)
        fd->pipe->writing = 0;

    if ((fd->readwrite == READ && !fd->pipe->writing) || (fd->readwrite == WRITE && !fd->pipe->reading))
    {
        deletePipe(fd->pipe);
        freeMemory(fd->pipe);
    }
    else if (fd->pipe->waitingPCB != NULL)
    {
        fd->pipe->waitingPCB->pstate = READY;
        fd->pipe->waitingPCB = NULL;
    }

    freeMemory(fd);
}

int pipeWrite(fd *fd, char *string)
{
    if (fd->readwrite != WRITE || (!fd->pipe->reading && fd->pipe->readableBytes == DATA_SIZE))
        return -1;

    int i = 0;
    while (string[i])
    {
        while (fd->pipe->readableBytes == DATA_SIZE)
        {
            fd->pipe->waitingPCB = blockCurrentProcess();
            forceScheduler();
        }

        fd->pipe->data[fd->pipe->writeIdx++] = string[i++];

        if (fd->pipe->writeIdx == DATA_SIZE)
            fd->pipe->writeIdx = 0;
        fd->pipe->readableBytes++;
    }
    if (fd->pipe->waitingPCB != NULL)
    {
        fd->pipe->waitingPCB->pstate = READY;
        fd->pipe->waitingPCB = NULL;
    }
    return i;
}

int pipeRead(fd *fd, char *buffer, int limit)
{
    
    if (fd->readwrite != READ)
        return -1;

        

    while (fd->pipe->readableBytes == 0)
    {
        // ncPrint("a");
        if (!fd->pipe->writing) {
            return 0;
        }
            
        fd->pipe->waitingPCB = blockCurrentProcess();
        forceScheduler();
    }

    int i = 0;
    while (i < limit && fd->pipe->readableBytes > 0)
    {
        fd->pipe->readableBytes--;
        if (fd->pipe->readIdx == DATA_SIZE)
            fd->pipe->readIdx = 0;

            buffer[i++] = fd->pipe->data[fd->pipe->readIdx++];
    }

    if (fd->pipe->waitingPCB != NULL)
    {
        fd->pipe->waitingPCB->pstate = READY;
        fd->pipe->waitingPCB = NULL;
    }
    return i;
}

void deleteProcessFromPipes(uint32_t pid)
{
    for (int i = 0; i < pipesQty; i++)
        if (pipes[i]->waitingPCB->pid == pid)
            pipes[i]->waitingPCB = NULL;
}