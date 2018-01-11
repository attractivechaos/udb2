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
	fprintf(stderr, "CPU time (n=%d): %.3f sec\n", n, t1 - t0);
	return 0;
}

#include <glib.h>

double time_32(uint32_t n, uint32_t x0)
{
	double t;
	uint32_t i, x;
	GHashTable *h;
	t = cputime();
	h = g_hash_table_new_full(g_int_hash, g_int_equal, 0, free);
	for (i = 0, x = x0; i < n; ++i) {
		gpointer v;
		uint32_t key;
		x = hash32(x);
		key = x % (n/2);
		v = g_hash_table_lookup(h, &key);
		if (v == 0) {
			gpointer p, q;
			p = (uint32_t*)malloc(sizeof(uint32_t));
			q = (uint32_t*)malloc(sizeof(uint32_t));
			*(uint32_t*)p = key, *(uint32_t*)q = 1;
			g_hash_table_insert(h, p, q);
		} else ++*(uint32_t*)v;
	}
	fprintf(stderr, "# unique keys: %d\n", g_hash_table_size(h));
	g_hash_table_destroy(h);
	return cputime() - t;
}
