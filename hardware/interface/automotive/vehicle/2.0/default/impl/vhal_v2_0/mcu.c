#include <string.h>

#include "mcu.h"

static inline uint8_t unpack_right_shift_u8(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value & mask) >> shift);
}

static inline uint8_t pack_left_shift_u8(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value << shift) & mask);
}

static inline uint16_t unpack_right_shift_u16(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint16_t)((uint16_t)(value & mask) >> shift);
}

static inline uint8_t pack_right_shift_u16(
    uint16_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value >> shift) & mask);
}

static inline uint8_t pack_left_shift_u16(
    uint16_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value << shift) & mask);
}

static inline uint16_t unpack_left_shift_u16(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint16_t)((uint16_t)(value & mask) << shift);
}

static inline uint8_t pack_right_shift_u32(
    uint32_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value >> shift) & mask);
}

static inline uint32_t unpack_right_shift_u32(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint32_t)((uint32_t)(value & mask) >> shift);
}

static inline uint8_t pack_left_shift_u32(
    uint32_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value << shift) & mask);
}

static inline uint32_t unpack_left_shift_u32(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint32_t)((uint32_t)(value & mask) << shift);
}

int mcu_info_001_pack(
    uint8_t *dst_p,
    const struct mcu_info_001_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    memset(&dst_p[0], 0, 8);
    dst_p[0] = src_p->lamplet_status;
    dst_p[1] = src_p->brake_level_alarm_status;
    dst_p[2] = src_p->main_seat_pressure_status;
    dst_p[3] = src_p->main_seat_safety_belt_status;
    dst_p[4] = src_p->engine_status;
    dst_p[5] = src_p->low_vol_battery_vol;
    return (8);
}

int mcu_info_001_unpack(
    struct mcu_info_001_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    dst_p->lamplet_status = src_p[0];
    dst_p->brake_level_alarm_status = src_p[1];
    dst_p->main_seat_pressure_status = src_p[2];
    dst_p->main_seat_safety_belt_status = src_p[3];
    dst_p->engine_status = src_p[4];
    dst_p->low_vol_battery_vol = src_p[5];

    return (0);
}

// soc 下发数据心跳打包
int mcu_hearbeat_0014_pack(
    uint8_t *dst_p,
    const struct hearbeat_0014 *src_p,
    size_t size){
    if(size < 8u){
        return (-1);
    }
    memset(&dst_p[0], 0, 8);
    dst_p[0] = src_p->day;
    dst_p[1] = src_p->hour;
    dst_p[2] = src_p->minute;
    dst_p[3] = src_p->second;
    return (8);
}

// soc 下发数据心跳解包
int mcu_hearbeat_0014_unpack(
    struct hearbeat_0014 *dst_p,
    const uint8_t *src_p,
    size_t size){
    if(size < 8u){
        return (-1);
    }
    dst_p->day = src_p[0];
    dst_p->hour = src_p[1];
    dst_p->minute = src_p[2];
    dst_p->second = src_p[3];
    return 0;
}


// int mcu_steering_info_002_pack(
//     uint8_t *dst_p,
//     const struct mcu_steering_info_002_t *src_p,
//     size_t size)
// {
//     if (size < 8u) {
//         return (-1);
//     }

//     memset(&dst_p[0], 0, 8);

//     dst_p[0] = src_p->key_function;
//     dst_p[1] = src_p->key_status;

//     return (8);
// }

int mcu_steering_info_002_unpack(
    struct mcu_steering_info_002_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }

    dst_p->key_previous_song = unpack_right_shift_u8(src_p[0], 0u, 0x01u);
    dst_p->key_next_song = unpack_right_shift_u8(src_p[0], 1u, 0x02u);
    dst_p->key_volume_up = unpack_right_shift_u8(src_p[0], 2u, 0x04u);
    dst_p->key_volume_dowm = unpack_right_shift_u8(src_p[0], 3u, 0x08u);
    dst_p->key_mode_mute = unpack_right_shift_u8(src_p[0], 4u, 0x10u);
    dst_p->key_telephone = unpack_right_shift_u8(src_p[0], 5u, 0x20u);
    dst_p->key_voice = unpack_right_shift_u8(src_p[0], 6u, 0x40u);
    dst_p->key_home = unpack_right_shift_u8(src_p[0], 7u, 0x80u);

    dst_p->key_custom = unpack_right_shift_u8(src_p[1], 0u, 0x01u);
    dst_p->key_long_volume_up = unpack_right_shift_u8(src_p[1], 1u, 0x02u);
    dst_p->key_long_volume_dowm = unpack_right_shift_u8(src_p[1], 2u, 0x04u);
    dst_p->key_long_telephone = unpack_right_shift_u8(src_p[1], 3u, 0x08u);
    dst_p->key_long_custom = unpack_right_shift_u8(src_p[1], 4u, 0x10u);
    dst_p->key_long_previous_song = unpack_right_shift_u8(src_p[1], 5u, 0x20u);
    dst_p->key_long_next_song = unpack_right_shift_u8(src_p[1], 6u, 0x40u);
    

    return (0);
}

