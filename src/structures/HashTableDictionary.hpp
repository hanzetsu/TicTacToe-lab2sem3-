#pragma once
#include "IDictionary.hpp"
#include "MutableArraySequence.hpp"
#include "LinkedList.hpp"
#include <stdexcept>
#include <cstdint>

template<class TKey, class TValue>
struct Pair {
    TKey key;
    TValue value;
    Pair() : key(), value() {}
    Pair(const TKey& k, const TValue& v) : key(k), value(v) {}
};

template<class TKey, class TValue>
class HashTableDictionary : public IDictionary<TKey, TValue> {
private:
    MutableArraySequence<LinkedList<Pair<TKey, TValue>>> buckets;
    int count;
    int capacity;

    int hash(const TKey& key) const {
        uint32_t h = (uint32_t)key * 2654435761u;
        return (int)(h % capacity);
    }

public:
    HashTableDictionary(int cap = 25) : count(0), capacity(cap) {
        for (int i = 0; i < cap; ++i) {
            buckets.Append(LinkedList<Pair<TKey, TValue>>());
        }
    }

    int GetCount() const override { return count; }
    int GetCapacity() const override { return capacity; }

    bool ContainsKey(const TKey& key) const override {
        int idx = hash(key);
        LinkedList<Pair<TKey, TValue>> bucket = buckets.Get(idx);
        for (int i = 0; i < (int)bucket.GetLength(); ++i) {
            if (bucket.Get(i).key == key) return true;
        }
        return false;
    }

    TValue Get(const TKey& key) const override {
        int idx = hash(key);
        LinkedList<Pair<TKey, TValue>> bucket = buckets.Get(idx);
        for (int i = 0; i < (int)bucket.GetLength(); ++i) {
            if (bucket.Get(i).key == key) return bucket.Get(i).value;
        }
        throw std::runtime_error("HashTableDictionary::Get: key not found");
    }

    void Add(const TKey& key, const TValue& value) override {
        int idx = hash(key);
        LinkedList<Pair<TKey, TValue>> bucket = buckets.Get(idx);

        // ключ уже есть — обновляем значение через итератор
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                it->value = value;
                buckets.Set(idx, bucket);
                return;
            }
        }

        bucket.Append(Pair<TKey, TValue>(key, value));
        buckets.Set(idx, bucket);
        ++count;
    }

    void Remove(const TKey& key) override {
        int idx = hash(key);
        LinkedList<Pair<TKey, TValue>> bucket = buckets.Get(idx);

        LinkedList<Pair<TKey, TValue>> newBucket;
        bool found = false;
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) { found = true; continue; }
            newBucket.Append(*it);
        }

        if (!found) throw std::runtime_error("HashTableDictionary::Remove: key not found");

        buckets.Set(idx, newBucket);
        --count;
    }
};