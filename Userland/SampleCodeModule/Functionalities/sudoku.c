#include <sudoku.h>
#include <libc.h>
#include <hangman.h>
#include <clock.h>
#include <chronometer.h>


#define JUMP 3
#define BUFFER_SIZE 120
#define STDIN 0
#define STDOUT 1
#define TRUE 1
#define FALSE 0
#define MAX_ARGUMENTS 2
#define GAME_SCREEN_IL 3
#define USER_PUZZLE 1
#define TEMP_PUZZLE 0


static int userPuzzle[SIZE][SIZE];
static int tempPuzzle[SIZE][SIZE];
static int startedGame;     // Si esta en 1 esta inicializado, si esta en 0 no


void sudokuGame(int flag) { 
    startedGame = 1;
    int puzzle[SIZE][SIZE] =  { {5, 3, 0,   0, 7, 0,   0, 0, 0},
                                {6, 0, 0,   1, 9, 5,   0, 0, 0},
                                {0, 9, 8,   0, 0, 0,   0, 6, 0},

                                {8, 0, 0,   0, 6, 0,   0, 0, 3},
                                {4, 0, 0,   8, 0, 3,   0, 0, 1},
                                {7, 0, 0,   0, 2, 0,   0, 0, 6},

                                {0, 6, 0,   0, 0, 0,   2, 8, 0},
                                {0, 0, 0,   4, 1, 9,   0, 0, 5},
                                {0, 0, 0,   0, 8, 0,   0, 7, 9} 
                            };

    copyPuzzle(puzzle, USER_PUZZLE);
    copyPuzzle(puzzle, TEMP_PUZZLE);
    printPuzzle();
    if(flag == 1) {
        userChoice();
    }
}


void printPuzzle(){
    clearScreen(GAME_SCREEN_IL);

    char * auxX = " 0 | 1  2  3  4  5  6  7  8  9 | X";
    int initialPositionX = ((80*2)*8);

    printInScreenPosition("Screen 3", initialPositionX + 64 + (80*2*16));

    int orientationX = 1;       // Si esta en 1 me muevo en el eje x, en 0 en el eje y
    printSudokuNumbers(auxX, initialPositionX, orientationX);

    auxX = "------------|--------|--------|---";
    printSudokuNumbers(auxX, initialPositionX + 80*2, orientationX);


    auxX = " 1 | 2 | 3 _ 4 | 5 | 6 _ 7 | 8 | 9 |---- Y";
    int initialPositionY = ((80*2)*8)+(80*2)+(80*2);    // Arranco desde la 10ma fila 
    printSudokuNumbers(auxX, initialPositionY, !orientationX);

    initialPositionY += ((5*2));
    for(int i = 0; i < SIZE; i++) {    // Inicializo i en 1 ya que si es 0 me escribe en cualquier lado.
        for(int j = 0; j < SIZE; j++) {
            char auxRow[2] = {0};
            auxRow[0] = userPuzzle[i][j] + '0';
            if((j % 3 == 0) && (j != 0)) {
                printSudokuNumbers("   ", initialPositionY + (j*2*3) + (i*80*2), orientationX); 
            }
            printSudokuNumbers(auxRow, initialPositionY + (j*2*3) + (i*80*2), orientationX);
        }
    }
}


int checkAvailable(int * row, int * column, int flag) {
    if(flag == USER_PUZZLE){
        int i, j;
        for (i = 0; i < SIZE; i++){
            for(j = 0; j < SIZE; j++){
                if (userPuzzle[i][j] == 0){
                    *row = i;
                    *column  = j;
                    return TRUE;
                }
            }
        }
        return FALSE;

    } else if(flag == TEMP_PUZZLE) {
        int i, j;
        for (i = 0; i < SIZE; i++){
            for(j = 0; j < SIZE; j++){
                if (tempPuzzle[i][j] == 0){
                    *row = i;
                    *column  = j;
                    return TRUE;
                }
            }
        }
        return FALSE;
    } else {
        return FALSE;
    }
}

