//
// Created by Darren on 2020/2/13.
//

#include "XEGL.h"
#include "../tools/Log.h"
#include <android/native_window.h>
#include <EGL/egl.h>
#include <mutex>

/**
 *
 * 代码比较简单，直接定义一个内部类来实现，后期视情况分离。
 *
 */

class CXEGL : public XEGL {
public:
    //初始化EGL
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;
    std::mutex mux;

    virtual bool Init(void *win) {
        ANativeWindow *nwin = (ANativeWindow *) win;

        Close();

        //1 获取EGLDisplay对象 显示设备
        mux.lock();
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (display == EGL_NO_DISPLAY) {
            mux.unlock();
            LOGE("CXEGL::Init:call eglGetDisplay failed!");
            return false;
        }
        LOGI("CXEGL::Init:eglGetDisplay success!");


        //2 初始化Display
        if (EGL_TRUE != eglInitialize(display, 0, 0)) {
            mux.unlock();
            LOGE("CXEGL::Init:call eglInitialize failed!");
            return false;
        }
        LOGI("CXEGL::Init:eglInitialize success!");


        //3 获取配置并创建surface
        EGLint configSpec[] = {
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_NONE
        };
        EGLConfig config = 0;
        EGLint numConfigs = 0;
        if (EGL_TRUE != eglChooseConfig(display, configSpec, &config, 1, &numConfigs)) {
            mux.unlock();
            LOGE("CXEGL::Init:call eglChooseConfig failed!");
            return false;
        }
        LOGI("CXEGL::Init:eglChooseConfig success!");
        surface = eglCreateWindowSurface(display, config, nwin, NULL);


        //4 创建并打开EGL上下文
        const EGLint ctxAttr[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr);
        if (context == EGL_NO_CONTEXT) {
            mux.unlock();
            LOGE("CXEGL::Init:call eglCreateContext failed!");
            return false;
        }
        LOGI("CXEGL::Init:eglCreateContext success!");

        if (EGL_TRUE != eglMakeCurrent(display, surface, surface, context)) {
            mux.unlock();
            LOGE("CXEGL::Init:call eglMakeCurrent failed!");
            return false;
        }
        mux.unlock();
        LOGE("CXEGL::Init:EGL success!");

        return true;
    }


    /**
     *
     * 关闭EGL，做数据清理。
     *
     *
     */
    virtual void Close()
    {
        mux.lock();
        if(display == EGL_NO_DISPLAY)
        {
            mux.unlock();
            return;
        }
        eglMakeCurrent(display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);

        if(surface != EGL_NO_SURFACE)
            eglDestroySurface(display,surface);
        if(context != EGL_NO_CONTEXT)
            eglDestroyContext(display,context);

        eglTerminate(display);

        display = EGL_NO_DISPLAY;
        surface = EGL_NO_SURFACE;
        context = EGL_NO_CONTEXT;
        mux.unlock();
    }


    virtual void Draw() {
        mux.lock();
        if (display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE) {
            mux.unlock();
            return;
        }
//        LOGD("EGL Draw #########");
        eglSwapBuffers(display, surface);
        mux.unlock();
    }
};

/**
 *
 * 获取XEGL对象。
 *
 */
XEGL *XEGL::Get() {
    static CXEGL egl;
    return &egl;
}



