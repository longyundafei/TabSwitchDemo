/*
 * Copyright (c) 2016 Sichuan Miaoqu Technology Co., Ltd.
 */

package com.bihu.carcontrol.common.util;

import android.app.Activity;
import android.content.ClipboardManager;
import android.content.Context;
import android.graphics.Rect;
import android.os.Environment;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.TextView;


import com.bihu.carcontrol.R;

import java.io.File;
import java.io.FileOutputStream;
import java.util.Timer;
import java.util.TimerTask;

/**
 * 屏幕工具类
 */
public class ScreenUtil {
    private ScreenUtil() {
        throw new AssertionError();
    }

    /**
     * 获取状态栏高
     *
     * @param activity 活动窗口
     * @return 状态栏高
     */
    public static int getStatusBarHeight(Activity activity) {
        Rect frame = new Rect();
        activity.getWindow().getDecorView().getWindowVisibleDisplayFrame(frame);
        return frame.top;
    }

    /**
     * 获取导航栏高
     *
     * @param context 应用程序上下文
     * @return 导航栏高
     */
//    public static int getNavigationBarHeight(Context context) {
//        return context.getResources().getDimensionPixelOffset(R.dimen.dp_44);
//    }

    /**
     * 获取顶部栏高
     *
     * @param activity 活动窗口
     * @return 顶部栏高
     */
//    public static int getTopBarHeight(Activity activity) {
//        return getStatusBarHeight(activity) + getNavigationBarHeight(activity);
//    }

    /**
     * 获根视图高
     *
     * @param activity 活动窗口
     * @return 根视图高
     */
    public static int getBaseViewHeight(Activity activity) {
        Rect frame = new Rect();
        activity.getWindow().getDecorView().getWindowVisibleDisplayFrame(frame);
        return frame.height();
    }

    /**
     * 获取中心视图高
     *
     * @param activity 活动窗口
     * @return 中心视图高
     */
//    public static int getCenterViewHeight(Activity activity) {
//        return getBaseViewHeight(activity) - getNavigationBarHeight(activity);
//    }

    /**
     * 获取屏幕宽
     *
     * @param context 应用程序上下文
     * @return 屏幕宽
     */
    public static int getScreenWidth(Context context) {
        WindowManager manager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        return manager.getDefaultDisplay().getWidth();
    }

    /**
     * 获取屏幕高
     *
     * @param context 应用程序上下文
     * @return 屏幕高
     */
    public static int getScreenHeight(Context context) {
        WindowManager manager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        return manager.getDefaultDisplay().getHeight();
    }

    /**
     * px转dp
     *
     * @param context 应用程序上下文
     * @param pxValue px
     * @return dp
     */
    public static int px2dp(Context context, float pxValue) {
        float scale = context.getResources().getDisplayMetrics().density;
        return (int) (pxValue / scale + 0.5f);
    }

    /**
     * dp转px
     *
     * @param context 应用程序上下文
     * @param dpValue dp
     * @return px
     */
    public static int dp2px(Context context, float dpValue) {
        float scale = context.getResources().getDisplayMetrics().density;
        return (int) (dpValue * scale + 0.5f);
    }

    /**
     * px转sp
     *
     * @param context 应用程序上下文
     * @param pxValue px
     * @return sp
     */
    public static int px2sp(Context context, float pxValue) {
        float fontScale = context.getResources().getDisplayMetrics().scaledDensity;
        return (int) (pxValue / fontScale + 0.5f);
    }

    public static int getScreenW(Context c) {
        return c.getResources().getDisplayMetrics().widthPixels;
    }

    public static int getScreenH(Context c) {
        return c.getResources().getDisplayMetrics().heightPixels;
    }

    public static int getScreenR(Context c) {
        return (int) Math.sqrt(Math.pow(getScreenH(c), 2) + Math.pow(getScreenW(c), 2)) / 2;
    }

    /**
     * sp转px
     *
     * @param context 应用程序上下文
     * @param spValue sp
     * @return px
     */
    public static int sp2px(Context context, float spValue) {
        float fontScale = context.getResources().getDisplayMetrics().scaledDensity;
        return (int) (spValue * fontScale + 0.5f);
    }

    public static void getAndroidScreenProperty(Context context) {
        WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        DisplayMetrics dm = new DisplayMetrics();
        wm.getDefaultDisplay().getMetrics(dm);
        int width = dm.widthPixels;         // 屏幕宽度（像素）
        int height = dm.heightPixels;       // 屏幕高度（像素）
        float density = dm.density;         // 屏幕密度（0.75 / 1.0 / 1.5）
        int densityDpi = dm.densityDpi;     // 屏幕密度dpi（120 / 160 / 240）
        // 屏幕宽度算法:屏幕宽度（像素）/屏幕密度
        int screenWidth = (int) (width / density);  // 屏幕宽度(dp)
        int screenHeight = (int) (height / density);// 屏幕高度(dp)


        Log.d("h_bl", "屏幕宽度（像素）：" + width);
        Log.d("h_bl", "屏幕高度（像素）：" + height);
        Log.d("h_bl", "屏幕密度（0.75 / 1.0 / 1.5）：" + density);
        Log.d("h_bl", "屏幕密度dpi（120 / 160 / 240）：" + densityDpi);
        Log.d("h_bl", "屏幕宽度（dp）：" + screenWidth);
        Log.d("h_bl", "屏幕高度（dp）：" + screenHeight);
    }

