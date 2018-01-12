all:run-test run-test-1

run-test:test.c ../common.c
	$(CC) -O2 -Wall $< tommy.c -o $@

run-test-1:test-1.c ../common.c
	$(CC) -O2 -Wall $< tommy.c -o $@

clean:
	rm -f run-test*
