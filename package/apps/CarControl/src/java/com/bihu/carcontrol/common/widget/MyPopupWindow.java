package com.bihu.carcontrol.common.widget;

import android.app.Activity;
import android.content.Context;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.PopupWindow;
import android.widget.TextView;

import com.bihu.carcontrol.R;
import com.bihu.carcontrol.common.util.LogUtil;

/**
 * FileName: MyPopupWindow
 * Author: WangXu
 * Date: 2024/11/7 10:16
 * Description:
 */
public class MyPopupWindow extends PopupWindow {
    private static MyPopupWindow myPopupWindow;

    public MyPopupWindow(Context context) {
        super(context);
    }

    public static MyPopupWindow getInstant(Context context) {
        if (myPopupWindow == null && context != null) {
            myPopupWindow = new MyPopupWindow(context);
        }
        return myPopupWindow;
    }

    public void showPopupWindowRight(Activity activity, View contentView, String content) {
        if (activity == null || contentView.getWindowToken() == null) {
            LogUtil.d("MyPopupWindow  activity == null:  " + (activity == null) + "   contentView.getWindowToken() == null：  " + (contentView.getWindowToken() == null));
            return;
        }
        View layoutInflater = LayoutInflater.from(activity).inflate(R.layout.popup_windows_view, null);
        myPopupWindow.setContentView(layoutInflater);
        TextView textView = layoutInflater.findViewById(R.id.tv_popup_content);
        textView.setText(content);
        myPopupWindow.setOutsideTouchable(true);
        myPopupWindow.setBackgroundDrawable(null);
        if (contentView.isShown()) {
            int[] location = new int[2];
            contentView.getLocationOnScreen(location);
            contentView.post(() -> {
                if (activity.isFinishing() || activity.isDestroyed()) {
                    LogUtil.d("activity.isFinishing() || activity.isDestroyed()");
                    return;
                }
                myPopupWindow.showAtLocation(activity.getWindow().getDecorView(), Gravity.NO_GRAVITY, (location[0] + contentView.getWidth() - 400), location[1] - 55);
                LogUtil.d("showAsDropDown  location[0] x: " + location[0] + "   contentView.getWidth(): " + contentView.getWidth() + "    y:" + location[1]);
            });
        } else {
            LogUtil.d("contentView.isShown() ====== false");
        }
    }

    public void dismissPopupWindow() {
        if (myPopupWindow != null && myPopupWindow.isShowing()) {
            myPopupWindow.dismiss();
        }
    }
}
