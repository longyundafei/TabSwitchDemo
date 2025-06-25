package com.bihu.carcontrol.common.util;

import android.app.Dialog;
import android.content.Context;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.bihu.carcontrol.R;
import com.bihu.carcontrol.common.adapter.CountryRecyclerViewAdapter;
import com.bihu.carcontrol.common.bean.CountryBean;

import java.util.ArrayList;
import java.util.List;

/**
 * FileName: CountrySelectDialog
 * Author: WangXu
 * Date: 2024/12/20 11:34
 * Description:
 */
public class CountrySelectDialog extends Dialog {

    public interface OnSelectItemClick {
        void selectItemClick(CountryBean countryBean);
    }

    public CountrySelectDialog(@NonNull Context context, List<CountryBean> countryBeans, int selectCountryCode, double customizedWidthSize, OnSelectItemClick onSelectItemClick) {
        super(context);
        initView(context, countryBeans, selectCountryCode, customizedWidthSize, onSelectItemClick);
    }

    private void initView(Context context, List<CountryBean> countryBeans, int selectCountryCode, double customizedWidthSize, OnSelectItemClick onSelectItemClick) {
        View view = LayoutInflater.from(context).inflate(R.layout.dialog_select_country, (ViewGroup) null);
        setContentView(view);

        view.setOnClickListener(v -> {
            dismiss();
        });
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
        layoutParams.height = 538;
        setCanceledOnTouchOutside(false);

        RecyclerView mRecyclerView = view.findViewById(R.id.recycler_view);
        LinearLayoutManager layoutManager = new LinearLayoutManager(context);
        layoutManager.setOrientation(LinearLayoutManager.VERTICAL);
        mRecyclerView.setLayoutManager(layoutManager);

        CountryRecyclerViewAdapter mAdapter = new CountryRecyclerViewAdapter(context, countryBeans, selectCountryCode);
        mAdapter.setOnItemClick(countryBean -> {
            onSelectItemClick.selectItemClick(countryBean);
            dismiss();
        });
        mRecyclerView.setAdapter(mAdapter);
    }
}
