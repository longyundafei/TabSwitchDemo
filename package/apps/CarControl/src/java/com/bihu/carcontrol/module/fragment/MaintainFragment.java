package com.bihu.carcontrol.module.fragment;

import android.app.Dialog;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;

import com.alibaba.fastjson.JSON;
import com.android.car.client.manager.vehicle.CarVehicleManager;
import com.android.car.core.bean.MaintenanceTips;
import com.android.car.core.server.vehicle.VehicleType;
import com.bihu.carcontrol.R;
import com.bihu.carcontrol.base.fragment.BaseFragment;
import com.bihu.carcontrol.common.event.EventBusActionCode;
import com.bihu.carcontrol.common.event.VehicleMessageEventBean;
import com.bihu.carcontrol.common.greendao.DBManager;
import com.bihu.carcontrol.common.greendao.db.MaintainBean;
import com.bihu.carcontrol.common.util.DeviceUtil;
import com.bihu.carcontrol.common.util.DialogUtil;
import com.bihu.carcontrol.common.util.FastClickCheckUtil;
import com.bihu.carcontrol.common.util.LogUtil;
import com.bihu.carcontrol.common.util.LongClickUtils;
import com.bihu.carcontrol.module.main.CarControlActivity;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

/**
 * FileName: MaintainFragment
 * Author: WangXu
 * Date: 2023/6/13 10:41
 * Description: 维保设置
 */
public class MaintainFragment extends BaseFragment implements View.OnClickListener {
    private RadioGroup timeDistanceRg;
    private TextView tvMaintainNum, tvTimeOrDistance, tvCountDownTime, tvSurplusHour, tvExceedMaintain;
    private ImageView ivMaintain, switchMaintainTips;
    private Dialog mDialog;
    private CountDownTimer countDownTimer;
    private final List<TextView> mMaintainTimeTvs = new ArrayList<>();
    private final List<TextView> mMaintainMileageTvs = new ArrayList<>();

    private boolean mIsOn;
    private int mDistance;
    private int mTime = 3;
    //保养剩余选择项  position = 0 剩余里程  position =1 剩余时间
    private int selectPosition = 0;
    //是否设置保养里程时间弹框后点击确认（维保人员操作，点击确认后自动设置维保为0，后续维保操作再设置值）
    private boolean resetMaintainFirstClick = false;
    //保养时间是否设置
    private boolean maintainTimeSet = true;
    //保养里程是否设置
    private boolean maintainMileageSet = true;
    private LinearLayout llMaintainSurplus, llSurplusHour, llExceedMaintain;
    private RadioButton rbLeft, rbRight;
    //数据库存储保养设置信息
    private MaintainBean maintainBean;
    private Timer remainingTimer;
    private TimerTask remainingTask;
    private float lastTotalDrivingMiller = 0f;

    @Override
    protected int getContentViewId() {
        return R.layout.fragment_maintain;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EventBus.getDefault().register(this);
    }

