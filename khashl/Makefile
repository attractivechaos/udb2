all:run-test run-test-del run-block-test

run-test:test.c ../common.c khashl.h
	$(CC) -O2 -Wall $< -o $@

run-test-del:test.c ../common.c khashl.h
	$(CC) -O2 -Wall -DUDB2_TEST_DEL $< -o $@

run-block-test:test-block.c ../common.c khashl.h
	$(CC) -O2 -Wall $< -o $@

clean:
	rm -fr run-*test* a.out
