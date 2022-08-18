//
// Created by Darren on 2020/2/17.
//

#include "FFPlayerBuilder.h"
#include "FFDemux.h"
#include "FFdecode.h"
#include "moduleAudio/FFResample.h"
#include "moduleVideo/GLVideoView.h"
#include "moduleAudio/SLAudioPlay.h"

BaseDemux *FFPlayerBuilder::CreateDemux()
{
    BaseDemux *ff = new FFDemux();
    return ff;
}

BaseDecode *FFPlayerBuilder::CreateDecode()
{
    BaseDecode *ff = new FFDecode();
    return ff;
}

BaseResample *FFPlayerBuilder::CreateResample()
{
    BaseResample *ff = new FFResample();
    return ff;
}

BaseVideoView *FFPlayerBuilder::CreateVideoView()
{
    BaseVideoView *ff = new GLVideoView();
    return ff;
}

BaseAudioPlay *FFPlayerBuilder::CreateAudioPlay()
{
    BaseAudioPlay *ff = new SLAudioPlay();
    return ff;
}

IPlayer *FFPlayerBuilder::CreatePlayer(unsigned char index)
{
    return IPlayer::Get(index);
}

void FFPlayerBuilder::InitHard(void *vm)
{
    FFDecode::InitHard(vm);
}