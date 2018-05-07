//
// Created by vipkid on 2018/5/3.
//

#ifndef LOGLIB_LOGGER_H
#define LOGLIB_LOGGER_H
#include <queue>
#include <iostream>
#include "logbuf.h"
#include <list>
#include <memory>
#include <condition_variable>
#include <mutex>
using namespace std;

//#define LOGGER(o,level,msg)   o.append("%s-%s-%s-%s:%s\n",__FILE__,__LINE__,__FUNCTION__,level,msg.c_str())
class logger :public enable_shared_from_this<logger>{
    list<logbuf*> m_bufList;
    logbuf* m_pCurrent;
    void write2File(logbuf& buf);
    mutex m_mutex;
    condition_variable m_cv;
public:
    enum {
        SUCESS = 0,
        NO_MEMORY = 1,
                NO_INIT = 2,
                OTHER
    };
    logger();
    int Init(char* filename);
    int append(string msg);
    static void loggerThread(weak_ptr<logger> ptr);
    ~logger(){}
};


#endif //LOGLIB_LOGGER_H
