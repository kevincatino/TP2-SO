// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <naiveConsole.h>
#include <lib.h>
#include <clockDriver.h>

#define ZERO_EXCEPTION_ID 0
#define OPCODE_EXCEPTION_ID 6 


static void zero_division();
static void opcode_exception();
void printRegisters();
uint64_t hexaToInt(char *s);
// int strlength(char * string);
int power(int num, int p);


void exceptionDispatcher(int exception, uint64_t *regs) { 
	if (exception == ZERO_EXCEPTION_ID){
		zero_division();
	} else if(exception == OPCODE_EXCEPTION_ID) {
		opcode_exception();
	}

	ncNewline();
	printRegisters(regs); 
	ncPrintStringColour("Rebooting the system...", YELLOW);
	sleep(5); 	// dejamos tiempo para ver los registros
	ncClear(); 	// para limpiar la terminal
}


static void zero_division() {
	ncPrintStringColour("Exception 0: Zero Division", RED);
}

static void opcode_exception(){		
	ncPrintStringColour("Exception 6: Invalid Operation Code", RED);
}


void printRegisters(uint64_t* registers) {
    char* registersName[17] = {"R15: ", "R14: ", "R13: ", "R12: ", "R11: ", "R10: ", "R9: ",
	"R8: ", "RSI: ", "RDI: ", "RBP: ", "RDX: ", "RCX: ", "RBX: ", "RAX: ", "RIP: ", "RSP: "};
	

    char registersHexa[30];
    ncPrintStringColour("Registers Values:", WHITE);
	ncPrintStringColour("\n", WHITE);
    for (int i = 0; i < 16; i++) {
        intToHexa(registers[i], registersHexa, 8);
        ncPrintStringColour(registersName[i], YELLOW);
        ncPrintStringColour(registersHexa, LIGHT_BLUE);
        ncPrintStringColour("\n", WHITE);
    }
	ncPrintStringColour(registersName[16], YELLOW);
	intToHexa(registers[17+1], registersHexa, 8);
	ncPrintStringColour(registersHexa, LIGHT_BLUE);

	ncPrintStringColour("\n", WHITE);
	
}


uint64_t hexaToInt(char *s) {
	
	uint64_t rta = 0;

	if (*s == '0' && *(s + 1) == 'x')
		s += 2;

	int len = strlength(s);

	for (int i = 0; i < len; i++)
	{
		int c = s[len - 1 - i] - '0';
		if (c < 0 || c > 9)
		{
			c = s[len - 1 - i] - 'A' + 10;
			if (c < 10 || c > 15)
				return 0;
		}
		rta += c * power(16, i);
	}
	return rta;
}

// int strlength(char * string) {
// 	int i=0;
// 	while(string[i]!=0) {
// 		i++;
// 	}
// 	return i;
// }

int power(int num, int p) {
	int res = 1;
	for(int i = 0; i < p; i++) {
		res *= 10;
	}
	return num*res;
}