//  mcu、soc 电源状态信息组包
int mcu_soc_power_status_00d_pack(
    uint8_t *dst_p,
    const struct mcu_soc_power_status *src_p,
    size_t size){
    if (size < 8u) {
        return (-1);
    }

    memset(&dst_p[0], 0, 8);
    dst_p[0] = src_p->mcu_power_status;
    dst_p[1] = src_p->soc_power_status;

    return (8);
}

// mcu、soc 电源状态信息解包
int mcu_soc_power_status_00d__unpack(
    struct mcu_soc_power_status *dst_p,
    const uint8_t *src_p,
    size_t size){
    if (size < 8u) {
        return (-1);
    }

    dst_p->mcu_power_status = src_p[0];
    dst_p->soc_power_status = src_p[1];

    return (0);
}

int mcu_headlight_control_003_pack(
    uint8_t *dst_p,
    const struct mcu_headlight_control_003_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }

    memset(&dst_p[0], 0, 8);

    dst_p[0] = src_p->light_status;

    return (8);
}


int mcu_headlight_control_003_unpack(
    struct mcu_headlight_control_003_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }

    dst_p->light_status = src_p[0];

    return (0);
}

int mcu_travel_info_0005_pack(
    uint8_t *dst_p,
    const struct mcu_travel_info_0005_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }

    memset(&dst_p[0], 0, 8);

    dst_p[0] |= pack_right_shift_u32(src_p->travel_range, 16u, 0xffu);
    dst_p[1] |= pack_right_shift_u32(src_p->travel_range, 8u, 0xffu);
    dst_p[2] |= pack_left_shift_u32(src_p->travel_range, 0u, 0xffu);
    dst_p[4]  |= pack_right_shift_u16(src_p->travel_time, 8u, 0xffu);
    dst_p[5]  |= pack_left_shift_u16(src_p->travel_time, 0u, 0xffu);
    dst_p[6]  |= pack_right_shift_u16(src_p->average_energy_consumption, 8u, 0xffu);
    dst_p[7]  |= pack_left_shift_u16(src_p->average_energy_consumption, 0u, 0xffu);
    return (8);
}

int mcu_travel_info_0005_unpack(
    struct mcu_travel_info_0005_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }

    dst_p->travel_range = unpack_left_shift_u32(src_p[0], 16u, 0xffu);
    dst_p->travel_range |= unpack_left_shift_u32(src_p[1], 8u, 0xffu);
    dst_p->travel_range |= unpack_right_shift_u32(src_p[2], 0u, 0xffu);
    dst_p->travel_time  = unpack_left_shift_u16(src_p[4], 8u, 0xffu);
    dst_p->travel_time  |= unpack_right_shift_u16(src_p[5], 0u, 0xffu);
    dst_p->average_energy_consumption  = unpack_left_shift_u16(src_p[6], 8u, 0xffu);
    dst_p->average_energy_consumption  |= unpack_right_shift_u16(src_p[7], 0u, 0xffu);
    return (0);
}

int mcu_screen_brightness_adjust_pack(uint8_t *dst_p,
    const struct mcu_screen_brightness_adjust  *src_p,
    size_t size
    ){
    if (size < 8u) {
        return (-1);
    }

    memset(&dst_p[0], 0, 8);

    dst_p[0] = src_p->light_level;
    dst_p[1] = src_p->auto_adjust_state;

    return (8);        
}

int mcu_screen_brightness_adjust_unpack(
    struct mcu_screen_brightness_adjust *dst_p,
    const uint8_t *src_p,
    size_t size
){
        if (size < 8u) {
        return (-1);
    }

    dst_p->light_level = src_p[0];
    dst_p->auto_adjust_state = src_p[1];
    return (0);
}

