//
// Created by vipkid on 2018/5/3.
//

#include "logbuf.h"
logbuf::logbuf() :m_full(false){

}
void logbuf::append(string s) {
    if (s.length() == 0){
        return;
    }
    m_queue.push(s);
}
int logbuf::len() {
    return m_queue.size();
}
void logbuf::move(logbuf &other) {
    swap(m_queue,other.m_queue);
}
string logbuf::pop() {
    if (m_queue.empty()){
        return "";
    }
    string f = m_queue.front();
    m_queue.pop();
    return f;
}