int checkBox(int row, int column, int val, int flag) {
    if(flag == USER_PUZZLE){
        int i, j;
        int squareRow, squareColumn;

        //CHECK COLUMNA
        for(i = 0; i < SIZE; i++){
            if (userPuzzle[i][column] == val)
                return FALSE;
        }

        //CHECK FILA
        for(j = 0; j < SIZE; j++){
            if (userPuzzle[row][j] == val)
                return FALSE;
        }

        //CHECK SUBCUADRADO
        squareRow = row - row % JUMP;
        squareColumn = column - column % JUMP;

        for(i = 0; i < JUMP; i++){
            for(j = 0; j < JUMP; j++){
                if(userPuzzle[squareRow + i][squareColumn + j] == val)
                    return FALSE;
            }
        }

        return TRUE;

    } else if(flag == TEMP_PUZZLE) { 
        int i, j;
        int squareRow, squareColumn;

        //CHECK COLUMNA
        for(i = 0; i < SIZE; i++){
            if (tempPuzzle[i][column] == val)
                return FALSE;
        }

        //CHECK FILA
        for(j = 0; j < SIZE; j++){
            if (tempPuzzle[row][j] == val)
                return FALSE;
        }

        //CHECK SUBCUADRADO
        squareRow = row - row % JUMP;
        squareColumn = column - column % JUMP;

        for(i = 0; i < JUMP; i++){
            for(j = 0; j < JUMP; j++){
                if(tempPuzzle[squareRow + i][squareColumn + j] == val)
                    return FALSE;
            }
        }

        return TRUE;
    } else {
        return FALSE;
    }

}

int solvePuzzle(int flag) {
    if(flag == USER_PUZZLE){
        int i, j, val;

        if(!checkAvailable(&i, &j, 1))
            return TRUE;    

        for(val = 1; val < SIZE + 1; val++){
            if(checkBox(i, j, val, USER_PUZZLE)){
                userPuzzle[i][j] = val;
                if(solvePuzzle(USER_PUZZLE))
                    return TRUE;
                else
                    userPuzzle[i][j] = 0;
            }
        }
        return FALSE;

    } else if(flag == TEMP_PUZZLE) {
        int i, j, val;

        if(!checkAvailable(&i, &j, 0))
            return TRUE;    

        for(val = 1; val < SIZE + 1; val++){
            if(checkBox(i, j, val, TEMP_PUZZLE)) {
                tempPuzzle[i][j] = val;
                if(solvePuzzle(TEMP_PUZZLE))
                    return TRUE;
                else
                    tempPuzzle[i][j] = 0;
            }
        }
        return FALSE;

    } else {
        return FALSE;
    }
}

void copyPuzzle(int puzzle[SIZE][SIZE], int flag) {
    if(flag == USER_PUZZLE){
        int i, j;
        for (i = 0; i < SIZE; i++){
            for(j = 0; j < SIZE; j++) {
                userPuzzle[i][j] = puzzle[i][j];
            }
         }
    } else if(flag == TEMP_PUZZLE) {
        int i, j;
        for (i = 0; i < SIZE; i++){
            for(j = 0; j < SIZE; j++) {
                tempPuzzle[i][j] = puzzle[i][j];
            }
        }
    }
}

