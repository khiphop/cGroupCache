/**
 * System design: LRU cache (least recently used).
 */
#include<iostream>
#include<map>
#include <mutex>
#include <thread>
#include "groupCache.hpp"

using namespace std;

class Dispatch {
private:
    std::map<std::string, GroupCache *> groups;
    std::string selfNode;
    mutex mu;

public:
    ConsistentHash *conHash;

    Dispatch(std::string self, int vnc) {
        conHash = new ConsistentHash(vnc);
        conHash->RegisterNode(self);

        selfNode = self;
    }

    void RegisterGroup(std::string group, int capacity, int ttl, string (*bsFunc)(string, string)) {
        groups[group] = new GroupCache("user", capacity, ttl, bsFunc);
    }

    void RegisterNode(const std::string &node) {
        conHash->RegisterNode(node);
    }

    bool JudgeSelf(std::string key) {
        string node = conHash->ChooseNode(key);
        cout << "Key ["<< key << "] belongs to " << node << endl;

        if (node == selfNode) {
            return true;
        } else {
            return false;
        }
    }

    bool SetHandler(std::string group, std::string key, std::string val) {
        if (JudgeSelf(key)) {
            // TODO set
            groups[group]->set(key, val);
        } else {
            string node = conHash->ChooseNode(key);

            cout << " Trigger Inner Set, Target: " << node << endl;
            cout << " Group: " << group << " Key: " << key << " Val: " << val << endl;

            // TODO inner set
        }

        return true;
    }

    string GetHandler(std::string group, std::string key) {
        string val;

        if (JudgeSelf(key)) {
            // TODO get
            val = groups[group]->get(key);
        } else {
            string node = conHash->ChooseNode(key);

            cout << " Trigger Inner Get, Target: " << node << endl;
            cout << " Group: " << group << " Key: " << key << endl;

            // TODO inner get
            val = "val from other node";
        }

        return val;
    }
};


//int main() {
//    auto *cc = new Dispatch("127.0.0.1:7000", 10);
//    cc->RegisterNode("127.0.0.1:7001");
//
//    cc->RegisterGroup("user", 10000, 0, NULL);
//    cc->RegisterGroup("club", 10000, 0, NULL);
//
//
//    cc->SetHandler("user", "u1001", "name:kevin");
//    string val = cc->GetHandler("user", "u1001");
//    cout << val << endl;
//}

