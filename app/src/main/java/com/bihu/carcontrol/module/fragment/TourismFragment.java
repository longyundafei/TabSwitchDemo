package com.bihu.carcontrol.module.fragment;

import android.app.Dialog;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;

import com.android.car.client.manager.vehicle.CarVehicleManager;
import com.android.car.core.bean.CarTravelInformation;
import com.android.car.core.bean.CarTravelMemory;
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
import com.bihu.carcontrol.common.util.StringUtils;
import com.bihu.carcontrol.module.main.CarControlActivity;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

/**
 * FileName: TourismFragment
 * Author: WangXu
 * Date: 2023/6/13 10:55
 * Description: 旅行信息
 */
public class TourismFragment extends BaseFragment {
    private RadioGroup tourismRg;
    private LinearLayout linLong, linShort;
    private TextView tvMileageShort, tvPowerShort, tvSpeedShort, tvMileageLong, tvPowerLong, tvSpeedLong, tvShortTimeDay, tvShortTimeHour,
            tvShortTimeMinutes, tvLongTimeDay, tvLongTimeHour, tvLongTimeMinutes;
    private int selectPosition = 1;//0：长期记忆 1：默认短期记忆
    private RadioButton radioButton1, radioButton2;
    private Dialog longMemoryResetDialog;
    private int speedInt;
    private float averagePowerConsumptionFloat;

    @Override
    protected int getContentViewId() {
        return R.layout.fragment_tourism;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EventBus.getDefault().register(this);
    }

    @Override
    protected void initView(View view) {
        tourismRg = view.findViewById(R.id.tourism_rg);

        linLong = view.findViewById(R.id.lin_long);
        tvMileageLong = view.findViewById(R.id.tv_mileage_long);
        tvPowerLong = view.findViewById(R.id.tv_power_long);
        tvLongTimeDay = view.findViewById(R.id.tv_time_day_long);
        tvLongTimeHour = view.findViewById(R.id.tv_time_hour_long);
        tvLongTimeMinutes = view.findViewById(R.id.tv_time_minutes_long);
        tvSpeedLong = view.findViewById(R.id.tv_speed_long);
        linShort = view.findViewById(R.id.lin_short);
        tvMileageShort = view.findViewById(R.id.tv_mileage_short);
        tvShortTimeDay = view.findViewById(R.id.tv_time_day_short);
        tvShortTimeHour = view.findViewById(R.id.tv_time_hour_short);
        tvShortTimeMinutes = view.findViewById(R.id.tv_time_minutes_short);
        tvPowerShort = view.findViewById(R.id.tv_power_short);
        tvSpeedShort = view.findViewById(R.id.tv_speed_short);
        radioButton1 = view.findViewById(R.id.rb_first);
        radioButton2 = view.findViewById(R.id.rb_second);
        CarControlActivity activity = (CarControlActivity) getActivity();
        assert activity != null;
        if (activity.hasInit) {//SDKCar初始化未完成时不能请求数据，请求会导致中间层后面都不返回任何数据
            CarTravelInformation carTravelInformation = CarVehicleManager.getInstance().getCarTravelInformation();
            refreshTourismUi(carTravelInformation);
        }

        tourismRg.setOnCheckedChangeListener((radioGroup, i) -> {
                    CarTravelInformation carTravelInformation1 = CarVehicleManager.getInstance().getCarTravelInformation();
                    if (i == R.id.rb_first) {
                        selectPosition = 0;
                        linLong.setVisibility(View.VISIBLE);
                        linShort.setVisibility(View.GONE);
                        radioButton1.setTextColor(getResources().getColor(R.color.white));
                        radioButton2.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
                        refreshTourismUi(carTravelInformation1);
                    } else if (i == R.id.rb_second) {
                        selectPosition = 1;
                        linLong.setVisibility(View.GONE);
                        linShort.setVisibility(View.VISIBLE);
                        radioButton1.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
                        radioButton2.setTextColor(getResources().getColor(R.color.white));
                        refreshTourismUi(carTravelInformation1);
                    }
                }
        );

        view.findViewById(R.id.tv_reset_btn).setOnClickListener(v -> {
            if (DeviceUtil.ifPowerOff()) {
                return;
            }
            showResetDialog();
        });
    }

