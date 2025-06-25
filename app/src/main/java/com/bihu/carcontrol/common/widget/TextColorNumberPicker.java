package com.bihu.carcontrol.common.widget;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.drawable.ColorDrawable;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.NumberPicker;

import java.lang.reflect.Field;

/**
 * FileName: TextColorNumberPicker
 * Author: WangXu
 * Date: 2023/8/17 14:18
 * Description:
 */
public class TextColorNumberPicker extends NumberPicker implements NumberPicker.OnValueChangeListener {
    private int mCurrentValue;

    public TextColorNumberPicker(Context context) {
        super(context);
    }

    public TextColorNumberPicker(Context context, AttributeSet attrs) {
        super(context, attrs);
        setOnValueChangedListener(this);
    }

    public TextColorNumberPicker(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @Override
    public void addView(View child) {
        super.addView(child);
        updateView(child);
    }

    @Override
    public void addView(View child, int index, ViewGroup.LayoutParams params) {
        super.addView(child, index, params);
        updateView(child);
    }

    @Override
    public void addView(View child, ViewGroup.LayoutParams params) {
        super.addView(child, params);
        updateView(child);
    }

    public void updateView(View view) {
        if (view instanceof EditText) {
            ((EditText) view).setTextColor(-1);
            ((EditText) view).setTextSize(10.0f);
        }
    }

    public void setPickerDividerColor(int color) {
        ColorDrawable drawable = new ColorDrawable(color);
        Field[] pickerFields = NumberPicker.class.getDeclaredFields();
        for (Field pf : pickerFields) {
            if (pf.getName().equals("mSelectionDivider")) {
                pf.setAccessible(true);
                try {
                    pf.set(this, drawable);
                    return;
                } catch (Resources.NotFoundException | IllegalAccessException | IllegalArgumentException e) {
                    e.printStackTrace();
                    return;
                }
            }
        }
    }

    public int getCurrentValue() {
        return this.mCurrentValue;
    }

    @Override
    public void setValue(int value) {
        super.setValue(value);
        this.mCurrentValue = value;
    }

    @Override
    public void onValueChange(NumberPicker numberPicker, int oldValue, int newValue) {
        this.mCurrentValue = newValue;
        Log.e("xxx", "mCurrentValue: " + this.mCurrentValue);
    }
}
