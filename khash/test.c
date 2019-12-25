#include "../common.c"
#include "khash.h"
KHASH_INIT(32a, uint32_t, uint32_t, 1, hash32, kh_int_hash_equal)

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	khash_t(32a) *h;
	h = kh_init(32a);
	for (i = 0, x = x0; i < n; ++i) {
		khint_t k;
		int absent;
		x = hash32(x);
		k = kh_put(32a, h, get_key(n, x), &absent);
#ifndef UDB2_TEST_DEL
		if (absent) kh_val(h, k) = 0;
		z += ++kh_val(h, k);
#else
		if (absent) ++z;
		else kh_del(32a, h, k);
#endif
	}
	fprintf(stderr, "# unique keys: %d; checksum: %u\n", kh_size(h), z);
	kh_destroy(32a, h);
}
