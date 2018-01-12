#include "../common.c"

// https://github.com/efficient/libcuckoo
// cloned on 2018-01-12
#include "cuckoohash_map.hh"

struct Hash32 {
	inline size_t operator()(const uint32_t x) const {
		return hash32(x);
	}
};

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	cuckoohash_map<uint32_t, uint32_t, Hash32> h;
	for (i = 0, x = x0; i < n; ++i) {
		x = hash32(x);
		h.upsert(get_key(n, x), [&z](uint32_t &v) { z += ++v; }); // FIXME: not sure if this is correct...
		++z;
	}
	fprintf(stderr, "# unique keys: %ld; checksum: %u\n", h.size(), z);
}
