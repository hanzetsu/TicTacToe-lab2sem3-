#pragma once

template<class TKey, class TValue>
class IDictionary {
public:
    virtual ~IDictionary() = default;

    virtual int    GetCount()    const = 0;
    virtual int    GetCapacity() const = 0;
    virtual TValue Get(const TKey& key) const = 0;
    virtual bool   ContainsKey(const TKey& key) const = 0;
    virtual void   Add(const TKey& key, const TValue& value) = 0;
    virtual void   Remove(const TKey& key) = 0;
};