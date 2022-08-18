//
// Created by Darren on 2020/2/13.
//



#include "XTexture.h"
#include "tools/Log.h"
#include "moduleVideo/XEGL.h"
#include "moduleVideo/XShader.h"


/**
 *
 * 代码较为简单，直接使用内部类继承。
 *
 * 封装Draw方法，包括shader的draw和EGL的draw。
 *
 */
class CXTexture : public XTexture {
public:
    //保存下创建shader。
    XShader sh;

    //保存下视频转码后的格式。
    XTextureType type;

    std::mutex mux;

    virtual void Drop() {
        mux.lock();
        XEGL::Get()->Close();
        sh.Close();
        mux.unlock();
        delete this;
    }

    virtual bool Init(void *win, XTextureType type) {
//        LOGE("CXTexture::Init...");
        mux.lock();
        XEGL::Get()->Close();
        sh.Close();
        this->type = type;
        if (!win) {
            mux.unlock();
            LOGE("CXTexture::Init:XTexture Init failed win is NULL!!");
            return false;
        }
        //EGL初始化：EGL只能初始化一次，否则会导致视频黑屏。
        if (!XEGL::Get()->Init(win)) {
            mux.unlock();
            LOGE("CXTexture::Init:get XEGL i is NULL!!");
            return false;
        }
        //shader初始化。
//        sh.Init((XShaderType)type);
        if (!sh.Init((XShaderType) type)) {
            LOGE("GLuint::InitShader:init Shader error！!");
        }
        mux.unlock();
        return true;
    }


    virtual void Draw(unsigned char *data[], int width, int height) {
        //设置纹理材质:yuv420。
        mux.lock();
        sh.GetTexture(0, width, height, data[0]);  // Y

        if (type == XTEXTURE_YUV420P) {
            sh.GetTexture(1, width / 2, height / 2, data[1]);  // U
            sh.GetTexture(2, width / 2, height / 2, data[2]);  // V
        } else {
            sh.GetTexture(1, width / 2, height / 2, data[1], true);  // UV
        }
        //shader渲染。
        sh.Draw();
        //EGL渲染。
        XEGL::Get()->Draw();
        mux.unlock();
    }


};

XTexture *XTexture::Create() {
    return new CXTexture();
}