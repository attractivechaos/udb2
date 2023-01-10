#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/resource.h>

/***********************************
 * Measuring CPU time and peak RSS *
 ***********************************/

static double cputime(void)
{
	struct rusage r;
	getrusage(RUSAGE_SELF, &r);
	return r.ru_utime.tv_sec + r.ru_stime.tv_sec + 1e-6 * (r.ru_utime.tv_usec + r.ru_stime.tv_usec);
}

static long peakrss(void)
{
	struct rusage r;
	getrusage(RUSAGE_SELF, &r);
#ifdef __linux__
	return r.ru_maxrss * 1024;
#else
	return r.ru_maxrss;
#endif
}

/******************
 * Key generation *
 ******************/

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

static inline uint32_t get_key(const uint32_t n, const uint32_t x)
{
	return hash32(x % (n>>2));
}

/**********************************************
 * For testing key generation time (baseline) *
 **********************************************/

uint64_t traverse_rng(uint32_t n, uint32_t x0)
{
	uint64_t sum = 0;
	uint32_t i, x;
	for (i = 0, x = x0; i < n; ++i) {
		x = hash32(x);
		sum += get_key(n, x);
	}
	return sum;
}

/*****************************************************
 * This is the hash function used by almost everyone *
 *****************************************************/

static inline uint32_t hash_fn(uint32_t key)
{
	return key;
}

/*****************
 * Main function *
 *****************/

int main(int argc, char *argv[])
{
	uint32_t test_int(uint32_t n, uint32_t x0);
	int c;
	double t, t0;
	uint32_t i, m = 5, max = 50000000, n = 10000000, x0 = 1, step;
	uint64_t sum;
	long m0;

	while ((c = getopt(argc, argv, "n:x:0:k:")) >= 0) {
		if (c == 'n') n = atol(optarg);
		else if (c == 'x') max = atol(optarg);
		else if (c == '0') x0 = atol(optarg);
		else if (c == 'k') m = atoi(optarg);
	}

	t = cputime();
	sum = traverse_rng(n, x0);
	t0 = cputime() - t;
	fprintf(stderr, "CPU time spent on RNG: %.3f sec; total sum: %lu\n", t0, (unsigned long)sum);
	m0 = peakrss();

	step = (max - n) / m;
	for (i = 0; i <= m; ++i, n += step) {
		double t, mem;
		uint32_t size;
		t = cputime();
		size = test_int(n, x0);
		t = cputime() - t;
		mem = (peakrss() - m0) / 1024.0 / 1024.0;
		printf("%d\t%d\t%.3f\t%.3f\t%.4f\t%.4f\n", i, n, t, mem, t * 1e6 / n, mem * 1e6 / size);
	}
	return 0;
}
