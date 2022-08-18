//
// Created by Darren on 2020/2/10.
//

/**
 *
 * DarrenAdd20200210:打印封装类。
 *
 */

#ifndef DARPLAYER_LOG_H
#define DARPLAYER_LOG_H

#include <android/log.h>
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,"##Jni:DarPlayer",__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"##Jni:DarPlayer",__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"##Jni:DarPlayer",__VA_ARGS__)


class Log {

};


#endif //DARPLAYER_LOG_H
