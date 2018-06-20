EXEC = pruebas
CC = gcc
CFILES = testing.c testing.h abb.c abb.h  pila.c pila.h main.c pruebas_alumno.c
CFLAGS = -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes

build: $(CFILES)
	$(CC) $(CFLAGS) $(CFILES) -o pruebas

run: build
	./pruebas

valgrind: build
	valgrind $(VFLAGS) ./pruebas

clean:
	rm -f *.0 pruebas


