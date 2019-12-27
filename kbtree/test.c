#include "../common.c"
#include "kbtree.h"

typedef struct {
	uint32_t key, cnt;
} aux_t;
#define aux_cmp(a, b) (((a).key > (b).key) - ((a).key < (b).key))
KBTREE_INIT(32, aux_t, aux_cmp)

uint32_t test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	kbtree_t(32) *h;
	h = kb_init(32, KB_DEFAULT_SIZE);
	for (i = 0, x = x0; i < n; ++i) {
		aux_t a, *p;
		x = hash32(x);
		a.key = get_key(n, x), a.cnt = 1;
		p = kb_getp(32, h, &a);
		if (p == 0) {
			kb_putp(32, h, &a);
			++z;
		} else z += ++p->cnt;
	}
	fprintf(stderr, "# unique keys: %d; checksum: %u\n", kb_size(h), z);
	x = kb_size(h);
	kb_destroy(32, h);
	return x;
}
