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
import com.bihu.carcontrol.common.bean.CountryBean;
import com.bihu.carcontrol.common.event.EventBusActionCode;
import com.bihu.carcontrol.common.event.VehicleMessageEventBean;
import com.bihu.carcontrol.common.util.AdasDialogUtil;
import com.bihu.carcontrol.common.util.Constants;
import com.bihu.carcontrol.common.util.CountrySelectDialog;
import com.bihu.carcontrol.common.util.Debounce;
import com.bihu.carcontrol.common.util.DeviceUtil;
import com.bihu.carcontrol.common.util.DialogUtil;
import com.bihu.carcontrol.common.util.FastClickCheckUtil;
import com.bihu.carcontrol.common.util.LogUtil;
import com.bihu.carcontrol.common.util.SPUtil;
import com.bihu.carcontrol.common.util.ToastMaster;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;
import java.util.Timer;
import java.util.TimerTask;

/**
 * FileName: IntelligentDrivingFragment
 * Author: WangXu
 * Date: 2024/6/7 10:51
 * Description: 智能驾驶
 */
public class IntelligentDrivingFragment extends BaseFragment implements View.OnClickListener {
    private TextView tvLaneClose, tvSound, tvVibration, tvSoundAndVibration, tvCollisionWarningBrakingClose, tvCollisionWarning, tvBraking, tvCollisionWarningBraking, tvCountry;
    private final List<TextView> mLaneDepartureWarningTvs = new ArrayList<>();
    private final List<TextView> mCollisionWarningBrakingTvs = new ArrayList<>();
    private boolean isLaneChangeAssist = true, isAvoidBigCars = true, isLaneCorrectionAssistance = true, isEmergencyLaneKeeping = true, isBlindSpotDetection = true,
            isDoorOpeningWarning = true, isRearCollisionWarning = true, isAsternTraverseWarning = true, isSpeedLimit = true, isOverSpeedWarning = true;
    private boolean laneChangeAssistClick = false, avoidBigCarsClick = false, laneCorrectionAssistanceClick = false, emergencyLaneKeepingClick = false,
            blindSpotDetectionClick = false, doorOpeningWarningClick = false, rearCollisionWarningClick = false, asternTraverseWarningClick = false,
            speedLimitClick = false, overSpeedWarningClick = false, forwardCollisionWarningBrakingClick = false, laneDepartureWarningClick = false,
            laneDepartureWarningCloseClick = false;
    private ImageView ivLccLaneKeeping, ivAvoidBigCars, ivLaneCorrectionAssistance, ivEmergencyLaneKeeping, ivDoorOpeningWarning, ivBlindSpotDetection,
            ivRearCollisionWarning, ivAsternTraverseWarning, ivSpeedLimit, ivOverSpeedWarning;
    private int countryIdInt = 0;
    private CountrySelectDialog countrySelectDialog;
    private List<CountryBean> countryBeans;
    private Dialog collisionWarningBrakingCloseDialog;
    private Dialog collisionWarningDialog;
    private Dialog collisionBrakingDialog;
    private Dialog emergencyLaneKeepingDialog;
    private AdasDialogUtil cruiseAssistTipsDialog;
    private AdasDialogUtil laneDepartureAssistTipsDialog;
    private Dialog forwardWarningAidDialog;
    private AdasDialogUtil lateralWarningAidDialog;
    private AdasDialogUtil backwardBlindAidDialog;
    private AdasDialogUtil intelligentRateLimitingAidDialog;
    private Debounce debounce;
    private LinearLayout llLccLaneKeeping, llAvoidBigCars, llLaneDepartureWarning, llLaneDepartureAssistItem, llLaneCorrectionAssistance, llEmergencyLaneKeeping,
            llEmergencyLaneKeepingItem, llBlindSpotDetection, llDoorOpeningWarning, llForwardWarningAidItem, llRearCollisionWarning, llAsternTraverseWarning,
            llCollisionWarningBraking, llSpeedLimit, llOverSpeedWarning, llLateralWarningAidItem, llBackwardBlindAidItem, llLimitingAidItem;
    private int LaneDepartureWarningClickInt = -1;
    private int forwardCollisionWarningBrakingClickInt = -1;
    private Dialog lccLanKeepingConfirmDialog;

    @Override
    protected int getContentViewId() {
        return R.layout.fragment_intelligent_driving;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EventBus.getDefault().register(this);
    }

    @Override
    protected void initView(View view) {
        //车道偏离预警
        llLaneDepartureWarning = view.findViewById(R.id.ll_lane_departure_warning);
        tvLaneClose = view.findViewById(R.id.tv_lane_close);
        tvSound = view.findViewById(R.id.tv_warning_only);
        tvVibration = view.findViewById(R.id.tv_vibration);
        tvSoundAndVibration = view.findViewById(R.id.tv_warning_and_vibration);
        mLaneDepartureWarningTvs.add(tvLaneClose);
        mLaneDepartureWarningTvs.add(tvVibration);
        mLaneDepartureWarningTvs.add(tvSound);
        mLaneDepartureWarningTvs.add(tvSoundAndVibration);
        tvLaneClose.setOnClickListener(this);
        tvSound.setOnClickListener(this);
        tvVibration.setOnClickListener(this);
        tvSoundAndVibration.setOnClickListener(this);

        //前向碰撞预警及紧急制动
        llCollisionWarningBraking = view.findViewById(R.id.ll_collision_warning_braking);
        tvCollisionWarningBrakingClose = view.findViewById(R.id.collision_warning_braking_close);
        tvCollisionWarning = view.findViewById(R.id.collision_warning);
        tvBraking = view.findViewById(R.id.braking);
        tvCollisionWarningBraking = view.findViewById(R.id.collision_warning_braking);
        mCollisionWarningBrakingTvs.add(tvCollisionWarningBrakingClose);
        mCollisionWarningBrakingTvs.add(tvCollisionWarning);
        mCollisionWarningBrakingTvs.add(tvBraking);
        mCollisionWarningBrakingTvs.add(tvCollisionWarningBraking);
        tvCollisionWarningBrakingClose.setOnClickListener(this);
        tvCollisionWarning.setOnClickListener(this);
        tvBraking.setOnClickListener(this);
        tvCollisionWarningBraking.setOnClickListener(this);

        ivLccLaneKeeping = view.findViewById(R.id.iv_lcc_lane_keeping);
        llLccLaneKeeping = view.findViewById(R.id.ll_lcc_lane_keeping);
        ivLccLaneKeeping.setOnClickListener(this);
        ivAvoidBigCars = view.findViewById(R.id.iv_avoid_big_cars);
        ivAvoidBigCars.setOnClickListener(this);
        llAvoidBigCars = view.findViewById(R.id.ll_avoid_big_cars);
        ivLaneCorrectionAssistance = view.findViewById(R.id.iv_lane_correction_assistance);
        llLaneCorrectionAssistance = view.findViewById(R.id.ll_lane_correction_assistance);
        ivLaneCorrectionAssistance.setOnClickListener(this);
        ivEmergencyLaneKeeping = view.findViewById(R.id.iv_emergency_lane_keeping);
        llEmergencyLaneKeeping = view.findViewById(R.id.ll_emergency_lane_keeping);
        ivEmergencyLaneKeeping.setOnClickListener(this);
        ivBlindSpotDetection = view.findViewById(R.id.iv_blind_spot_detection);
        llBlindSpotDetection = view.findViewById(R.id.ll_blind_spot_detection);
        ivBlindSpotDetection.setOnClickListener(this);
        ivDoorOpeningWarning = view.findViewById(R.id.iv_door_opening_warning);
        llDoorOpeningWarning = view.findViewById(R.id.ll_door_opening_warning);
        ivDoorOpeningWarning.setOnClickListener(this);
        ivRearCollisionWarning = view.findViewById(R.id.iv_rear_collision_warning);
        llRearCollisionWarning = view.findViewById(R.id.ll_rear_collision_warning);
        ivRearCollisionWarning.setOnClickListener(this);
        ivAsternTraverseWarning = view.findViewById(R.id.iv_astern_traverse_warning);
        llAsternTraverseWarning = view.findViewById(R.id.ll_astern_traverse_warning);
        ivAsternTraverseWarning.setOnClickListener(this);
        ivSpeedLimit = view.findViewById(R.id.iv_speed_limit);
        llSpeedLimit = view.findViewById(R.id.ll_speed_limit);
        ivSpeedLimit.setOnClickListener(this);
        ivOverSpeedWarning = view.findViewById(R.id.iv_over_speed_warning);
        llOverSpeedWarning = view.findViewById(R.id.ll_over_speed_warning);
        ivOverSpeedWarning.setOnClickListener(this);
        tvCountry = view.findViewById(R.id.tv_country);
        tvCountry.setOnClickListener(this);
        //LCC车道居中辅助配置字
        String lccLaneKeepingStr = SystemProperties.get(Constants.LCC_LANE_KEEPING_STR, "0x0");
        if (lccLaneKeepingStr.equals(Constants.PROPERTIES_SHOW)) {
            LogUtil.d("车道居中辅助配置字： 0x1  显示LCC车道居中辅助、避让大车");
            llLccLaneKeeping.setVisibility(View.VISIBLE);
        } else {
            LogUtil.d("车道居中辅助配置字：" + lccLaneKeepingStr + "  不显示LCC车道居中辅助、避让大车");
            llLccLaneKeeping.setVisibility(View.GONE);
        }

        //车道偏离预警及纠正
        llLaneDepartureAssistItem = view.findViewById(R.id.ll_lane_departure_assist_item);
        String laneDepartureAssistStr = SystemProperties.get(Constants.LANE_DEPARTURE_ASSIST_STR, "0x0");
        if (laneDepartureAssistStr.equals(Constants.PROPERTIES_SHOW)) {
            LogUtil.d("车道偏离预警及纠正配置字： 0x1  显示车道偏离预警及纠正");
            llLaneDepartureAssistItem.setVisibility(View.VISIBLE);
        } else {
            LogUtil.d("车道偏离预警及纠正配置字：" + laneDepartureAssistStr + "  不显示车道偏离预警及纠正");
            llLaneDepartureAssistItem.setVisibility(View.GONE);
        }

        //紧急车道保持
        llEmergencyLaneKeepingItem = view.findViewById(R.id.ll_emergency_lane_keeping_item);
        String emergencyLaneKeepingStr = SystemProperties.get(Constants.EMERGENCY_LANE_KEEPING_STR, "0x0");
        if (emergencyLaneKeepingStr.equals(Constants.PROPERTIES_SHOW)) {
            LogUtil.d("紧急车道保持配置字： 0x1  显示紧急车道保持");
            llEmergencyLaneKeepingItem.setVisibility(View.VISIBLE);
        } else {
            LogUtil.d("紧急车道保持配置字：" + emergencyLaneKeepingStr + "  不显示紧急车道保持");
            llEmergencyLaneKeepingItem.setVisibility(View.GONE);
        }

        //前向主动安全辅助
        llForwardWarningAidItem = view.findViewById(R.id.ll_forward_warning_aid_item);
        String forwardWarningAidStr = SystemProperties.get(Constants.FORWARD_WARNING_AID_STR, "0x0");
        if (forwardWarningAidStr.equals(Constants.PROPERTIES_SHOW)) {
            LogUtil.d("前向主动安全辅助配置字： 0x1  显示前向主动安全辅助");
            llForwardWarningAidItem.setVisibility(View.VISIBLE);
        } else {
            LogUtil.d("前向主动安全辅助配置字：" + forwardWarningAidStr + "  不显示前向主动安全辅助");
            llForwardWarningAidItem.setVisibility(View.GONE);
        }
        //侧向盲区辅助
        llLateralWarningAidItem = view.findViewById(R.id.ll_lateral_warning_aid_item);
        String lateralWarningAidStr = SystemProperties.get(Constants.LATERAL_WARNING_AID_STR, "0x0");
        if (lateralWarningAidStr.equals(Constants.PROPERTIES_SHOW)) {
            LogUtil.d("侧向盲区辅助配置字： 0x1  显示侧向盲区辅助");
            llLateralWarningAidItem.setVisibility(View.VISIBLE);
        } else {
            LogUtil.d("侧向盲区辅助配置字：" + lateralWarningAidStr + "  不显示侧向盲区辅助");
            llLateralWarningAidItem.setVisibility(View.GONE);
        }
        //后向盲区辅助
        llBackwardBlindAidItem = view.findViewById(R.id.ll_backward_blind_aid_item);
        String backwardBlindAidStr = SystemProperties.get(Constants.BACKWARD_BLIND_AID_STR, "0x0");
        if (backwardBlindAidStr.equals(Constants.PROPERTIES_SHOW)) {
            LogUtil.d("后向盲区辅助配置字： 0x1  显示后向盲区辅助");
            llBackwardBlindAidItem.setVisibility(View.VISIBLE);
        } else {
            LogUtil.d("后向盲区辅助配置字：" + backwardBlindAidStr + "  不显示后向盲区辅助");
            llBackwardBlindAidItem.setVisibility(View.GONE);
        }
        //智能限速辅助
        llLimitingAidItem = view.findViewById(R.id.ll_limiting_aid_item);
        String limitingAidStr = SystemProperties.get(Constants.LIMITING_AID_STR, "0x0");
        if (limitingAidStr.equals(Constants.PROPERTIES_SHOW)) {
            LogUtil.d("智能限速辅助配置字： 0x1  显示智能限速辅助");
            llLimitingAidItem.setVisibility(View.VISIBLE);
        } else {
            LogUtil.d("智能限速辅助配置字：" + limitingAidStr + "  不显示智能限速辅助");
            llLimitingAidItem.setVisibility(View.GONE);
        }

        view.findViewById(R.id.iv_cruise_assist_tips).setOnClickListener(this);
        view.findViewById(R.id.iv_lane_departure_assist_tips).setOnClickListener(this);
        view.findViewById(R.id.iv_emergency_lane_keeping_tips).setOnClickListener(this);
        view.findViewById(R.id.iv_forward_warning_aid_tips).setOnClickListener(this);
        view.findViewById(R.id.iv_lateral_warning_aid_tips).setOnClickListener(this);
        view.findViewById(R.id.iv_backward_blind_aid_tips).setOnClickListener(this);
        view.findViewById(R.id.iv_intelligent_rate_limiting_aid_tips).setOnClickListener(this);

        countryBeans = new ArrayList<>();
        countryBeans.add(new CountryBean(getString(R.string.china), 0));
        countryBeans.add(new CountryBean(getString(R.string.france), 1));
        countryBeans.add(new CountryBean(getString(R.string.germany), 2));
        countryBeans.add(new CountryBean(getString(R.string.italy), 3));
        countryBeans.add(new CountryBean(getString(R.string.netherlands), 4));
        countryBeans.add(new CountryBean(getString(R.string.belgium), 5));
        countryBeans.add(new CountryBean(getString(R.string.luxembourg), 6));
        countryBeans.add(new CountryBean(getString(R.string.denmark), 7));
        countryBeans.add(new CountryBean(getString(R.string.ireland), 8));
        countryBeans.add(new CountryBean(getString(R.string.greece), 9));
        countryBeans.add(new CountryBean(getString(R.string.portugal), 10));
        countryBeans.add(new CountryBean(getString(R.string.spain), 11));
        countryBeans.add(new CountryBean(getString(R.string.austria), 12));
        countryBeans.add(new CountryBean(getString(R.string.sweden), 13));
        countryBeans.add(new CountryBean(getString(R.string.finland), 14));
        countryBeans.add(new CountryBean(getString(R.string.malta), 15));
        countryBeans.add(new CountryBean(getString(R.string.cyprus), 16));
        countryBeans.add(new CountryBean(getString(R.string.poland), 17));
        countryBeans.add(new CountryBean(getString(R.string.hungary), 18));
        countryBeans.add(new CountryBean(getString(R.string.czech_republic), 19));
        countryBeans.add(new CountryBean(getString(R.string.slovakia), 20));
        countryBeans.add(new CountryBean(getString(R.string.slovenia), 21));
        countryBeans.add(new CountryBean(getString(R.string.estonia), 22));
        countryBeans.add(new CountryBean(getString(R.string.latvia), 23));
        countryBeans.add(new CountryBean(getString(R.string.lithuania), 24));
        countryBeans.add(new CountryBean(getString(R.string.romania), 25));
        countryBeans.add(new CountryBean(getString(R.string.bulgaria), 26));
        countryBeans.add(new CountryBean(getString(R.string.croatia), 27));
        countryBeans.add(new CountryBean(getString(R.string.britain), 28));
        countryBeans.add(new CountryBean(getString(R.string.norway), 29));
        countryBeans.add(new CountryBean(getString(R.string.switzerland), 30));
        countryBeans.add(new CountryBean(getString(R.string.other), 63));

        debounce = new Debounce(100); // 100ms 前向碰撞防抖

    }

