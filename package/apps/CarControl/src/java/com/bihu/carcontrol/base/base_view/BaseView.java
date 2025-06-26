package com.bihu.carcontrol.base.base_view;

import android.view.View;

public interface BaseView {
    View getRootView();

    void showProgress();

    void showProgress(int textResId);

    void showProgress(String message);

    void hideProgress();

    boolean isProgressShowing();

    void isNetworkReachable();
}
