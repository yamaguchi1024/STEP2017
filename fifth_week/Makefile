CXX = clang++
CXXFLAGS = -std=c++11 -O2 -ffast-math

all: 2opt 2.5opt 3opt random divide

2opt: 2opt.o
	$(CXX) $(CXXFLAGS) -o $@ $^

2.5opt: 2.5opt.o
	$(CXX) $(CXXFLAGS) -o $@ $^

3opt: 3opt.o
	$(CXX) $(CXXFLAGS) -o $@ $^

divide: divide.o
	$(CXX) $(CXXFLAGS) -o $@ $^

random: random.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f *.o 2opt 2.5opt 3opt random divide

.PHONY: all clean
