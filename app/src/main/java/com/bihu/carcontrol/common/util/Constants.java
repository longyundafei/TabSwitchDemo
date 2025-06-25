package com.bihu.carcontrol.common.util;

/**
 * FileName: Constant
 * Author: WangXu
 * Date: 2025/6/6 9:41
 * Description:
 */
public class Constants {
    public static final int SET_PROP_TIME_OUT = 1000;
    public static final int ERROR_CODE = -1024;
    public static final float ERROR_ALPHA = 0.4f;
    public static final String PROPERTIES_SHOW = "0x1";
    //智能远光灯配置字字段 1显示
    public static final String LIGHT_CONTROL_STR = "persist.gecko.variant.auto_high_beam";
    //迎宾解锁配置字字段 1显示
    public static final String WELCOME_TO_UNLOCK_STR = "persist.gecko.variant.ambient_light";
    //蠕行模式配置字字段 1显示
    public static final String WORM_MODE_STR = "persist.gecko.variant.traction_ctrl";
    //胎压学习2、胎压复位1 配置字字段
    public static final String PRESSURE_STR = "persist.gecko.variant.tpms";
    //LCC车道居中辅助配置字字段 1显示
    public static final String LCC_LANE_KEEPING_STR = "persist.gecko.variant.lane_assist";
    //ELK紧急车道保持配置字字段  1显示
    public static final String EMERGENCY_LANE_KEEPING_STR = "persist.gecko.variant.emergency_lane";
    //侧向盲区辅助配置字字段  1显示
    public static final String LATERAL_WARNING_AID_STR = "persist.gecko.variant.blindspot_asst";
    //后向盲区辅助配置字字段  1显示
    public static final String BACKWARD_BLIND_AID_STR = "persist.gecko.variant.rear_x_alert";
    //车道偏离预警及纠正配置字字段  1显示
    public static final String LANE_DEPARTURE_ASSIST_STR = "persist.gecko.variant.lat_safety";
    //前向主动安全辅助配置字字段  1显示
    public static final String FORWARD_WARNING_AID_STR = "persist.gecko.variant.longi_safety";
    //智能限速辅助配置字字段  1显示
    public static final String LIMITING_AID_STR = "persist.gecko.variant.speed_assist";
}
