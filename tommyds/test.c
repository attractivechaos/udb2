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
	double time_32_lin(uint32_t n, uint32_t x0);
	double time_32_dyn(uint32_t n, uint32_t x0);
	double time_32_trie(uint32_t n, uint32_t x0);
	double t0, t1;
	uint32_t n = 10000000, x0 = 1;
	if (argc > 1) n = atol(argv[1]);
	t0 = time_rng(n, x0);
	fprintf(stderr, "CPU time spent on RNG: %.3f sec\n", t0);
	t1 = time_32_lin(n, x0);
	fprintf(stderr, "CPU time (n=%d; hashlin): %.3f sec\n", n, t1 - t0);
	t1 = time_32_dyn(n, x0);
	fprintf(stderr, "CPU time (n=%d; hashdyn): %.3f sec\n", n, t1 - t0);
	t1 = time_32_trie(n, x0);
	fprintf(stderr, "CPU time (n=%d; trie): %.3f sec\n", n, t1 - t0);
	return 0;
}

// https://github.com/amadvance/tommyds
// checked out on 2018-01-11
#include "tommyhashlin.h"

typedef struct {
	uint32_t key;
	uint32_t cnt;
	tommy_node node;
} intcell_t;

int compare(const void* arg, const void* obj)
{
	return *(const uint32_t*)arg != ((const intcell_t*)obj)->key;
}

double time_32_lin(uint32_t n, uint32_t x0)
{
	double t;
	uint32_t i, x;
	tommy_hashlin h;
	t = cputime();
	tommy_hashlin_init(&h);
	for (i = 0, x = x0; i < n; ++i) {
		uint32_t key;
		intcell_t *p;
		x = hash32(x);
		key = x % (n/2);
		p = tommy_hashlin_search(&h, compare, &key, tommy_inthash_u32(key));
		if (!p) {
			p = malloc(sizeof(intcell_t));
			p->key = key, p->cnt = 1;
			tommy_hashlin_insert(&h, &p->node, p, tommy_inthash_u32(key));
		} else ++p->cnt;
	}
	fprintf(stderr, "# unique keys: %d\n", tommy_hashlin_count(&h));
	tommy_hashlin_done(&h);
	return cputime() - t;
}

#include "tommyhashdyn.h"
double time_32_dyn(uint32_t n, uint32_t x0)
{
	double t;
	uint32_t i, x;
	tommy_hashdyn h;
	t = cputime();
	tommy_hashdyn_init(&h);
	for (i = 0, x = x0; i < n; ++i) {
		uint32_t key;
		intcell_t *p;
		x = hash32(x);
		key = x % (n/2);
		p = tommy_hashdyn_search(&h, compare, &key, tommy_inthash_u32(key));
		if (!p) {
			p = malloc(sizeof(intcell_t));
			p->key = key, p->cnt = 1;
			tommy_hashdyn_insert(&h, &p->node, p, tommy_inthash_u32(key));
		} else ++p->cnt;
	}
	fprintf(stderr, "# unique keys: %d\n", tommy_hashdyn_count(&h));
	tommy_hashdyn_done(&h);
	return cputime() - t;
}

#include "tommytrie.h"
double time_32_trie(uint32_t n, uint32_t x0)
{
	double t;
	uint32_t i, x;
	tommy_allocator alloc;
	tommy_trie h;
	t = cputime();
	 tommy_allocator_init(&alloc, TOMMY_TRIE_BLOCK_SIZE, TOMMY_TRIE_BLOCK_SIZE);
	tommy_trie_init(&h, &alloc);
	for (i = 0, x = x0; i < n; ++i) {
		uint32_t key;
		intcell_t *p;
		x = hash32(x);
		key = x % (n/2);
		p = tommy_trie_search(&h, key);
		if (!p) {
			p = malloc(sizeof(intcell_t));
			p->key = key, p->cnt = 1;
			tommy_trie_insert(&h, &p->node, p, p->key);
		} else ++p->cnt;
	}
	fprintf(stderr, "# unique keys: %d\n", tommy_trie_count(&h));
	tommy_allocator_done(&alloc);
	return cputime() - t;
}
