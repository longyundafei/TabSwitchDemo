package com.bihu.carcontrol.module.main;

import android.car.VehiclePropertyIds;
import android.content.Context;
import android.content.res.Configuration;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.TextView;

import com.alibaba.fastjson.JSONObject;
import com.android.car.client.manager.vehicle.CarVehicleManager;
import com.android.car.core.base.MainServiceProxyConnector;
import com.android.car.core.bean.BatteryInfo;
import com.android.car.core.bean.CarTravelInformation;
import com.android.car.core.bean.CarWheelInfo;
import com.android.car.core.bean.ChargeInfo;
import com.android.car.core.bean.FatigueDrivingInfo;
import com.android.car.core.bean.MaintenanceTips;
import com.android.car.core.bean.VehicleDoorInfo;
import com.android.car.core.bean.VehicleLockInfo;
import com.android.car.core.bean.VehicleTempInfo;
import com.android.car.core.server.vehicle.ICarVehicle;
import com.android.car.core.server.vehicle.VehicleType;
import com.bihu.carcontrol.R;
import com.bihu.carcontrol.base.activity.BaseActivity;
import com.bihu.carcontrol.base.activity.BaseView;
import com.bihu.carcontrol.common.event.EventBusActionCode;
import com.bihu.carcontrol.common.event.VehicleMessageEventBean;
import com.bihu.carcontrol.common.util.LogUtil;
import com.bihu.carcontrol.module.fragment.CarSetFragment;
import com.bihu.carcontrol.module.fragment.DiveModeFragment;
import com.bihu.carcontrol.module.fragment.EnergyManageFragment;
import com.bihu.carcontrol.module.fragment.IntelligentDrivingFragment;
import com.bihu.carcontrol.module.fragment.MaintainFragment;
import com.bihu.carcontrol.module.fragment.SafetyFragment;
import com.bihu.carcontrol.module.fragment.TourismFragment;

import org.greenrobot.eventbus.EventBus;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * FileName: CarControlActivity
 * Author: WangXu
 * Date: 2024/6/12 10:25
 * Description: 车辆控制
 */
public class CarControlActivity extends BaseActivity implements BaseView, View.OnClickListener, MainServiceProxyConnector.ServiceReadyObserver {

    private final List<TextView> mTvList = new ArrayList();
    private final List<ImageView> mImgList = new ArrayList();
    private final List<Integer> mImgSelect = new ArrayList();
    private final List<Integer> mImgDefault = new ArrayList();

