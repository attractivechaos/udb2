all:run-test run-test-del

run-test:test.cpp ../common.c
	$(CXX) -O3 -Wall -std=c++14 $< -o $@

run-test-del:test.cpp ../common.c
	$(CXX) -O3 -Wall -std=c++14 -DUDB2_TEST_DEL $< -o $@

clean:
	rm -f run-test*
