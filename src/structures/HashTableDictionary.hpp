#pragma once
#include "IDictionary.hpp"
#include "MutableArraySequence.hpp"
#include "LinkedList.hpp"
#include "Exceptions.hpp"
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
    MutableArraySequence<LinkedList<Pair<TKey, TValue>>*> buckets;
    int count;
    int capacity;

    int hash(const TKey &key) const
    {
        uint64_t h = (uint64_t)key * 11400714819323198485ULL;
        return (int)(h % capacity);
    }

public:
    HashTableDictionary(int cap = 1024) : buckets(cap), count(0), capacity(cap)
    {
        for (int i = 0; i < cap; ++i) {
            buckets.Set(i, new LinkedList<Pair<TKey, TValue>>());
        }
    }

    ~HashTableDictionary() override
    {
        for (int i = 0; i < buckets.GetLength(); ++i) {
            delete buckets.Get(i);
        }
    }

    int GetCount() const override { return count; }
    int GetCapacity() const override { return capacity; }

    bool ContainsKey(const TKey &key) const override
    {
        int idx = hash(key);
        LinkedList<Pair<TKey, TValue>>* bucket = buckets.Get(idx);
        int len = (int)bucket->GetLength();
        for (int i = 0; i < len; ++i)
        {
            if (bucket->Get(i).key == key)
                return true;
        }
        return false;
    }

    TValue Get(const TKey &key) const override
    {
        int idx = hash(key);
        LinkedList<Pair<TKey, TValue>>* bucket = buckets.Get(idx);
        int len = (int)bucket->GetLength();
        for (int i = 0; i < len; ++i)
        {
            if (bucket->Get(i).key == key)
                return bucket->Get(i).value;
        }
        throw InvalidArgument("HashTableDictionary::Get: key not found");
    }

    void Add(const TKey &key, const TValue &value) override
    {
        int idx = hash(key);
        LinkedList<Pair<TKey, TValue>>* bucket = buckets.Get(idx);
        int len = (int)bucket->GetLength();

        for (int i = 0; i < len; ++i)
        {
            if (bucket->Get(i).key == key)
            {
                LinkedList<Pair<TKey, TValue>>* newBucket = new LinkedList<Pair<TKey, TValue>>();
                for (int j = 0; j < len; ++j)
                {
                    if (j == i)
                        newBucket->Append(Pair<TKey, TValue>(key, value));
                    else
                        newBucket->Append(bucket->Get(j));
                }
                delete bucket;
                buckets.Set(idx, newBucket);
                return;
            }
        }

        bucket->Append(Pair<TKey, TValue>(key, value));
        ++count;
    }

    void Remove(const TKey &key) override
    {
        int idx = hash(key);
        LinkedList<Pair<TKey, TValue>>* bucket = buckets.Get(idx);
        int len = (int)bucket->GetLength();

        LinkedList<Pair<TKey, TValue>>* newBucket = new LinkedList<Pair<TKey, TValue>>();
        bool found = false;
        for (int i = 0; i < len; ++i)
        {
            Pair<TKey, TValue> p = bucket->Get(i);
            if (p.key == key)
            {
                found = true;
                continue;
            }
            newBucket->Append(p);
        }

        if (!found)
        {
            delete newBucket;
            throw InvalidArgument("HashTableDictionary::Remove: key not found");
        }

        delete bucket;
        buckets.Set(idx, newBucket);
        --count;
    }
};