    @Override
    protected void initView(View view) {
        TextView tvMaintainTime0 = view.findViewById(R.id.tv_maintain_time_0);
        TextView tvMaintainTime3 = view.findViewById(R.id.tv_maintain_time_3);
        TextView tvMaintainTime6 = view.findViewById(R.id.tv_maintain_time_6);
        TextView tvMaintainTime9 = view.findViewById(R.id.tv_maintain_time_9);
        TextView tvMaintainTime12 = view.findViewById(R.id.tv_maintain_time_12);
        tvMaintainTime0.setOnClickListener(this);
        tvMaintainTime3.setOnClickListener(this);
        tvMaintainTime6.setOnClickListener(this);
        tvMaintainTime9.setOnClickListener(this);
        tvMaintainTime12.setOnClickListener(this);
        mMaintainTimeTvs.add(tvMaintainTime0);
        mMaintainTimeTvs.add(tvMaintainTime3);
        mMaintainTimeTvs.add(tvMaintainTime6);
        mMaintainTimeTvs.add(tvMaintainTime9);
        mMaintainTimeTvs.add(tvMaintainTime12);

        TextView tvMaintainMileage0 = view.findViewById(R.id.tv_maintain_mileage_0);
        TextView tvMaintainMileage3 = view.findViewById(R.id.tv_maintain_mileage_3);
        TextView tvMaintainMileage6 = view.findViewById(R.id.tv_maintain_mileage_5);
        TextView tvMaintainMileage9 = view.findViewById(R.id.tv_maintain_mileage_8);
        TextView tvMaintainMileage12 = view.findViewById(R.id.tv_maintain_mileage_10);
        TextView tvMaintainMileage15 = view.findViewById(R.id.tv_maintain_mileage_12);
        tvMaintainMileage0.setOnClickListener(this);
        tvMaintainMileage3.setOnClickListener(this);
        tvMaintainMileage6.setOnClickListener(this);
        tvMaintainMileage9.setOnClickListener(this);
        tvMaintainMileage12.setOnClickListener(this);
        tvMaintainMileage15.setOnClickListener(this);
        mMaintainMileageTvs.add(tvMaintainMileage0);
        mMaintainMileageTvs.add(tvMaintainMileage3);
        mMaintainMileageTvs.add(tvMaintainMileage6);
        mMaintainMileageTvs.add(tvMaintainMileage9);
        mMaintainMileageTvs.add(tvMaintainMileage12);
        mMaintainMileageTvs.add(tvMaintainMileage15);

        TextView tvMaintainTips = view.findViewById(R.id.tv_maintain_tips);
        LongClickUtils.setLongClick(new Handler(), tvMaintainTips, 5000, v -> {
            LogUtil.d("  保养提示5S长按 ");
            showMaintainClearDataDialog();
            return true;
        });


        timeDistanceRg = view.findViewById(R.id.time_distance_rg);
        tvMaintainNum = view.findViewById(R.id.tv_maintain_num);
        tvTimeOrDistance = view.findViewById(R.id.tv_time_or_distance);
        ivMaintain = view.findViewById(R.id.iv_maintain);
        ivMaintain.setOnClickListener(v -> {
//            showMaintainDialog();
            List<MaintainBean> maintainBeanList = DBManager.getMaintainBeanDao().loadAll();
            LogUtil.d("保养设置次数： " + maintainBeanList.size());
            if (maintainBeanList.size() > 0) {
                LogUtil.d("保养设置次数信息:  " + JSON.toJSONString(maintainBeanList));
            }
        });

        switchMaintainTips = view.findViewById(R.id.switch_maintain_tips);
        switchMaintainTips.setOnClickListener(v -> {
            if (DeviceUtil.ifPowerOff() || FastClickCheckUtil.isFastClick(500)) {
                return;
            }
            int value = mIsOn ? 0 : 1;
            setMaintenanceInfo(VehicleType.MAINTENANCE_REMAINING_TIPS, value);
        });

        llMaintainSurplus = view.findViewById(R.id.ll_maintain_surplus);
        rbLeft = view.findViewById(R.id.rb_left);
        rbRight = view.findViewById(R.id.rb_right);
        llSurplusHour = view.findViewById(R.id.ll_surplus_hour);
        tvSurplusHour = view.findViewById(R.id.tv_surplus_hour);
        llExceedMaintain = view.findViewById(R.id.ll_exceed_maintain);
        tvExceedMaintain = view.findViewById(R.id.tv_exceed_maintain);

        timeDistanceRg.setOnCheckedChangeListener((radioGroup, i) -> {
            if (i == R.id.rb_left) {
                selectPosition = 0;
                rbLeft.setTextColor(getResources().getColor(R.color.white));
                rbRight.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
                refreshMaintainSurplusUi();
            }
            if (i == R.id.rb_right) {
                selectPosition = 1;
                rbLeft.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
                rbRight.setTextColor(getResources().getColor(R.color.white));
                refreshMaintainSurplusUi();
            }
        });


        CarControlActivity activity = (CarControlActivity) getActivity();
        assert activity != null;
        if (activity.hasInit) {//SDKCar初始化未完成时不能请求数据

            geiMaintainInfo();
            postGetRemainingTime();
            refreshMaintainSurplusUi();
        }
    }

