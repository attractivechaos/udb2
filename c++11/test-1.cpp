#include "../common.c"
#include <unordered_map>

struct Hash32 {
	inline size_t operator()(const uint32_t x) const {
		return hash_fn(x);
	}
};

uint32_t test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	std::unordered_map<uint32_t, uint32_t, Hash32> h;
	for (i = 0, x = x0; i < n; ++i) {
		x = hash32(x);
		auto p = h.emplace(get_key(n, x), 0);
		z += ++p.first->second;
	}
	fprintf(stderr, "# unique keys: %ld; checksum: %u\n", h.size(), z);
	return h.size();
}
