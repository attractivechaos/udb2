#include "../common.c"

// https://github.com/sparsehash/sparsehash
// cloned on 2018-01-12
#include <sparsehash/dense_hash_map>

struct Hash32 {
	inline size_t operator()(const uint32_t x) const {
		return hash32(x);
	}
};

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	google::dense_hash_map<uint32_t, uint32_t, Hash32> h;
	h.set_empty_key(0xffffffffu);
	h.set_deleted_key(0xfffffffeu);
	for (i = 0, x = x0; i < n; ++i) {
		x = hash32(x);
		z += ++h[get_key(n, x)];
	}
	fprintf(stderr, "# unique keys: %ld; checksum: %u\n", h.size(), z);
}
