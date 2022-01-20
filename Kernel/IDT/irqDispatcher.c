#include <time.h>
#include <stdint.h>
#include <naiveConsole.h>
#include <lib.h>
#include <interrupts.h>
#include <keyboardDriver.h>

static void int_20();
static void int_21();


void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20();	// timer_handler
			break;
		case 1:
			int_21();	// keyboard_handler()
			break;
	}
	return;
}

void int_20() {
	timer_handler();
}

void int_21() {
	keyboard_handler();
}

