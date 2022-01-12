/**
 * System design: LRU cache (least recently used).
 */
#include<iostream>
#include<map>
#include <mutex>
#include <thread>
#include "lru.hpp"

using namespace std;

class MainCache {
private:
    mutex mu;
    LruCache *lruCache;
public:
    MainCache(int capacity, int ttl) {
        lruCache = new LruCache(capacity, ttl);
    }

    bool set(string key, string val) {
        mu.lock();

        lruCache->set(key, val.c_str());

        mu.unlock();

        return true;
    }

    string get(string key) {
        mu.lock();

        string val = lruCache->get(key);

        mu.unlock();

        return val;
    }
};


