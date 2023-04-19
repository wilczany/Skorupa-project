all: prog

prog: main.o
	gcc -o skorupa main.o

main.o: main.c
	gcc -c main.c -o main.o

clean:
	rm -f *.o skorupa