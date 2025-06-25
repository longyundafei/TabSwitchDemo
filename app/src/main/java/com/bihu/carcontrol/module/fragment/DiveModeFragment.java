package com.bihu.carcontrol.module.fragment;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.SystemProperties;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.TextView;

import androidx.core.widget.NestedScrollView;

import com.android.car.client.manager.vehicle.CarVehicleManager;
import com.android.car.core.server.vehicle.VehicleType;
import com.bihu.carcontrol.R;
import com.bihu.carcontrol.base.fragment.BaseFragment;
import com.bihu.carcontrol.common.event.EventBusActionCode;
import com.bihu.carcontrol.common.event.VehicleMessageEventBean;
import com.bihu.carcontrol.common.util.Constants;
import com.bihu.carcontrol.common.util.DeviceUtil;
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
 * FileName: DiveModeFragment
 * Author: WangXu
 * Date: 2023/6/13 10:41
 * Description: 驾驶模式设置
 */
public class DiveModeFragment extends BaseFragment implements View.OnClickListener {
    private TextView tvMode1, tvMode2, tvMode3, tvPower1, tvPower2, tvPower3, tvSeekProgressDriving, tvSeekProgressEnergy;
    private ImageView ivWormMode, ivWormModeTips, ivAutomaticParking, ivAutomaticParkingTips, ivBrakeDiscWiping, ivBrakeDiscWipingTips;
    //蠕行模式  自动驻车设置 制动盘擦拭设置
    private boolean isWormModeOn = true, isAutomaticParking = true, isBrakeDiscWiping = false;
    private boolean carModeSetClick = false, powerSteeringModeSetClick = false, wormModeOnClick = false, automaticParkingClick = false;

    private SeekBar seekBarDriving, seekBarEnergy;
    private LinearLayout llCarMode, llMode3Show, llPower;
    //是否手动设置驾驶驱动强度
    private boolean isSeekProgressDriving = true;
    //是否手动设置能量回收强度
    private boolean isSeekBarEnergy = true;
    private int powerSteeringModeSet = 0;
    private int carModeSet = 0;
    private LinearLayout llWormMode, llWormModeItem, llAutomaticParking;

    @Override
    protected int getContentViewId() {
        return R.layout.fragment_dive_mode;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EventBus.getDefault().register(this);
    }

