all:run-test

run-test:test.c ../common.c khashl.h
	$(CC) -O2 -Wall $< -o $@

clean:
	rm -fr run-test*
