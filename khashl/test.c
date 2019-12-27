#include "../common.c"
#include "khashl.h"

KHASHL_MAP_INIT(, intmap_t, intmap, uint32_t, int, hash_fn, kh_eq_generic)

uint32_t test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	intmap_t *h;
	h = intmap_init();
	for (i = 0, x = x0; i < n; ++i) {
		khint_t k;
		int absent;
		x = hash32(x);
		k = intmap_put(h, get_key(n, x), &absent);
#ifndef UDB2_TEST_DEL
		if (absent) kh_val(h, k) = 0;
		z += ++kh_val(h, k);
#else
		if (absent) ++z;
		else intmap_del(h, k);
#endif
	}
	fprintf(stderr, "# unique keys: %d; checksum: %u; capacity: %u\n", kh_size(h), z, kh_capacity(h));
	x = kh_size(h);
	intmap_destroy(h);
	return x;
}
