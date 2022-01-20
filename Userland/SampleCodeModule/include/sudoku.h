#ifndef _SUDOKU_H_
#define _SUDOKU_H_
#define SIZE 9


extern int sys_read(unsigned int fd, char* buffer, unsigned int buffercount);    
extern int sys_write(unsigned int fd, char*buffer, unsigned int buffersize); 

void sudokuGame();
void printPuzzle();
int checkAvailable(int* row, int* column, int flag);
int checkBox(int row, int column, int val, int flag);
int solvePuzzle(int flag);
void copyPuzzle(int puzzle[SIZE][SIZE], int flag);
void userChoice();
void continueSudoku();

#endif