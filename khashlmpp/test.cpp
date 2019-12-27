#include "../common.c"
#include "khashlm.hpp"

struct aux_hash {
	uint32_t operator() (uint32_t a) const {
		return hash_fn(a);
	};
};

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	klib::KHashMap<uint32_t, int, 6, aux_hash> h;
	for (i = 0, x = x0; i < n; ++i) {
		uint32_t k;
		int absent;
		x = hash32(x);
		k = h.put(get_key(n, x), &absent);
		if (absent) h.value(k) = 0;
		z += ++h.value(k);
	}
	fprintf(stderr, "# unique keys: %ld; checksum: %u\n", (long)h.size(), z);
}
