all:run-test run-test-1

run-test:test.cpp ../common.c
	$(CXX) -O3 -Wall -std=c++11 $< -o $@

run-test-1:test-1.cpp ../common.c
	$(CXX) -O3 -Wall -std=c++11 $< -o $@

clean:
	rm -f run-test*
