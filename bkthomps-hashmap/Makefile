all:run-test run-test-1a

run-test:test.c ../common.c unordered_map.c unordered_map.h
	$(CC) -O2 -Wall $< unordered_map.c -o $@

run-test-1a:test-1a.c ../common.c unordered_map.c unordered_map.h
	$(CC) -O2 -Wall $< unordered_map.c -o $@

clean:
	rm -f run-test*
