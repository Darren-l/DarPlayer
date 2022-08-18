//
// Created by Darren on 2020/2/15.
//

#include "FFResample.h"
#include "../tools/Log.h"
#include "FFResample.h"


extern "C"
{
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
}


/**
 *
 * 关闭重采样对象。
 *
 *
 */

void FFResample::Close()
{
    mux.lock();
    if(actx)
    {
        swr_free(&actx);
    }
    mux.unlock();
}

/**
 *
 * 打开重采样上下文，初始化重采样。
 *
 * 获取重采样上下文，获取声道数和采样格式。
 *
 * @param in
 * @param out
 * @return
 */
bool FFResample::Open(DParameter in,DParameter out){
    LOGI("FFResample::Open...");

    Close();
    mux.lock();

    //音频重采样上下文初始化
    actx = swr_alloc();
    if(!actx){
        LOGE("FFResample::Open:swr_alloc failed!");
    }
    actx = swr_alloc_set_opts(actx,
                              av_get_default_channel_layout(out.channels),
                              AV_SAMPLE_FMT_S16,out.sample_rate,
                              av_get_default_channel_layout(in.para->channels),
                              (AVSampleFormat)in.para->format,in.para->sample_rate,
                              0,0 );

    //初始化重采样上下文。
    int re = swr_init(actx);
    if(re != 0){
        LOGE("FFResample::Open:swr_init failed!");
        mux.unlock();
        return false;
    }
    LOGE("FFResample::Open:swr_init success!");


    //获取声道数和采样格式。
    outChannels = in.para->channels;
    outFormat = AV_SAMPLE_FMT_S16;
    mux.unlock();
    return true;

}


/**
 *
 * 对数据进行重采样，在调用该方法前，确保已经调用了open方法。
 *
 *
 * @param indata
 * @return
 */
BaseData FFResample::Resample(BaseData indata){
    if(indata.size<=0 || !indata.data){
        LOGE("FFResample::Resample: indata.size = 0 or indata.data is null!");
        return BaseData();
    }

    mux.lock();

    if(!actx){
        LOGE("FFResample::Resample: actx is null!");
        mux.unlock();
        return BaseData();
    }

//    LOGE("indata size is %d",indata.size);
    //强转回来，得到原始的fram帧数据对象。
    AVFrame *frame = (AVFrame *)indata.data;

    //输出空间的分配
    BaseData out;
    //计算该帧所需内存。
    int outsize = outChannels * frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat)outFormat);
    if(outsize <=0){
        LOGE("FFResample::Resample: get outsize error");
        mux.unlock();
        return BaseData();
    }

    //分配空间。
    out.Alloc(outsize);

    uint8_t *outArr[2] = {0};
    outArr[0] = out.data;
    //重采样，返回每通道的样本数。
    int len = swr_convert(actx,outArr,frame->nb_samples,(const uint8_t **)frame->data,frame->nb_samples);
    if(len<=0){
        LOGE("FFResample::Resample: call swr_convert error!!");
        mux.unlock();
        out.Drop();
        return BaseData();
    }

    //这里传下pts的值给audioplay对象。
    out.pts = indata.pts;
    mux.unlock();
//    LOGE("swr_convert success = %d",len);
    return out;
}