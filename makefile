CC = g++
CFLAGS = -std=c++11 -g -O0 -pthread
EXEC_NAME = MCMCIsingTri
INCLUDES =
LIBS =
OBJ_FILES = MCMCTest.o Lattice.o SpinLattice.o DualLattice.o Spin.o DimerEdge.o DimerNode.o MonteCarlo.o ParallelTempering.h

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

