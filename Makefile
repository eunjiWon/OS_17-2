PROGRAM=hw4
CC=gcc
SOURCES=hw4.c ExclusiveQueue.c
OBJECTS=$(SOURCES:.c=.o)
CFLAGS=-g

all: $(PROGRAM)

hw4: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(CFLAG) -lpthread $(CFLAGS)

hw4.o: hw4.c
	$(CC) -c $< -o $@ $(CFLAGS)

ExclusiveQueue.o: ExclusiveQueue.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm $(PROGRAM) *.o
