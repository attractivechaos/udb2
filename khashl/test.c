#include "../common.c"
#include "khashl.h"

typedef struct {
	uint32_t x, n;
} cnt_cell_t;

#define myhash(a) (hash32((a).x))
#define myeq(a, b) ((a).x == (b).x)
KHASHL_INIT(, intmap_t, intmap, cnt_cell_t, myhash, myeq)

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	intmap_t *h;
	h = intmap_init();
	for (i = 0, x = x0; i < n; ++i) {
		khint_t k;
		cnt_cell_t t;
		int absent;
		x = hash32(x);
		t.x = get_key(n, x);
		k = intmap_putp(h, &t, &absent);
		if (absent) kh_key(h, k).n = 0;
		z += ++kh_key(h, k).n;
	}
	fprintf(stderr, "# unique keys: %d; checksum: %u; capacity: %u\n", kh_size(h), z, kh_capacity(h));
	intmap_destroy(h);
}
