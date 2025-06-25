package com.bihu.carcontrol.common.util;

import android.content.Context;
import android.content.SharedPreferences;

public class SPUtil {
    public static final String SHARED_PREFERENCES_NAME = "preferences";// 偏好保存文件名
    public static final String KEY_MAINTAIN_MILEAGE ="key_maintain_mileage";//保养设置时的总里程
    public static final String KEY_MAINTAIN_TIME ="key_maintain_time";//保养设置时的时间
    public static final String KEY_RANGE_DISPLAY_MODE="key_range_display_mode";//续航里程模式
    public static final String KEY_COUNTRY_ID = "key_country_id";

    private SPUtil() {
        throw new AssertionError();
    }

    /**
     * 保存字符串
     *
     * @param context 应用程序上下文
     * @param key     键
     * @param value   值
     * @return true表示成功，false则反之
     */
    public static boolean putString(Context context, String key, String value) {
        SharedPreferences settings = context.getSharedPreferences(SHARED_PREFERENCES_NAME, Context
                .MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.putString(key, value);
        return editor.commit();
    }

    /**
     * 获取字符串
     *
     * @param context 应用程序上下文
     * @param key     键
     * @return 字符串
     */
    public static String getString(Context context, String key) {
        return getString(context, key, null);
    }

    /**
     * 获取字符串
     *
     * @param context      应用程序上下文
     * @param key          键
     * @param defaultValue 默认值
     * @return 字符串
     */
    public static String getString(Context context, String key, String defaultValue) {
        SharedPreferences settings = context.getSharedPreferences(SHARED_PREFERENCES_NAME, Context
                .MODE_PRIVATE);
        return settings.getString(key, defaultValue);
    }

    /**
     * 保存整型
     *
     * @param context 应用程序上下文
     * @param key     键
     * @param value   值
     * @return true表示成功，false则反之
     */
    public static boolean putInt(Context context, String key, int value) {
        SharedPreferences settings = context.getSharedPreferences(SHARED_PREFERENCES_NAME, Context
                .MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.putInt(key, value);
        return editor.commit();
    }

    /**
     * 获取整型
     *
     * @param context 应用程序上下文
     * @param key     键
     * @return 整型
     */
    public static int getInt(Context context, String key) {
        return getInt(context, key, -1);
    }

    /**
     * 获取整型
     *
     * @param context      应用程序上下文
     * @param key          键
     * @param defaultValue 默认值
     * @return 整型
     */
    public static int getInt(Context context, String key, int defaultValue) {
        SharedPreferences settings = context.getSharedPreferences(SHARED_PREFERENCES_NAME, Context
                .MODE_PRIVATE);
        return settings.getInt(key, defaultValue);
    }

    /**
     * 保存长整型
     *
     * @param context 应用程序上下文
     * @param key     键
     * @param value   值
     * @return true表示成功，false则反之
     */
    public static boolean putLong(Context context, String key, long value) {
        SharedPreferences settings = context.getSharedPreferences(SHARED_PREFERENCES_NAME, Context
                .MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.putLong(key, value);
        return editor.commit();
    }

    /**
     * 获取长整型
     *
     * @param context 应用程序上下文
     * @param key     键
     * @return 长整型
     */
    public static long getLong(Context context, String key) {
        return getLong(context, key, -1);
    }

    /**
     * 获取长整型
     *
     * @param context      应用程序上下文
     * @param key          键
     * @param defaultValue 默认值
     * @return 长整型
     */
    public static long getLong(Context context, String key, long defaultValue) {
        SharedPreferences settings = context.getSharedPreferences(SHARED_PREFERENCES_NAME, Context
                .MODE_PRIVATE);
        return settings.getLong(key, defaultValue);
    }

    /**
     * 保存浮点型
     *
     * @param context 应用程序上下文
     * @param key     键
     * @param value   值
     * @return true表示成功，false则反之
     */
    public static boolean putFloat(Context context, String key, float value) {
        SharedPreferences settings = context.getSharedPreferences(SHARED_PREFERENCES_NAME, Context
                .MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.putFloat(key, value);
        return editor.commit();
    }

    /**
     * 获取浮点型
     *
     * @param context 应用程序上下文
     * @param key     键
     * @return 浮点型
     */
    public static float getFloat(Context context, String key) {
        return getFloat(context, key, -1);
    }

    /**
     * 获取浮点型
     *
     * @param context      应用程序上下文
     * @param key          键
     * @param defaultValue 默认值
     * @return 浮点型
     */
    public static float getFloat(Context context, String key, float defaultValue) {
        SharedPreferences settings = context.getSharedPreferences(SHARED_PREFERENCES_NAME, Context
                .MODE_PRIVATE);
        return settings.getFloat(key, defaultValue);
    }

    /**
     * 保存布尔型
     *
     * @param context 应用程序上下文
     * @param key     键
     * @param value   默认值
     * @return true表示成功，false则反之
     */
    public static boolean putBoolean(Context context, String key, boolean value) {
        SharedPreferences settings = context.getSharedPreferences(SHARED_PREFERENCES_NAME, Context
                .MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.putBoolean(key, value);
        return editor.commit();
    }

    /**
     * 获取布尔型
     *
     * @param context 应用程序上下文
     * @param key     键
     * @return 布尔型
     */
    public static boolean getBoolean(Context context, String key) {
        return getBoolean(context, key, false);
    }

    /**
     * 获取布尔型
     *
     * @param context      应用程序上下文
     * @param key          键
     * @param defaultValue 默认值
     * @return 布尔型
     */
    public static boolean getBoolean(Context context, String key, boolean defaultValue) {
        SharedPreferences settings = context.getSharedPreferences(SHARED_PREFERENCES_NAME, Context
                .MODE_PRIVATE);
        return settings.getBoolean(key, defaultValue);
    }

    /**
     * 删除偏好保存
     *
     * @param context 应用程序上下文
     * @param key     键
     * @return true表示成功，false则反之
     */
    public static boolean remove(Context context, String key) {
        SharedPreferences settings = context.getSharedPreferences(SHARED_PREFERENCES_NAME, Context
                .MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.remove(key);
        return editor.commit();
    }

    /**
     * 清空偏好保存
     *
     * @param context 应用程序上下文
     * @return true表示成功，false则反之
     */
    public static boolean clear(Context context) {
        SharedPreferences settings = context.getSharedPreferences(SHARED_PREFERENCES_NAME, Context
                .MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.clear();
        return editor.commit();
    }
}
