/**
 * System design: LRU cache (least recently used).
 */
#include<iostream>
#include<map>
#include <mutex>
#include <thread>
#include "cache.hpp"
#include <iostream>
#include "consistent_hash.hpp"

using namespace std;

class GroupCache {
private:
    string groupName;
    mutex mu;
    MainCache *mainCache;
    int backSourceCd = 60;
    map<string, int> bsCdMap;
    map<string, int>::iterator it;
    string (*backSourceFunc)(string, string);

public:
    GroupCache(
            string name,
            int capacity,
            int ttl,
            string (*bsFunc)(string, string)
    ) {
        mainCache = new MainCache(capacity, ttl);
        groupName = name;
        backSourceFunc = *bsFunc;
    }


    bool setBackSourceCd(int second) {
        backSourceCd = second;

        return true;
    }

    bool set(string key, string val) {
        mainCache->set(key, val);

        return true;
    }

    string get(string key) {
        string val = mainCache->get(key);

        if (val.empty()) {
            mu.lock();
            val = onBackSource(key);
            mu.unlock();

            if (!val.empty()) {
                mainCache->set(key, val);
            }
        }

        return val;
    }

    string onBackSource(string key) {
        // 复检数据是否存在 / recheck val exists or not
        string val = mainCache->get(key);
        if (val != "") {
            return val;
        }

        time_t myt = time(NULL);
        it = bsCdMap.find(key);
        if (it != bsCdMap.end() && bsCdMap[key] > myt) {
            // back source cd
            cout << key << ":back source cd" << endl;
            return "";
        } else {
            bsCdMap[key] = myt + backSourceCd;

            if (backSourceFunc != NULL) {
                return backSourceFunc(groupName, key);
            } else {
                cout << "=== back source function not defined! ==="<< endl;

                return backSourceMock(groupName, key);
                // return "";
            }
        }
    }

    string backSourceMock(string group, string key) {
        cout << "Trigger backSourceMock " << group << ":" << key << endl;

        map<string, string> data = dataMock(group);

        const map<string, string>::iterator &iter = data.find(key);
        if (iter != data.end())
            return iter->second;
        else
            return "";
    }

    static map<string, string> dataMock(string group) {
        map<string, string> dataMock;

        if (group == "user") {
            dataMock.insert(pair<string, string>("kevin", "age:30,school:edu"));
            dataMock.insert(pair<string, string>("allen", "age:29,school:bju"));
            dataMock.insert(pair<string, string>("candy", "age:19,school:cqu"));
        }

        if (group == "club") {
            dataMock.insert(pair<string, string>("600100", "owner:001,name:happy"));
            dataMock.insert(pair<string, string>("202300", "owner:202,name:mj"));
            dataMock.insert(pair<string, string>("336551", "owner:336,name:let's play"));
        }

        return dataMock;
    }

};