    //刷新旅行信息
    private void refreshTourismUi(CarTravelInformation carTravelInformation) {
        if (carTravelInformation != null) {
            int powerStatus = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.BCM_POWER_STATE);
            switch (selectPosition) {
                case 0://长期记忆
                    CarTravelMemory longMemory = carTravelInformation.getLongMemory();
                    if (powerStatus == 0 || speedInt == Constants.ERROR_CODE) {//电源状态为0或者车速信号丢失时，选择长期记忆需要显示所有数据为 -- （20250612 去掉平均电耗丢失的判断）
                        setLongTravelInvalid();
                        LogUtil.d("电源状态为0或者车速信号丢失时,长期记忆需要显示所有数据为 --: " + "  电源状态: " + powerStatus + "   车速： " + speedInt);
                    } else {
                        if (null != longMemory && tvMileageLong != null) {
                            //获取长期记忆行驶⾥程
                            float avgMileage = longMemory.getAverageVehicleDrivingMiller();
                            LogUtil.d("长期记忆行驶⾥程 ： " + avgMileage);
                            if (avgMileage != Constants.ERROR_CODE) {
                                tvMileageLong.setText(StringUtils.formatDecimal1(avgMileage));
                            } else {
                                tvMileageLong.setText(getString(R.string.invalid));
                            }
                            //获取长期记忆⾏⻋时间(分钟)
                            int drivingTime = longMemory.getVehicleDrivingTimeChange();
                            LogUtil.d("长期记忆行驶时间(返回单位：分钟) ： " + drivingTime);
                            if (drivingTime != Constants.ERROR_CODE) {
                                tvLongTimeDay.setText(String.valueOf(drivingTime / (60 * 24)));
                                tvLongTimeHour.setText(String.valueOf((drivingTime / 60) % 24));
                                tvLongTimeMinutes.setText(String.valueOf(drivingTime % 60));
                                LogUtil.d("长期记忆行驶时间 ： " + drivingTime / (60 * 24) + "天" + (drivingTime / 60) % 24 + "小时"
                                        + drivingTime % 60 + "分钟");
                            } else {
                                tvLongTimeDay.setText(getString(R.string.invalid));
                                tvLongTimeHour.setText(getString(R.string.invalid));
                                tvLongTimeMinutes.setText(getString(R.string.invalid));
                            }
                            //长期记忆平均电耗
                            float avgPower = longMemory.getAveragePowerConsumption();
                            LogUtil.d("长期记忆平均电耗 ： " + avgPower);
                            if (avgPower != Constants.ERROR_CODE) {
                                tvPowerLong.setText(StringUtils.formatDecimal1(avgPower));
                            } else {
                                tvPowerLong.setText(getString(R.string.invalid));
                            }
                            //长期记忆平均⻋速
                            int tempAvgSpeed = longMemory.getAverageVehicleDrivingSpeed();
                            LogUtil.d("长期记忆平均车速 ： " + tempAvgSpeed);
                            if (tempAvgSpeed != Constants.ERROR_CODE) {
                                if (tempAvgSpeed > 255) {
                                    tempAvgSpeed = 255;
                                }
                                tvSpeedLong.setText(StringUtils.formatDecimal1(tempAvgSpeed));
                            } else {
                                tvSpeedLong.setText(getString(R.string.invalid));
                            }
                        }
                    }
                    break;
                case 1://短期记忆
                    CarTravelMemory shortMemory = carTravelInformation.getShortMemory();
                    if (null != shortMemory && tvMileageShort != null) {
                        if (powerStatus == 0 || speedInt == Constants.ERROR_CODE) {//电源状态为0或者车速信号丢失时，选择短期记忆需要显示所有数据为 -- （20250612 去掉平均电耗丢失的判断）
                            setShortTravelInvalid();
                            LogUtil.d("电源状态为0或者车速信号丢失时,短期记忆需要显示所有数据为 --: " + "  电源状态: " + powerStatus + "   车速： " + speedInt);
                        } else {
                            //获取短期记忆行驶⾥程
                            float avgMileage = shortMemory.getAverageVehicleDrivingMiller();
                            LogUtil.d("短期记忆行驶⾥程 ： " + avgMileage);
                            if ((int) avgMileage != Constants.ERROR_CODE) {
                                tvMileageShort.setText(StringUtils.formatDecimal1(avgMileage));
                            } else {
                                tvMileageShort.setText(getString(R.string.invalid));
                            }
                            //获取短期记忆行驶时间(返回单位：分钟)
                            int tempDrivingTimeData = shortMemory.getVehicleDrivingTimeChange();
                            LogUtil.d("行驶时间(返回单位：分钟) ： " + tempDrivingTimeData);
                            if (tempDrivingTimeData != Constants.ERROR_CODE) {
                                tvShortTimeDay.setText(String.valueOf(tempDrivingTimeData / (60 * 24)));
                                tvShortTimeHour.setText(String.valueOf((tempDrivingTimeData / 60) % 24));
                                tvShortTimeMinutes.setText(String.valueOf(tempDrivingTimeData % 60));
                                LogUtil.d("短期记忆行驶时间 ： " + tempDrivingTimeData / (60 * 24) + "天" + (tempDrivingTimeData / 60) % 24 + "小时"
                                        + tempDrivingTimeData % 60 + "分钟");
                            } else {
                                tvShortTimeDay.setText(getString(R.string.invalid));
                                tvShortTimeHour.setText(getString(R.string.invalid));
                                tvShortTimeMinutes.setText(getString(R.string.invalid));
                            }

                            //短期记忆平均电耗
                            float tempAvgPower = shortMemory.getAveragePowerConsumption();
                            LogUtil.d("短期记忆平均电耗 ： " + tempAvgPower);
                            if ((int) tempAvgPower != Constants.ERROR_CODE) {
                                tvPowerShort.setText(StringUtils.formatDecimal1(tempAvgPower));
                            } else {
                                tvPowerShort.setText(getString(R.string.invalid));
                            }
                            //短期记忆平均⻋速
                            int tempAvgSpeed = shortMemory.getAverageVehicleDrivingSpeed();
                            LogUtil.d("短期记忆平均车速 ： " + tempAvgSpeed);
                            if (tempAvgSpeed != Constants.ERROR_CODE) {
                                if (tempAvgSpeed > 255) {
                                    tempAvgSpeed = 255;
                                }
                                tvSpeedShort.setText(StringUtils.formatDecimal1(tempAvgSpeed));
                            } else {
                                tvSpeedShort.setText(getString(R.string.invalid));
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        } else {
            LogUtil.d("旅行信息为空");
        }
    }

    //长期数据重置
    private void showResetDialog() {
        View view1 = LayoutInflater.from(getActivity()).inflate(R.layout.dialog_remove_history, (ViewGroup) null);
        if (longMemoryResetDialog == null) {
            longMemoryResetDialog = DialogUtil.getDialog(getActivity(), view1, 0.36);
        }
        if (!longMemoryResetDialog.isShowing() && longMemoryResetDialog != null) {
            longMemoryResetDialog.show();
        }
        view1.findViewById(R.id.tv_confirm).setOnClickListener(v -> {
            longMemoryResetDialog.dismiss();
            if (DeviceUtil.ifPowerOff() || FastClickCheckUtil.isFastClick(500)) {
                return;
            }
            LogUtil.d("长期记忆数据重置");
            //数据重置
            CarVehicleManager.getInstance().resetLongMemoryData();
        });

        view1.findViewById(R.id.tv_cancel).setOnClickListener(v -> {
            longMemoryResetDialog.dismiss();
        });
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onHandleEvent(VehicleMessageEventBean event) {
        int code = event.getCode();
        if (code == EventBusActionCode.TRAVEL_MEMORY_CHANGE_CODE && null != event.getCmd()) {//旅行信息变化回调
            CarTravelInformation carTravelInformation = (CarTravelInformation) event.getCmd();
            refreshTourismUi(carTravelInformation);
        } else if (code == EventBusActionCode.POWER_STATE) {
            int cmd = (int) event.getCmd();
            if (cmd == 0 && selectPosition == 0) {//电源下电时，选择长期记忆需要显示所有数据为 --
                setLongTravelInvalid();
            }
        } else if (code == EventBusActionCode.DRIVING_SPEED_CHANGE) {//车速(车速/平均电耗丢失长期记忆显示 --)
            speedInt = (int) event.getCmd();
        } else if (code == EventBusActionCode.AVERAGE_POWER_CONSUMPTION) {//平均电耗(车速/平均电耗丢失长期记忆显示 --)
            averagePowerConsumptionFloat = (float) event.getCmd();
        }
    }

    private void setLongTravelInvalid() {
        tvMileageLong.setText(getString(R.string.invalid));
        tvLongTimeDay.setText(getString(R.string.invalid));
        tvLongTimeHour.setText(getString(R.string.invalid));
        tvLongTimeMinutes.setText(getString(R.string.invalid));
        tvPowerLong.setText(getString(R.string.invalid));
        tvSpeedLong.setText(getString(R.string.invalid));
    }

    private void setShortTravelInvalid() {
        tvMileageShort.setText(getString(R.string.invalid));
        tvShortTimeDay.setText(getString(R.string.invalid));
        tvShortTimeHour.setText(getString(R.string.invalid));
        tvShortTimeMinutes.setText(getString(R.string.invalid));
        tvPowerShort.setText(getString(R.string.invalid));
        tvSpeedShort.setText(getString(R.string.invalid));
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        EventBus.getDefault().unregister(this);
    }
}
