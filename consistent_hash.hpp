#ifndef consistent_hash_hpp
#define consistent_hash_hpp

#include <stdio.h>
#include <set>
#include <string>
#include <map>

class ConsistentHash {
private:
    std::map<uint32_t, std::string> hashRing;       // {key: hashVal, value: real ip}
    int virtualNodeCount;                           // count of virtual nodes for physical node
public:
    ConsistentHash(int vnc) : virtualNodeCount(vnc) {};

    ~ConsistentHash() {
        hashRing.clear();
    };

    static uint32_t HashHandler(std::string key);

    void RegisterNode(const std::string &node);

    void ComposeElementToRing(const std::string &node, const std::string &key);

    void DelElementFromRing(const std::string &key);

    void RemoveNode(const std::string &node);

    std::string ChooseNode(const std::string &key);
};

#endif /* consistent_hash_hpp */