TARGET = i-banco
CC = gcc
CFLAGS = -g -Wall -pedantic -pthread
LDFLAGS=-lpthread
EXCLUDE = i-banco-terminal.c
.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(filter-out $(EXCLUDE), $(wildcard *.c)))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	-rm -f *.o
	-rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
