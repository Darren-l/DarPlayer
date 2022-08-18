//
// Created by Darren on 2020/2/12.
//

/**
 *
 * DarrenAdd20200212:观察者基类。
 *
 * 观察者和主题都需要继承该类，观察者通过Update获取数据，主题需要通过add添加观察者，在notify中
 * 回调给观察者。
 *
 * 项目中用于将解封装后的数据，通过观察者发送给解码器。
 *
 *
 */

#ifndef DARPLAYER_DOBSERVER_H
#define DARPLAYER_DOBSERVER_H


#include "beans/BaseData.h"
#include "tools/Thread.h"
#include <vector>
#include <mutex>

class DObserver: public Thread {
public:
    virtual void Update(BaseData data){};

    //添加观察者，线程安全。
    void AddObs(DObserver * dObserver);
    //通知所有观察者，线程安全。
    void Notify(BaseData data);

protected:
    std::vector<DObserver *>doverctor;
    std::mutex mux;

};


#endif //DARPLAYER_DOBSERVER_H
