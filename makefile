CC = g++
CFLAGS = -Wall -std=c++11 -O2 -fopenmp
EXEC_NAME = MCMCIsingTri
INCLUDES =
LDFLAGS=-fopenmp 
LIBS =
OBJ_FILES = MCMCTest.o Lattice.o SpinLattice.o DualLattice.o Spin.o DimerEdge.o DimerNode.o MonteCarlo.o ParallelTempering.o

all : $(EXEC_NAME)

clean :
	rm $(EXEC_NAME) $(OBJ_FILES)  

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.cc
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.c
	gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<

