#include "../common.c"

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

uint32_t test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	tommy_hashlin h;
	tommy_hashlin_init(&h);
	for (i = 0, x = x0; i < n; ++i) {
		uint32_t key;
		intcell_t *p;
		x = hash32(x);
		key = get_key(n, x);
		p = tommy_hashlin_search(&h, compare, &key, hash_fn(key));
		if (!p) {
			p = malloc(sizeof(intcell_t));
			p->key = key, p->cnt = 1;
			tommy_hashlin_insert(&h, &p->node, p, hash_fn(key));
			++z;
		} else z += ++p->cnt;
	}
	fprintf(stderr, "# unique keys: %d; checksum: %u\n", tommy_hashlin_count(&h), z);
	x = tommy_hashlin_count(&h);
	tommy_hashlin_done(&h);
	return x;
}
