#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/resource.h>

static inline uint32_t hash32(uint32_t key)
{
    key += ~(key << 15);
    key ^=  (key >> 10);
    key +=  (key << 3);
    key ^=  (key >> 6);
    key += ~(key << 11);
    key ^=  (key >> 16);
    return key;
}

static double cputime(void)
{
	struct rusage r;
	getrusage(RUSAGE_SELF, &r);
	return r.ru_utime.tv_sec + r.ru_stime.tv_sec + 1e-6 * (r.ru_utime.tv_usec + r.ru_stime.tv_usec);
}

double time_rng(uint32_t n, uint32_t x0)
{
	uint32_t i, x;
	double t;
	t = cputime();
	for (i = 0, x = x0; i < n; ++i)
		x = hash32(x);
	return cputime() - t;
}

int main(int argc, char *argv[])
{
	double time_32(uint32_t n, uint32_t x0);
	double t0, t1;
	uint32_t n = 10000000, x0 = 1;
	if (argc > 1) n = atol(argv[1]);
	t0 = time_rng(n, x0);
	fprintf(stderr, "CPU time spent on RNG: %.3f sec\n", t0);
	t1 = time_32(n, x0);
	fprintf(stderr, "CPU time (n=%ld): %.3f sec\n", (long)n, t1 - t0);
	return 0;
}

// https://github.com/troydhanson/uthash
// checked out on 2018-01-11
#include "uthash.h"

typedef struct {
	uint32_t key;
	uint32_t cnt;
	UT_hash_handle hh;
} intcell_t;

double time_32(uint32_t n, uint32_t x0)
{
	double t;
	uint32_t i, x, n_unique = 0;
	intcell_t *h = 0, *r, *tmp;
	t = cputime();
	for (i = 0, x = x0; i < n; ++i) {
		uint32_t y;
		x = hash32(x);
		y = x % (n/2);
		HASH_FIND_INT(h, &y, r);
		if (r == 0) {
			r = (intcell_t*)malloc(sizeof(intcell_t));
			r->key = y, r->cnt = 0;
			HASH_ADD_INT(h, key, r);
			++n_unique;
		}
		++r->cnt;
	}
	fprintf(stderr, "# unique keys: %d\n", n_unique);
	HASH_ITER(hh, h, r, tmp) {
		HASH_DEL(h, r);
		free(r);
	}
	return cputime() - t;
}
