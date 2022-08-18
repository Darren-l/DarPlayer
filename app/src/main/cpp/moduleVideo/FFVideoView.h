//
// Created by Darren.liu on 2020/2/25.
//


/**
 *
 * DarrenAdd20200225: 使用FFFmpeg yuv转RGB的方式渲染视频。
 *
 *  该方式渲染视频兼容性较好，但cpu的开销较大。
 *
 */


#ifndef DARPLAYER_FFVIDEOVIEW_H
#define DARPLAYER_FFVIDEOVIEW_H


#include "../baseInterface/BaseVideoView.h"

class FFVideoView : public BaseVideoView {
public:
    //设置窗口句柄。
    virtual void SetRender(void *win);
    //updata调用该方法，进行新数据渲染。
    virtual void Render(BaseData data);
    virtual void Close();

protected:


};


#endif //DARPLAYER_FFVIDEOVIEW_H
