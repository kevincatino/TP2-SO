#include <shell.h>
#include <libc.h>
#include <test_lib.h>
#include <test_util.h>
#include <phylo.h>

#define LOAD_GAME 0
#define CONTINUE_GAME 1
#define NULL ((void *)0)
#define MAX_CMD_SIZE 20

char *commands[MAX_CMD_SIZE] = {"help", "clean", "mmTest", "syncTest", "loop", "kill", "cat", "wc", "filter", "phylo", "block", "ps", "memStatus", "nice", "pipe", "sem"};
void (*commandPointers[MAX_CMD_SIZE])(uint64_t, char **) = {helpMenu, clean, memoryManagerTest, syncTest, loop, kill, cat, wc, filter, phylo, block, ps, memStatus, nice, pipe, sem};
uint64_t commandsLength = sizeof(commands) / sizeof(commands[0]);

void initializeShell()
{
    initialMessage();
    generalShell();
}

void initialMessage()
{
    print("         _ _ _ ____ _    ____ ____ _  _ ____    ___ ____ \n");
    print("         | | | |___ |    |    |  | |\\\/| |___     |  |    \n");
    print("         |_|_| |___ |___ |___ |__| |  | |___     |  |__| \n");
    print("                                                         \n");
    print("                    ___   ___   ___                      \n");
    print("                   |     |   | |                         \n");
    print("                   |___  |   | |___                      \n");
    print("                       | |   |     |                     \n");
    print("                   ____| |___| ____|                     \n");

    print("If you want to see the help menu, please write \'help\'.\nOtherwise, enter a valid command.\n\n");
}



void generalShell()
{
    char input[MAX_BUFFER];
    int shellActivated = 1;
    while (shellActivated)
    {
        printUser();

        readInput(input, MAX_BUFFER, GENERAL_SCREEN);
        print("\n");
        shellActivated = menuCommands(input);
    }
}

void printUser()
{
    userPrint("User@TPE-SO:$ ");
}

void managePipe(char *command[], uint64_t pipeIdx, uint64_t argSize)
{
    int indexP1 = -1, indexP2 = -1;
    char *p1 = command[0];
    char *p2 = command[pipeIdx + 1];
    FileDes pipeRead = sys_createFd();
    FileDes pipeWrite = sys_createFd();
    sys_createPipe(pipeRead, pipeWrite);

    for (int i = 0; i < commandsLength; i++)
    {
        if (strCmp(p1, commands[i]) == 0)
            indexP1 = i;
        if (strCmp(p2, commands[i]) == 0)
            indexP2 = i;
    }

    if (indexP1 == -1 || indexP2 == -1)
    {
        print("Command not found, try 'help'\n");
        return;
    }

    sys_create_process((uint64_t)commandPointers[indexP1], 2, pipeIdx, command, STDIN, pipeWrite);
    sys_create_process((uint64_t)commandPointers[indexP2], 1, argSize - pipeIdx - 1, command + pipeIdx + 1, pipeRead, STDOUT);
}

int menuCommands(char *input)
{
    char *command[MAX_ARGUMENTS] = {NULL, NULL, NULL, NULL, NULL, NULL};
    int argSize = strtok(input, " ", command, MAX_ARGUMENTS);

    int index = 0;
    while (index < argSize - 1)
    {
        if (strCmp(command[index], "|") == 0)
        {
            managePipe(command, index, argSize);
            return TRUE;
        }
        index++;
    }

    int i;
    for (i = 0; i < commandsLength; i++)
    {
        if (strCmp(command[0], commands[i]) == 0)
        {
            if (argSize > 1 && strCmp(command[argSize - 1], "&") == 0)
            {
                sys_create_process((uint64_t)commandPointers[i], 2, argSize - 1, command, STDIN, STDOUT);
            }
            else
            {
                // print("my pid is ");
                // printNum(sys_pid());
                sys_create_process((uint64_t)commandPointers[i], 1, argSize, command, STDIN, STDOUT);
            }
            return TRUE;
        }
    }

    print("Invalid command, please enter a valid command. If you need help, write \'help\'.\n");

    return TRUE;
}

