#include "../common.c"
#include <absl/container/flat_hash_map.h>

struct Hash32 {
	inline size_t operator()(const uint32_t x) const {
		return hash_fn(x);
	}
};

uint32_t test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	absl::flat_hash_map<uint32_t, uint32_t, Hash32> h;
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
