#include "../common.c"
#include <sparsehash/sparse_hash_map>

typedef google::sparse_hash_map<uint32_t, uint32_t> inthash;

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	inthash h;
	h.set_deleted_key(0xfffffffeu);
	for (i = 0, x = x0; i < n; ++i) {
		x = hash32(x);
		z += ++h[get_key(n, x)];
	}
	fprintf(stderr, "# unique keys: %ld; checksum: %u\n", h.size(), z);
}
