all:run-test

run-test:test.cpp ../common.c khash.hpp
	$(CXX) -O3 -Wall $< -o $@

clean:
	rm -f run-test*
