CC = g++
CFLAGS = -Wall -Wextra -g
LDFLAGS =

SRCS = $(wildcard cpp/*.cpp)
OBJS = $(SRCS:cpp/%.cpp=o/%.o)

.PHONY: all clean

all: teste

teste: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ -lm -lraylib

o/main.o: cpp/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

o/%.o: cpp/%.cpp h/%.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:	
	rm -f teste $(OBJS)
