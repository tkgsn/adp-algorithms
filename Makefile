CC := g++
CFLAGS = -g -Wall -I./include -std=c++11

all: compile

compile: src/counter.cpp src/main.cpp
	$(CC) $(CFLAGS) -o out src/counter.cpp src/main.cpp src/noisy_counter.cpp

clean:
	rm out