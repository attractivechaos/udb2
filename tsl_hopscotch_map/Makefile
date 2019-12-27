all:run-test run-test-1 run-test-del run-test-del-1

run-test:test.cpp ../common.c
	$(CXX) -O3 -Wall -std=c++11 $< -o $@

run-test-1:test-1.cpp ../common.c
	$(CXX) -O3 -Wall -std=c++11 $< -o $@

run-test-del:test.cpp ../common.c
	$(CXX) -O3 -Wall -std=c++11 -DUDB2_TEST_DEL $< -o $@

run-test-del-1:test-1.cpp ../common.c
	$(CXX) -O3 -Wall -std=c++11 -DUDB2_TEST_DEL $< -o $@

clean:
	rm -f run-test*
