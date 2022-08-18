//
// Created by Darren on 2020/2/16.
//

#include "BaseAudioPlay.h"
#include "../tools/Log.h"

/**
 *
 * 作为重采样的观察者，接收重采样后的数据。
 *
 * 使用阻塞，缓冲数据满后会阻塞。
 *
 * @param data
 */
void BaseAudioPlay::Update(BaseData data) {
//    LOGE("BaseAudioPlay::Update: %d",data.size);
    //压入缓冲队列
    if(data.size<=0|| !data.data) return;
    while(!isExit){
        framesMutex.lock();
        if(frames.size() > maxFrame){
            framesMutex.unlock();
            Sleep(1);
            continue;
        }
        frames.push_back(data);
        framesMutex.unlock();
        break;
    }
}

/**
 * 数据清理。
 *
 *
 */
void BaseAudioPlay::Clear()
{
    framesMutex.lock();
    while(!frames.empty())
    {
        frames.front().Drop();
        frames.pop_front();
    }
    framesMutex.unlock();
}


/**
 *
 * 从缓冲队列中获取数据，阻塞式，没有数据会等待。
 *
 * 这里获取下pts，保存在audioPlay对象中，因为取完数据后就是音频的播放，这里可以看做是当前播放pts。
 *
 *
 * @return
 */
BaseData BaseAudioPlay::GetData()
{
    BaseData d;

    //注意：此处为手动设置线程变量，因为该线程不是单独开启的线程。
    isRuning = true;

    while(!isExit)
    {
        //判断线程线程是否调用了暂停。
        if(IsPause())
        {
            Sleep(2);
            continue;
        }

        framesMutex.lock();
        if(!frames.empty())
        {
            d = frames.front();
            frames.pop_front();
            framesMutex.unlock();
            //获取数据时，保存下当前的播放时间点，同步用。
            pts = d.pts;
            return d;
        }
        framesMutex.unlock();
        Sleep(1);
    }
    isRuning = false;


    return d;
}