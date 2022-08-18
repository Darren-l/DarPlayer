//
// Created by Darren on 2020/2/18.
//

/**
 *
 * DarrenAdd20200218: 对外暴露接口代理类，增加player对象的线程安全调用。
 *
 *
 */

#ifndef DARPLAYER_IPLAYERPORXY_H
#define DARPLAYER_IPLAYERPORXY_H


#include "IPlayer.h"
#include <mutex>

class IPlayerPorxy : public IPlayer {

public:
    static IPlayerPorxy*Get()
    {
        static IPlayerPorxy px;
        return &px;
    }
    void Init(void *vm = 0);

    virtual bool Open(const char *path);
    virtual bool Start();
    virtual void Close();
    virtual void InitView(void *win);

    //获取当前的播放进度 0.0 ~ 1.0
    virtual double PlayPos();
    //快进到指定位置。
    virtual bool Seek(double pos);
    virtual void SetPause(bool isP);
    //判断线程是否暂停。
    virtual bool IsPause();

protected:
    IPlayerPorxy(){}
    IPlayer *player = 0;
    std::mutex mux;

};


#endif //DARPLAYER_IPLAYERPORXY_H