    private void geiMaintainInfo() {
        MaintenanceTips maintenanceTips = CarVehicleManager.getInstance().requestGetMaintenanceInfo();

        if (maintenanceTips != null) {
            mTime = maintenanceTips.getMaintenanceRemainingTime();
            updateTimeView(mTime);

            mIsOn = maintenanceTips.isMaintenanceRemainingTips();
            switchMaintainTips.setImageResource(mIsOn ? R.drawable.ic_switch_on : R.drawable.ic_switch_off);

            mDistance = maintenanceTips.getMaintenanceRemainingMileage();
            updateMileView(mDistance);
        } else {
            LogUtil.d("获取保养信息为 null");
        }
    }

    private void postGetRemainingTime() {
        remainingTimer = new Timer();
        remainingTask = new TimerTask() {
            @Override
            public void run() {
                if (selectPosition == 1) {
                    llMaintainSurplus.post(() -> {
                        LogUtil.d("主动刷新获取保养时间");
                        refreshMaintainSurplusUi();
                    });
                }
            }
        };
        //一分钟刷新一次剩余时间
        remainingTimer.schedule(remainingTask, 60000, 60000);
    }

    private void setMaintenanceInfo(int vehicleType, int value) {
        CarVehicleManager.getInstance().requestSetMaintenanceInfo(vehicleType, value);
    }

    //确定保养完成，重新设置保养里程和时间
    private void showMaintainClearDataDialog() {
        View view = LayoutInflater.from(getActivity()).inflate(R.layout.dialog_maintenance_clear_data, (ViewGroup) null);
        mDialog = DialogUtil.getDialog(getActivity(), view, 0.36);
        mDialog.setCanceledOnTouchOutside(false);
        tvCountDownTime = view.findViewById(R.id.tv_count_down_time);
        countDownTime(30 * 1000, 1000);

        view.findViewById(R.id.tv_confirm).setOnClickListener(view2 -> {
            if (DeviceUtil.ifPowerOff() || FastClickCheckUtil.isFastClick(500)) {
                return;
            }
            //确定保养完成，重新设置保养里程和时间
            resetMaintainFirstClick = true;
            maintainTimeSet = false;
            maintainMileageSet = false;
            setMaintainTime(0);
            setMaintainMileage(0);
            maintainBean = new MaintainBean();
            hideDialogCancelCountDown();
        });

        view.findViewById(R.id.tv_cancel).setOnClickListener(view2 -> {
            hideDialogCancelCountDown();
            resetMaintainStatus();
        });
    }

    private void hideDialogCancelCountDown() {
        if (countDownTimer != null) {
            countDownTimer.cancel();
        }
        if (mDialog != null) {
            mDialog.dismiss();
        }
    }

