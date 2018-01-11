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

double time_rng(uint64_t n, uint64_t x0)
{
	uint64_t i, x;
	double t;
	t = cputime();
	for (i = 0, x = x0; i < n; ++i)
		x = hash32(x);
	return cputime() - t;
}

#include "khash.h"
KHASH_INIT(32, uint32_t, uint32_t, 1, hash32, kh_int_hash_equal)

double time_32(uint64_t n, uint64_t x0)
{
	double t;
	uint64_t i, x;
	khash_t(32) *h;
	t = cputime();
	h = kh_init(32);
	for (i = 0, x = x0; i < n; ++i) {
		khint_t k;
		int absent;
		x = hash32(x);
		k = kh_put(32, h, x % (n/2), &absent);
		if (absent) kh_val(h, k) = 0;
		++kh_val(h, k);
	}
	kh_destroy(32, h);
	fprintf(stderr, "# unique keys: %d\n", kh_size(h));
	return cputime() - t;
}

int main(int argc, char *argv[])
{
	double t0, t1;
	uint64_t n = 10000000, x0 = 1;
	if (argc > 1) n = atol(argv[1]);
	t0 = time_rng(n, x0);
	fprintf(stderr, "CPU time spent on RNG: %.3f sec\n", t0);
	t1 = time_32(n, x0);
	fprintf(stderr, "CPU time (n=%ld): %.3f sec\n", (long)n, t1 - t0);
	return 0;
}
