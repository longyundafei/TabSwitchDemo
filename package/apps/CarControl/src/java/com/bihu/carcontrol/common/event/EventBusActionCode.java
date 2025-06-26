package com.bihu.carcontrol.common.event;

/**
 * FileName: EventBusCode
 * Author: WangXu
 * Date: 2023/11/13 17:08
 * Description:eventbus action code
 */
public class EventBusActionCode {
    //车锁驻车时解锁设置
    public static final int LOCK_STATUS_CHANGE_CODE = 101;
    //低速行人警报
    public static final int WARNING_MODE_CHANGE_CODE = 102;
    //左后视镜
    public static final int LEFT_MIRROR_STATUS_CHANGE_CODE = 103;
    //右后视镜
    public static final int RIGHT_MIRROR_STATUS_CHANGE_CODE = 104;
    //保养设置发生变化
    public static final int MAINTENANCE_TIPS_INFO_CHANGE_CODE = 105;
    //能量回收模式变化
    public static final int ENERGY_MODE_CHANGE_CODE = 106;
    //车辆驾驶模式变化
    public static final int DRIVE_MODE_CHANGE_CODE = 107;
    //大灯高度变化
    public static final int LIGHT_HIGH_RATE_CHANGE_CODE = 108;
    //后雾灯开关变化
    public static final int INSTRUMENT_LIGHTS_CHANGE_CODE = 109;
    //续航里程模式变化
    public static final int RANGE_DISPLAY_MODE_CHANGE_CODE = 110;
    //旅行信息变化
    public static final int TRAVEL_MEMORY_CHANGE_CODE = 111;
    //首页切换tab,用作维保设置切换其他tab时关闭可设置状态
    public static final int HOME_SWITCH_TAB_CODE = 112;
    //总里程变化（这里用作维修保养剩余里程更新计算）
    public static final int TOTAL_DRIVING_MILLER_CHANGE_CODE = 113;
    //开门警示
    public static final int OPEN_DOOR_WARNING_CODE = 114;
    //近光灯开关变化
    public static final int LOW_BEAM_LIGHT_ON_CHANGE_CODE = 115;
    //刷新fragment数据，0 驾驶模式 7 方向盘
    public static final int FRESH_FRAGMENT_DATA = 116;
    //雨刮间歇时间
    public static final int WIPER_INTERVAL_TIME = 117;
    //伴我回家
    public static final int ACCOMPANY_BACK_HOME = 118;
    //迎宾解锁
    public static final int WELCOME_UNLOCK_STATUS = 120;
    //闭锁喇叭
    public static final int LOCK_HORN_ENABLE = 121;
    //自动驻车
    public static final int AUTO_PARKING_ON = 122;
    //制动盘擦拭
    public static final int BRAKE_DISC_WIPING_ON = 123;
    //助力转向模式
    public static final int POWER_STEERING_MODE = 124;
    //电子驻车模式
    public static final int ELECTRONIC_PARKING_MODE = 125;
    //驾驶驱动强度
    public static final int DRIVING_POWER_ST = 126;
    //能量回收强度
    public static final int ENERGY_RECOV_LEVEL = 127;
    //车辆上下电状态
    public static final int POWER_STATE = 128;
    //蠕行模式
    public static final int PERISTALTIC_MODE = 129;
    //按键背光亮度
    public static final int BACK_LADJVALLFB = 130;
    //解锁主动通风
    public static final int AC_UNLOCK_VENT_SET_FB = 131;
    //远离闭锁
    public static final int ILLMND_LOCK_STATUS = 132;
    //充电电流限制
    public static final int AC_CHARGING_MAX_CURR = 134;
    //胎压学习
    public static final int TPMS_LEARNING_REQ = 135;
    //充电目标电量
    public static final int CHARGING_LOOP_DATA_CHANGE_CODE = 136;
    //智能远光灯
    public static final int FCM_HMA_ENABLE_SW = 137;
    //LCC车道保持
    public static final int FCM_ICAENA_SWT = 138;
    //避让大车
    public static final int FCM_ICAAVD_TRUCK_SET = 139;
    //车道纠偏辅助
    public static final int FCM_LKA_ENABLE_SW = 140;
    //车道偏离预警
    public static final int FCM_LDW_ALERTMETHOD_SW = 141;
    //紧急车道保持
    public static final int FCM_ELKENB_SW = 142;
    //前向碰撞预警及紧急制动 0 关闭 1 仅预警 2仅制动 3 预警并制动
    public static final int FCM_FCW_ENABLE_SW = 143;
    //紧急制动
    public static final int HU_AEB_ENABLE_SW = 144;
    //盲区监测
    public static final int CRRR_BSD_ENABLE_SW = 145;
    //开门预警
    public static final int CRRR_DOWENA_SWT = 146;
    //后碰撞预警
    public static final int CRRR_RCW_ENABLE_SW = 147;
    //后方横向穿行预警
    public static final int CRRR_RCTA_ENABLE_SW = 148;
    //限速标识识别
    public static final int FCM_SLIF_ENABLE_SW = 149;
    //超速报警提示
    public static final int FCM_SLWF_ENABLE_SW = 150;
    //国家代码
    public static final int HU_NAVI_COUNTRY_ID = 151;
    //LDW报警方式
    public static final int FCM_LDW_ALERTMETHOD_SETFB = 152;
    //车速变化
    public static final int DRIVING_SPEED_CHANGE = 153;
    //平均电耗
    public static final int AVERAGE_POWER_CONSUMPTION = 154;
    //LCC激活
    public static final int FCM_ACT_DRV_ASST_STS_HMI = 155;
}
