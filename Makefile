CC := g++
CFLAGS = -g -Wall -I./include

all: compile

compile: src/counter.cpp src/main.cpp
	$(CC) $(CFLAGS) -o out src/counter.cpp src/main.cpp

clean:
	rm out