package com.bihu.carcontrol.common.util;

import android.app.Activity;
import android.app.Dialog;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.Gravity;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import com.bihu.carcontrol.R;

/**
 * FileName: DialogUtil
 * Author: WangXu
 * Date: 2023/6/15 10:33
 * Description: 弹窗
 */
public class DialogUtil {
    public static Dialog getDialog(Activity activity, View view, double customizedWidthSize) {
        Dialog dialog = new Dialog(activity, R.style.ActionSheetDialogStyle);
        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        dialog.setTitle((CharSequence) null);
        dialog.setCancelable(true);
        dialog.setContentView(view);
        dialog.getWindow().setBackgroundDrawableResource(R.drawable.bg_dialog);
        Window window = dialog.getWindow();
        window.setGravity(Gravity.CENTER);

        WindowManager.LayoutParams layoutParams = window.getAttributes();
        if (customizedWidthSize != 0) {
            WindowManager windowManager = window.getWindowManager();
            Display display = windowManager.getDefaultDisplay();
            DisplayMetrics displayMetrics = new DisplayMetrics();
            display.getMetrics(displayMetrics);
            layoutParams.width = (int) (displayMetrics.widthPixels * customizedWidthSize);
        } else {
            layoutParams.width = WindowManager.LayoutParams.WRAP_CONTENT;
            layoutParams.height = WindowManager.LayoutParams.WRAP_CONTENT;
        }
        window.setAttributes(layoutParams);
        dialog.show();
        return dialog;
    }
}
