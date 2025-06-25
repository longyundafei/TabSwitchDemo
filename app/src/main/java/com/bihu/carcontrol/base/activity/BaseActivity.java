package com.bihu.carcontrol.base.activity;

import android.app.Activity;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.FrameLayout;

import androidx.core.app.ActivityCompat;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentActivity;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import com.bihu.carcontrol.base.fragment.BaseFragment;
import com.bihu.carcontrol.common.util.ActivityStack;
import com.bihu.carcontrol.common.util.AppUtil;
import com.bihu.carcontrol.common.util.DeviceUtil;
import com.bihu.carcontrol.common.util.ScreenUtil;
import com.bihu.carcontrol.common.util.ToastMaster;
import com.bihu.carcontrol.module.MyApplication;

import io.reactivex.disposables.CompositeDisposable;

/**
 * 基本类
 *
 * @author Wangxu
 * @since 2023/6/12
 */
public abstract class BaseActivity extends FragmentActivity implements BaseView, ActivityCompat
        .OnRequestPermissionsResultCallback {

    //    private ProgressDialog progressDialog;
    private FrameLayout rootView;
    public MyApplication myApplication;
    //rxjava 注销
    public CompositeDisposable compositeDisposable;
//    protected ProgressDialog downloadDialog;
//    private LoadingDialog loadingDialog;

    protected void onProgressDialogDismiss(DialogInterface dialog) {
    }

    protected abstract void initView();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        myApplication = MyApplication.getApplication();
        compositeDisposable = new CompositeDisposable();
        initRootView();
        addToStack();
        Log.e("class", getClass().getSimpleName());
    }

    protected void initRootView() {
        rootView = new FrameLayout(this);
        setContentView(rootView, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
    }

    public View getRootView() {
        return rootView;
    }

    public void setContentView(int layoutResID) {
        View view = LayoutInflater.from(this).inflate(layoutResID, null);
        rootView.addView(view);
        initView();
    }

    @Override
    public void setContentView(View view) {
        rootView.addView(view);
        initView();
    }

    protected void setWindowBrightness(Activity activity, float brightness) {
        Window window = activity.getWindow();
        WindowManager.LayoutParams lp = window.getAttributes();
        lp.screenBrightness = brightness;
        window.setAttributes(lp);
    }

    protected MyApplication getAppContext() {
        return MyApplication.getApplication();
    }

    protected ActivityStack getActivityStack() {
        return getAppContext().getActivityStack();
    }

    protected void addToStack() {
        addToStack(null);
    }

    protected void addToStack(String tag) {
        ActivityStack activityStack = getActivityStack();
        if (activityStack != null)
            activityStack.add(this, tag);
    }

    protected void removeFromStack() {
        ActivityStack activityStack = getActivityStack();
        if (activityStack != null)
            activityStack.remove(this);
    }

    public void finishByTagOfStack(String tag) {
        ActivityStack activityStack = getActivityStack();
        if (activityStack != null)
            activityStack.finishByTag(tag);
    }

    public void post(Runnable r) {
        getAppContext().post(r);
    }

    public void postDelayed(Runnable r, int delayMillis) {
        getAppContext().postDelayed(r, delayMillis);
    }

    public void removeCallbacks(Runnable r) {
        getAppContext().removeCallbacks(r);
    }

    @Override
    protected void onRestoreInstanceState(Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        //fix Can not perform this action after onSaveInstanceState
        //http://stackoverflow.com/questions/7575921/illegalstateexception-can-not-perform-this-action-after-onsaveinstancestate-h
        outState.putString("WORKAROUND_FOR_BUG_19917_KEY", "WORKAROUND_FOR_BUG_19917_VALUE");
        super.onSaveInstanceState(outState);
    }

    @Override
    protected void onStart() {
        super.onStart();
        getActivityStack().startCount();
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
//        hideProgress();
    }

    @Override
    protected void onStop() {
        super.onStop();
        getActivityStack().stopCount();
    }

    @Override
    public void showProgress() {
//        if (loadingDialog == null) {
//            loadingDialog = new LoadingDialog(this, LoadingDialog.LOAD);
//        }
//        if (!loadingDialog.isShowing()) {
//            loadingDialog.show();
//        }
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
//            loadingDialog = new LoadingDialog(this, LoadingDialog.LOAD, textResId);
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
//            loadingDialog = new LoadingDialog(this, LoadingDialog.LOAD, message);
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

    @Override
    public void isNetworkReachable() {
        if (!DeviceUtil.isNetworkReachable(AppUtil.getContext())) {
            ToastMaster.shortToast("似乎已断开与互联网的连接");
        }
    }

    /////////////// 事件响应方法区 ///////////////
    protected void onRootViewClickHideSoftInput() {
        rootView.setOnClickListener(v -> {
//            ScreenUtil.hideSoftInput(this);
        });
    }

//    //升级弹框
//    public void showAppUpgradeDialog(VersionInfo versionInfo) {
//        if (versionInfo == null || DownloadManager.getInstance(BaseActivity.this).isDownloading()) {
//            ToastMaster.shortToast("正在下载中...请稍后再试");
//            return;
//        }
//
//        SimpleDialog.Builder appUpgradeBuild = new SimpleDialog.Builder(this);
//        appUpgradeBuild.setMessage("有新版本，请更新~");
//        appUpgradeBuild.setOnOkClickListener(() -> {//更新版本
//            try {
//                DownloadManager.getInstance(BaseActivity.this).release();
//            } catch (Exception e) {
//                e.printStackTrace();
//            }
//            String apkName = System.currentTimeMillis() + "AppUpdate.apk";
//            DownloadManager manager = DownloadManager.getInstance(BaseActivity.this);
//            UpdateConfiguration updateConfiguration = new UpdateConfiguration();
//            updateConfiguration.setOnDownloadListener(new OnDownloadListener() {
//                @Override
//                public void start() {
//                    //升级过程中，显示下载框， 不允许做其他操作
//                    showDownloadProgress(R.string.downloading);
//                }
//
//                @Override
//                public void downloading(int max, int progress) {
//                    if (downloadDialog != null && downloadDialog.isShowing()) {
//                        downloadDialog.setText("正在下载中 " + progress / (max / 100) + "%");
//                    }
//                }
//
//                @Override
//                public void done(File apk) {
//                    if (downloadDialog != null && downloadDialog.isShowing()) {
//                        downloadDialog.dismiss();
//                    }
//                }
//
//                @Override
//                public void cancel() {
//                    if (downloadDialog != null && downloadDialog.isShowing()) {
//                        downloadDialog.dismiss();
//                    }
//                }
//
//                @Override
//                public void error(Exception e) {
//                    if (downloadDialog != null && downloadDialog.isShowing()) {
//                        downloadDialog.dismiss();
//                    }
//                }
//            });
//            manager.setApkName(apkName)
//                    .setApkUrl(versionInfo.downloadUrl)
//                    .setDownloadPath(Environment.getExternalStorageDirectory() + "/AppUpdate")
//                    .setSmallIcon(R.drawable.img_logo_light)
//                    .setConfiguration(updateConfiguration)
//                    .download();
//
//        }).build().show();
//    }
//
//    public void showDownloadProgress(int textResId) {
//        downloadDialog = new ProgressDialog(this, R.style.TransparentDialog, textResId);
//        downloadDialog.setCancelable(false);
//        downloadDialog.setCanceledOnTouchOutside(false);
//        downloadDialog.setOnKeyListener((anInterface, keyCode, event) -> {
//            if (keyCode == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_DOWN) {
//                return true;
//            }
//            return false;
//        });
//        downloadDialog.show();
//    }

    public View inflaterView(int resId) {
        return LayoutInflater.from(this).inflate(resId, null);
    }

    protected Fragment attach(int replaceLayoutId, BaseFragment fragment, String tag) {
        return attach(replaceLayoutId, fragment, tag, new Bundle());
    }

    protected Fragment attach(int replaceLayoutId, BaseFragment fragment, String tag, Bundle args) {
        if (fragment == null)
            return null;
        if (fragment.isAdded()) {
            return fragment;
        }
        if (args != null && fragment.getArguments() == null) {
            fragment.setArguments(args);
        }
        FragmentTransaction ft = getSupportFragmentManager().beginTransaction();
        ft.replace(replaceLayoutId, fragment, tag);
        ft.addToBackStack(tag);
        ft.commitAllowingStateLoss();
        return fragment;
    }

    protected boolean showFragment(int replaceLayoutId, String tag) {
        return showFragment(replaceLayoutId, tag, new Bundle());
    }

    protected boolean showFragment(int replaceLayoutId, String tag, Bundle args) {
        FragmentManager fm = getSupportFragmentManager();
        BaseFragment fragment = (BaseFragment) fm.findFragmentByTag(tag);
        if (fragment != null) {
            if (args != null) {
                if (fragment.getArguments() != null) {
                    fragment.getArguments().putAll(args);
                } else {
                    fragment.setArguments(args);
                }
                if (fragment.isViewCreated) {
                    fragment.onArgumentsChanged();
                }
            }
            FragmentTransaction ft = fm.beginTransaction();
            ft.replace(replaceLayoutId, fragment);
            ft.addToBackStack(tag);
            ft.commitAllowingStateLoss();
            return true;
        } else {
            return false;
        }
    }

    protected BaseFragment showFragment(int replaceLayoutId, Class<? extends BaseFragment> fragmentClass, Bundle args) {
        FragmentManager fm = getSupportFragmentManager();
        String tag = fragmentClass.getSimpleName();
        BaseFragment fragment = (BaseFragment) fm.findFragmentByTag(tag);
        boolean isFragmentExist = true;
        if (fragment == null) {
            try {
                isFragmentExist = false;
                fragment = fragmentClass.newInstance();
                if (args != null) {
                    fragment.setArguments(args);
                } else {
                    fragment.setArguments(new Bundle());
                }

            } catch (InstantiationException e) {
                e.printStackTrace();
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            }
        } else {
            if (args != null) {
                if (fragment.getArguments() != null) {
                    fragment.getArguments().putAll(args);
                } else {
                    fragment.setArguments(args);
                }
                if (fragment.isViewCreated) {
                    fragment.onArgumentsChanged();
                }
            }
        }
        if (fragment == null)
            return null;
        if (fragment.isAdded()) {
            return fragment;
        }
        FragmentTransaction ft = fm.beginTransaction();
        if (isFragmentExist) {
            ft.replace(replaceLayoutId, fragment);
        } else {
            ft.replace(replaceLayoutId, fragment, tag);
        }

        ft.addToBackStack(tag);
        ft.commitAllowingStateLoss();
        return fragment;
    }

    ////    public boolean checkIfTokenNotExistGoToSplash() {
////        if (!UserService.getDefault().getUser().isTokenExist()) {
////            RSplash.splash(this);
////            return false;
////        }
////        return true;
////    }
//
    public void finishIfMainNotExistGoToMain() {
        if (!isFinishing()) {
            if (!getActivityStack().hasMain()) {
//                RMain.home(this, true);
            } else {
                finish();
            }
        }
    }

    /**
     * 在三星手机上重复点击返回键会导致异常Crash
     * Can not perform this action after onSaveInstanceState : FragmentActivity.onBackPressed
     */
    public void onBackPressed() {
        if (!isFinishing()) {
            super.onBackPressed();
        } else {
            finish();
        }
    }

    @Override
    public void finish() {
        super.finish();
        removeFromStack();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
//        if (loadingDialog != null && loadingDialog.isShowing()) {
//            loadingDialog.dismiss();
//        }
////        if (progressDialog != null && progressDialog.isShowing()) {
////            progressDialog.dismiss();
////        }
        if (compositeDisposable != null) {
            compositeDisposable.clear();
            compositeDisposable = null;
        }
        removeFromStack();
    }
}

