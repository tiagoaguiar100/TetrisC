# Makefile #

INCLUDES = list.h
OBJECTS = proj3.o list.o

all: $(INCLUDES) $(OBJECTS)
	gcc -ansi -Wall -pedantic -o proj3 $(OBJECTS)

debug:
	gcc -ansi -Wall -pedantic -g -o proj3 proj3.c list.c list.h

clean:
	rm -f *.o *.out proj3
