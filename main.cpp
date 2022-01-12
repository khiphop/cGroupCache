#include<iostream>
#include <string>
#include<map>
#include <thread>
#include "dispatch.hpp"

class Demo {

    public:



    /**
     * lruDemo
     */
    void lruDemo() {
        /*
         * cn: group容量是2, 依次设置 key:candy, kevin, allen, candy会被移除, 查询candy时没有数据触发回源
         * en: New a group with capacity 2, set key:candy, kevin, allen. 'candy' will be removed.
         * */
        auto *cc = new Dispatch("127.0.0.1:7000", 10);
        cc->RegisterGroup("user", 2, 0, NULL);
        cc->SetHandler("user", "candy", "age:19");
        cc->SetHandler("user", "kevin", "age:30");
        cc->SetHandler("user", "allen", "age:24");

        string val = cc->GetHandler("user", "candy");
        cout << val<< endl;

        val = cc->GetHandler("user", "candy");
        cout << val<< endl;
        cout << "==========================================\n"<< endl;

        /*
         * cn: group容量是2, 依次设置 key:candy, kevin. 查询 candy, candy会移到头部, 再设置allen, 此时 'kevin' 被remove
         * en: New a group with capacity 2, set key:candy, kevin. query 'candy', 'candy' move to head.
         *      after that set 'allen'. finally, 'kevin' will be removed.
         * */
        auto *dd = new Dispatch("127.0.0.1:7000", 10);
        dd->RegisterGroup("user", 2, 0, NULL);
        dd->SetHandler("user", "candy", "age:19");
        dd->SetHandler("user", "kevin", "age:30");
        dd->GetHandler("user", "candy");
        dd->SetHandler("user", "allen", "age:24");

        val = dd->GetHandler("user", "candy");
        cout << val<< endl;

        val = dd->GetHandler("user", "kevin");
        cout << val<< endl;
    }


    void backSourceDemo(){
        auto *cc = new Dispatch("127.0.0.1:7000", 10);
        cc->RegisterGroup("user", 10000, 0, NULL);
        string val = cc->GetHandler("user", "candy");
        cout << val << endl;
    }


    /**
     * cn: 示范虚拟节点数对于资源倾斜的规避作用
     * 虚拟节点越多, 节点分布越平均
     * en: Demo how virtual node count effects node distribution
     */
    void nodeDistributionDemo() {
        nodeDistribution(1);
        nodeDistribution(100);
    }

    void nodeDistribution(int vnc){
        cout <<  " vnc: " << vnc << endl;

        std::map<std::string, int> resMap;

        auto *cc = new Dispatch("127.0.0.1:7000", vnc);
        cc->RegisterNode("127.0.0.1:7001");
        cc->RegisterNode("127.0.0.1:7002");

        resMap["127.0.0.1:7000"] = 0;
        resMap["127.0.0.1:7001"] = 0;
        resMap["127.0.0.1:7002"] = 0;

        int totalTimes = 110;
        for (int idx = 0; idx < totalTimes; ++idx) {
            string node = cc->conHash->ChooseNode(to_string(idx) + "#node");

            resMap[node]++;
        }

        map<std::string, int>::iterator iter;
        iter = resMap.begin();
        while (iter != resMap.end()) {
            float rate = iter->second * 100 / totalTimes;
            cout << iter->first << " : " << rate << "%" << endl;
            iter++;
        }
    }
};


int main() {
    auto demo = new Demo();

    demo->lruDemo();
//    demo->backSourceDemo();
//    demo->nodeDistributionDemo();



}

