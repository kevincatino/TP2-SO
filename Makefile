
all:  bootloader kernel userland image
	@echo "\033[1;3;36m[Proyect compiled]\033[0m"


bootloader:
	@cd Bootloader; make all

kernel:
	@cd Kernel; make all

userland:
	@cd Userland; make all

image: kernel bootloader userland
	@cd Image; make all

clean:
	@cd Bootloader; make clean
	@cd Image; make clean
	@cd Kernel; make clean
	@cd Userland; make clean
	@echo "\033[1;3;35m[Proyect cleaned]\033[0m"


.PHONY: bootloader image collections kernel userland all clean
