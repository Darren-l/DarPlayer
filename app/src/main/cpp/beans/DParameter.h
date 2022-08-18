//
// Created by Darren on 2020/2/12.
//

/**
 *
 * DarrenAdd20200212:
 *
 *  该类用于保存解码器解码后传递给播放模块的输入参数，包括视频模块和音频模块。
 *
 *
 */

#ifndef DARPLAYER_DPARAMETER_H
#define DARPLAYER_DPARAMETER_H

struct AVCodecParameters;
class DParameter {
public:
    AVCodecParameters *para = 0;

    //默认通道数
    int channels = 2;
    //默认采样频率。
    int sample_rate = 44100;

};


#endif //DARPLAYER_DPARAMETER_H
