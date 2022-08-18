//
// Created by Darren on 2020/2/13.
//

#include "GLVideoView.h"
#include "../XTexture.h"

/**
 *
 * 获取窗口句柄，该句柄会传递给EGL。
 *
 * @param win
 */
void GLVideoView::SetRender(void *win) {
    this->view = win;
}

/**
 *
 * 关闭时，需要释放资源。
 *
 *
 */
void GLVideoView::Close() {
    mux.lock();
    if (txt) {
        txt->Drop();
        txt = 0;
    }

    mux.unlock();
}


/**
 *
 * 负责数据的渲染，在观察者回调中会调用该接口。
 *
 *
 */
void GLVideoView::Render(BaseData data) {
    if (!view) {
        LOGE("GLVideoView::Render:the windows is NULL!!");
        return;
    }
    if (!txt) {
        txt = XTexture::Create();
        txt->Init(view, (XTextureType) data.format);
    }

    txt->Draw(data.datas, data.width, data.height);

}
