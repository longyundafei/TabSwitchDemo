package com.bihu.carcontrol.module.fragment;

import android.app.Dialog;
import android.car.VehiclePropertyIds;
import android.os.Bundle;
import android.os.SystemProperties;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.android.car.client.manager.vehicle.CarVehicleManager;
import com.android.car.core.server.vehicle.VehicleType;
import com.bihu.carcontrol.R;
import com.bihu.carcontrol.base.fragment.BaseFragment;
import com.bihu.carcontrol.common.event.EventBusActionCode;
import com.bihu.carcontrol.common.event.VehicleMessageEventBean;
import com.bihu.carcontrol.common.util.Constants;
import com.bihu.carcontrol.common.util.DeviceUtil;
import com.bihu.carcontrol.common.util.DialogUtil;
import com.bihu.carcontrol.common.util.FastClickCheckUtil;
import com.bihu.carcontrol.common.util.LogUtil;
import com.bihu.carcontrol.common.util.ToastMaster;
import com.bihu.carcontrol.common.widget.MyPopupWindow;
import com.bihu.carcontrol.module.main.CarControlActivity;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

import java.util.Timer;
import java.util.TimerTask;

/**
 * FileName: SafetyFragment
 * Author: WangXu
 * Date: 2024/6/13 10:49
 * Description: 安全&维护
 */
public class SafetyFragment extends BaseFragment implements View.OnClickListener {
    private TextView tvVehiclePowerOff, tvTirePressureLearning, tvTirePressureReset, tvNormalMode, tvMaintainMode, tvTrailerMode;
    private boolean isVehiclePowerOff = true;
    private Dialog powerOffDialog;
    private Dialog powerOffFailDialog;
    private ImageView ivVehiclePowerOffTips, ivElectronicParkingModeTips, ivTirePressureLearningTips, ivTirePressureResetTips;
    private Dialog tirePressureResetDialog;
    private LinearLayout llParkingMode, llTirePressureLearningItem, llTirePressureResetItem;
    private int electronicParkingModeSet = -1;
    //电子驻车可以通过车辆的操作解除，vcu反馈值不对只判断车控点击下设之后的回调
    private boolean isElectronicParkingModeClick = false;
    //是否在车里下电设置的超时时间内，用作在下设到超时的1s内判断电源状态变化是否符合预期下电成功，不成功需要弹窗提示
    private boolean isPowerOffInTime = false;

    @Override
    protected int getContentViewId() {
        return R.layout.fragment_safety;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EventBus.getDefault().register(this);
    }

