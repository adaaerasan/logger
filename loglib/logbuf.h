//
// Created by vipkid on 2018/5/3.
//

#ifndef LOGLIB_LOGBUF_H
#define LOGLIB_LOGBUF_H
#include <iostream>
#include <queue>
using namespace std;

class logbuf {
    queue<string> m_queue;
    bool m_full;
public:
    logbuf();
    ~logbuf(){}
    void append(string s);
    int len();
    void move(logbuf& other);
    string pop();
    void setFull(bool full){m_full = full;}
    bool getFull()const{return m_full;}
};


#endif //LOGLIB_LOGBUF_H
