//
// Created by Darren on 2020/2/11.
//


#include "FFDemux.h"
#include "tools/Log.h"

extern "C" {
#include <libavformat/avformat.h>
}


//分数转为浮点数
static double r2d(AVRational r) {
    return r.num == 0 || r.den == 0 ? 0. : (double) r.num / (double) r.den;
}


/**
 *
 * 关闭解封装器。
 *
 *
 */
void FFDemux::Close() {
    mux.lock();
    if (avFormatContext)
        avformat_close_input(&avFormatContext);
    mux.unlock();
}


/**
 *
 * 打开一个视频流文件。
 *
 * 该方法可以获取视频流文件相关信息，主要是从头部box中获取，如视频封装格式，编码格式等。
 *
 * @param url ：本地视频文件，或流媒体url地址。
 * @return：true成功，false：失败。
 */

bool FFDemux::Open(const char *url) {

    LOGI("FFDemux::Open... url=%s", url);

    Close();

    //调用该方法会打开视频文件的头部box，从里面拿出视频相关信息并赋值。
    //注意，不是所有视频都要头部box，如flv格式就没有头部信息。
    mux.lock();
    int ret = avformat_open_input(&avFormatContext, url, NULL, NULL);
    if (ret != 0) {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
//        LOGE("Open: call avformat_open_input error, averrMes=%s" ,av_err2str(ret));
        LOGE("Open: call avformat_open_input error!! url=%s", url);
        return false;
    }
    LOGE("Open: call avformat_open_input success!!");


    //获取流的信息。
    //没有头部信息的码流，会从这里获取视频信息，主要通过pps和sps获取。
    ret = avformat_find_stream_info(avFormatContext, NULL);
    if (ret != 0) {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf));
//        LOGE("openPlay: call avformat_find_stream_info error,averrMes=%s" ,av_err2str(ret));
        LOGE("Open: call avformat_find_stream_info error!! url=%s", url);
        return false;
    }
    //打印下获取的视频时长和流的个数。
    this->totalMs = avFormatContext->duration / (AV_TIME_BASE / 1000);

    mux.unlock();
    LOGE("Open: call avformat_find_stream_info success!! duration=%d , numStreams=%d",
         this->totalMs, avFormatContext->nb_streams);



    //打开流后，这里就可以去获取下音视频的参数信息。~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~!!!这里感觉没有必要，可删除。
//    GetVideoParam();
//    GetAudioParam();

    return true;
}


//seek 位置 pos 0.0~1.0
bool FFDemux::Seek(double pos) {
    if (pos < 0 || pos > 1) {
        LOGE("Seek value must 0.0~1.0");
        return false;
    }
    bool re = false;
    mux.lock();
    if (!avFormatContext) {
        mux.unlock();
        return false;
    }
    //清理读取的缓冲
    avformat_flush(avFormatContext);
    long long seekPts = 0;
    //获取该位置的pts值。
    seekPts = avFormatContext->streams[videoStream]->duration * pos;

    //往后跳转到关键帧
    re = av_seek_frame(avFormatContext, videoStream, seekPts,
                       AVSEEK_FLAG_FRAME | AVSEEK_FLAG_BACKWARD);
    mux.unlock();
    return re;
}


/**
 *
 * 读取一帧数据.
 *
 * @return ：返回数据baseData对象。(注意返回指针需要做清理)
 */
