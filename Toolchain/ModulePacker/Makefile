MP=mp.bin
SOURCES=$(wildcard *.c)

all: $(MP)
	@echo "\033[3;36m[Toolchain/ModulePacker compiled]\033[0m"


$(MP): $(SOURCES)
	@gcc $(SOURCES) -o $(MP)

clean:
	-@rm -rf $(MP)
	@echo "\033[3;35m[Toolchain/ModulePacker cleaned]\033[0m"

.PHONY: all clean
