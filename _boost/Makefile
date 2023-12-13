BOOST_ROOT=.

all:run-test run-test-del

run-test:test.cpp ../common.c
	$(CXX) -O3 -Wall -std=c++11 -I$(BOOST_ROOT) $< -o $@

run-test-del:test.cpp ../common.c
	$(CXX) -O3 -Wall -std=c++11 -I$(BOOST_ROOT) -DUDB2_TEST_DEL $< -o $@

clean:
	rm -f run-test*
