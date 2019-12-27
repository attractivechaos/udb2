#ifndef __AC_KHASHL_HPP
#define __AC_KHASHL_HPP

#include <functional> // for std::equal_to
#include <cstdlib>
#include <cstring>
#include <stdint.h>

namespace klib {

static inline uint64_t kh_base_hash(uint64_t x) { return x * 11400714819323198485ULL; }

/***********
 * HashSet *
 ***********/

template<class T, class Hash, class Eq = std::equal_to<T>, typename khint_t = uint32_t>
class KHashSet1 {
	khint_t bits, count;
	uint32_t *used;
	T *keys;
	static inline uint32_t __kh_used(const uint32_t *flag, khint_t i) { return flag[i>>5] >> (i&0x1fU) & 1U; };
	static inline void __kh_set_used(uint32_t *flag, khint_t i) { flag[i>>5] |= 1U<<(i&0x1fU); };
	static inline void __kh_set_unused(uint32_t *flag, khint_t i) { flag[i>>5] &= ~(1U<<(i&0x1fU)); };
	static inline khint_t __kh_fsize(khint_t m) { return m<32? 1 : m>>5; }
	static inline khint_t get_bucket(uint64_t hash, int l, int b) { return hash << l >> (64 - b); }
public:
	KHashSet1() : bits(0), count(0), used(0), keys(0) {};
	~KHashSet1() { std::free(used); std::free(keys); };
	inline khint_t n_buckets() const { return used? khint_t(1) << bits : 0; }
	inline khint_t end() const { return n_buckets(); }
	inline khint_t size() const { return count; }
	inline T &key(khint_t x) { return keys[x]; };
	inline bool occupied(khint_t x) const { return (__kh_used(used, x) != 0); }
	void clear(void) { 
		if (!used) return;
		memset(used, 0, __kh_fsize(n_buckets()) * sizeof(uint32_t));
		count = 0;
	}
	khint_t get(const T &key, uint64_t hash, int pre_len) const {
		khint_t i, last, mask, nb;
		if (keys == 0) return 0;
		nb = n_buckets();
		mask = nb - khint_t(1);
		i = last = get_bucket(hash, pre_len, bits);
		while (__kh_used(used, i) && !Eq()(keys[i], key)) {
			i = (i + khint_t(1)) & mask;
			if (i == last) return nb;
		}
		return !__kh_used(used, i)? nb : i;
	}
	int resize(khint_t new_nb, int pre_len) {
		uint32_t *new_used = 0;
		khint_t j = 0, x = new_nb, nb, new_bits, new_mask;
		while ((x >>= khint_t(1)) != 0) ++j;
		if (new_nb & (new_nb - 1)) ++j;
		new_bits = j > 2? j : 2;
		new_nb = khint_t(1) << new_bits;
		if (count > (new_nb>>1) + (new_nb>>2)) return 0; /* requested size is too small */
		new_used = (uint32_t*)std::malloc(__kh_fsize(new_nb) * sizeof(uint32_t));
		memset(new_used, 0, __kh_fsize(new_nb) * sizeof(uint32_t));
		if (!new_used) return -1; /* not enough memory */
		nb = n_buckets();
		if (nb < new_nb) { /* expand */
			T *new_keys = (T*)std::realloc(keys, new_nb * sizeof(T));
			if (!new_keys) { std::free(new_used); return -1; }
			keys = new_keys;
		} /* otherwise shrink */
		new_mask = new_nb - 1;
		for (j = 0; j != nb; ++j) {
			if (!__kh_used(used, j)) continue;
			T key = keys[j];
			__kh_set_unused(used, j);
			while (1) { /* kick-out process; sort of like in Cuckoo hashing */
				khint_t i;
				i = get_bucket(kh_base_hash(Hash()(key)), pre_len, new_bits);
				while (__kh_used(new_used, i)) i = (i + khint_t(1)) & new_mask;
				__kh_set_used(new_used, i);
				if (i < nb && __kh_used(used, i)) { /* kick out the existing element */
					{ T tmp = keys[i]; keys[i] = key; key = tmp; }
					__kh_set_unused(used, i); /* mark it as deleted in the old hash table */
				} else { /* write the element and jump out of the loop */
					keys[i] = key;
					break;
				}
			}
		}
		if (nb > new_nb) /* shrink the hash table */
			keys = (T*)std::realloc(keys, new_nb * sizeof(T));
		std::free(used); /* free the working space */
		used = new_used, bits = new_bits;
		return 0;
	}
	khint_t put(const T &key, uint64_t hash, int *absent_, int pre_len) {
		khint_t nb, i, last, mask;
		int absent = -1;
		nb = n_buckets();
		if (count >= (nb>>1) + (nb>>2)) { /* rehashing */
			if (resize(nb + khint_t(1), pre_len) < 0) {
				if (absent_) *absent_ = -1;
				return nb;
			}
			nb = n_buckets();
		} /* TODO: to implement automatically shrinking; resize() already support shrinking */
		mask = nb - 1;
		i = last = get_bucket(hash, pre_len, bits);
		while (__kh_used(used, i) && !Eq()(keys[i], key)) {
			i = (i + 1U) & mask;
			if (i == last) break;
		}
		if (!__kh_used(used, i)) { /* not present at all */
			keys[i] = key;
			__kh_set_used(used, i);
			++count, absent = 1;
		} else absent = 0; /* Don't touch keys[i] if present */
		if (absent_) *absent_ = absent;
		return i;
	}
	int del(khint_t i, int pre_len) {
		khint_t j = i, k, mask;
		if (keys == 0) return 0;
		mask = n_buckets() - khint_t(1);
		while (1) {
			j = (j + khint_t(1)) & mask;
			if (j == i || !__kh_used(used, j)) break; /* j==i only when the table is completely full */
			k = get_bucket(kh_base_hash(Hash()(keys[j])), pre_len, bits);
			if (k <= i || k > j)
				keys[i] = keys[j], i = j;
		}
		__kh_set_unused(used, i);
		--count;
		return 1;
	}
};

template<class T, int pre_len, class Hash, class Eq = std::equal_to<T>, typename khint_t = uint32_t>
class KHashSet {
	KHashSet1<T, Hash, Eq, khint_t> h[1<<pre_len];
	static inline khint_t get_pre(uint64_t hash) { return hash >> (64 - pre_len); }
	static inline khint_t itr2pre(uint64_t itr) { return itr & ((1U<<pre_len) - 1); }
public:
	KHashSet() {};
	~KHashSet() {};
	inline T &key(uint64_t i) { return h[itr2pre(i)].key(i>>pre_len); };
	uint64_t get(const T &key) const {
		uint64_t hash = kh_base_hash(Hash()(key));
		khint_t pre = get_pre(hash);
		khint_t k = h[pre].get(key, hash, pre_len);
		return (uint64_t)k << pre_len | pre;
	}
	uint64_t put(const T &key, int *absent) {
		uint64_t hash = kh_base_hash(Hash()(key));
		khint_t pre = get_pre(hash);
		khint_t k = h[pre].put(key, hash, absent, pre_len);
		return (uint64_t)k << pre_len | pre;
	}
	int del(uint64_t i) { return h[itr2pre(i)].del(i>>pre_len, pre_len); }
	uint64_t size(void) {
		uint64_t s = 0;
		for (int i = 0; i < 1<<pre_len; ++i) s += h[i].size();
		return s;
	}
};

/***********
 * HashMap *
 ***********/

template<class KType, class VType>
struct KHashMapBucket { KType key; VType val; };

template<class T, class Hash, typename khint_t>
struct KHashMapHash { khint_t operator() (const T &a) const { return Hash()(a.key); } };

template<class T, class Eq>
struct KHashMapEq { bool operator() (const T &a, const T &b) const { return Eq()(a.key, b.key); } };

template<class KType, class VType, int pre_len, class Hash, class Eq=std::equal_to<KType>, typename khint_t=uint32_t>
class KHashMap : public KHashSet<KHashMapBucket<KType, VType>, pre_len,
		KHashMapHash<KHashMapBucket<KType, VType>, Hash, khint_t>,
		KHashMapEq<KHashMapBucket<KType, VType>, Eq>, khint_t>
{
	typedef KHashMapBucket<KType, VType> bucket_t;
	typedef KHashSet<bucket_t, pre_len, KHashMapHash<bucket_t, Hash, khint_t>, KHashMapEq<bucket_t, Eq>, khint_t> hashset_t;
public:
	uint64_t get(const KType &key) const {
		bucket_t t = { key, VType() };
		return hashset_t::get(t);
	}
	uint64_t put(const KType &key, int *absent) {
		bucket_t t = { key, VType() };
		return hashset_t::put(t, absent);
	}
	inline KType &key(uint64_t i) { return hashset_t::key(i).key; }
	inline VType &value(uint64_t i) { return hashset_t::key(i).val; }
};

/****************************
 * HashSet with cached hash *
 ****************************/

template<class KType, typename khint_t>
struct KHashSetCachedBucket { KType key; khint_t hash; };

template<class T, typename khint_t>
struct KHashCachedHash { khint_t operator() (const T &a) const { return a.hash; } };

template<class T, class Eq>
struct KHashCachedEq { bool operator() (const T &a, const T &b) const { return a.hash == b.hash && Eq()(a.key, b.key); } };

template<class KType, int pre_len, class Hash, class Eq = std::equal_to<KType>, typename khint_t = uint32_t>
class KHashSetCached : public KHashSet<KHashSetCachedBucket<KType, khint_t>, pre_len,
		KHashCachedHash<KHashSetCachedBucket<KType, khint_t>, khint_t>,
		KHashCachedEq<KHashSetCachedBucket<KType, khint_t>, Eq>, khint_t>
{
	typedef KHashSetCachedBucket<KType, khint_t> bucket_t;
	typedef KHashSet<bucket_t, pre_len, KHashCachedHash<bucket_t, khint_t>, KHashCachedEq<bucket_t, Eq>, khint_t> hashset_t;
public:
	uint64_t get(const KType &key) const {
		bucket_t t = { key, Hash()(key) };
		return hashset_t::get(t);
	}
	uint64_t put(const KType &key, int *absent) {
		bucket_t t = { key, Hash()(key) };
		return hashset_t::put(t, absent);
	}
	inline KType &key(uint64_t i) { return hashset_t::key(i).key; }
};

/****************************
 * HashMap with cached hash *
 ****************************/

template<class KType, class VType, typename khint_t>
struct KHashMapCachedBucket { KType key; VType val; khint_t hash; };

template<class KType, class VType, int pre_len, class Hash, class Eq = std::equal_to<KType>, typename khint_t = uint32_t>
class KHashMapCached : public KHashSet<KHashMapCachedBucket<KType, VType, khint_t>, pre_len,
		KHashCachedHash<KHashMapCachedBucket<KType, VType, khint_t>, khint_t>,
		KHashCachedEq<KHashMapCachedBucket<KType, VType, khint_t>, Eq>, khint_t>
{
	typedef KHashMapCachedBucket<KType, VType, khint_t> bucket_t;
	typedef KHashSet<bucket_t, pre_len, KHashCachedHash<bucket_t, khint_t>, KHashCachedEq<bucket_t, Eq>, khint_t> hashset_t;
public:
	uint64_t get(const KType &key) const {
		bucket_t t = { key, VType(), Hash()(key) };
		return hashset_t::get(t);
	}
	uint64_t put(const KType &key, int *absent) {
		bucket_t t = { key, VType(), Hash()(key) };
		return hashset_t::put(t, absent);
	}
	inline KType &key(uint64_t i) { return hashset_t::key(i).key; }
	inline VType &value(uint64_t i) { return hashset_t::key(i).val; }
};

}

#endif /* __AC_KHASHL_HPP */
