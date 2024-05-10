OS := $(shell uname)
LINUX = Linux
WINDOWS = Windows

ASM_FILES = src/functions.asm
C_FILES = src/main.c src/args.c src/string.c src/vec.c src/util.c

ifeq ($(OS),$(LINUX))
	EXECUTABLE_NAME = integrator
	NASM_SYSTEM = elf32
endif
ifeq ($(OS),$(WINDOWS))
	EXECUTABLE_NAME = integrator.exe
	NASM_SYTEM = win32
endif



target:
	$(shell mkdir target)


compile_asm: target $(ASM_FILES)
	$(foreach \
		file,\
		$(ASM_FILES),\
		$(shell nasm \
			-f $(NASM_SYSTEM) \
			$(file) \
			-o $(addprefix target/,$(call path_to_name,$(file)).obj)))
	

compile_c: compile_asm $(C_FILES)
	$(shell gcc \
		-Wall \
		-Wextra \
		-m32 \
		$(wildcard target/*.obj) \
		$(C_FILES) \
		-o $(addprefix target/,$(EXECUTABLE_NAME)))
	

all: compile_c


run: all
	@./target/$(EXECUTABLE_NAME)


clean:
	@rm -rf target


define path_to_name
	$(subst .,_,$(subst /,_,$(1)))
endef 