    public boolean hasInit = false;
    private CarSetFragment carSetFragment;
    private DiveModeFragment diveModeFragment;
    private EnergyManageFragment energyManageFragment;
    private IntelligentDrivingFragment intelligentDrivingFragment;
    private SafetyFragment safetyFragment;
    private MaintainFragment maintainFragment;
    private TourismFragment tourismFragment;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_car_control);

        MainServiceProxyConnector.getInstance().addCallback(this);
        LogUtil.d("MainServiceProxyConnector.getInstance().addCallback(this);");
    }

    @Override
    protected void initView() {
        LogUtil.d("CarControlActivity initView");

        Window window = getWindow();
        WindowManager.LayoutParams layoutParams = window.getAttributes();
        layoutParams.width = WindowManager.LayoutParams.MATCH_PARENT;
        layoutParams.height = WindowManager.LayoutParams.MATCH_PARENT;
        window.setAttributes(layoutParams);
        window.clearFlags(WindowManager.LayoutParams.FLAG_DIM_BEHIND);

        findViewById(R.id.iv_exit).setOnClickListener(this);

        TextView tvCarSet = findViewById(R.id.tv_car_set);
        TextView tvDrivingMode = findViewById(R.id.tv_driving_mode);
        TextView tvEnergyManage = findViewById(R.id.tv_energy_manage);
        TextView tvIntelligentDriving = findViewById(R.id.tv_intelligent_driving);
        TextView tvSafety = findViewById(R.id.tv_safe);
        TextView tvMaintain = findViewById(R.id.tv_maintain);
        TextView tvTourism = findViewById(R.id.tv_tourism);

        ImageView ivCarSet = findViewById(R.id.iv_car_set);
        ImageView ivDrivingMode = findViewById(R.id.iv_driving_mode);
        ImageView ivEnergyManage = findViewById(R.id.iv_energy_manage);
        ImageView ivIntelligentDriving = findViewById(R.id.iv_intelligent_driving);
        ImageView ivSafety = findViewById(R.id.iv_safety);
        ImageView ivMaintain = findViewById(R.id.iv_maintain);
        ImageView ivTourism = findViewById(R.id.iv_tourism);

        mTvList.add(tvCarSet);
        mTvList.add(tvDrivingMode);
        mTvList.add(tvEnergyManage);
        mTvList.add(tvIntelligentDriving);
        mTvList.add(tvSafety);
        mTvList.add(tvMaintain);
        mTvList.add(tvTourism);

        mImgList.add(ivCarSet);
        mImgList.add(ivDrivingMode);
        mImgList.add(ivEnergyManage);
        mImgList.add(ivIntelligentDriving);
        mImgList.add(ivSafety);
        mImgList.add(ivMaintain);
        mImgList.add(ivTourism);

        mImgSelect.add(R.drawable.tag1_true);
        mImgSelect.add(R.drawable.tag2_true);
        mImgSelect.add(R.drawable.tag3_true);
        mImgSelect.add(R.drawable.tag4_true);
        mImgSelect.add(R.drawable.tag5_true);
        mImgSelect.add(R.drawable.tag6_true);
        mImgSelect.add(R.drawable.tag7_true);

        mImgDefault.add(R.drawable.tag1_false);
        mImgDefault.add(R.drawable.tag2_false);
        mImgDefault.add(R.drawable.tag3_false);
        mImgDefault.add(R.drawable.tag4_false);
        mImgDefault.add(R.drawable.tag5_false);
        mImgDefault.add(R.drawable.tag6_false);
        mImgDefault.add(R.drawable.tag7_false);

        tvCarSet.setOnClickListener(this);
        tvDrivingMode.setOnClickListener(this);
        tvEnergyManage.setOnClickListener(this);
        tvIntelligentDriving.setOnClickListener(this);
        tvSafety.setOnClickListener(this);
        tvMaintain.setOnClickListener(this);
        tvTourism.setOnClickListener(this);
        int index = getIntent().getIntExtra("index", 0);
        if (index == 0 || index == 7) {
            switchTab(index);
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        LogUtil.d("CarControlActivity onResume initManager");
        initManager();
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (hasInit) {//初始化成功才能移除回调，否则会获取不到数据
            CarVehicleManager.getInstance().unregisterCallback(carVehicle);
            LogUtil.d("CarControlActivity onPause unregisterCallback(carVehicle)");
        }
    }

    private void initManager() {
        LogUtil.d("MainServiceProxyConnector.getInstance().isServiceReady():  " + MainServiceProxyConnector.getInstance().isServiceReady());
        LogUtil.d("hasInit:  " + hasInit);
        //判断是否初始化成功
        if (!MainServiceProxyConnector.getInstance().isServiceReady()) {
            MainServiceProxyConnector.getInstance().addCallback(this);
            return;
        }

        LogUtil.d("registerCallback(carVehicle)");
        CarVehicleManager.getInstance().registerCallback(carVehicle);
        if (hasInit) {
            return;
        }

        int index = getIntent().getIntExtra("index", 0);
        LogUtil.d("index: " + index);
        switchTab(index);

        hasInit = true;
        LogUtil.d("hasInit:   ----- true");
        if (index == 0) {
            postDelayed(() -> {
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.FRESH_FRAGMENT_DATA, index));
            }, 200);
        }
    }

    @Override
    public void onServiceConnected() {
        LogUtil.d("onServiceConnected  initManager");
        initManager();
    }

    @Override
    public void onServiceDisconnected() {
        hasInit = false;
        CarVehicleManager.getInstance().unregisterCallback(carVehicle);
        LogUtil.d("onServiceDisconnected  unregisterCallback(carVehicle)");
    }

    @Override
    public void onClick(View v) {
        int id = v.getId();
        if (id == R.id.tv_car_set) {
            switchTab(0);
        } else if (id == R.id.tv_driving_mode) {
            switchTab(1);
        } else if (id == R.id.tv_energy_manage) {
            switchTab(2);
        } else if (id == R.id.tv_intelligent_driving) {
            switchTab(3);
        } else if (id == R.id.tv_safe) {
            switchTab(4);
        } else if (id == R.id.tv_maintain) {
            switchTab(5);
        } else if (id == R.id.tv_tourism) {
            switchTab(6);
        } else if (id == R.id.iv_exit) {
            EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.HOME_SWITCH_TAB_CODE, -1));
