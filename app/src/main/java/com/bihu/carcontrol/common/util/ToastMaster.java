package com.bihu.carcontrol.common.util;

import android.content.Context;
import android.os.Looper;
import android.view.Gravity;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import androidx.fragment.app.Fragment;

import com.bihu.carcontrol.module.MyApplication;

public class ToastMaster {

    private static Toast mToast;

    private ToastMaster() {

    }

    public static void shortToast(Fragment fragment, String toastText) {
        if (fragment.isAdded() && !fragment.isDetached() && fragment.getActivity() != null) {
            toast(MyApplication.getApplication(), toastText, Toast.LENGTH_SHORT);
        } else {
            LogUtil.e("showToast but Fragmnet not attached to a context");
        }
    }

    public static void shortToast(String toastText) {
        toast(MyApplication.getApplication(), toastText, Toast.LENGTH_SHORT);
    }

    public static void shortToast(int textId) {
        toast(MyApplication.getApplication(), textId, Toast.LENGTH_SHORT);
    }

    public static void longToast(String toastText) {
        toast(MyApplication.getApplication(), toastText, Toast.LENGTH_LONG);
    }

    private static void toast(Context context, int text, int duration) {
        toast(context, context.getResources().getString(text), duration);
    }

    private static void toast(Context context, final String text, int duration) {
        if (Looper.getMainLooper().getThread() == Thread.currentThread()) {
            _toast(context, text, duration);
        } else {
            MyApplication.getApplication().post(new ToastRunnable(text));
        }
    }

    private static void _toast(Context context, String text, int duration) {
        if (mToast == null) {
            mToast = Toast.makeText(context, text, duration);
            mToast.setText(text);
            View contentView = mToast.getView();
            if (contentView != null) {
                View v = contentView.findViewById(android.R.id.message);
                if (v instanceof TextView) {
                    ((TextView) v).setGravity(Gravity.CENTER);
                }
            }
        } else if (text != null) {
            mToast.setText(text);
        }
        mToast.show();
    }

    private static class ToastRunnable implements Runnable {
        String text;

        ToastRunnable(String text) {
            this.text = text;
        }

        @Override
        public void run() {
            _toast(MyApplication.getApplication(), text, Toast.LENGTH_LONG);
        }
    }
}
