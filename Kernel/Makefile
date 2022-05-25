include Makefile.inc

KERNEL=kernel.bin

MM=BUDDY

SOURCES=$(wildcard *.c)
SOURCES_LIBASM=$(wildcard LibASM/*.asm)
SOURCES_LIBC=$(wildcard LibC/*.c)
SOURCES_DRIVERS=$(wildcard Drivers/*.c)
SOURCES_IDT=$(wildcard IDT/*.c)
SOURCES_UTILITIES=$(wildcard utilities/*.c)



OBJECTS=$(SOURCES:.c=.o)
OBJECTS_LIBASM=$(SOURCES_LIBASM:.asm=.o)
OBJECTS_LIBC=$(SOURCES_LIBC:.c=.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
OBJECTS_IDT=$(SOURCES_IDT:.c=.o)
OBJECTS_UTILITIES=$(SOURCES_UTILITIES:.c=.o)


LOADERSRC=loader.asm
LOADEROBJECT=$(LOADERSRC:.asm=.o)



STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_LIBASM) $(OBJECTS_LIBC) $(OBJECTS_DRIVERS) $(OBJECTS_IDT) $(OBJECTS_UTILITIES)
	@$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_LIBASM) $(OBJECTS_LIBC) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_IDT) $(OBJECTS_UTILITIES)
	@$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_LIBASM) $(OBJECTS_LIBC) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_IDT) $(OBJECTS_UTILITIES)
	@echo "\033[3;36m[Kernel compiled]\033[0m"


%.o: %.c
	@$(GCC) $(GCCFLAGS) -D$(MM) -I./include -I./Drivers -I./IDT -I./LibC -I./utilities -c $< -o $@
	@echo "\033[32mCompiled object file \033[0m $@"


%.o : %.asm
	@$(ASM) $(ASMFLAGS) $< -o $@
	@echo "\033[32mCompiled object file \033[0m $@"


$(LOADEROBJECT):
	@$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	-@rm -rf LibC/*.o LibASM/*.o *.o *.bin Drivers/*.o IDT/*.o *.o *.bin kernel.elf utilities/*.o
	@echo "\033[3;35m[Kernel cleaned]\033[0m"

.PHONY: all clean