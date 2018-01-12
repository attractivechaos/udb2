#include "../common.c"
#include "khash.h"

#if !defined(ALT) || ALT == 0
KHASH_INIT(32a, uint32_t, uint32_t, 1, hash32, kh_int_hash_equal)

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x;
	khash_t(32a) *h;
	h = kh_init(32a);
	for (i = 0, x = x0; i < n; ++i) {
		khint_t k;
		int absent;
		x = hash32(x);
		k = kh_put(32a, h, get_key(n, x), &absent);
		if (absent) kh_val(h, k) = 0;
		++kh_val(h, k);
	}
	kh_destroy(32a, h);
	fprintf(stderr, "# unique keys: %d\n", kh_size(h));
}
#elif ALT == 1
typedef struct {
	uint32_t key, cnt;
} aux_t;
#define aux_eq(a, b) ((a).key == (b).key)
#define aux_hash(a) (hash32((a).key))
KHASH_INIT(32b, aux_t, char, 0, aux_hash, aux_eq)

void test_int(uint32_t n, uint32_t x0)
{
	uint32_t i, x;
	khash_t(32b) *h;
	h = kh_init(32b);
	for (i = 0, x = x0; i < n; ++i) {
		aux_t a;
		khint_t k;
		int absent;
		x = hash32(x);
		a.key = get_key(n, x), a.cnt = 0;
		k = kh_put(32b, h, a, &absent);
		++kh_key(h, k).cnt;
	}
	kh_destroy(32b, h);
	fprintf(stderr, "# unique keys: %d\n", kh_size(h));
}
#endif
