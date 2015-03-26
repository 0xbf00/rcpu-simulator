# General settings
CC 						:= gcc
CC_FLAGS  				:= -std=c11 -O0 -ggdb -Wall

SRC 					:= $(shell find . -name *.c)
OBJ_FILES               := $(SRC:%.c=%.o)
OUT_FILE 				:= bin/rcpu_simulator

# Build rcpu_simulator
rcpu_simulator: $(OBJ_FILES)
	@echo [Debug] rcpu_simulator: Linking object files
	@$(CC) -o $(OUT_FILE) $^

# compile individual object files
%.o: %.c
	@echo Compiling $<
	@$(CC) $(CC_FLAGS) -c -o $@ $<

all: rcpu_simulator

clean:
	@echo Cleaning up
	@find . -name '*.o' -exec rm -f {} \;

.PHONY: clean