    @Override
    protected void initView(View view) {
        tvVehiclePowerOff = view.findViewById(R.id.tv_vehicle_power_off);
        ivVehiclePowerOffTips = view.findViewById(R.id.iv_power_off_tips);
        ivElectronicParkingModeTips = view.findViewById(R.id.iv_electronic_parking_mode_tips);
        tvTirePressureLearning = view.findViewById(R.id.tv_tire_pressure_learning);
        llTirePressureLearningItem = view.findViewById(R.id.ll_tire_pressure_learning_item);
        ivTirePressureLearningTips = view.findViewById(R.id.iv_tire_pressure_learning_tips);
        tvTirePressureReset = view.findViewById(R.id.tv_tire_pressure_reset);
        llTirePressureResetItem = view.findViewById(R.id.ll_tire_pressure_reset_item);
        ivTirePressureResetTips = view.findViewById(R.id.iv_tire_pressure_reset_tips);
        tvVehiclePowerOff.setOnClickListener(this);
        ivVehiclePowerOffTips.setOnClickListener(this);
        ivElectronicParkingModeTips.setOnClickListener(this);
        tvTirePressureLearning.setOnClickListener(this);
        ivTirePressureLearningTips.setOnClickListener(this);
        tvTirePressureReset.setOnClickListener(this);
        ivTirePressureResetTips.setOnClickListener(this);

        llParkingMode = view.findViewById(R.id.ll_parking_mode);
        tvNormalMode = view.findViewById(R.id.tv_normal_mode);
        tvMaintainMode = view.findViewById(R.id.tv_maintain_mode);
        tvTrailerMode = view.findViewById(R.id.tv_trailer_mode);
        tvNormalMode.setOnClickListener(this);
        tvMaintainMode.setOnClickListener(this);
        tvTrailerMode.setOnClickListener(this);

        //胎压学习2、胎压复位1 配置字字段
        String pressureStr = SystemProperties.get(Constants.PRESSURE_STR, "0x0");
        if (pressureStr.equals(Constants.PROPERTIES_SHOW)) {
            LogUtil.d("胎压复位配置字字段： 0x1  显示胎压复位");
            llTirePressureLearningItem.setVisibility(View.GONE);
            llTirePressureResetItem.setVisibility(View.VISIBLE);
        } else if (pressureStr.equals("0x2")) {
            LogUtil.d("胎压学习配置字字段： 0x2  显示胎压学习");
            llTirePressureLearningItem.setVisibility(View.VISIBLE);
            llTirePressureResetItem.setVisibility(View.GONE);
        } else {
            LogUtil.d("胎压学习、胎压复位配置字：  " + pressureStr + "  无效，都不显示");
            llTirePressureLearningItem.setVisibility(View.GONE);
            llTirePressureResetItem.setVisibility(View.GONE);
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        CarControlActivity activity = (CarControlActivity) getActivity();
        assert activity != null;
        if (activity.hasInit) {//SDKCar初始化未完成时不能请求数据，请求会导致中间层后面都不返回任何数据
            initData();
        }
    }

    private void initData() {
        //电子驻车模式 6 拖车模式  7/8 维修模式  非6/7/8为正常模式
        int electronicParkingModeInt = getElectronicParkingModeInt();
        refreshElectronicParkingModeView(electronicParkingModeInt);
        LogUtil.d("电子驻车模式： " + electronicParkingModeInt);
        //车辆下电开关
        int powerStateInt = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.BCM_POWER_STATE);
        refreshPowerStateView(powerStateInt);
        LogUtil.d("获取车辆下电值： " + powerStateInt);
        //胎压学习
        int tirePressureLearningInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_HU_TPMSLEARNINGREQ);
        refreshTirePressureLearningView(tirePressureLearningInt);
        LogUtil.d("获取胎压学习值： " + tirePressureLearningInt);
    }

    private int getElectronicParkingModeInt() {
        return CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.GECKO_ELECTRONIC_PARKING_MODE);
    }

    //车辆下电
    private void refreshPowerStateView(int powerStateInt) {
        //0 OFF 1ACC 2ON 3 STAR -1024信号丢失
        if (powerStateInt == Constants.ERROR_CODE) {
            isVehiclePowerOff = false;
            tvVehiclePowerOff.setAlpha(0.7f);
            tvVehiclePowerOff.setClickable(false);
        } else if (powerStateInt == 0) {
            isVehiclePowerOff = false;
            tvVehiclePowerOff.setAlpha(0.7f);
            if (!tvVehiclePowerOff.isClickable()) {
                tvVehiclePowerOff.setClickable(true);
            }
        } else {
            isVehiclePowerOff = true;
            tvVehiclePowerOff.setAlpha(1f);
            if (!tvVehiclePowerOff.isClickable()) {
                tvVehiclePowerOff.setClickable(true);
            }
        }
    }

    private void showSetupFailureToast() {
        ToastMaster.shortToast(SafetyFragment.this, getString(R.string.setup_failure));
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onHandleEvent(VehicleMessageEventBean event) {
        int code = event.getCode();
        //if (Debug) Log.d(TAG, "code = " + code + "cmd = " + cmd);
        if (code == EventBusActionCode.ELECTRONIC_PARKING_MODE) {//电子驻车模式
            int cmd = (int) event.getCmd();
            if (isElectronicParkingModeClick && ((electronicParkingModeSet == 1 && (cmd != 6 && cmd != 4)) || (electronicParkingModeSet == 2 && (cmd != 7 && cmd != 8)) || (electronicParkingModeSet == 0 && (cmd == 6 || cmd == 7 || cmd == 8)))) {
                showSetupFailureToast();
                LogUtil.d("电子驻车模式下设值和返回值不对应下设值： " + electronicParkingModeSet + "    返回值： " + cmd);
            }
            cancelElectronicParkingModeTimeOut();
            refreshElectronicParkingModeView(cmd);
            isElectronicParkingModeClick = false;
        } else if (code == EventBusActionCode.POWER_STATE) {//车辆下电
            int cmd = (int) event.getCmd();
            if (isPowerOffInTime && cmd != 0) {
                showPowerOffFailDialog();
            }
            isPowerOffInTime = false;
            tvVehiclePowerOff.setClickable(true);
            cancelPowerOffTimeOut();
            refreshPowerStateView(cmd);
        } else if (code == EventBusActionCode.TPMS_LEARNING_REQ) {//胎压学习
            refreshTirePressureLearningView((int) event.getCmd());
        }
    }

    //胎压学习UI刷新
    private void refreshTirePressureLearningView(int tirePressureLearningInt) {
        if (tirePressureLearningInt == 2) {
            tvTirePressureLearning.setClickable(false);
            tvTirePressureLearning.setAlpha(0.7f);
            tvTirePressureLearning.setText(getString(R.string.learning));
        } else {
            tvTirePressureLearning.setClickable(true);
            tvTirePressureLearning.setAlpha(1f);
            tvTirePressureLearning.setText(getString(R.string.tire_pressure_learning));
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
        if (id == R.id.iv_power_off_tips) {//车辆下电tips
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivVehiclePowerOffTips, getString(R.string.vehicle_power_off_tips));
            }
        } else if (id == R.id.iv_electronic_parking_mode_tips) {//电子驻车模式tips
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivElectronicParkingModeTips, getString(R.string.electronic_parking_mode_tips));
            }
        } else if (id == R.id.iv_tire_pressure_learning_tips) {//胎压学习tips
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivTirePressureLearningTips, getString(R.string.tire_pressure_learning_tips));
            }
        } else if (id == R.id.iv_tire_pressure_reset_tips) {//胎压复位tips
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivTirePressureResetTips, getString(R.string.tire_pressure_reset_tips));
            }
        } else {
            if (DeviceUtil.ifPowerOff() || FastClickCheckUtil.isFastClick(500)) {
                return;
            }
            if (id == R.id.tv_vehicle_power_off) {//车辆下电
                if (isVehiclePowerOff) {//车辆为上电状态1ACC 2ON 3 START才可以下电
                    showPowerOffDialog();
                }
            } else if (id == R.id.tv_tire_pressure_learning) {//胎压学习
                CarVehicleManager.getInstance().setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_HU_TPMSLEARNINGREQ, 1);
                LogUtil.d("胎压学习设置： 1 开始学习");
            } else if (id == R.id.tv_normal_mode) {//电子驻车模式 正常模式
                if (getElectronicParkingModeInt() == 6 || getElectronicParkingModeInt() == 7 || getElectronicParkingModeInt() == 8) {
                    if (!isBrakePedalDepressed()) {
                        ToastMaster.shortToast(getString(R.string.pressed_first));
                        return;
                    }
                    //6 拖车模式  7/8 维修模式  非6/7/8为正常模式
                    refreshElectronicParkingModeView(1);
                    LogUtil.d("电子驻车设置： 正常模式 0x0 ");
                    setElectronicParkingMode(0);
                }
            } else if (id == R.id.tv_maintain_mode) {//电子驻车模式 维修模式
                if (getElectronicParkingModeInt() != 7 && getElectronicParkingModeInt() != 8) {
                    if (!isBrakePedalDepressed()) {
                        ToastMaster.shortToast(getString(R.string.pressed_first));
                        return;
                    }
                    refreshElectronicParkingModeView(7);
                    LogUtil.d("电子驻车设置： 维修模式 0x2");
                    setElectronicParkingMode(2);
                }
            } else if (id == R.id.tv_trailer_mode) {//电子驻车模式 拖车模式
                if (getElectronicParkingModeInt() != 6) {
                    if (!isBrakePedalDepressed()) {
                        ToastMaster.shortToast(getString(R.string.pressed_first));
                        return;
                    }
                    refreshElectronicParkingModeView(6);
                    LogUtil.d("电子驻车设置：拖车模式 0x1 ");
                    setElectronicParkingMode(1);
                }
            } else if (id == R.id.tv_tire_pressure_reset) {//胎压复位(无需回调)
                showTirePressureResetDialog();
            }
        }
    }

    private boolean isCanShowPopup() {
        return (isAdded() && !isDetached() && getActivity() != null && !getActivity().isFinishing() && !getActivity().isDestroyed());
    }

    //制动踏板是否踩下 brakePedalApplied == 1 && brakePedalAppliedQ == 1 (制动踏板踩下)
    private boolean isBrakePedalDepressed() {
        int brakePedalApplied = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_EHB_BRAKE_PEDAL_APPLIED);
        int brakePedalAppliedQ = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_EHB_BRAKE_PEDAL_APPLIED_Q);
        LogUtil.d("制动踏板是否踩下： " + (brakePedalApplied == 1 && brakePedalAppliedQ == 1) + "  brakePedalApplied:  " + brakePedalApplied + "   brakePedalAppliedQ: " + brakePedalAppliedQ);
        return brakePedalApplied == 1 && brakePedalAppliedQ == 1;
    }

    private void setElectronicParkingMode(int ElectronicParkingModeInt) {
        isElectronicParkingModeClick = true;
        //下设电子驻车 拖车模式 0x1 维修模式 0x2 正常模式 0x0
        setPropertyValue(VehicleType.GECKO_ELECTRONIC_PARKING_MODE, ElectronicParkingModeInt);
        electronicParkingModeSet = ElectronicParkingModeInt;
        cancelElectronicParkingModeTimeOut();
        postElectronicParkingModeTimeOutDelayed();
    }

    private void showPowerOffDialog() {
        View view = LayoutInflater.from(getActivity()).inflate(R.layout.dialog_power_off, (ViewGroup) null);
        if (powerOffDialog == null) {
            powerOffDialog = DialogUtil.getDialog(getActivity(), view, 0.36);
            powerOffDialog.setCanceledOnTouchOutside(false);
        }
        if (!powerOffDialog.isShowing() && powerOffDialog != null) {
            powerOffDialog.show();
        }

        view.findViewById(R.id.tv_confirm).setOnClickListener(v -> {
            setPropertyValue(VehicleType.BCM_POWER_STATE, 1);
            LogUtil.d("车辆下电设置： 值： 1");
            isPowerOffInTime = true;
            tvVehiclePowerOff.setClickable(false);
            cancelPowerOffTimeOut();
            postPowerOffTimeOutDelayed();
            hidePowerOffDialog();
        });

        view.findViewById(R.id.tv_cancel).setOnClickListener(v -> {
            hidePowerOffDialog();
        });
    }

    private void hidePowerOffDialog() {
        if (powerOffDialog != null) {
            powerOffDialog.dismiss();
        }
    }

    private void hidePowerOffFailDialog() {
        if (powerOffFailDialog != null) {
            powerOffFailDialog.dismiss();
        }
    }

    private void showPowerOffFailDialog() {
        View view = LayoutInflater.from(getActivity()).inflate(R.layout.dialog_power_off_fail, (ViewGroup) null);
        if (powerOffFailDialog == null) {
            powerOffFailDialog = DialogUtil.getDialog(getActivity(), view, 0.36);
            powerOffFailDialog.setCanceledOnTouchOutside(false);
        }
        if (!powerOffFailDialog.isShowing() && powerOffFailDialog != null) {
            powerOffFailDialog.show();
        }

        view.findViewById(R.id.tv_i_see).setOnClickListener(v -> {
            hidePowerOffFailDialog();
        });
    }

    private void showTirePressureResetDialog() {
        View view = LayoutInflater.from(getActivity()).inflate(R.layout.dialog_tire_pressure_reset, (ViewGroup) null);
        if (tirePressureResetDialog == null) {
            tirePressureResetDialog = DialogUtil.getDialog(getActivity(), view, 0.36);
            tirePressureResetDialog.setCanceledOnTouchOutside(false);
        }
        if (!tirePressureResetDialog.isShowing() && tirePressureResetDialog != null) {
            tirePressureResetDialog.show();
        }

        view.findViewById(R.id.tv_confirm).setOnClickListener(v -> {
            CarVehicleManager.getInstance().setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_HU_TPMSRESETREQ, 1);
            LogUtil.d("胎压复位设置： 1 复位");
            hideTirePressureResetDialog();
        });

        view.findViewById(R.id.tv_cancel).setOnClickListener(v -> {
            hideTirePressureResetDialog();
        });
    }

    private void hideTirePressureResetDialog() {
        if (tirePressureResetDialog != null) {
            tirePressureResetDialog.dismiss();
        }
    }

    //属性设置
    private void setPropertyValue(int key, int value) {
        CarVehicleManager.getInstance().requestSetIntPropertyAsRoot(key, value);
    }

    //电子驻车模式  6 拖车模式（有个4的过程状态）  7/8 维修模式  非6/7/8为正常模式
    public void refreshElectronicParkingModeView(int electronicParkingModeInt) {
        if (electronicParkingModeInt == Constants.ERROR_CODE) {
            llParkingMode.setAlpha(Constants.ERROR_ALPHA);
            tvTrailerMode.setClickable(false);
            tvNormalMode.setClickable(false);
            tvMaintainMode.setClickable(false);
        } else {
            if (electronicParkingModeInt != 4) {//4的状态为其他模式切换到拖车模式的过程状态，不要更新UI 20250421
                if (llParkingMode.getAlpha() < 1.0f) {
                    llParkingMode.setAlpha(1.0f);
                }
                if (!tvTrailerMode.isClickable()) {
                    tvTrailerMode.setClickable(true);
                }
                if (!tvNormalMode.isClickable()) {
                    tvNormalMode.setClickable(true);
                }
                if (!tvMaintainMode.isClickable()) {
                    tvMaintainMode.setClickable(true);
                }
                //6 拖车模式（有个4的过程状态）  7/8 维修模式  非6/7/8为正常模式
                if (electronicParkingModeInt == 6) {//拖车模式
                    tvTrailerMode.setTextColor(getResources().getColor(R.color.white));
                    tvTrailerMode.setBackgroundResource(R.drawable.bg_segmented_btn);
                    tvNormalMode.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
                    tvNormalMode.setBackground(null);
                    tvMaintainMode.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
                    tvMaintainMode.setBackground(null);
                } else if (electronicParkingModeInt == 8 || electronicParkingModeInt == 7) {//维修模式
                    tvMaintainMode.setTextColor(getResources().getColor(R.color.white));
                    tvMaintainMode.setBackgroundResource(R.drawable.bg_segmented_btn);
                    tvNormalMode.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
                    tvNormalMode.setBackground(null);
                    tvTrailerMode.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
                    tvTrailerMode.setBackground(null);
                } else {//正常模式
                    tvNormalMode.setTextColor(getResources().getColor(R.color.white));
                    tvNormalMode.setBackgroundResource(R.drawable.bg_segmented_btn);
                    tvMaintainMode.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
                    tvMaintainMode.setBackground(null);
                    tvTrailerMode.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
                    tvTrailerMode.setBackground(null);
                }
            }
        }
    }

    private Timer electronicParkingModeTimeOutTimer;
    private TimerTask electronicParkingModeTimeOutTask;

    private void postElectronicParkingModeTimeOutDelayed() {
        electronicParkingModeTimeOutTimer = new Timer();
        electronicParkingModeTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                llParkingMode.post(() -> {
                    showSetupFailureToast();
                    isElectronicParkingModeClick = false;
                    int electronicParkingModeInt = getElectronicParkingModeInt();
                    refreshElectronicParkingModeView(electronicParkingModeInt);
                    LogUtil.d("设置失败，设置电子驻车模式超时： " + electronicParkingModeInt + "  electronicParkingModeSet: " + electronicParkingModeSet);
                });
            }
        };
        electronicParkingModeTimeOutTimer.schedule(electronicParkingModeTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelElectronicParkingModeTimeOut() {
        if (electronicParkingModeTimeOutTimer != null) {
            electronicParkingModeTimeOutTimer.cancel();
            electronicParkingModeTimeOutTimer = null;
        }
        if (electronicParkingModeTimeOutTask != null) {
            electronicParkingModeTimeOutTask.cancel();
            electronicParkingModeTimeOutTask = null;
        }
    }

    private Timer powerOffTimeOutTimer;
    private TimerTask powerOffTimeOutTask;

    private void postPowerOffTimeOutDelayed() {
        powerOffTimeOutTimer = new Timer();
        powerOffTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                tvVehiclePowerOff.post(() -> {
                    LogUtil.d("车辆下电，设置超时");
                    isPowerOffInTime = false;
                    tvVehiclePowerOff.setClickable(true);
                    showPowerOffFailDialog();
                });
            }
        };
        powerOffTimeOutTimer.schedule(powerOffTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelPowerOffTimeOut() {
        if (powerOffTimeOutTimer != null) {
            powerOffTimeOutTimer.cancel();
            powerOffTimeOutTimer = null;
        }
        if (powerOffTimeOutTask != null) {
            powerOffTimeOutTask.cancel();
            powerOffTimeOutTask = null;
        }
    }
}
