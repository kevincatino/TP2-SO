#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <idtLoader.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <the_memory_manager.h>

#include <keyboardDriver.h>
#include <clockDriver.h>
#include <scheduler.h>



extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;
static void * const mem_for_mem_manager = (void*)0x1000000; // todo: chequear el valor correcto para mem manager
static void * const managed_memory = (void*)0x1100000; // todo: chequear el valor correcto para managed mem

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void * getStackBase() {
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary() {

	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	load_idt();
	
	return getStackBase();
}


int main() {
	// ncClear();
	
	init_mem_manager(mem_for_mem_manager, managed_memory);

	initScheduler();

	createProcessWrapper(sampleCodeModuleAddress, 1, 1, "Shell");
	ncPrint("HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHa");
	while(1);
	return ((EntryPoint)sampleCodeModuleAddress)();
}
