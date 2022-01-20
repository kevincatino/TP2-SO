#include <libc.h>
#include <hangman.h>
#include <sudoku.h>
#include <chronometer.h>
#include <clock.h>

#define NUMBER_LIVES 6
#define BUFFER_SIZE 8
#define MAX_ARGUMENTS 2
#define COUNT_ALPHABET_LETTERS 27

	
static char * guessWord = "arquitectura";
static int lives;
static int actualCorrect;
static int oldCorrect;
static uint64_t lengthOfWord;
static int guessedLetters[12];	// 12 es la longitud de la palabra a avidinar (arquitectura)
static int alreadyGuessed;  
static char guessedLetter;	
static int gameEndFlag;		
static const int initialPosition = (80*2*8) + 80 + 2;		// Posicion inicial de la pantalla (donde se va a empezar a escribir)


void hangmanGame(int flag) {
	lives = NUMBER_LIVES;
	actualCorrect = 0;
	oldCorrect = 0;
	lengthOfWord = strlength(guessWord);
	alreadyGuessed = 0;  
    gameEndFlag = 0;

	for(int i = 0; i < lengthOfWord; i++) {
		guessedLetters[i] = 0;
	}


	if(flag == 1) {
		initializeGame();
	} else {
		printInitialHangmanGame();
	}
}


void initializeGame() {


	char buffer[BUFFER_SIZE];		// Donde guardo lo que leo de entrada estandar
	char *command[MAX_ARGUMENTS];	// Donde guardo en una posicion cada palabra por separado que hay en buffer.



	while (actualCorrect < lengthOfWord && !gameEndFlag) {
		clearScreen(GAME_SCREEN_IR);
		userPrint("User@TPE-ARQUI:$ ");
		printInScreenPosition("Hangman Word: ", initialPosition);
		printInScreenPosition("Screen 4", initialPosition + (80*2*16)+ 62);
	
		for(int i = 1; i <= lengthOfWord; i++) { 
			if(guessedLetters[i-1] == 1) {
				char letter = guessWord[i-1];	
				char letterPrint[2] = {letter, '\0'};
				printInScreenPosition(letterPrint, initialPosition + 26 + (i*2));	
			} else {
				printInScreenPosition("-", initialPosition + 26 + (i*2));
			}
		}	

		printInScreenPosition("Number of lives: ", initialPosition + (80*2*2));
		char aux[2] = {lives + '0', '\0'};
		printInScreenPosition(aux, initialPosition + (80*2*2) + (17*2));

 		while(TRUE) {
            print("\nPress Enter to continue playing. Write \'quit\' to end the game and see the solution.\n");
            userPrint("User@TPE-ARQUI:$ ");
            readInput(buffer, BUFFER_SIZE, GAME_SCREEN_IR);
            char *command[MAX_ARGUMENTS];
            strtok(buffer, ' ', command, MAX_ARGUMENTS);	
        
            if(((strCmp(command[0], "quit")) == 0) || ((strCmp(command[0], "Quit")) == 0)) {
                print("\n");
				gameEndFlag = 1;
                break;
            } else if(((strCmp(command[0], "cs")) == 0)) {
                if(((strCmp(command[1], "1")) == 0)) {
                    print("\nYou cant access to time screen.");
                } else if(((strCmp(command[1], "2")) == 0)) {
                    print("\n");
                    chronometerGame();
					return;     
                } else if(((strCmp(command[1], "3")) == 0)) {
					print("\n");
                    continueSudoku();
					return;     
                } else if(((strCmp(command[1], "4")) == 0)) {
					print("\nYou already are in Hangman.");
                } else {
                    print("\nInvalid argument.");
                }
            } else {
                break;
            }
        }

		if(gameEndFlag == 1) {
			break;
		}


		print("Enter a guess letter: ");
		readInput(buffer, BUFFER_SIZE, GAME_SCREEN_IR);
		strtok(buffer, ' ', command, MAX_ARGUMENTS);

		guessedLetter = command[0][0];

		alreadyGuessed = 0; 
		oldCorrect = actualCorrect;

		if(guessedLetter == '4') {
			print("\n");
			gameEndFlag = 1;
		}
		
        int alreadyFound = 0; 		// chequea que el usuario ya ingreso esta letra en un intento pasado
		for(int i = 0; i < lengthOfWord && !alreadyFound; i++) {	
			if(guessedLetters[i] == 1) {				// Me fijo que letras adivine
				if(guessWord[i] == guessedLetter) {		// Si la actual letra  coincide con una de la palabra y esa ya estaba adivinada
					alreadyGuessed = 1; 				
					alreadyFound = 1;					// Activo el flag y salgo del for
				} 
				continue;
			}
		
			if(guessedLetter == guessWord[i]) {			// Si la letra que puse, no estaba adivinada
				guessedLetters[i] = 1;					// La pongo como adivinada
				actualCorrect++;						// Incremento las correctas
			}		
		
		}

		if(gameEndFlag == 0) {		// Para que cuando haga el exit, no imprima ninguno de estos mensajes.
			if((oldCorrect == actualCorrect) && (alreadyGuessed == 0)) {
				lives--;
				print("\nSorry, wrong guess\n");
				
				if (lives == 0) {
					gameEndFlag = 1;
				}
			} else if(alreadyGuessed == 1) {
				print("\nAlready Guessed!!\n");
			} else {
				print("\nCorrect guess :)\n");
			}
		}

		if(actualCorrect == lengthOfWord) {
			printInScreenPosition("Hangman Word: ", initialPosition);
			printInScreenPosition(guessWord, initialPosition + (14*2));
			
		} else if(lives == 0) {
			printInScreenPosition("Number of lives: ", initialPosition + (80*2*2));
			char aux[2] = {lives + '0', '\0'};
			printInScreenPosition(aux, initialPosition + (80*2*2) + (17*2));
		}
		
	
	} // while loop
	
	if(gameEndFlag == 1 && lives != 0) {	
		printInScreenPosition("The user quit early.", initialPosition+(80*2*6));
		printInScreenPosition("The word was: ", initialPosition+(80*2*7));
		printInScreenPosition(guessWord, initialPosition + (80*2*7) + (14*2));	
	} else if (lives == 0) {
		printInScreenPosition("Sorry, your number of lives is 0,", initialPosition+(80*2*6));
		printInScreenPosition("so you lose. :(", initialPosition+(80*2*7));
		printInScreenPosition("The word was: ", initialPosition+(80*2*8));
		printInScreenPosition(guessWord, initialPosition + (80*2*8) + (14*2));	
	} else  {	
		printInScreenPosition("YOU WIN!!! :), the word was: ", initialPosition+(80*2*6));
		printInScreenPosition(guessWord, initialPosition + (80*2*7));	
	} 
		
}


void printInitialHangmanGame() {
	printInScreenPosition("Screen 4", initialPosition + (80*2*16)+ 62);
	printInScreenPosition("Hangman Word: ", initialPosition);
	for(int i = 1; i <= lengthOfWord; i++) { 
		if(guessedLetters[i-1] == 1) {
			char letter = guessWord[i-1];	
			char letterPrint[2] = {letter, '\0'};
			printInScreenPosition(letterPrint, initialPosition + 26 + (i*2));	
		} else {
			printInScreenPosition("-", initialPosition + 26 + (i*2));
		}
	}

	printInScreenPosition("Number of lives: ", initialPosition + (80*2*2));
	char aux[2] = {lives + '0', '\0'};
	printInScreenPosition(aux, initialPosition + (80*2*2) + (17*2));
}



void continueHangman() {
	initializeGame();
}