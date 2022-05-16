include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard *.c)
SOURCES_C=$(wildcard LibC/*.c)	
SOURCES_TEST=$(wildcard tests/*.c)	
SOURCES_FUNC=$(wildcard Functionalities/*.c)	
SOURCES_ASM=$(wildcard LibASM/*.asm)


OBJECTS=$(SOURCES:.c=.o)
OBJECTS_LIBC=$(SOURCES_C:.c=.o) 
OBJECTS_TEST=$(SOURCES_TEST:.c=.o)
OBJECTS_FUNC=$(SOURCES_FUNC:.c=.o)  
OBJECTS_LIBASM=$(SOURCES_ASM:.asm=.o)
	


all: $(MODULE)

$(MODULE): $(OBJECTS) $(OBJECTS_LIBC) $(OBJECTS_TEST) $(OBJECTS_FUNC) $(OBJECTS_LIBASM) 
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld _loader.o $(filter-out _loader.o,$(OBJECTS)) $(OBJECTS_LIBC) $(OBJECTS_TEST) $(OBJECTS_FUNC) $(OBJECTS_LIBASM) -o ../$(MODULE)
	@$(LD) $(LDFLAGS) -I./include -T sampleCodeModule.ld _loader.o $(filter-out _loader.o,$(OBJECTS)) $(OBJECTS_LIBC) $(OBJECTS_TEST) $(OBJECTS_FUNC) $(OBJECTS_LIBASM) --oformat=elf64-x86-64 -o ../0000-sampleCodeModule.elf
	@echo "\033[3;36m[Userland/SampleCodeModule compiled]\033[0m"


%.o : %.asm
	@$(ASM) $(ASMFLAGS) $< -o $@
	@echo "\033[32mCompiled object file \033[0m $@"


%.o: %.c
	@$(GCC) $(GCCFLAGS) -I./include -I./LibC -I./Functionalities -c $< -o $@
	@echo "\033[32mCompiled object file \033[0m $@"


clean:
	-@rm -rf *.o LibASM/*.o LibC/*.o Functionalities/*.o *.bin ../0000-sampleCodeModule.elf
	@echo "\033[3;35m[Userland/SampleCodeModule cleaned]\033[0m"


.PHONY: all clean print