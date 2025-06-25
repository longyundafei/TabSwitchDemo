package com.bihu.carcontrol.module.fragment;

import android.annotation.SuppressLint;
import android.app.Dialog;
import android.os.Bundle;
import android.os.SystemProperties;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;

import androidx.core.content.ContextCompat;
import androidx.core.widget.NestedScrollView;

import com.android.car.client.manager.vehicle.CarVehicleManager;
import com.android.car.core.server.vehicle.VehicleType;
import com.bihu.carcontrol.R;
import com.bihu.carcontrol.base.fragment.BaseFragment;
import com.bihu.carcontrol.common.event.EventBusActionCode;
import com.bihu.carcontrol.common.event.VehicleMessageEventBean;
import com.bihu.carcontrol.common.util.ChargingTimeSelectDialog;
import com.bihu.carcontrol.common.util.Constants;
import com.bihu.carcontrol.common.util.DeviceUtil;
import com.bihu.carcontrol.common.util.DialogUtil;
import com.bihu.carcontrol.common.util.FastClickCheckUtil;
import com.bihu.carcontrol.common.util.LogUtil;
import com.bihu.carcontrol.common.util.SPUtil;
import com.bihu.carcontrol.common.util.ToastMaster;
import com.bihu.carcontrol.common.widget.CustomChargingDateView;
import com.bihu.carcontrol.common.widget.MyPopupWindow;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.Timer;
import java.util.TimerTask;

/**
 * FileName: EnergyManageFragment
 * Author: WangXu
 * Date: 2024/6/6 10:43
 * Description: 能量管理
 */
public class EnergyManageFragment extends BaseFragment implements View.OnClickListener {
    private TextView tvSingle, tvLoop;
    private final List<TextView> mMileageTvs = new ArrayList<>();
    private final List<TextView> mChargeModeTvs = new ArrayList<>();
    private TextView tvSeekProgressChargingCurrent, tvSeekProgressTarget, tvStartHour, tvStartMin, tvEndHour, tvEndMin, tvTomorrow, tvEditSave, tvImmediateCharge, tvReservationCharging;
    private SeekBar seekBarChargingCurrent, seekBarTarget;
    private LinearLayout llStartTime, llEndTime, llChargingContent, llChargingMode;
    private RelativeLayout rlChargingInfo;
    private CustomChargingDateView customChargingDate;
    private ImageView ivChargingCurrentTips;
    private ChargingTimeSelectDialog chargingStartTimeSelectDialog;
    private ChargingTimeSelectDialog chargingEndTimeSelectDialog;
    //编辑状态
    private boolean isChargingEdit = false;
    //预约充电模式  立即充电/预约充电
    private boolean mNow = true;
    //预约充电方式  单次/循环
    private boolean mSingle = true;
    private Dialog targetDialog;
    private int chargingCurrentInt;
    private int target;
    private int rangeModeInt;
    private int startHour;
    private int startMin;
    private int endHour;
    private int endMin;
    private int lastClickViewId = 1;
    private boolean chargingCurrentClick = false;
    private boolean targetClick = false;
    private boolean chargingModeClick = false;

    @Override
    protected int getContentViewId() {
        return R.layout.fragment_energy_manage;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EventBus.getDefault().register(this);
    }

    @Override
    protected void initView(View view) {
        TextView tvChtc = view.findViewById(R.id.tv_chtc);
        TextView tvWltp = view.findViewById(R.id.tv_wltp);
        mMileageTvs.add(tvChtc);
        mMileageTvs.add(tvWltp);

        tvImmediateCharge = view.findViewById(R.id.tv_immediate_charge);
        tvReservationCharging = view.findViewById(R.id.tv_reservation_charging);
        mChargeModeTvs.add(tvImmediateCharge);
        mChargeModeTvs.add(tvReservationCharging);

        tvSingle = view.findViewById(R.id.tv_single);
        tvLoop = view.findViewById(R.id.tv_loop);
        customChargingDate = view.findViewById(R.id.cus_charging_date);

        llChargingMode = view.findViewById(R.id.ll_charging_mode);
        rlChargingInfo = view.findViewById(R.id.rl_charging_info);
        llChargingContent = view.findViewById(R.id.ll_charging_content);
        seekBarChargingCurrent = view.findViewById(R.id.seek_bar_charging_current);
        ivChargingCurrentTips = view.findViewById(R.id.iv_charging_current_tips);
        tvSeekProgressChargingCurrent = view.findViewById(R.id.tv_seek_progress_charging_current);
        llStartTime = view.findViewById(R.id.ll_start_time);
        tvStartHour = view.findViewById(R.id.tv_start_hour);
        tvStartMin = view.findViewById(R.id.tv_start_min);
        llEndTime = view.findViewById(R.id.ll_end_time);
        tvEndHour = view.findViewById(R.id.tv_end_hour);
        tvEndMin = view.findViewById(R.id.tv_end_min);
        tvTomorrow = view.findViewById(R.id.tv_tomorrow);
        tvEditSave = view.findViewById(R.id.tv_edit_save);

        tvChtc.setOnClickListener(this);
        tvWltp.setOnClickListener(this);
        tvImmediateCharge.setOnClickListener(this);
        tvReservationCharging.setOnClickListener(this);
        tvSingle.setOnClickListener(this);
        tvLoop.setOnClickListener(this);
        ivChargingCurrentTips.setOnClickListener(this);
        llStartTime.setOnClickListener(this);
        llEndTime.setOnClickListener(this);
        tvEditSave.setOnClickListener(this);
        seekBarTarget = view.findViewById(R.id.seek_bar_target);
        tvSeekProgressTarget = view.findViewById(R.id.tv_seek_progress_target);

        NestedScrollView scrollView = view.findViewById(R.id.nested_scroll_view);
        scrollView.setOnScrollChangeListener((View.OnScrollChangeListener) (v, scrollX, scrollY, oldScrollX, oldScrollY) -> MyPopupWindow.getInstant(getContext()).dismissPopupWindow());

        initData();
    }

