//
// Created by vipkid on 2018/5/3.
//

#include "logger.h"
#include <thread>
using namespace std;
logger::logger() {
}
int logger::Init(char *filename) {
    const int BUF_LEN = 5;
    m_pCurrent = 0;
    int code = 0;
    for(int i = 0;i<BUF_LEN;i++){
        logbuf* p = new logbuf();
        if (p == 0){
            code = NO_MEMORY;
            break;
        }
        m_bufList.push_back(p);
        if (m_pCurrent == 0){
            m_pCurrent = p;
        }
    }
    if(code != 0){
        for(auto it = m_bufList.begin();it != m_bufList.end();it++){
            if (*it){
                delete *it;
            }
        }
        m_bufList.clear();
        m_pCurrent = 0;
    }
    thread t(bind(logger::loggerThread,weak_ptr<logger>(shared_from_this())));
    t.detach();
}
void logger::loggerThread(weak_ptr<logger> ptr) {
    while(true){
        shared_ptr<logger> p = ptr.lock();
        if (!p){
            return;
        }

        logbuf b;
        {
            unique_lock<mutex> lck(p->m_mutex);
            if (p->m_cv.wait_for(lck,chrono::seconds(3)) == cv_status::timeout){
                b.move(*p->m_pCurrent);
                p->m_pCurrent->setFull(false);
            }else{
                list<logbuf*>& buf = p->m_bufList;
                for(auto it = buf.begin();it != buf.end();it++){
                    if ((*it)->getFull()){
                        b.move(*(*it));
                        (*it)->setFull(false);
                        break;
                    }
                }
            }
        }
        p->write2File(b);

    }
}
void logger::write2File(logbuf &buf) {
    cout<<"buflen:" << buf.len()<<endl;
    while(true){
        string s = buf.pop();
        if (s != ""){
            //cout<< "log: "<<s<<endl;
            this_thread::sleep_for(chrono::microseconds(10));
        }else{
            return;
        }
    }
}
int logger::append(string msg) {
    const int SIGLE_BUF_LEN = 5000;
    if (!m_pCurrent){
        return NO_INIT;
    }
    unique_lock<mutex> lck(m_mutex);
    m_pCurrent->append(msg);
    if (m_pCurrent->len() > SIGLE_BUF_LEN){
        m_cv.notify_one();
        m_pCurrent->setFull(true);
        logbuf* ptmp = 0;
        for(auto it = m_bufList.begin();it != m_bufList.end();it++){
            if (!(*it)->getFull()){
                ptmp = *it;
            }
        }
        if(ptmp == 0){
            logbuf* p = new logbuf();
            if (p == 0){
                m_pCurrent->pop();
                m_pCurrent->append(msg);
            }else{
                m_bufList.push_back(p);
                m_pCurrent = p;
            }
        }
    }
    return SUCESS;
}
