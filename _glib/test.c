#include "../common.c"
#include <glib.h>

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	GHashTable *h;
	h = g_hash_table_new_full(g_int_hash, g_int_equal, 0, free);
	for (i = 0, x = x0; i < n; ++i) {
		gpointer v;
		uint32_t key;
		x = hash32(x);
		key = get_key(n, x);
		v = g_hash_table_lookup(h, &key);
		if (v == 0) {
			gpointer p, q;
			p = (uint32_t*)malloc(sizeof(uint32_t));
			q = (uint32_t*)malloc(sizeof(uint32_t));
			*(uint32_t*)p = key, *(uint32_t*)q = 1;
			g_hash_table_insert(h, p, q);
			++z;
		} else z += ++*(uint32_t*)v;
	}
	fprintf(stderr, "# unique keys: %d; checksum: %u\n", g_hash_table_size(h), z);
	g_hash_table_destroy(h);
}
