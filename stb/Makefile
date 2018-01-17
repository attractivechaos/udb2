all:run-test

run-test:test.c ../common.c stb.h
	$(CC) -O2 -Wall $< -o $@

clean:
	rm -f run-test*
