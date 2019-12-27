#include "../common.c"

// https://github.com/bkthomps/Containers
// checked out on 2018-01-11
#include "unordered_map.h"

static int compare_int(const void *const one, const void *const two) // FIXME: this doesn't compare uint32_t
{
	const int a = *(int *) one;
	const int b = *(int *) two;
	return a - b;
}

static unsigned long hash_int(const void *const key)
{
	return hash_fn(*(uint32_t*)key);
}

uint32_t test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	unordered_map h;
	h = unordered_map_init(sizeof(uint32_t), sizeof(uint32_t), hash_int, compare_int);
	for (i = 0, x = x0; i < n; ++i) {
		uint32_t key, v = 1;
		x = hash32(x);
		key = get_key(n, x);
		unordered_map_put(h, &key, &v);
		z += v;
	}
	fprintf(stderr, "# unique keys: %d; checksum: %u\n", unordered_map_size(h), z);
	x = unordered_map_size(h);
	unordered_map_destroy(h);
	return x;
}
