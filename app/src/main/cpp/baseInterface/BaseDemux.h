//
// Created by Darren on 2020/2/10.
//

/**
 *
 * DarrenAdd20200210：解封装抽象基类。
 *
 */

#ifndef DARPLAYER_BASEDEMUX_H
#define DARPLAYER_BASEDEMUX_H


#include "../beans/BaseData.h"
#include "../DObserver.h"
#include "../beans/DParameter.h"

class BaseDemux:public DObserver {

public:
    //打开一个视频文件或流媒体的url，支持http，https，rmtp，rtsp。
    virtual bool Open(const char *url) = 0;

    //关闭解封装器
    virtual void Close() = 0;

    //seek 位置 pos 0.0~1.0
    virtual bool Seek(double pos) = 0;

    //读取一帧数据。（注意：该数据不会被清理，需要调用者清理）
    virtual BaseData Read() = 0;

    //获取视频参数
    virtual DParameter GetVideoParam() = 0;

    //获取音频参数
    virtual DParameter GetAudioParam() = 0;

    //总时长，单位：毫秒。该时长是从流的头部Box中获取，部分流可能不存在box头，无法获取，如：flv。
    int totalMs = 0;

protected:
    virtual void Main();

};


#endif //DARPLAYER_BASEDEMUX_H
