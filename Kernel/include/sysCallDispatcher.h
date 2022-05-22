#ifndef _SYSCALLDISPATCHER_H_
#define _SYSCALLDISPATCHER_H_

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#include <time.h>
#include <keyboardDriver.h>
#include <clockDriver.h>
#include <naiveConsole.h>
#include <time.h>

uint64_t sysCallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
uint64_t sys_write(unsigned int fd, const char* buffer, uint64_t count);
uint64_t sys_read(unsigned int fd, char* buffer, uint64_t count);
void sys_print_user(unsigned int fd, const char * buffer, uint64_t count);
char sys_get_char();
uint64_t sys_get_time(uint64_t mode);
void sys_screen_divition();
void sys_screen_clear(uint64_t mode);
uint64_t sys_memory_dump(uint64_t buffer, uint64_t address, uint64_t amount);
void sys_print_sudoku_numbers(char* line, uint64_t position, uint64_t orientationX);
void sys_print_in_screen_position(char* line, uint64_t position);
void sys_screen_mode(uint64_t mode);
uint64_t sys_get_milli_seconds(uint64_t time);
void sys_print_regs();

extern uint64_t asmGetByte(uint64_t position);
extern void _hlt(void);

#endif
