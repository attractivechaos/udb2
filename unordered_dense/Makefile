all:run-test run-test-del

run-test:test.cpp ../common.c unordered_dense.h
	$(CXX) -O3 -Wall -std=c++17 $< -o $@

run-test-del:test.cpp ../common.c unordered_dense.h
	$(CXX) -O3 -Wall -std=c++17 -DUDB2_TEST_DEL $< -o $@

clean:
	rm -f run-test*
