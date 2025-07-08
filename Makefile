CFLAGS = -Wall -Wextra -g
LFLAGS = -lm -lraylib
BIN = bin
EXE = simulation

SRCS = $(filter-out src/main.cpp, $(wildcard src/*.cpp))
OBJS = $(SRCS:src/%.cpp=$(BIN)/%.o)

.PHONY: all run clean

all: $(BIN) $(EXE)

shared: $(SHARED)/$(BIN) $(SHARED_OBJS)

$(BIN)/%.o: src/%.cpp src/%.h 
	g++ $(CFLAGS) -c $< -o $@

$(EXE): $(OBJS) src/main.cpp
	g++ $(CFLAGS) -c src/main.cpp -o $(BIN)/main.o
	g++ $(CFLAGS) $(OBJS) $(BIN)/main.o -o $@ $(LFLAGS) 

$(BIN):
	@test ! -d $@ && mkdir $@

run: all
	./$(EXE)

clean:
	rm -rf $(BIN) $(EXE)