void helpMenu(uint64_t argc, char *argv)
{
    print("The commands and descriptions availables are:\n\n");
    print("\'help\' - Display the commands and descriptions of the functions availables.\n");
    print("\'clean\' - Clean the shell.\n");
    print("\'mmTest\' - Memory test.\n");
    print("\'syncTest\' - Semaphore test.\n");
    print("\'loop\' - Print a process ID with a message. Each one determinated by a quantity of seconds.\n"); 
    print("\'kill\' - Kill a process given its ID.\n");
    print("\'cat\' - Print the stdin as it is received.\n");
    print("\'wc\' - Count the input lines.\n");
    print("\'filter\' - Filter vocals from input.\n");
    print("\'phylo\' - Implement the dining philosophers problem.\n");
    print("\'block\' - Change the state of a process between blocked and ready given its ID.\n");
    print("\'ps\' - Print the list of all processes with their properties.\n");
    print("\'memStatus\' - Print the status of the memory.\n");
    print("\'nice\' - Change the priority of a process given iID and the new priority.\n");
    print("\'pipe\' Print a list of all the pipes with their status.- \n");
    print("\'sem\' - Print a list of all semaphores with their properties\n");

    sys_exit();
}

void clean(uint64_t argc, char *argv[])
{
    clearScreen();
    sys_exit();
}

void memoryManagerTest(uint64_t argc, char *argv[])
{
    test_mm(argc, argv);
    sys_exit();
}

void syncTest(uint64_t argc, char *argv[])
{
    if (argc == 1)
    {
        testSync();
    }
    else if (strCmp(argv[1], "no-sem") == 0)
    {
        testNoSync();
    }
    sys_exit();
}

void kill(uint64_t argc, char *argv[])
{
    if (argc < 2)
    {
        print("Es necesario ingresar el pid del proceso\n");
        sys_exit();
        return;
    }

    uint32_t pid = (uint32_t)satoi(argv[1]);

    sys_killPid(pid);

    sys_exit();
}

void loop(uint64_t argc, char *argv[])
{

    if (argc < 2)
    {
        print("Es necesario ingresar el tiempo de loop\n");
        sys_exit();
        return;
    }

    int64_t delay = satoi(argv[1]);

    while (1)
    {
        print("Loop from: ");
        printNum(sys_pid());
        putChar('\n');
        sleep((uint64_t)delay);
    }
    sys_exit();
}

void memStatus(uint64_t argc, char *argv[])
{
    sys_printMem();
    sys_exit();
}

void cat(uint64_t argc, char *argv[])
{

    if (argc == 1)
    {
        char c;
        while ((c = getChar()) != 0)
        {
            putChar(c);
        }
    }
    else
    {

        int argIdx;
        for (argIdx = 1; argIdx < argc; argIdx++)
        {
            char string[MAX_BUFFER];
            char *lines[MAX_CMD_SIZE];
            strCpy(string, argv[argIdx]);
            int lineCount = strtok(string, "\\n", lines, MAX_CMD_SIZE);
            int i;
            for (i = 0; i < lineCount; i++)
            {
                print(lines[i]);
                putChar('\n');
            }
        }
    }

    sys_exit();
}

void filter(uint64_t argc, char *argv[])
{
    char c;
    while ((c = getChar()) != 0)
    {
        int i = 0;
        switch (c)
        {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
        case 'A':
        case 'E':
        case 'I':
        case 'O':
        case 'U':
            break;
        default:
            putChar(c);
        }
        i++;
    }
    sys_exit();
}

void wc(uint64_t argc, char *argv[])
{
    int n = 0;
    char c = 0;
    while ((c = getChar()) != 0)
        if (c == '\n')
        {
            n++;
        }

    print("Cantidad de lineas leidas: ");
    printNum(n);
    putChar('\n');
    sys_exit();
}

void ps(uint64_t argc, char *argv[])
{
    sys_ps();
    sys_exit();
}

void pipe(uint64_t argc, char *argv[])
{
    sys_printPipes();
    sys_exit();
}

void sem(uint64_t argc, char *argv[])
{
    sys_printSemaphores();
    sys_exit();
}

void block(uint64_t argc, char *argv[])
{
    if (argc == 1)
    {
        print("Es necesario el pid del proceso");
        sys_exit();
        return;
    }

    uint32_t pid = (uint32_t)satoi(argv[1]);

    sys_change_process_state(pid, -1);

    sys_exit();
}

void nice(uint64_t argc, char *argv[])
{
    if (argc <= 2)
    {
        print("Es necesario el pid del proceso");
        sys_exit();
        return;
    }

    uint32_t pid = (uint32_t)satoi(argv[1]);

    uint32_t priority = (uint32_t)satoi(argv[2]);

    sys_change_process_priority(pid, priority);

    sys_exit();
}
