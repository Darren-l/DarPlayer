package com.best.darplayer;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Environment;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import java.io.File;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL;
import javax.microedition.khronos.opengles.GL10;
import javax.sql.RowSetReader;


/**
 *
 * DarrenAdd20200202:播放器View java层封装类。
 *
 * 继承GLSurfaceView: 方便以后使用openGl库。
 *
 *
 */
public class DarPlayerView extends GLSurfaceView implements SurfaceHolder.Callback,GLSurfaceView.Renderer, View.OnClickListener {

//    public native void openPlayer(String url, Surface surface);

    public native void InitView(Object surface);

    public DarPlayerView(Context context, AttributeSet attrs) {
        super(context, attrs);
//        getHolder().addCallback(this);
        init();
        setOnClickListener( this );
    }

    private  void init(){
        //DarrenNote:解决android8.0后，无法进入app问题。
        setRenderer(this);
    }


    @Override
    public void surfaceCreated(final SurfaceHolder surfaceHolder) {
        InitView(surfaceHolder.getSurface());


    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

    }


    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

    }

    @Override
    public void onDrawFrame(GL10 gl) {

    }

    @Override
    public void onClick(View v) {
        PlayOrPause();
    }

    public native void PlayOrPause();
}
