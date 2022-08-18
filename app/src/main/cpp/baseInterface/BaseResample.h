//
// Created by Darren on 2020/2/15.
//

/**
 *
 * DarrenAdd20200215:音频重采样基类。
 *
 * 继承观察者，该类为解码器的观察者，接收解码后的音频数据。
 *
 */

#ifndef DARPLAYER_BASERESAMPLE_H
#define DARPLAYER_BASERESAMPLE_H


#include "../beans/DParameter.h"
#include "../beans/BaseData.h"
#include "../DObserver.h"

class BaseResample : public DObserver {
public:
    virtual bool Open(DParameter in,DParameter out=DParameter()) = 0;
    virtual BaseData Resample(BaseData indata) = 0;
    virtual void Close() = 0;
    virtual void Update(BaseData data);

    //默认声道数。
    int outChannels = 2;
    int outFormat = 1;

};


#endif //DARPLAYER_BASERESAMPLE_H
