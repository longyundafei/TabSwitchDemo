package com.bihu.carcontrol.common.util;

import android.Manifest;
import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.location.LocationManager;
import android.net.Uri;
import android.provider.Settings;

import androidx.core.app.ActivityCompat;

import com.bihu.carcontrol.module.MyApplication;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.List;

public class AppUtil {
    private AppUtil() {
        throw new AssertionError();
    }

    /**
     * 获取应用程序
     *
     * @return 应用程序
     */
    public static MyApplication getApplication() {
        return MyApplication.getApplication();
    }

    /**
     * 获取应用程序上下文
     *
     * @return 应用程序上下文
     */
    public static Context getContext() {
        return MyApplication.getApplication();
    }

    /**
     * 获取进程名
     *
     * @return 进程名
     */
    public static String getProcessName() {
        String processName = null;
        File file = new File("/proc/" + android.os.Process.myPid() + "/" + "cmdline");
        FileReader fileReader = null;
        BufferedReader bufferedReader = null;
        try {
            fileReader = new FileReader(file);
            bufferedReader = new BufferedReader(fileReader);
            processName = bufferedReader.readLine().trim();
        } catch (Exception e) {
            e.printStackTrace();
            LogUtil.e("message");
        } finally {
            try {
                if (bufferedReader != null) {
                    bufferedReader.close();
                }
                if (fileReader != null) {
                    fileReader.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
                LogUtil.e("IOException");
            }
        }
        return processName;
    }

    /**
     * 获取当前App的包名
     *
     * @param context 应用程序上下文
     * @return 当前App的包名
     */
    public static String getAppPackageName(Context context) {
        return context.getPackageName();
    }

    /**
     * 获取当前App的版本名称
     *
     * @param context 应用程序上下文
     * @return 当前App的版本名称
     */
    public static String getAppVersionName(Context context) {
        String versionName = "";
        try {
            versionName = context.getPackageManager().getPackageInfo(context.getPackageName(), 0)
                    .versionName;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
            LogUtil.e("message");
        }
        return versionName;
    }

    //比对版本号，检测App是否有版本升级
    public static boolean checkVersionUpdate(Context context, String newVersion) {
        String oldVersion = getAppVersionName(context);
        String[] oldVerSplit = oldVersion.split("\\.");
        String[] newVerSplit = newVersion.split("\\.");
        for (int i = 0; i < 4; i++) {
            int oldVerNum = Integer.parseInt(i < oldVerSplit.length ? oldVerSplit[i] : "0");
            int newVerNum = Integer.parseInt(i < newVerSplit.length ? newVerSplit[i] : "0");
            if (newVerNum > oldVerNum) {
                return true;
            } else if (newVerNum < oldVerNum) {
                return false;
            }
        }
        return false;
    }

    public static boolean isAppOnForeground(Context context) {
        ActivityManager activityManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
        List<ActivityManager.RunningAppProcessInfo> appProcesses = null;
        if (activityManager != null) {
            appProcesses = activityManager.getRunningAppProcesses();
        }
        if (appProcesses != null) {
            final String packageName = context.getPackageName();
            for (ActivityManager.RunningAppProcessInfo appProcess : appProcesses) {
                if (appProcess.importance == ActivityManager.RunningAppProcessInfo.IMPORTANCE_FOREGROUND && appProcess.processName.equals(packageName)) {
                    return true;
                }
            }
        }
        return false;
    }
//
//    public static void callPhone(Context context, String phone) {
//        AndPermission.with(context).runtime().permission(Permission.CALL_PHONE).onGranted(data -> {
//            if (ActivityCompat.checkSelfPermission(context, Manifest.permission.CALL_PHONE) != PackageManager.PERMISSION_GRANTED) {
//                ToastMaster.shortToast("请允许拨打电话权限");
//                startAppSettings(context);
//            } else {
//                context.startActivity(new Intent(Intent.ACTION_CALL, Uri.parse("tel:" + phone)));
//            }
//        }).onDenied(data -> {
//            ToastMaster.shortToast("授权失败");
//        }).start();
//    }

    public static void startAppSettings(Context context) {
        Intent intent = new Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
        intent.setData(Uri.parse("package:" + context.getPackageName()));
        context.startActivity(intent);
    }

    public static boolean checkGPSIsOpen(Context context) {
        LocationManager locationManager = (LocationManager) context.getSystemService(Context.LOCATION_SERVICE);
        // 通过GPS卫星定位，定位级别可以精确到街（通过24颗卫星定位，在室外和空旷的地方定位准确、速度快）
        boolean gps = locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER);
//        // 通过WLAN或移动网络(3G/2G)确定的位置（也称作AGPS，辅助GPS定位。主要用于在室内或遮盖物（建筑群或茂密的深林等）密集的地方定位）
//        boolean network = locationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER);
        if (gps) {
            return true;
        }
        return false;
    }

    public static void goToGpsSetting(Activity activity) {
        //跳转GPS设置界面
        Intent intent = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);
        activity.startActivity(intent);
    }

}
