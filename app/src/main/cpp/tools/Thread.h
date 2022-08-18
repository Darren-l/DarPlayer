//
// Created by Darren on 2020/2/11.
//

/**
 *
 * DarrenAdd20200211:线程基类。
 *
 * 继承该类，需要重写Main()方法，调用start后，会调用Main()方法。
 *
 *
 */

#ifndef DARPLAYER_THREAD_H
#define DARPLAYER_THREAD_H

//C++：将方法放到类外面声明，则为全局的方法，导入该头文件可以直接引用。
void Sleep(int mis);

class Thread {
public:

    //启动线程，执行Main()接口。
    virtual bool start();

    //停止线程。（非安全停止）
    virtual void stop();

    //该main有做空实现，允许子类不重写该接口。
    //防止该继承者没有实现该接口，直接调用start引起的空指针问题。
    virtual void Main() {};


    virtual void SetPause(bool isP);

    /**
     *
     * 返回线程的暂停状态。
     *
     * @return
     */
    virtual bool IsPause() {
        isPausing = isPause;
        return isPause;
    }


private:
    void ThreadMain();


protected:
    //线程是否退出。
    bool isExit = false;
    //线程是否正在运行。
    bool isRuning = false;

    bool isPause = false;
    bool isPausing = false;

};


#endif //DARPLAYER_THREAD_H
