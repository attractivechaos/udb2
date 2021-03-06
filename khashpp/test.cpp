#include "../common.c"
#include "khash.hpp"

struct aux_t {
	uint32_t key, cnt;
};

struct aux_eq {
	bool operator() (const aux_t &a, const aux_t &b) const {
		return a.key == b.key;
	};
};

struct aux_hash {
	uint32_t operator() (const aux_t &a) const {
		return hash_fn(a.key);
	};
};

uint32_t test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	klib::KHash<aux_t, aux_hash, aux_eq> h;
	for (i = 0, x = x0; i < n; ++i) {
		aux_t a;
		size_t k;
		int absent;
		x = hash32(x);
		a.key = get_key(n, x), a.cnt = 0;
		k = h.put(a, &absent);
		z += ++h.at(k).cnt;
	}
	fprintf(stderr, "# unique keys: %d; checksum: %u\n", h.size(), z);
	return h.size();
}
