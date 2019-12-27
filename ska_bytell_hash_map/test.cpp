#include "../common.c"

// https://github.com/skarupke/flat_hash_map
// cloned on 2018-09-29
#include "bytell_hash_map.hpp"

struct Hash32 {
	inline size_t operator()(const uint32_t x) const {
		return hash_fn(x);
	}
	//typedef ska::power_of_two_hash_policy hash_policy;
};

uint32_t test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	ska::bytell_hash_map<uint32_t, uint32_t, Hash32> h;
	h.max_load_factor(0.75f);
	for (i = 0, x = x0; i < n; ++i) {
		x = hash32(x);
#ifndef UDB2_TEST_DEL
		z += ++h[get_key(n, x)];
#else
		auto p = h.insert(std::pair<uint32_t, uint32_t>(get_key(n, x), i));
		if (p.second == false) h.erase(p.first);
#endif
	}
	fprintf(stderr, "# unique keys: %ld; checksum: %u\n", h.size(), z);
	return h.size();
}
