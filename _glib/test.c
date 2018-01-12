#include "../common.c"
#include <glib.h>

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x, z = 0;
	GHashTable *h;
	h = g_hash_table_new(NULL, NULL);
	for (i = 0, x = x0; i < n; ++i) {
		gpointer v, ori_key;
		uint32_t key, cnt;
		x = hash32(x);
		key = get_key(n, x);
		if (g_hash_table_lookup_extended(h, GINT_TO_POINTER(key), &ori_key, &v)) {
			cnt = GPOINTER_TO_INT(v) + 1;
		} else {
			cnt = 1;
		}
		g_hash_table_insert(h, GINT_TO_POINTER(key), GINT_TO_POINTER(cnt));
		z += cnt;
	}
	fprintf(stderr, "# unique keys: %d; checksum: %u\n", g_hash_table_size(h), z);
	g_hash_table_destroy(h);
}
