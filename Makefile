CXXFLAGS = -std=c++11 -O2

.PhONY: all

all: add_rev.out quantizedcp.out testcp.out

add_rev.out: add_rev.cpp
	$(CXX) $(CXXFLAGS) -o add_rev.out add_rev.cpp

quantizedcp.out: quantizedcp.cpp
	$(CXX) $(CXXFLAGS) -o quantizedcp.out quantizedcp.cpp

testcp.out: testcp.cpp
	$(CXX) $(CXXFLAGS) -o testcp.out testcp.cpp
