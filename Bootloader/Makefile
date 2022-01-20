all: bmfs pure64
	@echo "\033[3;36m[Bootloader compiled]\033[0m"


bmfs:
	@cd BMFS; $(MAKE) all
pure64:
	@cd Pure64; $(MAKE) all
clean:
	@cd BMFS; $(MAKE) clean
	@cd Pure64; $(MAKE) clean
	@echo "\033[3;35m[Bootloader cleaned]\033[0m"


.PHONY: all bmfs pure64 clean
