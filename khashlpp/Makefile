all:run-test run-test-del

run-test:test.cpp ../common.c khashl.hpp
	$(CXX) -O3 -Wall $< -o $@

run-test-del:test.cpp ../common.c khashl.hpp
	$(CXX) -O3 -Wall -DUDB2_TEST_DEL $< -o $@

clean:
	rm -f run-test*
