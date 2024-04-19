# Makefile
# A very simple makefile for compiling a Java program.  This file compiles
# the sim_cache.java file, and relies on javac to compile all its 
# dependencies automatically.
#
# If you require any special options to be passed to javac, modify the
# CFLAGS variable.  You may want to comment out the DEBUG option before
# running your simulations.

CC = g++
OPT = -O3
#OPT = -g
WARN = -Wall
CFLAGS = $(OPT) $(WARN) $(INC) $(LIB)

# List all your .cc files here (source files, excluding header files)
SIM_SRC = sim.cpp

# List corresponding compiled object files here (.o files)
SIM_OBJ = sim.o
 
#################################

# default rule

all: sim
	@echo "my work is done here..."


# rule for making sim_cache

sim: $(SIM_OBJ)
	$(CC) -o sim $(CFLAGS) $(SIM_OBJ) -lm
	@echo "-----------DONE WITH SIM_CACHE-----------"


# generic rule for converting any .cc file to any .o file
 
.cc.o:
	$(CC) $(CFLAGS)  -c $*.cc


# type "make clean" to remove all .o files plus the sim_cache binary

clean:
	rm -f *.o sim


# type "make clobber" to remove all .o files (leaves sim_cache binary)

clobber:
	rm -f *.o