//            System.exit(0);
            moveTaskToBack(true);
        }
    }

    private void switchTab(int index) {
        if (!hasInit && index != 0) {//未初始化完成不能获取数据，会导致CarSDK不通
            return;
        }
        for (int i = 0; i < mTvList.size(); i++) {
            TextView textView = mTvList.get(i);
            if (i == index) {
                textView.setBackgroundResource(R.drawable.bg_car_control_tab_itm_select);
                textView.setTextColor(getResources().getColor(R.color.tab_select_text_color));
                mImgList.get(i).setImageResource(mImgSelect.get(i));
            } else {
                textView.setBackground(null);
                textView.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
                mImgList.get(i).setImageResource(mImgDefault.get(i));
            }
        }
        EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.HOME_SWITCH_TAB_CODE, index));
        switch (index) {
            case 0://车辆设置
                if (carSetFragment == null) {
                    carSetFragment = new CarSetFragment();
                }
                attach(R.id.content_frame, carSetFragment, "carSetFragment");
                break;
            case 1://驾驶模式
                if (diveModeFragment == null) {
                    diveModeFragment = new DiveModeFragment();
                }
                attach(R.id.content_frame, diveModeFragment, "diveModeFragment");
                break;
            case 2://能量管理
                if (energyManageFragment == null) {
                    energyManageFragment = new EnergyManageFragment();
                }
                attach(R.id.content_frame, energyManageFragment, "energyManageFragment");
                break;
            case 3://智能驾驶
                if (intelligentDrivingFragment == null) {
                    intelligentDrivingFragment = new IntelligentDrivingFragment();
                }
                attach(R.id.content_frame, intelligentDrivingFragment, "intelligentDrivingFragment");
                break;
            case 4://安全&维护
                if (safetyFragment == null) {
                    safetyFragment = new SafetyFragment();
                }
                attach(R.id.content_frame, safetyFragment, "safetyFragment");
                break;
            case 5://维修保养
                if (maintainFragment == null) {
                    maintainFragment = new MaintainFragment();
                }
                attach(R.id.content_frame, maintainFragment, "maintainFragment");
                break;
            case 6://行程信息
                if (tourismFragment == null) {
                    tourismFragment = new TourismFragment();
                }
                attach(R.id.content_frame, tourismFragment, "tourismFragment");
                break;
            default:
                break;
        }
    }

    @Override
    protected void attachBaseContext(Context newBase) {
        super.attachBaseContext(newBase);
        overrideConfiguration(newBase);
    }

    private void overrideConfiguration(Context context) {
        Configuration configuration = context.getResources().getConfiguration();
        configuration.fontScale = 1.0f;
        configuration.densityDpi = 160;
        applyOverrideConfiguration(configuration);
    }

    //车辆信息回调接口
    private final ICarVehicle carVehicle = new ICarVehicle() {
        @Override
        public void onVehicleDrivingSpeedChange(int speed) {
            //车速变化，单位KM
            LogUtil.d("车速变化回调，单位KM： " + speed);
            EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.DRIVING_SPEED_CHANGE, speed));
        }

        @Override
        public void onVehicleInstantaneousPowerConsumptionChange(float value) {
            //瞬时电压，单位V，比如12.5V
        }

        @Override
        public void onVehicleDrivingMillerChange(float miller) {
            // 续航里程,单位KM，如12000.5KM
        }

        @Override
        public void onVehicleTotalDrivingMillerChange(float v) {
            //总里程变化（这里用作维修保养剩余里程更新计算）
            EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.TOTAL_DRIVING_MILLER_CHANGE_CODE, v));
        }

        @Override
        public void onVehicleDrivingTimeChange(String time) {
            // 行驶时间，如30小时12分15秒，最终格式为HH-MM-SS
        }

        @Override
        public void onVehicleGearChange(int gear) {
            //档位信息，具体定义在常量中
            //CAR_GEAR_INFORMATION_P,CAR_GEAR_INFORMATION_R
            //CAR_GEAR_INFORMATION_D,CAR_GEAR_INFORMATION_N
        }

        @Override
        public void onAverageVehicleDrivingSpeedChange(int speed) {
            // 平均车速，单位KM，如70KM
        }

        @Override
        public void onAverageVehicleDrivingMillerChange(float miller) {
            // 平均里程，单位KM，如12355.5KM
        }

        @Override
        public void onAveragePowerConsumption(float value) {
            // 平均电耗
            LogUtil.d("平均电耗回调: " + value);
            EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.AVERAGE_POWER_CONSUMPTION, value));
        }

        @Override
        public void onVehicleTravelMemoryChange(CarTravelInformation carTravelInformation) {
            // 旅行信息变化,Java对象，里面有短期和长期
//            LogUtil.d("旅行信息变化回调 onVehicleTravelMemoryChange: ");
            EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.TRAVEL_MEMORY_CHANGE_CODE, carTravelInformation));
        }

        @Override
        public void onVehicleBatteryInfoChange(BatteryInfo batteryInfo) {
            // 电池信息变化,Java对象，里面有电池相关信息
        }

        @Override
        public void onVehicleWarningModeChange(int i, boolean b) {
            //警告信息发生变化，
            //其中包含 VehicleType.WARNING_LOW_SPEED_PEDESTRIAN 等
//            if (VehicleType.WARNING_LOW_SPEED_PEDESTRIAN == i) {//低速行人警报
//                LogUtil.d("低速行人警报回调： " + b);
//                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.WARNING_MODE_CHANGE_CODE, b));
//            }
        }

        @Override
        public void onVehicleInstrumentLightsChange(int i, boolean b) {
            // 仪表灯相关发生变化
            //VehicleType.LOW_BRAKE_FLUID 制动液灯亮起
            //VehicleType.LOW_BEAM_LIGHT_ON 近光灯开启
            //VehicleType.FOG_LIGHT_ON 雾灯开启
//            if (VehicleType.FOG_LIGHT_ON == i) {//后雾灯开关
//                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.INSTRUMENT_LIGHTS_CHANGE_CODE, b));
//                LogUtil.d("后雾灯开关状态更改回调： ", b);
//            } else if (VehicleType.LOW_BEAM_LIGHT_ON == i) {//近光灯开关
//                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.LOW_BEAM_LIGHT_ON_CHANGE_CODE, b));
//                LogUtil.d("近光灯开关状态更改回调： ", b);
//            }
        }

        //故障接口回调
        @Override
        public void onVehicleFaultPromptChange(int i, int i1) {

        }

        /*@Override
        public void onVehicleInstrumentLightsChange(Map<Integer, Boolean> sparseArray) {
            // 仪表灯相关发生变化，下面是Usage
            // mXXXImageView.setVisibility(sparseArray.get(VehicleType.LOW_BRAKE_FLUID)?:VISIBLE:GONE) 制动液灯亮起
            // mXXXImageView.setVisibility(sparseArray.get(VehicleType.LOW_BEAM_LIGHT_ON)?:VISIBLE:GONE) 近光灯开启
            // mXXXImageView.setVisibility(sparseArray.get(VehicleType.FOG_LIGHT_ON)?:VISIBLE:GONE) 雾灯开启
            if (sparseArray != null && sparseArray.get(VehicleType.FOG_LIGHT_ON) != null) {
                boolean fogLightOn = sparseArray.get(VehicleType.FOG_LIGHT_ON);
                EventBus.getDefault().post(new LightRearFogEventBus(fogLightOn));
                LogUtil.d("后雾灯开关状态更改回调： ", fogLightOn);
            }
        }*/

        @Override
        public void onVehicleDoorStateChange(VehicleDoorInfo vehicleDoorInfo) {
            //车门信息发生变化
        }

        @Override
        public void onSeatBeltStateChange(boolean b) {
            // 安全带信息
        }

        @Override
        public void onEngineStateChange(boolean b) {
            // 电机状态信息
        }

        @Override
        public void onVehicleDriveModeChange(int i) {
            //驾驶模式，具体定义在常量中
            //CAR_MODE_NORMAL，CAR_MODE_SPORT
            //CAR_MODE_ECO，CAR_MODE_CM
            /*EventBus.getDefault().post(new DiveModeEventBean(i));*/
            LogUtil.d("驾驶模式改变回调 onVehicleDriveModeChange: " + i);
            EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.DRIVE_MODE_CHANGE_CODE, i));
        }

        @Override
        public void onVehicleLightHighRateChange(int i) {
            // 车辆大灯高度状态改变
            EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.LIGHT_HIGH_RATE_CHANGE_CODE, i));
            LogUtil.d("车辆大灯高度状态改变回调 onVehicleLightHighRateChange: " + i);
        }

        @Override
        public void onVehicleFatigueDrivingInfoChange(FatigueDrivingInfo fatigueDrivingInfo) {
            // 疲劳驾驶设置
        }

        @Override
        public void onVehicleMaintenanceTipsInfoChange(MaintenanceTips maintenanceTips) {
            // 保养设置发生变化
            EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.MAINTENANCE_TIPS_INFO_CHANGE_CODE, maintenanceTips));
        }

        @Override
        public void onVehicleRangeDisplayModeChange(int i) {
            // 续航里程展示模式 0:CHTC  1:WLTP
            LogUtil.d("续航里程展示模式 回调： " + i + "  模式： " + (i == 0 ? "CHTC" : "WLTP"));
            EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.RANGE_DISPLAY_MODE_CHANGE_CODE, i));
        }

        @Override
        public void onVehicleLockStatusChange(VehicleLockInfo vehicleLockInfo) {
            // 车锁设置
//            EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.LOCK_STATUS_CHANGE_CODE, vehicleLockInfo));
        }

        @Override
        public void onVehicleRightMirrorStatusChange(int i) {
            //右边后视镜状态
            EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.RIGHT_MIRROR_STATUS_CHANGE_CODE, i));
        }

        @Override
        public void onVehicleLeftMirrorStatusChange(int i) {
            // 左边后视镜状态
            EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.LEFT_MIRROR_STATUS_CHANGE_CODE, i));
        }

        @Override
        public void onVehicleEnergyModeChange(Map<Integer, Boolean> map) {
        }

        //充电信息接口回调
        @Override
        public void onChargeInfoChange(ChargeInfo chargeInfo) {

        }

        @Override
        public void onCarWheelInfoChange(CarWheelInfo carWheelInfo) {

        }

        @Override
        public void onVehicleBCMPowerStatusChange(int i) {
            //车辆下电  0 OFF 1ACC 2ON 3Start
            EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.POWER_STATE, i));
            LogUtil.d("车辆下电回调： " + i + (i == 0 ? "   下电" : "   上电"));
        }

        @Override
        public void onVehicleSettingModeChange(int i, int i1) {
            if (i == VehicleType.PERISTALTIC_MODE) {//蠕行模式
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.PERISTALTIC_MODE, i1));
                LogUtil.d("蠕行模式回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_BCM_BACKLADJVALLFB) {//按键背光亮度
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.BACK_LADJVALLFB, i1));
                LogUtil.d("按键背光亮度回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_AC_ACUNLOCKVENTSETFB) {//解锁主动通风
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.AC_UNLOCK_VENT_SET_FB, i1));
                LogUtil.d("解锁主动通风回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_ILLMND_LOCK_STATUS) {//远离闭锁
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.ILLMND_LOCK_STATUS, i1));
                LogUtil.d("远离闭锁回调： " + i1);
            } else if (i == VehicleType.GECKO_VCU_ACCHRGMAXCURR) {//充电电流限制
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.AC_CHARGING_MAX_CURR, i1));
                LogUtil.d("充电电流限制回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_HU_TPMSLEARNINGREQ) {//胎压学习
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.TPMS_LEARNING_REQ, i1));
                LogUtil.d("胎压学习回调： " + i1);
            } else if (i == VehicleType.GECKO_CHRAGINGMODESP || i == VehicleType.GECKO_CCM_APPOINTMENT_MODE ||
                    i == VehicleType.GECKO_CCM_CHRAGING_BEGIN_TIME_HOUR || i == VehicleType.GECKO_CCM_CHRAGING_BEGIN_TIME_MINUTE ||
                    i == VehicleType.GECKO_CCM_CHRAGING_END_TIME_HOUR || i == VehicleType.GECKO_CCM_CHRAGING_END_TIME_MINUTE ||
                    i == VehicleType.GECKO_CCM_CHRAGING_TARGET_SOC) {//预约充电信息，充电目标电量
                EventBus.getDefault().post(new VehicleMessageEventBean(i, i1));
            } else if (i == VehiclePropertyIds.GECKO_FCM_HMASETFB) {//智能远关灯
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.FCM_HMA_ENABLE_SW, i1));
                LogUtil.d("智能远关灯回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_FCM_ICASETFB) {//LCC车道保持
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.FCM_ICAENA_SWT, i1));
                LogUtil.d("LCC车道保持回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_FCM_ICATRUCKSETFB) {//避让大车
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.FCM_ICAAVD_TRUCK_SET, i1));
                LogUtil.d("避让大车回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_FCM_AEBFCW_SETFB) {//前向碰撞预警及紧急制动  0 关闭 1 仅预警 2仅制动 3 预警并制动
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.FCM_FCW_ENABLE_SW, i1));
                LogUtil.d("前向碰撞预警及紧急制动回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_FCM_LKASETFB) {//车道纠偏辅助
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.FCM_LKA_ENABLE_SW, i1));
                LogUtil.d("车道纠偏辅助回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_FCM_LDWSETFB) {//车道偏离预警设置
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.FCM_LDW_ALERTMETHOD_SW, i1));
                LogUtil.d("车道偏离预警回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_FCM_LDWALERTMETHODSETFB) {//LDW车道偏离预警报警方式
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.FCM_LDW_ALERTMETHOD_SETFB, i1));
                LogUtil.d("LDW报警方式回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_FCM_ELKSETFB) {//紧急车道保持
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.FCM_ELKENB_SW, i1));
                LogUtil.d("紧急车道保持回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_CRRR_BSD_SETFB) {//盲区监测
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.CRRR_BSD_ENABLE_SW, i1));
                LogUtil.d("盲区监测回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_CRRR_DOWSETFB) {//开门预警
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.CRRR_DOWENA_SWT, i1));
                LogUtil.d("开门预警回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_CRRR_RCW_SETFB) {//后碰撞预警
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.CRRR_RCW_ENABLE_SW, i1));
                LogUtil.d("后碰撞预警回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_CRRR_RCTA_SETFB) {//后方横向穿行预警
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.CRRR_RCTA_ENABLE_SW, i1));
                LogUtil.d("后方横向穿行预警回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_FCM_SLIFSETFB) {//限速标识识别
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.FCM_SLIF_ENABLE_SW, i1));
                LogUtil.d("限速标识识别回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_FCM_SLWFSETFB) {//超速报警提示
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.FCM_SLWF_ENABLE_SW, i1));
                LogUtil.d("超速报警提示回调： " + i1);
            } else if (i == VehiclePropertyIds.GECKO_HU_NAV_COUNTRYID) {//国家代码
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.HU_NAVI_COUNTRY_ID, i1));
                LogUtil.d("国家代码回调： " + i1);
            } else if (i == VehicleType.GECKO_FCM_ACTDRVASSTSTSHMI) {//ADAS激活
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.FCM_ACT_DRV_ASST_STS_HMI, i1));
                LogUtil.d("ADAS激活回调： " + i1);
            }
        }

        @Override
        public void updateVehicleDialogTips(int i, int i1) {

        }

        @Override
        public void onVehicleChargingLoopDataChanges(List<Integer> list) {
            LogUtil.d("回调 充电开始星期列表日期: " + JSONObject.toJSONString(list));
            EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.CHARGING_LOOP_DATA_CHANGE_CODE, list));
        }

        @Override
        public void onVehicleGlobalIntegerPropertyChange(int i, int i1) {
            //VehicleType.GECKO_CCM_BCM_OPENDOOR_WARN_SET 中控设置开门警示开关
            //VehicleType.GECKO_BCM_DRIVELOCKST 用于设置中控锁的属性
            //VehicleType.GECKO_CCM_ENERGY_RECOV_LEVEL_CMD  用于能量等级设定
            //VehicleType.GECKO_CCM_SCREEN_BRIGHTNESS_ADJUST 用于屏幕亮度调节
            if (i == VehicleType.GECKO_CCM_BCM_OPENDOOR_WARN_SET) {//开门警示
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.OPEN_DOOR_WARNING_CODE, i1));
                LogUtil.d("开门警示回调： " + i1);
            } else if (i == VehicleType.GECKO_WIPER_INTERVAL_TIME) {//雨刮间歇时间
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.WIPER_INTERVAL_TIME, i1));
                LogUtil.d("雨刮间歇时间回调： " + i1);
            } else if (i == VehicleType.GECKO_ACCOMPANY_BACK_HOME_DURATION) {//伴我回家
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.ACCOMPANY_BACK_HOME, i1));
                LogUtil.d("伴我回家回调： " + i1);
            } else if (i == VehicleType.GECKO_WELCOME_UNLOCK_STATUS) {//迎宾解锁
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.WELCOME_UNLOCK_STATUS, i1));
                LogUtil.d("迎宾解锁回调： " + i1);
            } else if (i == VehicleType.GECKO_LOCK_HORN_ENABLE) {//闭锁喇叭
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.LOCK_HORN_ENABLE, i1));
                LogUtil.d("闭锁喇叭回调： " + i1);
            } else if (i == VehicleType.GECKO_AUTO_PARKING_ON) {//自动驻车
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.AUTO_PARKING_ON, i1));
                LogUtil.d("自动驻车回调： " + i1);
            } else if (i == VehicleType.GECKO_BRAKE_DISC_WIPING_ON) {//制动盘擦拭
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.BRAKE_DISC_WIPING_ON, i1));
                LogUtil.d("制动盘擦拭回调： " + i1);
            } else if (i == VehicleType.GECKO_POWER_STEERING_MODE) {//助力转向模式
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.POWER_STEERING_MODE, i1));
                LogUtil.d("助力转向模式回调： " + i1);
            } else if (i == VehicleType.GECKO_ELECTRONIC_PARKING_MODE) {//电子驻车模式
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.ELECTRONIC_PARKING_MODE, i1));
                LogUtil.d("电子驻车模式回调： " + i1);
            } else if (i == VehicleType.GECKO_DRIVING_POWER_ST) {//驾驶驱动强度
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.DRIVING_POWER_ST, i1));
                LogUtil.d("驾驶驱动强度回调： " + i1);
            } else if (i == VehicleType.GECKO_CCM_ENERGY_RECOV_LEVEL_CMD) {//能量回收强度
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.ENERGY_RECOV_LEVEL, i1));
                LogUtil.d("能量回收强度回调： " + i1);
            } else if (i == VehicleType.WARNING_LOW_SPEED_PEDESTRIAN) {//低速行人警报
                LogUtil.d("低速行人警报回调： " + i1);
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.WARNING_MODE_CHANGE_CODE, i1));
            } else if (i == VehicleType.UNLOCK_WHEN_PARKING) {//驻车自动解锁
                LogUtil.d("驻车自动解锁回调： " + i1);
                EventBus.getDefault().post(new VehicleMessageEventBean(EventBusActionCode.LOCK_STATUS_CHANGE_CODE, i1));
            }
        }

        @Override
        public void onVehicleTempChange(VehicleTempInfo vehicleTempInfo) {

        }

        @Override
        public void onFMAutoSeekResult(List<Integer> list) {

        }
    };

    @Override
    protected void onDestroy() {
        super.onDestroy();
        LogUtil.d("CarControlActivity: onDestroy");
//        CarVehicleManager.getInstance().unregisterCallback(carVehicle);
//        MainServiceProxyConnector.getInstance().removeCallback(this);
    }
}