    @Override
    protected void initView(View view) {
        LogUtil.d("DiveModeFragment:  initView");
        llCarMode = view.findViewById(R.id.ll_car_mode);
        tvMode1 = view.findViewById(R.id.tv_mode_1);//常规模式
        tvMode2 = view.findViewById(R.id.tv_mode_2);//经济模式
        tvMode3 = view.findViewById(R.id.tv_mode_3);//运动模式
        tvMode1.setOnClickListener(this);
        tvMode2.setOnClickListener(this);
        tvMode3.setOnClickListener(this);

        llPower = view.findViewById(R.id.ll_power);
        tvPower1 = view.findViewById(R.id.tv_power_1);//轻便
        tvPower2 = view.findViewById(R.id.tv_power_2);//标准
        tvPower3 = view.findViewById(R.id.tv_power_3);//重载
        tvPower1.setOnClickListener(this);
        tvPower2.setOnClickListener(this);
        tvPower3.setOnClickListener(this);

        seekBarDriving = view.findViewById(R.id.seek_bar_driving);
        tvSeekProgressDriving = view.findViewById(R.id.tv_seek_progress_driving);

        seekBarEnergy = view.findViewById(R.id.seek_bar_energy);
        tvSeekProgressEnergy = view.findViewById(R.id.tv_seek_progress_energy);

        llMode3Show = view.findViewById(R.id.ll_mode_3_show);
        ivWormMode = view.findViewById(R.id.iv_worm_mode);
        llWormMode = view.findViewById(R.id.ll_worm_mode);
        llWormModeItem = view.findViewById(R.id.ll_worm_mode_item);
        ivWormModeTips = view.findViewById(R.id.iv_worm_mode_tips);
        ivAutomaticParking = view.findViewById(R.id.iv_automatic_parking);
        llAutomaticParking = view.findViewById(R.id.ll_automatic_parking);
        ivAutomaticParkingTips = view.findViewById(R.id.iv_automatic_parking_tips);
        ivBrakeDiscWiping = view.findViewById(R.id.iv_brake_disc_wiping);
        ivBrakeDiscWipingTips = view.findViewById(R.id.iv_brake_disc_wiping_tips);
        ivWormMode.setOnClickListener(this);
        ivWormModeTips.setOnClickListener(this);
        ivAutomaticParking.setOnClickListener(this);
        ivAutomaticParkingTips.setOnClickListener(this);
        ivBrakeDiscWiping.setOnClickListener(this);
        ivBrakeDiscWipingTips.setOnClickListener(this);
        String wormModeStr = SystemProperties.get(Constants.WORM_MODE_STR, "0x0");
        if (wormModeStr.equals(Constants.PROPERTIES_SHOW)) {
            LogUtil.d("蠕行模式配置字: 0x1 显示迎宾解锁 ");
            llWormModeItem.setVisibility(View.VISIBLE);
        } else {
            LogUtil.d("蠕行模式配置字:  " + wormModeStr + "  不显示蠕行模式");
            llWormModeItem.setVisibility(View.GONE);
        }

        NestedScrollView scrollView = view.findViewById(R.id.nested_scroll_view);
        scrollView.setOnScrollChangeListener((View.OnScrollChangeListener) (v, scrollX, scrollY, oldScrollX, oldScrollY) -> MyPopupWindow.getInstant(getContext()).dismissPopupWindow());
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

    //获取驾驶模式的数据
    public void initData() {
        //获取当前车辆驾驶模式
        int drivingMode = getDrivingMode();
        LogUtil.d("获取当前车辆驾驶模式: " + drivingMode);
        switch (drivingMode) {
            case VehicleType.CAR_MODE_ECO:
                refreshCarModeUi(1);
                break;
            case VehicleType.CAR_MODE_AI:
                refreshCarModeUi(2);
                break;
            case VehicleType.CAR_MODE_INDIVIDUALIZE:
                refreshCarModeUi(3);
                break;
            default:
                refreshCarModeUi(0);
                break;
        }

        int drivingPowerInt = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.GECKO_DRIVING_POWER_ST);
        seekBarDriving.setProgress(drivingPowerInt);
        tvSeekProgressDriving.setText(String.format("%s%%", drivingPowerInt));
        LogUtil.d("获取驾驶驱动强度： " + drivingPowerInt);
        //驾驶驱动强度
        seekBarDriving.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                tvSeekProgressDriving.setText(String.format("%s%%", progress));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                isSeekProgressDriving = true;
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                int progress = seekBar.getProgress();
                tvSeekProgressDriving.setText(String.format("%s%%", progress));
                if (isSeekProgressDriving) {
                    CarVehicleManager.getInstance().requestSetIntPropertyAsRoot(VehicleType.GECKO_DRIVING_POWER_ST, progress);
                    LogUtil.d("设置驾驶驱动强度： " + progress);
                    seekBarDriving.setClickable(false);
                    seekBarDriving.setEnabled(false);
                    seekBarDriving.setFocusable(false);
//                    setSeekBarUnClick(seekBarDriving, false);
                    cancelDrivingPowerTimeOut();
                    postDrivingPowerTimeOutDelayed();
                }
                isSeekProgressDriving = true;
            }
        });

        int energyLevelInt = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.GECKO_CCM_ENERGY_RECOV_LEVEL_CMD);
        seekBarEnergy.setProgress(energyLevelInt);
        tvSeekProgressEnergy.setText(String.format("%s%%", energyLevelInt));
        LogUtil.d("获取能量回收强度： " + energyLevelInt);
        seekBarEnergy.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                tvSeekProgressEnergy.setText(String.format("%s%%", progress));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                isSeekBarEnergy = true;
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                int progress = seekBar.getProgress();
                tvSeekProgressEnergy.setText(String.format("%s%%", progress));
                if (isSeekBarEnergy) {
                    setEnergyRecoverLevel(progress);
                    seekBarEnergy.setClickable(false);
                    seekBarEnergy.setEnabled(false);
                    seekBarEnergy.setFocusable(false);
//                    setSeekBarUnClick(seekBarEnergy, false);
                    cancelEnergyTimeOut();
                    postEnergyTimeOutDelayed();
                }
                isSeekBarEnergy = true;

            }
        });

        //蠕行模式
