#include "../common-block.c"
#include "khashl.h"

KHASHL_CMAP_INIT(, blockmap_t, blockmap, udb_data_t, int, hash_fn_block, hash_eq_block)

uint32_t test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	udb_data_t t;
	blockmap_t *h;
	h = blockmap_init();
	for (i = 0, x = x0; i < n; ++i) {
		khint_t k;
		int absent;
		x = hash32(x);
		get_key_block(n, x, t.data);
		k = blockmap_put(h, t, &absent);
#ifndef UDB2_TEST_DEL
		if (absent) kh_val(h, k) = 0;
		z += ++kh_val(h, k);
#else
		if (absent) ++z;
		else blockmap_del(h, k);
#endif
	}
	fprintf(stderr, "# unique keys: %d; checksum: %u; capacity: %u\n", kh_size(h), z, kh_capacity(h));
	x = kh_size(h);
	blockmap_destroy(h);
	return x;
}
