CXX = g++
CXXFLAGS = -O3 -std=c++20 -pthread -Wall

all: bench

bench: bench.cpp
	$(CXX) $(CXXFLAGS) bench.cpp -o bench

clean:
	rm -f bench
