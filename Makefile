CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =

SRCS = $(wildcard cpp/*.c)
OBJS = $(SRCS:cpp/%.c=o/%.o)

.PHONY: all clean

all: teste

teste: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ -lm -lraylib

o/main.o: cpp/main.c
	$(CC) $(CFLAGS) -c $< -o $@

o/%.o: cpp/%.c h/%.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:	
	rm -f teste $(OBJS)
