#include "../common.c"
#include <unordered_map>

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	std::unordered_map<uint32_t, uint32_t> h;
	for (i = 0, x = x0; i < n; ++i) {
		x = hash32(x);
		auto p = h.emplace(get_key(n, x), 0);
		z += ++p.first->second;
	}
	fprintf(stderr, "# unique keys: %ld; checksum: %u\n", h.size(), z);
}
