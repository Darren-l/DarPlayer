//
// Created by Darren on 2020/2/17.
//

/**
 *
 * DarrenAdd20200217:使用建造者模式增加一个返回player对象的构建者。
 *
 * 该类会自动构建player对象，调用者无需关注player内部实现。
 *
 * 该类使用FFmpeg，初始化各个对象，包括：解封装器Demux，解码器Decode，重采样resample，
 * 控制视频渲染videoView，控制音频播放audioPlay。
 *
 *
 */

#ifndef DARPLAYER_FFPLAYERBUILDER_H
#define DARPLAYER_FFPLAYERBUILDER_H


#include "baseInterface/BasePlayerBuilder.h"

class FFPlayerBuilder: public BasePlayerBuilder {
public:
    static void InitHard(void *vm);
    static FFPlayerBuilder *Get()
    {
        static FFPlayerBuilder ff;
        return &ff;
    }
protected:
    FFPlayerBuilder(){};
    virtual BaseDemux *CreateDemux();
    virtual BaseDecode *CreateDecode();
    virtual BaseResample *CreateResample();
    virtual BaseVideoView *CreateVideoView();
    virtual BaseAudioPlay *CreateAudioPlay();
    virtual IPlayer *CreatePlayer(unsigned char index=0);

};


#endif //DARPLAYER_FFPLAYERBUILDER_H
