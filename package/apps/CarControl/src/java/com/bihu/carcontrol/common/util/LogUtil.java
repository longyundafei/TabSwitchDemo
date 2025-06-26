package com.bihu.carcontrol.common.util;

import android.util.Log;

public class LogUtil {
    public static void log(String tagName, String msg) {
        int strLength = msg.length();
        int start = 0;
        int end = 2000;
        for (int i = 0; i < 99; i++) {
            if (strLength > end) {
                Log.i(tagName + i, msg.substring(start, end));
                start = end;
                end += 2000;
            } else {
                Log.i(tagName + i, msg.substring(start, strLength));
                break;
            }
        }
    }

    private static Boolean LOG_SWITCH = true; // 日志文件总开关
    private static String LOG_TAG = "xxx  "; // 默认的tag
    private static char LOG_TYPE = 'v';// 输入日志类型，v代表输出所有信息,w则只输出警告...

    /**************************** Warn *********************************/
    public static void w(Object msg) {
        w(LOG_TAG, msg);
    }

    public static void w(String tag, Object msg) {
        w(tag, msg, null);
    }

    public static void w(String tag, Object msg, Throwable tr) {
        log(tag, msg.toString(), tr, 'w');
    }

    /*************************** Error ********************************/
    public static void e(Object msg) {
        e(LOG_TAG, msg);
    }

    public static void e(String tag, Object msg) {
        e(tag, msg, null);
    }

    public static void e(String tag, Object msg, Throwable tr) {
        log(tag, msg.toString(), tr, 'e');
    }

    /*************************** Debug ********************************/
    public static void d(Object msg) {
        d(LOG_TAG, msg);
    }

    public static void d(String tag, Object msg) {// 调试信息
        d(tag, msg, null);
    }

    public static void d(String tag, Object msg, Throwable tr) {
        log(tag, msg.toString(), tr, 'd');
    }

    /**************************** Info *********************************/
    public static void i(Object msg) {
        i(LOG_TAG, msg);
    }

    public static void i(String tag, Object msg) {
        i(tag, msg, null);
    }

    public static void i(String tag, Object msg, Throwable tr) {
        log(tag, msg.toString(), tr, 'i');
    }

    /************************** Verbose ********************************/
    public static void v(Object msg) {
        v(LOG_TAG, msg);
    }

    public static void v(String tag, Object msg) {
        v(tag, msg, null);
    }

    public static void v(String tag, Object msg, Throwable tr) {
        log(tag, msg.toString(), tr, 'v');
    }

    /**
     * 根据tag, msg和等级，输出日志
     *
     * @param tag
     * @param msg
     * @param level
     */
    private static void log(String tag, String msg, Throwable tr, char level) {
//        if (LOG_SWITCH && BuildConfig.LOG_DEBUG) {
        if ('e' == level && ('e' == LOG_TYPE || 'v' == LOG_TYPE)) { // 输出错误信息
            Log.e(tag, msg, tr);
        } else if ('w' == level && ('w' == LOG_TYPE || 'v' == LOG_TYPE)) {
            Log.w(tag, msg, tr);
        } else if ('d' == level && ('d' == LOG_TYPE || 'v' == LOG_TYPE)) {
            Log.d(tag, msg, tr);
        } else if ('i' == level && ('d' == LOG_TYPE || 'v' == LOG_TYPE)) {
            Log.i(tag, msg, tr);
        } else {
            Log.v(tag, msg, tr);
        }
//        }
    }

}
