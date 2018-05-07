#include <iostream>
#include <functional>
#include "logger.h"
#include <thread>
#include <chrono>
#define _GLIBCXX_USE_NANOSLEEP
using namespace std;


class TestA{
public:
    void func(int a,int b){
        cout<<"func"<<a << " " << b <<endl;
    }
};
TestA a;
void ttfunc(function<void(int,int)> f){
    f(1,2);
}

function<void(int,int)> f1 = bind(&TestA::func,&a,placeholders::_1,placeholders::_2);

class callbackA{
public:
    void f(int a,int b){
        cout<<"f in" << a <<endl;
    }
};
//template <typename handler>
class inter{
public:
    virtual void run()=0;
};
template<typename handler>
class C:public inter{
    void* m_ptr;
public:
    C(void* p){m_ptr = p;}
    handler* Change(){
        handler* p = static_cast<handler*>(m_ptr);
        return p;
    }
    void run(){
        handler* p = Change();
        (*p)();
    }
};
class B{
    typedef function<void(int,int)> F2;
    inter * m_c;
public:
    template<typename h>
    void addCB(h f){
        void* p = malloc(sizeof(f));
        memcpy(p,&f,sizeof(f));
        m_c = new C<h>(p);
    }
    void run(){
        m_c->run();
    }
};


void testlogger(){
    shared_ptr<logger> pl(new logger);
    pl->Init("test");
    for(int i = 0;i<1000000;i++){
        pl->append("test1");
        std::this_thread::sleep_for(chrono::milliseconds(1));
    }
    std::this_thread::sleep_for(chrono::seconds(100));
    cout<<"finish"<<endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    callbackA ta;
    B tb;
    {
        auto testcb = bind(&callbackA::f,&ta,1,2);
        tb.addCB(testcb);
    }
    tb.run();
    ttfunc(f1);
    //yield();
    testlogger();
    //std::this_thread::yield();
    return 0;
}