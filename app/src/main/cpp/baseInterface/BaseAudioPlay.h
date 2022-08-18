//
// Created by Darren on 2020/2/16.
//

/**
 *
 * DarrenAdd20200216:音频播放模块基类。
 *
 * 该类继承观察者类，作为重采样对象的观察者，接收重采样后的数据，将数据压入缓冲队列。
 *
 *
 */

#ifndef DARPLAYER_BASEAUDIOPLAY_H
#define DARPLAYER_BASEAUDIOPLAY_H


#include "../beans/BaseData.h"
#include "../beans/DParameter.h"
#include "../DObserver.h"
#include <list>

class BaseAudioPlay : public DObserver{
public:

    //从观察者对象中拿到数据，压入缓冲队列中，如果队列满，则阻塞等待。
    virtual void Update(BaseData data);

    //获取缓冲数据，如没有则阻塞
    virtual BaseData GetData();

    virtual bool StartPlay(DParameter out) = 0;

    virtual void Close() = 0;

    virtual void Clear();

    //设置默认最大缓冲，200帧
    int maxFrame = 150;

    //保存下pts的信息，同步时需要使用。
    int pts = 0;

protected:
    //缓冲列表。
    std::list <BaseData> frames;
    //线程锁。
    std::mutex framesMutex;

private:



};


#endif //DARPLAYER_BASEAUDIOPLAY_H
