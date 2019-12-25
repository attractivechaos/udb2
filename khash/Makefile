all:run-test run-test-del run-test-1 run-test-del-1

run-test:test.c ../common.c khash.h
	$(CC) -O2 -Wall $< -o $@

run-test-del:test.c ../common.c khash.h
	$(CC) -O2 -Wall -DUDB2_TEST_DEL $< -o $@

run-test-1:test-1.c ../common.c khash.h
	$(CC) -O2 -Wall $< -o $@

run-test-del-1:test-1.c ../common.c khash.h
	$(CC) -O2 -Wall -DUDB2_TEST_DEL $< -o $@

clean:
	rm -fr run-test* a.out *.dSYM
