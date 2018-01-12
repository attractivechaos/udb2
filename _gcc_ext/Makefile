all:run-test run-test-1

run-test:test.cpp ../common.c
	$(CXX) -O3 -Wall $< -o $@

run-test-1:test-1.cpp ../common.c
	$(CXX) -O3 -Wall $< -o $@

run-test-2:test-2.cpp ../common.c
	$(CXX) -O3 -Wall $< -o $@

clean:
	rm -f run-test*
