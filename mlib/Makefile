all:run-test run-test-del

run-test:test.c ../common.c m-dict.h
	$(CC) -O2 -Wall $< -o $@

run-test-del:test.c ../common.c m-dict.h
	$(CC) -O2 -Wall -DUDB2_TEST_DEL $< -o $@

clean:
	rm -fr run-*test* a.out
