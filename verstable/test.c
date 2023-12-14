#include "../common.c"

#define NAME intmap_t
#define KEY_TY uint32_t
#define VAL_TY int
#define HASH_FN hash_fn
#define CMPR_FN vt_cmpr_integer
#include "verstable.h"

uint32_t test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	intmap_t h;
	intmap_t_init(&h);
	for (i = 0, x = x0; i < n; ++i) {
		uint32_t key;
		x = hash32(x);
		key = get_key(n, x);
		size_t ori_size = intmap_t_size(&h);
		intmap_t_itr itr = intmap_t_get_or_insert(&h, key, 0);
#ifndef UDB2_TEST_DEL
		z += ++itr.data->val;
#else
		if (intmap_t_size(&h) == ori_size)
			intmap_t_erase_itr(&h, itr);
		else ++z;
#endif
	}
	fprintf(stderr, "# unique keys: %ld; checksum: %u; capacity: %lu\n",
			intmap_t_size(&h), z, intmap_t_bucket_count(&h));
	x = intmap_t_size(&h);
	intmap_t_cleanup(&h);
	return x;
}
