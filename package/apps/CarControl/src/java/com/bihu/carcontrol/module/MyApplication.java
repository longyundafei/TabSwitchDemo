package com.bihu.carcontrol.module;

import com.android.car.client.MainSDKInitializer;
import com.android.car.client.SdkInitListener;
import com.bihu.carcontrol.common.greendao.DBFactory;
import com.bihu.carcontrol.common.util.LogUtil;
import java.lang.reflect.Field;

public class MyApplication extends CommonApp {
    private static final String LOG_TAG = "MyApplication";

    public static MyApplication getApplication() {
        return (MyApplication) instance;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        LogUtil.d("MyApplication: onCreate");
    LogUtil.d("MyApplication onCreate start");

    try {
        Field f = Class.forName("com.bihu.carcontrol.common.greendao.db.RadioBeanDao").getDeclaredField("TABLENAME");
        f.setAccessible(true);
        String tablename = (String) f.get(null);
        LogUtil.d("RadioBeanDao TABLENAME = " + tablename);
    } catch (Exception e) {
        LogUtil.e("Failed to get TABLENAME from RadioBeanDao", e);
    }

    LogUtil.d("MyApplication onCreate end");
        DBFactory.init(this);
        initMainSDK();
    }

    //初始化壁虎中间件SDK
    private void initMainSDK() {
        LogUtil.d("初始化壁虎中间件SDK");
        //初始化SDK
        MainSDKInitializer.getInstance().initMainSDK(MyApplication.getApplication(), new SdkInitListener() {
            @Override
            public void onSdkInitSuccess() {
                //初始化成功
                LogUtil.d("中间件SDK 初始化成功");
            }

            @Override
            public void onSdkDisconnected() {
                //初始化失败
                LogUtil.d("中间件SDK 初始化失败");
                initMainSDK();
            }
        });
    }
}
