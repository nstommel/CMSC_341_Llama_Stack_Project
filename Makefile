# Nicholas Stommel - Project 1 Makefile

CCFLAGS = -Wall -ansi -ggdb
DRIVER = Driver
OBJF = $(DRIVER).o Llama.o LlamaNode.o
LNODE_DEP = LlamaNode.cpp LlamaNode.h
LLAMA_DEP = Llama.cpp Llama.h

# Add optional constants for a driver using an external user-created class
# in Llama container
EXT_CLASS = OvCoInt
EXT_DRIVER = driver3
EXT_DEP = $(EXT_CLASS).cpp $(EXT_CLASS).h

SUBMIT_DIR = /afs/umbc.edu/users/s/l/slupoli/pub/cs341/stommel1/proj1/src

all: Driver.out Ext_Driver.out

Driver.out: $(OBJF)
	g++ $(CCFLAGS) $(OBJF) -o Driver.out

# Optional target executable for use with a driver including an external class
Ext_Driver.out: $(EXT_DRIVER).cpp $(EXT_DEP) Llama.o LlamaNode.o
	g++ $(CCFLAGS) Llama.o LlamaNode.o $(EXT_DRIVER).cpp $(EXT_CLASS).cpp -o Ext_Driver.out

$(DRIVER).o: $(DRIVER).cpp $(LNODE_DEP) $(LLAMA_DEP)
	g++ $(CCFLAGS) -c $(DRIVER).cpp

Llama.o: $(LNODE_DEP) $(LLAMA_DEP)
	g++ $(CCFLAGS) -c Llama.cpp

LlamaNode.o: $(LNODE_DEP)
	g++ $(CCFLAGS) -c LlamaNode.cpp

.PHONY: clean
clean:
	rm -f *~ *.o *.out

# Two separate make commands for running executables. Dependency on .out file 
# included so that if make clean was performed and the executable does not exist, 
# the required targets are compiled and linked
.PHONY: run
run: Driver.out
	./Driver.out

.PHONY: run-ext
run-ext: Ext_Driver.out
	./Ext_Driver.out

.PHONY: submit
submit:
	cp Llama.h Llama.cpp Driver.cpp Makefile $(SUBMIT_DIR)

