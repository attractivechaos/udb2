#include "../common.c"

// https://github.com/amadvance/tommyds
// checked out on 2018-01-11
#include "tommytrie.h"

typedef struct {
	uint32_t key;
	uint32_t cnt;
	tommy_node node;
} intcell_t;

uint32_t test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	tommy_allocator alloc;
	tommy_trie h;
	 tommy_allocator_init(&alloc, TOMMY_TRIE_BLOCK_SIZE, TOMMY_TRIE_BLOCK_SIZE);
	tommy_trie_init(&h, &alloc);
	for (i = 0, x = x0; i < n; ++i) {
		uint32_t key;
		intcell_t *p;
		x = hash32(x);
		key = get_key(n, x);
		p = tommy_trie_search(&h, key);
		if (!p) {
			p = malloc(sizeof(intcell_t));
			p->key = key, p->cnt = 1;
			tommy_trie_insert(&h, &p->node, p, p->key);
			++z;
		} else z += ++p->cnt;
	}
	fprintf(stderr, "# unique keys: %d; checksum: %u\n", tommy_trie_count(&h), z);
	x = tommy_trie_count(&h);
	tommy_allocator_done(&alloc);
	return x;
}
