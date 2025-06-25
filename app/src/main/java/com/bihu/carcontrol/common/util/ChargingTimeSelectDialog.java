package com.bihu.carcontrol.common.util;

import android.app.Dialog;
import android.content.Context;
import android.text.TextUtils;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.NumberPicker;
import android.widget.TextView;

import androidx.annotation.NonNull;

import com.bihu.carcontrol.R;

/**
 * FileName: ChargingTimeSelectDialog
 * Author: WangXu
 * Date: 2024/11/29 10:24
 * Description:
 */
public class ChargingTimeSelectDialog extends Dialog {
    private NumberPicker mpHourPicker;
    private NumberPicker mpMinPicker;

    public interface ChargingDialogClickLister {
        void onChargingTimeConfirmClick(int hourInt, int minInt);
    }

    public ChargingTimeSelectDialog(@NonNull Context context, String title, double customizedWidthSize, ChargingDialogClickLister chargingDialogClickLister) {
        super(context);
        initView(context, title, customizedWidthSize, chargingDialogClickLister);
    }

    private void initView(Context context, String title, double customizedWidthSize, ChargingDialogClickLister chargingDialogClickLister) {
        View view = LayoutInflater.from(context).inflate(R.layout.dialog_charging_time, (ViewGroup) null);
        setContentView(view);

        Window window = getWindow();
        assert window != null;
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
        }
        layoutParams.height = WindowManager.LayoutParams.WRAP_CONTENT;
        setCanceledOnTouchOutside(false);

        TextView tvTitle = view.findViewById(R.id.tv_title);
        if (!TextUtils.isEmpty(title)) {
            tvTitle.setText(title);
        }
        mpHourPicker = view.findViewById(R.id.hourPicker);
        mpMinPicker = view.findViewById(R.id.minPicker);
        String[] hourNumbers = new String[24];
        for (int i = 0; i < 24; i++) {
            if (i < 10) {
                hourNumbers[i] = "0" + i;
            } else {
                hourNumbers[i] = i + "";
            }
        }
        mpHourPicker.setDisplayedValues(hourNumbers);
        mpHourPicker.setMaxValue(hourNumbers.length - 1);
        mpHourPicker.setMinValue(0);
        mpHourPicker.setTextColor(context.getResources().getColor(R.color.title_text_color));
        mpHourPicker.setTextSize(18);

        String[] minNumbers = new String[60];
        for (int i = 0; i < 60; i++) {
            if (i < 10) {
                minNumbers[i] = "0" + i;
            } else {
                minNumbers[i] = i + "";
            }
        }
        mpMinPicker.setDisplayedValues(minNumbers);
        mpMinPicker.setMaxValue(minNumbers.length - 1);
        mpMinPicker.setMinValue(0);
        mpMinPicker.setTextColor(context.getResources().getColor(R.color.title_text_color));
        mpMinPicker.setTextSize(18);
        view.findViewById(R.id.tv_confirm).setOnClickListener(v -> {
            chargingDialogClickLister.onChargingTimeConfirmClick(mpHourPicker.getValue(), mpMinPicker.getValue());
            if (isShowing()) {
                dismiss();
            }
        });

        view.findViewById(R.id.tv_cancel).setOnClickListener(v -> {
            if (isShowing()) {
                dismiss();
            }
        });
    }

    public void setHourMinValueInt(int hourInt, int minInt) {
        if (mpHourPicker != null) {
            mpHourPicker.setValue(hourInt);
        }
        if (mpMinPicker != null) {
            mpMinPicker.setValue(minInt);
        }
    }

    public void showDialog() {
        if (!isShowing()) {
            show();
        }
    }
}
