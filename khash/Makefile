all:run-test run-test-1

run-test:test.c ../common.c khash.h
	$(CC) -O2 -Wall $< -o $@

run-test-1:test-1.c ../common.c khash.h
	$(CC) -O2 -Wall $< -o $@

clean:
	rm -f run-test*