    private void countDownTime(long millisInFuture, long countDownInterval) {
        countDownTimer = new CountDownTimer(millisInFuture, countDownInterval) {
            @Override
            public void onTick(long millisUntilFinished) {
                tvCountDownTime.setText(String.valueOf((millisUntilFinished / 1000) + 1));
            }

            @Override
            public void onFinish() {
                hideDialogCancelCountDown();
            }
        };
        countDownTimer.start();
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onHandleEvent(VehicleMessageEventBean event) {
        int code = event.getCode();
        //if (Debug) Log.d(TAG, "code = " + code + "cmd = " + cmd);
        if (code == EventBusActionCode.MAINTENANCE_TIPS_INFO_CHANGE_CODE) {//保养信息设置变化
            MaintenanceTips cmd = (MaintenanceTips) event.getCmd();
            assert cmd != null;
            updateForChange(cmd);
        } else if (code == EventBusActionCode.HOME_SWITCH_TAB_CODE) {
            int index = (int) event.getCmd();
            if (index != 3) {//首页切换tab，不是维保的话重置维保的可设置状态
                LogUtil.d("首页切换tab:  " + index);
                resetMaintainStatus();
            }
        } else if (code == EventBusActionCode.TOTAL_DRIVING_MILLER_CHANGE_CODE) {//总里程变化，计算剩余里程
            if (selectPosition == 0) {
                float cmd = (float) event.getCmd();
                if (cmd != lastTotalDrivingMiller) {//总里程有变化再刷新保养信息
                    refreshMaintainSurplusUi();
                    lastTotalDrivingMiller = cmd;
                }
            }
        }
    }

    private void updateForChange(MaintenanceTips cmd) {
        // maintenanceRemainingMileage;//保养剩余里程
        // maintenanceRemainingTime;//保养剩余时间
        // maintenanceRemainingErrorMsg;//保养错误信息
        // maintenanceRemainingTips;//是否开启保养提醒
        boolean isOn = cmd.isMaintenanceRemainingTips();
        if (mIsOn != isOn) {
            mIsOn = isOn;
            switchMaintainTips.setImageResource(mIsOn ? R.drawable.ic_switch_on : R.drawable.ic_switch_off);
        }
        LogUtil.d("保养提示回调： " + isOn);
        if (isOn) {
            llMaintainSurplus.setVisibility(View.VISIBLE);
            rbLeft.setEnabled(true);
            rbRight.setEnabled(true);
            rbLeft.setChecked(true);
            rbRight.setChecked(false);
            selectPosition = 0;
            rbLeft.setTextColor(getResources().getColor(R.color.white));
            rbRight.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
        } else {
            llMaintainSurplus.setVisibility(View.GONE);
            llExceedMaintain.setVisibility(View.GONE);
            rbLeft.setChecked(false);
            rbRight.setChecked(false);
            rbLeft.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            rbRight.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            rbLeft.setEnabled(false);
            rbRight.setEnabled(false);
            //必须清除状态，不然再开启选中状态不生效
            timeDistanceRg.clearCheck();
        }

        int mileage = cmd.getMaintenanceRemainingMileage();
        if (mDistance != mileage) {
            mDistance = mileage;
            LogUtil.d("保养距离回调： " + mileage);
            updateMileView(mDistance);
        }

        int time = cmd.getMaintenanceRemainingTime();
        if (time != mTime) {
            mTime = time;
            LogUtil.d("保养时间回调： " + time);
            updateTimeView(mTime);
        }

        //刷新保养剩余里程或时间
        refreshMaintainSurplusUi();
    }

    private void updateMileView(int value) {
        if (value == 0) {
            refreshMaintainMileageUi(0);
        } else if (value == 3000) {
            refreshMaintainMileageUi(1);
        } else if (value == 5000) {
            refreshMaintainMileageUi(2);
        } else if (value == 8000) {
            refreshMaintainMileageUi(3);
        } else if (value == 10000) {
            refreshMaintainMileageUi(4);
        } else if (value == 12000) {
            refreshMaintainMileageUi(5);
        }
    }

    private void updateTimeView(int value) {
        if (value == 0) {
            refreshMaintainTimeUi(0);
        } else if (value == 3) {
            refreshMaintainTimeUi(1);
        } else if (value == 6) {
            refreshMaintainTimeUi(2);
        } else if (value == 9) {
            refreshMaintainTimeUi(3);
        } else if (value == 12) {
            refreshMaintainTimeUi(4);
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        LogUtil.d("维修保养 onDestroy");
        hideDialogCancelCountDown();
        EventBus.getDefault().unregister(this);
        if (remainingTimer != null) {
            remainingTimer.cancel();
            remainingTimer = null;
        }
        if (remainingTask != null) {
            remainingTask.cancel();
            remainingTask = null;
        }
    }

    //刷新保养剩余里程或时间 position = 0 剩余里程  position =1 剩余时间
    private void refreshMaintainSurplusUi() {
        if (!mIsOn) {
            llMaintainSurplus.setVisibility(View.GONE);
            return;
        }

        //获取保养信息
        MaintenanceTips maintenanceInfo = CarVehicleManager.getInstance().requestGetMaintenanceInfo();
        if (selectPosition == 0) {//保养剩余里程
            if (maintenanceInfo != null) {
                llSurplusHour.setVisibility(View.GONE);

                //设置的保养里程
                int mRemainingMileage = maintenanceInfo.getMaintenanceRemainingMileage();
                LogUtil.d("获取设置的保养里程:  " + mRemainingMileage);
                if (mRemainingMileage != 0 && mRemainingMileage != 3000 && mRemainingMileage != 5000 && mRemainingMileage != 8000
                        && mRemainingMileage != 10000 && mRemainingMileage != 12000) {//设置保养数据异常剩余里程显示 --
                    showErrDistanceUi();
                } else if (mRemainingMileage == 0) {
                    llMaintainSurplus.setVisibility(View.GONE);
                    llExceedMaintain.setVisibility(View.GONE);
                } else {
                    //获取当前总里程
                    int totalMillers = (int) CarVehicleManager.getInstance().getVehicleDrivingTotalMillers();
                    LogUtil.d("获取当前总里程:  " + totalMillers);
                    int lastSettingTotalMillers = (int) maintenanceInfo.getMaintenanceStartMileage();
                    LogUtil.d("当前保养周期内开始里程： " + lastSettingTotalMillers);

                    int surplusMileage = (lastSettingTotalMillers + mRemainingMileage) - totalMillers;
                    if (surplusMileage > 12000) {//超出最大保养里程为异常数 显示 --
                        showErrDistanceUi();
                    } else if (surplusMileage >= 0) {//保养里程内显示剩余多少公里里程需要保养
                        llMaintainSurplus.setVisibility(View.VISIBLE);
                        llExceedMaintain.setVisibility(View.GONE);
                        tvMaintainNum.setText(String.valueOf(surplusMileage));
//                        LogUtil.d("保养剩余里程计算结果：" + surplusMileage);
                        tvTimeOrDistance.setText(getString(R.string.kilometers_need_maintenance));
                    } else {//超过保养里程
                        llMaintainSurplus.setVisibility(View.GONE);
                        llExceedMaintain.setVisibility(View.VISIBLE);
                        tvExceedMaintain.setText(getString(R.string.exceeding_maintenance_mileage));
                    }
                }
            } else {
                showErrDistanceUi();
            }

            ivMaintain.setImageResource(R.drawable.maintenance_one);
        } else if (selectPosition == 1) {//保养剩余时间
            if (maintenanceInfo != null) {
                //获取设置的保养时间
                int mRemainingTime = maintenanceInfo.getMaintenanceRemainingTime();
                LogUtil.d("获取设置的保养时间:  " + mRemainingTime);

                if (mRemainingTime != 0 && mRemainingTime != 3 && mRemainingTime != 6 && mRemainingTime != 9 && mRemainingTime != 12) {//设置保养时间数据异常保养剩余时间显示 --
                    showErrTimeUi();
                } else if (mRemainingTime == 0) {//设置保养时间为off时
                    llMaintainSurplus.setVisibility(View.GONE);
                    llExceedMaintain.setVisibility(View.GONE);
                } else {
                    int startTime = maintenanceInfo.getMaintenanceStartTime();
                    LogUtil.d("当前保养周期内开始时间： " + startTime);
                    long lastSettingTime = startTime == 0 ? System.currentTimeMillis() + 10 : (startTime * 1000L);

                    LogUtil.d("获取上次设置保养时的时间:  " + lastSettingTime);
                    long oneHour = 60 * 60 * 1000;
                    LogUtil.d("当前时间:  " + System.currentTimeMillis());
                    long surplusTime = (lastSettingTime + ((long) mRemainingTime * 30 * 24 * oneHour)) - System.currentTimeMillis();
                    LogUtil.d("保养剩余时间计算结果 surplusTime ： " + surplusTime);
                    LogUtil.d("保养剩余时间计算结果： " + surplusTime / (oneHour * 24) + "  天  " + (surplusTime / oneHour) % 24 + "小时" + (surplusTime / 60000) % 60 + "分钟");
                    if ((surplusTime / (oneHour * 24)) > 360) {//剩余保养天数大于一年，异常数据显示“--”
                        showErrTimeUi();
                    } else if (surplusTime >= 0) {//保养时间内
                        llMaintainSurplus.setVisibility(View.VISIBLE);
                        llSurplusHour.setVisibility(View.VISIBLE);
                        llExceedMaintain.setVisibility(View.GONE);
                        tvMaintainNum.setText(String.valueOf(surplusTime / (oneHour * 24)));
                        tvSurplusHour.setText(String.valueOf((surplusTime / oneHour) % 24));
                        tvTimeOrDistance.setText(getString(R.string.hourly_maintenance_required));
                    } else {//超过保养时间
                        llMaintainSurplus.setVisibility(View.GONE);
                        llExceedMaintain.setVisibility(View.VISIBLE);
                        tvExceedMaintain.setText(getString(R.string.exceeding_maintenance_time));
                    }
                }
            } else {//无保养数据
                showErrTimeUi();
            }

            ivMaintain.setImageResource(R.drawable.maintenance_two);
        }
    }

    private void showErrTimeUi() {
        llMaintainSurplus.setVisibility(View.VISIBLE);
        llExceedMaintain.setVisibility(View.GONE);
        tvMaintainNum.setText("--");
        tvTimeOrDistance.setText(getString(R.string.hourly_maintenance_required));

    }

    private void showErrDistanceUi() {
        llMaintainSurplus.setVisibility(View.VISIBLE);
        llExceedMaintain.setVisibility(View.GONE);
        tvMaintainNum.setText("--");
        tvTimeOrDistance.setText(getString(R.string.kilometers_need_maintenance));
    }

    @Override
    public void onClick(View v) {
        int id = v.getId();
        if (id == R.id.tv_maintain_time_0) {
            setMaintainTime(0);
        } else if (id == R.id.tv_maintain_time_3) {
            setMaintainTime(1);
        } else if (id == R.id.tv_maintain_time_6) {
            setMaintainTime(2);
        } else if (id == R.id.tv_maintain_time_9) {
            setMaintainTime(3);
        } else if (id == R.id.tv_maintain_time_12) {
            setMaintainTime(4);
        } else if (id == R.id.tv_maintain_mileage_0) {
            setMaintainMileage(0);
        } else if (id == R.id.tv_maintain_mileage_3) {
            setMaintainMileage(1);
        } else if (id == R.id.tv_maintain_mileage_5) {
            setMaintainMileage(2);
        } else if (id == R.id.tv_maintain_mileage_8) {
            setMaintainMileage(3);
        } else if (id == R.id.tv_maintain_mileage_10) {
            setMaintainMileage(4);
        } else if (id == R.id.tv_maintain_mileage_12) {
            setMaintainMileage(5);
        }
    }

    //保养时间设置
    public void setMaintainTime(int maintainTime) {
        if (!maintainTimeSet) {//未点击才可点击，已经点击设置过之后，不可点击，状态维持到下次弹出保养完成设置点击确认之后
            int timeSet = 12;
            switch (maintainTime) {
                case 0:
                    LogUtil.d("保养时间设置 OFF ");
                    timeSet = 0;
                    break;
                case 1:
                    LogUtil.d("保养时间设置 3 ： ");
                    timeSet = 3;
                    break;
                case 2:
                    LogUtil.d("保养时间设置 6 ： ");
                    timeSet = 6;
                    break;
                case 3:
                    LogUtil.d("保养时间设置 9 ： ");
                    timeSet = 9;
                    break;
                case 4:
                    LogUtil.d("保养时间设置 12 ： ");
                    timeSet = 12;
                    break;
            }
            if (!resetMaintainFirstClick) {//不是自动设置点击，进入修改设置状态
                maintainTimeSet = true;
                if (maintainBean != null) {
                    maintainBean.setIsMaintainTimeSet(true);
                    maintainBean.setMaintainTimeSetTime(System.currentTimeMillis());
                    maintainBean.setMaintainTimeSetValue(timeSet);
                    DBManager.getMaintainBeanDao().getSession().insertOrReplace(maintainBean);
                }
            }
            //保养时间设置（⽉）,同时mcu存储当前设置的时间
            setMaintenanceInfo(VehicleType.MAINTENANCE_REMAINING_TIME, timeSet);
        }
    }

    private void refreshMaintainTimeUi(int position) {
        for (int i = 0; i < mMaintainTimeTvs.size(); i++) {
            TextView textView = mMaintainTimeTvs.get(i);
            if (position == i) {
                textView.setBackgroundResource(R.drawable.bg_segmented_btn);
                textView.setTextColor(getResources().getColor(R.color.white));
            } else {
                textView.setBackground(null);
                textView.setTextColor(getResources().getColor(R.color.title_text_color));
            }
        }
    }

    //保养里程设置
    public void setMaintainMileage(int maintainMileage) {
        if (!maintainMileageSet) {
            int mileageSet = 3000;
            switch (maintainMileage) {
                case 0:
                    LogUtil.d("保养里程设置 OFF ： ");
                    mileageSet = 0;
                    break;
                case 1:
                    LogUtil.d("保养里程设置 : 3000KM");
                    mileageSet = 3000;
                    break;
                case 2:
                    LogUtil.d("保养里程设置 : 5000KM");
                    mileageSet = 5000;
                    break;
                case 3:
                    LogUtil.d("保养里程设置 : 8000KM");
                    mileageSet = 8000;
                    break;
                case 4:
                    LogUtil.d("保养里程设置 : 10000KM");
                    mileageSet = 10000;
                    break;
                case 5:
                    LogUtil.d("保养里程设置 : 12000KM");
                    mileageSet = 12000;
                    break;
            }
            if (resetMaintainFirstClick) {
                resetMaintainFirstClick = false;//弹框后第一次自动设置后修改状态，下次手动点击修改被点击状态
            } else {
                maintainMileageSet = true;
                if (maintainBean != null) {
                    maintainBean.setIsMaintainTimeSet(true);
                    maintainBean.setMaintainTimeSetTime(System.currentTimeMillis());
					maintainBean.setMaintainTimeSetValue(mileageSet);
                    DBManager.getMaintainBeanDao().getSession().insertOrReplace(maintainBean);
                }
            }
            //保养距离设置（Km）
            setMaintenanceInfo(VehicleType.MAINTENANCE_REMAINING_MILEAGE, mileageSet);
            //mcu存储设置里程是的总里程，用作获取计算当前周期内剩余里程
            float totalMillers = CarVehicleManager.getInstance().getVehicleDrivingTotalMillers();
            CarVehicleManager.getInstance().requestSetFloatPropertyAsRoot(VehicleType.MAINTENANCE_START_MILEAGE, totalMillers);
            LogUtil.d("设置当前周期内初始里程： " + totalMillers);
        }
    }

    private void refreshMaintainMileageUi(int position) {
        for (int i = 0; i < mMaintainMileageTvs.size(); i++) {
            TextView textView = mMaintainMileageTvs.get(i);
            if (position == i) {
                textView.setBackgroundResource(R.drawable.bg_segmented_btn);
                textView.setTextColor(getResources().getColor(R.color.white));
            } else {
                textView.setBackground(null);
                textView.setTextColor(getResources().getColor(R.color.title_text_color));
            }
        }
    }

    @Override
    public void onPause() {
        super.onPause();
        LogUtil.d("维修保养 onPause");
        resetMaintainStatus();
    }

    //重置维保的可设置状态
    private void resetMaintainStatus() {
        resetMaintainFirstClick = false;
        maintainTimeSet = true;
        maintainMileageSet = true;
        maintainBean = null;
    }
}
