//
// Created by Darren on 2020/2/11.
//

#include "Thread.h"
#include "Log.h"
#include <thread>

using namespace std;


/**
 *
 * DarrenNote：这里封装一个线程sleep方法，单位毫秒。
 *
 * start()和stop()：
 * 控制线程的开启和停止，使用isRuning和isExit两个变量来控制，isExit为线程终止信号，isRunning为线程
 * 执行结束标志位。start()和stop()方法并非真正去结束线程。
 *
 * SetPause():
 * 设置线程的暂停和恢复，isPause代表当前线程暂停信号，isPausing为线程暂停标志位。同样该方法并非真正
 * 去暂停线程。
 *
 *
 * @param mis
 */
void Sleep(int mis) {
    chrono::milliseconds du(mis);
    this_thread::sleep_for(du);
}

bool Thread::start() {
    isExit = false;
    isPause = false;
    thread th(&Thread::ThreadMain, this);
    th.detach();
    return true;
}

void Thread::stop() {
    LOGI("Stop 停止线程begin!");
    isExit = true;
    for (int i = 0; i < 200; i++) {
        if (!isRuning) {
            LOGI("Stop 停止线程成功!");
            return;
        }
        Sleep(1);
    }
    LOGI("Stop 停止线程超时!");

}


void Thread::SetPause(bool isP) {
    isPause = isP;
    //等待100毫秒
    for (int i = 0; i < 10; i++) {
        if (isPausing == isP) {
            break;
        }
        Sleep(10);
    }

}

void Thread::ThreadMain() {

    isRuning = true;
    LOGI("线程函数进入");
    Main();
    LOGI("线程函数退出");
    isRuning = false;

}
