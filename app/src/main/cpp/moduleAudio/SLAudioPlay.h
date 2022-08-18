//
// Created by Darren on 2020/2/16.
//

/**
 *
 * OpenSL播放模块。
 *
 */

#ifndef DARPLAYER_SLAUDIOPLAY_H
#define DARPLAYER_SLAUDIOPLAY_H


#include "../baseInterface/BaseAudioPlay.h"

class SLAudioPlay : public BaseAudioPlay{
public:
    virtual bool StartPlay(DParameter out);
    virtual void Close();
    void PlayCall(void *bufq);

    SLAudioPlay();
    virtual ~SLAudioPlay();

protected:
    unsigned char *buf = 0;
    std::mutex mux;


};


#endif //DARPLAYER_SLAUDIOPLAY_H
