/**
 * LRU algo
 */
#include<iostream>
#include<map>

using namespace std;

struct CacheNode {
    string key;
    string value;
    CacheNode *pre, *next;

    CacheNode(string k, string v) : key(std::move(k)), value(std::move(v)), pre(nullptr), next(nullptr) {}
};

class LruCache {
private:
    int capacity;
    int ttl;
    CacheNode *head, *tail;
    map<string, CacheNode *> mp;          // Use hashmap to store
public:
    LruCache(int c, int t) {
        capacity = c;
        ttl = t;
        head = nullptr;
        tail = nullptr;
    }

    string get(const string& key) {
        map<string, CacheNode *>::iterator it = mp.find(key);
        if (it != mp.end()) {
            CacheNode *node = it->second;
            remove(node);
            setHead(node);

            return node->value;
        } else {
            return "";
        }
    }

    void set(const string& key, const char* value) {
        map<string, CacheNode *>::iterator it = mp.find(key);

        if (it != mp.end()) {
            // key exists
            CacheNode *node = it->second;
            node->value = value;

            // move to head
            move2Head(node);
        } else {
            // new key
            CacheNode *newNode = new CacheNode(key, value);

            setHead(newNode);

            mp[key] = newNode;

            limitCheck();
        }
    }


    void limitCheck() {
        checkKeyCount();
        checkByte();
    }

    void checkKeyCount() {
//        cout << "checkKeyCount" << endl;
//        cout << mp.size() << endl;

        if (mp.size() > capacity) {
            cout << "Trigger Lru Remove" << endl;

            map<string, CacheNode *>::iterator iter = mp.find(tail->key);
            remove(tail);
            mp.erase(iter);
        }
    }

    void checkByte() {
        // TODO
    }

    void move2Head(CacheNode *node) {
        remove(node);
        setHead(node);
    }

    void remove(CacheNode *node) {
        if (node->pre != nullptr) {
            node->pre->next = node->next;
        } else {
            head = node->next;
        }
        if (node->next != nullptr) {
            node->next->pre = node->pre;
        } else {
            tail = node->pre;
        }
    }

    void setHead(CacheNode *node) {
        node->next = head;
        node->pre = nullptr;

        if (head != nullptr) {
            head->pre = node;
        }
        head = node;
        if (tail == nullptr) {
            tail = head;
        }
    }
};


