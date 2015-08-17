CC=g++
CFLAGS=-I

_OBJ = YAUC.o
_DEP = Unit.h

%.o: %.c $(_DEP)
	$(CC) -c -o $@ $< $(CFLAGS)

yaucMake: $(_OBJ)
	$(CC) -o YAUC $(_OBJ)
