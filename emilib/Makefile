all:run-test run-test-del

run-test:test.cpp ../common.c hash_map.hpp
	$(CXX) -O3 -Wall -std=c++11 -I. $< -o $@

run-test-del:test.cpp ../common.c hash_map.hpp
	$(CXX) -O3 -Wall -std=c++11 -DUDB2_TEST_DEL -I. $< -o $@

clean:
	rm -f run-test*
