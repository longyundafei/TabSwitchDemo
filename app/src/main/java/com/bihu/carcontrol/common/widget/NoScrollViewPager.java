package com.bihu.carcontrol.common.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.MotionEvent;

import androidx.viewpager.widget.ViewPager;

/**
 * FileName: NoScrollViewPager
 * Author: WangXu
 * Date: 2023/6/13 11:09
 * Description:
 */
public class NoScrollViewPager extends ViewPager {

    private boolean noScroll;

    public NoScrollViewPager(Context context) {
        super(context);
        this.noScroll = true;
    }

    public NoScrollViewPager(Context context, AttributeSet attrs) {
        super(context, attrs);
        this.noScroll = true;
    }

    public void setNoScroll(boolean noScroll) {
        this.noScroll = noScroll;
    }

    @Override
    public void scrollTo(int x, int y) {
        super.scrollTo(x, y);
    }

    @Override
    public boolean onTouchEvent(MotionEvent ev) {
        if (this.noScroll) {
            return false;
        }
        return super.onTouchEvent(ev);
    }

    @Override
    public boolean onInterceptTouchEvent(MotionEvent arg0) {
        if (this.noScroll) {
            return false;
        }
        return super.onInterceptTouchEvent(arg0);
    }

    @Override
    public void setCurrentItem(int item, boolean smoothScroll) {
        super.setCurrentItem(item, smoothScroll);
    }

    @Override
    public void setCurrentItem(int item) {
        super.setCurrentItem(item);
    }

}
