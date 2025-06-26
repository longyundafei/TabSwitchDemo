package com.bihu.carcontrol.module.fragment;

import android.car.VehiclePropertyIds;
import android.os.Bundle;
import android.os.SystemProperties;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
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
import com.bihu.carcontrol.common.event.TestEvent;

import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

/**
 * FileName: CarSetFragment
 * Author: WangXu
 * Date: 2024/6/6 10:45
 * Description: 车辆设置
 */
public class CarSetFragment extends BaseFragment implements View.OnClickListener {
    private ImageView ivWiperIntervalTips, ivAccompanyMeHomeTips, ivBrightnessTips, ivSmartHighBeamTips, ivUnlockWhenParking, ivUnlockWhenParkingTips, ivWelcomeToUnlock, ivWelcomeToUnlockTips, ivAwayFromBlocking, ivAwayFromBlockingTips, ivBlockingHorn, ivBlockingHornTips, ivLowSpeed, ivLowSpeedTips, ivActiveVentilation, ivActiveVentilationTips, ivSmartHighBeam;
    private TextView tvWiper1, tvWiper2, tvWiper3, tvWiper4, tvWiper5, tvSmartHighBeam;
    private boolean isUnlockWhenParking = true, isWelcomeToUnlock = true, isAwayFromBlocking = true, isBlockingHorn = true, isActiveVentilation = true, isLowSpeed = true, isSmartHighBeamOpen = false;
    private boolean unlockWhenParkingClick = false, welcomeToUnlockClick = false, awayFromBlockingClick = false, blockingHornClick = false, activeVentilationClick = false,
            lowSpeedClick = false, smartHighBeamOpenClick = false;
    private List<TextView> mWiperTextViews = new ArrayList<>();
    private List<TextView> mAccompanyTextViews = new ArrayList<>();
    private List<TextView> mKeyTextViews = new ArrayList<>();
    private LinearLayout llSmartHighBeam, llLightControl, llWiper, llAccompanyBackHome, llKeyBackLight;

    private int wiperClickInt = -1;
    private boolean wiperClickClick = false;
    private int accompanyBackHomeInt = -1;
    private boolean accompanyBackHomeClick = false;
    private int KeyBackLightInt = -1;
    private boolean KeyBackLightClick = false;
    private LinearLayout llUnlockWhenParking, llWelcomeToUnlock, llWelcomeToUnlockItem, llAwayFromBlocking, llBlockingHorn, llLowSpeed, llActiveVentilation;

