ASM=nasm
BMFS_MBR=bmfs_mbr.sys
PXESTART=pxestart.sys
PURE64=pure64.sys

all: bmfs_mbr.sys pxestart.sys pure64.sys
	@echo "\033[3;36m[Bootloader/Pure64 compiled]\033[0m"

$(BMFS_MBR):
	@$(ASM) src/bootsectors/bmfs_mbr.asm -o $(BMFS_MBR)
$(PXESTART):
	@$(ASM) src/bootsectors/pxestart.asm -o $(PXESTART)
$(PURE64):
	@cd src; $(ASM) pure64.asm -o ../$(PURE64)
clean:
	-@rm -rf *.sys
	@echo "\033[3;35m[Bootloader/Pure64 cleaned]\033[0m"

.PHONY: all clean
