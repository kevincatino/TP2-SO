include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: sampleCodeModule sampleDataModule
	@echo "\033[3;36m[Userland compiled]\033[0m"


sampleCodeModule:
	@cd SampleCodeModule; make

sampleDataModule:
	@printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	@cd SampleCodeModule; make clean
	@rm -rf *.bin
	@echo "\033[3;35m[Userland cleaned]\033[0m"


.PHONY: sampleCodeModule all clean
