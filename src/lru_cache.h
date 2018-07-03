#ifndef _LRU_CACHE_H__
#define _LRU_CACHE_H__


#include <cstdint>
#include <list>
#include <unordered_map>
#include <functional>

//#include <iostream>


template<class TKey, class TValue>
class LRUCache {
public:
    LRUCache(uint32_t capacity, const TValue& empty_value);
    ~LRUCache();

    TValue get(const TKey& key);
    void set(const TKey& key, const TValue& value);

protected:
    uint32_t capacity; // Max # of items to store.
    TValue empty_value; // Value to return when item not in cache

    // Order in which keys have been accessed. Most recent first.
    std::list<TKey> access_order;

    // Map to store (key, value) pairs.
    std::unordered_map<TKey, TValue> cache;

    // Map for quick lookup of access order.
    std::unordered_map<TKey, typename std::list<TKey>::iterator> access_order_it;

    void bring_to_front(const TKey& key);
    void remove_lru();
};


template<class TKey, class TValue>
class CachedFunction : public LRUCache<TKey,TValue> {
public:
    CachedFunction(std::function<TValue(const TKey&)> f, uint32_t capacity);
    ~CachedFunction();
    
    TValue eval(const TKey& arg);

    TValue operator()(const TKey& arg);
    
private:
    std::function<TValue(const TKey&)> f;
};


template<class TKey, class TValue>
CachedFunction<TKey, TValue>::CachedFunction(std::function<TValue(const TKey&)> f, uint32_t capacity)
    : LRUCache<TKey, TValue>(capacity, TValue()), f(f)
{}


template<class TKey, class TValue>
CachedFunction<TKey, TValue>::~CachedFunction() {}


template<class TKey, class TValue>
TValue CachedFunction<TKey, TValue>::eval(const TKey& arg) {
    // Look up key in cache
    auto cache_it = this->cache.find(arg);
    if(cache_it == this->cache.end()) { // Key not found
        //std::cout << "Evaluating f(arg)" << std::endl;

        TValue value = f(arg);
        this->set(arg, value);
        return value;
    } else { // Key found
        //std::cout << "Using cached result of f(arg)" << std::endl;

        // Update access order
        this->bring_to_front(arg);
        // Return cached value
        return cache_it->second;
    }
}


template<class TKey, class TValue>
TValue CachedFunction<TKey, TValue>::operator()(const TKey& arg) {
    return eval(arg);
}


template<class TKey, class TValue>
LRUCache<TKey, TValue>::LRUCache(uint32_t capacity, const TValue& empty_value)
    : capacity(capacity), empty_value(empty_value)
{}


template<class TKey, class TValue>
LRUCache<TKey, TValue>::~LRUCache() {}


template<class TKey, class TValue>
TValue LRUCache<TKey, TValue>::get(const TKey& key) {
    // Look up key in cache
    auto cache_it = cache.find(key);
    if(cache_it == cache.end()) { // Key not found
        return empty_value;
    } else { // Key found
        // Update access order
        bring_to_front(key);
        // Return cached value
        return cache_it->second;
    }
}


template<class TKey, class TValue>
void LRUCache<TKey, TValue>::set(const TKey& key, const TValue& value) {
    // Look up key in cache
    auto cache_it = cache.find(key);
    if(cache_it == cache.end()) { // Key not in cache
        // Add (key, value) pair to cache
        cache_it = cache.insert({key, value}).first;
        // Insert key into access-order list
        access_order.push_front(key);
        // Update access-order lookup
        access_order_it.insert({key, access_order.begin()});
        // If over capacity, remove least-recently-used key
        if(cache.size() > capacity) {
            remove_lru();
        }
    } else { // Key found
        // Bring key to front of access order list
        bring_to_front(key);
        // Update value
        cache_it->second = value;
    }
}


template<class TKey, class TValue>
void LRUCache<TKey, TValue>::remove_lru() {
    // Look up key of least-recently-used element
    const TKey& key = access_order.back();
    // Erase key from cache and access-order-iterator lookup
    cache.erase(key);
    access_order_it.erase(key);
    // Erase key from access-order list
    access_order.pop_back();
}


template<class TKey, class TValue>
void LRUCache<TKey, TValue>::bring_to_front(const TKey& key) {
    auto ao_it = access_order_it.find(key); // Assumed to be found
    if(ao_it->second != access_order.begin()) {
        // Move key to front of access-order list
        access_order.erase(ao_it->second);
        access_order.push_front(key);
        // Update location of key in access-order-iterator lookup
        ao_it->second = access_order.begin();
    }
}


#endif // _LRU_CACHE_H__
