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
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.core.content.ContextCompat;

import com.bihu.carcontrol.R;

/**
 * FileName: AdasDialogUtil
 * Author: WangXu
 * Date: 2025/2/27 11:12
 * Description: 弹窗
 */
public class AdasDialogUtil extends Dialog {
    private Dialog dialog;
    private LinearLayout llAdas1;
    private LinearLayout llAdas2;
    private TextView textView1;
    private TextView textView2;

    public AdasDialogUtil(@NonNull Context context) {
        super(context);
    }

    public AdasDialogUtil(@NonNull Context context, int viewInt1, int viewInt2) {
        super(context);
        initDialog(context, viewInt1, viewInt2, 1);
    }

    public AdasDialogUtil(@NonNull Context context, int viewInt1, int viewInt2, int showTab) {
        super(context);
        initDialog(context, viewInt1, viewInt2, showTab);
    }

    public void initDialog(Context context, int viewInt1, int viewInt2, int showTab) {
        dialog = new Dialog(context, R.style.DialogStyle);
        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        dialog.setTitle((CharSequence) null);
        dialog.setCancelable(true);
        View view = LayoutInflater.from(context).inflate(R.layout.dialog_adas_content, (ViewGroup) null);
        dialog.setContentView(view);
        dialog.getWindow().setBackgroundDrawableResource(R.drawable.bg_dialog);
        Window window = dialog.getWindow();
        window.setGravity(Gravity.CENTER);
        dialog.setCanceledOnTouchOutside(false);

        WindowManager.LayoutParams layoutParams = window.getAttributes();
        WindowManager windowManager = window.getWindowManager();
        Display display = windowManager.getDefaultDisplay();
        DisplayMetrics displayMetrics = new DisplayMetrics();
        display.getMetrics(displayMetrics);
        layoutParams.width = (int) (displayMetrics.widthPixels * 0.58);
        window.setAttributes(layoutParams);

        view.findViewById(R.id.iv_close).setOnClickListener(v -> {
            dismissDialog();
        });
        textView1 = view.findViewById(R.id.tv_asda_dialog_tab1);
        textView2 = view.findViewById(R.id.tv_asda_dialog_tab2);
        View view1 = LayoutInflater.from(context).inflate(viewInt1, (ViewGroup) null);
        View view2 = LayoutInflater.from(context).inflate(viewInt2, (ViewGroup) null);
        llAdas1 = view.findViewById(R.id.ll_adas1);
        llAdas2 = view.findViewById(R.id.ll_adas2);
        llAdas1.addView(view1);
        llAdas2.addView(view2);
        if (showTab == 1) {
            showLLAdas1(context);
        }

        if (showTab == 2) {
            showLLAdas2(context);
        }

        textView1.setOnClickListener(v -> {
            showLLAdas1(context);
        });

        textView2.setOnClickListener(v -> {
            showLLAdas2(context);
        });
    }

    private void showLLAdas2(Context context) {
        llAdas1.setVisibility(View.GONE);
        llAdas2.setVisibility(View.VISIBLE);
        textView1.setBackground(null);
        textView1.setTextColor(ContextCompat.getColor(context, R.color.tab_unselect_text_color));
        textView2.setBackgroundResource(R.drawable.bg_dialog_tab_item);
        textView2.setTextColor(ContextCompat.getColor(context, R.color.theme_blue));
    }

    private void showLLAdas1(Context context) {
        llAdas1.setVisibility(View.VISIBLE);
        llAdas2.setVisibility(View.GONE);
        textView1.setBackgroundResource(R.drawable.bg_dialog_tab_item);
        textView1.setTextColor(ContextCompat.getColor(context, R.color.theme_blue));
        textView2.setBackground(null);
        textView2.setTextColor(ContextCompat.getColor(context, R.color.tab_unselect_text_color));
    }

    public void showDialog() {
        if (dialog != null && !dialog.isShowing()) {
            dialog.show();
        }
    }

    public void dismissDialog() {
        if (dialog != null && dialog.isShowing()) {
            dialog.dismiss();
        }
    }

    public void setTab(String tabStr1, String tabStr2) {
        if (!TextUtils.isEmpty(tabStr1)) {
            textView1.setText(tabStr1);
        }
        if (!TextUtils.isEmpty(tabStr2)) {
            textView2.setText(tabStr2);
        }
    }

    public void showTabLayout(Context context, int showTab) {
        if (showTab == 1) {
            showLLAdas1(context);
        } else if (showTab == 2) {
            showLLAdas2(context);
        }
    }
}
