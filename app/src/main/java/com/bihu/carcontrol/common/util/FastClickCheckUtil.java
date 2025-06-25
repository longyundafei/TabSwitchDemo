package com.bihu.carcontrol.common.util;

import android.os.SystemClock;

import com.bihu.carcontrol.R;
import com.bihu.carcontrol.module.MyApplication;

/**
 * Create by WangXu
 * on 2023/6/15 11:04
 **/
public class FastClickCheckUtil {

    private static long lastClickTime;
    private static int deafDoubleClickTime = 500;

    public synchronized static boolean isFastClick(int doubleClickTime) {
        long time = SystemClock.elapsedRealtime();
        if (time - lastClickTime < doubleClickTime) {
            ToastMaster.shortToast(MyApplication.getApplication().getString(R.string.fast_click));
            return true;
        }
        lastClickTime = time;
        return false;
    }

    public synchronized static boolean isFastClick() {
        long time = SystemClock.elapsedRealtime();
        if (time - lastClickTime < deafDoubleClickTime) {
            return true;
        }
        lastClickTime = time;
        return false;
    }

    public synchronized static boolean isFastClickNoToast(int doubleClickTime) {
        long time = SystemClock.elapsedRealtime();
        if (time - lastClickTime < doubleClickTime) {
            return true;
        }
        lastClickTime = time;
        return false;
    }
}
