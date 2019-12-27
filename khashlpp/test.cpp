#include "../common.c"
#include "khashl.hpp"

struct aux_hash {
	uint32_t operator() (uint32_t a) const {
		return hash_fn(a);
	};
};

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	klib::KHashMap<uint32_t, int, aux_hash> h;
	for (i = 0, x = x0; i < n; ++i) {
		uint32_t k;
		int absent;
		x = hash32(x);
		k = h.put(get_key(n, x), &absent);
#ifndef UDB2_TEST_DEL
		if (absent) h.value(k) = 0;
		z += ++h.value(k);
#else
		if (absent) ++z;
		else h.del(k);
#endif
	}
	fprintf(stderr, "# unique keys: %d; checksum: %u; capacity: %d\n", h.size(), z, h.n_buckets());
}