    @Override
    protected int getContentViewId() {
        return R.layout.fragment_car_set;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EventBus.getDefault().register(this);
    }
    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onEvent(TestEvent event) {
        // TODO: 实际处理逻辑
        LogUtil.d("CarSetFragment", "收到事件: " + event.message);
    }
    @Override
    protected void initView(View view) {
        LogUtil.d(" CarSetFragment:  initView");
        ivWiperIntervalTips = view.findViewById(R.id.iv_wiper_interval_tips);
        llWiper = view.findViewById(R.id.ll_wiper);
        tvWiper1 = view.findViewById(R.id.tv_wiper_1);
        tvWiper2 = view.findViewById(R.id.tv_wiper_2);
        tvWiper3 = view.findViewById(R.id.tv_wiper_3);
        tvWiper4 = view.findViewById(R.id.tv_wiper_4);
        tvWiper5 = view.findViewById(R.id.tv_wiper_5);
        mWiperTextViews.add(tvWiper1);
        mWiperTextViews.add(tvWiper2);
        mWiperTextViews.add(tvWiper3);
        mWiperTextViews.add(tvWiper4);
        mWiperTextViews.add(tvWiper5);
        tvWiper1.setOnClickListener(this);
        tvWiper2.setOnClickListener(this);
        tvWiper3.setOnClickListener(this);
        tvWiper4.setOnClickListener(this);
        tvWiper5.setOnClickListener(this);
        ivWiperIntervalTips.setOnClickListener(this);

        NestedScrollView scrollView = view.findViewById(R.id.nested_scroll_view);
        scrollView.setOnScrollChangeListener((View.OnScrollChangeListener) (v, scrollX, scrollY, oldScrollX, oldScrollY) -> MyPopupWindow.getInstant(getContext()).dismissPopupWindow());

        ivAccompanyMeHomeTips = view.findViewById(R.id.iv_accompany_me_home_tips);
        llAccompanyBackHome = view.findViewById(R.id.ll_accompany_back_home);
        TextView tvAccompany1 = view.findViewById(R.id.tv_accompany_1);
        TextView tvAccompany2 = view.findViewById(R.id.tv_accompany_2);
        TextView tvAccompany3 = view.findViewById(R.id.tv_accompany_3);
        TextView tvAccompany4 = view.findViewById(R.id.tv_accompany_4);
        TextView tvAccompany5 = view.findViewById(R.id.tv_accompany_5);
        mAccompanyTextViews.add(tvAccompany1);
        mAccompanyTextViews.add(tvAccompany2);
        mAccompanyTextViews.add(tvAccompany3);
        mAccompanyTextViews.add(tvAccompany4);
        mAccompanyTextViews.add(tvAccompany5);
        tvAccompany1.setOnClickListener(this);
        tvAccompany2.setOnClickListener(this);
        tvAccompany3.setOnClickListener(this);
        tvAccompany4.setOnClickListener(this);
        tvAccompany5.setOnClickListener(this);
        ivAccompanyMeHomeTips.setOnClickListener(this);

        ivBrightnessTips = view.findViewById(R.id.iv_brightness_tips);
        llKeyBackLight = view.findViewById(R.id.ll_key_back_light);
        TextView tvKey1 = view.findViewById(R.id.tv_key_1);
        TextView tvKey2 = view.findViewById(R.id.tv_key_2);
        TextView tvKey3 = view.findViewById(R.id.tv_key_3);
        TextView tvKey4 = view.findViewById(R.id.tv_key_4);
        TextView tvKey5 = view.findViewById(R.id.tv_key_5);
        mKeyTextViews.add(tvKey1);
        mKeyTextViews.add(tvKey2);
        mKeyTextViews.add(tvKey3);
        mKeyTextViews.add(tvKey4);
        mKeyTextViews.add(tvKey5);
        tvKey1.setOnClickListener(this);
        tvKey2.setOnClickListener(this);
        tvKey3.setOnClickListener(this);
        tvKey4.setOnClickListener(this);
        tvKey5.setOnClickListener(this);
        ivBrightnessTips.setOnClickListener(this);

        ivSmartHighBeamTips = view.findViewById(R.id.iv_smart_high_beam_tips);//智能远光灯tips
        llLightControl = view.findViewById(R.id.ll_light_control);
        llSmartHighBeam = view.findViewById(R.id.ll_smart_high_beam);//智能远光灯
        ivSmartHighBeam = view.findViewById(R.id.iv_smart_high_beam);//智能远光灯image
        tvSmartHighBeam = view.findViewById(R.id.tv_smart_high_beam);//智能远光灯TextView
        ivUnlockWhenParking = view.findViewById(R.id.iv_unlock_when_parking);//驻车自动解锁
        llUnlockWhenParking = view.findViewById(R.id.ll_unlock_when_parking);
        ivUnlockWhenParkingTips = view.findViewById(R.id.iv_unlock_tips);//驻车自动解锁tips
        ivWelcomeToUnlock = view.findViewById(R.id.iv_welcome_to_unlock);//迎宾解锁
        llWelcomeToUnlock = view.findViewById(R.id.ll_welcome_to_unlock);
        llWelcomeToUnlockItem = view.findViewById(R.id.ll_welcome_to_unlock_item);
        ivWelcomeToUnlockTips = view.findViewById(R.id.iv_welcome_to_unlock_tips);//迎宾解锁tips
        ivAwayFromBlocking = view.findViewById(R.id.iv_away_from_blocking);//远离闭锁
        llAwayFromBlocking = view.findViewById(R.id.ll_away_from_blocking);
        ivAwayFromBlockingTips = view.findViewById(R.id.iv_away_from_blocking_tips);//远离闭锁tips
        ivBlockingHorn = view.findViewById(R.id.iv_blocking_horn);//闭锁喇叭
        llBlockingHorn = view.findViewById(R.id.ll_blocking_horn);
        ivBlockingHornTips = view.findViewById(R.id.iv_blocking_horn_tips);//闭锁喇叭tips
        ivLowSpeed = view.findViewById(R.id.iv_low_speed);//低速行人报警
        llLowSpeed = view.findViewById(R.id.ll_low_speed);
        ivLowSpeedTips = view.findViewById(R.id.iv_low_speed_tips);//低速行人报警tips
        ivActiveVentilation = view.findViewById(R.id.iv_active_ventilation);//解锁主动通风
        llActiveVentilation = view.findViewById(R.id.ll_active_ventilation);
        ivActiveVentilationTips = view.findViewById(R.id.iv_active_ventilation_tips);//解锁主动通风tips

        llSmartHighBeam.setOnClickListener(this);
        ivSmartHighBeamTips.setOnClickListener(this);
        ivUnlockWhenParking.setOnClickListener(this);
        ivUnlockWhenParkingTips.setOnClickListener(this);
        ivWelcomeToUnlockTips.setOnClickListener(this);
        ivAwayFromBlockingTips.setOnClickListener(this);
        ivBlockingHornTips.setOnClickListener(this);
        ivWelcomeToUnlock.setOnClickListener(this);
        ivAwayFromBlocking.setOnClickListener(this);
        ivBlockingHorn.setOnClickListener(this);
        ivLowSpeed.setOnClickListener(this);
        ivLowSpeedTips.setOnClickListener(this);
        ivActiveVentilation.setOnClickListener(this);
        ivActiveVentilationTips.setOnClickListener(this);

        //智能远光灯配置字 1显示
        String lightControlStr = SystemProperties.get(Constants.LIGHT_CONTROL_STR, "0x0");
        if (lightControlStr.equals(Constants.PROPERTIES_SHOW)) {
            LogUtil.d("智能远光灯配置字: 0x1 显示智能远光灯");
            llLightControl.setVisibility(View.VISIBLE);
        } else {
            LogUtil.d("智能远光灯配置字:  " + lightControlStr + "  不显示智能远光灯");
            llLightControl.setVisibility(View.GONE);
        }
        //迎宾解锁配置字 1显示
        String welcomeToUnlockStr = SystemProperties.get(Constants.WELCOME_TO_UNLOCK_STR, "0x0");
        if (welcomeToUnlockStr.equals(Constants.PROPERTIES_SHOW)) {
            LogUtil.d("迎宾解锁配置字: 0x1 显示迎宾解锁 ");
            llWelcomeToUnlockItem.setVisibility(View.VISIBLE);
        } else {
            LogUtil.d("迎宾解锁配置字:  " + welcomeToUnlockStr + "  不显示迎宾解锁");
            llWelcomeToUnlockItem.setVisibility(View.GONE);
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        LogUtil.d("CarSetFragment onResume");
        CarControlActivity activity = (CarControlActivity) getActivity();
        if (activity == null) {
            LogUtil.d("CarSetFragment  CarControlActivity activity == null");
            return;
        }
        if (ivUnlockWhenParking == null) {
            LogUtil.d("ivUnlockWhenParking == null");
            return;
        }
        if (activity.hasInit) {//SDKCar初始化未完成时不能请求数据，请求会导致中间层后面都不返回任何数据
            initData();
        }
    }

    private void initData() {
        int smartHighBeamOpenInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_HMASETFB);
        LogUtil.d("获取智能远光灯值：" + smartHighBeamOpenInt);
        refreshSmartHighBeamUi(smartHighBeamOpenInt);
        //雨刮间歇时间
        int wiperTimeInt = getWiperTimeInt();
        refreshWiperView(wiperTimeInt);
        LogUtil.d("获取雨刮间歇时间值：" + wiperTimeInt);
        //伴我回家 0 off,1 30s, 2 60s,3 90s, 4 120s
        int accompanyBackInt = getAccompanyBackInt();
        refreshAccompanyBackHomeView(accompanyBackInt);
        LogUtil.d("获取伴我回家值：" + accompanyBackInt);
        //按键背光亮度
        int keyBackLightInt = getKeyBackLightInt();
        updateKeyBackLightView(keyBackLightInt);
        LogUtil.d("获取按键背光亮度：" + keyBackLightInt);
        //驻车自动解锁 0开，1关
        int unlockWhenParkingInt = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.UNLOCK_WHEN_PARKING);
        LogUtil.d("获取驻车自动解锁值： " + unlockWhenParkingInt);
        refreshUnlockWhenParking(unlockWhenParkingInt);
        //迎宾解锁 1开，0关
        int welcomeUnlockInt = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.GECKO_WELCOME_UNLOCK_STATUS);
        refreshWelcomeUnlock(welcomeUnlockInt);
        LogUtil.d("获取迎宾解锁值： " + welcomeUnlockInt);
        //远离闭锁 1开，0关
        int awayFromBlockingInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_ILLMND_LOCK_STATUS);
        refreshAwayFromBlocking(awayFromBlockingInt);
        LogUtil.d("获取远离闭锁值： " + awayFromBlockingInt);
        //闭锁喇叭 0关，1开
        int blockingHornInt = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.GECKO_LOCK_HORN_ENABLE);
        refreshBlockingHorn(blockingHornInt);
        LogUtil.d("获取闭锁喇叭值： " + blockingHornInt);
        //低速行人报警
        int lowSpeedInt = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.WARNING_LOW_SPEED_PEDESTRIAN);
        LogUtil.d("获取低速行人报警值： " + lowSpeedInt);
        refreshLowSpeed(lowSpeedInt);
        //解锁主动通风 1关，2开
        int ActiveVentilationInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_AC_ACUNLOCKVENTSETFB);
        refreshActiveVentilation(ActiveVentilationInt);
        LogUtil.d("获取解锁主动通风值： " + ActiveVentilationInt);
    }

    //获取按键背光亮度
    private int getKeyBackLightInt() {
        return CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_BCM_BACKLADJVALLFB);
    }

    //获取伴我回家 0 off,1 30s, 2 60s,3 90s, 4 120s
    private int getAccompanyBackInt() {
        return CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.GECKO_ACCOMPANY_BACK_HOME_DURATION);
    }

    //获取雨刮间歇时间
    private int getWiperTimeInt() {
        return CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.GECKO_WIPER_INTERVAL_TIME);
    }

    //驻车自动解锁
    private void refreshUnlockWhenParking(int unlockWhenParkingInt) {
        if (Constants.ERROR_CODE == unlockWhenParkingInt) {
            llUnlockWhenParking.setAlpha(Constants.ERROR_ALPHA);
            ivUnlockWhenParking.setClickable(false);
        } else {
            ivUnlockWhenParking.setClickable(true);
            if (llUnlockWhenParking != null && llUnlockWhenParking.getAlpha() < 1.0f) {
                llUnlockWhenParking.setAlpha(1.0f);
            }
            //0 开 1 关
            isUnlockWhenParking = (unlockWhenParkingInt == 0);
            ivUnlockWhenParking.setImageResource(getSwitchRes(isUnlockWhenParking));
        }
    }

    //迎宾解锁
    private void refreshWelcomeUnlock(int welcomeUnlockInt) {
        if (Constants.ERROR_CODE == welcomeUnlockInt) {
            llWelcomeToUnlock.setAlpha(Constants.ERROR_ALPHA);
            ivWelcomeToUnlock.setClickable(false);
        } else {
            if (!ivWelcomeToUnlock.isClickable()) {
                ivWelcomeToUnlock.setClickable(true);
            }
            if (llWelcomeToUnlock != null && llWelcomeToUnlock.getAlpha() < 1.0f) {
                llWelcomeToUnlock.setAlpha(1.0f);
            }
            isWelcomeToUnlock = (welcomeUnlockInt == 1);
            ivWelcomeToUnlock.setImageResource(getSwitchRes(isWelcomeToUnlock));
        }
    }

    //远离闭锁
    private void refreshAwayFromBlocking(int isAwayFromBlockingInt) {
        if (Constants.ERROR_CODE == isAwayFromBlockingInt) {
            llAwayFromBlocking.setAlpha(Constants.ERROR_ALPHA);
            ivAwayFromBlocking.setClickable(false);
        } else {
            if (!ivAwayFromBlocking.isClickable()) {
                ivAwayFromBlocking.setClickable(true);
            }
            if (llAwayFromBlocking != null && llAwayFromBlocking.getAlpha() < 1.0f) {
                llAwayFromBlocking.setAlpha(1.0f);
            }
            isAwayFromBlocking = (isAwayFromBlockingInt == 1);
            ivAwayFromBlocking.setImageResource(getSwitchRes(isAwayFromBlocking));
        }
    }

    //闭锁喇叭
    private void refreshBlockingHorn(int blockingHornInt) {
        if (Constants.ERROR_CODE == blockingHornInt) {
            llBlockingHorn.setAlpha(Constants.ERROR_ALPHA);
            ivBlockingHorn.setClickable(false);
        } else {
            if (!ivBlockingHorn.isClickable()) {
                ivBlockingHorn.setClickable(true);
            }
            if (llBlockingHorn != null && llBlockingHorn.getAlpha() < 1.0f) {
                llBlockingHorn.setAlpha(1.0f);
            }
            isBlockingHorn = (blockingHornInt == 1);
            ivBlockingHorn.setImageResource(getSwitchRes(isBlockingHorn));
        }
    }

    //低速行人报警
    private void refreshLowSpeed(int lowSpeedInt) {
        if (Constants.ERROR_CODE == lowSpeedInt) {//信号丢失
            llLowSpeed.setAlpha(Constants.ERROR_ALPHA);
            ivLowSpeed.setClickable(false);
        } else {
            if (!ivLowSpeed.isClickable()) {
                ivLowSpeed.setClickable(true);
            }
            if (llLowSpeed != null && llLowSpeed.getAlpha() < 1.0f) {
                llLowSpeed.setAlpha(1.0f);
            }
            isLowSpeed = (lowSpeedInt == 1);
            //1 开 0关
            ivLowSpeed.setImageResource(getSwitchRes(isLowSpeed));
        }
    }

    //解锁主动通风
    private void refreshActiveVentilation(int isActiveVentilationInt) {
        if (Constants.ERROR_CODE == isActiveVentilationInt) {
            llActiveVentilation.setAlpha(Constants.ERROR_ALPHA);
            ivActiveVentilation.setClickable(false);
        } else {
            if (!ivActiveVentilation.isClickable()) {
                ivActiveVentilation.setClickable(true);
            }
            if (llActiveVentilation != null && llActiveVentilation.getAlpha() < 1.0f) {
                llActiveVentilation.setAlpha(1.0f);
            }
            //2 开 1 关
            isActiveVentilation = (isActiveVentilationInt == 2);
            ivActiveVentilation.setImageResource(getSwitchRes(isActiveVentilation));
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        EventBus.getDefault().unregister(this);
    }

    private int getSwitchRes(boolean res) {
        return res ? R.drawable.ic_switch_on : R.drawable.ic_switch_off;
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onHandleEvent(VehicleMessageEventBean event) {
        int code = event.getCode();
        if (EventBusActionCode.FRESH_FRAGMENT_DATA == code && (int) event.getCmd() == 0) {//获取车辆设置数据（sdk初始化成功后调用）
            LogUtil.d("刷新车辆设置数据");
            initData();
        } else if (code == EventBusActionCode.FCM_HMA_ENABLE_SW) {//智能远光灯
            int cmd = (int) event.getCmd();
            if (smartHighBeamOpenClick && cmd != Constants.ERROR_CODE && (((int) event.getCmd() == 1) == isSmartHighBeamOpen)) {//下设值和返回值不对应
                showSetupFailureToast();
                LogUtil.d("智能远光灯下设值和返回值不对应下设值: " + !isSmartHighBeamOpen + "  反馈值：" + (cmd == 1));
            }
            smartHighBeamOpenClick = false;
            cancelSmartHighBeamTimeOut();
            refreshSmartHighBeamUi(cmd);
        } else if (code == EventBusActionCode.WIPER_INTERVAL_TIME) {//雨刮间歇时间
            int cmd = (int) event.getCmd();
            if (wiperClickClick && cmd != Constants.ERROR_CODE && (wiperClickInt != cmd)) {//下设值和返回值不对应
                showSetupFailureToast();
                LogUtil.d("雨刮间歇时间下设值和返回值不对应下设值: " + wiperClickInt + "  反馈值：" + cmd);
            }
            wiperClickClick = false;
            cancelWiperTimeOut();
            refreshWiperView(cmd);
        } else if (code == EventBusActionCode.ACCOMPANY_BACK_HOME) {//伴我回家
            int cmd = (int) event.getCmd();
            if (accompanyBackHomeClick && cmd != Constants.ERROR_CODE && (accompanyBackHomeInt != cmd)) {//下设值和返回值不对应
                showSetupFailureToast();
                LogUtil.d("伴我回家下设值和返回值不对应下设值: " + accompanyBackHomeInt + "  反馈值：" + cmd);
            }
            accompanyBackHomeClick = false;
            cancelAccompanyBackHomeTimeOut();
            refreshAccompanyBackHomeView(cmd);
        } else if (code == EventBusActionCode.BACK_LADJVALLFB) {//按键背光亮度
            int cmd = (int) event.getCmd();
            if (KeyBackLightClick && cmd != Constants.ERROR_CODE && (KeyBackLightInt != cmd)) {//下设值和返回值不对应
                showSetupFailureToast();
                LogUtil.d("按键背光亮度下设值和返回值不对应下设值: " + KeyBackLightInt + "  反馈值：" + cmd);
            }
            KeyBackLightClick = false;
            cancelKeyBackLightTimeOut();
            updateKeyBackLightView(cmd);
        } else if (code == EventBusActionCode.LOCK_STATUS_CHANGE_CODE) {//驻车自动解锁
            int cmd = (int) event.getCmd();
            if (unlockWhenParkingClick && cmd != Constants.ERROR_CODE && (((int) event.getCmd() == 0) == isUnlockWhenParking)) {//下设值和返回值不对应
                showSetupFailureToast();
                LogUtil.d("驻车自动解锁下设值和返回值不对应下设值: " + !isUnlockWhenParking + "  反馈值：" + (cmd == 0));
            }
            unlockWhenParkingClick = false;
            cancelUnlockWhenParkingTimeOut();
            refreshUnlockWhenParking(cmd);
        } else if (code == EventBusActionCode.WELCOME_UNLOCK_STATUS) {//迎宾解锁
            int cmd = (int) event.getCmd();
            if (welcomeToUnlockClick && cmd != Constants.ERROR_CODE && (((int) event.getCmd() == 1) == isWelcomeToUnlock)) {//下设值和返回值不对应
                showSetupFailureToast();
                LogUtil.d("迎宾解锁下设值和返回值不对应下设值: " + !isWelcomeToUnlock + "  反馈值：" + (cmd == 1));
            }
            welcomeToUnlockClick = false;
            cancelWelcomeToUnlockTimeOut();
            refreshWelcomeUnlock(cmd);
        } else if (code == EventBusActionCode.ILLMND_LOCK_STATUS) {//远离闭锁
            int cmd = (int) event.getCmd();
            if (awayFromBlockingClick && cmd != Constants.ERROR_CODE && (((int) event.getCmd() == 1) == isAwayFromBlocking)) {//下设值和返回值不对应
                showSetupFailureToast();
                LogUtil.d("远离闭锁下设值和返回值不对应下设值: " + !isAwayFromBlocking + "  反馈值：" + (cmd == 1));
            }
            awayFromBlockingClick = false;
            cancelAwayFromBlockingTimeOut();
            refreshAwayFromBlocking(cmd);
        } else if (code == EventBusActionCode.LOCK_HORN_ENABLE) {//闭锁喇叭
            int cmd = (int) event.getCmd();
            if (blockingHornClick && cmd != Constants.ERROR_CODE && (((int) event.getCmd() == 1) == isBlockingHorn)) {//下设值和返回值不对应
                showSetupFailureToast();
                LogUtil.d("闭锁喇叭下设值和返回值不对应下设值: " + !isBlockingHorn + "  反馈值：" + (cmd == 1));
            }
            blockingHornClick = false;
            cancelBlockingHornTimeOut();
            refreshBlockingHorn(cmd);
        } else if (code == EventBusActionCode.WARNING_MODE_CHANGE_CODE) {//低速行人报警
            int cmd = (int) event.getCmd();
            if (lowSpeedClick && cmd != Constants.ERROR_CODE && (((int) event.getCmd() == 1) == isLowSpeed)) {//下设值和返回值不对应
                showSetupFailureToast();
                LogUtil.d("低速行人报警下设值和返回值不对应下设值: " + !isLowSpeed + "  反馈值：" + (cmd == 1));
            }
            lowSpeedClick = false;
            cancelLowSpeedWarningTimeOut();
            refreshLowSpeed(cmd);
        } else if (code == EventBusActionCode.AC_UNLOCK_VENT_SET_FB) {//解锁主动通风
            int cmd = (int) event.getCmd();
            if (activeVentilationClick && cmd != Constants.ERROR_CODE && (((int) event.getCmd() == 2) == isActiveVentilation)) {//下设值和返回值不对应
                showSetupFailureToast();
                LogUtil.d("解锁主动通风下设值和返回值不对应下设值: " + !isActiveVentilation + "  反馈值：" + (cmd == 2));
            }
            activeVentilationClick = false;
            cancelActiveVentilationTimeOut();
            refreshActiveVentilation(cmd);
        }
    }

    private void showSetupFailureToast() {
        ToastMaster.shortToast(CarSetFragment.this, getString(R.string.setup_failure));
    }

    @Override
    public void onClick(View v) {
        int id = v.getId();
        if (id == R.id.iv_smart_high_beam_tips) {//智能远光灯tips
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivSmartHighBeamTips, getString(R.string.smart_high_beam_tips));
            }
        } else if (id == R.id.iv_wiper_interval_tips) {//雨刮间歇时间tips
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivWiperIntervalTips, getString(R.string.wiper_interval_tips));
            }
        } else if (id == R.id.iv_accompany_me_home_tips) {//伴我回家tips
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivAccompanyMeHomeTips, getString(R.string.accompany_me_home_tips));
            }
        } else if (id == R.id.iv_brightness_tips) {//按键背光亮度tips
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivBrightnessTips, getString(R.string.brightness_tips));
            }
        } else if (id == R.id.iv_unlock_tips) {//驻车自动解锁tips
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivUnlockWhenParkingTips, getString(R.string.unlock_when_parking_tips));
            }
        } else if (id == R.id.iv_welcome_to_unlock_tips) {//迎宾解锁tips
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivWelcomeToUnlockTips, getString(R.string.welcome_to_unlock_tips));
            }
        } else if (id == R.id.iv_away_from_blocking_tips) {//远离闭锁tips
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivAwayFromBlockingTips, getString(R.string.away_from_blocking_tips));
            }
        } else if (id == R.id.iv_blocking_horn_tips) {//闭锁喇叭tips
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivBlockingHornTips, getString(R.string.blocking_horn_tips));
            }
        } else if (id == R.id.iv_low_speed_tips) {//低速行人报警tips
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivLowSpeedTips, getString(R.string.low_speed_tips));
            }
        } else if (id == R.id.iv_active_ventilation_tips) {//解锁主动通风tips
            if (isCanShowPopup()) {
                MyPopupWindow.getInstant(getContext()).showPopupWindowRight(getActivity(), ivActiveVentilationTips, getString(R.string.active_ventilation_tips));
            }
        } else {
            if (DeviceUtil.ifPowerOff() || FastClickCheckUtil.isFastClick(500)) {//信号相关操作需要电源上电才能操作,频繁点击也做过滤不下设
                return;
            }
            if (id == R.id.ll_smart_high_beam) {//智能远光灯 1 ON  2 OFF
                setSmartHighBeam();
            } else if (id == R.id.tv_wiper_1) {//雨刮间歇时间 1档
                if (getWiperTimeInt() != 0) {
                    setWiper(1);
                }
            } else if (id == R.id.tv_wiper_2) {//雨刮间歇时间 2档
                if (getWiperTimeInt() != 1) {
                    setWiper(2);
                }
            } else if (id == R.id.tv_wiper_3) {//雨刮间歇时间 3档
                if (getWiperTimeInt() != 2) {
                    setWiper(3);
                }
            } else if (id == R.id.tv_wiper_4) {//雨刮间歇时间 4档
                if (getWiperTimeInt() != 3) {
                    setWiper(4);
                }
            } else if (id == R.id.tv_wiper_5) {//雨刮间歇时间 5档
                if (getWiperTimeInt() != 4) {
                    setWiper(5);
                }
            } else if (id == R.id.tv_accompany_1) {//伴我回家 OFF
                if (getAccompanyBackInt() != 0) {
                    setAccompanyBackHome(1);
                    LogUtil.d("伴我回家OFF设置： " + 1);
                }
            } else if (id == R.id.tv_accompany_2) {//伴我回家 30秒
                if (getAccompanyBackInt() != 1) {
                    LogUtil.d("伴我回家30秒设置： " + 2);
                    setAccompanyBackHome(2);
                }
            } else if (id == R.id.tv_accompany_3) {//伴我回家 60秒
                if (getAccompanyBackInt() != 2) {
                    LogUtil.d("伴我回家60秒设置： " + 3);
                    setAccompanyBackHome(3);
                }
            } else if (id == R.id.tv_accompany_4) {//伴我回家 90秒
                if (getAccompanyBackInt() != 3) {
                    LogUtil.d("伴我回家90秒设置： " + 4);
                    setAccompanyBackHome(4);
                }
            } else if (id == R.id.tv_accompany_5) {//伴我回家 120秒
                if (getAccompanyBackInt() != 4) {
                    LogUtil.d("伴我回家120秒设置： " + 5);
                    setAccompanyBackHome(5);
                }
            } else if (id == R.id.tv_key_1) {//按键背光亮度 1档
                if (getKeyBackLightInt() != 0) {
                    LogUtil.d("按键背光亮度设置： 1档");
                    setKeyBackLight(1);
                }
            } else if (id == R.id.tv_key_2) {//按键背光亮度 2档
                if (getKeyBackLightInt() != 1) {
                    LogUtil.d("按键背光亮度设置： 2档");
                    setKeyBackLight(2);
                }
            } else if (id == R.id.tv_key_3) {//按键背光亮度 3档
                if (getKeyBackLightInt() != 2) {
                    LogUtil.d("按键背光亮度设置： 3档");
                    setKeyBackLight(3);
                }
            } else if (id == R.id.tv_key_4) {//按键背光亮度 4档
                if (getKeyBackLightInt() != 3) {
                    LogUtil.d("按键背光亮度设置： 4档");
                    setKeyBackLight(4);
                }
            } else if (id == R.id.tv_key_5) {//按键背光亮度 5档
                if (getKeyBackLightInt() != 4) {
                    LogUtil.d("按键背光亮度设置： 5档");
                    setKeyBackLight(5);
                }
            } else if (id == R.id.iv_unlock_when_parking) {//驻车自动解锁
                setUnlockWhenParking();
            } else if (id == R.id.iv_welcome_to_unlock) {//迎宾解锁
                setWelcomeToUnlock();
            } else if (id == R.id.iv_away_from_blocking) {//远离闭锁
                setAwayFromBlocking();
            } else if (id == R.id.iv_blocking_horn) {//闭锁喇叭
                setBlockingHorn();
            } else if (id == R.id.iv_low_speed) {//低速行人报警
                setLowSpeed();
            } else if (id == R.id.iv_active_ventilation) {//解锁主动通风
                setActiveVentilation();
            }
        }
    }

    private boolean isCanShowPopup() {
        return (isAdded() && !isDetached() && getActivity() != null && !getActivity().isFinishing() && !getActivity().isDestroyed());
    }

    //按键背光亮度设置
    private void setKeyBackLight(int value) {
        setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_BCM_BACKLADJVALLFB, value);
        updateKeyBackLightView(value - 1);
        KeyBackLightInt = value - 1;
        KeyBackLightClick = true;
        cancelKeyBackLightTimeOut();
        postKeyBackLightTimeOutDelayed();
    }

    //伴我回家设置
    private void setAccompanyBackHome(int value) {
        setPropertyValue(VehicleType.GECKO_ACCOMPANY_BACK_HOME_DURATION, value);
        refreshAccompanyBackHomeView(value - 1);
//        for (int i = 0; i < mAccompanyTextViews.size(); i++) {
//            TextView textView = mAccompanyTextViews.get(i);
//            textView.setClickable(false);
//        }
        accompanyBackHomeInt = value - 1;
        accompanyBackHomeClick = true;
        cancelAccompanyBackHomeTimeOut();
        postAccompanyBackHomeTimeOutDelayed();
    }

    //雨刮间歇时间设置
    private void setWiper(int value) {
        setPropertyValue(VehicleType.GECKO_WIPER_INTERVAL_TIME, value);
        LogUtil.d("雨刮间歇时间设置:  " + value + "  档");
        refreshWiperView(value - 1);
        wiperClickInt = value - 1;
//        for (int i = 0; i < mWiperTextViews.size(); i++) {
//            TextView textView = mWiperTextViews.get(i);
//            textView.setClickable(false);
//        }
        wiperClickClick = true;
        cancelWiperTimeOut();
        postWiperTimeOutDelayed();
    }

    //智能远光灯
    private void setSmartHighBeam() {
        setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_HMASETFB, !isSmartHighBeamOpen ? 1 : 2);
        LogUtil.d("智能远光灯设置： " + !isSmartHighBeamOpen + "   值：  " + (!isSmartHighBeamOpen ? 1 : 2));
        if (!isSmartHighBeamOpen) {
            llSmartHighBeam.setBackgroundResource(R.drawable.bg_segmented_btn);
            ivSmartHighBeam.setImageResource(R.drawable.ic_full_beam_open);
            tvSmartHighBeam.setTextColor(getResources().getColor(R.color.white));
        } else {
            llSmartHighBeam.setBackgroundResource(R.drawable.bg_segmented_view);
            ivSmartHighBeam.setImageResource(R.drawable.ic_full_beam_close);
            tvSmartHighBeam.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
        }
        llSmartHighBeam.setClickable(false);
        smartHighBeamOpenClick = true;
        cancelSmartHighBeamTimeOut();
        postSmartHighBeamTimeOutDelayed();
    }

    //驻车自动解锁
    private void setUnlockWhenParking() {
        //下设 0关 1开
        CarVehicleManager.getInstance().requestSetVehicleLockMode(VehicleType.UNLOCK_WHEN_PARKING, !isUnlockWhenParking);
        LogUtil.d("驻车自动解锁设置：  " + !isUnlockWhenParking + "  值： " + (!isUnlockWhenParking ? 1 : 0));

        ivUnlockWhenParking.setImageResource(getSwitchRes(!isUnlockWhenParking));
        ivUnlockWhenParking.setClickable(false);
        unlockWhenParkingClick = true;
        cancelUnlockWhenParkingTimeOut();
        postUnlockWhenParkingTimeOutDelayed();
    }

    //迎宾解锁设置
    private void setWelcomeToUnlock() {
        setPropertyValue(VehicleType.GECKO_WELCOME_UNLOCK_STATUS, !isWelcomeToUnlock ? 2 : 1);
        LogUtil.d("迎宾解锁设置  " + !isWelcomeToUnlock + "  值： " + (!isWelcomeToUnlock ? 2 : 1));
        ivWelcomeToUnlock.setImageResource(getSwitchRes(!isWelcomeToUnlock));
        ivWelcomeToUnlock.setClickable(false);
        welcomeToUnlockClick = true;
        cancelWelcomeToUnlockTimeOut();
        postWelcomeToUnlockTimeOutDelayed();
    }

    //远离闭锁设置
    private void setAwayFromBlocking() {
        setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_ILLMND_LOCK_STATUS, !isAwayFromBlocking ? 2 : 1);
        LogUtil.d("远离闭锁设置  " + !isAwayFromBlocking + "  值： " + (!isAwayFromBlocking ? 2 : 1));
        ivAwayFromBlocking.setImageResource(getSwitchRes(!isAwayFromBlocking));
        ivAwayFromBlocking.setClickable(false);
        awayFromBlockingClick = true;
        cancelAwayFromBlockingTimeOut();
        postAwayFromBlockingTimeOutDelayed();
    }

    //闭锁喇叭设置
    private void setBlockingHorn() {
        setPropertyValue(VehicleType.GECKO_LOCK_HORN_ENABLE, !isBlockingHorn ? 2 : 1);
        LogUtil.d("闭锁喇叭设置  " + !isBlockingHorn + "  值： " + (!isBlockingHorn ? 2 : 1));
        ivBlockingHorn.setImageResource(getSwitchRes(!isBlockingHorn));
        ivBlockingHorn.setClickable(false);
        blockingHornClick = true;
        cancelBlockingHornTimeOut();
        postBlockingHornTimeOutDelayed();
    }

    //低速行人报警
    private void setLowSpeed() {
        CarVehicleManager.getInstance().requestSetVehicleWarning(VehicleType.WARNING_LOW_SPEED_PEDESTRIAN, true);
        LogUtil.d("低速行人报警设置: 只要点击就发true (1) ，不发其他值  ");
        ivLowSpeed.setImageResource(getSwitchRes(!isLowSpeed));
        ivLowSpeed.setClickable(false);
        lowSpeedClick = true;
        cancelLowSpeedWarningTimeOut();
        postLowSpeedWarningTimeOutDelayed();
    }

    //解锁主动通风设置
    private void setActiveVentilation() {
        setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_AC_ACUNLOCKVENTSETFB, !isActiveVentilation ? 2 : 1);
        LogUtil.d("解锁主动通风设置  " + !isActiveVentilation + "  值： " + (!isActiveVentilation ? 2 : 1));
        ivActiveVentilation.setImageResource(getSwitchRes(!isActiveVentilation));
        ivActiveVentilation.setClickable(false);
        activeVentilationClick = true;
        cancelActiveVentilationTimeOut();
        postActiveVentilationTimeOutDelayed();
    }

    //智能远光灯UI刷新
    private void refreshSmartHighBeamUi(int smartHighBeamOpenInt) {
        if (Constants.ERROR_CODE == smartHighBeamOpenInt) {
            llSmartHighBeam.setAlpha(Constants.ERROR_ALPHA);
            llSmartHighBeam.setClickable(false);
        } else {
            llSmartHighBeam.setClickable(true);
            if (llSmartHighBeam != null && llSmartHighBeam.getAlpha() < 1.0f) {
                llSmartHighBeam.setAlpha(1.0f);
            }
            isSmartHighBeamOpen = (smartHighBeamOpenInt == 1);
            if (isSmartHighBeamOpen) {
                llSmartHighBeam.setBackgroundResource(R.drawable.bg_segmented_btn);
                ivSmartHighBeam.setImageResource(R.drawable.ic_full_beam_open);
                tvSmartHighBeam.setTextColor(getResources().getColor(R.color.white));
            } else {
                llSmartHighBeam.setBackgroundResource(R.drawable.bg_segmented_view);
                ivSmartHighBeam.setImageResource(R.drawable.ic_full_beam_close);
                tvSmartHighBeam.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            }
        }
    }

    //属性设置
    private void setPropertyValue(int key, int value) {
        CarVehicleManager.getInstance().requestSetIntPropertyAsRoot(key, value);
    }

    private void setIntPropertyByHALVehicle(int key, int value) {
        CarVehicleManager.getInstance().setIntPropertyByHALVehicle(key, value);
    }

    //雨刮间歇时间
    private void refreshWiperView(int value) {
        if (Constants.ERROR_CODE == value) {//信号丢失
            llWiper.setAlpha(Constants.ERROR_ALPHA);
            for (int i = 0; i < mWiperTextViews.size(); i++) {
                TextView textView = mWiperTextViews.get(i);
                textView.setClickable(false);
            }
            return;
        } else {
            if (llWiper != null && llWiper.getAlpha() < 1.0f) {
                llWiper.setAlpha(1.0f);
            }
        }
        for (int i = 0; i < mWiperTextViews.size(); i++) {
            TextView textView = mWiperTextViews.get(i);
            if (!textView.isClickable()) {
                textView.setClickable(true);
            }
            if (i == value) {
                textView.setTextColor(getResources().getColor(R.color.white));
                textView.setBackgroundResource(R.drawable.bg_segmented_btn);
            } else {
                textView.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
                textView.setBackground(null);
            }
        }
    }

    //伴我回家
    private void refreshAccompanyBackHomeView(int value) {
        if (Constants.ERROR_CODE == value) {//信号丢失
            llAccompanyBackHome.setAlpha(Constants.ERROR_ALPHA);
            for (int i = 0; i < mAccompanyTextViews.size(); i++) {
                TextView textView = mAccompanyTextViews.get(i);
                textView.setClickable(false);
            }
            return;
        } else {
            if (llAccompanyBackHome != null && llAccompanyBackHome.getAlpha() < 1.0f) {
                llAccompanyBackHome.setAlpha(1.0f);
            }
        }
        for (int i = 0; i < mAccompanyTextViews.size(); i++) {
            TextView textView = mAccompanyTextViews.get(i);
            if (!textView.isClickable()) {
                textView.setClickable(true);
            }
            if (i == value) {
                textView.setTextColor(getResources().getColor(R.color.white));
                textView.setBackgroundResource(R.drawable.bg_segmented_btn);
            } else {
                textView.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
                textView.setBackground(null);
            }
        }
    }

    //按键背光亮度
    private void updateKeyBackLightView(int value) {
        if (Constants.ERROR_CODE == value) {//信号丢失
            llKeyBackLight.setAlpha(Constants.ERROR_ALPHA);
            for (int i = 0; i < mKeyTextViews.size(); i++) {
                TextView textView = mKeyTextViews.get(i);
                textView.setClickable(false);
            }
            return;
        } else {
            if (llKeyBackLight != null && llKeyBackLight.getAlpha() < 1.0f) {
                llKeyBackLight.setAlpha(1.0f);
            }
        }
        for (int i = 0; i < mKeyTextViews.size(); i++) {
            TextView textView = mKeyTextViews.get(i);
            if (!textView.isClickable()) {
                textView.setClickable(true);
            }
            if (i == value) {
                textView.setTextColor(getResources().getColor(R.color.white));
                textView.setBackgroundResource(R.drawable.bg_segmented_btn);
            } else {
                textView.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
                textView.setBackground(null);
            }
        }
    }

    private Timer keyBackLightTimeOutTimer;
    private TimerTask keyBackLightTimeOutTask;

    private void postKeyBackLightTimeOutDelayed() {
        keyBackLightTimeOutTimer = new Timer();
        keyBackLightTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                llKeyBackLight.post(() -> {
                    showSetupFailureToast();
//                    for (int i = 0; i < mKeyTextViews.size(); i++) {
//                        TextView textView = mKeyTextViews.get(i);
//                        textView.setClickable(true);
//                    }
                    //按键背光亮度
                    int keyBackLightInt = getKeyBackLightInt();
                    updateKeyBackLightView(keyBackLightInt);
                    LogUtil.d("设置失败，设置按键背光亮度超时： " + keyBackLightInt);
                    KeyBackLightClick = false;
                });
            }
        };
        keyBackLightTimeOutTimer.schedule(keyBackLightTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelKeyBackLightTimeOut() {
        if (keyBackLightTimeOutTimer != null) {
            keyBackLightTimeOutTimer.cancel();
            keyBackLightTimeOutTimer = null;
        }
        if (keyBackLightTimeOutTask != null) {
            keyBackLightTimeOutTask.cancel();
            keyBackLightTimeOutTask = null;
        }
    }

    private Timer accompanyBackHomeTimeOutTimer;
    private TimerTask accompanyBackHomeTimeOutTask;

    private void postAccompanyBackHomeTimeOutDelayed() {
        accompanyBackHomeTimeOutTimer = new Timer();
        accompanyBackHomeTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                llAccompanyBackHome.post(() -> {
                    showSetupFailureToast();
//                    for (int i = 0; i < mAccompanyTextViews.size(); i++) {
//                        TextView textView = mAccompanyTextViews.get(i);
//                        textView.setClickable(true);
//                    }
                    accompanyBackHomeClick = false;
                    //伴我回家 0 off,1 30s, 2 60s,3 90s, 4 120s
                    int accompanyBackInt = getAccompanyBackInt();
                    refreshAccompanyBackHomeView(accompanyBackInt);
                    LogUtil.d("设置失败，设置伴我回家(0 off,1 30s, 2 60s,3 90s, 4 120s) 超时： " + accompanyBackInt);
                });
            }
        };
        accompanyBackHomeTimeOutTimer.schedule(accompanyBackHomeTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelAccompanyBackHomeTimeOut() {
        if (accompanyBackHomeTimeOutTimer != null) {
            accompanyBackHomeTimeOutTimer.cancel();
            accompanyBackHomeTimeOutTimer = null;
        }
        if (accompanyBackHomeTimeOutTask != null) {
            accompanyBackHomeTimeOutTask.cancel();
            accompanyBackHomeTimeOutTask = null;
        }
    }

    private Timer wiperTimeOutTimer;
    private TimerTask wiperTimeOutTask;

    private void postWiperTimeOutDelayed() {
        wiperTimeOutTimer = new Timer();
        wiperTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                tvWiper1.post(() -> {
                    showSetupFailureToast();
//                    for (int i = 0; i < mWiperTextViews.size(); i++) {
//                        TextView textView = mWiperTextViews.get(i);
//                        textView.setClickable(true);
//                    }
                    wiperClickClick = false;
                    int wiperTimeInt = getWiperTimeInt();
                    refreshWiperView(wiperTimeInt);
                    LogUtil.d("设置失败，设置雨刮间歇时间超时： " + wiperTimeInt);
                });
            }
        };
        wiperTimeOutTimer.schedule(wiperTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelWiperTimeOut() {
        if (wiperTimeOutTimer != null) {
            wiperTimeOutTimer.cancel();
            wiperTimeOutTimer = null;
        }
        if (wiperTimeOutTask != null) {
            wiperTimeOutTask.cancel();
            wiperTimeOutTask = null;
        }
    }

    private Timer smartHighBeamTimeOutTimer;
    private TimerTask smartHighBeamTimeOutTask;

    private void postSmartHighBeamTimeOutDelayed() {
        smartHighBeamTimeOutTimer = new Timer();
        smartHighBeamTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                llSmartHighBeam.post(() -> {
                    showSetupFailureToast();
                    llSmartHighBeam.setClickable(true);
                    smartHighBeamOpenClick = false;
                    if (isSmartHighBeamOpen) {
                        llSmartHighBeam.setBackgroundResource(R.drawable.bg_segmented_btn);
                        ivSmartHighBeam.setImageResource(R.drawable.ic_full_beam_open);
                        tvSmartHighBeam.setTextColor(getResources().getColor(R.color.white));
                    } else {
                        llSmartHighBeam.setBackgroundResource(R.drawable.bg_segmented_view);
                        ivSmartHighBeam.setImageResource(R.drawable.ic_full_beam_close);
                        tvSmartHighBeam.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
                    }
                    LogUtil.d("设置失败，设置智能远光灯超时： " + isSmartHighBeamOpen);
                });
            }
        };
        smartHighBeamTimeOutTimer.schedule(smartHighBeamTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelSmartHighBeamTimeOut() {
        if (smartHighBeamTimeOutTimer != null) {
            smartHighBeamTimeOutTimer.cancel();
            smartHighBeamTimeOutTimer = null;
        }
        if (smartHighBeamTimeOutTask != null) {
            smartHighBeamTimeOutTask.cancel();
            smartHighBeamTimeOutTask = null;
        }
    }

    private Timer unlockWhenParkingTimeOutTimer;
    private TimerTask unlockWhenParkingTimeOutTask;

    //驻车自动解锁
    private void postUnlockWhenParkingTimeOutDelayed() {
        unlockWhenParkingTimeOutTimer = new Timer();
        unlockWhenParkingTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivUnlockWhenParking.post(() -> {
                    showSetupFailureToast();
                    ivUnlockWhenParking.setClickable(true);
                    ivUnlockWhenParking.setImageResource(getSwitchRes(isUnlockWhenParking));
                    LogUtil.d("设置失败，设置驻车自动解锁超时： " + isUnlockWhenParking);
                    unlockWhenParkingClick = false;
                });
            }
        };
        unlockWhenParkingTimeOutTimer.schedule(unlockWhenParkingTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelUnlockWhenParkingTimeOut() {
        if (unlockWhenParkingTimeOutTimer != null) {
            unlockWhenParkingTimeOutTimer.cancel();
            unlockWhenParkingTimeOutTimer = null;
        }
        if (unlockWhenParkingTimeOutTask != null) {
            unlockWhenParkingTimeOutTask.cancel();
            unlockWhenParkingTimeOutTask = null;
        }
    }

    private Timer welcomeToUnlockTimeOutTimer;
    private TimerTask welcomeToUnlockTimeOutTask;

    private void postWelcomeToUnlockTimeOutDelayed() {
        welcomeToUnlockTimeOutTimer = new Timer();
        welcomeToUnlockTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivWelcomeToUnlock.post(() -> {
                    showSetupFailureToast();
                    ivWelcomeToUnlock.setClickable(true);
                    ivWelcomeToUnlock.setImageResource(getSwitchRes(isWelcomeToUnlock));
                    LogUtil.d("设置失败，设置迎宾解锁超时： " + isWelcomeToUnlock);
                    welcomeToUnlockClick = false;
                });
            }
        };
        welcomeToUnlockTimeOutTimer.schedule(welcomeToUnlockTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelWelcomeToUnlockTimeOut() {
        if (welcomeToUnlockTimeOutTimer != null) {
            welcomeToUnlockTimeOutTimer.cancel();
            welcomeToUnlockTimeOutTimer = null;
        }
        if (welcomeToUnlockTimeOutTask != null) {
            welcomeToUnlockTimeOutTask.cancel();
            welcomeToUnlockTimeOutTask = null;
        }
    }

    private Timer awayFromBlockingTimeOutTimer;
    private TimerTask awayFromBlockingTimeOutTask;

    private void postAwayFromBlockingTimeOutDelayed() {
        awayFromBlockingTimeOutTimer = new Timer();
        awayFromBlockingTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivWelcomeToUnlock.post(() -> {
                    showSetupFailureToast();
                    ivAwayFromBlocking.setClickable(true);
                    awayFromBlockingClick = false;
                    ivAwayFromBlocking.setImageResource(getSwitchRes(isAwayFromBlocking));
                    LogUtil.d("设置失败，设置远离闭锁超时： " + isAwayFromBlocking);
                });
            }
        };
        awayFromBlockingTimeOutTimer.schedule(awayFromBlockingTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelAwayFromBlockingTimeOut() {
        if (awayFromBlockingTimeOutTimer != null) {
            awayFromBlockingTimeOutTimer.cancel();
            awayFromBlockingTimeOutTimer = null;
        }
        if (awayFromBlockingTimeOutTask != null) {
            awayFromBlockingTimeOutTask.cancel();
            awayFromBlockingTimeOutTask = null;
        }
    }

    private Timer lowSpeedWarningTimeOutTimer;
    private TimerTask lowSpeedWarningTimeOutTask;

    private void postLowSpeedWarningTimeOutDelayed() {
        lowSpeedWarningTimeOutTimer = new Timer();
        lowSpeedWarningTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivLowSpeed.post(() -> {
                    showSetupFailureToast();
                    ivLowSpeed.setClickable(true);
                    lowSpeedClick = false;
                    ivLowSpeed.setImageResource(getSwitchRes(isLowSpeed));
                    LogUtil.d("设置失败，设置低速行人超时： " + isLowSpeed);
                });
            }
        };
        lowSpeedWarningTimeOutTimer.schedule(lowSpeedWarningTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelLowSpeedWarningTimeOut() {
        if (lowSpeedWarningTimeOutTimer != null) {
            lowSpeedWarningTimeOutTimer.cancel();
            lowSpeedWarningTimeOutTimer = null;
        }
        if (lowSpeedWarningTimeOutTask != null) {
            lowSpeedWarningTimeOutTask.cancel();
            lowSpeedWarningTimeOutTask = null;
        }
    }

    private Timer blockingHornTimeOutTimer;
    private TimerTask blockingHornTimeOutTask;

    private void postBlockingHornTimeOutDelayed() {
        blockingHornTimeOutTimer = new Timer();
        blockingHornTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivBlockingHorn.post(() -> {
                    showSetupFailureToast();
                    ivBlockingHorn.setClickable(true);
                    blockingHornClick = false;
                    ivBlockingHorn.setImageResource(getSwitchRes(isBlockingHorn));
                    LogUtil.d("设置失败，设置闭锁喇叭超时： " + isBlockingHorn);
                });
            }
        };
        blockingHornTimeOutTimer.schedule(blockingHornTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelBlockingHornTimeOut() {
        if (blockingHornTimeOutTimer != null) {
            blockingHornTimeOutTimer.cancel();
            blockingHornTimeOutTimer = null;
        }
        if (blockingHornTimeOutTask != null) {
            blockingHornTimeOutTask.cancel();
            blockingHornTimeOutTask = null;
        }
    }

    private Timer activeVentilationTimeOutTimer;
    private TimerTask activeVentilationTimeOutTask;

    private void postActiveVentilationTimeOutDelayed() {
        activeVentilationTimeOutTimer = new Timer();
        activeVentilationTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivActiveVentilation.post(() -> {
                    showSetupFailureToast();
                    ivActiveVentilation.setClickable(true);
                    activeVentilationClick = false;
                    ivActiveVentilation.setImageResource(getSwitchRes(isActiveVentilation));
                    LogUtil.d("设置失败，设置解锁主动通风超时： " + isActiveVentilation);
                });
            }
        };
        activeVentilationTimeOutTimer.schedule(activeVentilationTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelActiveVentilationTimeOut() {
        if (activeVentilationTimeOutTimer != null) {
            activeVentilationTimeOutTimer.cancel();
            activeVentilationTimeOutTimer = null;
        }
        if (activeVentilationTimeOutTask != null) {
            activeVentilationTimeOutTask.cancel();
            activeVentilationTimeOutTask = null;
        }
    }
}
