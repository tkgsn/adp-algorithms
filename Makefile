CC := g++
CFLAGS = -g -Wall -I./include -std=c++11

.PHONY: clean

out: src/counter.cpp src/main.cpp
	$(CC) $(CFLAGS) -o out src/counter.cpp src/main.cpp src/noisy_counter.cpp

out_test:
	$(CC) $(CFLAGS) -I./googletest/googletest/include -L./googletest/build/lib -lgtest -pthread -o out_test test/test_main.cpp test/test_counter.cpp test/test_noisy_counter.cpp src/noisy_counter.cpp src/counter.cpp

clean:
	rm -f out
	rm -f out_test