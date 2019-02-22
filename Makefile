CXXFLAGS = -std=c++11 -O2
HEADERS = data.hpp model_file_writer.hpp random_sampler.hpp rank_score.hpp signal_operation.hpp testdata.hpp triple.hpp triple_key_gen.hpp updater.hpp

.PhONY: all
all: add_rev.out quantizedcp.out testcp.out cp.out

add_rev.out: add_rev.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o add_rev.out add_rev.cpp

cp.out: cp.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o cp.out cp.cpp

quantizedcp.out: quantizedcp.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o quantizedcp.out quantizedcp.cpp

testcp.out: testcp.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o testcp.out testcp.cpp
