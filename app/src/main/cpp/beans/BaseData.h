//
// Created by Darren on 2020/2/10.
//

/**
 *
 * DarrenAdd20200210:数据存储基类。
 *
 * 1. 用于保存读取视频文件的pactet数据。
 *
 * 2. 用于保存解码后的原始帧数据。
 *
 *
 */

#ifndef DARPLAYER_BASEDATA_H
#define DARPLAYER_BASEDATA_H

//数据类型，音频包，视频包。
enum XDataType
{
    //视频类型
    AVPACKET_TYPE = 0,
    //音频类型。
    UCHAR_TYPE = 1
};


struct BaseData {

public:
    //当前对象的数据类型，XDataType视频或音频。
    int type = 0;

    //存放数据对象的指针。
    unsigned char *data = 0;

    //该数据是否为音频。
    bool isAudio = false;
    //当前数据大小
    int size = 0;

    //视频宽高。
    int width = 0;
    int height = 0;

    //解码后的数据类型，如yuv420,nv12,nv21。
    int format = 0;

    //存放视频数据的指针。
    unsigned char *datas[8] = {0};

    //当前播放进度。
    int pts = 0;

    //数据清理。
    void Drop();

    //申请空间。
    bool Alloc(int size,const char *data=0);
};


#endif //DARPLAYER_BASEDATA_H
