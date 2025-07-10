#ifndef MCU_H
#define MCU_H

#ifdef __cplusplus
extern "C" {
#endif

//信息上报标识
struct mcu_info_001_t {
    /**
     * 小灯状态信号
     *
     * 0x00：关闭状态
     * 0x01：开启状态
     * 其它：0xff未知错误，0x02->0xfe，开发自定义，用于排查内部问题。
     */
    uint8_t lamplet_status;

    /**
     * 制动液位报警信号
     *
     * 0x00：正常状态
     * 0x01：报警状态
     * 其它：0xff未知错误，0x02->0xfe，开发自定义，用于排查内部问题。
     */
    uint8_t brake_level_alarm_status;

    /**
     * 主驾座位压力检测信号
     *
     * 0x00：未检测到压力
     * 0x01：有检测到压力
     * 其它：0xff未知错误，0x02->0xfe，开发自定义，用于排查内部问题。
     */
    uint8_t main_seat_pressure_status;

    /**
     * 主驾安全带检测信号
     *
     * 0x00：未检测到安全带
     * 0x01：有检测到安全带
     * 其它：0xff未知错误，0x02->0xfe，开发自定义，用于排查内部问题。
     */
    uint8_t main_seat_safety_belt_status;

    /**
     * 引擎信号
     *
     * 0x00：无
     * 0x01：有
     * 其它：0xff未知错误，0x02->0xfe，开发自定义，用于排查内部问题。
     */
    uint8_t engine_status;

    /**
     * 保留
     *
     * 可用于定义MCU错误日志号，将MCU内部运行状态或信息上传给SOC，SOC记录于MCU日志文件中。
     */
    uint8_t low_vol_battery_vol;

    /**
     * 低压蓄电池电压
     *
     * 取值范围0-250，未知错误0xff，精度0.1V
     */
    uint8_t unused[2];

};

int mcu_info_001_pack(
    uint8_t *dst_p,
    const struct mcu_info_001_t *src_p,
    size_t size);

int mcu_info_001_unpack(
    struct mcu_info_001_t *dst_p,
    const uint8_t *src_p,
    size_t size);


//方控信号上报
struct mcu_steering_info_002_t {
    //1表示按下事件，0表示无按下事件
    //bit0：上一曲
    uint8_t key_previous_song;
    //bit1：下一曲
    uint8_t key_next_song;
    //bit2：音量+
    uint8_t key_volume_up;
    //bit3：音量-
    uint8_t key_volume_dowm;
    //bit4：MODE/静音
    uint8_t key_mode_mute;
    //bit5：电话
    uint8_t key_telephone;
    //bit6：语音识别
    uint8_t key_voice;
    //bit7：主界面
    uint8_t key_home;
    //bit0：自定义按键
    uint8_t key_custom;
    //bit1: 长按音量+
    uint8_t key_long_volume_up;
    //bit2: 长按音量-
    uint8_t key_long_volume_dowm;
    //bit3: 长按电话
    uint8_t key_long_telephone;
    //bit4: 长按自定义按键
    uint8_t key_long_custom;
    //bit5：长按上一曲
    uint8_t key_long_previous_song;
    //bit6：长按下一曲
    uint8_t key_long_next_song;

};

// int mcu_steering_info_002_pack(
//     uint8_t *dst_p,
//     const struct mcu_steering_info_002_t *src_p,
//     size_t size);

int mcu_steering_info_002_unpack(
    struct mcu_steering_info_002_t *dst_p,
    const uint8_t *src_p,
    size_t size);

//大灯高度控制
struct mcu_headlight_control_003_t {
    /**
     * 档位标识
     *
     * 0x00：0档，大灯照射最远
     * 0x01：1档
     * 0x02：2档
     * 0x03：3档，大灯照射最近
     */
    uint8_t light_status;

    /**
     * 保留
     *
     */
    uint8_t unused[7];
};

struct mcu_travel_info_0005_t {
    uint32_t  travel_range;
    uint16_t  travel_time;
    uint16_t  average_energy_consumption;
};

int mcu_travel_info_0005_pack(
    uint8_t *dst_p,
    const struct mcu_travel_info_0005_t *src_p,
    size_t size);

int mcu_travel_info_0005_unpack(
    struct mcu_travel_info_0005_t *dst_p,
    const uint8_t *src_p,
    size_t size);


// fm 频道调节
struct mcu_fm_channel_adjust {
    /**
     * 0x01: 搜索
     * 0x02：上一曲
     * 0x03：下一曲
     * 0x04: 播放
     * 0x05: 暂停
     * 0x06: 收藏
     * 0x07: 调频
     * 0xFF: 无效
     * 注：如果发调频的话需要发电台频道，否则电台频道忽略 
    */
    uint8_t  channel_function;

    /**
     * 精度：0.1
     * 单位：MHZ
     * 无效 0xFFFF
    */
    uint16_t  radio_station; 
};

int mcu_fm_channel_adjust_pack(
    uint8_t *dst_p,
    const struct mcu_fm_channel_adjust *src_p,
    size_t size);

int mcu_fm_channel_adjust_unpack(
    struct mcu_fm_channel_adjust *dst_p,
    const uint8_t *src_p,
    size_t size);

//soc 下发心跳包 下发的数据有日、时、分、秒
struct hearbeat_0014{
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t unused[4];    
};    

// soc 下发数据心跳打包
int mcu_hearbeat_0014_pack(
    uint8_t *dst_p,
    const struct hearbeat_0014 *src_p,
    size_t size);

// soc 下发数据心跳解包
int mcu_hearbeat_0014_unpack(
    struct hearbeat_0014 *dst_p,
    const uint8_t *src_p,
    size_t size);


//  fm 评到返回列表
struct mcu_fm_list_0013{
    // fm 频道总数
    uint8_t channel_total;
    
    /*
    精度：0.1
    单位：MHZ
    无效 0xFFFF
    */
    uint8_t current_frame_number;

    /*
    频道列表数据，数据位占6个字节
    每个频道占用两个字节,此处采用uint16_t的数组来存储获取到的fm频道列表，每次最多传输3个 fm 频道
    */
    uint16_t channel_data [3];
};

int mcu_fm_list_0013_pack(
    uint8_t *dst_p,
    const struct mcu_fm_list_0013 *src_p,
    size_t size);

int mcu_fm_list_0013_unpack(
    struct mcu_fm_list_0013 *dst_p,
    const uint8_t *src_p,
    size_t size);


struct mcu_screen_brightness_adjust{

    /**
     *  值范围 0~10
     *  默认值 5 
     *  无效 0xFF
     */

    uint8_t light_level;

    uint8_t auto_adjust_state;

    uint8_t unused[6];
};

int mcu_screen_brightness_adjust_pack(uint8_t *dst_p,
    const struct mcu_screen_brightness_adjust  *src_p,
    size_t size
    );

int mcu_screen_brightness_adjust_unpack(
    struct mcu_screen_brightness_adjust *dst_p,
    const uint8_t *src_p,
    size_t size
);



int mcu_headlight_control_003_pack(
    uint8_t *dst_p,
    const struct mcu_headlight_control_003_t *src_p,
    size_t size);

int mcu_headlight_control_003_unpack(
    struct mcu_headlight_control_003_t *dst_p,
    const uint8_t *src_p,
    size_t size);

//预约充电
struct mcu_appointment_chraging_006_t {
    //预约模式（预约充电方式）00：单次，01：循环
    uint8_t appointment_mode;
    //开始时间（时）范围：00 ~ 23
    uint8_t chraging_begin_time_hour;
    //开始时间（分）范围：00 ~ 59
    uint8_t chraging_begin_time_minute;
    // 结束时间（时）	范围：00 ~ 23
    uint8_t chraging_end_time_hour;
    // 结束时间（分）	范围：00 ~ 59
    uint8_t chraging_end_time_minute;
    // 循环标识 周日
    uint8_t sunday_loop;
    // 循环标识 周一
    uint8_t monday_loop;
    // 循环标识 周二
    uint8_t tuesday_loop;
    // 循环标识 周三
    uint8_t wednesday_loop;
    // 循环标识 周四
    uint8_t thursday_loop;
    // 循环标识 周五
    uint8_t friday_loop;
    // 循环标识 周六
    uint8_t saturday_loop;
    // 充电目标电量	"范围：0 ~ 100 
    uint8_t chraging_target_soc;
};

int mcu_appointment_chraging_006_pack(
    uint8_t *dst_p,
    const struct mcu_appointment_chraging_006_t *src_p,
    size_t size);

int mcu_appointment_chraging_006_unpack(
    struct mcu_appointment_chraging_006_t *dst_p,
    const uint8_t *src_p,
    size_t size);

// 此结构体用于mcu 与soc 同步最新电源信息
struct mcu_soc_power_status{
    /*
    mcu 当前的电源状态
    1： BATOFF模式：车辆蓄电池亏电模式;
    2： SLEEP模式：整车休眠模式;
    3： STANDBY模式：待机模式;
    4： ACTIVE模式：全功能运行模式;
    5： STR模式：SOC挂起到RAM模式;
    */    
    uint8_t mcu_power_status;

    /*
    soc 当前电源状态
    1： BATOFF模式：车辆蓄电池亏电模式;
    2： SLEEP模式：整车休眠模式;
    3： STANDBY模式：待机模式;
    4： ACTIVE模式：全功能运行模式;
    5： STR模式：SOC挂起到RAM模式;
    */
    uint8_t soc_power_status;
    uint8_t unused [6];
};

//  mcu、soc 电源状态信息组包接口
int mcu_soc_power_status_00d_pack(
    uint8_t *dst_p,
    const struct mcu_soc_power_status *src_p,
    size_t size);

// mcu、soc 电源状态信息解包接口
int mcu_soc_power_status_00d__unpack(
    struct mcu_soc_power_status *dst_p,
    const uint8_t *src_p,
    size_t size);

struct muc_volume_00a_t {
    //导航音量
    uint8_t mcu_voiume_navigation;
    //语音播报音量
    uint8_t mcu_voiume_voic;
    //多媒体音量
    uint8_t mcu_voiume_multimedia;
    //电话音量
    uint8_t mcu_voiume_phone;
    //报警音量
    uint8_t mcu_voiume_alarm;
    //按键音
    uint8_t mcu_voiume_key;
};
int muc_volume_00a_pack(
    uint8_t *dst_p,
    const struct muc_volume_00a_t *src_p,
    size_t size);
int muc_volume_00a_unpack(
    struct muc_volume_00a_t *dst_p,
    const uint8_t *src_p,
    size_t size);



struct mcu_info_vin_0108_t {
    uint8_t mcu_info_vin1[8];
};

int mcu_info_vin_0108_pack(
    uint8_t *dst_p,
    const struct mcu_info_vin_0108_t *src_p,
    size_t size);

int mcu_info_vin_0108_unpack(
    struct mcu_info_vin_0108_t *dst_p,
    const uint8_t *src_p,
    size_t size);

struct mcu_info_vin_0208_t {
    uint8_t mcu_info_vin2[8];
};

int mcu_info_vin_0208_pack(
    uint8_t *dst_p,
    const struct mcu_info_vin_0208_t *src_p,
    size_t size);

int mcu_info_vin_0208_unpack(
    struct mcu_info_vin_0208_t *dst_p,
    const uint8_t *src_p,
    size_t size);

struct mcu_info_vin_0308_t {
    uint8_t mcu_info_vin3[8];
};

int mcu_info_vin_0308_pack(
    uint8_t *dst_p,
    const struct mcu_info_vin_0308_t *src_p,
    size_t size);

int mcu_info_vin_0308_unpack(
    struct mcu_info_vin_0308_t *dst_p,
    const uint8_t *src_p,
    size_t size);



struct mcu_info_sn_010D_t {
    uint8_t mcu_info_sn1[8];
};

int mcu_info_sn_010D_pack(
    uint8_t *dst_p,
    const struct mcu_info_sn_010D_t *src_p,
    size_t size);

int mcu_info_sn_010D_unpack(
    struct mcu_info_sn_010D_t *dst_p,
    const uint8_t *src_p,
    size_t size);

struct mcu_info_sn_020D_t {
    uint8_t mcu_info_sn2[8];
};

int mcu_info_sn_020D_pack(
    uint8_t *dst_p,
    const struct mcu_info_sn_020D_t *src_p,
    size_t size);

int mcu_info_sn_020D_unpack(
    struct mcu_info_sn_020D_t *dst_p,
    const uint8_t *src_p,
    size_t size);

struct mcu_info_sn_030D_t {
    uint8_t mcu_info_sn3[8];
};

int mcu_info_sn_030D_pack(
    uint8_t *dst_p,
    const struct mcu_info_sn_030D_t *src_p,
    size_t size);

int mcu_info_sn_030D_unpack(
    struct mcu_info_sn_030D_t *dst_p,
    const uint8_t *src_p,
    size_t size);

struct mcu_info_version_000F_t {
    uint8_t mcu_info_version[8];
};
struct mcu_trav_info_t {
    uint32_t maleage;
    uint16_t time;
    uint16_t speed;
    int avgengc;
};

int mcu_info_version_000F_pack(
    uint8_t *dst_p,
    const struct mcu_info_version_000F_t *src_p,
    size_t size);

int mcu_info_version_000F_unpack(
    struct mcu_info_version_000F_t *dst_p,
    const uint8_t *src_p,
    size_t size);
int mcu_trav_info_unpack(struct mcu_trav_info_t *dst_p,
    const uint8_t *src_p, size_t size);

struct can_communication_state_0x10{
/*
低位开始：值1 表示通信正常，0表示通信异常
第一位： CHASSISE CAN通信状态
第二位： OBDCAN通信状态
第三位： EVCAN通信状态
第四位：BODY CAN通信状态
示例：
CAN0~3他通讯正常， 上报 0x0F, 对应二进制1111
CAN0~3他通讯均异常， 上报 0x00, 对应二进制0
EVCAN/BODY CAN通信正常其他异常,上报 0x05，对应二进制0101
*/
    uint8_t chassise_can_state;
    uint8_t obd_can_state;
    uint8_t ev_can_state;
    uint8_t body_can_state;
    uint8_t unused[4];
};

#ifdef __cplusplus
}
#endif

#endif
