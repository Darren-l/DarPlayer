//
// Created by Darren on 2020/2/12.
//

/**
 *
 * DarrenAdd20200212:解码器基类。
 *
 * DarrenNote：
 *
 *  1.编解码器是观察者对象，需要实现Update方法，通过该接口获取从解封装器中丢过来的数据。
 *
 *  2.这里有个生产者和消费者模式，编解码器对象是消费者。目前设置的最大缓冲队列是200帧。
 *
 *  3.接收数据包和发送数据包线程都做了安全处理。
 *
 *
 *
 *
 */

#ifndef DARPLAYER_BASEDECODE_H
#define DARPLAYER_BASEDECODE_H


#include "../DObserver.h"
#include "../beans/DParameter.h"
#include <list>

class BaseDecode: public DObserver{
public:
    //C++11：=0为纯虚函数，要求继承者必须实现该方法。
    virtual bool Open(DParameter dParameter,bool isHard=false) = 0;

    //关闭解码器。
    virtual void Close() = 0;

    //数据清理。
    virtual void Clear();

    //future模型 发送数据到线程解码
    virtual bool SendPacket(BaseData pkt) = 0;

    //从线程中获取解码结果  再次调用会复用上次空间，线程不安全
    virtual BaseData RecvFrame() = 0;

    bool isAudio = false;

    //由主体notify的数据 阻塞。
    virtual void Update(BaseData pkt);

    //最大的队列缓冲
    int maxList = 150;

    //同步时间，再次打开文件要清理
    //同步pts，目前用的是随音频同步，该值为当前音频的pts。
    int synPts = 0;

    //解码的pts。
    int pts = 0;

protected:
    //实现线程main接口。
    virtual void Main();

    //缓冲区队列。
    std::list<BaseData> packs;
    //线程安全Mutex。
    std::mutex packsMutex;

private:


};


#endif //DARPLAYER_BASEDECODE_H
