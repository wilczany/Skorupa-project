OBJ = main.o fork.o read.o builtin.o history.o

all: prog

prog: $(OBJ)
	gcc -Wall $(OBJ) -o prog

$(OBJ):fork.h read.h builtin.h history.c

.PHONY: clean

clean:
	rm -f *.o prog