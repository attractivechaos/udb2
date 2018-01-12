#include "../common.c"

// https://github.com/troydhanson/uthash
// checked out on 2018-01-11
#include "uthash.h"

typedef struct {
	uint32_t key;
	uint32_t cnt;
	UT_hash_handle hh;
} intcell_t;

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, n_unique = 0, z = 0;
	intcell_t *h = 0, *r, *tmp;
	for (i = 0, x = x0; i < n; ++i) {
		uint32_t y;
		x = hash32(x);
		y = get_key(n, x);
		HASH_FIND_INT(h, &y, r);
		if (r == 0) {
			r = (intcell_t*)malloc(sizeof(intcell_t));
			r->key = y, r->cnt = 0;
			HASH_ADD_INT(h, key, r);
			++n_unique;
		}
		z += ++r->cnt;
	}
	HASH_ITER(hh, h, r, tmp) {
		HASH_DEL(h, r);
		free(r);
	}
	fprintf(stderr, "# unique keys: %d; checksum: %u\n", n_unique, z);
}
