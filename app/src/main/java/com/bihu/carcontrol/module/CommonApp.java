package com.bihu.carcontrol.module;

import android.app.Application;
import android.app.NotificationManager;
import android.content.Context;
import android.os.Handler;
import android.os.Looper;

import com.bihu.carcontrol.common.util.ActivityStack;

public class CommonApp extends Application {

    protected Handler mainHandler;
    protected static CommonApp instance;
    private ActivityStack activityStack;

    public CommonApp() {
    }

    public static CommonApp getApplication() {
        return instance;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        instance = this;
        activityStack = new ActivityStack();
        mainHandler = new Handler(Looper.getMainLooper());
    }

    public ActivityStack getActivityStack() {
        return activityStack;
    }

    public void post(Runnable r) {
        if (r != null) {
            mainHandler.post(r);
        }
    }

    public void postDelayed(Runnable r, int delayMillis) {
        if (r != null) {
            mainHandler.postDelayed(r, delayMillis);
        }
    }

    public void removeCallbacks(Runnable r) {
        if (r != null) {
            mainHandler.removeCallbacks(r);
        }
    }

    public void clearAllNotification() {
        NotificationManager manager = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
        manager.cancelAll();
    }

}