    public static void getDp() {
        int width = 1080;//屏幕宽度
        int height = 1812;//屏幕高度
        float screenInch = 5.2f;//屏幕尺寸
//设备密度公式
        float density = (float) Math.sqrt(width * width + height * height) / screenInch / 160;

        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<resources>\n");
        for (int px = 0; px <= 1000; px += 2) {
            //像素值除以density
            String dp = px * 1.0f / density + "";
            //拼接成资源文件的内容，方便引用
            if (dp.indexOf(".") + 4 < dp.length()) {//保留3位小数
                dp = dp.substring(0, dp.indexOf(".") + 4);
            }
            stringBuilder.append("<dimen name=\"px").append(px + "").append("dp\">").append(dp).append("dp</dimen>\n");
        }
        stringBuilder.append("</resources>");
//        saveFile(stringBuilder.toString());
        Log.d("TAG", stringBuilder.toString());
    }

    public static void saveFile(String str) {
        String filePath = null;
        boolean hasSDCard = Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED);
        if (hasSDCard) { // SD卡根目录的px2dp.text
            filePath = Environment.getExternalStorageDirectory().toString() + File.separator + "px2dp.txt";
        } else  // 系统下载缓存根目录的px2dp.text
            filePath = Environment.getDownloadCacheDirectory().toString() + File.separator + "px2dp.txt";

        try {
            File file = new File(filePath);
            if (!file.exists()) {
                File dir = new File(file.getParent());
                dir.mkdirs();
                file.createNewFile();
            }
            FileOutputStream outStream = new FileOutputStream(file);
            outStream.write(str.getBytes());
            outStream.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 显示软键盘
     *
     * @param context 应用程序上下文
     * @param edit    输入框
     */
    public static void showSoftInput(final Context context, final EditText edit) {
        edit.setFocusable(true);
        edit.setFocusableInTouchMode(true);
        edit.requestFocus();
        edit.setSelection(edit.getText().length());
        new Timer().schedule(new TimerTask() {
            @Override
            public void run() {
                InputMethodManager inputManager = (InputMethodManager) context.getSystemService(Context.INPUT_METHOD_SERVICE);
                inputManager.showSoftInput(edit, 0);
            }
        }, 200);
    }

    /**
     * 隐藏软键盘
     *
     * @param activity 活动窗口
     */
    public static void hideSoftInput(Activity activity) {
        View view = activity.getWindow().peekDecorView();
        if (view != null) {
            InputMethodManager inputManager = (InputMethodManager) activity.getSystemService
                    (Context.INPUT_METHOD_SERVICE);
            if (inputManager.isActive() && view.getWindowToken() != null) {
                inputManager.hideSoftInputFromWindow(view.getWindowToken(), 0);
            }
        }
    }

    /**
     * 隐藏软键盘
     *
     * @param context 应用程序上下文
     * @param edit    输入框
     */
    public static void hideSoftInput(Context context, EditText edit) {
        edit.clearFocus();
        InputMethodManager inputManager = (InputMethodManager) context.getSystemService(Context
                .INPUT_METHOD_SERVICE);
        if (inputManager.isActive() && edit.getWindowToken() != null) {
            inputManager.hideSoftInputFromWindow(edit.getWindowToken(), 0);
        }
    }

    /**
     * 切换软键盘
     *
     * @param context 应用程序上下文
     * @param edit    输入框
     */
    public static void toggleSoftInput(Context context, EditText edit) {
        edit.setFocusable(true);
        edit.setFocusableInTouchMode(true);
        edit.requestFocus();
        InputMethodManager inputManager = (InputMethodManager) context.getSystemService(Context
                .INPUT_METHOD_SERVICE);
        inputManager.toggleSoftInput(InputMethodManager.SHOW_FORCED, 0);
    }

    /**
     * 复制文字到剪贴板
     *
     * @param context   应用程序上下文
     * @param tvContent 文字框
     */
    public static void copyText(Context context, TextView tvContent) {
        ClipboardManager clipboardManager = (ClipboardManager) context.getSystemService(Context.CLIPBOARD_SERVICE);
        clipboardManager.setText(tvContent.getText().toString().trim());
    }

    /**
     * 复制文字到剪贴板
     *
     * @param context 应用程序上下文
     * @param content 文字内容
     */
    public static void copyText(Context context, String content) {
        ClipboardManager clipboardManager = (ClipboardManager) context.getSystemService(Context.CLIPBOARD_SERVICE);
        clipboardManager.setText(content.trim());
    }
}
