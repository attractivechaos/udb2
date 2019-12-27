all:run-test run-test-del

run-test:test.cpp ../common.c robin_hood.h
	$(CXX) -O3 -Wall -std=c++11 $< -o $@

run-test-del:test.cpp ../common.c robin_hood.h
	$(CXX) -O3 -Wall -std=c++11 -DUDB2_TEST_DEL $< -o $@

clean:
	rm -f run-test*
