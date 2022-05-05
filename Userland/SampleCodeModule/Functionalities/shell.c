#include <shell.h>
#include <libc.h>

#define LOAD_GAME 0
#define CONTINUE_GAME 1

static uint64_t *registersCopy;

int gameCommands(char *input);
void initializeAllGames();
void memoryManagerTest();

void loop() {
    static int prev=0;
    while(1) {
        int ticks = sys_get_ticks();
        if(ticks != prev && ticks %10 == 0) {
            print("A");
            prev = ticks;
        }
        
            
    } 
}

void initializeShell()
{
    //sys_create_process(&loop, 2, 1, "loop");
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

int menuCommands(char *input)
{
    char *command[MAX_ARGUMENTS];
    int argSize = strtok(input, ' ', command, MAX_ARGUMENTS);

    if (argSize > 0 && argSize <= 2)
    {
        if ((strCmp(command[0], "help")) == 0)
        {
            helpMenu();
        }
        else if (strCmp(command[0], "zeroDivExc") == 0)
        {
            zeroDivExc();
        }
        else if (strCmp(command[0], "invOpCodeExc") == 0)
        {
            invOpCodeExc();
        }
        else if (strCmp(command[0], "inforeg") == 0)
        {
            if (checkTabCount() != 0)
            {
                infoReg();
            }
            else
            {
                print("First you have to save the registers using the \'tab\' button.\n");
            }
        }
        else if (strCmp(command[0], "printmem") == 0)
        {
            if (command[1] != 0)
            {
                if (command[1][0] == '0' && (command[1][1] == 'x' || command[1][1] == 'X'))
                {
                    printMem(command[1]);
                }
                else
                {
                    print("Incorrect Arguemnt format. Correct format: \'0xX\'.\n");
                }
            }
            else
            {
                print("Incorrect function call, you have to pass a valid argument.\n");
            }
        }
        else if (strCmp(command[0], "time") == 0)
        {
            getTime();
        }
        else if (strCmp(command[0], "games") == 0)
        {
            games();
            clearScreen(GENERAL_SCREEN);
            changeScreen(GENERAL_SCREEN);
        }
        else if (strCmp(command[0], "exit") == 0)
        {
            globalExit();
            return FALSE;
        }
        else if (strCmp(command[0], "clean") == 0)
        {
            clearScreen(GENERAL_SCREEN);
        }
        else if (strCmp(command[0], "mmTest") == 0){
            memoryManagerTest();
        }
        else
        {
            print("Invalid command, please enter a valid command. If you need help, write \'help\'.\n");
        }
    }
    else
    {
        print("Invalid amount of parameters, please enter a valid command. If you need help,\nwrite \'help\'.\n");
    }
    return TRUE;
}

void helpMenu()
{
    print("The commands and descriptions availables are:\n\n");
    print("\'help\' - Displays the commands and descriptions of the functions availables.\n");
    print("\'zeroDivExc\' - Displays a exception of invalid division by zero.\n");
    print("\'invOpCodeExc\' - Displays a exception of invalid operation code.\n");
    print("\'inforeg\' - Displays the information of the registers saved by the user using \'tab\' button.\n");
    print("\'printmem\' - Makes a 32 Bytes memory dump to screen from the address passed by\nargument.\n");
    print("\'time\' - Displays the current time and date.\n");
    print("\'games\' - Divide the screen in 4 (current time, chronometer, sudoku, hangman).\n");
    print("\'exit\' - Exit form the OS.\n");
    print("\'clean\' - To clean the shell.\n");
}

void zeroDivExc()
{
    div_zero();
}

// source: https://www.felixcloutier.com/x86/ud
void invOpCodeExc()
{
    throwInvalidOpcode();
}

void infoReg()
{
    sys_print_regs();
}

void printMem(char *hexaAddress)
{
    char buffer[MAX_BUFFER];
    char byte[MAX_BUFFER] = {0};
    sys_memory_dump(buffer, hexaToInt(hexaAddress + 2, 16), 32);

    print("Dump memory of 32 bytes form ");
    print(hexaAddress);
    print(" addres: \n");
    for (int i = 0; i < 32; i++)
    {
        if (i % 4 == 0)
        {
            print("\n");
        }
        print("    ");
        print(intToHexa(buffer[i], byte, 1));
        print("  ");
    }
    print("\n");
}

void getTime()
{
    char aux[3];

    print("Date (DD-MM-YY): ");
    intToString(time(DAY), aux);
    print(aux);
    print("/");
    intToString(time(MOUNTH), aux);
    print(aux);
    print("/");
    printNum(time(YEAR) + 2000);
    print(", Time: ");
    intToString(time(HOURS), aux);
    print(aux);
    print(":");
    intToString(time(MINUTES), aux);
    print(aux);
    print(":");
    intToString(time(SECONDS), aux);
    print(aux);
    print("\n");
}

void games()
{
    char input[MAX_BUFFER];
    char c[1];
    clearScreen(GENERAL_SCREEN);
    print("You enter to the game section. Here are the games rules:\n");
    print("Clock Rules:\n");
    print("\t Yo can't access to that terminal\n");
    print("\nChronometer Rules (to acces, type: \'cs 2\'):\n");
    print("\tTo start type \'s\', to pause \'p\' and to restart \'r\'.\n");
    print("\nSudoku Rules (to acces, type: \'cs 3\'):\n");
    print("\t1. The objective of Sudoku is to fill a 9x9 grid made of small squares so that each row, each column, and each full 3x3 squares use the numbers 1-9.\n\t2. While solving the sudoku you can only use each number one time in the square, column, and row.\n\t3. You can only insert numbers in the squares that have value \'0\'\n\t4. If you give up and want to see a solved sudoku please write \'quit\'.\n");
    print("\nHangman Rules (to acces, type: \'cs 4\'):\n");
    print("\t1. You have to guess the word.\n\t2. You have to guess one letter at a time.\n\tIf you write more than one letter, only the first one will be considered.\n\t3. If you want to end de game please write \'quit\'.\n");
    print("\nAlright, thats all. If you are ready, press any letter (no the enter).\n");
    print("Note: you are NOT going to see this message in the game view, so read it\ncarefully.\n");
    print("GOOD LUCK!!!.");
    while (TRUE)
    {
        readInput(c, 2, GENERAL_SCREEN);
        if (c[0] != '\0')
        {
            break;
        }
    }

    changeScreen(GAMES_SCREEN);
    getScreenDivition();
    initializeAllGames();
    int gameShell = 1;
    while (gameShell)
    {
        timeGame();
        printUser();
        readInput(input, MAX_BUFFER, GAMES_SCREEN);
        print("\n");
        gameShell = gameCommands(input);
    }
}

int gameCommands(char *input)
{
    timeGame();
    char *command[MAX_GAMES_ARGUMENTS];
    int argSize = strtok(input, ' ', command, MAX_GAMES_ARGUMENTS);
    if (argSize == 2 && strCmp(command[0], "cs") == 0)
    {
        if (strCmp(command[1], "1") == 0)
        {
            print("You can't change to time screen.\n");
        }
        else if (strCmp(command[1], "2") == 0)
        {
            chronometerGame();
        }
        else if (strCmp(command[1], "3") == 0)
        {
            print("\n");
            sudokuGame(CONTINUE_GAME);
        }
        else if (strCmp(command[1], "4") == 0)
        {
            print("\n");
            hangmanGame(CONTINUE_GAME);
        }
        else
        {
            print("Incorrect arguement, please enter a valid number of screen.\n");
        }
    }
    else if (argSize == 1 && strCmp(command[0], "quit") == 0)
    {
        print("The user exit the game screen. Going back to the shell.\n");
        return FALSE;
    }
    else if (strCmp(command[0], "help") == 0)
    {
        print("To acces to a game, write: \'cs\' and the number of screen. For example, to access the sudoku, write: \'cs 3\'. Write \'quit\' to exit the game screen.\n");
    }
    else
    {
        print("Incorrect arguement, please enter a valid instruction. Write \'help\' if you need it, otherwise, \'quit\' to exit the game screen.\n");
    }
    return TRUE;
}

void initializeAllGames()
{
    timeGame();
    printInitialChronometer();
    sudokuGame(LOAD_GAME);
    hangmanGame(LOAD_GAME);
}

void globalExit()
{
    clearScreen(GENERAL_SCREEN);
    print("Thanks for using our OS!!! Come back soon!!!\n\n");
}

void updateRegs(uint64_t *registers)
{
    for (int i = 0; i < 17; i++)
    {
        registersCopy[i] = registers[i];
    }
}

void memoryManagerTest() {
    print("Aca se correria el test de memoria\n");
}