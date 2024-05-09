ASM_FILES = src/functions.asm
C_FILES = src/main.c
EXECUTABLE_NAME = integrator


target:
	@mkdir target


all: target
	$(foreach \
		file,\
		$(ASM_FILES),\
		$(shell nasm \
			-f win32 \
			$(file) \
			-o $(addprefix target/,$(call path_to_name,$(file)).o)))
	
	$(foreach \
		file,\
		$(C_FILES),\
		$(shell gcc \
			-m32 \
			-Wall \
			-Wextra \
			$(file) \
			-o $(addprefix target/,$(call path_to_name,$(file))).o))
	
	$(shell gcc \
		-m32 \
		$(wildcard target/*.o) \
		-o target/$(EXECUTABLE_NAME).exe)


run: all
	./target/$(EXECUTABLE_NAME).exe


clean:
	@rm -rf target


define path_to_name
	$(subst .,_,$(subst /,_,$(1)))
endef 