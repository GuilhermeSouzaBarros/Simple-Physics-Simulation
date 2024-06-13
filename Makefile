CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: teste

teste: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ -lm -lraylib

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f teste $(OBJS)
