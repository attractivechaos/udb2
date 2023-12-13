#include "../common.c"
#include "m-dict.h"

static inline int oor_equal_p(uint32_t k, unsigned char n)
{
	return (k == (uint32_t)(-n-1));
}

static inline void oor_set(uint32_t *k, unsigned char n)
{
	*k = (uint32_t)(-n-1);
}

DICT_OA_DEF2(intmap, uint32_t, M_OPEXTEND(M_BASIC_OPLIST, OOR_EQUAL(oor_equal_p), OOR_SET(API_2(oor_set))), int, M_BASIC_OPLIST)

uint32_t test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	intmap_t h;
	intmap_init(h);
	for (i = 0, x = x0; i < n; ++i) {
		uint32_t key;
		int *p;
		x = hash32(x);
		key = get_key(n, x);
		p = intmap_get(h, key);
#ifndef UDB2_TEST_DEL
		if (p == 0) {
			intmap_set_at(h, key, 1);
			++z;
		} else z += ++*p;
#else
		if (p == 0) {
			intmap_set_at(h, key, 1);
			++z;
		} else intmap_erase(h, key);
#endif
	}
	fprintf(stderr, "# unique keys: %ld; checksum: %u\n", intmap_size(h), z);
	x = intmap_size(h);
	intmap_clear(h);
	return x;
}
