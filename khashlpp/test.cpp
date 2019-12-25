#include "../common.c"
#include "khashl.hpp"

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
		return hash32(a.key);
	};
};

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	klib::KHashL<aux_t, aux_hash, aux_eq> h;
	for (i = 0, x = x0; i < n; ++i) {
		aux_t a;
		size_t k;
		int absent;
		x = hash32(x);
		a.key = get_key(n, x), a.cnt = 0;
		k = h.put(a, &absent);
		z += ++h.at(k).cnt;
	}
	fprintf(stderr, "# unique keys: %d; checksum: %u; capacity: %d\n", h.size(), z, h.n_buckets());
}
