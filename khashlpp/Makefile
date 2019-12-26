all:run-test

run-test:test.cpp ../common.c khashl.hpp
	$(CXX) -O3 -Wall $< -o $@

clean:
	rm -f run-test*
