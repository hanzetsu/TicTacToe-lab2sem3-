#pragma once
#include "IDictionary.hpp"
#include "MutableArraySequence.hpp"
#include "LinkedList.hpp"
#include <stdexcept>
#include <cstdint>

template <class TKey, class TValue>
struct Pair
{
    TKey key;
    TValue value;
    Pair() : key(), value() {}
    Pair(const TKey &k, const TValue &v) : key(k), value(v) {}
};

template <class TKey, class TValue>
class HashTableDictionary : public IDictionary<TKey, TValue>
{
private:
    MutableArraySequence<LinkedList<Pair<TKey, TValue>>> buckets;
    int count;
    int capacity;

    int hash(const TKey &key) const
    {
        uint64_t h = (uint64_t)key * 11400714819323198485ULL;
        return (int)(h % capacity);
    }

public:
    HashTableDictionary(int cap = 1024)
        : buckets(cap), count(0), capacity(cap) {}

    int GetCount() const override { return count; }
    int GetCapacity() const override { return capacity; }

    bool ContainsKey(const TKey &key) const override
    {
        int idx = hash(key);
        LinkedList<Pair<TKey, TValue>> bucket = buckets.Get(idx);
        int len = (int)bucket.GetLength();
        for (int i = 0; i < len; ++i)
        {
            if (bucket.Get(i).key == key)
                return true;
        }
        return false;
    }

    TValue Get(const TKey &key) const override
    {
        int idx = hash(key);
        LinkedList<Pair<TKey, TValue>> bucket = buckets.Get(idx);
        int len = (int)bucket.GetLength();
        for (int i = 0; i < len; ++i)
        {
            if (bucket.Get(i).key == key)
                return bucket.Get(i).value;
        }
        throw std::runtime_error("HashTableDictionary::Get: key not found");
    }

    void Add(const TKey &key, const TValue &value) override
    {
        int idx = hash(key);
        LinkedList<Pair<TKey, TValue>> bucket = buckets.Get(idx);

        int len = (int)bucket.GetLength();
        for (int i = 0; i < len; ++i)
        {
            if (bucket.Get(i).key == key)
            {
                Pair<TKey, TValue> p = bucket.Get(i);
                p.value = value;
                LinkedList<Pair<TKey, TValue>> newBucket;
                for (int j = 0; j < len; ++j)
                {
                    if (j == i)
                        newBucket.Append(p);
                    else
                        newBucket.Append(bucket.Get(j));
                }
                buckets.Set(idx, newBucket);
                return;
            }
        }

        bucket.Append(Pair<TKey, TValue>(key, value));
        buckets.Set(idx, bucket);
        ++count;
    }

    void Remove(const TKey &key) override
    {
        int idx = hash(key);
        LinkedList<Pair<TKey, TValue>> bucket = buckets.Get(idx);
        int len = (int)bucket.GetLength();

        LinkedList<Pair<TKey, TValue>> newBucket;
        bool found = false;
        for (int i = 0; i < len; ++i)
        {
            Pair<TKey, TValue> p = bucket.Get(i);
            if (p.key == key)
            {
                found = true;
                continue;
            }
            newBucket.Append(p);
        }

        if (!found)
            throw std::runtime_error("HashTableDictionary::Remove: key not found");

        buckets.Set(idx, newBucket);
        --count;
    }
};