all:run-test

run-test:test.cpp ../common.c hash_map.hpp
	$(CXX) -O3 -Wall -std=c++11 -I. $< -o $@

clean:
	rm -f run-test*
