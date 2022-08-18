//
// Created by Darren on 2020/2/12.
//



#include "FFDecode.h"
#include "tools/Log.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavcodec/jni.h>
}


void FFDecode::InitHard(void *vm) {
    av_jni_set_java_vm(vm, 0);
}


/**
 *
 * 关闭解码器。
 *
 *
 */

void FFDecode::Close() {
    BaseDecode::Clear();
    mux.lock();
    pts = 0;
    if (frame)
        av_frame_free(&frame);
    if (avCodecContext) {
        avcodec_close(avCodecContext);
        avcodec_free_context(&avCodecContext);
    }
    mux.unlock();
}


/**
 *
 * 清理缓冲队列，清理FFmpetg的缓冲队列。
 *
 *  父类decode 的clear只是清空缓冲队列，这里增加一个清空FFmpeg的缓冲，同时也需要调用父类的clear。
 *
 *
 */
void FFDecode::Clear() {
    BaseDecode::Clear();
    mux.lock();
    if (avCodecContext)
        avcodec_flush_buffers(avCodecContext);  //清空ffmpeg的缓冲队列。
    mux.unlock();
}

/**
 *
 * 打开FFmpeg的解码器。
 *
 * DarrenNot：音频和视频共用该open接口。两者区分在于para参数中codec_id的不同，
 * 由于para对象是从steam流中获取的，所以音视频肯定不同。
 *
 * @param para ：解码器所需参数。
 * @param isHard ：是否为硬解码。
 * @return ：true成功，false失败。保存
 */

bool FFDecode::Open(DParameter para, bool isHard) {


    Close();
    if (!para.para) {
        return false;
    }

    AVCodecParameters *parameters = para.para;
    //1. 获取软解码器：通过codec_id,区分音频解码器还是视频解码器，因为para对象是从stream流中获取的。
    AVCodec *avCodec = avcodec_find_decoder(parameters->codec_id);

    if (isHard) {
        //获取硬解码器。
        avCodec = avcodec_find_decoder_by_name("h264_mediacodec");
    }

    if (!avCodec) {
        LOGE("FFDecode::Open: call aavcodec_find_decoder error!!");
        return false;
    }

    //2. 获取视频编解码上下文。
    mux.lock();
    avCodecContext = avcodec_alloc_context3(avCodec);
    //将fromateContext相关编解码数据值传递给avCodecCOntext。
    avcodec_parameters_to_context(avCodecContext, parameters);
    //设置解码线程数,建议为4
    avCodecContext->thread_count = 8;


    //3. 打开解码器。
    int ret = avcodec_open2(avCodecContext, NULL, NULL);
    if (ret != 0) {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(ret, buf, sizeof(buf) - 1);
        LOGE("FFDecode::Open: call avcodec_open2 error,av_codec video open error!!averrMes=%s",
             buf);
        return false;
    }

    //记录下当前编解码器的类型。
    if (avCodecContext->codec_type == AVMEDIA_TYPE_VIDEO) {
        this->isAudio = false;
    } else {
        this->isAudio = true;
    }
    mux.unlock();

    LOGE("FFDecode::Open: call avcodec_open2 success!! av_codec type isAudio=%d", this->isAudio);
    return true;
}


/**
 *
 * 发送包到缓冲队列。
 *
 * @param pkt
 * @return
 */

bool FFDecode::SendPacket(BaseData pkt) {
    //包有问题，直接丢弃。
    if (pkt.size <= 0 || !pkt.data) {
        return false;
    }

    mux.lock();

    if (!avCodecContext) {
        mux.unlock();
        return false;
    }

    //发送包到解码器缓冲队列。
    int re = avcodec_send_packet(avCodecContext, (AVPacket *) pkt.data);
    mux.unlock();
    if (re != 0) {
        return false;
    }

    return true;
}

/**
 *
 * 从线程中获取缓冲队列解码结果
 *
 * @return 返回数据对象，包括：该帧的数据data，该帧数据的大小。
 *
 */
BaseData FFDecode::RecvFrame() {

    mux.lock();
    if (!avCodecContext) {
        mux.unlock();
        return BaseData();
    }
    if (!frame) {
        frame = av_frame_alloc();
    }
    //读取一帧。
    int re = avcodec_receive_frame(avCodecContext, frame);
    if (re != 0) {
        mux.unlock();
        return BaseData();
    }

    BaseData baseData;
    baseData.data = (unsigned char *) frame;

    //视频帧和音频帧的大小计算不同，这里做区分。
    if (avCodecContext->codec_type == AVMEDIA_TYPE_VIDEO) {
        //视频帧大小 = 一行R * 一行G * 一行B *视频的总像素高度。
        baseData.size =
                (frame->linesize[0] + frame->linesize[1] + frame->linesize[2]) * frame->height;
        baseData.width = frame->width;
        baseData.height = frame->height;
    } else {
        //音频帧大小 = 样本字节数 * 单通道样本数 * 通道数。
        baseData.size =
                av_get_bytes_per_sample((AVSampleFormat) frame->format) * frame->nb_samples * 2;
    }

    //把当前解码后的帧格式保存下，如视频：YUV420,NV12,NV21。后面shader初始化及draw需要用。
    baseData.format = frame->format;
    //if(!isAudio)
    //    XLOGE("FFDecode::RecvFrame: video data format is %d",frame->format);

    memcpy(baseData.datas, frame->data, sizeof(baseData.datas));

    //这里把当前帧的pts拿出来，存到data中。
    baseData.pts = frame->pts;
    pts = baseData.pts;
    mux.unlock();

    return baseData;
}
