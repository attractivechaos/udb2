#include "../common.c"

// https://github.com/skarupke/flat_hash_map
// cloned on 2018-09-29
#include "bytell_hash_map.hpp"

struct Hash32 {
	inline size_t operator()(const uint32_t x) const {
		return hash32(x);
	}
	//typedef ska::power_of_two_hash_policy hash_policy;
};

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	ska::bytell_hash_map<uint32_t, uint32_t, Hash32> h;
	h.max_load_factor(0.75f);
	for (i = 0, x = x0; i < n; ++i) {
		x = hash32(x);
		z += ++h[get_key(n, x)];
	}
	fprintf(stderr, "# unique keys: %ld; checksum: %u\n", h.size(), z);
}
