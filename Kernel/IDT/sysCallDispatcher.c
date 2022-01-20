#include <sysCallDispatcher.h>
#include <lib.h>


typedef uint64_t (*TypeSysCall)(uint64_t, uint64_t, uint64_t);


static TypeSysCall arraySysCalls[255] = {(TypeSysCall)&sys_read, (TypeSysCall)&sys_write, (TypeSysCall)&sys_get_char, (TypeSysCall)&sys_get_time, (TypeSysCall)&sys_screen_divition, (TypeSysCall)&sys_screen_clear, (TypeSysCall)&sys_memory_dump, (TypeSysCall)&sys_print_user, (TypeSysCall)&sys_print_sudoku_numbers, (TypeSysCall)&sys_print_in_screen_position, (TypeSysCall)&sys_screen_mode, (TypeSysCall)&sys_get_milli_seconds, (TypeSysCall)&sys_print_regs};


uint64_t sysCallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rax) {
    TypeSysCall sysCall = arraySysCalls[rax];
    if(sysCall != 0)
        return sysCall(rdi, rsi, rdx);
    
    return 0;
}


uint64_t sys_write(unsigned int fd, const char * buffer, uint64_t count){
    char color;
    if (fd == 1) {
        color = WHITE;
    } else if (fd == 2) {
        color = RED;
    } else {
        ncPrint("Error in file descriptor");
        ncNewline();
        return 0;
    }
    
    while (count-- && *buffer != 0) {
        ncPrintCharColour(*buffer, color);
        buffer++;
    }
    return count == 0;
}


uint64_t sys_read(unsigned int fd, char* buffer, uint64_t count){

    unsigned char *copyFromBuffer = getBuffer();
    int idx = 0;

    if(fd == STDIN){
        for(idx = 0; copyFromBuffer[idx] != 0 && idx < count; idx++){
            buffer[idx + 1] = copyFromBuffer[idx];
        }
    }
    buffer[idx] = '\0';
    return 0; 
}


char sys_get_char() {
    return getChar();
}


uint64_t sys_get_time(uint64_t mode) {
    return getRTC(mode);
}


void sys_screen_divition() {
    ncScreenDivition();
}


void sys_screen_clear(uint64_t mode) {
    ncClearScreen(mode);
}


uint64_t sys_memory_dump(uint64_t buffer, uint64_t address, uint64_t count){
    char * auxBuffer = (char *) buffer;
	for(int i = 0 ; i < count; i++){
		int position = address + i;
		auxBuffer[i] = (char) asmGetByte(position);
	}
    return 0;
}


void sys_print_user(unsigned int fd, const char * buffer, uint64_t count) {
    while (count-- && *buffer != 0) {
        ncPrintCharColour(*buffer, GREEN);
        buffer++;
    }
}


void sys_print_sudoku_numbers(char* line, uint64_t position, uint64_t orientationX) {
    ncPrintSudokuNumbers(line, position, orientationX);
}


void sys_print_in_screen_position(char* line, uint64_t position) {
    ncPrintInScreenPosition(line, position);
}

void sys_screen_mode(uint64_t mode){
    ncChangeScreen(mode);
}


uint64_t sys_get_milli_seconds(uint64_t time) {
    uint64_t start = milli_seconds_elapsed();
    do {
        _hlt();
    } while(milli_seconds_elapsed() - start < time);
    return 1;
}


void sys_print_regs() {
    uint64_t* regs = getRegs();

    char* registersName[17] = {"R15: ", "R14: ", "R13: ", "R12: ", "R11: ", "R10: ", "R09: ", 
    "R08: ", "RSI: ", "RDI: ", "RBP: ", "RDX: ", "RCX: ", "RBX: ", "RAX: ", "RSP: ", "RIP: "};

    char registersHexa[30];
    ncPrintStringColour("Registers Values:\n", WHITE);
    for (int i = 0; i < 17; i++) {
        intToHexa(regs[i], registersHexa, 8);
        ncPrintStringColour(registersName[i], YELLOW);
        ncPrintStringColour(registersHexa, WHITE);
        ncNewline();
    }
    ncNewline();
}