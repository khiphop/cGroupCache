#include "consistent_hash.hpp"
#include <iostream>
#include <map>
#include <string>
#include <sstream>

/**
 * Fowler-Noll-Vo Hash algorithm
 * https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
 */
uint32_t ConsistentHash::HashHandler(std::string key) {
    const int p = 16777619;
    uint32_t hash = 2166136261;
    for (int idx = 0; idx < key.size(); ++idx) {
        hash = (hash ^ key[idx]) * p;
    }
    hash += hash << 13;
    hash ^= hash >> 7;
    hash += hash << 3;
    hash ^= hash >> 17;
    hash += hash << 5;
    if (hash < 0) {
        hash = -hash;
    }
    return hash;
}


std::basic_stringstream<char> pkgVirtualNodeName(std::string node, int no) {
    std::stringstream nodeKey;
    nodeKey << node << "-vn-" << no;

    return nodeKey;
}

/**
 * Register Node
 * cn: 注册一个物理节点时，同时注册其关联的所有虚拟节点
 * en: Register a Physical Node And its virtual nodes
 */
void ConsistentHash::RegisterNode(const std::string &node) {
    ComposeElementToRing(node, node);

    for (int j = 0; j < virtualNodeCount; ++j) {
        std::stringstream nodeKey = pkgVirtualNodeName(node, j);

        ComposeElementToRing(node, nodeKey.str());
    }
}

/**
 * Compose Element To the HashRing
 */
void ConsistentHash::ComposeElementToRing(const std::string &node, const std::string &key) {
    uint32_t hashVal = HashHandler(key);
    hashRing.insert({hashVal, node});
}


void ConsistentHash::RemoveNode(const std::string &node) {
    DelElementFromRing(node);

    for (int j = 0; j < virtualNodeCount; ++j) {
        std::stringstream nodeKey = pkgVirtualNodeName(node, j);

        DelElementFromRing(nodeKey.str());
    }
}

/**
 * Delete Element From the HashRing
 */
void ConsistentHash::DelElementFromRing(const std::string &key) {
    uint32_t hashVal = HashHandler(key);

    auto it = hashRing.find(hashVal);
    if (it != hashRing.end()) {
        hashRing.erase(it);
    }
}

/**
 * Choose a node for key
 * cn: 从小到大找到一个大于等于 参数key的哈希值 的虚拟节点. 找不到则返回第一个
 * en: Ascending to find an element which ≥ hashVal. return the first one if you can't find
 */
std::string ConsistentHash::ChooseNode(const std::string &key) {
    uint32_t hashVal = HashHandler(key);
    auto it = hashRing.lower_bound(hashVal);

    if (it == hashRing.end()) {
        if (hashRing.empty()) {
            std::cout << "no available nodes" << '\n';
        }

        return hashRing.begin()->second;
    }

    return it->second;
}


