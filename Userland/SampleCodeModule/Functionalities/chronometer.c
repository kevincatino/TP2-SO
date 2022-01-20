#include <libc.h>
#include <chronometer.h>
#include <hangman.h>
#include <sudoku.h>
#include <clock.h>

#define BUFFER_SIZE 8
#define MAX_ARGUMENTS 2
    
static int start;
static int sec, min, Dsec;

void chronometerGame() {
    char buffer[BUFFER_SIZE];		
	char *command[MAX_ARGUMENTS];
    while(TRUE) {
        stopWatch();
        print("\nEnter \'s\' to start, \'p\' to pause, \'r\' to restart or cs and the number to change screen.\n");
        print("To exit, write \'quit\'.\n");
        userPrint("User@TPE-ARQUI:$ ");
        readInput(buffer, BUFFER_SIZE, GAME_SCREEN_IR);
        strtok(buffer, ' ', command, MAX_ARGUMENTS);  

        if((strCmp(command[0], "s")) == 0) {
            start_stopWatch();
            stopWatch();
        } else if((strCmp(command[0], "p")) == 0) {
            pause_stopWatch();
        } else if((strCmp(command[0], "r")) == 0) {
            reset_stopWatch();
        } else if((strCmp(command[0], "cs")) == 0) {
            if(((strCmp(command[1], "1")) == 0)) {
                print("\nCannot acces to time screen.\n");
            } else if(((strCmp(command[1], "2")) == 0)) {
                print("\nYou already are in Chronometer.\n");
            } else if(((strCmp(command[1], "3")) == 0)) {
                print("\n");
                continueSudoku();
                return;     
            } else if(((strCmp(command[1], "4")) == 0)) {
                print("\n");
                continueHangman();
                return;     
            } else {
                print("\nInvalid argument.\n");
            }
        } else if(((strCmp(command[0], "quit")) == 0) || ((strCmp(command[0], "Quit")) == 0)) {
            pause_stopWatch();
            reset_stopWatch();
            print("\n");
            return;
        }
    }
    
}


void stopWatch() {
    char minutes[3], seconds[3], Dseconds[3];

    if(start) {
        Dsec++;
        getMilliSeconds(100);

        if(Dsec == 9){ 
            sec++;
            Dsec = 0;
        }

        if(sec == 59){
            min++;
            sec = 0;
            Dsec = 0;
        }

        if(min == 59){
            min = 0;
            sec = 0;
            Dsec = 0;
        }

        intToString(min, minutes);
        intToString(sec, seconds);
        intToString(Dsec, Dseconds);
        printInScreenPosition(minutes, (80*2*5) + (80) + (16*2));   
        printInScreenPosition(":", (80*2*5) + (80) + (2*18));
        printInScreenPosition(seconds, (80*2*5) + (80) + (2*19));
        printInScreenPosition(":", (80*2*5) + (80) + (2*21));
        printInScreenPosition(Dseconds, (80*2*5) + (80) + (2*22));
    }

    
}


void start_stopWatch() {
    start = 1;
}

void pause_stopWatch() {
    start = 0;
}

void reset_stopWatch() {
    Dsec = 0;
    sec = 0;
    min = 0;
    start = 0;
    printInScreenPosition("               00:00:00", (80*2*5) + (80) + 2);
}


void printInitialChronometer() {
    printInScreenPosition("Screen 2", (80*2*6)+ (80) + 64);
    printInScreenPosition("               00:00:00", (80*2*5) + (80)  + 2);
}