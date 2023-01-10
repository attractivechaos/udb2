#include "../common-block.c"
#include "khashl.h"

KHASHL_CMAP_INIT(, blockmap_t, blockmap, udb_data_t, int, hash_fn_block, hash_eq_block)

uint32_t test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	khint_t k;
	udb_data_t t0, t;
	uint8_t block[MAX_BLOCK_LEN];
	blockmap_t *h;

	t0.data = block;
	h = blockmap_init();
	for (i = 0, x = x0; i < n; ++i) {
		khint_t k;
		int absent;
		x = hash32(x);
		t0.len = get_key_block(n, x, t0.data);
		k = blockmap_put(h, t0, &absent);
		if (absent) {
			t.len = t0.len;
			t.data = (uint8_t*)malloc(t.len);
			memcpy(t.data, t0.data, t.len);
			kh_key(h, k) = t;
			kh_val(h, k) = 0;
		}
		z += ++kh_val(h, k);
	}
	fprintf(stderr, "# unique keys: %d; checksum: %u; capacity: %u\n", kh_size(h), z, kh_capacity(h));
	x = kh_size(h);
	for (k = 0; k != kh_end(h); ++k)
		if (kh_exist(h, k))
			free(kh_key(h, k).data);
	blockmap_destroy(h);
	return x;
}
