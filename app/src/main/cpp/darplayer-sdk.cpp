

/**
 *
 * DarrenAdd20200202: 播放器jni层封装类。
 *
 *  该类用于对java层暴露的执行播放器操作接口类：打开播放器，开始，暂停，停止，快进退。
 *
 * DarrenNote：
 *      1. android6.0以上系统，必须增加申请动态权限，否则将会av_formate_open_init()失败。
 *
 *      2. 支持两种渲染方式，openGL和FFmpeg YUV转RGB，可根据参数传递选择，前者能大幅减少CPU及电量的消耗
 *      ，后者对编码格式的兼容性更好。目前默认为使用openGL进行渲染。
 *
 *      3. 支持软硬解码，默认为硬解码，可根据参数选择传递。
 *
 *      4. 该项目中关于线程的开启和停止并非实际对线程进行操作，非安全。
 *
 *      5. open中有做800ms的延迟，规避多次且快速打开视频，清理动作未完成引起的内存异常问题，此部分后期
 *      可以封装到java层做，也可以做回调判断。注意：若上一次打开清理未完成就开启下一次打开，可能引起崩
 *      溃或渲染异常。
 *
 *      6. 如果使用openGL的方式显示，该播放器目前支持的编码格式有：yuv420P,NV12和NV21。如果使用FFmpeg
 *      方式进行渲染，则原则上可以兼容大部分格式。目前硬解码和FFmpeg转出来的大多数为yuv420P。
 *
 *      7. 本项目音视频同步策略为：视频随音频同步，通过音视频pts的对比，控制视频的解码缓冲队列实现音视
 *      频同步。
 *
 *      8. 本项目seek部分的寻帧策略为：向后寻找关键帧，且有做跳转实际帧的优化。
 *
 *
 *
 */

#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/native_window_jni.h>

#include "tools/Log.h"
#include "IPlayer.h"
#include "IPlayerPorxy.h"


extern "C"
JNIEXPORT void JNICALL
Java_com_best_darplayer_DarPlayerView_openPlayer(JNIEnv *env, jobject thiz, jstring url,jobject surface) {
    // TODO: implement openPlayer()

}


/**
 *
 * 传递硬解码所需vm环境，该方法在加载动态库时自动加载。
 *
 */

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm,void *res) {
    IPlayerPorxy::Get()->Init(vm);
    return JNI_VERSION_1_4;
}

/**
 *
 * 传递窗口句柄。
 *
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_best_darplayer_DarPlayerView_InitView(JNIEnv *env, jobject thiz, jobject surface) {
    // TODO: implement InitView()
    //Test XEGL初始化。
    ANativeWindow *win = ANativeWindow_fromSurface(env,surface);
    IPlayerPorxy::Get()->InitView(win);

}


/**
 *
 * 打开视频模块。
 *
 *
 */

static IPlayer *player = NULL;
extern "C"
JNIEXPORT void JNICALL
Java_com_best_darplayer_MainActivity_open(JNIEnv *env, jobject thiz, jstring url) {
    // TODO: implement test()
    LOGI("Java_com_best_darplayer_MainActivity_test...");
    //http://jsy.zb.yd.atianqi.com/app_4/ydgdws.m3u8?bitrate=2000
    const char *murl = env->GetStringUTFChars(url, 0);

    IPlayerPorxy::Get()->Open(murl);
    IPlayerPorxy::Get()->Start();
//    IPlayerPorxy::Get()->Seek(0.5);
}




/**
 *
 * 设置当前进度seek模块。
 *
 *
 */
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_best_darplayer_MainActivity_PlayPos(JNIEnv *env, jobject thiz) {
    // TODO: implement PlayPos()
    return IPlayerPorxy::Get()->PlayPos();
}



/**
 *
 * 恢复播放测试模块。
 *
 *
 */

extern "C"
JNIEXPORT void JNICALL
Java_com_best_darplayer_MainActivity_restart(JNIEnv *env, jobject thiz) {
    // TODO: implement restart()
    IPlayerPorxy::Get()->SetPause(false);

}


/**
 *
 * 暂停播放模块。
 *
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_best_darplayer_MainActivity_pause(JNIEnv *env, jobject thiz) {
    // TODO: implement pause()
    IPlayerPorxy::Get()->SetPause(true);
}

/**
 *
 * 暂停/播放。
 *
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_best_darplayer_DarPlayerView_PlayOrPause(JNIEnv *env, jobject thiz) {
    // TODO: implement PlayOrPause()
    IPlayerPorxy::Get()->SetPause(!IPlayerPorxy::Get()->IsPause());

}

/**
 *
 * 跳转指定位置。
 *
 *
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_best_darplayer_MainActivity_Seek(JNIEnv *env, jobject thiz, jdouble pos) {
    // TODO: implement Seek()

    IPlayerPorxy::Get()->Seek(pos);

}