int mcu_appointment_chraging_006_pack(
    uint8_t *dst_p,
    const struct mcu_appointment_chraging_006_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    memset(&dst_p[0], 0, 8);
    dst_p[0] = src_p->appointment_mode;
    dst_p[1] = src_p->chraging_begin_time_hour;
    dst_p[2] = src_p->chraging_begin_time_minute;
    dst_p[3] = src_p->chraging_end_time_hour;
    dst_p[4] = src_p->chraging_end_time_minute;
    dst_p[5] |= pack_left_shift_u8(src_p->sunday_loop, 0u, 0x01u);
    dst_p[5] |= pack_left_shift_u8(src_p->monday_loop, 1u,  0x02u);
    dst_p[5] |= pack_left_shift_u8(src_p->tuesday_loop, 2u,  0x04u);
    dst_p[5] |= pack_left_shift_u8(src_p->wednesday_loop, 3u, 0x08u);
    dst_p[5] |= pack_left_shift_u8(src_p->thursday_loop, 4u, 0x10u);
    dst_p[5] |= pack_left_shift_u8(src_p->friday_loop, 5u, 0x20u);
    dst_p[5] |= pack_left_shift_u8(src_p->saturday_loop, 6u, 0x40u);
    dst_p[6] = src_p->chraging_target_soc;
    return (0);
}


int mcu_appointment_chraging_006_unpack(
    struct mcu_appointment_chraging_006_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }

    dst_p->appointment_mode = src_p[0];
    dst_p->chraging_begin_time_hour = src_p[1];
    dst_p->chraging_begin_time_minute = src_p[2];
    dst_p->chraging_end_time_hour = src_p[3];
    dst_p->chraging_end_time_minute = src_p[4];
    dst_p->sunday_loop = unpack_right_shift_u8(src_p[5], 0u, 0x01u);
    dst_p->monday_loop = unpack_right_shift_u8(src_p[5], 1u, 0x02u);
    dst_p->tuesday_loop = unpack_right_shift_u8(src_p[5], 2u, 0x04u);
    dst_p->wednesday_loop = unpack_right_shift_u8(src_p[5], 3u, 0x08u);
    dst_p->thursday_loop = unpack_right_shift_u8(src_p[5], 4u, 0x10u);
    dst_p->friday_loop = unpack_right_shift_u8(src_p[5], 5u, 0x20u);
    dst_p->saturday_loop = unpack_right_shift_u8(src_p[5], 6u, 0x40u);
    dst_p->chraging_target_soc = src_p[6];

    return (0);
}

int mcu_fm_channel_adjust_pack(
    uint8_t *dst_p,
    const struct mcu_fm_channel_adjust *src_p,
    size_t size){
    if (size < 8u) {
        return (-1);
    }
    memset(&dst_p[0], 0, 8);        
    dst_p[0]  |= pack_left_shift_u8(src_p->channel_function, 0u, 0xffu);
    dst_p[1]  |= pack_right_shift_u16(src_p->radio_station, 8u, 0xffu);
    dst_p[2]  |= pack_left_shift_u16(src_p->radio_station, 0u, 0xffu);
    return (8);
}


int mcu_fm_channel_adjust_unpack(
    struct mcu_fm_channel_adjust *dst_p,
    const uint8_t *src_p,
    size_t size){
    if (size < 8u) {
        return (-1);
    }

    dst_p->channel_function =  unpack_right_shift_u8(src_p[0], 0u, 0xffu);
    dst_p->radio_station    =  unpack_left_shift_u16(src_p[1], 8u, 0xffu);
    dst_p->radio_station    |= unpack_right_shift_u16(src_p[2], 0u, 0xffu);  
    return 0;
}

