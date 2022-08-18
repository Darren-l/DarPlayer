//
// Created by Darren on 2020/2/12.
//

#include "BaseDecode.h"
#include "../tools/Log.h"


/**
 * 从生产者Demux解封装对象获取观察数据，并将数据放进缓冲列表。
 *
 * DarrenNote: 观察者模式是阻塞模式，当Update被阻塞时，数据也将被阻塞。
 *
 * @param pkt
 */
void BaseDecode::Update(BaseData pkt) {
    //这里处理下，视频的接视频包，音频接音频包。
    if (pkt.isAudio != isAudio) {
        return;
    }
    while (!isExit) {
        //线程安全。
        packsMutex.lock();
        //如果队列满了，则会被这里阻塞。
        if (packs.size() < maxList) {
            //将生产者的数据丢到队列中
            packs.push_back(pkt);
            packsMutex.unlock();
            break;
        }
        packsMutex.unlock();
        Sleep(1);
    }
}


/**
 *
 * 数据清理，主要是缓冲队列清理。
 *
 *
 */

void BaseDecode::Clear() {
    packsMutex.lock();
    while (!packs.empty()) {
        packs.front().Drop();
        packs.pop_front();
    }
    pts = 0;
    synPts = 0;
    packsMutex.unlock();
}


/**
 *
 * 从队列中取出数据（消费者），发送到解码线程队列中，待解码后，再将数据发送给观察者（主题）。
 *
 * DarrenNote：此处设计为编解码对象即是观察者，也是主题。在接收解封装器数据时，编解码对象为观察者。解码后
 * 数据发送，编解码对象此时为主题，接收方为观察者。
 *
 *
 */
void BaseDecode::Main() {
    while (!isExit) {

        //判断外部是否调用了暂停。
        if (IsPause()) {
            Sleep(2);
            continue;
        }

        packsMutex.lock();
        //首先判断下缓冲队列是否有数据，没数据就轮询。


        //判断音视频同步，如果视频解码进度大于音频当前播放进度，则等待。
        if (!isAudio && synPts > 0) {
            if (synPts < pts) {
                packsMutex.unlock();
                Sleep(1);
                continue;
            }
        }

        /**
         *
         * 没数据或读取到最后。
         *
         */
        if (packs.empty()) {
            packsMutex.unlock();
            Sleep(1);
            continue;
        }
        //从缓冲队列中取出packet（此时是消费者）。
        BaseData pack = packs.front();
        packs.pop_front();

        //发送数据到解码线程队列，一个数据包，可能解码多个结果,所以需要做轮询。
        //注意：这里的解码线程队列是ffmpeg自己的解码队列。
        if (this->SendPacket(pack)) {
            while (!isExit) {
                //获取解码数据
                BaseData frame = this->RecvFrame();
                //数据为空时，代表该数据包已经解码结束，此时跳出循环，去拿下一个数据包。
                if (!frame.data)
                    break;
//                LOGI("RecvFrame %d",frame.size);
                //这里保存下当前解码后的pts，做同步使用。
                pts = frame.pts;
                //发送数据给观察者。（此时编解码对象是主题）
                this->Notify(frame);
            }
        }
        //释放内存。
        pack.Drop();
        packsMutex.unlock();
    }
}
