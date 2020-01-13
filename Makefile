
TARGET = main
CC = g++
CFLAGS = -Wall 

.PHONY: default all clean

default: 
	$(CC) -std=c++11 main.cpp -o $(TARGET) $(CFLAGS)

all: default

OBJECTS = main.cpp
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $@

clean:
	-rm -f *.out
	-rm -f $(TARGET)