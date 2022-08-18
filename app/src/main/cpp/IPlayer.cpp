//
// Created by Darren on 2020/2/17.
//


#include "baseInterface/BaseDemux.h"
#include "baseInterface/BaseDecode.h"
#include "baseInterface/BaseAudioPlay.h"
#include "baseInterface/BaseVideoView.h"
#include "baseInterface/BaseResample.h"
#include "IPlayer.h"
#include "tools/Log.h"


/**
 *
 * 获取当前类对象。
 *
 * @param index
 * @return
 */
IPlayer *IPlayer::Get(unsigned char index) {
    static IPlayer p[256];
    return &p[index];
}


/**
 *
 * 获取当前播放进度，当前进度=当前解码pts / 解封装器中的总时长。
 *
 * 注意：部分视频总时长解封装器open时无法获取，需要从流中的para中获取，对于该部分的码流，暂时未做兼容，
 * 后期视情况做兼容。
 *
 *
 * @return
 */
double IPlayer::PlayPos() {
    double pos = 0.0;
    mux.lock();

    int total = 0;
    if (demux)
        total = demux->totalMs;
    if (total > 0) {
        if (vdecode) {
            pos = (double) vdecode->pts / (double) total;
        }
    }
    mux.unlock();
    return pos;
}


/**
 *
 * 跳转到指定位置播放。
 *
 *
 *
 * @param pos
 * @return
 */
bool IPlayer::Seek(double pos) {
    bool re = false;
    if (!demux) return false;

    //暂停所有线程
    SetPause(true);

    //清理缓冲
    //2 清理缓冲队列
    mux.lock();
    if (vdecode)
        vdecode->Clear(); //清理缓冲队列，清理ffmpeg的缓冲
    if (adecode)
        adecode->Clear();
    if (audioPlay)
        audioPlay->Clear();


    //seek先跳转到关键帧。
    re = demux->Seek(pos);

    //先判断该数据是否包含有视频。
    if (!vdecode) {
        mux.unlock();
        SetPause(false);
        return re;
    }
    //解码到实际需要显示的帧。
    int seekPts = pos * demux->totalMs;
    while (!isExit) {
        //解封装数据。
        BaseData pkt = demux->Read();

        if (pkt.size <= 0)break;
        if (pkt.isAudio) {              //如果是音频数据。
            if (pkt.pts < seekPts) {
                pkt.Drop();
                continue;
            }
            //写入缓冲队列
            demux->Notify(pkt);
            continue;
        }

        //视频数据。
        //解码需要显示的帧之前的数据
        vdecode->SendPacket(pkt);
        pkt.Drop();
        BaseData data = vdecode->RecvFrame();
        if (data.size <= 0) {
            continue;
        }
        if (data.pts >= seekPts) {
            //vdecode->Notify(data);
            break;
        }
    }
    mux.unlock();

    //处理结束，恢复暂停状态线程。
    SetPause(false);
    return re;
}

/**
 *
 * 视频暂停和恢复。
 *
 *
 * @param isP
 */
void IPlayer::SetPause(bool isP) {
    mux.lock();
    Thread::SetPause(isP);

    //暂停解封装。
    if (demux)
        demux->SetPause(isP);

    //暂停视频解码。
    if (vdecode)
        vdecode->SetPause(isP);

    //暂停音频解码。
    if (adecode)
        adecode->SetPause(isP);

    //暂停音频重采样。
    if (audioPlay)
        audioPlay->SetPause(isP);
    mux.unlock();
}


/**
 *
 * 获取解封装器，解码器，重采样。
 *
 * 可能存在不需要解码或重采样的数据。
 *
 * @param path
 * @return
 */

bool IPlayer::Open(const char *path) {


    Close();
    mux.lock();

    //打开解封装器。
    if (!demux || !demux->Open(path)) {
        LOGE("IPlayer::Open:demux->Open %s failed!", path);
        mux.unlock();
        return false;
    }


    //打开视频编解码器。
    //解码 解码可能不需要，如果是解封之后就是原始数据
    if (!vdecode || !vdecode->Open(demux->GetVideoParam(), isHardDecode)) {
        LOGE("IPlayer::Open:vdecode->Open %s failed!", path);
        //return false;
    }
    //打开音频编解码器。
    if (!adecode || !adecode->Open(demux->GetAudioParam())) {
        LOGE("IPlayer::Open:adecode->Open %s failed!", path);
        //return false;
    }


    //重采样 有可能不需要，解码后或者解封后可能是直接能播放的数据
    //从解封装器中获取下重采样的参数。
//    if (outPara.sample_rate <= 0) {
    outPara = demux->GetAudioParam();
//    }
    //初始化重采样对象。
    if (!resample || !resample->Open(demux->GetAudioParam(), outPara)) {
        LOGE("IPlayer::Open:resample->Open %s failed!", path);
    }
    mux.unlock();
    return true;
}


void IPlayer::Close() {
    mux.lock();
    //2 先关闭主体线程，再清理观察者
    //同步线程
    Thread::stop();
    //解封装
    if (demux)
        demux->stop();
    //解码
    if (vdecode)
        vdecode->stop();
    if (adecode)
        adecode->stop();
    if (audioPlay)
        audioPlay->stop();

    //2 清理缓冲队列
    if (vdecode)
        vdecode->Clear();
    if (adecode)
        adecode->Clear();
    if (audioPlay)
        audioPlay->Clear();

    //3 清理资源
    if (audioPlay)
        audioPlay->Close();
    if (videoView)
        videoView->Close();
    if (vdecode)
        vdecode->Close();
    if (adecode)
        adecode->Close();
    if (demux)
        demux->Close();
    mux.unlock();


}

/**
 *
 * 解封装，音视频解码器启动。
 *
 * 解码器线程需要优先启动，规避后面启动，播放器前几秒数据异常问题。
 *
 *
 * @return
 */
bool IPlayer::Start() {
    mux.lock();

    if (vdecode)
        vdecode->start();

    if (!demux || !demux->start()) {
        mux.unlock();
        LOGE("demux->Start failed!");
        return false;
    }
    if (adecode)
        adecode->start();
    if (audioPlay)
        audioPlay->StartPlay(outPara);
    Thread::start();
    mux.unlock();

    return true;
}


void IPlayer::InitView(void *win) {
    if (videoView) {
        //这里做下close，规避线程引起的窗口对象不同问题。
        videoView->Close();

        videoView->SetRender(win);
    }

}

/**
 *
 * 该线程用于音视频同步，同步方式：
 *
 *  像decode提供音视频播放时间pts，让视频解码器根据音频提供的pts同步解码。
 *
 *
 */
void IPlayer::Main() {
    while (!isExit) {
        mux.lock();
        if (!audioPlay || !vdecode) {
            mux.unlock();
            Sleep(2);
            continue;
        }

        //同步
        //获取音频的pts 告诉视频
        int apts = audioPlay->pts;
//        LOGE("apts = %d", apts);
        vdecode->synPts = apts;

        mux.unlock();
        Sleep(2);
    }
}
