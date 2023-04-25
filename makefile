OBJ = main.o fork.o read.o

all: prog

prog: $(OBJ)
	gcc $(OBJ) -o prog

$(OBJ): fork.c

.PHONY: clean

clean:
	rm -f *.o prog