#include <stdio.h>
#include <unistd.h>
#include <string.h>
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

typedef struct {
	uint8_t data[16];
} udb_data_t;

static inline uint32_t get_key_block(const uint32_t n, const uint32_t x, uint8_t data[16])
{
	uint32_t y;
	uint64_t z;
	y = hash32(x % (n>>2));
	z = (uint64_t)y << 32 | y;
	memcpy(&data[0], &z, 8);
	memcpy(&data[8], &z, 8);
	return y;
}

/**********************************************
 * For testing key generation time (baseline) *
 **********************************************/

uint64_t traverse_rng(uint32_t n, uint32_t x0)
{
	uint8_t data[16];
	uint64_t sum = 0;
	uint32_t i, x;
	for (i = 0, x = x0; i < n; ++i) {
		x = hash32(x);
		sum += get_key_block(n, x, data);
	}
	return sum;
}

/*****************************************************
 * This is the hash function used by almost everyone *
 *****************************************************/

static inline uint32_t murmur_32_scramble(uint32_t k)
{
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    return k;
}

uint32_t murmur3_32(const uint8_t* key, size_t len, uint32_t seed) // from wiki
{
	uint32_t h = seed;
    uint32_t k;
	size_t i;
    for (i = len >> 2; i; i--) {
        memcpy(&k, key, sizeof(uint32_t));
        key += sizeof(uint32_t);
        h ^= murmur_32_scramble(k);
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
    }
    k = 0;
    for (i = len & 3; i; i--) {
        k <<= 8;
        k |= key[i - 1];
    }
    h ^= murmur_32_scramble(k);
	h ^= len;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}

static inline uint32_t hash_fn_block(const udb_data_t x)
{
	return murmur3_32(x.data, 16, 11);
}

static inline int hash_eq_block(const udb_data_t x, const udb_data_t y)
{
	return memcmp(x.data, y.data, 16) == 0;
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
