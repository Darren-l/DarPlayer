//
// Created by Darren on 2020/2/13.
//


/**
 *
 * 该类负责创建shader和XEGL并初始化。由VIdeoVIew负责调用。
 *
 * 该封装类符合开闭原则，开放扩展，封闭修改。
 *
 *
 *
 */

#ifndef DARPLAYER_XTEXTURE_H
#define DARPLAYER_XTEXTURE_H

//定义一个解码后的视频格式枚举，暂时只支持三种类型。
enum XTextureType
{
    XTEXTURE_YUV420P = 0,  // Y 4  u 1 v 1
    XTEXTURE_NV12 = 25,    // Y4   uv1
    XTEXTURE_NV21 = 26     // Y4   vu1

};


class XTexture {
public:
    static XTexture *Create();
    virtual bool Init(void *win,XTextureType type=XTEXTURE_YUV420P) = 0;
    virtual void Draw(unsigned char *data[],int width,int height) = 0;
    virtual void Drop() = 0;
    virtual ~XTexture(){};

protected:
    XTexture(){};

};


#endif //DARPLAYER_XTEXTURE_H
