CC := g++
CFLAGS = -g -Wall -I./include -std=c++11

.PHONY: clean

out: src/counter.cpp src/main.cpp
	$(CC) $(CFLAGS) -o out src/counter.cpp src/main.cpp src/noisy_counter.cpp

out_test:
	$(CC) $(CFLAGS) -I./googletest/googletest/include -L./googletest/build/lib -lgtest_main -lgtest -pthread -o out_test test/test.cpp

clean:
	rm out