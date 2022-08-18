//
// Created by Darren on 2020/2/17.
//

#include "BasePlayerBuilder.h"
#include "BaseVideoView.h"
#include "BaseResample.h"
#include "BaseDecode.h"
#include "BaseAudioPlay.h"
#include "BaseDemux.h"


IPlayer *BasePlayerBuilder::BuilderPlayer(unsigned char index)
{
    IPlayer *play = CreatePlayer(index);

    //获取解封装器。
    BaseDemux *ffDemux = CreateDemux();

    //获取视频编解码器。
    BaseDecode *vdecode = CreateDecode();

    //获取频编解码器。
    BaseDecode *adecode = CreateDecode();

    //把解封装添加到解码器的观察者队列中。
    ffDemux->AddObs(vdecode);
    ffDemux->AddObs(adecode);

    //显示观察视频解码器
    //把IVideoView加到解码器观察者队列。
    BaseVideoView *view = CreateVideoView();
    vdecode->AddObs(view);

    //初始化重采样对象，并把自己加入decodec观察者队列。
    BaseResample *resample = CreateResample();
    adecode->AddObs(resample);

    //初始化SL播放器对象，并将播放器作为重采样的观察者，加入观察者队列。
    BaseAudioPlay *audioPlay = CreateAudioPlay();
    resample->AddObs(audioPlay);

    play->demux = ffDemux;
    play->adecode = adecode;
    play->vdecode = vdecode;
    play->videoView = view;
    play->resample = resample;
    play->audioPlay = audioPlay;
    return play;
}