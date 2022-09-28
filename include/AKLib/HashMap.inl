
NSB(AKLib)

template <typename K, typename T>
AKL_INLINE HashEntry<K, T>::HashEntry(const K& key, const T& value)
	: key{ key }, value{ value }
{
}

template <typename K, typename T>
AKL_INLINE HashEntry<K, T>::~HashEntry()
{
}

AKL_INLINE unsigned int AKLHash(void* data, size_t size) 
{
	char* _data = (char*)data;

	uint32_t hash = 2166136261u;

	for (int i = 0; i < size; i++)
	{
		hash ^= _data[i];
		hash *= 16777619;
	}

	return hash;
}

template <>
AKL_INLINE unsigned int HashHelper::Hash(String& v)
{
	return AKLHash(v.c_str(), v.Size());
}

template <>
AKL_INLINE unsigned int HashHelper::Hash(const char*& v)
{
	return AKLHash(&v, strlen(v));
}

template <typename K, typename T>
AKL_INLINE void HashHelper::Init(K& key, T& val)
{
	key = K{};
	val = T{};
}

template <typename K, typename T>
AKL_INLINE void HashHelper::Destroy(K& key, T& val)
{
	key = K{};
	val = T{};
}

template <typename K>
AKL_INLINE unsigned int HashHelper::Hash(K& v) 
{
	return AKLHash(&v, sizeof(v));
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE HashIterator<K, T, I, D, F>::HashIterator(HashEntry<K, T>* entry, HashMap<K, T, I, D, F>* map)
	: entry{ entry }, map{ map }
{
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE HashIterator<K, T, I, D, F>::~HashIterator()
{
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE void HashIterator<K, T, I, D, F>::operator ++()
{
	HashEntry<K, T>* b = entry;

	HashEntry<K, T>* end = map->end().entry;

	for (HashEntry<K, T>*& e = entry; e != end; e++)
	{
		if (e->hasValue)
		{
			if (e == b) continue;

			//return HashIterator<K, T, I, D, F>(e, map);
			return;
		}
	}

	entry = end;
	//return HashIterator<K, T, I, D, F>(end, map);
	return;
}

template <typename K, typename T, void(I)(K&, T&), void(D)(K&, T&), unsigned int(F)(K&)>
AKL_INLINE void HashIterator<K, T, I, D, F>::operator --()
{
	HashEntry<K, T>* b = entry;

	HashEntry<K, T>* begin = map->begin().entry;

	for (HashEntry<K, T>*& e = entry; e != begin; e--)
	{
		if (e->hasValue)
		{
			if (e == b) continue;

			//return HashIterator<K, T, I, D, F>(e, map);
			return;
		}
	}

	entry = begin;
	//return HashIterator<K, T, I, D, F>(begin, map);
	return;
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE HashEntry<K, T>& HashIterator<K, T, I, D, F>::operator * ()
{
	return *entry;
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE HashEntry<K, T>* HashIterator<K, T, I, D, F>::operator -> ()
{
	return entry;
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE bool HashIterator<K, T, I, D, F>::operator == (HashIterator<K, T, I, D, F> other)
{
	if (other.map != map) return false;
	return other.entry == entry;
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE bool HashIterator<K, T, I, D, F>::operator != (HashIterator<K, T, I, D, F> other)
{
	if (other.map != map) return true;
	return other.entry != entry;
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE HashMap<K, T, I, D, F>::HashMap()
	: count{ 0 }, capacity{ 8 }, entries{ Allocator<HashEntry<K, T>>::Allocate(8, true) }
{
	for (int i = 0; i < capacity; i++) 
	{
		I(entries[i].key, entries[i].value);
		entries[i].hasValue = false;
	}
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE HashMap<K, T, I, D, F>::HashMap(std::initializer_list<HashEntry<K, T>> list)
	: count{ 0 }, capacity{ 8 }, entries{ Allocator<HashEntry<K, T>>::Allocate(8, true) }
{
	for (int i = 0; i < capacity; i++)
	{
		I(entries[i].key, entries[i].value);
		entries[i].hasValue = false;
	}

	for (const HashEntry<K, T>& entry : list) 
	{
		Set(entry.key, entry.value);
	}
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE HashMap<K, T, I, D, F>::~HashMap()
{
	for (int i = 0; i < capacity; i++) 
	{
		HashEntry<K, T>* entry = &entries[i];

		if (entry->hasValue) 
		{
			D(entry->key, entry->value);
			entry->hasValue = false;
		}
	}
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE HashIterator<K, T, I, D, F> HashMap<K, T, I, D, F>::begin()
{
	for (HashEntry<K, T>* e = entries; e != end().entry; e++) 
	{
		if (e->hasValue) return HashIterator<K, T, I, D, F>(e, this);
	}

	return end();
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE HashIterator<K, T, I, D, F> HashMap<K, T, I, D, F>::end()
{
	return HashIterator<K, T, I, D, F>(entries + capacity, this);
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE bool HashMap<K, T, I, D, F>::Set(K key, T value)
{
	if ((count) > ((capacity >> 1) + (capacity >> 2))) // if (count > (capacity * 0.75))
	{
		int cap = (capacity << 1) + 1;
		AdjustCapacity(cap);
	}

	HashEntry<K, T>* entry = FindEntry(entries, capacity, key);

	bool isNewKey = !entry->hasValue;
	if (isNewKey) count++;

	entry->key = key;
	entry->value = value;
	entry->hasValue = true;
	return isNewKey;
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE bool HashMap<K, T, I, D, F>::Get(K key, T* value)
{
	if (count == 0) return false;

	HashEntry<K, T>* entry = FindEntry(entries, capacity, key);
	if (!entry->hasValue) return false;

	*value = entry->value;
	return true;
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE bool HashMap<K, T, I, D, F>::ContainsKey(K key) 
{
	return FindEntry(entries, capacity, key)->hasValue;
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE bool HashMap<K, T, I, D, F>::ContainsValue(T value)
{
	for (Iterator i = begin(); i != end(); ++i) 
	{
		HashEntry<K, T>& e = *i;

		if (e.hasValue && e.value == value)
			return true;
	}

	return false;
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE bool HashMap<K, T, I, D, F>::Delete(K key)
{
	if (count == 0) return false;

	HashEntry<K, T>* entry = FindEntry(entries, capacity, key);
	if (!entry->hasValue) return false;

	D(entry->key, entry->value);
	entry->hasValue == false;

	return true;
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE void HashMap<K, T, I, D, F>::Resize(size_t newSize) 
{
	AdjustCapacity(newSize);
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE void HashMap<K, T, I, D, F>::AddAll(HashMap<K, T, I, D, F>& other)
{
	for (int i = 0; i < other.capacity; i++) 
	{
		HashEntry<K, T>* entry = &other.entries[i];

		if (entry->hasValue) 
		{
			Set(entry->key, entry->value);
		}
	}
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE void HashMap<K, T, I, D, F>::Traverse(std::function<void(T&)> Func)
{
	for (int i = 0; i < capacity; i++) 
	{
		HashEntry<K, T>& entry = entries[i];

		if (entry.hasValue) Func(entry.value);
	}
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE void HashMap<K, T, I, D, F>::Traverse(std::function<void(K&, T&)> Func)
{
	for (int i = 0; i < capacity; i++)
	{
		HashEntry<K, T>& entry = entries[i];

		if (entry.hasValue) Func(entry.key, entry.value);
	}
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE void HashMap<K, T, I, D, F>::Traverse(std::function<void(K*, T*)> Func)
{
	for (int i = 0; i < capacity; i++)
	{
		HashEntry<K, T>& entry = entries[i];

		if (entry.hasValue) Func(&entry.key, &entry.value);
	}
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE size_t HashMap<K, T, I, D, F>::Size() 
{
	return capacity;
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE HashEntry<K, T>* HashMap<K, T, I, D, F>::Data()
{
	return entries;
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE T& HashMap<K, T, I, D, F>::operator [] (K key) 
{
	HashEntry<K, T>* entry = FindEntry(entries, capacity, key);

	if (!entry->hasValue)
	{
		Set(key, T());

		entry = FindEntry(entries, capacity, key);
	}

	return entry->value;
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE HashEntry<K, T>* HashMap<K, T, I, D, F>::FindEntry(HashEntry<K, T>* entries, size_t capacity, K& key)
{
	uint32_t index = F(key) % capacity;
	HashEntry<K, T>* tombstone = NULL;

	for (;;) 
	{
		HashEntry<K, T>* entry = &entries[index];

		if (!entry->hasValue) 
		{
			if (entry->value == T()) 
			{
				// Empty entry
				return tombstone != NULL ? tombstone : entry;
			}
			else 
			{
				// We found a tombstone
				if (tombstone == NULL) tombstone = entry;
			}
		}
		else if (entry->key == key) 
		{
			// We found the key
			return entry;
		}

		index = (index + 1) % capacity;
	}
}

template <typename K, typename T, void(*I)(K&, T&), void(*D)(K&, T&), unsigned int(*F)(K&)>
AKL_INLINE void HashMap<K, T, I, D, F>::AdjustCapacity(size_t cap)
{
	if (cap == 0) 
	{
		for (int i = 0; i < capacity; i++) 
		{
			HashEntry<K, T>* entry = &entries[i];

			if (entry->hasValue) 
			{
				D(entry->key, entry->value);
			}
		}

		entries = Allocator<HashEntry<K, T>>::Deallocate(entries);
		capacity = 0;
		count = 0;
		return;
	}

	HashEntry<K, T>* temp_entries = Allocator<HashEntry<K, T>>::Allocate(cap);

	for (int i = 0; i < cap; i++)
	{
		I(temp_entries[i].key, temp_entries[i].value);
		temp_entries[i].hasValue = false;
	}

	count = 0;
	for (int i = 0; i < capacity; i++) 
	{
		HashEntry<K, T>* entry = &entries[i];
		if (!entry->hasValue) continue;

		HashEntry<K, T>* dest = FindEntry(temp_entries, cap, entry->key);
		dest->key = entry->key;
		dest->value = entry->value;
		dest->hasValue = true;
		count++;
	}

	Allocator<HashEntry<K, T>>::Deallocate(entries);

	capacity = cap;
	entries = temp_entries;
}

NSE()