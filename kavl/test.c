#include "../common.c"
#include "kavl.h"

typedef struct aux_s {
	uint32_t key, cnt;
	KAVL_HEAD(struct aux_s) head;
} aux_t;
#define aux_cmp(a, b) (((a)->key > (b)->key) - ((a)->key < (b)->key))
KAVL_INIT(32, aux_t, head, aux_cmp)

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	aux_t *root = 0, *p, *q;
	p = (aux_t*)malloc(sizeof(aux_t));
	for (i = 0, x = x0; i < n; ++i) {
		x = hash32(x);
		p->key = get_key(n, x), p->cnt = 0;
		q = kavl_insert(32, &root, p, 0);
		if (p == q) p = (aux_t*)malloc(sizeof(aux_t));
		z += ++q->cnt;
	}
	free(p);
	fprintf(stderr, "# unique keys: %d; checksum: %u\n", kavl_size(head, root), z);
	kavl_free(aux_t, head, root, free);
}
