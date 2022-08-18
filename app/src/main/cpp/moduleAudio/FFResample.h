//
// Created by Darren on 2020/2/15.
//

#include "../baseInterface/BaseResample.h"

#ifndef DARPLAYER_FFRESAMPLE_H
#define DARPLAYER_FFRESAMPLE_H

struct SwrContext;
class FFResample : public BaseResample{
public:
    virtual bool Open(DParameter in,DParameter out=DParameter());
    virtual void Close();
    virtual BaseData Resample(BaseData indata);

protected:
    SwrContext *actx = 0;
    std::mutex mux;


};


#endif //DARPLAYER_FFRESAMPLE_H
