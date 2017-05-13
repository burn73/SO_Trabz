all: biblioteca clean

biblioteca: biblioteca.o main.o
	gcc -o proj biblioteca.o main.o
	./proj ${ARGS}

biblioteca.o: biblioteca.c
	gcc -o biblioteca.o -c biblioteca.c -W -Wall -ansi -pedantic

main.o: main.c biblioteca.h
	gcc -o main.o -c main.c -W -Wall -ansi -pedantic

clean:
	rm -rf *.o