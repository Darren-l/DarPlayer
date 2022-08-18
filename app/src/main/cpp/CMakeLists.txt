
###DarrenAdd:加载ffmpeg c库。

cmake_minimum_required(VERSION 3.4.1)

#添加头文件路径（相对于本文件路径）
include_directories(include)

#设置ffmpeg库所在路径的变量
set(FFSO ${CMAKE_CURRENT_SOURCE_DIR}/../libs/${ANDROID_ABI})

#把ffmpeg相关动态库加进来。
add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${FFSO}/libavcodec.so)

add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${FFSO}/libavformat.so)

add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${FFSO}/libavutil.so)

add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${FFSO}/libswscale.so)

add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${FFSO}/libswresample.so)


add_library( # Sets the name of the library.
        darplayer-sdk

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        darplayer-sdk.cpp
        baseInterface/BaseDemux.cpp
        FFDemux.cpp
        tools/Log.cpp
        beans/BaseData.cpp
        tools/Thread.cpp
        DObserver.cpp
        beans/DParameter.cpp
        baseInterface/BaseDecode.cpp
        FFDecode.cpp
        moduleVideo/XEGL.cpp
        moduleVideo/XShader.cpp
        baseInterface/BaseVideoView.cpp
        XTexture.cpp
        moduleVideo/GLVideoView.cpp
        baseInterface/BaseResample.cpp
        moduleAudio/FFResample.cpp
        baseInterface/BaseAudioPlay.cpp
        moduleAudio/SLAudioPlay.cpp
        IPlayer.cpp
        baseInterface/BasePlayerBuilder.cpp
        FFPlayerBuilder.cpp
        IPlayerPorxy.cpp
        moduleVideo/FFVideoView.cpp

        )


find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )


target_link_libraries( # Specifies the target library.
        darplayer-sdk
        GLESv2 EGL
        OpenSLES
        android
        avcodec avformat avutil swscale swresample
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib} )