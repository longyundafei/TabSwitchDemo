package com.bihu.carcontrol.base.fragment;

import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.provider.Settings;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;


import com.bihu.carcontrol.base.base_view.BaseView;
import com.bihu.carcontrol.common.util.AppUtil;
import com.bihu.carcontrol.common.util.DeviceUtil;
import com.bihu.carcontrol.common.util.ScreenUtil;
import com.bihu.carcontrol.common.util.ToastMaster;
import com.bihu.carcontrol.module.MyApplication;

import io.reactivex.disposables.CompositeDisposable;

public abstract class BaseFragment extends Fragment implements BaseView {

    /////////////// 控件成员变量声明区 ///////////////

    private ViewGroup rootView;
    //    private ProgressDialog progressDialog;
    public CompositeDisposable compositeDisposable;
    public boolean isViewCreated; // UI 是否初使化完成
//    private LoadingDialog loadingDialog;
    /////////////// 内部接口声明区 ///////////////

    protected abstract int getContentViewId();

    protected abstract void initView(View view);

    public MyApplication myApplication;

    /////////////// 初始化方法区 ///////////////

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        myApplication = MyApplication.getApplication();
        compositeDisposable = new CompositeDisposable();
        if (getContentViewId() == 0) {
            return super.onCreateView(inflater, container, savedInstanceState);
        } else {
            if (rootView == null) {
                rootView = (ViewGroup) inflater.inflate(getContentViewId(), null, false);
                rootView.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        onRootViewClick();
                    }
                });
                onInflateViewAfter(rootView);
                onInitViewBefore();
                initView(rootView);
            } else {
                ViewGroup parent = (ViewGroup) rootView.getParent();
                if (parent != null) {
                    parent.removeView(rootView);
                }
            }
            return rootView;
        }
    }

    protected void onInflateViewAfter(View view) {
        isViewCreated = true;
    }

    protected void onInitViewBefore() {

    }

    protected void onInitViewAfter() {

    }

    public void onArgumentsChanged() {

    }
    /////////////// 事件响应方法区 ///////////////

    protected void onRootViewClick() {
//        ScreenUtil.hideSoftInput(getActivity());
    }

    protected void onProgressDialogDismiss(DialogInterface dialog) {

    }

    /////////////// 普通逻辑方法区 ///////////////

    protected String getPageName() {
        return getClass().getSimpleName();
    }

    protected MyApplication getAppContext() {
        return MyApplication.getApplication();
    }

    public void post(Runnable r) {
        getAppContext().post(r);
    }

    public void postDelayed(Runnable r, int delayMillis) {
        getAppContext().postDelayed(r, delayMillis);
    }


    /////////////// 重载逻辑方法区 ///////////////

    @Override
    public View getRootView() {
        return rootView;
    }

    @Override
    public void showProgress() {
//        if (loadingDialog == null) {
//            loadingDialog = new LoadingDialog(getActivity(), LoadingDialog.LOAD);
//        }
//        loadingDialog.show();
////        if (progressDialog == null) {
////            progressDialog = new ProgressDialog(this, R.style.TransparentDialog);
////            progressDialog.setOnDismissListener(new DialogInterface.OnDismissListener() {
////                @Override
////                public void onDismiss(DialogInterface dialogInterface) {
////                    onProgressDialogDismiss(dialogInterface);
////                }
////            });
////        } else {
////            progressDialog.setText(R.string.common_progress_loading);
////        }
////        progressDialog.show();
    }

    //
    @Override
    public void showProgress(int textResId) {
//        if (loadingDialog == null) {
//            loadingDialog = new LoadingDialog(getActivity(), LoadingDialog.LOAD, textResId);
//        }
//        loadingDialog.show();
//
////        if (progressDialog == null) {
////            progressDialog = new ProgressDialog(this, R.style.TransparentDialog, textResId);
////            progressDialog.setOnDismissListener(new DialogInterface.OnDismissListener() {
////                @Override
////                public void onDismiss(DialogInterface dialogInterface) {
////                    onProgressDialogDismiss(dialogInterface);
////                }
////            });
////        } else {
////            progressDialog.setText(textResId);
////        }
////        progressDialog.show();
    }

    //
    @Override
    public void showProgress(String message) {
//        if (loadingDialog == null) {
//            loadingDialog = new LoadingDialog(getActivity(), LoadingDialog.LOAD, message);
//        }
//        loadingDialog.show();
//
////        if (progressDialog == null) {
////            progressDialog = new ProgressDialog(this, R.style.TransparentDialog, message);
////            progressDialog.setOnDismissListener(new DialogInterface.OnDismissListener() {
////                @Override
////                public void onDismiss(DialogInterface dialogInterface) {
////                    onProgressDialogDismiss(dialogInterface);
////                }
////            });
////        } else {
////            progressDialog.setText(message);
////        }
////        progressDialog.show();
    }

    //
    @Override
    public void hideProgress() {
//        if (loadingDialog != null && loadingDialog.isShowing()) {
//            try {
//                loadingDialog.dismiss();
//            } catch (Exception e) {
//                e.printStackTrace();
//            }
//        }
//        loadingDialog = null;
////        if (progressDialog != null && progressDialog.isShowing()) {
////            try {
////                progressDialog.dismiss();
////            } catch (Exception e) {
////                e.printStackTrace();
////            }
////        }
////        progressDialog = null;
    }

    //
    @Override
    public boolean isProgressShowing() {
//        return loadingDialog != null && loadingDialog.isShowing();
        return false;
    }

    /////////////// 发起异步任务方法区 ///////////////

    /////////////// 异步任务回调方法区 ///////////////

    /////////////// 生命周期回调方法区 ///////////////


    @Override
    public void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);
        String FRAGMENTS_TAG = "android:support:fragments";
        outState.remove(FRAGMENTS_TAG);
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public void onViewCreated(@NonNull View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        onInitViewAfter();
    }

    @Override
    public void onResume() {
        super.onResume();
    }

    @Override
    public void onPause() {
        super.onPause();
//        ScreenUtil.hideSoftInput(getActivity());
//        hideProgress();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        hideProgress();
        if (compositeDisposable != null) {
            compositeDisposable.clear();
            compositeDisposable = null;
        }
        rootView = null;
    }

    @Override
    public void onDetach() {
        super.onDetach();
    }
    /////////////// 内部类声明区 ///////////////

    public void startAppSettings() {
        Intent intent = new Intent(
                Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
        intent.setData(Uri.parse("package:" + getActivity().getPackageName()));
        startActivity(intent);
    }


    @Override
    public void isNetworkReachable() {
        if (!DeviceUtil.isNetworkReachable(AppUtil.getContext())) {
            ToastMaster.shortToast("似乎已断开与互联网的连接");
        }
    }

}
