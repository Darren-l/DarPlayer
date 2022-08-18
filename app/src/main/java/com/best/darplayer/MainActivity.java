package com.best.darplayer;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceView;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.SeekBar;

import com.blankj.utilcode.constant.PermissionConstants;
import com.blankj.utilcode.util.PermissionUtils;

import java.io.File;


/**
 *
 * 测试播放器播放主页面。
 *
 *
 *
 */

public class MainActivity extends AppCompatActivity implements SeekBar.OnSeekBarChangeListener {

    static {
        System.loadLibrary("darplayer-sdk");
    }

    //测试打开视频接口
    public native void open(String url);
    //测试seek播放指定进度接口。
    public native double PlayPos();
    //测试暂停播放接口
    public native void pause();
    //测试恢复播放接口。
    public native void restart();
    //跳转到指定位置。
    public native void Seek(double pos);

    SurfaceView surfaceView;
    SeekBar seekBar;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //去掉标题栏
        supportRequestWindowFeature( Window.FEATURE_NO_TITLE);
        //全屏，隐藏状态
        getWindow().setFlags( WindowManager.LayoutParams.FLAG_FULLSCREEN ,
                WindowManager.LayoutParams.FLAG_FULLSCREEN
        );
        //屏幕为横屏
//        setRequestedOrientation( ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE );

        setContentView(R.layout.activity_main);

        //申请权限。
        requestPermission ();


        final File file1 = new File("/storage/input3.mp4");
        final File file2 = new File(Environment.getExternalStorageDirectory(), "input0.mp4");

        String url = "http://jsy.zb.yd.atianqi.com/app_4/ydgdws.m3u8?bitrate=2000";

        String url2 = " http://vfx.mtime.cn/Video/2019/02/04/mp4/190204084208765161.mp4";

        Button bt = findViewById(R.id.open_bt);
        Button bt2 = findViewById(R.id.open_bt2);

        Button pause_bt = findViewById(R.id.pause_bt);
        Button restart_bt = findViewById(R.id.restart_bt);

        seekBar = findViewById(R.id.seek_bar);
        seekBar.setMax(1000);

        seekBar.setOnSeekBarChangeListener( this );

        bt.setOnClickListener(getOnclickLis(url));
        bt2.setOnClickListener(getOnclickLis(file1.getAbsolutePath()));
        pause_bt.setOnClickListener(onPauseClickListener);
        restart_bt.setOnClickListener(restartClickListener);


//        test(file.getAbsolutePath());
//        test("http://jsy.zb.yd.atianqi.com/app_4/ydgdws.m3u8?bitrate=2000");

//        findViewById(R.id.open_bt).setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                ((DarPlayerView)findViewById(R.id.dar_player_view)).openPlayer();
//            }
//        });


    }


    View.OnClickListener onPauseClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            pause();
        }
    };


    View.OnClickListener restartClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            restart();
        }
    };

    public View.OnClickListener getOnclickLis(final String url){

        return new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                open(url);

                increateSeekBar(seekBar).start();
            }
        };
    }

    //进度条获取进度测试线程。
    public Thread increateSeekBar(final SeekBar seekBar){


         return new Thread(new Runnable() {
             @Override
             public void run() {
                 while (true) {
                     seekBar.setProgress((int) (PlayPos() * 1000));
                     try {
                         Thread.sleep(40);
                     } catch (InterruptedException e) {
                         e.printStackTrace();
                     }
                 }
             }
         });

    }



    private  int retryTime = 5;
    @SuppressLint("WrongConstant")
    private void requestPermission (){
        PermissionUtils.permission(PermissionConstants.STORAGE,PermissionConstants.PHONE).callback(new PermissionUtils.SimpleCallback() {
            @Override
            public void onGranted() {
            }
            @Override
            public void onDenied() {
                if(retryTime>=5){
                }else{
                    requestPermission();
                }
            }
        }).rationale(new PermissionUtils.OnRationaleListener() {
            @Override
            public void rationale(ShouldRequest shouldRequest) {
                shouldRequest.again(true);
            }
        }).request();
    }


    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {
        Seek( (double)seekBar.getProgress()/(double)seekBar.getMax() );

    }
}
