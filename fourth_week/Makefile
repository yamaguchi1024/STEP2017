CXX = clang++
CXXFLAGS = -Wl,-stack_size,0x10000000

all: scc

scc: scc.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f *.o scc

.PHONY: all clean
