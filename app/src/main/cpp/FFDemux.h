//
// Created by Darren on 2020/2/11.
//

/**
 *
 * DarrenAdd20200211:ffmpeg解封装器类。
 *
 *
 */

#ifndef DARPLAYER_FFDEMUX_H
#define DARPLAYER_FFDEMUX_H


#include "baseInterface/BaseDemux.h"
#include "beans/DParameter.h"

struct AVFormatContext;

class FFDemux: public BaseDemux {

public:

    //打开一个视频文件或流媒体的url，支持http，https，rmtp，rtsp。
    virtual bool Open(const char *url);

    //关闭解封装器。
    virtual void Close();

    //seek 位置 pos 0.0~1.0
    virtual bool Seek(double pos);

    //获取视频参数,open后从流的头部Box中获取。
    virtual DParameter GetVideoParam();

    //获取音频参数
    virtual DParameter GetAudioParam();

    //读取一帧数据。（注意：该数据不会被清理，需要调用者清理）
    virtual BaseData Read();

    FFDemux();


private:
    //DarrenNote:C++11特性，允许私有变量在定义中初始化，但构造方法必须为无参构造。
    //否则对象变量初始化应该放到构造方法中。
    AVFormatContext *avFormatContext = 0;
    //音视频索引号。
    int audioStream = 1;
    int videoStream = 0;

    std::mutex mux;

protected:


};


#endif //DARPLAYER_FFDEMUX_H
