//
// Created by Darren on 2020/2/12.
//

/**
 *
 * DarrenAdd2020212:FFmpeg解码器类。
 *
 * 考虑到后期可能更换解码器，把解码器部分抽象出来。
 *
 * 1. 目前解码器有两个，一个是音频解码器，一个是视频解码器，都需要单独初始化，二者区别为Stream的
 * para参数不同。
 * 2. 两个解码器都是通过观察者回调从封装器拿数据。
 * 3.
 *
 *
 */

#ifndef DARPLAYER_FFDECODE_H
#define DARPLAYER_FFDECODE_H


#include "baseInterface/BaseDecode.h"
struct AVCodecContext;
struct AVFrame;
class FFDecode : public BaseDecode {

public:
    static void InitHard(void *vm);

    virtual bool Open(DParameter para,bool isHard=false);

    virtual void Close();

    virtual void Clear();

    //future模型 发送数据到线程解码
    virtual bool SendPacket(BaseData pkt);

    //从线程中获取解码结果  再次调用会复用上次空间，线程不安全
    virtual BaseData RecvFrame() ;


protected:
    AVCodecContext *avCodecContext = 0;

    //解码后的原始帧缓冲区
    AVFrame *frame = 0;

    std::mutex mux;

};


#endif //DARPLAYER_FFDECODE_H
