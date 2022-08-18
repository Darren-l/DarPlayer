//
// Created by Darren on 2020/2/17.
//

#ifndef DARPLAYER_IPLAYER_H
#define DARPLAYER_IPLAYER_H


/**
 *
 * DarrenAdd20200217: 对使用者暴露的接口类。
 *
 *
 */

#include "tools/Thread.h"
#include "beans/DParameter.h"
#include <mutex>

class BaseDemux;
class BaseAudioPlay;
class BaseVideoView;
class BaseResample;
class BaseDecode;

class IPlayer : public Thread {
public:
    static IPlayer *Get(unsigned char index=0);
    virtual bool Open(const char *path);
    virtual bool Start();
    virtual void InitView(void *win);
    virtual void Close();


    //获取当前的播放进度 0.0 ~ 1.0
    virtual double PlayPos();
    virtual bool Seek(double pos);
    virtual void SetPause(bool isP);

    //是否视频硬解码
    bool isHardDecode = true;

    //音频输出参数配置
    DParameter outPara;

    BaseDemux *demux = 0;
    BaseDecode *vdecode = 0;
    BaseDecode *adecode = 0;
    BaseResample *resample = 0;
    BaseVideoView *videoView = 0;
    BaseAudioPlay *audioPlay = 0;

protected:
    //用作音视频同步
    void Main();
    //线程安全锁。
    std::mutex mux;
    IPlayer(){};
};


#endif //DARPLAYER_IPLAYER_H
