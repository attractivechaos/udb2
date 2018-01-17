#include "../common.c"

// https://github.com/nothings/stb
// cloned on 2018-01-17
#define STB_DEFINE
#include "stb.h"

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0, size = 0;
	stb_idict *h;
	h = stb_idict_create();
	for (i = 0, x = x0; i < n; ++i) {
		uint32_t key, val = 1;
		x = hash32(x);
		key = get_key(n, x);
		if (stb_idict_get_flag(h, key, &val)) {
			stb_idict_set(h, key, val + 1);
		} else {
			stb_idict_set(h, key, 1);
			++size;
		}
		z += val;
	}
	fprintf(stderr, "# unique keys: %d; checksum: %u\n", size, z);
	stb_idict_destroy(h);
}
