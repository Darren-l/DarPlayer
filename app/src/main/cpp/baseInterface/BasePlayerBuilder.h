//
// Created by Darren on 2020/2/17.
//

/**
 *
 * DarrenAdd20200217:增加一个返回player对象的构建者基类。
 *
 * 该类会自动构建player对象，调用者无需关注player内部实现。
 *
 *
 */
#ifndef DARPLAYER_BASEPLAYERBUILDER_H
#define DARPLAYER_BASEPLAYERBUILDER_H

#include "../IPlayer.h"

class BasePlayerBuilder {
public:
    //默认index为0，只有一个播放窗口。
    virtual IPlayer *BuilderPlayer(unsigned char index=0);
protected:
    virtual BaseDemux *CreateDemux() = 0;
    virtual BaseDecode *CreateDecode() = 0;
    virtual BaseResample *CreateResample() = 0;
    virtual BaseVideoView *CreateVideoView()  = 0;
    virtual BaseAudioPlay *CreateAudioPlay() = 0;
    virtual IPlayer *CreatePlayer(unsigned char index=0) = 0;

};


#endif //DARPLAYER_BASEPLAYERBUILDER_H