void userChoice() {
    int i, j;
    char buffer[BUFFER_SIZE];


    while(TRUE){
        if(!checkAvailable(&i, &j, USER_PUZZLE)) {
            print("\nGood Job You Solved the Sudoku!!!\n");
            printInScreenPosition("SOLVED SUDOKU!", ((80*2)*22)+(2*8));
            return;
        }

        while(TRUE) {
            print("Press Enter to continue playing. Write \'quit\' to end the and see the solution.\n");
            userPrint("User@TPE-ARQUI:$ ");
            readInput(buffer, BUFFER_SIZE, GAME_SCREEN_IL);
            char *command[MAX_ARGUMENTS];
            strtok(buffer, ' ', command, MAX_ARGUMENTS);  
        
            
            if(((strCmp(command[0], "quit")) == 0) || ((strCmp(command[0], "Quit")) == 0)) {
                print("\n");
                solvePuzzle(USER_PUZZLE);
                printPuzzle();
                printInScreenPosition("SOLVED SUDOKU!", ((80*2)*22)+(2*8));
                return;
            } else if(((strCmp(command[0], "cs")) == 0)) {
                if(((strCmp(command[1], "1")) == 0)) {
                    print("\nYou cant access to time screen\n");
                } else if(((strCmp(command[1], "2")) == 0)) {
                    print("\n");
                    chronometerGame();
                    return;     
                } else if(((strCmp(command[1], "3")) == 0)) {
                    print("\nYou already are in Sudoku.\n");
                } else if(((strCmp(command[1], "4")) == 0)) {
                    print("\n");
                    continueHangman();  
                    return;     
                } else {
                    print("\nInvalid argument.");
                }
            } else {
                break;
            }
        }


        print("\nPlease Enter Coordinate for the square you want to insert the value to in the following format \"X Y\": ");
        char buffer[BUFFER_SIZE];       // Usado para las coordenadas X e Y
        readInput(buffer, BUFFER_SIZE, GAME_SCREEN_IL);
        print("\n");
        char *comand[MAX_ARGUMENTS];
        strtok(buffer, ' ', comand, MAX_ARGUMENTS);
        
        int positions[MAX_ARGUMENTS];
        stringToIntArray(comand, positions, MAX_ARGUMENTS);
        
        while(TRUE) {
            if ((positions[0] > SIZE) || (positions[0] < 1) || (positions[1] > SIZE) || (positions[1] < 1) || (userPuzzle[positions[1] - 1][positions[0] - 1] != 0)) { // El ultimo or lo que hace es chequea que en mi coordenada, si es valida, sea un cero
                print("You can't insert value to this Coordinate, please pick a new Coordinate: ");
                readInput(buffer, BUFFER_SIZE, GAME_SCREEN_IL);
                print("\n");
                strtok(buffer, ' ', comand, MAX_ARGUMENTS);
                stringToIntArray(comand, positions, MAX_ARGUMENTS);
            } else {
                positions[0] -= 1;
                positions[1] -= 1;
                break;
            }
        }


        print("Please insert value from 1 to 9: ");
        char buffer2[MAX_ARGUMENTS];       // Usado para las coordenadas X e Y
        readInput(buffer2, MAX_ARGUMENTS, GAME_SCREEN_IL);
        print("\n");

        int userVal = charToDigit(buffer2); 

        while(TRUE) {
            if((userVal > SIZE) || (userVal < 1)){
                print("\nPlease insert valid value: ");
                readInput(buffer2, MAX_ARGUMENTS, GAME_SCREEN_IL);
                userVal = charToDigit(buffer2); 
            } else {
                break;
            }
        }


        char aux[2];
        if(checkBox(positions[1], positions[0], userVal, USER_PUZZLE)) {
            userPuzzle[positions[1]][positions[0]] = userVal;
        } else {
            print("\nIncorrect value for the X = "); 
            toStr(positions[0]+1, aux);
            print(aux);
            print(" Y = ");
            toStr(positions[1]+1, aux);
            print(aux);
            print(" coordinate, please try again.\n");
        }

        for (i = 0; i < SIZE; i++) {
            for(j = 0; j < SIZE; j++) {
                tempPuzzle[i][j] = userPuzzle[i][j];
            }
        }

        if(!solvePuzzle(TEMP_PUZZLE)) {
            print("\nIncorrect value for the X = "); 
            toStr(positions[0]+1, aux);
            print(aux);
            print(" Y = ");
            toStr(positions[1]+1, aux);
            print(aux);
            print(" coordinate, please try again\n");
            userPuzzle[positions[1]][positions[0]] = 0;
        }
        printPuzzle();
    }
    return;
}



void continueSudoku() {
    userChoice();
}