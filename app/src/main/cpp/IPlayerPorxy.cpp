//
// Created by Darren on 2020/2/18.
//

/**
 *
 * DarrenNote20200219:所有调用都加上线程安全锁，增加线程安全，规避多线程同时调用该对象的方法，引起的线程安全问题。
 *
 * 确保线程调用该类方法，执行完后能继续下一个调用。
 *
 */

#include "IPlayerPorxy.h"
#include "FFPlayerBuilder.h"


/**
 *
 * 关闭播放器并清理内存。
 *
 *
 */

void IPlayerPorxy::Close() {
    mux.lock();
    if (player)
        player->Close();
    mux.unlock();
}

/**
 *
 * 传递vm环境（硬解码时需要该参数），初始化播放器所需对象，如解封装器，解码器等。
 *
 *
 * @param vm：虚拟机对象。
 */
void IPlayerPorxy::Init(void *vm) {
    mux.lock();
    if (vm) {
        FFPlayerBuilder::InitHard(vm);
    }
    if (!player)
        player = FFPlayerBuilder::Get()->BuilderPlayer();

    mux.unlock();
}


/**
 *
 * 快进到指定位置，pos in 0~1
 *
 *
 * @param pos
 * @return
 */
bool IPlayerPorxy::Seek(double pos) {
    bool re = false;
    mux.lock();
    if (player) {
        re = player->Seek(pos);
    }
    mux.unlock();
    return re;
}


//获取当前的播放进度 0.0 ~ 1.0
double IPlayerPorxy::PlayPos() {
    double pos = 0.0;
    mux.lock();
    if (player) {
        pos = player->PlayPos();
    }
    mux.unlock();
    return pos;
}


bool IPlayerPorxy::IsPause() {
    bool re = false;
    mux.lock();
    if (player)
        re = player->IsPause();
    mux.unlock();
    return re;
}

void IPlayerPorxy::SetPause(bool isP) {
    mux.lock();
    if (player)
        player->SetPause(isP);
    mux.unlock();
}


/**
 *
 * 获取播放器所需对象。
 *
 * @param path：视频文件路径，支持http等网络协议。
 * @return
 */
bool IPlayerPorxy::Open(const char *path) {
    bool re = false;
    mux.lock();
    if (player) {
        player->isHardDecode = isHardDecode;
        re = player->Open(path);
    }

    mux.unlock();
    return re;
}

/**
 *
 * 开始解封装，解码线程。
 *
 * @return：返回线程是否启动成功。
 */
bool IPlayerPorxy::Start() {
    bool re = false;
    mux.lock();
    if (player)
        re = player->Start();
    mux.unlock();
    return re;
}

/**
 *
 * 传递窗口句柄。
 *
 * @param win ：窗口句柄。
 */
void IPlayerPorxy::InitView(void *win) {
    mux.lock();
    if (player)
        player->InitView(win);
    mux.unlock();
}