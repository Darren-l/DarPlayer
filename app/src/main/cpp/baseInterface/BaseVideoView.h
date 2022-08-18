//
// Created by Darren on 2020/2/13.
//

/**
 *
 * 负责连接FFmpeg，作为解码器对象的观察者，帮助Texture从观察者中获取解码后的data数据。从java层获取窗口句柄。
 *
 *
 */

#ifndef DARPLAYER_BASEVIDEOVIEW_H
#define DARPLAYER_BASEVIDEOVIEW_H


#include "../beans/BaseData.h"
#include "../DObserver.h"

class BaseVideoView : public DObserver{
public:
    virtual void SetRender(void *win) = 0;
    virtual void Render(BaseData data) = 0;
    virtual void Update(BaseData data);
    virtual void Close() = 0;

};


#endif //DARPLAYER_BASEVIDEOVIEW_H