int mcu_fm_list_0013_pack(
    uint8_t *dst_p,
    const struct mcu_fm_list_0013 *src_p,
    size_t size){
    if (size < 8u) {
        return (-1);
    }
    memset(&dst_p[0], 0, 8);
    dst_p[0]  |= pack_left_shift_u8(src_p->channel_total, 0u, 0xffu);
    dst_p[1]  |= pack_left_shift_u8(src_p->current_frame_number, 0u, 0xffu);

    dst_p[2]  |= pack_right_shift_u16(src_p->channel_data[0], 8u, 0xffu);
    dst_p[3]  |= pack_left_shift_u16(src_p->channel_data[0], 0u, 0xffu);    

    dst_p[4]  |= pack_right_shift_u16(src_p->channel_data[1], 8u, 0xffu);
    dst_p[5]  |= pack_left_shift_u16(src_p->channel_data[1], 0u, 0xffu);

    dst_p[6]  |= pack_right_shift_u16(src_p->channel_data[2], 8u, 0xffu);
    dst_p[7]  |= pack_left_shift_u16(src_p->channel_data[2], 0u, 0xffu);
    return (8);
}

int mcu_fm_list_0013_unpack(
    struct mcu_fm_list_0013 *dst_p,
    const uint8_t *src_p,
    size_t size){
    if (size < 8u) {
        return (-1);
    }    
    dst_p->channel_total = unpack_right_shift_u8(src_p[0], 0u, 0xffu);
    dst_p->current_frame_number = unpack_right_shift_u8(src_p[1], 0u, 0xffu);
    dst_p->channel_data[0] = unpack_left_shift_u16(src_p[2], 8u, 0xffu);
    dst_p->channel_data[0]  |= unpack_right_shift_u16(src_p[3], 0u, 0xffu);

    dst_p->channel_data[1] = unpack_left_shift_u16(src_p[4], 8u, 0xffu);
    dst_p->channel_data[1]  |= unpack_right_shift_u16(src_p[5], 0u, 0xffu);

    dst_p->channel_data[2] = unpack_left_shift_u16(src_p[6], 8u, 0xffu);
    dst_p->channel_data[2]  |= unpack_right_shift_u16(src_p[7], 0u, 0xffu);
    return 0;        
}


int muc_volume_00a_pack(uint8_t *dst_p, const struct muc_volume_00a_t *src_p, size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    memset(&dst_p[0], 0, 8);
    dst_p[0] = src_p->mcu_voiume_navigation;
    dst_p[1] = src_p->mcu_voiume_voic;
    dst_p[2] = src_p->mcu_voiume_multimedia;
    dst_p[3] = src_p->mcu_voiume_phone;
    dst_p[4] = src_p->mcu_voiume_alarm;
    dst_p[5] = src_p->mcu_voiume_key;
    return (8);
}
int muc_volume_00a_unpack(struct muc_volume_00a_t *dst_p, const uint8_t *src_p, size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    dst_p->mcu_voiume_navigation = src_p[0];
    dst_p->mcu_voiume_voic = src_p[1];
    dst_p->mcu_voiume_multimedia = src_p[2];
    dst_p->mcu_voiume_phone = src_p[3];
    dst_p->mcu_voiume_alarm = src_p[4];
    dst_p->mcu_voiume_key = src_p[5];
    return (0);
}

int mcu_info_vin_0108_pack(
    uint8_t *dst_p,
    const struct mcu_info_vin_0108_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    memset(&dst_p[0], 0, 8);
    dst_p[0] = src_p->mcu_info_vin1[0];
    dst_p[1] = src_p->mcu_info_vin1[1];
    dst_p[2] = src_p->mcu_info_vin1[2];
    dst_p[3] = src_p->mcu_info_vin1[3];
    dst_p[4] = src_p->mcu_info_vin1[4];
    dst_p[5] = src_p->mcu_info_vin1[5];
    dst_p[6] = src_p->mcu_info_vin1[6];
    dst_p[7] = src_p->mcu_info_vin1[7];
    return (8);
}

int mcu_info_vin_0108_unpack(
    struct mcu_info_vin_0108_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    dst_p->mcu_info_vin1[0] = src_p[0];
    dst_p->mcu_info_vin1[1] = src_p[1];
    dst_p->mcu_info_vin1[2] = src_p[2];
    dst_p->mcu_info_vin1[3] = src_p[3];
    dst_p->mcu_info_vin1[4] = src_p[4];
    dst_p->mcu_info_vin1[5] = src_p[5];
    dst_p->mcu_info_vin1[6] = src_p[6];
    dst_p->mcu_info_vin1[7] = src_p[7];
    return (0);
}

