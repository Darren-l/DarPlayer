//
// Created by Darren on 2020/2/13.
//

/**
 *
 * OpenGL开放图形库的跨平台本地窗口连接器。用于连接sharder和系统平台之间的中间件。
 *
 * 需要拿到平台窗口的句柄。
 *
 *
 */

#ifndef DARPLAYER_XEGL_H
#define DARPLAYER_XEGL_H


class XEGL {

public:
    virtual bool Init(void *win) = 0;
    virtual void Close() = 0;
    virtual void Draw() = 0;
    //静态对象。
    static XEGL *Get();

protected:
    XEGL(){}

private:

};


#endif //DARPLAYER_XEGL_H
