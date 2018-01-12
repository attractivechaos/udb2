all:run-test run-test-1

run-test:test.cpp ../common.c
	$(CXX) -O3 -Wall -I. $< -o $@

run-test-1:test-1.cpp ../common.c
	$(CXX) -O3 -Wall -I. $< -o $@

clean:
	rm -f run-test*
