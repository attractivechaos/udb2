all:run-test

run-test:test.c ../common.c uthash.h
	$(CC) -O2 -Wall $< -o $@

clean:
	rm -f run-test*
