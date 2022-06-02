CC=gcc
CFLAGS=-Wall -g -pedantic
INC_D=./inc/
SRC_D=./src/

SOURCES=unbounded_int.c main.c
OBJECTS=$(SOURCES:.c=.o)

all: test

test: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: $(SRC_D)%.c
	$(CC) $(CFLAGS) -I $(INC_D) -o $@ -c $<

clean:
	find . -name "*~" -delete
	find . -name "*.o" -delete
