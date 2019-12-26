#include "../common.c"
#include "khash.h"

typedef struct {
	uint32_t key, cnt;
} aux_t;
#define aux_eq(a, b) ((a).key == (b).key)
#define aux_hash(a) (hash_fn((a).key))
KHASH_INIT(32b, aux_t, char, 0, aux_hash, aux_eq)

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	khash_t(32b) *h;
	h = kh_init(32b);
	for (i = 0, x = x0; i < n; ++i) {
		aux_t a;
		khint_t k;
		int absent;
		x = hash32(x);
		a.key = get_key(n, x), a.cnt = 0;
		k = kh_put(32b, h, a, &absent);
#ifndef UDB2_TEST_DEL
		z += ++kh_key(h, k).cnt;
#else
		if (absent) ++z;
		else kh_del(32b, h, k);
#endif
	}
	fprintf(stderr, "# unique keys: %d; checksum: %u\n", kh_size(h), z);
	kh_destroy(32b, h);
}
