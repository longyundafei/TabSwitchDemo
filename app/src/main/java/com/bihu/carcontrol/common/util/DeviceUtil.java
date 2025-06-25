package com.bihu.carcontrol.common.util;

import android.app.KeyguardManager;
import android.content.Context;
import android.content.pm.PackageManager;
import android.net.ConnectivityManager;
import android.net.NetworkCapabilities;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Environment;
import android.telephony.TelephonyManager;

import com.android.car.client.manager.vehicle.CarVehicleManager;
import com.android.car.core.server.vehicle.VehicleType;
import com.bihu.carcontrol.R;
import com.bihu.carcontrol.module.MyApplication;

public class DeviceUtil {
    private DeviceUtil() {
        throw new AssertionError();
    }

    /**
     * 判断当前设备是否有SD卡
     *
     * @return true表示有，false则反之
     */
    public static boolean haveSDCard() {
        return Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED);
    }

    /**
     * 判断当前设备是否处于锁屏（睡眠）状态
     *
     * @param context 应用程序上下文
     * @return true表示是，false则反之
     */
    public static boolean isSleep(Context context) {
        KeyguardManager kgMgr = (KeyguardManager) context.getSystemService(Context
                .KEYGUARD_SERVICE);
        return kgMgr.inKeyguardRestrictedInputMode();
    }

    /**
     * 判断当前设备是否为手机
     *
     * @param context 应用程序上下文
     * @return true表示是，false则反之
     */
    public static boolean isPhone(Context context) {
        TelephonyManager telephony = (TelephonyManager) context.getSystemService(Context
                .TELEPHONY_SERVICE);
        return telephony.getPhoneType() != TelephonyManager.PHONE_TYPE_NONE;
    }

    /**
     * 获取移动终端类型
     *
     * @param context 应用程序上下文
     * @return 移动终端类型
     */
    public static int getPhoneType(Context context) {
        TelephonyManager telephonyManager = (TelephonyManager) context.getSystemService(Context
                .TELEPHONY_SERVICE);
        return telephonyManager.getPhoneType();
    }

    /**
     * 判断是否有可用网络
     *
     * @param context 应用程序上下文
     * @return true表示有，false则反之
     */
    public static boolean isNetworkReachable(Context context) {
        ConnectivityManager cm = (ConnectivityManager) context.getSystemService(Context
                .CONNECTIVITY_SERVICE);
        NetworkInfo current = cm.getActiveNetworkInfo();
        return current != null && current.isAvailable();
    }

    /**
     * 是否连接WIFI
     */
    public static boolean isWifiConnected(Context context) {
        ConnectivityManager connectivityManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo wifiNetworkInfo = connectivityManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
        if (wifiNetworkInfo.isConnected()) {
            return true;
        }
        return false;
    }

    /**
     * 判断当前网络是否可用(6.0以上版本)
     * 实时（可判断当前网络是否可用）
     *
     * @param context
     * @return
     */
    public static boolean isNetSystemUsable(Context context) {
        boolean isNetUsable = false;
        ConnectivityManager manager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            NetworkCapabilities networkCapabilities = manager.getNetworkCapabilities(manager.getActiveNetwork());
            if (networkCapabilities != null) {
                isNetUsable = networkCapabilities.hasCapability(NetworkCapabilities.NET_CAPABILITY_VALIDATED);
            }
        }
        return isNetUsable;
    }

    /**
     * 判断网络是否可用
     * （wifi连接，但是无网络不可判断）
     *
     * @return
     */
    public static boolean isNetworkAvailable() {
        ConnectivityManager cm = (ConnectivityManager) MyApplication.getApplication().getSystemService(Context.CONNECTIVITY_SERVICE);
        if (cm == null)
            return false;
        try {
            NetworkInfo[] info = cm.getAllNetworkInfo();
            if (info != null) {
                for (int i = 0; i < info.length; i++) {
                    if (info[i].getState() == NetworkInfo.State.CONNECTED)
                        return true;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    /**
     * 获取Wifi的SSID
     */
    public static String getWifiName(Context context) {
        WifiManager wifiManager = (WifiManager) context.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        if (Build.VERSION.SDK_INT == Build.VERSION_CODES.O_MR1) {
            //android 8.1,SDK 27
            ConnectivityManager connManager = (ConnectivityManager) context.getApplicationContext().getSystemService(Context.CONNECTIVITY_SERVICE);
            assert connManager != null;
            NetworkInfo networkInfo = connManager.getActiveNetworkInfo();
            if (networkInfo.isConnected()) {
                if (networkInfo.getExtraInfo() != null) {
                    return networkInfo.getExtraInfo().replace("\"", "");
                }
            }
        }
        assert wifiManager != null;
        WifiInfo info = wifiManager.getConnectionInfo();
        String ssid = info.getSSID().replace("\"", "");
        if (ssid.equals("<unknown ssid>")) {//未找到ssid
            ssid = "";
        }
        return ssid;
    }

    /**
     * 判断是否有闪光灯功能
     *
     * @param context
     * @return
     */
    public static boolean hasFlash(Context context) {
        return context.getPackageManager().hasSystemFeature(PackageManager.FEATURE_CAMERA_FLASH);
    }

    /**
     * 获取手机系统版本号
     */
    public static String getSysVersion() {
        return Build.VERSION.RELEASE;
    }

    /**
     * 获取手机型号
     */
    public static String getPhoneType() {
        return Build.MODEL;
    }

    /**
     * 车辆上电 0 OFF 1ACC 2ON 3Start
     */
    public static boolean ifPowerOff() {
        int powerStatus = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.BCM_POWER_STATE);
        if (powerStatus == 0) {
            LogUtil.d("电源状态： OFF  无法操作车控");
            ToastMaster.shortToast(MyApplication.getApplication().getString(R.string.vehicle_power_off_cannot_operating));
        }
        if (powerStatus == Constants.ERROR_CODE) {
            LogUtil.d("电源状态：已丢失  无法操作车控");
        }
        LogUtil.d("获取当前电源状态： " + powerStatus);
        return powerStatus == 0 || powerStatus == Constants.ERROR_CODE;
    }
}
