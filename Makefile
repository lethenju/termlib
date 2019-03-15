# Makefile for Termlib source and examples
# MIT License
# Julien LE THENO

all: setup clock 
# Chosen example

SHELL:=/bin/bash
BUILD_DIR=build
OBJECTS_DIR=$(BUILD_DIR)/obj
EXE_DIR=$(BUILD_DIR)/exe
SRC_DIR=src
EXAMPLES_DIR=examples
INC_DIR+=inc log_system/inc
INC_PARAM=$(foreach d, $(INC_DIR), -I$d)
F1_EXISTS=$(shell [ -e $(BUILD_DIR) ] && echo Y || echo N )

### EXAMPLES TARGETS

# CLOCK EXAMPLE
clock: clean lib clock.o
	gcc -o $(EXE_DIR)/clock_exe $(OBJECTS_DIR)/* -lpthread -lm

clock.o: $(EXAMPLES_DIR)/clock.c 
	gcc -g -c $(EXAMPLES_DIR)/clock.c $(SRC_DIR) $(INC_PARAM) -o  $(OBJECTS_DIR)/clock.o

# MAIN EXAMPLE
main: clean lib main.o 
	gcc -o $(EXE_DIR)/main_exe $(OBJECTS_DIR)/* log_system/build/obj/*.o -lpthread -lm

main.o: log_system_lib $(EXAMPLES_DIR)/main.c
	gcc -g -c $(EXAMPLES_DIR)/main.c $(SRC_DIR) $(INC_PARAM) -o  $(OBJECTS_DIR)/main.o

# PHYSICS EXAMPLE
physics: clean lib physics.o
	gcc -o $(EXE_DIR)/physics_exe $(OBJECTS_DIR)/* -lpthread -lm

physics.o: $(EXAMPLES_DIR)/physics.c $(SRC_DIR)/termlib.h $(SRC_DIR)/screen.h
	gcc -g -c $(EXAMPLES_DIR)/physics.c $(INC_PARAM) -o  $(OBJECTS_DIR)/physics.o

### END EXAMPLE TARGETS

###

log_system_lib:
	@echo "Building log system lib"
	cd log_system && $(MAKE) lib

log_system_server:
	cd log_system && $(MAKE) server
		

### LIB TARGET 

lib : setup clean log_system_lib screen.o cursor.o termlib.o

screen.o: $(SRC_DIR)/screen.c
	gcc -g -c $(SRC_DIR)/screen.c $(INC_PARAM) -o  $(OBJECTS_DIR)/screen.o

cursor.o: $(SRC_DIR)/cursor.c
	gcc -g -c $(SRC_DIR)/cursor.c $(INC_PARAM) -o  $(OBJECTS_DIR)/cursor.o

termlib.o: $(SRC_DIR)/termlib.c
	gcc -g -c $(SRC_DIR)/termlib.c $(INC_PARAM) -o  $(OBJECTS_DIR)/termlib.o

### END LIB TARGET


setup:
ifeq ($(F1_EXISTS),N)
	@echo "Creating build directory"
	mkdir $(BUILD_DIR)
	mkdir $(EXE_DIR)
	mkdir $(OBJECTS_DIR)
endif

make clean:
	rm -f $(EXE_DIR)/*
	rm -f $(OBJECTS_DIR)/*
	cd log_system && $(MAKE) clean
	

make rebuild: clean all
