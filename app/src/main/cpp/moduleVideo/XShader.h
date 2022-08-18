//
// Created by Darren on 2020/2/13.
//

/**
 *
 * OpenGL开放图形库的着色器，目前设计方案是视频提取出原始帧后用该库进行渲染。规避使用ffmpeg进行
 *
 * yuv转rgb显示时cpu占用率高的问题。
 *
 * 1. 顶点着色器：3D绘制，目前是2d所以z轴坐标是0。
 * 2. 片元着色器。
 *
 *
 */

#ifndef DARPLAYER_XSHADER_H
#define DARPLAYER_XSHADER_H

#include <mutex>

enum XShaderType
{
    XSHADER_YUV420P = 0,    //软解码和虚拟机
    XSHADER_NV12 = 25,      //手机
    XSHADER_NV21 = 26
};

class XShader {

public:
    virtual bool Init(XShaderType type=XSHADER_YUV420P);

    virtual void Close();

    //获取材质并映射到内存
    ////抽象出该方法，用于不同格式，设置纹理材质。
    virtual void GetTexture(unsigned int index,int width,int height, unsigned char *buf, bool isa=false);
    virtual void Draw();

protected:
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;
    unsigned int texts[100] = {0};
    std::mutex mux;

};


#endif //DARPLAYER_XSHADER_H
