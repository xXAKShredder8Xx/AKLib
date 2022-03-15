#ifndef AK_LIB_HASH_MAP_H
#define AK_LIB_HASH_MAP_H

#include "AKLib/common.h"
#include "AKLib/String.h"
#include <functional>

NSB(AKLib)

template <typename K, typename T>
struct AKLIB_API HashEntry
{
	inline HashEntry(const K& key, const T& value);
	inline ~HashEntry();

	K key;
	T value;
	bool hasValue;
};

struct AKLIB_API HashHelper
{
	template <typename K, typename T>
	inline static void Init(K& key, T& val);

	template <typename K, typename T>
	inline static void Destroy(K& key, T& val);

	template <typename K>
	inline static unsigned int Hash(K& t);
};

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
class HashMap;

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
struct AKLIB_API HashIterator
{
	inline HashIterator(HashEntry<K, T>* entry, HashMap<K, T, I, D, F>* map);
	inline ~HashIterator();

	inline void operator ++ ();
	inline void operator -- ();
	inline HashEntry<K, T>& operator * ();
	inline HashEntry<K, T>* operator -> ();
	inline bool operator == (HashIterator<K, T, I, D, F> other);
	inline bool operator != (HashIterator<K, T, I, D, F> other);

	HashEntry<K, T>* entry;
	HashMap<K, T, I, D, F>* map;
};

template <typename K, typename T, 
	void(*I)(K&, T&) = HashHelper::Init<K, T>,
	void(*D)(K&, T&) = HashHelper::Destroy<K, T>,
	unsigned int(*F)(K&) = HashHelper::Hash<K>>
class AKLIB_API HashMap
{
public:
	using Iterator = HashIterator<K, T, I, D, F>;

	inline HashMap();
	inline HashMap(std::initializer_list<HashEntry<K, T>> list);
	inline ~HashMap();

	inline HashIterator<K, T, I, D, F> begin();
	inline HashIterator<K, T, I, D, F> end();

	inline bool Set(K key, T value);
	inline bool Get(K key, T* value);
	inline bool ContainsKey(K key);
	inline bool ContainsValue(T value);
	inline bool Delete(K key);
	inline void Resize(size_t newSize);
	inline void AddAll(HashMap<K, T, I, D, F>& other);
	inline void Traverse(std::function<void(T&)> Func);
	inline void Traverse(std::function<void(K&, T&)> Func);
	inline void Traverse(std::function<void(K*, T*)> Func);
	inline size_t Size();
	inline HashEntry<K, T>* Data();

	inline T& operator [](K key);
private:
	inline static HashEntry<K, T>* FindEntry(HashEntry<K, T>* entries, size_t capacity, K& key);
	inline void AdjustCapacity(size_t cap);
	size_t count;
	size_t capacity;
	HashEntry<K, T>* entries;
};

NSE()

#define AKL_INLINE inline
#include "AKLib/HashMap.inl"

#endif // AK_LIB_HASH_MAP_H