//
// Created by Darren on 2020/2/10.
//


#include "BaseData.h"

extern "C" {
#include <libavcodec/avcodec.h>
}

void BaseData::Drop() {
    if (!data)
        return;
    if (type == AVPACKET_TYPE)
        av_packet_free((AVPacket **) &data);
    else
        delete data;
    data = 0;
    size = 0;
}


bool BaseData::Alloc(int size, const char *data) {

    Drop();
    //默认为音频的类型。
    type = UCHAR_TYPE;
    if (size <= 0)
        return false;
    this->data = new unsigned char[size];
    if (!this->data)
        return false;
    if (data) {
        //将数据拷贝一份。
        memcpy(this->data, data, size);
    }
    this->size = size;
    return true;
}

