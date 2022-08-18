//
// Created by Darren on 2020/2/13.
//


/**
 *
 * 使用OpenGl渲染视频。
 *
 * 负责连接FFmpeg，作为解码器对象的观察者，帮助Texture从观察者中获取解码后的data数据。从java层获取窗口句柄。
 *
 *
 */

#ifndef DARPLAYER_GLVIDEOVIEW_H
#define DARPLAYER_GLVIDEOVIEW_H


#include "../baseInterface/BaseVideoView.h"
#include "../tools/Log.h"
class XTexture;

class GLVideoView : public BaseVideoView {
public:
    //设置窗口句柄。
    virtual void SetRender(void *win);
    //updata调用该方法，进行新数据渲染。
    virtual void Render(BaseData data);
    virtual void Close();


protected:
    void *view = 0;
    XTexture *txt = 0;
    std::mutex mux;

};


#endif //DARPLAYER_GLVIDEOVIEW_H