BaseData FFDemux::Read() {
//    LOGI("FFDemux::Read... ");
    mux.lock();
    if (!avFormatContext) {
        LOGE("FFDemux::Read: avFormatContext is null!! ");
        mux.unlock();
        return BaseData();
    }

    BaseData baseData;

    AVPacket *avPacket = av_packet_alloc();
    int ret = av_read_frame(avFormatContext, avPacket);
    if (ret != 0) {
        //出错或读完,则返回数据。
        mux.unlock();
        av_packet_free(&avPacket);
        return baseData;
    }

//    LOGI("Read:... curren packet size=%d,pts=%lld",avPacket->size,avPacket->pts);
    //成功读取一帧，则存在baseData里。
    baseData.data = (unsigned char *) avPacket;
    baseData.size = avPacket->size;

    //这里再存一下流信息,区分当前data是什么类型的流。
    if (avPacket->stream_index == audioStream) {
        baseData.isAudio = true;
    } else if (avPacket->stream_index == videoStream) {
        baseData.isAudio = false;
    } else {
        mux.unlock();
        av_packet_free(&avPacket);
        return BaseData();
    }



    //转换pts
    avPacket->pts = avPacket->pts *
                    (1000 * r2d(avFormatContext->streams[avPacket->stream_index]->time_base));
    avPacket->dts = avPacket->dts *
                    (1000 * r2d(avFormatContext->streams[avPacket->stream_index]->time_base));
    baseData.pts = (int) avPacket->pts;

    mux.unlock();

    return baseData;

}


/**
 *
 * 获取视频流参数。
 *
 * @return 返回参数对象。
 */
DParameter FFDemux::GetVideoParam() {
    mux.lock();
    if (!avFormatContext) {
        mux.unlock();
        LOGE("FFDemux::GetVideoParam: avFormatContext is NULL！");
        return DParameter();
    }
    //获取了视频流索引
    int re = av_find_best_stream(avFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (re < 0) {
        mux.unlock();
        LOGE("FFDemux::GetVideoParam:: call av_find_best_stream failed!");
        return DParameter();
    }
    videoStream = re;

    DParameter para;
    para.para = avFormatContext->streams[re]->codecpar;


    mux.unlock();

    LOGI("FFDemux::GetVideoParam:para.para->format=%d。codecId=%d", para.para->format,
         para.para->codec_id);
    //获取下avstream，打印流的信息。
//    AVStream *avStream = avFormatContext->streams[re];
    LOGI("FFDemux::GetVideoParam: video Data，get data from avStream,width=%d height=%d codeid=%d pixformat=%d",
         avFormatContext->streams[re]->codecpar->width,
         avFormatContext->streams[re]->codecpar->height,
         avFormatContext->streams[re]->codecpar->codec_id,   //编解码格式：如：h264
         avFormatContext->streams[re]->codecpar->format         //像素格式：如yuv
    );


    return para;
}

/**
 *
 * 获取音频流参数。
 *
 * @return: 返回参数对象。
 */
DParameter FFDemux::GetAudioParam() {
    mux.lock();
    if (!avFormatContext) {
        LOGE("FFDemux::GetAudioParam: avFormatContext is NULL！");
        mux.unlock();
        return DParameter();
    }
    //获取了视频流索引
    int re = av_find_best_stream(avFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if (re < 0) {
        LOGE("FFDemux::GetAudioParam:: call av_find_best_stream failed!");
        mux.unlock();
        return DParameter();
    }
    audioStream = re;

    DParameter para;
    para.para = avFormatContext->streams[re]->codecpar;
    para.channels = avFormatContext->streams[re]->codecpar->channels;
    para.sample_rate = avFormatContext->streams[re]->codecpar->sample_rate;

    mux.unlock();

    //获取下avstream，打印流的信息。
//    AVStream *avStream = avFormatContext->streams[re];
    LOGI("openPlay: audio Data，sample_rate=%d channels=%d sample_format=%d",
         avFormatContext->streams[re]->codecpar->sample_rate, //每秒音频采样频率：如44100（CD）
         avFormatContext->streams[re]->codecpar->channels,  //声道数
         avFormatContext->streams[re]->codecpar->format     //采样格式：如pcm signed 32。
    );

    return para;
}


/**
 * FFmpeg相关初始化放到对象构造方法中。
 *
 */
FFDemux::FFDemux() {

    LOGI("FFDemux:create FFDemux obj...");
    //注册初始化封装相关。
    av_register_all();

    //注册初始化网络,初始化解码器相关。
    avcodec_register_all();
    avformat_network_init();

    LOGI("FFDemux：register all end!");

}


