#include "../common.c"
#include <map>

uint32_t test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	std::map<uint32_t, uint32_t> h;
	for (i = 0, x = x0; i < n; ++i) {
		x = hash32(x);
		z += ++h[get_key(n, x)];
	}
	fprintf(stderr, "# unique keys: %ld; checksum: %u\n", h.size(), z);
	return h.size();
}
