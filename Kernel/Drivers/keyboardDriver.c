// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboardDriver.h>
#include <naiveConsole.h>
#include <keyboardDriver.h>
#include <scheduler.h>
#include <pipe.h>


static int shiftFlag = 0;
static int mayusFlag = 0;
static unsigned int keyboardBufferSize = 0;
static unsigned char keyboardBuffer[MAX_SIZE];



static char scanCodeToChar[KEYS][2] = {
    {0, 0}, {ESC, ESC}, {'1', '!'}, {'2', '@'}, {'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '^'}, {'7', '&'}, {'8', '*'}, {'9', '('}, {'0', ')'}, {'-', '_'}, {'=', '+'}, {'\b', '\b'}, {'\t', '\t'}, {'q', 'Q'}, {'w', 'W'}, {'e', 'E'}, {'r', 'R'}, {'t', 'T'}, {'y', 'Y'}, {'u', 'U'}, {'i', 'I'}, {'o', 'O'}, {'p', 'P'}, {'[', '{'}, {']', '}'}, {'\n', '\n'}, {0, 0}, {'a', 'A'}, {'s', 'S'}, {'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'}, {'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {';', ':'}, {'\'', '\"'}, {'`', '~'}, {0, 0}, {'\\', '|'}, {'z', 'Z'}, {'x', 'X'}, {'c', 'C'}, {'v', 'V'}, {'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',', '<'}, {'.', '>'}, {'/', '?'}, {0, 0}, {0, 0}, {0, 0}, {' ', ' '}
	};


void keyboard_handler() {
    

    while (kbDetection()) {
        int scanCode = kbScanCode();
        // ncPrintDec(scanCode);
        if (scanCode == SHIFTL || scanCode == SHIFTR) {
            shiftFlag = 1;
        } else if (scanCode == SHIFTL_RELESED || scanCode == SHIFTR_RELESED) {
            shiftFlag = 0;
        } else if (scanCode == CAPSLOCKS) {
            mayusFlag = !mayusFlag;
        }
        else if (scanCode == 129) {
            // printProcessList();
            killForeground();
            // printProcessList();
        }
        if (scanCode > 1 && scanCode < KEYS && scanCodeToChar[scanCode][0] != 0) {
            putCharInBuffer(scanToChar(scanCode));
        }
                        awakeProcessFromKBQueue();
    }

    
}

char scanToChar(int scancode) {
    return scanCodeToChar[scancode][mayusFlag || shiftFlag];
}



void putCharInBuffer(char sc) {
    if (keyboardBufferSize <= MAX_SIZE) {
        keyboardBuffer[keyboardBufferSize++] = sc;
    }
}


char getChar() {
    // FileDes stdin = getStdin();
    while (keyboardBufferSize == 0) {
        getProcessIntoKBQueue();
    }
    char key = keyboardBuffer[0];
    if (keyboardBufferSize > 0) {	
        for (int i = 1; i < keyboardBufferSize; i++) {
            keyboardBuffer[i - 1] = keyboardBuffer[i];
        }
    }
    keyboardBufferSize--;
    return key;
}

