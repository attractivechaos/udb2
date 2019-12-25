/* The MIT License

   Copyright (c) 2019 by Attractive Chaos <attractor@live.co.uk>

   Permission is hereby granted, free of charge, to any person obtaining
   a copy of this software and associated documentation files (the
   "Software"), to deal in the Software without restriction, including
   without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#ifndef __AC_KHASHL_H
#define __AC_KHASHL_H

#define AC_VERSION_KHASHL_H "0.1"

#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* compiler specific configuration */

#if UINT_MAX == 0xffffffffu
typedef unsigned int khint32_t;
#elif ULONG_MAX == 0xffffffffu
typedef unsigned long khint32_t;
#endif

#if ULONG_MAX == ULLONG_MAX
typedef unsigned long khint64_t;
#else
typedef unsigned long long khint64_t;
#endif

#ifndef kh_inline
#ifdef _MSC_VER
#define kh_inline __inline
#else
#define kh_inline inline
#endif
#endif /* kh_inline */

#ifndef klib_unused
#if (defined __clang__ && __clang_major__ >= 3) || (defined __GNUC__ && __GNUC__ >= 3)
#define klib_unused __attribute__ ((__unused__))
#else
#define klib_unused
#endif
#endif /* klib_unused */

#define KH_LOCAL static kh_inline klib_unused

typedef khint32_t khint_t;
typedef khint_t khiter_t;

#define __kh_used(flag, i)       (flag[i>>5] >> (i&0x1fU) & 1U)
#define __kh_set_used(flag, i)   (flag[i>>5] |= 1U<<(i&0x1fU))
#define __kh_set_unused(flag, i) (flag[i>>5] &= ~(1U<<(i&0x1fU)))

#define __kh_fsize(m) ((m) < 32? 1 : (m)>>5)

static inline khint_t __kh_h2b(uint32_t hash, uint32_t bits) { return hash * 2654435769U >> (32 - bits); }

#ifndef kroundup32
#define kroundup32(x) (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, ++(x))
#endif

#ifndef kcalloc
#define kcalloc(N,Z) calloc(N,Z)
#endif
#ifndef kmalloc
#define kmalloc(Z) malloc(Z)
#endif
#ifndef krealloc
#define krealloc(P,Z) realloc(P,Z)
#endif
#ifndef kfree
#define kfree(P) free(P)
#endif

#define __KHASHL_TYPE(HType, khkey_t) \
	typedef struct { \
		khint32_t bits, count; \
		khint32_t *used; \
		khkey_t *keys; \
	} HType;

#define __KHASHL_PROTOTYPES(HType, prefix, khkey_t) \
	extern HType *prefix##_init(void); \
	extern void prefix##_destroy(HType *h); \
	extern void prefix##_clear(HType *h); \
	extern khint_t prefix##_getp(const HType *h, const khkey_t *key); \
	extern int prefix##_resize(HType *h, khint_t new_n_buckets); \
	extern khint_t prefix##_putp(HType *h, const khkey_t *key, int *absent); \
	extern void prefix##_del(HType *h, khint_t k);

#define __KHASHL_IMPL_BASIC(SCOPE, HType, prefix) \
	SCOPE HType *prefix##_init(void) { \
		return (HType*)kcalloc(1, sizeof(HType)); \
	} \
	SCOPE void prefix##_destroy(HType *h) { \
		if (!h) return; \
		kfree((void *)h->keys); kfree(h->used); \
		kfree(h); \
	} \
	SCOPE void prefix##_clear(HType *h) { \
		if (h && h->used) { \
			uint32_t n_buckets = 1U << h->bits; \
			memset(h->used, 0, __kh_fsize(n_buckets) * sizeof(khint32_t)); \
			h->count = 0; \
		} \
	}

#define __KHASHL_IMPL_GET(SCOPE, HType, prefix, khkey_t, __hash_fn, __hash_eq) \
	SCOPE khint_t prefix##_getp(const HType *h, const khkey_t *key) { \
		khint_t k, i, last, n_buckets, mask; \
		if (h->keys == 0) return 0; \
		n_buckets = 1U << h->bits; \
		mask = n_buckets - 1U; \
		k = __hash_fn(*key); \
		i = last = __kh_h2b(k, h->bits); \
		while (__kh_used(h->used, i) && !__hash_eq(h->keys[i], *key)) { \
				i = (i + 1U) & mask; \
			if (i == last) return n_buckets; \
		} \
		return !__kh_used(h->used, i)? n_buckets : i; \
	} \
	SCOPE khint_t prefix##_get(const HType *h, khkey_t key) { return prefix##_getp(h, &key); }

