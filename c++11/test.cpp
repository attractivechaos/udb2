#include "../common.c"
#include <unordered_map>

void test_int(uint32_t n, uint32_t x0)
{
	double t;
	uint32_t i, x;
	std::unordered_map<uint32_t, uint32_t> h;
	for (i = 0, x = x0; i < n; ++i) {
		x = hash32(x);
		++h[get_key(n, x)];
	}
	fprintf(stderr, "# unique keys: %ld\n", h.size());
}
