#########################################
# 关键保留资源（避免按钮文字缺失）
#########################################

# 保留所有 R$ 字段，特别是 R.string.*, R.id.* 等资源
-keep class **.R$* {
    public static final int *;
}

# 禁用资源移除优化（防止 layout 资源关联的 string 被删）
-dontshrink
-dontoptimize
-dontobfuscate

#########################################
# GreenDAO 保留表字段
#########################################
-keepclassmembers class * extends org.greenrobot.greendao.AbstractDao {
    public static final java.lang.String TABLENAME;
}
-keep class com.bihu.carcontrol.common.greendao.** { *; }
-keep class org.greenrobot.greendao.** { *; }

#########################################
# EventBus
#########################################
-keepclassmembers class ** {
    @org.greenrobot.eventbus.Subscribe public *;
}
-keepclassmembers class * {
    public void onEvent*(**);
}
-keep class org.greenrobot.eventbus.** { *; }

#########################################
# 保留 Activity / Fragment / View 等
#########################################
-keep class * extends android.app.Activity { *; }
-keep class * extends android.app.Service { *; }
-keep class * extends android.content.BroadcastReceiver { *; }
-keep class * extends androidx.fragment.app.Fragment { *; }

-keep class com.bihu.carcontrol.module.MyApplication { *; }

# 保留布局中 onClick 方法
-keepclassmembers class * {
    public void *(android.view.View);
}

#########################################
# fastjson（如果你用了）
#########################################
-keep class com.alibaba.fastjson.** { *; }
-keepattributes Signature
-keepattributes *Annotation*

#########################################
# 枚举和注解
#########################################
-keepclassmembers enum * {
    public static **[] values();
    public static ** valueOf(java.lang.String);
}
-keep @interface * { *; }

# 构造器
-keepnames class * {
    public <init>(...);
}