    @Override
    public void onResume() {
        super.onResume();
        new Thread(() -> {
            chargingCurrentInt = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.GECKO_VCU_ACCHRGMAXCURR);
            LogUtil.d("获取充电电流限制：" + chargingCurrentInt + "A");
            target = CarVehicleManager.getInstance().requestGetChargingSetting(VehicleType.GECKO_CCM_CHRAGING_TARGET_SOC);
            LogUtil.d("获取目标电量： " + target + "%");

            seekBarChargingCurrent.post(this::refreshSeekAndRangeModeUI);
        }).start();
    }

    private void initData() {
        new Thread(() -> {
            //获取上次设置的续航里程模式onCreate再次下设 0 CHTC 1 WLTP
            rangeModeInt = SPUtil.getInt(Objects.requireNonNull(getContext()), SPUtil.KEY_RANGE_DISPLAY_MODE, 0);
            LogUtil.d("初次进入获取应用本地续航里程模式下设一次（CarSdk要求）" + "模式为： " + (rangeModeInt == 0 ? "CHTC" : "WLTP"));
            setRangeDisplayMode(rangeModeInt);

            initChargingMode();
            initChargingCircle();
            customChargingDate.restore();
            customChargingDate.setEditState(isChargingEdit);
            initChargingTime();
        }).start();

        seekBarChargingCurrent.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                tvSeekProgressChargingCurrent.setText(String.format("%sA", progress));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                int progress = seekBar.getProgress();
                chargingCurrentClick = true;
                //充电电流限制
                CarVehicleManager.getInstance().requestSetIntPropertyAsRoot(VehicleType.GECKO_VCU_ACCHRGMAXCURR, progress);
                LogUtil.d("设置充电电流限制： " + progress);

                seekBarChargingCurrent.setClickable(false);
                seekBarChargingCurrent.setEnabled(false);
                seekBarChargingCurrent.setFocusable(false);
//                setSeekBarUnClick(seekBarChargingCurrent, false);
                cancelChargingCurrentTimeOut();
                postChargingCurrentTimeOutDelayed();
            }
        });

        seekBarTarget.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                tvSeekProgressTarget.setText(String.format("%s%%", progress));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                int progress = seekBar.getProgress();
                if (CarVehicleManager.getInstance().getBatteryInfo() == null) {
                    LogUtil.d("获取预约信息异常，电池剩余电量为空，无法设置目标电量");
                    return;
                }
                int batteryLevel = (int) CarVehicleManager.getInstance().getBatteryInfo().getBatteryLevel();
                LogUtil.d("获取当前电池剩余电量： " + batteryLevel);
                if (progress <= batteryLevel) {//目标电量小于当前电池剩余电量
                    View view = LayoutInflater.from(getActivity()).inflate(R.layout.dialog_target, (ViewGroup) null);
                    if (targetDialog == null) {
                        targetDialog = DialogUtil.getDialog(getActivity(), view, 0.36);
                        targetDialog.setCanceledOnTouchOutside(false);
                    }
                    if (!targetDialog.isShowing()) {
                        targetDialog.show();
                    }
                    view.findViewById(R.id.tv_confirm).setOnClickListener(v -> {
                        targetClick = true;
                        CarVehicleManager.getInstance().updateChargingSetting(VehicleType.GECKO_CCM_CHRAGING_TARGET_SOC, seekBar.getProgress());
                        LogUtil.d("设置预约充电目标电量: " + seekBar.getProgress());
                        seekBarTarget.setClickable(false);
                        seekBarTarget.setEnabled(false);
                        seekBarTarget.setFocusable(false);
//                        setSeekBarUnClick(seekBarTarget,false);
                        cancelTargetTimeOut();
                        postTargetTimeOutDelayed();
                        if (targetDialog.isShowing()) {
                            targetDialog.dismiss();
                        }
                    });
                    view.findViewById(R.id.tv_cancel).setOnClickListener(v -> {//取消弹框，回到当前目标电量
                        int target = CarVehicleManager.getInstance().requestGetChargingSetting(VehicleType.GECKO_CCM_CHRAGING_TARGET_SOC);
                        LogUtil.d("取消弹框，回到当前目标电量: " + target);
                        seekBarTarget.setProgress(target);
                        if (targetDialog.isShowing()) {
                            targetDialog.dismiss();
                        }
                    });
                } else {
                    targetClick = true;
                    CarVehicleManager.getInstance().updateChargingSetting(VehicleType.GECKO_CCM_CHRAGING_TARGET_SOC, progress);
                    LogUtil.d("设置预约充电目标电量: " + progress);
                    seekBarTarget.setClickable(false);
                    seekBarTarget.setEnabled(false);
                    seekBarTarget.setFocusable(false);
//                    setSeekBarUnClick(seekBarTarget, false);
                    cancelTargetTimeOut();
                    postTargetTimeOutDelayed();
                }
            }
        });
    }

    private Timer chargingCurrentTimeOutTimer;
    private TimerTask chargingCurrentTimeOutTask;

    //充电电流限制设置超时
    private void postChargingCurrentTimeOutDelayed() {
        chargingCurrentTimeOutTimer = new Timer();
        chargingCurrentTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                seekBarChargingCurrent.post(() -> {
                    showSetupFailureToast();
                    seekBarChargingCurrent.setClickable(true);
                    seekBarChargingCurrent.setEnabled(true);
                    seekBarChargingCurrent.setFocusable(true);
//                    setSeekBarUnClick(seekBarChargingCurrent, true);
                    chargingCurrentClick = false;
                    LogUtil.d("设置失败，设置超时,获取目标电量显示UI: " + target);
                });
            }
        };
        chargingCurrentTimeOutTimer.schedule(chargingCurrentTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelChargingCurrentTimeOut() {
        if (chargingCurrentTimeOutTimer != null) {
            chargingCurrentTimeOutTimer.cancel();
            chargingCurrentTimeOutTimer = null;
        }
        if (chargingCurrentTimeOutTask != null) {
            chargingCurrentTimeOutTask.cancel();
            chargingCurrentTimeOutTask = null;
        }
    }

    private Timer targetTimeOutTimer;
    private TimerTask targetTimeOutTask;

    //目标电量设置超时
    private void postTargetTimeOutDelayed() {
        targetTimeOutTimer = new Timer();
        targetTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                seekBarTarget.post(() -> {
                    showSetupFailureToast();
                    seekBarTarget.setClickable(true);
                    seekBarTarget.setEnabled(true);
                    seekBarTarget.setFocusable(true);
//                    setSeekBarUnClick(seekBarTarget,true);
                    targetClick = false;
                    int target = CarVehicleManager.getInstance().requestGetChargingSetting(VehicleType.GECKO_CCM_CHRAGING_TARGET_SOC);
                    seekBarTarget.setProgress(target);
                    LogUtil.d("设置失败，设置超时,获取目标电量显示UI: " + target);
                });
            }
        };
        targetTimeOutTimer.schedule(targetTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void showSetupFailureToast() {
        ToastMaster.shortToast(EnergyManageFragment.this, getString(R.string.setup_failure));
    }

    private void cancelTargetTimeOut() {
        if (targetTimeOutTimer != null) {
            targetTimeOutTimer.cancel();
            targetTimeOutTimer = null;
        }
        if (targetTimeOutTask != null) {
            targetTimeOutTask.cancel();
            targetTimeOutTask = null;
        }
    }

    @SuppressLint("ClickableViewAccessibility")
    private void setSeekBarUnClick(SeekBar seekBar, boolean clickable) {
        seekBar.setOnTouchListener((v, event) -> !clickable);
    }

    private void refreshSeekAndRangeModeUI() {
        freshChargingCurrentSeekBarUi();//充电电流限制
        freshTargetSeekBarUI();//目标电量
        // 0 CHTC 1 WLTP
        refreshRangeModeUI(rangeModeInt);
    }

    private void freshChargingCurrentSeekBarUi() {
        if (chargingCurrentInt == Constants.ERROR_CODE) {
            seekBarChargingCurrent.setProgress(1);
            tvSeekProgressChargingCurrent.setVisibility(View.GONE);
            seekBarChargingCurrent.setClickable(false);
            seekBarChargingCurrent.setEnabled(false);
            seekBarChargingCurrent.setFocusable(false);
//            setSeekBarUnClick(seekBarChargingCurrent,false);
        } else if (chargingCurrentInt != -1) {
            if (chargingCurrentInt == 0) {//为0时为异常值，显示但是不能设置
                seekBarChargingCurrent.setClickable(false);
                seekBarChargingCurrent.setEnabled(false);
                seekBarChargingCurrent.setFocusable(false);
//                setSeekBarUnClick(seekBarChargingCurrent,false);
            } else {
                seekBarChargingCurrent.setClickable(true);
                seekBarChargingCurrent.setEnabled(true);
                seekBarChargingCurrent.setFocusable(true);
//                setSeekBarUnClick(seekBarChargingCurrent,true);
            }
            seekBarChargingCurrent.setProgress(chargingCurrentInt);
            tvSeekProgressChargingCurrent.setVisibility(View.VISIBLE);
            tvSeekProgressChargingCurrent.setText(String.format("%sA", chargingCurrentInt));
        }
    }

    private void freshTargetSeekBarUI() {
        if (target == Constants.ERROR_CODE) {//信号丢失
            seekBarTarget.setProgress(0);
            tvSeekProgressTarget.setVisibility(View.GONE);
            seekBarTarget.setClickable(false);
            seekBarTarget.setEnabled(false);
            seekBarTarget.setFocusable(false);
//            setSeekBarUnClick(seekBarTarget,false);
        } else {
            seekBarTarget.setProgress(target);
            tvSeekProgressTarget.setVisibility(View.VISIBLE);
            tvSeekProgressTarget.setText(String.format("%s%%", target));
            if (target < 50) {//目标电量小于50为异常值，显示，但是不能设置
                seekBarTarget.setClickable(false);
                seekBarTarget.setEnabled(false);
                seekBarTarget.setFocusable(false);
//                setSeekBarUnClick(seekBarTarget,false);
            } else {
                seekBarTarget.setClickable(true);
                seekBarTarget.setEnabled(true);
                seekBarTarget.setFocusable(true);
//                setSeekBarUnClick(seekBarTarget,true);
            }
        }
    }

    private void initChargingMode() {
        int value = CarVehicleManager.getInstance().requestGetChargingSetting(VehicleType.GECKO_CHRAGINGMODESP);
        LogUtil.d("获取预约充电模式: " + value);
        //0x0: invalid 0x1: 立即充电 0x2: 预约充电开启 0x3: Reserved
        if (value == 2) {//预约充电
            mNow = false;
            LogUtil.d("获取充电模式：预约充电");
        } else {//立即充电
            mNow = true;
            LogUtil.d("获取充电模式：立即充电");
        }
        rlChargingInfo.post(() -> {
            refreshChargingModeUI(value);
        });
    }

    public void initChargingCircle() {
        int chargingCircle = CarVehicleManager.getInstance().requestGetChargingSetting(VehicleType.GECKO_CCM_APPOINTMENT_MODE);
        LogUtil.d("获取预约充电方式： " + chargingCircle);
        if (chargingCircle == 0) {
            mSingle = true;
            LogUtil.d("获取充电方式：单次");
        } else {
            mSingle = false;
            LogUtil.d("获取充电方式：循环");
        }
        tvSingle.post(this::refreshSingleLoopUi);
    }

    public void initChargingTime() {
        startHour = CarVehicleManager.getInstance().requestGetChargingSetting(VehicleType.GECKO_CCM_CHRAGING_BEGIN_TIME_HOUR);
        startMin = CarVehicleManager.getInstance().requestGetChargingSetting(VehicleType.GECKO_CCM_CHRAGING_BEGIN_TIME_MINUTE);
        LogUtil.d("获取预约充电开始时间：" + startHour + "时" + startMin + "分");
        if (startHour < 0 | startHour > 23) {//mcu可能出现异常值的处理
            startHour = 0;
            LogUtil.d("获取预约充电时间： 小时异常 ------" + startMin);
        }
        if (startMin < 0 | startMin > 59) {//mcu可能出现异常值的处理
            LogUtil.d("获取预约充电时间： 分钟异常 -----" + startMin);
            startMin = 0;
        }
        endHour = CarVehicleManager.getInstance().requestGetChargingSetting(VehicleType.GECKO_CCM_CHRAGING_END_TIME_HOUR);
        endMin = CarVehicleManager.getInstance().requestGetChargingSetting(VehicleType.GECKO_CCM_CHRAGING_END_TIME_MINUTE);
        LogUtil.d("获取预约充电结束时间：" + endHour + "时" + endMin + "分");
        if (endHour < 0 | endHour > 23) {//mcu可能出现异常值的处理
            endHour = 0;
            LogUtil.d("获取预约充电结束时间： 小时异常 ------" + endHour);
        }
        if (endMin < 0 | endMin > 59) {//mcu可能出现异常值的处理
            LogUtil.d("获取预约充电结束时间： 分钟异常 -----" + endMin);
            endMin = 0;
        }
        tvStartHour.post(() -> {
            tvStartHour.setText(startHour < 10 ? "0" + startHour : String.valueOf(startHour));
            tvStartMin.setText(startMin < 10 ? "0" + startMin : String.valueOf(startMin));
            tvEndHour.setText(endHour < 10 ? "0" + endHour : String.valueOf(endHour));
            tvEndMin.setText(endMin < 10 ? "0" + endMin : String.valueOf(endMin));
            ifTomorrowTvShow();
        });
    }

    //次日UI是否显示
    private void ifTomorrowTvShow() {
        int startHour = Integer.parseInt(tvStartHour.getText().toString());
        int startMin = Integer.parseInt(tvStartMin.getText().toString());
        int endHour = Integer.parseInt(tvEndHour.getText().toString());
        int endMin = Integer.parseInt(tvEndMin.getText().toString());
        if (startHour > endHour | ((startHour == endHour) && startMin >= endMin)) {
            tvTomorrow.setVisibility(View.VISIBLE);
        } else {
            tvTomorrow.setVisibility(View.GONE);
        }
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onHandleEvent(VehicleMessageEventBean eventBean) {
        int code = eventBean.getCode();
        if (EventBusActionCode.RANGE_DISPLAY_MODE_CHANGE_CODE == code && eventBean.getCmd() != null) {//续航里程回调
            int drivingSelectMode = (int) eventBean.getCmd();
            refreshRangeModeUI(drivingSelectMode);
            rangeModeInt = drivingSelectMode;
            SPUtil.putInt(Objects.requireNonNull(getContext()), SPUtil.KEY_RANGE_DISPLAY_MODE, drivingSelectMode);
            SystemProperties.set("persist.gecko.rangemode", String.valueOf(drivingSelectMode));
        } else if (code == EventBusActionCode.AC_CHARGING_MAX_CURR) {//充电电流限制
            int cmd = (int) eventBean.getCmd();
            if (chargingCurrentClick && cmd != Constants.ERROR_CODE && (cmd != seekBarChargingCurrent.getProgress())) {//下设值和返回值不对应
                showSetupFailureToast();
                LogUtil.d("充电电流限制下设值和返回值不对应下设值: " + seekBarChargingCurrent.getProgress() + "  反馈值：" + cmd);
            }
            chargingCurrentClick = false;
            chargingCurrentInt = cmd;
            freshChargingCurrentSeekBarUi();

            cancelChargingCurrentTimeOut();
        } else if (code == VehicleType.GECKO_CHRAGINGMODESP) {//预约充电模式 270795063
            int cmd = (int) eventBean.getCmd();
            LogUtil.d("回调 预约充电模式: " + cmd);
            if (chargingModeClick && cmd != Constants.ERROR_CODE && (cmd == 1) != mNow) {
                showSetupFailureToast();
                LogUtil.d("预约充电模式下设值和返回值不对应下设值: " + (mNow ? "立即充电" : "预约充电") + "  反馈值：" + (cmd == 1 ? "立即充电" : "预约充电"));
            }
            chargingModeClick = false;
            cancelChargingModeTimeOut();
            // 回调 0x0: invalid 0x1: 立即充电 0x2: 预约充电开启 0x3: Reserved 修改时间：20250513
            this.mNow = (cmd != 2);
            LogUtil.d("回调 预约充电模式: " + (cmd != 2 ? "立即充电" : "预约充电"));
            if (cmd == 1) {//20250509 修改bug9932 下设和回调不对称过滤点击的问题
                lastClickViewId = R.id.tv_immediate_charge;
            } else if (cmd == 2) {
                lastClickViewId = R.id.tv_reservation_charging;
            } else if (cmd == Constants.ERROR_CODE) {
                lastClickViewId = 1;
            }
            refreshChargingModeUI(cmd);
        } else if (code == VehicleType.GECKO_CCM_APPOINTMENT_MODE) {//预约模式（预约充电方式）00：单次，01：循环 270795056
            int cmd = (int) eventBean.getCmd();
            this.mSingle = (cmd == 0);
            refreshSingleLoopUi();
            LogUtil.d("回调 预约充电方式: " + (cmd == 0 ? "单次" : "循环"));
            LogUtil.d("回调 预约充电方式: " + cmd);
        } else if (code == VehicleType.GECKO_CCM_CHRAGING_BEGIN_TIME_HOUR) {// 开始时间（时）范围：00 ~ 23 270795057
            int cmd = (int) eventBean.getCmd();
            tvStartHour.setText(cmd < 10 ? "0" + cmd : String.valueOf(cmd));
            LogUtil.d("回调 开始时间（时）范围: " + cmd);
            ifTomorrowTvShow();
        } else if (code == VehicleType.GECKO_CCM_CHRAGING_BEGIN_TIME_MINUTE) {// 开始时间（分）范围：00 ~ 59 270795058
            int cmd = (int) eventBean.getCmd();
            tvStartMin.setText(cmd < 10 ? "0" + cmd : String.valueOf(cmd));
            LogUtil.d("回调 开始时间（分）范围: " + cmd);
            ifTomorrowTvShow();
        } else if (code == VehicleType.GECKO_CCM_CHRAGING_END_TIME_HOUR) {// 结束时间（时）	范围：00 ~ 23  270795059
            int cmd = (int) eventBean.getCmd();
            tvEndHour.setText(cmd < 10 ? "0" + cmd : String.valueOf(cmd));
            LogUtil.d("回调 结束时间（时）: " + cmd);
            ifTomorrowTvShow();
        } else if (code == VehicleType.GECKO_CCM_CHRAGING_END_TIME_MINUTE) {// 结束时间（分）	范围：00 ~ 59 270795060
            int cmd = (int) eventBean.getCmd();
            tvEndMin.setText(cmd < 10 ? "0" + cmd : String.valueOf(cmd));
            LogUtil.d("回调 结束时间（分）: " + cmd);
            ifTomorrowTvShow();
        } else if (code == VehicleType.GECKO_CCM_CHRAGING_TARGET_SOC) {//充电目标电量	"范围：50 ~ 100" 270795062
            int cmd = (int) eventBean.getCmd();
            if (targetClick && cmd != Constants.ERROR_CODE && (cmd != seekBarTarget.getProgress())) {//下设值和返回值不对应
                showSetupFailureToast();
                LogUtil.d("充电目标电量下设值和返回值不对应下设值: " + seekBarTarget.getProgress() + "  反馈值：" + cmd);
            }
            targetClick = false;
            target = cmd;
            freshTargetSeekBarUI();
            LogUtil.d("回调 充电目标电量: " + cmd);
            cancelTargetTimeOut();
        } else if (code == EventBusActionCode.CHARGING_LOOP_DATA_CHANGE_CODE) {//充电开始日期  102
            customChargingDate.restore();
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        EventBus.getDefault().unregister(this);
    }

    @Override
    public void onClick(View v) {
        int id = v.getId();
        if (id == R.id.iv_charging_current_tips) {
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivChargingCurrentTips, getString(R.string.charging_current_tips));
            }
            return;
        }
        if (DeviceUtil.ifPowerOff()) {
            return;
        }

        //预约信息点击不做频繁点击限制（点击时未下设）
        if (id == R.id.ll_start_time) {//开始时间
            if (isChargingEdit) {
                showStartTimeDialog();
            }
            return;
        } else if (id == R.id.ll_end_time) {//结束时间
            if (isChargingEdit) {
                showEndTimeDialog();
            }
            return;
        } else if (id == R.id.tv_single) {//单次
            if (isChargingEdit) {
                mSingle = true;
                refreshSingleLoopUi();
            }
            return;
        } else if (id == R.id.tv_loop) {//循环
            if (isChargingEdit) {
                mSingle = false;
                refreshSingleLoopUi();
            }
            return;
        }

        if (lastClickViewId == R.id.tv_reservation_charging && id == R.id.tv_reservation_charging) {
            LogUtil.d("上次点击是预约充电， 再次点击不提示频繁点击且不下设");
            return;
        }
        if (lastClickViewId == R.id.tv_immediate_charge && id == R.id.tv_immediate_charge) {
            LogUtil.d("上次点击是立即充电， 再次点击不提示频繁点击且不下设");
            return;
        }
        if (FastClickCheckUtil.isFastClick(500)) {
            return;
        }
        lastClickViewId = id;//记录上次点击的id，用做下次是立即充电或者预约充电按钮点击判断，不弹toast提示
        if (id == R.id.tv_chtc) {//CHTC
            if (rangeModeInt != 0) {
                setRangeDisplayMode(0);
            }
        } else if (id == R.id.tv_wltp) {//WLTP
            if (rangeModeInt != 1) {
                setRangeDisplayMode(1);
            }
        } else if (id == R.id.tv_immediate_charge) {//立即充电
            mNow = true;
            chargingModeClick = true;
            setChargingMode();
            refreshChargingModeUI(1);
            cancelChargingModeTimeOut();
            postChargingModeTimeOutDelayed();
        } else if (id == R.id.tv_reservation_charging) {//预约充电
            mNow = false;
            chargingModeClick = true;
            setChargingMode();
            if (customChargingDate.showWarning()) {
                LogUtil.d("预约充电日期没有选中任何一天需要toast提示");
                ToastMaster.shortToast(getString(R.string.please_set_charging_date));
            }
            refreshChargingModeUI(2);
            cancelChargingModeTimeOut();
            postChargingModeTimeOutDelayed();
        } else if (id == R.id.tv_edit_save) {
            if (isChargingEdit) {//保存
                if (customChargingDate.showWarning()) {
                    ToastMaster.shortToast("请至少选择一天");
                    return;
                }
                llChargingContent.setAlpha(0.6f);
                isChargingEdit = false;
                tvEditSave.setText(getString(R.string.edit));
                customChargingDate.setEditState(false);
                submit();
                LogUtil.d("xxx: ", "   tvStartHour   " + Integer.valueOf(tvStartHour.getText().toString()) + "   tvStartMin   " + Integer.valueOf(tvStartMin.getText().toString()));
                LogUtil.d("xxx: ", "   tvEndHour   " + Integer.valueOf(tvEndHour.getText().toString()) + "   tvEndMin   " + Integer.valueOf(tvEndMin.getText().toString()));
            } else {//编辑中
                llChargingContent.setAlpha(1f);
                isChargingEdit = true;
                customChargingDate.setEditState(true);
                tvEditSave.setText(getString(R.string.save));
            }
        }
    }

    private boolean isCanShowPopup() {
        return (isAdded() && !isDetached() && getActivity() != null && !getActivity().isFinishing() && !getActivity().isDestroyed());
    }

    private void refreshSingleLoopUi() {
        if (mSingle) {//单次
            tvSingle.setBackgroundResource(R.drawable.bg_segmented_btn);
            tvSingle.setTextColor(ContextCompat.getColor(Objects.requireNonNull(getContext()), R.color.white));
            tvLoop.setBackground(null);
            tvLoop.setTextColor(ContextCompat.getColor(Objects.requireNonNull(getContext()), R.color.tab_unselect_text_color));
            customChargingDate.setSingleSelect(true, true);
        } else {
            tvSingle.setBackground(null);
            tvSingle.setTextColor(ContextCompat.getColor(Objects.requireNonNull(getContext()), R.color.tab_unselect_text_color));
            tvLoop.setBackgroundResource(R.drawable.bg_segmented_btn);
            tvLoop.setTextColor(ContextCompat.getColor(Objects.requireNonNull(getContext()), R.color.white));
            customChargingDate.setSingleSelect(false, false);
        }
    }

    private void submit() {
        setChargingCircle();
        if (!mNow) {//预约充电
            customChargingDate.save();//预约充电星期列表设置
            CarVehicleManager.getInstance().updateChargingSetting(VehicleType.GECKO_CCM_CHRAGING_BEGIN_TIME_HOUR, Integer.parseInt(tvStartHour.getText().toString()));
            LogUtil.d("设置预约充电开始时间(时): " + Integer.parseInt(tvStartHour.getText().toString()));
            CarVehicleManager.getInstance().updateChargingSetting(VehicleType.GECKO_CCM_CHRAGING_BEGIN_TIME_MINUTE, Integer.parseInt(tvStartMin.getText().toString()));
            LogUtil.d("设置预约充电开始时间(分): " + Integer.parseInt(tvStartMin.getText().toString()));
            CarVehicleManager.getInstance().updateChargingSetting(VehicleType.GECKO_CCM_CHRAGING_END_TIME_HOUR, Integer.parseInt(tvEndHour.getText().toString()));
            LogUtil.d("设置预约充电结束时间(时): " + Integer.parseInt(tvEndHour.getText().toString()));
            CarVehicleManager.getInstance().updateChargingSetting(VehicleType.GECKO_CCM_CHRAGING_END_TIME_MINUTE, Integer.parseInt(tvEndMin.getText().toString()));
            LogUtil.d("设置预约充电结束时间(分): " + Integer.parseInt(tvEndMin.getText().toString()));
            //预约充电模式，延时100发，配合SOC存储数据的处理
            postDelayed(this::setChargingMode, 100);
        }
    }

    //设置预约充电模式
    private void setChargingMode() {//下设 0立即充电 1预约充电
        CarVehicleManager.getInstance().updateChargingSetting(VehicleType.GECKO_CHRAGINGMODESP, mNow ? 0 : 1);
        LogUtil.d("设置预约充电模式: " + (mNow ? "立即充电" : "预约充电"));
        LogUtil.d("设置预约充电模式: " + (mNow ? 0 : 1));
    }

    //设置预约充电方式
    private void setChargingCircle() {
        CarVehicleManager.getInstance().updateChargingSetting(VehicleType.GECKO_CCM_APPOINTMENT_MODE, mSingle ? 0 : 1);
        LogUtil.d("设置预约充电方式: " + (mSingle ? "单次" : "循环"));
        LogUtil.d("设置预约充电方式: " + (mSingle ? 0 : 1));
    }

    private void showStartTimeDialog() {
        if (chargingStartTimeSelectDialog == null) {
            chargingStartTimeSelectDialog = new ChargingTimeSelectDialog(Objects.requireNonNull(getActivity()), getString(R.string.charging_start), 0.40, (hourInt, minInt) -> {
                tvStartHour.setText(hourInt < 10 ? "0" + hourInt : String.valueOf(hourInt));
                tvStartMin.setText(minInt < 10 ? "0" + minInt : String.valueOf(minInt));
                ifTomorrowTvShow();
            });
        }
        chargingStartTimeSelectDialog.setHourMinValueInt(Integer.parseInt(tvStartHour.getText().toString()), Integer.parseInt(tvStartMin.getText().toString()));
        chargingStartTimeSelectDialog.showDialog();
    }

    private void showEndTimeDialog() {
        if (chargingEndTimeSelectDialog == null) {
            chargingEndTimeSelectDialog = new ChargingTimeSelectDialog(Objects.requireNonNull(getActivity()), getString(R.string.charging_end), 0.40, (hourInt, minInt) -> {
                tvEndHour.setText(hourInt < 10 ? "0" + hourInt : String.valueOf(hourInt));
                tvEndMin.setText(minInt < 10 ? "0" + minInt : String.valueOf(minInt));
                ifTomorrowTvShow();
            });
        }
        chargingEndTimeSelectDialog.setHourMinValueInt(Integer.parseInt(tvEndHour.getText().toString()), Integer.parseInt(tvEndMin.getText().toString()));
        chargingEndTimeSelectDialog.showDialog();
    }

    //设置续航里程模式
    public void setRangeDisplayMode(int mode) {
        LogUtil.d("续航里程下设mode： " + mode + "模式为： " + (mode == 0 ? "CHTC" : "WLTP"));
        CarVehicleManager.getInstance().setVehicleRangeDisplayMode(mode);
    }

    //续航里程模式UI刷新
    public void refreshRangeModeUI(int position) {
        for (int i = 0; i < mMileageTvs.size(); i++) {
            TextView textView = mMileageTvs.get(i);
            if (position == i) {
                textView.setBackgroundResource(R.drawable.bg_segmented_btn);
                textView.setTextColor(getResources().getColor(R.color.white));
            } else {
                textView.setBackground(null);
                textView.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            }
        }
    }

    //预约充电模式  0x0: invalid 0x1: 立即充电 0x2: 预约充电开启 0x3: Reserved  -1024信号丢失
    public void refreshChargingModeUI(int chargingMode) {
        if (chargingMode == 1) {
            rlChargingInfo.setVisibility(View.GONE);
            llChargingContent.setVisibility(View.GONE);
        } else if (chargingMode == 2) {
            rlChargingInfo.setVisibility(View.VISIBLE);
            llChargingContent.setVisibility(View.VISIBLE);
        } else {//信号丢失或者 0x0: invalid   0x3: Reserved 做置灰处理
            rlChargingInfo.setVisibility(View.GONE);
            llChargingContent.setVisibility(View.GONE);
            llChargingMode.setAlpha(Constants.ERROR_ALPHA);
            tvImmediateCharge.setClickable(false);
            tvReservationCharging.setClickable(false);
            return;
        }
        if (isAdded() && getContext() != null && llChargingMode.getAlpha() < 1.0f) {
            llChargingMode.setAlpha(1.0f);
        }
        tvImmediateCharge.setClickable(true);
        tvReservationCharging.setClickable(true);
        for (int i = 0; i < mChargeModeTvs.size(); i++) {
            TextView textView = mChargeModeTvs.get(i);
            if ((chargingMode - 1) == i) {
                textView.setBackgroundResource(R.drawable.bg_segmented_btn);
                textView.setTextColor(getResources().getColor(R.color.white));
            } else {
                textView.setBackground(null);
                textView.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            }
        }
    }

    private Timer chargingModeTimeOutTimer;
    private TimerTask chargingModeTimeOutTask;

    private void postChargingModeTimeOutDelayed() {
        chargingModeTimeOutTimer = new Timer();
        chargingModeTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                tvImmediateCharge.post(() -> {
                    LogUtil.d("设置失败，设置预约充电模式超时 ------ ");
                    initChargingMode();
                    chargingModeClick = false;
                    lastClickViewId = 1;//设置失败清除过滤点击
                    showSetupFailureToast();
                });
            }
        };
        chargingModeTimeOutTimer.schedule(chargingModeTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelChargingModeTimeOut() {
        if (chargingModeTimeOutTimer != null) {
            chargingModeTimeOutTimer.cancel();
            chargingModeTimeOutTimer = null;
        }
        if (chargingModeTimeOutTask != null) {
            chargingModeTimeOutTask.cancel();
            chargingModeTimeOutTask = null;
        }
    }
}
