#include <shell.h>
#include <libc.h>
#include <test_lib.h>
#include <test_util.h>

#define LOAD_GAME 0
#define CONTINUE_GAME 1
#define NULL ((void *)0)
#define MAX_CMD_SIZE 10

static uint64_t *registersCopy;

char *commands[MAX_CMD_SIZE] = {"help", "clean", "mmTest", "syncTest", "loop", "kill"};
void (*commandPointers[MAX_CMD_SIZE])(uint64_t, char **) = {helpMenu, clearScreen, memoryManagerTest, syncTest, loop, kill};
uint64_t commandsLength = sizeof(commands) / sizeof(commands[0]);


void initializeShell()
{
    initialMessage();
    generalShell();
}

void initialMessage()
{
    print("         _ _ _ ____ _    ____ ____ _  _ ____    ___ ____ \n");
    print("         | | | |___ |    |    |  | |\\/| |___     |  |  | \n");
    print("         |_|_| |___ |___ |___ |__| |  | |___     |  |__| \n");
    print("                                                         \n");
    print("                ____ ____ ____ _  _ _    ____ ____              \n");
    print("                |__| |__/ |  | |  | | __ |  | [__               \n");
    print("                |  | |  \\ |_\\| |__| |    |__| ___]              \n\n");
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
    userPrint("User@TPE-ARQUI:$ ");
}

void managePipe(char *command[], uint64_t pipeIdx, uint64_t argSize)
{
    int indexP1 = -1, indexP2 = -1;
    char *p1 = command[0];
    char *p2 = command[pipeIdx + 1];
    FileDes pipeRead = sys_createFd();
    FileDes pipeWrite = sys_createFd();
    sys_createPipe(pipeRead, pipeWrite);

    for (int i = 0; i < commandsLength; i++) {
    	if (strCmp(p1, commands[i]))
    		indexP1 = i;
    	if (strCmp(p2, commands[i]))
    		indexP2 = i;
    }

    if (indexP1 == -1 || indexP2 == -1) {
    	print("Command not found, try 'help'\n");
    	return;
    }

    sys_create_process((uint64_t)commandPointers[indexP1], 2, pipeIdx, command, STDIN, pipeWrite);
    sys_create_process((uint64_t)commandPointers[indexP2], 2, argSize-pipeIdx-1, command + pipeIdx + 1, pipeRead, STDOUT);
}

int menuCommands(char *input)
{
    char *command[MAX_ARGUMENTS] = {NULL, NULL, NULL, NULL, NULL, NULL};
    int argSize = strtok(input, ' ', command, MAX_ARGUMENTS);

    // int index = 0;
    // while (index < argSize - 1)
    // {
    //     if (strCmp(command[index], "|"))
    //     {
    //         managePipe(command, index, argSize);
    //         return TRUE;
    //     }
    //     index++;
    // }
    int i;
    for (i = 0; i < commandsLength; i++)
    {
        if (strCmp(command[0], commands[i]) == 0)
        {
            // print(commands[i]);
            if (strCmp(command[argSize - 1], "&") == 0)
            {
                sys_create_process((uint64_t)commandPointers[i], 2, argSize-1, command, STDIN, STDOUT);
            }
            else
            {
                (commandPointers[i])(argSize, command);
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
    print("\'help\' - Displays the commands and descriptions of the functions availables.\n");
    print("\'zeroDivExc\' - Displays a exception of invalid division by zero.\n");
    print("\'invOpCodeExc\' - Displays a exception of invalid operation code.\n");
    print("\'inforeg\' - Displays the information of the registers saved by the user using \'tab\' button.\n");
    print("\'printmem\' - Makes a 32 Bytes memory dump to screen from the address passed by\nargument.\n");
    print("\'time\' - Displays the current time and date.\n");
    print("\'exit\' - Exit form the OS.\n");
    print("\'clean\' - To clean the shell.\n");

    sys_exit();
}




void updateRegs(uint64_t *registers)
{
    for (int i = 0; i < 17; i++)
    {
        registersCopy[i] = registers[i];
    }
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
}

void kill(uint64_t argc, char * argv[]) {
    if (argc < 2) {
        print("Es necesario ingresar el pid del proceso\n");
        sys_exit();
        return;
    }

    uint32_t pid = (uint32_t) satoi(argv[1]);

    sys_killPid(pid);
}


void loop(uint64_t argc, char * argv[]) {

    if (argc < 2) {
        print("Es necesario ingresar el tiempo de loop\n");
        sys_exit();
        return;
    }

	int64_t delay = satoi(argv[1]);

	while(1) {
		print("Loop from: ");
		printNum(sys_pid());
		putChar('\n');
		sleep((uint64_t)delay);
	}
    sys_exit();
}

// void cat(uint64_t argc, char * argv[]) {
// 	char buffer[200];
// 	while(getChar(buffer) > 0) {
// 		print(buffer);
// 		print("\n");
// 	}
// 	sys_exit(); 
// }

// void wc(uint64_t argc, char * argv[]) {
// 	char buffer[200];
// 	int n = 0;
// 	while (getChar(buffer) > 0)
// 		n++;

// 	print("Cantidad de lineas leidas: ");
// 	printNum(n);
// 	print("\n");
// 	sys_exit();
// }

// void filter(uint64_t argc,char * argv[]) {
// 	char buffer[200];
//   while(getChar(buffer) > 0) {
//     int i = 0;
// 		while (buffer[i]) {
// 			switch (buffer[i]) {
// 				case 'a':
// 				case 'e':
// 				case 'i':
// 				case 'o':
// 				case 'u':
// 				case 'A':
// 				case 'E':
// 				case 'I':
// 				case 'O':
// 				case 'U':
// 					break;
// 				default:
// 					putChar(buffer[i]);
// 			}
// 			i++;
// 		}
//     print("\n");
// 	}
// 	sys_exit();
// }