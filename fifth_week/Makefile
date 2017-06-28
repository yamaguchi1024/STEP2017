CXX = clang++
CXXFLAGS =

all: opt

opt: opt.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f *.o opt

.PHONY: all clean
