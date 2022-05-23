#include <sysCallDispatcher.h>
#include <the_memory_manager.h>
#include <scheduler.h>
#include <lib.h>
#include "../include/time.h"
#include "../include/semaphore.h"
#include <pipe.h>
#include <keyboardDriver.h>

typedef uint64_t (*TypeSysCall)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

static uint64_t syscall;

void exit()
{
    ncPrint("EXIT");
    // exitCurrentProcess();
}

void loadSyscallNum(uint64_t rax)
{
    syscall = rax;
}

static TypeSysCall arraySysCalls[255] = {(TypeSysCall)&sys_read, (TypeSysCall)&sys_write, (TypeSysCall)&sys_get_char, (TypeSysCall)&sys_get_time, (TypeSysCall)&sys_screen_divition, (TypeSysCall)&sys_screen_clear, (TypeSysCall)&sys_memory_dump, (TypeSysCall)&sys_print_user, (TypeSysCall)&sys_print_sudoku_numbers, (TypeSysCall)&sys_print_in_screen_position, (TypeSysCall)&sys_screen_mode, (TypeSysCall)&sys_get_milli_seconds, (TypeSysCall)&sys_print_regs, (TypeSysCall)&allocMemory, (TypeSysCall)&freeMemory, (TypeSysCall)&createProcessForUser, (TypeSysCall)&ticks_elapsed, (TypeSysCall)&changeProcessPriorityForUser, (TypeSysCall)&changeProcessStateForUser, (TypeSysCall)&exitCurrentProcess, (TypeSysCall)&my_signal, (TypeSysCall)&my_wait, (TypeSysCall)&my_sem_open, (TypeSysCall)&my_sem_close, (TypeSysCall)&getCurrentPid, (TypeSysCall)&forceScheduler, (TypeSysCall)&createFd, (TypeSysCall)&createPipe, (TypeSysCall)&openPipeId, (TypeSysCall)&closeFd, (TypeSysCall)&pipeWrite, (TypeSysCall)&pipeRead, (TypeSysCall)&killPid};

uint64_t sysCallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
    TypeSysCall sysCall = arraySysCalls[syscall];
    if (sysCall != 0)
        return sysCall(rdi, rsi, rdx, rcx, r8, r9);

    return 0;
}

uint64_t sys_write(char *buffer, uint64_t count)
{

    FileDes stdout = getStdout();

    // if (fd == 1) {
    //     color = WHITE;
    // } else if (fd == 2) {
    //     color = RED;
    // } else {
    //     ncPrint("Error in file descriptor");
    //     ncNewline();
    //     return 0;
    // }

    // ncPrintDec((uint64_t) stdout);

    if (stdout == STDOUT)
    {
        while (count-- && *buffer != 0)
        {
            ncPrintCharColour(*buffer, WHITE);
            buffer++;
        }
    }
    else
    {
        // ncPrintStringColour("Writing on pipe\n", WHITE);
        // ncPrint(buffer);
        buffer[count] = 0;
        pipeWrite(stdout, buffer);
    }
    return count == 0;
}

uint64_t sys_read(char *buffer, uint64_t count)
{

    // unsigned char *copyFromBuffer = getBuffer();

    FileDes stdin = getStdin();
    // pcb *currentProcess = getCurrentProcess();
    // if (currentProcess->pid > 1 && stdin == STDIN)
    // {
    //     buffer[0] = 0;
    //     return 0;
    // }

    uint64_t i = 0;
    while (i < count)
    {
        if (stdin == STDIN)
        {
            // ncPrint("Leo de stdin");
            buffer[i] = getChar();
        }
        else
        {
            if(pipeRead(stdin, &buffer[i], 1) == 0)
                return i;
            // ncPrintChar(buffer[i]);
            // ncPrintChar(' ');
        }

        // if (buffer[i] == 0)
        //     return i;

        i++;
    }

    return i;
}

char sys_get_char()
{
    char c;
    if(sys_read(&c, 1) == 0)
        return 0;

    return c;
    // ncPrint("retorno ");
    // ncPrintDec(c);
    // ncPrint(" ");

}

uint64_t sys_get_time(uint64_t mode)
{
    return getRTC(mode);
}

void sys_screen_divition()
{
    ncScreenDivition();
}

void sys_screen_clear(uint64_t mode)
{
    ncClearScreen(mode);
}

uint64_t sys_memory_dump(uint64_t buffer, uint64_t address, uint64_t count)
{
    char *auxBuffer = (char *)buffer;
    for (int i = 0; i < count; i++)
    {
        int position = address + i;
        auxBuffer[i] = (char)asmGetByte(position);
    }
    return 0;
}

void sys_print_user(unsigned int fd, const char *buffer, uint64_t count)
{
    while (count-- && *buffer != 0)
    {
        ncPrintCharColour(*buffer, GREEN);
        buffer++;
    }
}

void sys_print_sudoku_numbers(char *line, uint64_t position, uint64_t orientationX)
{
    ncPrintSudokuNumbers(line, position, orientationX);
}

void sys_print_in_screen_position(char *line, uint64_t position)
{
    ncPrintInScreenPosition(line, position);
}

void sys_screen_mode(uint64_t mode)
{
    ncChangeScreen(mode);
}

uint64_t sys_get_milli_seconds(uint64_t time)
{
    uint64_t start = milli_seconds_elapsed();
    do
    {
        _hlt();
    } while (milli_seconds_elapsed() - start < time);
    return 1;
}

void sys_print_regs()
{
    // uint64_t* regs = getRegs();

    // char* registersName[17] = {"R15: ", "R14: ", "R13: ", "R12: ", "R11: ", "R10: ", "R09: ",
    // "R08: ", "RSI: ", "RDI: ", "RBP: ", "RDX: ", "RCX: ", "RBX: ", "RAX: ", "RSP: ", "RIP: "};

    // char registersHexa[30];
    // ncPrintStringColour("Registers Values:\n", WHITE);
    // for (int i = 0; i < 17; i++) {
    //     intToHexa(regs[i], registersHexa, 8);
    //     ncPrintStringColour(registersName[i], YELLOW);
    //     ncPrintStringColour(registersHexa, WHITE);
    //     ncNewline();
    // }
    // ncNewline();
}