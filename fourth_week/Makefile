CXX = clang++

all: scc

scc: scc.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f *.o scc

.PHONY: all clean