//        isWormModeOn = CarVehicleManager.getInstance().requestGetVehicleEnergyMode(VehicleType.PERISTALTIC_MODE);
//        ivWormMode.setImageResource(getSwitchRes(isWormModeOn));
        int peristalticModeInt = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.PERISTALTIC_MODE);
        LogUtil.d("获取蠕行模式: " + peristalticModeInt);
        refreshWormMode(peristalticModeInt);

        //助力转向模式
        int powerSteeringModeInt = getPowerSteeringModeInt();
        LogUtil.d("获取助力转向模式: " + powerSteeringModeInt);
        refreshPowerSteeringModeView(powerSteeringModeInt);

        //ADAS激活状态
        int adasStsHmi = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.GECKO_FCM_ACTDRVASSTSTSHMI);
        LogUtil.d("获取ADAS激活状态（2已激活 其他：未激活）： " + adasStsHmi);
        refreshAdasStsHmiPSM(adasStsHmi);

        //自动驻车开关
        int autoParkingOnInt = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.GECKO_AUTO_PARKING_ON);
        LogUtil.d("获取自动驻车开关: " + autoParkingOnInt);
        refreshAutoParkingOnView(autoParkingOnInt);
        //制动盘擦拭开关
        int brakeDiscWipingOnInt = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.GECKO_BRAKE_DISC_WIPING_ON);
        LogUtil.d("获取制动盘擦拭开关: " + brakeDiscWipingOnInt);
        refreshBrakeDiscWipingOnView(brakeDiscWipingOnInt);
    }

    //获取助力转向模式
    private int getPowerSteeringModeInt() {
        return CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.GECKO_POWER_STEERING_MODE);
    }

    //获取当前车辆驾驶模式
    private int getDrivingMode() {
        return CarVehicleManager.getInstance().getVehicleDrivingMode();
    }

    //初始化是否手动设置驾驶驱动强度和能量回收强度
    private void resetDrivingAndEnergy() {
        isSeekProgressDriving = true;
        isSeekBarEnergy = true;
    }

    //蠕行模式
    private void refreshWormMode(int peristalticModeInt) {
        if (peristalticModeInt == Constants.ERROR_CODE) {//蠕行模式信号丢失
            llWormMode.setAlpha(Constants.ERROR_ALPHA);
            ivWormMode.setClickable(false);
        } else {
            if (llWormMode.getAlpha() < 1.0f) {
                llWormMode.setAlpha(1.0f);
            }
            if (!ivWormMode.isClickable()) {
                ivWormMode.setClickable(true);
            }
            isWormModeOn = (peristalticModeInt == 0);
            ivWormMode.setImageResource(getSwitchRes(isWormModeOn));
        }
    }

    //自动驻车
    private void refreshAutoParkingOnView(int autoParkingOnInt) {
        if (autoParkingOnInt == Constants.ERROR_CODE) {//自动驻车信号丢失
            llAutomaticParking.setAlpha(Constants.ERROR_ALPHA);
            ivAutomaticParking.setClickable(false);
        } else {
            if (llAutomaticParking.getAlpha() < 1.0f) {
                llAutomaticParking.setAlpha(1.0f);
            }
            if (!ivAutomaticParking.isClickable()) {
                ivAutomaticParking.setClickable(true);
            }
            //0 关   1-2-3-4-5 任意一个值都为开
            isAutomaticParking = (autoParkingOnInt == 5 || autoParkingOnInt == 4 || autoParkingOnInt == 3 || autoParkingOnInt == 2 || autoParkingOnInt == 1);
            ivAutomaticParking.setImageResource(getSwitchRes(isAutomaticParking));
        }
    }

    //制动盘擦拭
    private void refreshBrakeDiscWipingOnView(int brakeDiscWipingOnInt) {
        isBrakeDiscWiping = (brakeDiscWipingOnInt == 1);
        ivBrakeDiscWiping.setImageResource(getSwitchRes(isBrakeDiscWiping));
    }

    private int getSwitchRes(boolean res) {
        return res ? R.drawable.ic_switch_on : R.drawable.ic_switch_off;
    }

    @Override
    public void onClick(View v) {
        int id = v.getId();
        if (id == R.id.iv_worm_mode_tips) {//蠕行模式tips
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivWormModeTips, getString(R.string.worm_mode_tips));
            }
        } else if (id == R.id.iv_automatic_parking_tips) {//自动驻车tips
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivAutomaticParkingTips, getString(R.string.automatic_parking_tips));
            }
        } else if (id == R.id.iv_brake_disc_wiping_tips) {//制动盘擦拭tips
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivBrakeDiscWipingTips, getString(R.string.brake_disc_wiping_tips));
            }
        } else {
            if (DeviceUtil.ifPowerOff() || FastClickCheckUtil.isFastClick(500)) {
                return;
            }
            if (id == R.id.tv_mode_1) {//经济模式
                if (getDrivingMode() != VehicleType.CAR_MODE_ECO) {
                    setMode(1);
                }
            } else if (id == R.id.tv_mode_2) {//智能模式
                if (getDrivingMode() != VehicleType.CAR_MODE_AI) {
                    setMode(2);
                }
            } else if (id == R.id.tv_mode_3) {//个性化模式
                if (getDrivingMode() != VehicleType.CAR_MODE_INDIVIDUALIZE) {
                    setMode(3);
                }
            } else if (id == R.id.tv_power_1) {//轻便
                if (getPowerSteeringModeInt() != 2) {
                    setPowerSteeringMode(2);
                    LogUtil.d("助力转向模式设置：  轻便  2");
                }
            } else if (id == R.id.tv_power_2) {//标准
                if (getPowerSteeringModeInt() != 1) {
                    setPowerSteeringMode(1);
                    LogUtil.d("助力转向模式设置：  标准  1");
                }
            } else if (id == R.id.tv_power_3) {//重载
                if (getPowerSteeringModeInt() != 3) {
                    setPowerSteeringMode(3);
                    LogUtil.d("助力转向模式设置：  重载  3");
                }
            } else if (id == R.id.iv_worm_mode) {//蠕行模式
                setWormMode();
            } else if (id == R.id.iv_automatic_parking) {//自动驻车
                setAutomaticParking();
            } else if (id == R.id.iv_brake_disc_wiping) {//制动盘擦拭 20250108产品确认制动盘擦拭不接收回调，下设就变UI状态
                //1 on, 0 off
                setPropertyValue(VehicleType.GECKO_BRAKE_DISC_WIPING_ON, !isBrakeDiscWiping ? 1 : 0);
                LogUtil.d("制动盘擦拭设置： " + !isBrakeDiscWiping + "  下设值： " + (!isBrakeDiscWiping ? 1 : 0));
                refreshBrakeDiscWipingOnView(!isBrakeDiscWiping ? 1 : 0);
            }
        }
    }

    private boolean isCanShowPopup() {
        return (isAdded() && !isDetached() && getActivity() != null && !getActivity().isFinishing() && !getActivity().isDestroyed());
    }

    //蠕行模式设置
    private void setWormMode() {
        //下传 true是1，false是0 旧有映射逻辑，这里跟新需求下设信号有差别
        setPropertyValue(VehicleType.PERISTALTIC_MODE, !isWormModeOn ? 1 : 0);
        LogUtil.d("蠕行模式设置： " + !isWormModeOn + "  下设值： " + (!isWormModeOn ? 1 : 0));
        ivWormMode.setImageResource(getSwitchRes(!isWormModeOn));
        ivWormMode.setClickable(false);
        wormModeOnClick = true;
        cancelWormModeTimeOut();
        postWormModeTimeOutDelayed();
    }

    private void setPowerSteeringMode(int powerSteeringModeInt) {
        setPropertyValue(VehicleType.GECKO_POWER_STEERING_MODE, powerSteeringModeInt);
        powerSteeringModeSet = powerSteeringModeInt;
        powerSteeringModeSetClick = true;
        refreshPowerSteeringModeView(powerSteeringModeInt);
        cancelPowerSteeringModeTimeOut();
        postPowerSteeringModeTimeOutDelayed();
    }

    //自动驻车
    private void setAutomaticParking() {
        setPropertyValue(VehicleType.GECKO_AUTO_PARKING_ON, !isAutomaticParking ? 1 : 2);
        LogUtil.d("自动驻车设置： " + !isAutomaticParking + "  下设值： " + (!isAutomaticParking ? 1 : 2));
        ivAutomaticParking.setImageResource(getSwitchRes(!isAutomaticParking));
        ivAutomaticParking.setClickable(false);
        automaticParkingClick = true;
        cancelAutomaticParkingTimeOut();
        postAutomaticParkingTimeOutDelayed();
    }


    //属性设置
    private void setPropertyValue(int key, int value) {
        CarVehicleManager.getInstance().requestSetIntPropertyAsRoot(key, value);
    }

    //驾驶模式UI更新
    public void refreshCarModeUi(int position) {
        if (tvMode1 == null) {
            return;
        }
        if (position == 1 || position == 2 || position == 3) {
            if (llCarMode.getAlpha() < 1.0f) {
                llCarMode.setAlpha(1.0f);
            }
            tvMode1.setClickable(true);
            tvMode2.setClickable(true);
            tvMode3.setClickable(true);
        } else {
            tvMode1.setClickable(false);
            tvMode2.setClickable(false);
            tvMode3.setClickable(false);
        }
        if (position == 1) {//ECO模式
            llMode3Show.setVisibility(View.GONE);
            tvMode1.setBackgroundResource(R.drawable.bg_segmented_btn);
            tvMode1.setTextColor(getResources().getColor(R.color.white));
            tvMode2.setBackground(null);
            tvMode2.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            tvMode3.setBackground(null);
            tvMode3.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
        } else if (position == 2) {//AI模式
            llMode3Show.setVisibility(View.GONE);
            tvMode1.setBackground(null);
            tvMode1.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            tvMode2.setBackgroundResource(R.drawable.bg_segmented_btn);
            tvMode2.setTextColor(getResources().getColor(R.color.white));
            tvMode3.setBackground(null);
            tvMode3.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
        } else if (position == 3) {//个性化模式
            llMode3Show.setVisibility(View.VISIBLE);
            //显示驾驶驱动强度和能量回收强度，同时初始化可设置
            resetDrivingAndEnergy();
            tvMode1.setBackground(null);
            tvMode1.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            tvMode2.setBackground(null);
            tvMode2.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            tvMode3.setBackgroundResource(R.drawable.bg_segmented_btn);
            tvMode3.setTextColor(getResources().getColor(R.color.white));
        } else if (position == Constants.ERROR_CODE) {//驾驶模式信号丢失
            llCarMode.setAlpha(Constants.ERROR_ALPHA);
        } else {
            llCarMode.setAlpha(Constants.ERROR_ALPHA);
            llMode3Show.setVisibility(View.GONE);
            tvMode1.setBackground(null);
            tvMode1.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            tvMode2.setBackground(null);
            tvMode2.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            tvMode3.setBackground(null);
            tvMode3.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
        }
    }

    //设置驾驶模式
    public void setMode(int mode) {
        LogUtil.d("设置当前车辆驾驶模式点击 " + mode);
        int carMode = VehicleType.CAR_MODE_ECO;
        carModeSetClick = true;
        switch (mode) {
            case 1:
                carMode = VehicleType.CAR_MODE_ECO;
                LogUtil.d("设置当前车辆驾驶模式 CAR_MODE_ECO: " + carMode);
                break;
            case 2:
                carMode = VehicleType.CAR_MODE_AI;
                LogUtil.d("设置当前车辆驾驶模式 CAR_MODE_AI: " + carMode);
                break;
            case 3:
                carMode = VehicleType.CAR_MODE_INDIVIDUALIZE;
                LogUtil.d("设置当前车辆驾驶模式 CAR_MODE_INDIVIDUALIZE:  " + carMode);
                break;
        }
        CarVehicleManager.getInstance().requestSetVehicleMode(carMode);

        carModeSet = carMode;
        refreshCarModeUi(mode);
        cancelCarModeTimeOut();
        postCarModeTimeOutDelayed();
    }

    //助力转向模式UI更新
    public void refreshPowerSteeringModeView(int powerSteeringModeInt) {
        if (powerSteeringModeInt == 1 || powerSteeringModeInt == 2 || powerSteeringModeInt == 3) {
            if (llPower.getAlpha() < 1.0f) {
                llPower.setAlpha(1.0f);
            }
            tvPower1.setClickable(true);
            tvPower2.setClickable(true);
            tvPower3.setClickable(true);
        } else {
            tvPower1.setClickable(false);
            tvPower2.setClickable(false);
            tvPower3.setClickable(false);
        }
        //标准模式 0x1 轻便模式 0x2  重载模式 0x3
        if (powerSteeringModeInt == 1) {//标准
            tvPower1.setBackground(null);
            tvPower1.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            tvPower2.setBackgroundResource(R.drawable.bg_segmented_btn);
            tvPower2.setTextColor(getResources().getColor(R.color.white));
            tvPower3.setBackground(null);
            tvPower3.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
        } else if (powerSteeringModeInt == 2) {//轻便
            tvPower1.setBackgroundResource(R.drawable.bg_segmented_btn);
            tvPower1.setTextColor(getResources().getColor(R.color.white));
            tvPower2.setBackground(null);
            tvPower2.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            tvPower3.setBackground(null);
            tvPower3.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
        } else if (powerSteeringModeInt == 3) {//重载
            tvPower1.setBackground(null);
            tvPower1.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            tvPower2.setBackground(null);
            tvPower2.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            tvPower3.setBackgroundResource(R.drawable.bg_segmented_btn);
            tvPower3.setTextColor(getResources().getColor(R.color.white));
        } else if (powerSteeringModeInt == Constants.ERROR_CODE) {//助力转向信号丢失
            llPower.setAlpha(Constants.ERROR_ALPHA);
        } else {
            llPower.setAlpha(Constants.ERROR_ALPHA);
            tvPower1.setBackground(null);
            tvPower1.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            tvPower2.setBackground(null);
            tvPower2.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            tvPower3.setBackground(null);
            tvPower3.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
        }
    }

    private void showSetupFailureToast() {
        ToastMaster.shortToast(DiveModeFragment.this, getString(R.string.setup_failure));
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onModeChange(VehicleMessageEventBean eventBean) {
        int code = eventBean.getCode();
        if (code == EventBusActionCode.PERISTALTIC_MODE && eventBean.getCmd() != null) {//蠕行模式回调
            int peristalticModeInt = (int) eventBean.getCmd();
            if (wormModeOnClick && peristalticModeInt != Constants.ERROR_CODE && ((peristalticModeInt == 0) == isWormModeOn)) {//下设值和返回值不对应
                showSetupFailureToast();
                LogUtil.d("蠕行模式下设值和返回值不对应下设值: " + !isWormModeOn + "  反馈值：" + (peristalticModeInt == 0));
            }
            wormModeOnClick = false;
            cancelWormModeTimeOut();
            refreshWormMode(peristalticModeInt);
        } else if (EventBusActionCode.DRIVE_MODE_CHANGE_CODE == code && eventBean.getCmd() != null) {//驾驶模式回调
            int changeMode = (int) eventBean.getCmd();
            if (carModeSetClick && changeMode != Constants.ERROR_CODE && changeMode != carModeSet) {//
                showSetupFailureToast();
                LogUtil.d("驾驶模式下设值和返回值不对应下设值： " + carModeSet + "    返回值： " + changeMode);
            }
            carModeSetClick = false;
            cancelCarModeTimeOut();
            if (changeMode == VehicleType.CAR_MODE_ECO) {//经济模式
                refreshCarModeUi(1);
            } else if (changeMode == VehicleType.CAR_MODE_AI) {//智能模式
                refreshCarModeUi(2);
            } else if (changeMode == VehicleType.CAR_MODE_INDIVIDUALIZE) {//个性化模式
                refreshCarModeUi(3);
            } else if (changeMode == Constants.ERROR_CODE) {
                refreshCarModeUi(Constants.ERROR_CODE);
            } else {
                //驾驶模式异常
                refreshCarModeUi(0);
            }
        } else if (code == EventBusActionCode.POWER_STEERING_MODE) {//助力转向模式
            int cmd = (int) eventBean.getCmd();
            cancelPowerSteeringModeTimeOut();
            if (powerSteeringModeSetClick && cmd != Constants.ERROR_CODE && powerSteeringModeSet != cmd) {//下设值和VCU返回值不对应 toast提示
                showSetupFailureToast();
                LogUtil.d("助力转向模式下设值和返回值不对应下设值： " + powerSteeringModeSet + "    返回值： " + cmd);
            }
            powerSteeringModeSetClick = false;
            refreshPowerSteeringModeView(cmd);
        } else if (code == EventBusActionCode.AUTO_PARKING_ON) {//自动驻车
            int cmd = (int) eventBean.getCmd();
            if (automaticParkingClick && cmd != Constants.ERROR_CODE && ((cmd == 5 || cmd == 4 || cmd == 3 || cmd == 2 || cmd == 1) == isAutomaticParking)) {//下设值和返回值不对应
                showSetupFailureToast();
                LogUtil.d("自动驻车下设值和返回值不对应下设值: " + !isAutomaticParking + "  反馈值：" + (cmd == 5 || cmd == 4 || cmd == 3 || cmd == 2 || cmd == 1));
            }
            automaticParkingClick = false;
            cancelAutomaticParkingTimeOut();
            refreshAutoParkingOnView(cmd);
        } else if (code == EventBusActionCode.BRAKE_DISC_WIPING_ON) {//制动盘擦拭 20250108产品确认制动盘擦拭不接收回调，下设就变UI状态
//            refreshBrakeDiscWipingOnView((int) eventBean.getCmd());
        } else if (code == EventBusActionCode.DRIVING_POWER_ST) {//驾驶驱动强度
            int cmd = (int) eventBean.getCmd();
            if (cmd != Constants.ERROR_CODE && (cmd != seekBarDriving.getProgress())) {//下设值和返回值不对应
                showSetupFailureToast();
                LogUtil.d("驾驶驱动强度下设值和返回值不对应下设值: " + seekBarDriving.getProgress() + "  反馈值：" + cmd);
            }
            //回调只设置显示的值，不再发送下设驱动值指令
            isSeekProgressDriving = false;
            seekBarDriving.setProgress(cmd);
            if (cmd == Constants.ERROR_CODE) {
                seekBarDriving.setClickable(false);
                seekBarDriving.setEnabled(false);
                seekBarDriving.setFocusable(false);
            } else {
                seekBarDriving.setClickable(true);
                seekBarDriving.setEnabled(true);
                seekBarDriving.setFocusable(true);
            }
//            setSeekBarUnClick(seekBarDriving,true);
            cancelDrivingPowerTimeOut();
        } else if (code == EventBusActionCode.ENERGY_RECOV_LEVEL) {//能量回收强度
            int cmd = (int) eventBean.getCmd();
            if (cmd != Constants.ERROR_CODE && (cmd != seekBarEnergy.getProgress())) {//下设值和返回值不对应
                showSetupFailureToast();
                LogUtil.d("能量回收强度下设值和返回值不对应下设值: " + seekBarEnergy.getProgress() + "  反馈值：" + cmd);
            }
            //回调只设置显示的值，不再发送下设驱动值指令
            isSeekBarEnergy = false;
            seekBarEnergy.setProgress(cmd);
            if (cmd == Constants.ERROR_CODE) {
                seekBarEnergy.setClickable(false);
                seekBarEnergy.setEnabled(false);
                seekBarEnergy.setFocusable(false);
            } else {
                seekBarEnergy.setClickable(true);
                seekBarEnergy.setEnabled(true);
                seekBarEnergy.setFocusable(true);
            }
//            setSeekBarUnClick(seekBarEnergy, true);
            cancelEnergyTimeOut();
        } else if (code == EventBusActionCode.FCM_ACT_DRV_ASST_STS_HMI) {//ADAS激活  2已激活 其他关闭
            int cmd = (int) eventBean.getCmd();
            refreshAdasStsHmiPSM(cmd);
        } else if (code == EventBusActionCode.DRIVING_SPEED_CHANGE) {//车速
            int speedInt = (int) eventBean.getCmd();
            if (speedInt > 3) {//行车中不能设置助力转向
                if (llPower.getAlpha() == 1.0f) {
                    LogUtil.d("车速大于3，助力转向置灰:车速：" + speedInt);
                    llPower.setAlpha(Constants.ERROR_ALPHA);
                }
                if (tvPower1.isClickable()) {
                    tvPower1.setClickable(false);
                }
                if (tvPower2.isClickable()) {
                    tvPower2.setClickable(false);
                }
                if (tvPower3.isClickable()) {
                    tvPower3.setClickable(false);
                }
            } else {
                int powerSteeringModeInt = getPowerSteeringModeInt();
                LogUtil.d("车速小于3，获取助力转向模式: " + powerSteeringModeInt);
                refreshPowerSteeringModeView(powerSteeringModeInt);
            }
        }
    }

    private void refreshAdasStsHmiPSM(int cmd) {
        if (cmd == 2) {
            LogUtil.d("ADAS激活 助力转向需置灰");
            llPower.setAlpha(Constants.ERROR_ALPHA);
            tvPower1.setClickable(false);
            tvPower2.setClickable(false);
            tvPower3.setClickable(false);
        } else {
            int powerSteeringModeInt = getPowerSteeringModeInt();
            LogUtil.d("ADAS关闭，获取助力转向模式: " + powerSteeringModeInt);
            refreshPowerSteeringModeView(powerSteeringModeInt);
        }
    }

    //设置能量回收百分比
    private void setEnergyRecoverLevel(int percent) {
        LogUtil.d("设置能量回收强度： " + percent);
        CarVehicleManager.getInstance().requestSetIntPropertyAsRoot(VehicleType.GECKO_CCM_ENERGY_RECOV_LEVEL_CMD, percent);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        EventBus.getDefault().unregister(this);
    }

    private Timer carModeTimeOutTimer;
    private TimerTask carModeTimeOutTask;

    private void postCarModeTimeOutDelayed() {
        carModeTimeOutTimer = new Timer();
        carModeTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                tvMode1.post(() -> {
                    showSetupFailureToast();
                    //获取当前车辆驾驶模式
                    int drivingMode = getDrivingMode();
                    LogUtil.d("获取当前车辆驾驶模式: " + drivingMode);
                    switch (drivingMode) {
                        case VehicleType.CAR_MODE_ECO:
                            refreshCarModeUi(1);
                            break;
                        case VehicleType.CAR_MODE_AI:
                            refreshCarModeUi(2);
                            break;
                        case VehicleType.CAR_MODE_INDIVIDUALIZE:
                            refreshCarModeUi(3);
                            break;
                        default:
                            refreshCarModeUi(0);
                            break;
                    }
                    LogUtil.d("设置失败，设置驾驶模式超时： " + drivingMode);
                    carModeSetClick = false;
                });
            }
        };
        carModeTimeOutTimer.schedule(carModeTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelCarModeTimeOut() {
        if (carModeTimeOutTimer != null) {
            carModeTimeOutTimer.cancel();
            carModeTimeOutTimer = null;
        }
        if (carModeTimeOutTask != null) {
            carModeTimeOutTask.cancel();
            carModeTimeOutTask = null;
        }
    }

    private Timer powerSteeringModeTimeOutTimer;
    private TimerTask powerSteeringModeTimeOutTask;

    private void postPowerSteeringModeTimeOutDelayed() {
        powerSteeringModeTimeOutTimer = new Timer();
        powerSteeringModeTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                llPower.post(() -> {
                    showSetupFailureToast();
                    powerSteeringModeSetClick = false;
                    int powerSteeringModeInt = getPowerSteeringModeInt();
                    refreshPowerSteeringModeView(powerSteeringModeInt);
                    LogUtil.d("设置失败，设置助力转向超时： " + powerSteeringModeInt + "  powerSteeringModeSet: " + powerSteeringModeSet);
                });
            }
        };
        powerSteeringModeTimeOutTimer.schedule(powerSteeringModeTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelPowerSteeringModeTimeOut() {
        if (powerSteeringModeTimeOutTimer != null) {
            powerSteeringModeTimeOutTimer.cancel();
            powerSteeringModeTimeOutTimer = null;
        }
        if (powerSteeringModeTimeOutTask != null) {
            powerSteeringModeTimeOutTask.cancel();
            powerSteeringModeTimeOutTask = null;
        }
    }

    private Timer wormModeTimeOutTimer;
    private TimerTask wormModeTimeOutTask;

    private void postWormModeTimeOutDelayed() {
        wormModeTimeOutTimer = new Timer();
        wormModeTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivWormMode.post(() -> {
                    showSetupFailureToast();
                    ivWormMode.setClickable(true);
                    wormModeOnClick = false;
                    int peristalticModeInt = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.PERISTALTIC_MODE);
                    refreshWormMode(peristalticModeInt);
                    LogUtil.d("设置失败，设置蠕行模式超时： " + peristalticModeInt + "  isWormModeOn: " + isWormModeOn);
                });
            }
        };
        wormModeTimeOutTimer.schedule(wormModeTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelWormModeTimeOut() {
        if (wormModeTimeOutTimer != null) {
            wormModeTimeOutTimer.cancel();
            wormModeTimeOutTimer = null;
        }
        if (wormModeTimeOutTask != null) {
            wormModeTimeOutTask.cancel();
            wormModeTimeOutTask = null;
        }
    }

    private Timer automaticParkingTimeOutTimer;
    private TimerTask automaticParkingTimeOutTask;

    private void postAutomaticParkingTimeOutDelayed() {
        automaticParkingTimeOutTimer = new Timer();
        automaticParkingTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivAutomaticParking.post(() -> {
                    showSetupFailureToast();
                    ivAutomaticParking.setClickable(true);
                    automaticParkingClick = false;
                    int autoParkingOnInt = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.GECKO_AUTO_PARKING_ON);
                    refreshAutoParkingOnView(autoParkingOnInt);
                    LogUtil.d("设置失败，设置自动驻车超时： " + autoParkingOnInt + "  isAutomaticParking:" + isAutomaticParking);
                });
            }
        };
        automaticParkingTimeOutTimer.schedule(automaticParkingTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelAutomaticParkingTimeOut() {
        if (automaticParkingTimeOutTimer != null) {
            automaticParkingTimeOutTimer.cancel();
            automaticParkingTimeOutTimer = null;
        }
        if (automaticParkingTimeOutTask != null) {
            automaticParkingTimeOutTask.cancel();
            automaticParkingTimeOutTask = null;
        }
    }

    private Timer drivingPowerTimeOutTimer;
    private TimerTask drivingPowerTimeOutTask;

    //驾驶驱动强度设置超时
    private void postDrivingPowerTimeOutDelayed() {
        drivingPowerTimeOutTimer = new Timer();
        drivingPowerTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                seekBarDriving.post(() -> {
                    showSetupFailureToast();
                    seekBarDriving.setClickable(true);
                    seekBarDriving.setEnabled(true);
                    seekBarDriving.setFocusable(true);
//                    setSeekBarUnClick(seekBarDriving, true);
                    int drivingPowerInt = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.GECKO_DRIVING_POWER_ST);
                    LogUtil.d("驾驶驱动强度设置超时，设置当前驾驶驱动强度： " + drivingPowerInt);
                    seekBarDriving.setProgress(drivingPowerInt);
                });
            }
        };
        drivingPowerTimeOutTimer.schedule(drivingPowerTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelDrivingPowerTimeOut() {
        if (drivingPowerTimeOutTimer != null) {
            drivingPowerTimeOutTimer.cancel();
            drivingPowerTimeOutTimer = null;
        }
        if (drivingPowerTimeOutTask != null) {
            drivingPowerTimeOutTask.cancel();
            drivingPowerTimeOutTask = null;
        }
    }

    private Timer energyTimeOutTimer;
    private TimerTask energyTimeOutTask;

    //能量回收强度设置超时
    private void postEnergyTimeOutDelayed() {
        energyTimeOutTimer = new Timer();
        energyTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                seekBarEnergy.post(() -> {
                    showSetupFailureToast();
                    seekBarEnergy.setClickable(true);
                    seekBarEnergy.setEnabled(true);
                    seekBarEnergy.setFocusable(true);
//                    setSeekBarUnClick(seekBarEnergy, true);
                    int energyLevelInt = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.GECKO_CCM_ENERGY_RECOV_LEVEL_CMD);
                    seekBarEnergy.setProgress(energyLevelInt);
                    LogUtil.d("能量回收强度设置超时，设置当前能量回收强度： " + energyLevelInt);
                });
            }
        };
        energyTimeOutTimer.schedule(energyTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelEnergyTimeOut() {
        if (energyTimeOutTimer != null) {
            energyTimeOutTimer.cancel();
            energyTimeOutTimer = null;
        }
        if (energyTimeOutTask != null) {
            energyTimeOutTask.cancel();
            energyTimeOutTask = null;
        }
    }

    @SuppressLint("ClickableViewAccessibility")
    private void setSeekBarUnClick(SeekBar seekBar, boolean clickable) {
        seekBar.setOnTouchListener((v, event) -> !clickable);
    }
}