int mcu_info_vin_0208_pack(
    uint8_t *dst_p,
    const struct mcu_info_vin_0208_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    memset(&dst_p[0], 0, 8);
    dst_p[0] = src_p->mcu_info_vin2[0];
    dst_p[1] = src_p->mcu_info_vin2[1];
    dst_p[2] = src_p->mcu_info_vin2[2];
    dst_p[3] = src_p->mcu_info_vin2[3];
    dst_p[4] = src_p->mcu_info_vin2[4];
    dst_p[5] = src_p->mcu_info_vin2[5];
    dst_p[6] = src_p->mcu_info_vin2[6];
    dst_p[7] = src_p->mcu_info_vin2[7];
    return (8);
}

int mcu_info_vin_0208_unpack(
    struct mcu_info_vin_0208_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    dst_p->mcu_info_vin2[0] = src_p[0];
    dst_p->mcu_info_vin2[1] = src_p[1];
    dst_p->mcu_info_vin2[2] = src_p[2];
    dst_p->mcu_info_vin2[3] = src_p[3];
    dst_p->mcu_info_vin2[4] = src_p[4];
    dst_p->mcu_info_vin2[5] = src_p[5];
    dst_p->mcu_info_vin2[6] = src_p[6];
    dst_p->mcu_info_vin2[7] = src_p[7];
    return (0);
}


int mcu_info_vin_0308_pack(
    uint8_t *dst_p,
    const struct mcu_info_vin_0308_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    memset(&dst_p[0], 0, 8);
    dst_p[0] = src_p->mcu_info_vin3[0];
    dst_p[1] = src_p->mcu_info_vin3[1];
    dst_p[2] = src_p->mcu_info_vin3[2];
    dst_p[3] = src_p->mcu_info_vin3[3];
    dst_p[4] = src_p->mcu_info_vin3[4];
    dst_p[5] = src_p->mcu_info_vin3[5];
    dst_p[6] = src_p->mcu_info_vin3[6];
    dst_p[7] = src_p->mcu_info_vin3[7];
    return (8);
}

int mcu_info_vin_0308_unpack(
    struct mcu_info_vin_0308_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    dst_p->mcu_info_vin3[0] = src_p[0];
    dst_p->mcu_info_vin3[1] = src_p[1];
    dst_p->mcu_info_vin3[2] = src_p[2];
    dst_p->mcu_info_vin3[3] = src_p[3];
    dst_p->mcu_info_vin3[4] = src_p[4];
    dst_p->mcu_info_vin3[5] = src_p[5];
    dst_p->mcu_info_vin3[6] = src_p[6];
    dst_p->mcu_info_vin3[7] = src_p[7];
    return (0);
}


int mcu_info_sn_010D_pack(
    uint8_t *dst_p,
    const struct mcu_info_sn_010D_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    memset(&dst_p[0], 0, 8);
    dst_p[0] = src_p->mcu_info_sn1[0];
    dst_p[1] = src_p->mcu_info_sn1[1];
    dst_p[2] = src_p->mcu_info_sn1[2];
    dst_p[3] = src_p->mcu_info_sn1[3];
    dst_p[4] = src_p->mcu_info_sn1[4];
    dst_p[5] = src_p->mcu_info_sn1[5];
    dst_p[6] = src_p->mcu_info_sn1[6];
    dst_p[7] = src_p->mcu_info_sn1[7];
    return (8);
}

int mcu_info_sn_010D_unpack(
    struct mcu_info_sn_010D_t *dst_p,
    const uint8_t *src_p,
    size_t size)

{
    if (size < 8u) {
        return (-1);
    }
    dst_p->mcu_info_sn1[0] = src_p[0];
    dst_p->mcu_info_sn1[1] = src_p[1];
    dst_p->mcu_info_sn1[2] = src_p[2];
    dst_p->mcu_info_sn1[3] = src_p[3];
    dst_p->mcu_info_sn1[4] = src_p[4];
    dst_p->mcu_info_sn1[5] = src_p[5];
    dst_p->mcu_info_sn1[6] = src_p[6];
    dst_p->mcu_info_sn1[7] = src_p[7];
    return (0);
}


int mcu_info_sn_020D_pack(
    uint8_t *dst_p,
    const struct mcu_info_sn_020D_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    memset(&dst_p[0], 0, 8);
    dst_p[0] = src_p->mcu_info_sn2[0];
    dst_p[1] = src_p->mcu_info_sn2[1];
    dst_p[2] = src_p->mcu_info_sn2[2];
    dst_p[3] = src_p->mcu_info_sn2[3];
    dst_p[4] = src_p->mcu_info_sn2[4];
    dst_p[5] = src_p->mcu_info_sn2[5];
    dst_p[6] = src_p->mcu_info_sn2[6];
    dst_p[7] = src_p->mcu_info_sn2[7];
    return (8);
}