    @Override
    public void onResume() {
        super.onResume();
        initData();
    }

    private void initData() {
        //LCC车道居中辅助
        int lccLaneKeepingInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_ICASETFB);
        LogUtil.d("获取LCC车道居中辅助： " + lccLaneKeepingInt);
        refreshLccLanKeeping(lccLaneKeepingInt);
        //避让大车
        int avoidBigCarsInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_ICATRUCKSETFB);
        LogUtil.d("获取避让大车： " + avoidBigCarsInt);
        refreshAvoidBigCars(avoidBigCarsInt);

        //ADAS激活状态
        int adasStsHmi = CarVehicleManager.getInstance().requestIntPropertyAsRoot(VehicleType.GECKO_FCM_ACTDRVASSTSTSHMI);
        LogUtil.d("获取ADAS激活状态（2已激活 其他：未激活）： " + adasStsHmi);
        refreshAdasStsHmi(adasStsHmi);

        //车道纠偏辅助
        int laneCorrectionAssistanceInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_LKASETFB);
        LogUtil.d("获取车道纠偏辅助： " + laneCorrectionAssistanceInt);
        refreshLaneCorrectionAssistance(laneCorrectionAssistanceInt);
        //车道偏离预警设置 1 ON 0 OFF
        int laneDepartureWarningInt = getLaneDepartureWarningInt();
        if (laneDepartureWarningInt == Constants.ERROR_CODE) {
            refreshLaneDepartureWarningUi(laneDepartureWarningInt);
        } else {
            refreshLaneDepartureWarningUi(laneDepartureWarningInt == 1 ? -1 : 0);//-1车道偏离预警打开UI按钮置灰 0 车道偏离预警关闭ui显示高亮
        }
        //LDW报警方式 1仅振动 2 仅预警 3 预警并振动
        int LDWWarningInt = getLDWWarningInt();
        if (LDWWarningInt == Constants.ERROR_CODE) {
            refreshLaneDepartureWarningUi(LDWWarningInt);
        } else {
            if (laneDepartureWarningInt == 1 && LDWWarningInt > 0) {//车道偏离打开才能显示LDW报警是什么方式
                refreshLaneDepartureWarningUi(LDWWarningInt);
            }
        }
        LogUtil.d("获取LDW报警方式： " + LDWWarningInt);
        //紧急车道保持
        int emergencyLaneKeepingInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_ELKSETFB);
        refreshEmergencyLaneKeeping(emergencyLaneKeepingInt);
        LogUtil.d("获取紧急车道保持： " + emergencyLaneKeepingInt);
        //前向碰撞预警及紧急制动 0 关闭 1 仅预警 2仅制动 3 预警并制动
        int forwardCollisionWarningBrakingInt = getForwardCollisionWarningBrakingInt();
        refreshForwardCollisionWarningBraking(forwardCollisionWarningBrakingInt);
        LogUtil.d("获取前向碰撞预警及紧急制动： " + forwardCollisionWarningBrakingInt);
        if (forwardCollisionWarningBrakingInt == 0) {
            LogUtil.d("------- 关闭");
        } else if (forwardCollisionWarningBrakingInt == 1) {
            LogUtil.d("------- 仅预警");
        } else if (forwardCollisionWarningBrakingInt == 2) {
            LogUtil.d("------- 2仅制动");
        } else if (forwardCollisionWarningBrakingInt == 3) {
            LogUtil.d("------- 预警并制动");
        }
        //盲区监测
        int blindSpotDetectionInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_CRRR_BSD_SETFB);
        refreshBlindSpotDetection(blindSpotDetectionInt);
        LogUtil.d("获取盲区监测： " + blindSpotDetectionInt);
        //开门预警
        int doorOpeningWarningInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_CRRR_DOWSETFB);
        refreshDoorOpeningWarning(doorOpeningWarningInt);
        LogUtil.d("获取开门预警： " + doorOpeningWarningInt);
        //后向碰撞预警
        int rearCollisionWarningInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_CRRR_RCW_SETFB);
        refreshRearCollisionWarning(rearCollisionWarningInt);
        LogUtil.d("获取后向碰撞预警： " + rearCollisionWarningInt);
        //后方横向穿行预警
        int asternTraverseWarningInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_CRRR_RCTA_SETFB);
        refreshAsternTraverseWarning(asternTraverseWarningInt);
        LogUtil.d("获取后方横向穿行预警： " + asternTraverseWarningInt);
        //限速标识识别
        int speedLimitInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_SLIFSETFB);
        refreshSpeedLimit(speedLimitInt);
        LogUtil.d("获取限速标识识别： " + speedLimitInt);
        //超速报警提示
        int overSpeedWarningInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_SLWFSETFB);
        refreshOverSpeedWarning(overSpeedWarningInt);
        LogUtil.d("获取超速报警提示： " + overSpeedWarningInt);
        //国家代码
        countryIdInt = SPUtil.getInt(Objects.requireNonNull(getContext()), SPUtil.KEY_COUNTRY_ID, 0);
        refreshCountryIdUi(countryIdInt);
        LogUtil.d("获取国家代码： " + countryIdInt);
    }

    //ADAS激活状态
    private void refreshAdasStsHmi(int adasStsHmi) {
        if (adasStsHmi == 2) {//ADAS已激活：LCC居中辅助开关和避让大车开关置灰显示，不可点击设置
            llLccLaneKeeping.setAlpha(Constants.ERROR_ALPHA);
            ivLccLaneKeeping.setClickable(false);
            llAvoidBigCars.setAlpha(Constants.ERROR_ALPHA);
            ivAvoidBigCars.setClickable(false);
        } else {
            int lccLaneKeepingInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_ICASETFB);
            LogUtil.d("ADAS未激活 获取LCC车道居中辅助并刷新： " + lccLaneKeepingInt);
            refreshLccLanKeeping(lccLaneKeepingInt);
            //避让大车
            int avoidBigCarsInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_ICATRUCKSETFB);
            LogUtil.d("ADAS未激活 获取避让大车并刷新： " + avoidBigCarsInt);
            refreshAvoidBigCars(avoidBigCarsInt);
        }
    }

    private int getForwardCollisionWarningBrakingInt() {
        return CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_AEBFCW_SETFB);
    }

    private int getLDWWarningInt() {
        return CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_LDWALERTMETHODSETFB);
    }

    //获取车道偏离预警设置 1 ON 0 OFF
    private int getLaneDepartureWarningInt() {
        int laneDepartureWarningInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_LDWSETFB);
        LogUtil.d("获取车道偏离预警设置： " + laneDepartureWarningInt + (laneDepartureWarningInt == 1 ? "  打开" : "  关闭"));
        return laneDepartureWarningInt;
    }

    @Override
    public void onClick(View v) {
        int id = v.getId();
        if (id == R.id.tv_country) {//国家代码
            if (DeviceUtil.ifPowerOff() || FastClickCheckUtil.isFastClickNoToast(1000)) {
                return;
            }
            countrySelectDialog = new CountrySelectDialog(Objects.requireNonNull(getContext()), countryBeans, countryIdInt, 0.40, countryBean -> {
                setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_HU_NAV_COUNTRYID, countryBean.getCountryCode());
                LogUtil.d("选中国家设置： " + countryBean.getCountryName() + "国家代码： " + countryBean.getCountryCode());
            });
            countrySelectDialog.show();
        } else if (id == R.id.iv_cruise_assist_tips) {//续航辅助tips
            if (isCanShowPopup()) {
                if (cruiseAssistTipsDialog == null) {
                    cruiseAssistTipsDialog = new AdasDialogUtil(Objects.requireNonNull(getContext()), R.layout.dialog_lcc_lane_keeping, R.layout.dailog_avoid_big_cars);
                    cruiseAssistTipsDialog.setTab(getString(R.string.lcc_lane_keeping), getString(R.string.avoid_big_cars));
                }
                cruiseAssistTipsDialog.showTabLayout(getContext(), 1);
                cruiseAssistTipsDialog.showDialog();
            }
        } else if (id == R.id.iv_lane_departure_assist_tips) {//车道偏离预警及纠正tips
            if (isCanShowPopup()) {
                if (laneDepartureAssistTipsDialog == null) {
                    laneDepartureAssistTipsDialog = new AdasDialogUtil(Objects.requireNonNull(getContext()), R.layout.dialog_lane_departure_warning, R.layout.dialog_lane_correction_assistance);
                    laneDepartureAssistTipsDialog.setTab(getString(R.string.lane_departure_warning), getString(R.string.lane_correction_assistance));
                }
                laneDepartureAssistTipsDialog.showTabLayout(getContext(), 1);
                laneDepartureAssistTipsDialog.showDialog();
            }
        } else if (id == R.id.iv_emergency_lane_keeping_tips) {//紧急车道保持tips
            if (isCanShowPopup()) {
                showEmergencyLaneKeepingDialog();
            }
        } else if (id == R.id.iv_forward_warning_aid_tips) {//前向主动安全辅助tips
            if (isCanShowPopup()) {
                showForwardWarningAidDialog();
            }
        } else if (id == R.id.iv_lateral_warning_aid_tips) {//侧向盲区辅助tips
            if (isCanShowPopup()) {
                if (lateralWarningAidDialog == null) {
                    lateralWarningAidDialog = new AdasDialogUtil(Objects.requireNonNull(getContext()), R.layout.dialog_blind_spot_detection, R.layout.dialog_door_opening_warning);
                    lateralWarningAidDialog.setTab(getString(R.string.blind_spot_detection), getString(R.string.door_opening_warning));
                }
                lateralWarningAidDialog.showTabLayout(getContext(), 1);
                lateralWarningAidDialog.showDialog();
            }
        } else if (id == R.id.iv_backward_blind_aid_tips) {//后向盲区辅助tips
            if (isCanShowPopup()) {
                if (backwardBlindAidDialog == null) {
                    backwardBlindAidDialog = new AdasDialogUtil(Objects.requireNonNull(getContext()), R.layout.dialog_rear_collision_warning, R.layout.dialog_astern_traverse_warning);
                    backwardBlindAidDialog.setTab(getString(R.string.rear_collision_warning), getString(R.string.astern_traverse_warning));
                }
                backwardBlindAidDialog.showTabLayout(getContext(), 1);
                backwardBlindAidDialog.showDialog();
            }
        } else if (id == R.id.iv_intelligent_rate_limiting_aid_tips) {//智能限速辅助tips
            if (isCanShowPopup()) {
                if (intelligentRateLimitingAidDialog == null) {
                    intelligentRateLimitingAidDialog = new AdasDialogUtil(Objects.requireNonNull(getContext()), R.layout.dialog_speed_limit_identification_aid, R.layout.dialog_over_speed_warning);
                    intelligentRateLimitingAidDialog.setTab(getString(R.string.speed_limit_identification_aid), getString(R.string.over_speed_warning));
                }
                intelligentRateLimitingAidDialog.showTabLayout(getContext(), 1);
                intelligentRateLimitingAidDialog.showDialog();
            }
        } else {
            if (DeviceUtil.ifPowerOff() || FastClickCheckUtil.isFastClick(500)) {
                return;
            }
            if (id == R.id.tv_lane_close) {//车道偏离预警 下设值 2 ON打开 1 OFF关闭
                if (getLaneDepartureWarningInt() != 0) {//关闭状态下不下发关闭信号 获取值 1 ON 0 OFF
//                    setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_LDWSETFB, 1);
                    List<Integer> list = new ArrayList<>(Arrays.asList(1, 0));
                    setLDWAndAlert(list);
                    LogUtil.d("车道偏离预警设置：关闭");
                    LaneDepartureWarningClickInt = 0;
                    laneDepartureWarningCloseClick = true;
                    refreshLaneDepartureWarningUi(0);
                    setLaneDepartureWarningClickableFalse();
                    cancelLaneDepartureWarningTimeOut();
                    postLaneDepartureWarningTimeOutDelayed();
                }
            } else if (id == R.id.tv_warning_only) {//车道偏离预警 2 仅预警
                if (getLaneDepartureWarningInt() == 0 || getLDWWarningInt() != 2) {
//                    setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_LDWSETFB, 2);
                    List<Integer> list = new ArrayList<>(Arrays.asList(2, 2));
                    setLDWAndAlert(list);
                    LogUtil.d("车道偏离预警设置：打开");
                    LaneDepartureWarningClickInt = 2;
                    laneDepartureWarningClick = true;
                    laneDepartureWarningCloseClick = true;
                    refreshLaneDepartureWarningUi(2);
//                    setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_LDWALERTMETHODSETFB, 2);
                    LogUtil.d("LDW报警方式设置： 仅预警  下设值： 2");

                    setLaneDepartureWarningClickableFalse();
                    cancelLaneDepartureWarningTimeOut();
                    postLaneDepartureWarningTimeOutDelayed();
                }
            } else if (id == R.id.tv_vibration) {//车道偏离预警 1 仅振动
                if (getLaneDepartureWarningInt() == 0 || getLDWWarningInt() != 1) {
//                    setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_LDWSETFB, 2);
                    List<Integer> list = new ArrayList<>(Arrays.asList(2, 1));
                    setLDWAndAlert(list);
                    LogUtil.d("车道偏离预警设置：打开");
                    refreshLaneDepartureWarningUi(1);
                    LaneDepartureWarningClickInt = 1;
                    laneDepartureWarningClick = true;
                    laneDepartureWarningCloseClick = true;
//                    setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_LDWALERTMETHODSETFB, 1);
                    LogUtil.d("LDW报警方式设置： 仅振动  下设值： 1");
                    setLaneDepartureWarningClickableFalse();
                    cancelLaneDepartureWarningTimeOut();
                    postLaneDepartureWarningTimeOutDelayed();
                }
            } else if (id == R.id.tv_warning_and_vibration) {//车道偏离预警 3 预警并振动
                if (getLaneDepartureWarningInt() == 0 || getLDWWarningInt() != 3) {
//                    setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_LDWSETFB, 2);
                    List<Integer> list = new ArrayList<>(Arrays.asList(2, 3));
                    setLDWAndAlert(list);
                    LogUtil.d("车道偏离预警设置：打开");
                    LaneDepartureWarningClickInt = 3;
                    laneDepartureWarningClick = true;
                    laneDepartureWarningCloseClick = true;
                    refreshLaneDepartureWarningUi(3);
//                    setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_LDWALERTMETHODSETFB, 3);
                    LogUtil.d("LDW报警方式设置： 预警并振动  下设值： 3");
                    setLaneDepartureWarningClickableFalse();
                    cancelLaneDepartureWarningTimeOut();
                    postLaneDepartureWarningTimeOutDelayed();
                }
            } else if (id == R.id.collision_warning_braking_close) {//前向碰撞预警及紧急制动关闭     2 ON打开 1 OFF关闭
                if (getForwardCollisionWarningBrakingInt() != 0) {//0 关闭 1 仅预警 2仅制动 3 预警并制动
                    showCollisionWarningBrakingCloseDialog();
                }
            } else if (id == R.id.collision_warning) {//仅预警
                if (getForwardCollisionWarningBrakingInt() != 1) {
                    showCollisionWarningDialog();
                }
            } else if (id == R.id.braking) {//仅制动
                if (getForwardCollisionWarningBrakingInt() != 2) {
                    showCollisionBrakingDialog();
                }
            } else if (id == R.id.collision_warning_braking) {//预警并制动
                if (getForwardCollisionWarningBrakingInt() != 3) {
//                    setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_FCWSETFB, 2);//预警 开
//                    setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_AEBSETFB, 2);//制动 开
//                    //aeb => 0  fcw => 1
//                    int arrInt[] = {2, 2};
                    List<Integer> list = new ArrayList<>(Arrays.asList(2, 2));
                    setFCWAndAEB(list);
                    LogUtil.d("前向碰撞预警及紧急制动设置： 预警并制动");
                    //前向碰撞预警及紧急制动 0 关闭 1 仅预警 2仅制动 3 预警并制动
                    refreshForwardCollisionWarningBraking(3);
                    forwardCollisionWarningBrakingClickInt = 3;
                    forwardCollisionWarningBrakingClick = true;

                    setForwardCollisionWarningBrakingClickableFalse();
                    cancelForwardCollisionWarningBrakingTimeOut();
                    postForwardCollisionWarningBrakingTimeOutDelayed();
                }
            } else if (id == R.id.iv_lcc_lane_keeping) {//LCC车道保持 2 ON  1 OFF
                if (!isLaneChangeAssist) {
                    showLccLanKeepingDialog();
                } else {
                    setLccLanKeeping();
                }
            } else if (id == R.id.iv_avoid_big_cars) {//避让大车 2 ON  1 OFF
                setAvoidBigCars();
            } else if (id == R.id.iv_lane_correction_assistance) {//车道纠偏辅助  2 ON  1 OFF
                setLaneCorrectionAssistance();
            } else if (id == R.id.iv_emergency_lane_keeping) {//紧急车道保持 4 ON  1 OFF
                setEmergencyLaneKeeping();
            } else if (id == R.id.iv_blind_spot_detection) {//盲区监测  2 ON  1 OFF
                setBlindSpotDetection();
            } else if (id == R.id.iv_door_opening_warning) {//开门预警  2 ON  1 OFF
                setDoorOpeningWarning();
            } else if (id == R.id.iv_rear_collision_warning) {//后向碰撞预警 2 ON  1 OFF
                setRearCollisionWarning();
            } else if (id == R.id.iv_astern_traverse_warning) {//后方横向穿行预警 2 ON  1 OFF
                setAsternTraverseWarning();
            } else if (id == R.id.iv_speed_limit) {//限速标识识别  2 ON  1 OFF
                setSpeedLimit();
            } else if (id == R.id.iv_over_speed_warning) {//超速报警提示  2 ON  1 OFF
                setOverSpeedWarning();
            }
        }
    }

    //设置向前碰撞预警和紧急制动  List<Integer>  （index 0）AEBSETFB 紧急制动，（index 1） FCWSETFB 向前碰撞预警
    private void setFCWAndAEB(List<Integer> list) {
        CarVehicleManager.getInstance().requestSetFCWAndAEB(list);
    }

    //车道偏离预警信号设置 List<Integer>  （index 0）LDWSETFB 预警开关， （index 1） LDWALERTMETHODSETFB 预警方式 1 仅振动 2 仅预警 3 预警并振动
    private void setLDWAndAlert(List<Integer> list) {
        CarVehicleManager.getInstance().requestSetLDWAndAlert(list);
    }

    private boolean isCanShowPopup() {
        return (isAdded() && !isDetached() && getActivity() != null && !getActivity().isFinishing() && !getActivity().isDestroyed());
    }

    private void setLaneDepartureWarningClickableFalse() {
        for (int i = 0; i < mLaneDepartureWarningTvs.size(); i++) {
            TextView textView = mLaneDepartureWarningTvs.get(i);
            textView.setClickable(false);
        }
    }

    private void setForwardCollisionWarningBrakingClickableFalse() {
        for (int i = 0; i < mCollisionWarningBrakingTvs.size(); i++) {
            TextView textView = mCollisionWarningBrakingTvs.get(i);
            textView.setClickable(false);
        }
    }

    //超速报警提示   2 ON  1 OFF
    private void setOverSpeedWarning() {
        setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_SLWFSETFB, !isOverSpeedWarning ? 2 : 1);
        LogUtil.d("超速报警提示设置: " + !isOverSpeedWarning + "  值： " + (!isOverSpeedWarning ? 2 : 1));
        ivOverSpeedWarning.setImageResource(getSwitchRes(!isOverSpeedWarning));
        ivOverSpeedWarning.setClickable(false);
        overSpeedWarningClick = true;
        cancelOverSpeedWarningTimeOut();
        postOverSpeedWarningTimeOutDelayed();
    }

    //限速标识识别  2 ON  1 OFF
    private void setSpeedLimit() {
        setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_SLIFSETFB, !isSpeedLimit ? 2 : 1);
        LogUtil.d("限速标识识别设置: " + !isSpeedLimit + "  值： " + (!isSpeedLimit ? 2 : 1));
        ivSpeedLimit.setImageResource(getSwitchRes(!isSpeedLimit));
        ivSpeedLimit.setClickable(false);
        speedLimitClick = true;
        cancelSpeedLimitTimeOut();
        postSpeedLimitTimeOutDelayed();
    }

    //后方横向穿行预警 2 ON  1 OFF
    private void setAsternTraverseWarning() {
        setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_CRRR_RCTA_SETFB, !isAsternTraverseWarning ? 2 : 1);
        LogUtil.d("后方横向穿行预警设置: " + !isAsternTraverseWarning + "  值： " + (!isAsternTraverseWarning ? 2 : 1));
        ivAsternTraverseWarning.setImageResource(getSwitchRes(!isAsternTraverseWarning));
        ivAsternTraverseWarning.setClickable(false);
        asternTraverseWarningClick = true;
        cancelAsternTraverseWarningTimeOut();
        postAsternTraverseWarningTimeOutDelayed();
    }

    //后向碰撞预警 2 ON  1 OFF
    private void setRearCollisionWarning() {
        setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_CRRR_RCW_SETFB, !isRearCollisionWarning ? 2 : 1);
        LogUtil.d("后向碰撞预警设置: " + !isRearCollisionWarning + "  值： " + (!isRearCollisionWarning ? 2 : 1));
        ivRearCollisionWarning.setImageResource(getSwitchRes(!isRearCollisionWarning));
        ivRearCollisionWarning.setClickable(false);
        rearCollisionWarningClick = true;
        cancelRearCollisionWarningTimeOut();
        postRearCollisionWarningTimeOutDelayed();
    }

    //开门预警  2 ON  1 OFF
    private void setDoorOpeningWarning() {
        setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_CRRR_DOWSETFB, !isDoorOpeningWarning ? 2 : 1);
        LogUtil.d("开门预警设置: " + !isDoorOpeningWarning + "  值： " + (!isDoorOpeningWarning ? 2 : 1));
        ivDoorOpeningWarning.setImageResource(getSwitchRes(!isDoorOpeningWarning));
        ivDoorOpeningWarning.setClickable(false);
        doorOpeningWarningClick = true;
        cancelDoorOpeningWarningTimeOut();
        postDoorOpeningWarningTimeOutDelayed();
    }

    //盲区监测  2 ON  1 OFF
    private void setBlindSpotDetection() {
        setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_CRRR_BSD_SETFB, !isBlindSpotDetection ? 2 : 1);
        LogUtil.d("盲区监测设置: " + !isBlindSpotDetection + "  值： " + (!isBlindSpotDetection ? 2 : 1));
        ivBlindSpotDetection.setImageResource(getSwitchRes(!isBlindSpotDetection));
        ivBlindSpotDetection.setClickable(false);
        blindSpotDetectionClick = true;
        cancelBlindSpotDetectionTimeOut();
        postBlindSpotDetectionTimeOutDelayed();
    }

    //紧急车道保持 4 ON  1 OFF
    private void setEmergencyLaneKeeping() {
        setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_ELKSETFB, !isEmergencyLaneKeeping ? 4 : 1);
        LogUtil.d("紧急车道保持设置: " + !isEmergencyLaneKeeping + "  值： " + (!isEmergencyLaneKeeping ? 4 : 1));
        ivEmergencyLaneKeeping.setImageResource(getSwitchRes(!isEmergencyLaneKeeping));
        ivEmergencyLaneKeeping.setClickable(false);
        emergencyLaneKeepingClick = true;
        cancelEmergencyLaneKeepingTimeOut();
        postEmergencyLaneKeepingTimeOutDelayed();
    }

    //车道纠偏辅助  2 ON  1 OFF
    private void setLaneCorrectionAssistance() {
        setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_LKASETFB, !isLaneCorrectionAssistance ? 2 : 1);
        LogUtil.d("车道纠偏辅助设置: " + !isLaneCorrectionAssistance + "  值： " + (!isLaneCorrectionAssistance ? 2 : 1));
        ivLaneCorrectionAssistance.setImageResource(getSwitchRes(!isLaneCorrectionAssistance));
        ivLaneCorrectionAssistance.setClickable(false);
        laneCorrectionAssistanceClick = true;
        cancelLaneCorrectionAssistanceTimeOut();
        postLaneCorrectionAssistanceTimeOutDelayed();
    }

    //避让大车 2 ON  1 OFF
    private void setAvoidBigCars() {
        setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_ICATRUCKSETFB, !isAvoidBigCars ? 2 : 1);
        LogUtil.d("避让大车设置: " + !isAvoidBigCars + "  值： " + (!isAvoidBigCars ? 2 : 1));
        ivAvoidBigCars.setImageResource(getSwitchRes(!isAvoidBigCars));
        ivAvoidBigCars.setClickable(false);
        avoidBigCarsClick = true;
        cancelAvoidBigCarsTimeOut();
        postAvoidBigCarsTimeOutDelayed();
    }

    //LCC车道保持
    private void setLccLanKeeping() {
        setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_ICASETFB, !isLaneChangeAssist ? 2 : 1);
        LogUtil.d("LCC车道保持设置: " + !isLaneChangeAssist + "  值： " + (!isLaneChangeAssist ? 2 : 1));
        ivLccLaneKeeping.setImageResource(getSwitchRes(!isLaneChangeAssist));
        ivLccLaneKeeping.setClickable(false);
        laneChangeAssistClick = true;
        cancelLccLaneKeepingTimeOut();
        postLccLaneKeepingTimeOutDelayed();
    }

    //LCC车道保持dialog
    private void showLccLanKeepingDialog() {
        View view = LayoutInflater.from(getActivity()).inflate(R.layout.dialog_lcc_lane_keeping_confirm, (ViewGroup) null);
        if (lccLanKeepingConfirmDialog == null) {
            lccLanKeepingConfirmDialog = DialogUtil.getDialog(getActivity(), view, 0.40);
            lccLanKeepingConfirmDialog.setCanceledOnTouchOutside(false);
        }
        if (!lccLanKeepingConfirmDialog.isShowing()) {
            lccLanKeepingConfirmDialog.show();
        }

        view.findViewById(R.id.tv_confirm).setOnClickListener(v -> {
            LogUtil.d("LCC车道保持开启弹框，确定点击");
            setLccLanKeeping();
            if (lccLanKeepingConfirmDialog.isShowing()) {
                lccLanKeepingConfirmDialog.dismiss();
            }
        });

        view.findViewById(R.id.tv_cancel).setOnClickListener(v -> {
            if (lccLanKeepingConfirmDialog.isShowing()) {
                lccLanKeepingConfirmDialog.dismiss();
            }
            LogUtil.d("LCC车道保持弹框：点击取消");
        });
    }


    //紧急车道保持dialog
    private void showEmergencyLaneKeepingDialog() {
        View view = LayoutInflater.from(getActivity()).inflate(R.layout.dialog_emergency_lane_keeping, (ViewGroup) null);
        if (emergencyLaneKeepingDialog == null) {
            emergencyLaneKeepingDialog = DialogUtil.getDialog(getActivity(), view, 0.58);
            emergencyLaneKeepingDialog.setCanceledOnTouchOutside(false);
        }
        if (!emergencyLaneKeepingDialog.isShowing()) {
            emergencyLaneKeepingDialog.show();
        }
        view.findViewById(R.id.iv_close).setOnClickListener(v -> {
            if (emergencyLaneKeepingDialog.isShowing()) {
                emergencyLaneKeepingDialog.dismiss();
            }
        });
    }

    //前向主动安全辅助dialog
    private void showForwardWarningAidDialog() {
        View view = LayoutInflater.from(getActivity()).inflate(R.layout.dialog_forward_warning_aid, (ViewGroup) null);
        if (forwardWarningAidDialog == null) {
            forwardWarningAidDialog = DialogUtil.getDialog(getActivity(), view, 0.58);
            forwardWarningAidDialog.setCanceledOnTouchOutside(false);
        }
        if (!forwardWarningAidDialog.isShowing()) {
            forwardWarningAidDialog.show();
        }
        view.findViewById(R.id.iv_close).setOnClickListener(v -> {
            if (forwardWarningAidDialog.isShowing()) {
                forwardWarningAidDialog.dismiss();
            }
        });
    }

    //前向碰撞预警及紧急制动设置二次确认弹窗
    private void showCollisionWarningBrakingCloseDialog() {
        View view = LayoutInflater.from(getActivity()).inflate(R.layout.dialog_collision_warning_braking_close, (ViewGroup) null);
        if (collisionWarningBrakingCloseDialog == null) {
            collisionWarningBrakingCloseDialog = DialogUtil.getDialog(getActivity(), view, 0.36);
        }
        if (!collisionWarningBrakingCloseDialog.isShowing()) {
            collisionWarningBrakingCloseDialog.show();
        }

        view.findViewById(R.id.tv_confirm).setOnClickListener(v -> {
//            setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_FCWSETFB, 1);//预警 关
//            setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_AEBSETFB, 1);//制动 关
            List<Integer> list = new ArrayList<>(Arrays.asList(1, 1));
            setFCWAndAEB(list);
            //前向碰撞预警及紧急制动 0 关闭 1 仅预警 2仅制动 3 预警并制动
            refreshForwardCollisionWarningBraking(0);
            forwardCollisionWarningBrakingClickInt = 0;
            forwardCollisionWarningBrakingClick = true;
            LogUtil.d("前向碰撞预警及紧急制动设置： 关闭");
            setForwardCollisionWarningBrakingClickableFalse();
            cancelForwardCollisionWarningBrakingTimeOut();
            postForwardCollisionWarningBrakingTimeOutDelayed();
            if (collisionWarningBrakingCloseDialog.isShowing()) {
                collisionWarningBrakingCloseDialog.dismiss();
            }
        });

        view.findViewById(R.id.tv_cancel).setOnClickListener(v -> {
            if (collisionWarningBrakingCloseDialog.isShowing()) {
                collisionWarningBrakingCloseDialog.dismiss();
            }
            LogUtil.d("前向碰撞预警及紧急制动设置弹窗：点击取消");
        });
    }

    //前向碰撞仅预警二次确认弹窗
    private void showCollisionWarningDialog() {
        View view = LayoutInflater.from(getActivity()).inflate(R.layout.dialog_collision_warning, (ViewGroup) null);
        if (collisionWarningDialog == null) {
            collisionWarningDialog = DialogUtil.getDialog(getActivity(), view, 0.36);
        }
        if (!collisionWarningDialog.isShowing()) {
            collisionWarningDialog.show();
        }

        view.findViewById(R.id.tv_confirm).setOnClickListener(v -> {
//            setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_FCWSETFB, 2);//预警 开
//            setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_AEBSETFB, 1);//制动 关
            //aeb => 0  fcw => 1
            List<Integer> list = new ArrayList<>(Arrays.asList(1, 2));
            setFCWAndAEB(list);
            //前向碰撞预警及紧急制动 0 关闭 1 仅预警 2仅制动 3 预警并制动
            refreshForwardCollisionWarningBraking(1);
            forwardCollisionWarningBrakingClickInt = 1;
            forwardCollisionWarningBrakingClick = true;
            setForwardCollisionWarningBrakingClickableFalse();
            cancelForwardCollisionWarningBrakingTimeOut();
            postForwardCollisionWarningBrakingTimeOutDelayed();
            LogUtil.d("前向碰撞预警及紧急制动设置： 仅预警");
            if (collisionWarningDialog.isShowing()) {
                collisionWarningDialog.dismiss();
            }
        });

        view.findViewById(R.id.tv_cancel).setOnClickListener(v -> {
            if (collisionWarningDialog.isShowing()) {
                collisionWarningDialog.dismiss();
            }
            LogUtil.d("前向碰撞仅预警设置弹窗：点击取消");
        });
    }

    //前向碰撞仅制动二次确认弹窗
    private void showCollisionBrakingDialog() {
        View view = LayoutInflater.from(getActivity()).inflate(R.layout.dialog_collision_braking, (ViewGroup) null);
        if (collisionBrakingDialog == null) {
            collisionBrakingDialog = DialogUtil.getDialog(getActivity(), view, 0.36);
        }
        if (!collisionBrakingDialog.isShowing()) {
            collisionBrakingDialog.show();
        }

        view.findViewById(R.id.tv_confirm).setOnClickListener(v -> {
//            setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_FCWSETFB, 1);//预警 关
//            setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_AEBSETFB, 2);//制动 开
            //aeb => 0  fcw => 1
            List<Integer> list = new ArrayList<>(Arrays.asList(2, 1));
            setFCWAndAEB(list);
            LogUtil.d("前向碰撞预警及紧急制动设置： 仅制动");
            //前向碰撞预警及紧急制动 0 关闭 1 仅预警 2仅制动 3 预警并制动
            refreshForwardCollisionWarningBraking(2);
            forwardCollisionWarningBrakingClickInt = 2;
            forwardCollisionWarningBrakingClick = true;
            setForwardCollisionWarningBrakingClickableFalse();
            cancelForwardCollisionWarningBrakingTimeOut();
            postForwardCollisionWarningBrakingTimeOutDelayed();
            if (collisionBrakingDialog.isShowing()) {
                collisionBrakingDialog.dismiss();
            }
        });

        view.findViewById(R.id.tv_cancel).setOnClickListener(v -> {
            if (collisionBrakingDialog.isShowing()) {
                collisionBrakingDialog.dismiss();
            }
            LogUtil.d("前向碰撞仅制动设置弹窗：点击取消");
        });
    }

    private void setIntPropertyByHALVehicle(int key, int value) {
        CarVehicleManager.getInstance().setIntPropertyByHALVehicle(key, value);
    }

    //LCC车道居中辅助 1 ON 0 OFF
    private void refreshLccLanKeeping(int lccLaneKeepingInt) {
        if (Constants.ERROR_CODE == lccLaneKeepingInt) {
            llLccLaneKeeping.setAlpha(Constants.ERROR_ALPHA);
            ivLccLaneKeeping.setClickable(false);
        } else {
            if (!ivLccLaneKeeping.isClickable()) {
                ivLccLaneKeeping.setClickable(true);
            }
            if (llLccLaneKeeping.getAlpha() < 1.0f) {
                llLccLaneKeeping.setAlpha(1.0f);
            }
            isLaneChangeAssist = (lccLaneKeepingInt == 1);
            ivLccLaneKeeping.setImageResource(getSwitchRes(isLaneChangeAssist));
            if (!isLaneChangeAssist) {//LCC车道居中辅助关闭需要下设大车避让关闭，并置灰
                setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_ICATRUCKSETFB, 1);
                LogUtil.d("LCC车道居中辅助关闭需要下设大车避让关闭值： 1");
                llAvoidBigCars.setAlpha(Constants.ERROR_ALPHA);
                ivAvoidBigCars.setClickable(false);
            } else {//LCC车道居中辅助打开，大车避让不置灰，可点击
                if (!ivAvoidBigCars.isClickable()) {
                    ivAvoidBigCars.setClickable(true);
                }
                if (llAvoidBigCars.getAlpha() < 1.0f) {
                    llAvoidBigCars.setAlpha(1.0f);
                }
            }
        }
    }

    //避让大车  1 ON 0 OFF
    private void refreshAvoidBigCars(int avoidBigCarsInt) {
        if (Constants.ERROR_CODE == avoidBigCarsInt) {
            llAvoidBigCars.setAlpha(Constants.ERROR_ALPHA);
            ivAvoidBigCars.setClickable(false);
        } else {
            if (!isLaneChangeAssist) {//LCC车道居中辅助关闭,避让大车需要置灰不可点击
                llAvoidBigCars.setAlpha(Constants.ERROR_ALPHA);
                ivAvoidBigCars.setClickable(false);
            } else {
                if (!ivAvoidBigCars.isClickable()) {
                    ivAvoidBigCars.setClickable(true);
                }
                if (llAvoidBigCars.getAlpha() < 1.0f) {
                    llAvoidBigCars.setAlpha(1.0f);
                }
            }
            isAvoidBigCars = (avoidBigCarsInt == 1);
            ivAvoidBigCars.setImageResource(getSwitchRes(isAvoidBigCars));
        }
    }

    //前向碰撞预警及紧急制动  0 关闭 1 仅预警 2仅制动 3 预警并制动 4\5\6\7 Reserved
    private void refreshForwardCollisionWarningBraking(int value) {
        if (Constants.ERROR_CODE == value || value == 4 || value == 5 | value == 6 || value == 7) {//信号丢失
            llCollisionWarningBraking.setAlpha(Constants.ERROR_ALPHA);
            for (int i = 0; i < mCollisionWarningBrakingTvs.size(); i++) {
                TextView textView = mCollisionWarningBrakingTvs.get(i);
                textView.setClickable(false);
            }
            return;
        } else {
            if (llCollisionWarningBraking.getAlpha() < 1.0f) {
                llCollisionWarningBraking.setAlpha(1.0f);
            }
        }
        for (int i = 0; i < mCollisionWarningBrakingTvs.size(); i++) {
            TextView textView = mCollisionWarningBrakingTvs.get(i);
            if (!textView.isClickable()) {
                textView.setClickable(true);
            }
            if (value == i) {
                textView.setBackgroundResource(R.drawable.bg_segmented_btn);
                textView.setTextColor(getResources().getColor(R.color.white));
            } else {
                textView.setBackground(null);
                textView.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            }
        }
    }

    //车道纠偏辅助  1 ON 0 OFF
    private void refreshLaneCorrectionAssistance(int laneCorrectionAssistanceInt) {
        if (Constants.ERROR_CODE == laneCorrectionAssistanceInt) {
            llLaneCorrectionAssistance.setAlpha(Constants.ERROR_ALPHA);
            ivLaneCorrectionAssistance.setClickable(false);
        } else {
            if (!ivLaneCorrectionAssistance.isClickable()) {
                ivLaneCorrectionAssistance.setClickable(true);
            }
            if (llLaneCorrectionAssistance.getAlpha() < 1.0f) {
                llLaneCorrectionAssistance.setAlpha(1.0f);
            }
            isLaneCorrectionAssistance = (laneCorrectionAssistanceInt == 1);
            ivLaneCorrectionAssistance.setImageResource(getSwitchRes(isLaneCorrectionAssistance));
        }
    }

    //紧急车道保持   3 ON 0 OFF
    private void refreshEmergencyLaneKeeping(int emergencyLaneKeepingInt) {
        if (Constants.ERROR_CODE == emergencyLaneKeepingInt) {
            llEmergencyLaneKeeping.setAlpha(Constants.ERROR_ALPHA);
            ivEmergencyLaneKeeping.setClickable(false);
        } else {
            if (!ivEmergencyLaneKeeping.isClickable()) {
                ivEmergencyLaneKeeping.setClickable(true);
            }
            if (llEmergencyLaneKeeping.getAlpha() < 1.0f) {
                llEmergencyLaneKeeping.setAlpha(1.0f);
            }
            isEmergencyLaneKeeping = (emergencyLaneKeepingInt == 3);
            ivEmergencyLaneKeeping.setImageResource(getSwitchRes(isEmergencyLaneKeeping));
        }
    }

    //盲区监测   1 ON 0 OFF
    private void refreshBlindSpotDetection(int blindSpotDetectionAneWarningInt) {
        if (Constants.ERROR_CODE == blindSpotDetectionAneWarningInt) {
            llBlindSpotDetection.setAlpha(Constants.ERROR_ALPHA);
            ivBlindSpotDetection.setClickable(false);
        } else {
            if (!ivBlindSpotDetection.isClickable()) {
                ivBlindSpotDetection.setClickable(true);
            }
            if (llBlindSpotDetection.getAlpha() < 1.0f) {
                llBlindSpotDetection.setAlpha(1.0f);
            }
            isBlindSpotDetection = (blindSpotDetectionAneWarningInt == 1);
            ivBlindSpotDetection.setImageResource(getSwitchRes(isBlindSpotDetection));
        }
    }

    //开门预警   1 ON 0 OFF
    private void refreshDoorOpeningWarning(int doorOpeningWarningInt) {
        if (Constants.ERROR_CODE == doorOpeningWarningInt) {
            llDoorOpeningWarning.setAlpha(Constants.ERROR_ALPHA);
            ivDoorOpeningWarning.setClickable(false);
        } else {
            if (!ivDoorOpeningWarning.isClickable()) {
                ivDoorOpeningWarning.setClickable(true);
            }
            if (llDoorOpeningWarning.getAlpha() < 1.0f) {
                llDoorOpeningWarning.setAlpha(1.0f);
            }
            isDoorOpeningWarning = (doorOpeningWarningInt == 1);
            ivDoorOpeningWarning.setImageResource(getSwitchRes(isDoorOpeningWarning));
        }
    }

    //后向碰撞预警   1 ON 0 OFF
    private void refreshRearCollisionWarning(int rearCollisionWarningInt) {
        if (Constants.ERROR_CODE == rearCollisionWarningInt) {
            llRearCollisionWarning.setAlpha(Constants.ERROR_ALPHA);
            ivRearCollisionWarning.setClickable(false);
        } else {
            if (!ivRearCollisionWarning.isClickable()) {
                ivRearCollisionWarning.setClickable(true);
            }
            if (llRearCollisionWarning.getAlpha() < 1.0f) {
                llRearCollisionWarning.setAlpha(1.0f);
            }
            isRearCollisionWarning = (rearCollisionWarningInt == 1);
            ivRearCollisionWarning.setImageResource(getSwitchRes(isRearCollisionWarning));
        }
    }

    //后方横向穿行预警    1 ON 0 OFF
    private void refreshAsternTraverseWarning(int asternTraverseWarningInt) {
        if (Constants.ERROR_CODE == asternTraverseWarningInt) {
            llAsternTraverseWarning.setAlpha(Constants.ERROR_ALPHA);
            ivAsternTraverseWarning.setClickable(false);
        } else {
            if (!ivAsternTraverseWarning.isClickable()) {
                ivAsternTraverseWarning.setClickable(true);
            }
            if (llAsternTraverseWarning.getAlpha() < 1.0f) {
                llAsternTraverseWarning.setAlpha(1.0f);
            }
            isAsternTraverseWarning = (asternTraverseWarningInt == 1);
            ivAsternTraverseWarning.setImageResource(getSwitchRes(isAsternTraverseWarning));
        }
    }

    //限速标识识别    2 ON 1 OFF 0 Reserved 3 Reserved
    private void refreshSpeedLimit(int speedLimitInt) {
        if (Constants.ERROR_CODE == speedLimitInt || speedLimitInt == 0 || speedLimitInt == 3) {
            llSpeedLimit.setAlpha(Constants.ERROR_ALPHA);
            ivSpeedLimit.setClickable(false);
            tvCountry.setAlpha(Constants.ERROR_ALPHA);
            tvCountry.setClickable(false);
        } else {
            if (!ivSpeedLimit.isClickable()) {
                ivSpeedLimit.setClickable(true);
            }
            if (llSpeedLimit.getAlpha() < 1.0f) {
                llSpeedLimit.setAlpha(1.0f);
            }
            isSpeedLimit = (speedLimitInt == 2);
            //,限速识别开关打开时才高亮和可以点击设置
            if (isSpeedLimit) {
                tvCountry.setAlpha(1.0f);
                tvCountry.setClickable(true);
                llOverSpeedWarning.setAlpha(1.0f);
                ivOverSpeedWarning.setClickable(true);
            } else {//限速识别开关关闭时，应该下设超速报警提示关闭，并置灰显示，同时置灰“国家”且能设置“国家”，
                tvCountry.setAlpha(Constants.ERROR_ALPHA);
                tvCountry.setClickable(false);
                llOverSpeedWarning.setAlpha(Constants.ERROR_ALPHA);
                ivOverSpeedWarning.setClickable(false);
                setIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_SLWFSETFB, 1);
                LogUtil.d("限速识别开关关闭时，超速报警提示设置: 关闭 值： 1");
            }
            ivSpeedLimit.setImageResource(getSwitchRes(isSpeedLimit));
        }
    }

    //超速报警提示    2 ON 1 OFF 0 Reserved 3 Reserved
    private void refreshOverSpeedWarning(int overSpeedWarningInt) {
        if (Constants.ERROR_CODE == overSpeedWarningInt || overSpeedWarningInt == 0 || overSpeedWarningInt == 3) {
            llOverSpeedWarning.setAlpha(Constants.ERROR_ALPHA);
            ivOverSpeedWarning.setClickable(false);
        } else {
            if (!isSpeedLimit) {//限速识别开关关闭时
                llOverSpeedWarning.setAlpha(Constants.ERROR_ALPHA);
                ivOverSpeedWarning.setClickable(false);
            } else {
                if (!ivOverSpeedWarning.isClickable()) {
                    ivOverSpeedWarning.setClickable(true);
                }
                if (llOverSpeedWarning.getAlpha() < 1.0f) {
                    llOverSpeedWarning.setAlpha(1.0f);
                }
            }
            isOverSpeedWarning = (overSpeedWarningInt == 2);
            ivOverSpeedWarning.setImageResource(getSwitchRes(isOverSpeedWarning));
        }
    }

    //车道偏离预警
    private void refreshLaneDepartureWarningUi(int value) {
        if (Constants.ERROR_CODE == value) {//信号丢失
            llLaneDepartureWarning.setAlpha(Constants.ERROR_ALPHA);
            for (int i = 0; i < mLaneDepartureWarningTvs.size(); i++) {
                TextView textView = mLaneDepartureWarningTvs.get(i);
                textView.setClickable(false);
            }
            return;
        } else {
            if (llLaneDepartureWarning.getAlpha() < 1.0f) {
                llLaneDepartureWarning.setAlpha(1.0f);
            }
        }
        for (int i = 0; i < mLaneDepartureWarningTvs.size(); i++) {
            TextView textView = mLaneDepartureWarningTvs.get(i);
            if (!textView.isClickable()) {
                textView.setClickable(true);
            }
            if (value == i) {
                textView.setBackgroundResource(R.drawable.bg_segmented_btn);
                textView.setTextColor(getResources().getColor(R.color.white));
            } else {
                textView.setBackground(null);
                textView.setTextColor(getResources().getColor(R.color.tab_unselect_text_color));
            }
        }
    }

    private void refreshCountryIdUi(int countryCodeInt) {
        for (int i = 0; i < countryBeans.size(); i++) {
            int countryCode = countryBeans.get(i).getCountryCode();
            if (countryCode == countryCodeInt) {
                tvCountry.setText(countryBeans.get(i).getCountryName());
            }
        }
    }

    private int getSwitchRes(boolean res) {
        return res ? R.drawable.ic_switch_on : R.drawable.ic_switch_off;
    }

    private void showSetupFailureToast() {
        ToastMaster.shortToast(IntelligentDrivingFragment.this, getString(R.string.setup_failure));
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onModeChange(VehicleMessageEventBean eventBean) {
        int code = eventBean.getCode();
        if (code == EventBusActionCode.FCM_ICAENA_SWT) {//LCC车道保持
            int cmd = (int) eventBean.getCmd();
            if (laneChangeAssistClick && cmd != Constants.ERROR_CODE && (cmd == 1) == isLaneChangeAssist) {
                showSetupFailureToast();
                LogUtil.d("LCC车道保持下设值和返回值不对应下设值: " + !isLaneChangeAssist + "  反馈值：" + (cmd == 1));
            }
            laneChangeAssistClick = false;
            cancelLccLaneKeepingTimeOut();
            refreshLccLanKeeping(cmd);
        } else if (code == EventBusActionCode.FCM_ICAAVD_TRUCK_SET) {//避让大车
            int cmd = (int) eventBean.getCmd();
            if (avoidBigCarsClick && cmd != Constants.ERROR_CODE && (cmd == 1) == isAvoidBigCars) {
                showSetupFailureToast();
                LogUtil.d("避让大车下设值和返回值不对应下设值: " + !isAvoidBigCars + "  反馈值：" + (cmd == 1));
            }
            avoidBigCarsClick = false;
            cancelAvoidBigCarsTimeOut();
            refreshAvoidBigCars(cmd);
        } else if (code == EventBusActionCode.FCM_ACT_DRV_ASST_STS_HMI) {//ADAS激活  2已激活 其他关闭
            int cmd = (int) eventBean.getCmd();
            refreshAdasStsHmi(cmd);
        } else if (code == EventBusActionCode.FCM_LKA_ENABLE_SW) {//车道纠偏辅助
            int cmd = (int) eventBean.getCmd();
            if (laneCorrectionAssistanceClick && cmd != Constants.ERROR_CODE && (cmd == 1) == isLaneCorrectionAssistance) {
                showSetupFailureToast();
                LogUtil.d("车道纠偏辅助下设值和返回值不对应下设值: " + !isLaneCorrectionAssistance + "  反馈值：" + (cmd == 1));
            }
            laneCorrectionAssistanceClick = false;
            cancelLaneCorrectionAssistanceTimeOut();
            refreshLaneCorrectionAssistance(cmd);
        } else if (code == EventBusActionCode.FCM_LDW_ALERTMETHOD_SW) {//车道偏离预警 1 ON 0 OFF
            int cmd = (int) eventBean.getCmd();
            cancelLaneDepartureWarningTimeOut();
            if (cmd == 0 || cmd == Constants.ERROR_CODE) {//预警OFF时只更新显示关闭高亮
                if (laneDepartureWarningCloseClick && cmd == 0 && (cmd != LaneDepartureWarningClickInt)) {
                    showSetupFailureToast();
                    LogUtil.d("车道偏离预警下设值和返回值不对应下设值: " + 1 + "关闭   反馈值：" + cmd);
                }

                laneDepartureWarningCloseClick = false;
                refreshLaneDepartureWarningUi(cmd);
            } else if (cmd == 1) {//预警ON打开的情况下 100ms后去获取报警方式更新UI(两个信号回调可能有时间差)
                postDelayed(() -> {
                    int LDWWarningInt = getLDWWarningInt();
                    if (LDWWarningInt == Constants.ERROR_CODE) {
                        refreshLaneDepartureWarningUi(LDWWarningInt);
                        LogUtil.d("车道偏离预警ON:  延时500ms获取报警方式更新UI： LDW信号丢失");
                        laneDepartureWarningCloseClick = false;
                    } else {
                        if (LDWWarningInt > 0) {
                            if (laneDepartureWarningCloseClick && LDWWarningInt != LaneDepartureWarningClickInt) {
                                showSetupFailureToast();
                                LogUtil.d("车道偏离预警下设值和返回值不对应下设值: " + LaneDepartureWarningClickInt + "  反馈值：" + LDWWarningInt);
                            }
                            refreshLaneDepartureWarningUi(LDWWarningInt);
                            LogUtil.d("车道偏离预警ON:  延时500ms获取报警方式更新UI： " + LDWWarningInt + " ------1仅振动 2 仅预警 3 预警并振动");
                            laneDepartureWarningCloseClick = false;
                        }
                    }
                }, 100);
            }
        } else if (code == EventBusActionCode.FCM_LDW_ALERTMETHOD_SETFB) {//LDW报警方式车道偏离预警报警方式
            int cmd = (int) eventBean.getCmd();
            if (laneDepartureWarningClick && cmd != Constants.ERROR_CODE && cmd != LaneDepartureWarningClickInt) {
                showSetupFailureToast();
                LogUtil.d("LDW报警方式下设值和返回值不对应下设值: " + LaneDepartureWarningClickInt + "  反馈值：" + cmd);
            }

            laneDepartureWarningClick = false;
            cancelLaneDepartureWarningTimeOut();
            refreshLaneDepartureWarningUi(cmd);
        } else if (code == EventBusActionCode.FCM_ELKENB_SW) {//紧急车道保持
            int cmd = (int) eventBean.getCmd();
            if (emergencyLaneKeepingClick && cmd != Constants.ERROR_CODE && (cmd == 3) == isEmergencyLaneKeeping) {
                showSetupFailureToast();
                LogUtil.d("紧急车道保持下设值和返回值不对应下设值: " + !isEmergencyLaneKeeping + "  反馈值：" + (cmd == 3));
            }
            emergencyLaneKeepingClick = false;
            cancelEmergencyLaneKeepingTimeOut();
            refreshEmergencyLaneKeeping(cmd);
        } else if (code == EventBusActionCode.FCM_FCW_ENABLE_SW) {//前向碰撞预警及紧急制动  0 关闭 1 仅预警 2仅制动 3 预警并制动
            debounce.onEvent(() -> {//两个信号onChange的方式,回调有一次或者两次回调  在100ms内多次回调算最后一次的值更新UI
                int cmd = (int) eventBean.getCmd();
                if (forwardCollisionWarningBrakingClick && cmd != Constants.ERROR_CODE && (cmd != forwardCollisionWarningBrakingClickInt)) {
                    showSetupFailureToast();
                    LogUtil.d("前向碰撞预警及紧急制动下设值和返回值不对应下设值: " + forwardCollisionWarningBrakingClickInt + "  反馈值：" + cmd);
                }
                forwardCollisionWarningBrakingClick = false;
                LogUtil.d("响应前向碰撞预警及紧急制动最终值：" + cmd);
                cancelForwardCollisionWarningBrakingTimeOut();
                refreshForwardCollisionWarningBraking(cmd);
            });
        } else if (code == EventBusActionCode.CRRR_BSD_ENABLE_SW) {//盲区监测
            int cmd = (int) eventBean.getCmd();
            if (blindSpotDetectionClick && cmd != Constants.ERROR_CODE && (cmd == 1) == isBlindSpotDetection) {
                showSetupFailureToast();
                LogUtil.d("盲区监测下设值和返回值不对应下设值: " + !isBlindSpotDetection + "  反馈值：" + (cmd == 1));
            }
            blindSpotDetectionClick = false;
            cancelBlindSpotDetectionTimeOut();
            refreshBlindSpotDetection(cmd);
        } else if (code == EventBusActionCode.CRRR_DOWENA_SWT) {//开门预警
            int cmd = (int) eventBean.getCmd();
            if (doorOpeningWarningClick && cmd != Constants.ERROR_CODE && (cmd == 1) == isDoorOpeningWarning) {
                showSetupFailureToast();
                LogUtil.d("开门预警下设值和返回值不对应下设值: " + !isDoorOpeningWarning + "  反馈值：" + (cmd == 1));
            }
            doorOpeningWarningClick = false;
            cancelDoorOpeningWarningTimeOut();
            refreshDoorOpeningWarning(cmd);
        } else if (code == EventBusActionCode.CRRR_RCW_ENABLE_SW) { //后向碰撞预警
            int cmd = (int) eventBean.getCmd();
            if (rearCollisionWarningClick && cmd != Constants.ERROR_CODE && (cmd == 1) == isRearCollisionWarning) {
                showSetupFailureToast();
                LogUtil.d("开门预警下设值和返回值不对应下设值: " + !isRearCollisionWarning + "  反馈值：" + (cmd == 1));
            }
            rearCollisionWarningClick = false;
            cancelRearCollisionWarningTimeOut();
            refreshRearCollisionWarning(cmd);
        } else if (code == EventBusActionCode.CRRR_RCTA_ENABLE_SW) {//后方横向穿行预警
            int cmd = (int) eventBean.getCmd();
            if (asternTraverseWarningClick && cmd != Constants.ERROR_CODE && (cmd == 1) == isAsternTraverseWarning) {
                showSetupFailureToast();
                LogUtil.d("后方横向穿行预警下设值和返回值不对应下设值: " + !isAsternTraverseWarning + "  反馈值：" + (cmd == 1));
            }
            asternTraverseWarningClick = false;
            cancelAsternTraverseWarningTimeOut();
            refreshAsternTraverseWarning(cmd);
        } else if (code == EventBusActionCode.FCM_SLIF_ENABLE_SW) {//限速标识识别
            int cmd = (int) eventBean.getCmd();
            if (speedLimitClick && cmd != Constants.ERROR_CODE && (cmd == 2) == isSpeedLimit) {
                showSetupFailureToast();
                LogUtil.d("限速标识识别下设值和返回值不对应下设值: " + !isSpeedLimit + "  反馈值：" + (cmd == 2));
            }
            speedLimitClick = false;
            cancelSpeedLimitTimeOut();
            refreshSpeedLimit(cmd);
        } else if (code == EventBusActionCode.FCM_SLWF_ENABLE_SW) { //超速报警提示
            int cmd = (int) eventBean.getCmd();
            if (overSpeedWarningClick && cmd != Constants.ERROR_CODE && (cmd == 2) == isOverSpeedWarning) {
                showSetupFailureToast();
                LogUtil.d("超速报警提示下设值和返回值不对应下设值: " + !isOverSpeedWarning + "  反馈值：" + (cmd == 2));
            }
            overSpeedWarningClick = false;
            cancelOverSpeedWarningTimeOut();
            refreshOverSpeedWarning(cmd);
        } else if (code == EventBusActionCode.HU_NAVI_COUNTRY_ID) {//国家代码
            int cmd = (int) eventBean.getCmd();
            refreshCountryIdUi(cmd);
            countryIdInt = cmd;
            SPUtil.putInt(Objects.requireNonNull(getContext()), SPUtil.KEY_COUNTRY_ID, cmd);
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        EventBus.getDefault().unregister(this);
    }

    private Timer lccLaneKeepingTimeOutTimer;
    private TimerTask lccLaneKeepingTimeOutTask;

    private void postLccLaneKeepingTimeOutDelayed() {
        lccLaneKeepingTimeOutTimer = new Timer();
        lccLaneKeepingTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivLccLaneKeeping.post(() -> {
                    showSetupFailureToast();
                    ivLccLaneKeeping.setClickable(true);
                    laneChangeAssistClick = false;
                    int lccLaneKeepingInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_ICASETFB);
                    refreshLccLanKeeping(lccLaneKeepingInt);
                    LogUtil.d("设置失败，设置LCC车道居中辅助超时： " + lccLaneKeepingInt + "  isLaneChangeAssist:" + isLaneChangeAssist);
                });
            }
        };
        lccLaneKeepingTimeOutTimer.schedule(lccLaneKeepingTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelLccLaneKeepingTimeOut() {
        if (lccLaneKeepingTimeOutTimer != null) {
            lccLaneKeepingTimeOutTimer.cancel();
            lccLaneKeepingTimeOutTimer = null;
        }
        if (lccLaneKeepingTimeOutTask != null) {
            lccLaneKeepingTimeOutTask.cancel();
            lccLaneKeepingTimeOutTask = null;
        }
    }

    private Timer avoidBigCarsTimeOutTimer;
    private TimerTask avoidBigCarsTimeOutTask;

    private void postAvoidBigCarsTimeOutDelayed() {
        avoidBigCarsTimeOutTimer = new Timer();
        avoidBigCarsTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivAvoidBigCars.post(() -> {
                    showSetupFailureToast();
                    ivAvoidBigCars.setClickable(true);
                    avoidBigCarsClick = false;
                    int avoidBigCarsInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_ICATRUCKSETFB);
                    refreshAvoidBigCars(avoidBigCarsInt);
                    LogUtil.d("设置失败，设置避让大车超时： " + avoidBigCarsInt + "  isAvoidBigCars:" + isAvoidBigCars);
                });
            }
        };
        avoidBigCarsTimeOutTimer.schedule(avoidBigCarsTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelAvoidBigCarsTimeOut() {
        if (avoidBigCarsTimeOutTimer != null) {
            avoidBigCarsTimeOutTimer.cancel();
            avoidBigCarsTimeOutTimer = null;
        }
        if (avoidBigCarsTimeOutTask != null) {
            avoidBigCarsTimeOutTask.cancel();
            avoidBigCarsTimeOutTask = null;
        }
    }

    private Timer laneCorrectionAssistanceTimeOutTimer;
    private TimerTask laneCorrectionAssistanceTimeOutTask;

    private void postLaneCorrectionAssistanceTimeOutDelayed() {
        laneCorrectionAssistanceTimeOutTimer = new Timer();
        laneCorrectionAssistanceTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivLaneCorrectionAssistance.post(() -> {
                    showSetupFailureToast();
                    ivLaneCorrectionAssistance.setClickable(true);
                    laneCorrectionAssistanceClick = false;
                    int laneCorrectionAssistanceInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_LKASETFB);
                    refreshLaneCorrectionAssistance(laneCorrectionAssistanceInt);
                    LogUtil.d("设置失败，设置车道纠偏辅助超时： " + laneCorrectionAssistanceInt + "  isLaneCorrectionAssistance:" + isLaneCorrectionAssistance);
                });

            }
        };
        laneCorrectionAssistanceTimeOutTimer.schedule(laneCorrectionAssistanceTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelLaneCorrectionAssistanceTimeOut() {
        if (laneCorrectionAssistanceTimeOutTimer != null) {
            laneCorrectionAssistanceTimeOutTimer.cancel();
            laneCorrectionAssistanceTimeOutTimer = null;
        }
        if (laneCorrectionAssistanceTimeOutTask != null) {
            laneCorrectionAssistanceTimeOutTask.cancel();
            laneCorrectionAssistanceTimeOutTask = null;
        }
    }

    private Timer emergencyLaneKeepingTimeOutTimer;
    private TimerTask emergencyLaneKeepingTimeOutTask;

    private void postEmergencyLaneKeepingTimeOutDelayed() {
        emergencyLaneKeepingTimeOutTimer = new Timer();
        emergencyLaneKeepingTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivEmergencyLaneKeeping.post(() -> {
                    showSetupFailureToast();
                    ivEmergencyLaneKeeping.setClickable(true);
                    emergencyLaneKeepingClick = false;
                    int emergencyLaneKeepingInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_ELKSETFB);
                    refreshEmergencyLaneKeeping(emergencyLaneKeepingInt);
                    LogUtil.d("设置失败，设置紧急车道保持超时： " + emergencyLaneKeepingInt + "  isEmergencyLaneKeeping:" + isEmergencyLaneKeeping);
                });
            }
        };
        emergencyLaneKeepingTimeOutTimer.schedule(emergencyLaneKeepingTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelEmergencyLaneKeepingTimeOut() {
        if (emergencyLaneKeepingTimeOutTimer != null) {
            emergencyLaneKeepingTimeOutTimer.cancel();
            emergencyLaneKeepingTimeOutTimer = null;
        }
        if (emergencyLaneKeepingTimeOutTask != null) {
            emergencyLaneKeepingTimeOutTask.cancel();
            emergencyLaneKeepingTimeOutTask = null;
        }
    }

    private Timer blindSpotDetectionTimeOutTimer;
    private TimerTask blindSpotDetectionTimeOutTask;

    private void postBlindSpotDetectionTimeOutDelayed() {
        blindSpotDetectionTimeOutTimer = new Timer();
        blindSpotDetectionTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivBlindSpotDetection.post(() -> {
                    showSetupFailureToast();
                    ivBlindSpotDetection.setClickable(true);
                    blindSpotDetectionClick = false;
                    //盲区监测
                    int blindSpotDetectionInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_CRRR_BSD_SETFB);
                    refreshBlindSpotDetection(blindSpotDetectionInt);
                    LogUtil.d("设置失败，设置盲区监测超时： " + blindSpotDetectionInt + "  isBlindSpotDetection:" + isBlindSpotDetection);
                });
            }
        };
        blindSpotDetectionTimeOutTimer.schedule(blindSpotDetectionTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelBlindSpotDetectionTimeOut() {
        if (blindSpotDetectionTimeOutTimer != null) {
            blindSpotDetectionTimeOutTimer.cancel();
            blindSpotDetectionTimeOutTimer = null;
        }
        if (blindSpotDetectionTimeOutTask != null) {
            blindSpotDetectionTimeOutTask.cancel();
            blindSpotDetectionTimeOutTask = null;
        }
    }

    private Timer doorOpeningWarningTimeOutTimer;
    private TimerTask doorOpeningWarningTimeOutTask;

    private void postDoorOpeningWarningTimeOutDelayed() {
        doorOpeningWarningTimeOutTimer = new Timer();
        doorOpeningWarningTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivDoorOpeningWarning.post(() -> {
                    showSetupFailureToast();
                    ivDoorOpeningWarning.setClickable(true);
                    doorOpeningWarningClick = false;
                    int doorOpeningWarningInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_CRRR_DOWSETFB);
                    refreshDoorOpeningWarning(doorOpeningWarningInt);
                    LogUtil.d("设置失败，设置开门预警超时： " + doorOpeningWarningInt + "  isDoorOpeningWarning:" + isDoorOpeningWarning);
                });
            }
        };
        doorOpeningWarningTimeOutTimer.schedule(doorOpeningWarningTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelDoorOpeningWarningTimeOut() {
        if (doorOpeningWarningTimeOutTimer != null) {
            doorOpeningWarningTimeOutTimer.cancel();
            doorOpeningWarningTimeOutTimer = null;
        }
        if (doorOpeningWarningTimeOutTask != null) {
            doorOpeningWarningTimeOutTask.cancel();
            doorOpeningWarningTimeOutTask = null;
        }
    }

    private Timer rearCollisionWarningTimeOutTimer;
    private TimerTask rearCollisionWarningTimeOutTask;

    private void postRearCollisionWarningTimeOutDelayed() {
        rearCollisionWarningTimeOutTimer = new Timer();
        rearCollisionWarningTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivRearCollisionWarning.post(() -> {
                    showSetupFailureToast();
                    ivRearCollisionWarning.setClickable(true);
                    rearCollisionWarningClick = false;
                    int rearCollisionWarningInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_CRRR_RCW_SETFB);
                    refreshRearCollisionWarning(rearCollisionWarningInt);
                    LogUtil.d("设置失败，设置后向碰撞预警超时： " + rearCollisionWarningInt + "  isRearCollisionWarning:" + isRearCollisionWarning);
                });
            }
        };
        rearCollisionWarningTimeOutTimer.schedule(rearCollisionWarningTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelRearCollisionWarningTimeOut() {
        if (rearCollisionWarningTimeOutTimer != null) {
            rearCollisionWarningTimeOutTimer.cancel();
            rearCollisionWarningTimeOutTimer = null;
        }
        if (rearCollisionWarningTimeOutTask != null) {
            rearCollisionWarningTimeOutTask.cancel();
            rearCollisionWarningTimeOutTask = null;
        }
    }

    private Timer asternTraverseWarningTimeOutTimer;
    private TimerTask asternTraverseWarningTimeOutTask;

    private void postAsternTraverseWarningTimeOutDelayed() {
        asternTraverseWarningTimeOutTimer = new Timer();
        asternTraverseWarningTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivAsternTraverseWarning.post(() -> {
                    showSetupFailureToast();
                    ivAsternTraverseWarning.setClickable(true);
                    asternTraverseWarningClick = false;
                    int asternTraverseWarningInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_CRRR_RCTA_SETFB);
                    refreshAsternTraverseWarning(asternTraverseWarningInt);
                    LogUtil.d("设置失败，设置后方横向穿行预警超时： " + asternTraverseWarningInt + "  isAsternTraverseWarning:" + isAsternTraverseWarning);
                });
            }
        };
        asternTraverseWarningTimeOutTimer.schedule(asternTraverseWarningTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelAsternTraverseWarningTimeOut() {
        if (asternTraverseWarningTimeOutTimer != null) {
            asternTraverseWarningTimeOutTimer.cancel();
            asternTraverseWarningTimeOutTimer = null;
        }
        if (asternTraverseWarningTimeOutTask != null) {
            asternTraverseWarningTimeOutTask.cancel();
            asternTraverseWarningTimeOutTask = null;
        }
    }

    private Timer speedLimitTimeOutTimer;
    private TimerTask speedLimitTimeOutTask;

    private void postSpeedLimitTimeOutDelayed() {
        speedLimitTimeOutTimer = new Timer();
        speedLimitTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivSpeedLimit.post(() -> {
                    showSetupFailureToast();
                    ivSpeedLimit.setClickable(true);
                    speedLimitClick = false;
                    int speedLimitInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_SLIFSETFB);
                    refreshSpeedLimit(speedLimitInt);
                    LogUtil.d("设置失败，设置限速标识识别超时： " + speedLimitInt + "  isSpeedLimit:" + isSpeedLimit);
                });
            }
        };
        speedLimitTimeOutTimer.schedule(speedLimitTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelSpeedLimitTimeOut() {
        if (speedLimitTimeOutTimer != null) {
            speedLimitTimeOutTimer.cancel();
            speedLimitTimeOutTimer = null;
        }
        if (speedLimitTimeOutTask != null) {
            speedLimitTimeOutTask.cancel();
            speedLimitTimeOutTask = null;
        }
    }

    private Timer overSpeedWarningTimeOutTimer;
    private TimerTask overSpeedWarningTimeOutTask;

    private void postOverSpeedWarningTimeOutDelayed() {
        overSpeedWarningTimeOutTimer = new Timer();
        overSpeedWarningTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                ivOverSpeedWarning.post(() -> {
                    showSetupFailureToast();
                    ivOverSpeedWarning.setClickable(true);
                    overSpeedWarningClick = false;
                    int overSpeedWarningInt = CarVehicleManager.getInstance().getIntPropertyByHALVehicle(VehiclePropertyIds.GECKO_FCM_SLWFSETFB);
                    refreshOverSpeedWarning(overSpeedWarningInt);
                    LogUtil.d("设置失败，设置超速报警提示超时： " + overSpeedWarningInt + "  isOverSpeedWarning:" + isOverSpeedWarning);
                });
            }
        };
        overSpeedWarningTimeOutTimer.schedule(overSpeedWarningTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelOverSpeedWarningTimeOut() {
        if (overSpeedWarningTimeOutTimer != null) {
            overSpeedWarningTimeOutTimer.cancel();
            overSpeedWarningTimeOutTimer = null;
        }
        if (overSpeedWarningTimeOutTask != null) {
            overSpeedWarningTimeOutTask.cancel();
            overSpeedWarningTimeOutTask = null;
        }
    }

    private Timer laneDepartureWarningTimeOutTimer;
    private TimerTask laneDepartureWarningTimeOutTask;

    private void postLaneDepartureWarningTimeOutDelayed() {
        laneDepartureWarningTimeOutTimer = new Timer();
        laneDepartureWarningTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                llLaneDepartureWarning.post(() -> {
                    showSetupFailureToast();
                    laneDepartureWarningClick = false;
                    laneDepartureWarningCloseClick = false;
                    //车道偏离预警设置 1 ON 0 OFF
                    int laneDepartureWarningInt = getLaneDepartureWarningInt();

                    LogUtil.d("设置失败，设置车道偏离预警超时： " + laneDepartureWarningInt + (laneDepartureWarningInt == 1 ? "  打开" : "  关闭"));
                    if (laneDepartureWarningInt == Constants.ERROR_CODE) {
                        refreshLaneDepartureWarningUi(laneDepartureWarningInt);
                    } else {
                        refreshLaneDepartureWarningUi(laneDepartureWarningInt == 1 ? -1 : 0);//-1车道偏离预警打开UI按钮置灰 0 车道偏离预警关闭ui显示高亮
                        if (laneDepartureWarningInt == 1) {//-1车道偏离预警打开UI按钮置灰
                            //LDW报警方式 1仅振动 2 仅预警 3 预警并振动
                            int LDWWarningInt = getLDWWarningInt();
                            if (LDWWarningInt == Constants.ERROR_CODE) {
                                refreshLaneDepartureWarningUi(LDWWarningInt);
                            } else {
                                if (LDWWarningInt > 0) {
                                    refreshLaneDepartureWarningUi(LDWWarningInt);
                                }
                            }
                            LogUtil.d("设置失败，设置车道偏离预警超时： " + LDWWarningInt + " ------1仅振动 2 仅预警 3 预警并振动");
                        }
                    }
                });
            }
        };
        laneDepartureWarningTimeOutTimer.schedule(laneDepartureWarningTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelLaneDepartureWarningTimeOut() {
        if (laneDepartureWarningTimeOutTimer != null) {
            laneDepartureWarningTimeOutTimer.cancel();
            laneDepartureWarningTimeOutTimer = null;
        }
        if (laneDepartureWarningTimeOutTask != null) {
            laneDepartureWarningTimeOutTask.cancel();
            laneDepartureWarningTimeOutTask = null;
        }
    }

    private Timer forwardCollisionWarningBrakingTimeOutTimer;
    private TimerTask forwardCollisionWarningBrakingTimeOutTask;

    private void postForwardCollisionWarningBrakingTimeOutDelayed() {
        forwardCollisionWarningBrakingTimeOutTimer = new Timer();
        forwardCollisionWarningBrakingTimeOutTask = new TimerTask() {
            @Override
            public void run() {
                llCollisionWarningBraking.post(() -> {
                    showSetupFailureToast();
                    forwardCollisionWarningBrakingClick = false;
                    //前向碰撞预警及紧急制动 0 关闭 1 仅预警 2仅制动 3 预警并制动
                    int forwardCollisionWarningBrakingInt = getForwardCollisionWarningBrakingInt();
                    refreshForwardCollisionWarningBraking(forwardCollisionWarningBrakingInt);
                    LogUtil.d("设置失败，设置前向碰撞预警及紧急制动超时： " + forwardCollisionWarningBrakingInt + " ------0 关闭 1 仅预警 2仅制动 3 预警并制动");
                });
            }
        };
        forwardCollisionWarningBrakingTimeOutTimer.schedule(forwardCollisionWarningBrakingTimeOutTask, Constants.SET_PROP_TIME_OUT);
    }

    private void cancelForwardCollisionWarningBrakingTimeOut() {
        if (forwardCollisionWarningBrakingTimeOutTimer != null) {
            forwardCollisionWarningBrakingTimeOutTimer.cancel();
            forwardCollisionWarningBrakingTimeOutTimer = null;
        }
        if (forwardCollisionWarningBrakingTimeOutTask != null) {
            forwardCollisionWarningBrakingTimeOutTask.cancel();
            forwardCollisionWarningBrakingTimeOutTask = null;
        }
    }
}
