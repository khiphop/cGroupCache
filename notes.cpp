#include<iostream>
#include <vector>
#include <sstream>
#include <stdarg.h>
using namespace std;

int forTest()
{
    std::vector<int> arr;
    arr.push_back(1);
    arr.push_back(2);

    for (auto n : arr)
    {
        std::cout << n << std::endl;
    }

    return 0;
}

int stringStream(){
    stringstream ss;
    ss << "hello ";
    ss << "world!";

    std::cout << ss.str() << std::endl;
    // 对stringstream而言，operator<< 是一直往字符串流中写字符
    // 而不是覆盖之前输入的字符，这一点不同于str()成员函数方法,str()函数创建的是一个临时的string对象
    //这个string对象在函数str()语句结束就会被销毁，因而一般使用时应先声明一个string对象s，将str()赋值给s
    //const string s=ss.str();//这样会有一个string拷贝的过程
    /*又或者是const string& s=ss.str();这样就是用s直接引用了ss.str()这个临时变量，就“延长了临时变量ss.str()的
    生命周期”，使得ss.str()生命结束时刻和s一样*/
    return 0;

}


void stdargFunc(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    auto a = va_arg(ap, char*);
    auto b = va_arg(ap, char*);

    cout << fmt << ", " << a << ", " << b << endl;

    va_end(ap);
}

int stdarg(){
    stdargFunc("123");
    system("pause");
    return 0;
}

int main()
{
//    forTest();
//    stringStream();
    stdarg();
}