int mcu_info_sn_020D_unpack(
    struct mcu_info_sn_020D_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    dst_p->mcu_info_sn2[0] = src_p[0];
    dst_p->mcu_info_sn2[1] = src_p[1];
    dst_p->mcu_info_sn2[2] = src_p[2];
    dst_p->mcu_info_sn2[3] = src_p[3];
    dst_p->mcu_info_sn2[4] = src_p[4];
    dst_p->mcu_info_sn2[5] = src_p[5];
    dst_p->mcu_info_sn2[6] = src_p[6];
    dst_p->mcu_info_sn2[7] = src_p[7];
    return (0);
}

int mcu_info_sn_030D_pack(
    uint8_t *dst_p,
    const struct mcu_info_sn_030D_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    memset(&dst_p[0], 0, 8);
    dst_p[0] = src_p->mcu_info_sn3[0];
    dst_p[1] = src_p->mcu_info_sn3[1];
    dst_p[2] = src_p->mcu_info_sn3[2];
    dst_p[3] = src_p->mcu_info_sn3[3];
    dst_p[4] = src_p->mcu_info_sn3[4];
    dst_p[5] = src_p->mcu_info_sn3[5];
    dst_p[6] = src_p->mcu_info_sn3[6];
    dst_p[7] = src_p->mcu_info_sn3[7];
    return (8);
}

int mcu_info_sn_030D_unpack(
    struct mcu_info_sn_030D_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    dst_p->mcu_info_sn3[0] = src_p[0];
    dst_p->mcu_info_sn3[1] = src_p[1];
    dst_p->mcu_info_sn3[2] = src_p[2];
    dst_p->mcu_info_sn3[3] = src_p[3];
    dst_p->mcu_info_sn3[4] = src_p[4];
    dst_p->mcu_info_sn3[5] = src_p[5];
    dst_p->mcu_info_sn3[6] = src_p[6];
    dst_p->mcu_info_sn3[7] = src_p[7];
    return (0);
}

int mcu_info_version_000F_pack(
    uint8_t *dst_p,
    const struct mcu_info_version_000F_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    memset(&dst_p[0], 0, 8);
    dst_p[0] = src_p->mcu_info_version[0];
    dst_p[1] = src_p->mcu_info_version[1];
    dst_p[2] = src_p->mcu_info_version[2];
    dst_p[3] = src_p->mcu_info_version[3];
    dst_p[4] = src_p->mcu_info_version[4];
    dst_p[5] = src_p->mcu_info_version[5];
    dst_p[6] = src_p->mcu_info_version[6];
    dst_p[7] = src_p->mcu_info_version[7];
    return (8);
}

int mcu_info_version_000F_unpack(
    struct mcu_info_version_000F_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    dst_p->mcu_info_version[0] = src_p[0];
    dst_p->mcu_info_version[1] = src_p[1];
    dst_p->mcu_info_version[2] = src_p[2];
    dst_p->mcu_info_version[3] = src_p[3];
    dst_p->mcu_info_version[4] = src_p[4];
    dst_p->mcu_info_version[5] = src_p[5];
    dst_p->mcu_info_version[6] = src_p[6];
    dst_p->mcu_info_version[7] = src_p[7];
    return (0);
}
int mcu_trav_info_unpack(struct mcu_trav_info_t *dst_p,
    const uint8_t *src_p, size_t size)
{
    if (size < 8u) {
        return (-1);
    }
    dst_p->maleage = src_p[2]<<16;
    dst_p->maleage |= src_p[1]<<8;
    dst_p->maleage |= src_p[0];

    dst_p->time = (src_p[4]<<8)|src_p[3];

    dst_p->speed = (src_p[6]&0xf)<<8;
    dst_p->speed |= src_p[5];

    dst_p->avgengc = (src_p[7]&0x7f)<<4;
    dst_p->avgengc |= (src_p[6]>>4)&0xf;

    if (src_p[7]&0x80) {
        dst_p->avgengc = -dst_p->avgengc;
    }
    return 0;
}



