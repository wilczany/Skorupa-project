OBJ = main.o fork.o read.o builtin.o

all: prog

prog: $(OBJ)
	gcc $(OBJ) -o prog

$(OBJ):fork.h read.h builtin.h

.PHONY: clean

clean:
	rm -f *.o prog