#define __KHASHL_IMPL_RESIZE(SCOPE, HType, prefix, khkey_t, __hash_fn, __hash_eq) \
	SCOPE int prefix##_resize(HType *h, khint_t new_n_buckets) { \
		khint32_t *new_used = 0; \
		khint_t j, n_buckets, new_bits, new_mask; \
		kroundup32(new_n_buckets); \
		for (j = 0; j < 32; ++j) \
			if (new_n_buckets>>j&1) break; \
		new_bits = j > 2? j : 2; \
		new_n_buckets = 1U << new_bits; \
		if (h->count > (new_n_buckets>>1) + (new_n_buckets>>2)) return 0; /* requested size is too small */ \
		new_used = (khint32_t*)kmalloc(__kh_fsize(new_n_buckets) * sizeof(khint32_t)); \
		memset(new_used, 0, __kh_fsize(new_n_buckets) * sizeof(khint32_t)); \
		if (!new_used) return -1; /* not enough memory */ \
		n_buckets = h->keys? 1U<<h->bits : 0U; \
		if (n_buckets < new_n_buckets) { /* expand */ \
			khkey_t *new_keys = (khkey_t*)krealloc((void*)h->keys, new_n_buckets * sizeof(khkey_t)); \
			if (!new_keys) { kfree(new_used); return -1; } \
			h->keys = new_keys; \
		} /* otherwise shrink */ \
		new_mask = new_n_buckets - 1; \
		for (j = 0; j != n_buckets; ++j) { \
			khkey_t key; \
			if (!__kh_used(h->used, j)) continue; \
			key = h->keys[j]; \
			__kh_set_unused(h->used, j); \
			while (1) { /* kick-out process; sort of like in Cuckoo hashing */ \
				khint_t k, i; \
				k = __hash_fn(key); \
				i = __kh_h2b(k, new_bits); \
				while (__kh_used(new_used, i)) i = (i + 1) & new_mask; \
				__kh_set_used(new_used, i); \
				if (i < n_buckets && __kh_used(h->used, i)) { /* kick out the existing element */ \
					{ khkey_t tmp = h->keys[i]; h->keys[i] = key; key = tmp; } \
					__kh_set_unused(h->used, i); /* mark it as deleted in the old hash table */ \
				} else { /* write the element and jump out of the loop */ \
					h->keys[i] = key; \
					break; \
				} \
			} \
		} \
		if (n_buckets > new_n_buckets) /* shrink the hash table */ \
			h->keys = (khkey_t*)krealloc((void *)h->keys, new_n_buckets * sizeof(khkey_t)); \
		kfree(h->used); /* free the working space */ \
		h->used = new_used, h->bits = new_bits; \
		return 0; \
	}

#define __KHASHL_IMPL_PUT(SCOPE, HType, prefix, khkey_t, __hash_fn, __hash_eq) \
	SCOPE khint_t prefix##_putp(HType *h, const khkey_t *key, int *absent) { \
		khint_t n_buckets, k, i, last, mask; \
		n_buckets = h->keys? 1U<<h->bits : 0U; \
		*absent = -1; \
		if (h->count >= (n_buckets>>1) + (n_buckets>>2)) { /* rehashing */ \
			if (prefix##_resize(h, n_buckets + 1U) < 0) \
				return n_buckets; \
			n_buckets = 1U<<h->bits; \
		} /* TODO: to implement automatically shrinking; resize() already support shrinking */ \
		mask = n_buckets - 1; \
		k = __hash_fn(*key); \
		i = last = __kh_h2b(k, h->bits); \
		while (__kh_used(h->used, i) && !__hash_eq(h->keys[i], *key)) { \
			i = (i + 1U) & mask; \
			if (i == last) break; \
		} \
		if (!__kh_used(h->used, i)) { /* not present at all */ \
			h->keys[i] = *key; \
			__kh_set_used(h->used, i); \
			++h->count; \
			*absent = 1; \
		} else *absent = 0; /* Don't touch h->keys[i] if present */ \
		return i; \
	} \
	SCOPE khint_t prefix##_put(HType *h, khkey_t key, int *absent) { return prefix##_putp(h, &key, absent); }

#define KHASHL_DECLARE(HType, prefix, khkey_t) \
	__KHASH_TYPE(HType, khkey_t) \
	__KHASH_PROTOTYPES(HType, prefix, khkey_t)

#define KHASHL_INIT(SCOPE, HType, prefix, khkey_t, __hash_fn, __hash_eq) \
	__KHASHL_TYPE(HType, khkey_t) \
	__KHASHL_IMPL_BASIC(SCOPE, HType, prefix) \
	__KHASHL_IMPL_GET(SCOPE, HType, prefix, khkey_t, __hash_fn, __hash_eq) \
	__KHASHL_IMPL_RESIZE(SCOPE, HType, prefix, khkey_t, __hash_fn, __hash_eq) \
	__KHASHL_IMPL_PUT(SCOPE, HType, prefix, khkey_t, __hash_fn, __hash_eq)

#define kh_key(h, x) ((h)->keys[x])
#define kh_size(h) ((h)->count)
#define kh_capacity(h) ((h)->keys? 1U<<(h)->bits : 0U)
#define kh_end(h) kh_capacity(h)

#endif /* __AC_KHASHL_H */
