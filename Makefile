CXX = clang++
CXXFLAGS = -Wall -std=c++11 -O2

all: main

main: main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f *.o main

.PHONY: all clean
