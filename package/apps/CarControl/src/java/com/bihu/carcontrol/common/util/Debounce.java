package com.bihu.carcontrol.common.util;

/**
 * FileName: Debounce
 * Author: WangXu
 * Date: 2025/3/3 18:23
 * Description:
 */
import android.os.Handler;
import android.os.Looper;
public class Debounce {
    private final Handler handler = new Handler(Looper.getMainLooper()); // 主线程的 Handler
    private Runnable task; // 用于保存当前的任务
    private final long delayMillis; // 延迟时间

    public Debounce(long delayMillis) {
        this.delayMillis = delayMillis;
    }

    /**
     * 处理回调  callback 回调逻辑
     */
    public void onEvent(Runnable callback) {
        // 如果之前有未执行的任务，移除它
        if (task != null) {
            handler.removeCallbacks(task);
        }

        // 创建一个新的任务
        task = () -> {
            callback.run(); // 执行回调
            task = null; // 清空任务
        };

        // 延迟执行任务
        handler.postDelayed(task, delayMillis);
    }

    /**
     * 取消未执行的任务
     */
    public void cancel() {
        if (task != null) {
            handler.removeCallbacks(task);
            task = null;
        }
    }
}
