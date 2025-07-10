#include <iostream>
#include <log/log.h>
#include "DebugManager.h"
#include "FunctionMap.h"
#include "ev_can_ccm_4_0.h"
#include "body_can_ccm_4_0.h"
#include "chassise_can_ccm_4_0.h"
#include "mcu.h"
#include "RecordData.h"
#include <string>
#include <android-base/logging.h>
#include "me11_cmx_baccanfd_ots_car_v02.h"

namespace android::hardware::automotive::vehicle::V2_0::impl {
//---------------------合包---------------------
void printData(const std::vector<uint8_t> packet)
{
    std::cout << "packet:   ";
    for (size_t i = 0; i < packet.size(); i++) {
        std::cout << std::hex << "0x" << static_cast<int>(packet[i]) << " ";
    }
    std::cout << std::endl;
}

bool is_valid_data(std::vector<uint8_t> message)
{
    if (message.size() < 8) {
        return false;
    }
    int size  = static_cast<int> (message.size());
    for (int i = 0; i < size; i++) {
        if (message[i] != 0xff) {
            return true;
        }
    }

    return false;
}

// std::vector<uint8_t> a_xxxxxxxxxxxxxx(void *buf){
//     struct xxxxxxxxxxxxxx *dst_p = static_cast<struct xxxxxxxxxxxxxx *>(buf);
//     uint8_t data[8] = { 0 };
//     xxxxxxxxxxxxxx(data, dst_p, 8);

//     std::vector<uint8_t> packet(data, data + 8);
//     // printData(packet);
//     return packet;
// }

//ev_can
std::vector<uint8_t> a_me11_cmx_v02_hu_c_bac_pack(void *buf){
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_hu_c_bac_t *dst_p = static_cast<struct me11_cmx_baccanfd_ots_car_v02_07_20240520_hu_c_bac_t *>(buf);
    uint8_t data[8] = { 0 };
    me11_cmx_baccanfd_ots_car_v02_07_20240520_hu_c_bac_pack(data, dst_p, 8);

    std::vector<uint8_t> packet(data, data + 8);
    // printData(packet);
    return packet;
}

std::vector<uint8_t> a_me11_cmx_v02_hu_b_bac_pack(void *buf){
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_hu_b_bac_t *dst_p = static_cast<struct me11_cmx_baccanfd_ots_car_v02_07_20240520_hu_b_bac_t *>(buf);
    uint8_t data[32] = { 0 };
    me11_cmx_baccanfd_ots_car_v02_07_20240520_hu_b_bac_pack(data, dst_p, 32);

    std::vector<uint8_t> packet(data, data + 32);
    // printData(packet);
    return packet;
}

std::vector<uint8_t> a_me11_cmx_v02_hu_a_bac_pack(void *buf){
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_hu_a_bac_t *dst_p = static_cast<struct me11_cmx_baccanfd_ots_car_v02_07_20240520_hu_a_bac_t *>(buf);
    uint8_t data[64] = { 0 };
    me11_cmx_baccanfd_ots_car_v02_07_20240520_hu_a_bac_pack(data, dst_p, 64);

    std::vector<uint8_t> packet(data, data + 64);
    // printData(packet);
    return packet;
}

std::vector<uint8_t> a_me11_cmx_v02_nm_asr_cdc_bac_pack(void *buf){
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_nm_asr_cdc_bac_t *dst_p = static_cast<struct me11_cmx_baccanfd_ots_car_v02_07_20240520_nm_asr_cdc_bac_t *>(buf);
    uint8_t data[8] = { 0 };
    me11_cmx_baccanfd_ots_car_v02_07_20240520_nm_asr_cdc_bac_pack(data, dst_p, 8);
    std::vector<uint8_t> packet(data, data + 8);
    // printData(packet);
    return packet;
}

std::vector<uint8_t> a_me11_cmx_v02_tbox_bjs_time_bac_pack(void *buf){
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_tbox_bjs_time_bac_t *dst_p = static_cast<struct me11_cmx_baccanfd_ots_car_v02_07_20240520_tbox_bjs_time_bac_t *>(buf);
    uint8_t data[8] = { 0 };
    me11_cmx_baccanfd_ots_car_v02_07_20240520_tbox_bjs_time_bac_pack(data, dst_p, 8);
    std::vector<uint8_t> packet(data, data + 8);
    // printData(packet);
    return packet;
}

const std::unordered_map<uint16_t, packFunctionPtr> FunctionMap::pack_ev_can_ = {
    {0x01EB, &a_me11_cmx_v02_hu_a_bac_pack},
    {0x01D0, &a_me11_cmx_v02_hu_b_bac_pack},
    {0x02FB, &a_me11_cmx_v02_hu_c_bac_pack},
    {0x0422, &a_me11_cmx_v02_nm_asr_cdc_bac_pack},
    {0x03B3, &a_me11_cmx_v02_tbox_bjs_time_bac_pack},
};

//body_can
const std::unordered_map<uint16_t, packFunctionPtr> FunctionMap::pack_body_can_ = {
};

//chassise_can
const std::unordered_map<uint16_t, packFunctionPtr> FunctionMap::pack_chassise_can_ = {
};

//mcu
std::vector<uint8_t> a_mcu_info_001_pack(void *buf){
    struct mcu_info_001_t *dst_p = static_cast<struct mcu_info_001_t *>(buf);
    uint8_t data[8] = { 0 };
    mcu_info_001_pack(data, dst_p, 8);

    std::vector<uint8_t> packet(data, data + 8);
    // printData(packet);
    return packet;
}

// soc 下发心跳包打包
std::vector<uint8_t> a_mcu_hearbeat_0014_pack(void *buf){
    struct hearbeat_0014 *dst_p = static_cast<struct hearbeat_0014 *>(buf);
    uint8_t data[8] = { 0 };
    mcu_hearbeat_0014_pack(data, dst_p, 8);

    std::vector<uint8_t> packet(data, data + 8);
    // printData(packet);
    return packet;
}



std::vector<uint8_t> a_mcu_headlight_control_003_pack(void *buf){
    struct mcu_headlight_control_003_t *dst_p = static_cast<struct mcu_headlight_control_003_t *>(buf);
    uint8_t data[8] = { 0 };
    mcu_headlight_control_003_pack(data, dst_p, 8);

    std::vector<uint8_t> packet(data, data + 8);
    // printData(packet);
    return packet;
}

std::vector<uint8_t> a_mcu_rema_disp_status_20_pack(void *buf){
    struct mcu_headlight_control_003_t *dst_p = static_cast<struct mcu_headlight_control_003_t *>(buf);
    uint8_t data[8] = { 0 };
    mcu_headlight_control_003_pack(data, dst_p, 8);

    std::vector<uint8_t> packet(data, data + 8);
    // printData(packet);
    return packet;
}

std::vector<uint8_t> a_mcu_screen_brightness_adjust_pack(void *buf){
    struct mcu_screen_brightness_adjust *dst_p = static_cast<struct mcu_screen_brightness_adjust *>(buf);
    uint8_t data[8] = { 0 };
    mcu_screen_brightness_adjust_pack(data, dst_p, 8);

    std::vector<uint8_t> packet(data, data + 8);
    // printData(packet);
    return packet;
}

std::vector<uint8_t> a_mcu_appointment_chraging_006_pack(void *buf){
    struct mcu_appointment_chraging_006_t *dst_p = static_cast<struct mcu_appointment_chraging_006_t *>(buf);
    uint8_t data[8] = { 0 };
    mcu_appointment_chraging_006_pack(data, dst_p, 8);
    ALOGE("a_mcu_appointment_chraging_006_pack");
    std::vector<uint8_t> packet(data, data + 8);
    printData(packet);
    return packet;
}

// mcu_fm_channel_adjust_pack
std::vector<uint8_t> a_mcu_fm_channel_adjust_00B_pack(void *buf){
    struct mcu_fm_channel_adjust *dst_p = static_cast<struct mcu_fm_channel_adjust *>(buf);
    uint8_t data[8] = { 0 };
    mcu_fm_channel_adjust_pack(data, dst_p, 8);
    ALOGE("a_mcu_appointment_chraging_006_pack");
    std::vector<uint8_t> packet(data, data + 8);
    printData(packet);
    return packet;
}

// mcu_fm_list_0013_pack
std::vector<uint8_t> a_mcu_fm_list_0013_pack(void *buf){
    struct mcu_fm_list_0013 *dst_p = static_cast<struct mcu_fm_list_0013 *>(buf);
    uint8_t data[8] = { 0 };
    mcu_fm_list_0013_pack(data, dst_p, 8);
    ALOGE("a_mcu_appointment_chraging_006_pack");
    std::vector<uint8_t> packet(data, data + 8);
    printData(packet);
    return packet;
}

//mcu
std::vector<uint8_t> a_mcu_soc_power_status_00d_pack(void *buf){
    struct mcu_soc_power_status *dst_p = static_cast<struct mcu_soc_power_status *>(buf);
    uint8_t data[8] = { 0 };
    mcu_soc_power_status_00d_pack(data, dst_p, 8);

    std::vector<uint8_t> packet(data, data + 8);
    // printData(packet);
    return packet;
}

std::vector<uint8_t> a_muc_volume_00a_pack(void *buf){
    struct muc_volume_00a_t *dst_p = static_cast<struct muc_volume_00a_t *>(buf);
    uint8_t data[8] = { 0 };
    muc_volume_00a_pack(data, dst_p, 8);
    std::vector<uint8_t> packet(data, data + 8);
    // printData(packet);
    return packet;
}

const std::unordered_map<uint16_t, packFunctionPtr> FunctionMap::pack_mcu_ = {
    {0x0001, &a_mcu_info_001_pack},
    //{0x0002, &a_mcu_steering_info_002_pack},
    {0x0003, &a_mcu_headlight_control_003_pack},
    {0x0006, &a_mcu_appointment_chraging_006_pack},
    {0x0009, &a_mcu_screen_brightness_adjust_pack},
    {0x000B, &a_mcu_fm_channel_adjust_00B_pack},
    {0x0013, &a_mcu_fm_list_0013_pack},
    {0x0014, &a_mcu_hearbeat_0014_pack},
    {0x000d, &a_mcu_soc_power_status_00d_pack},
    {0x0020, &a_mcu_rema_disp_status_20_pack},
    {0x00a,  &a_muc_volume_00a_pack},
};

std::vector<uint8_t> FunctionMap::packcallFunction(uint16_t id,void *buf, BusIdentifier busIdentifier) {
    std::vector<uint8_t> ret;
        switch(busIdentifier){
        case BusIdentifier::BODY_CAN:
            {
                auto it = pack_body_can_.find(id);
                if (it != pack_body_can_.end()) {
                    packFunctionPtr function = it->second;
                    return function(buf);
                } else {
                    ALOGE("BODY_CAN pack err, id:%x not found!!", id);
                    return ret;
                }
                break;
            }
        case BusIdentifier::EV_CAN:
            {
                auto it = pack_ev_can_.find(id);
                if (it != pack_ev_can_.end()) {
                    packFunctionPtr function = it->second;
                    return function(buf);
                } else {
                    ALOGE("EV_CAN pack err, id:%x not found!!", id);
                    return ret;
                }
                break;
            }
        case BusIdentifier::CH_CAN:
            {
                auto it = pack_chassise_can_.find(id);
                if (it != pack_chassise_can_.end()) {
                    packFunctionPtr function = it->second;
                    return function(buf);
                } else {
                    ALOGE("CH_CAN pack err, id:%x not found!!", id);
                    return ret;
                }
                break;
            }
        case BusIdentifier::MCU:
            {
                auto it = pack_mcu_.find(id);
                if (it != pack_mcu_.end()) {
                    packFunctionPtr function = it->second;
                    return function(buf);
                } else {
                    ALOGE("MCU pack err, id:%x not found!!", id);
                    return ret;
                }
                break;
            }
        default:  ALOGE("unpackca err, Wrong  BUS id:%d", static_cast<int>(busIdentifier));
    }

    
    return ret;
}

//---------------------解包---------------------
// void a_x(std::vector<uint8_t>& message, void *data){
//     struct xxxxx dst_p;
// 	   xxxxxxxxxxx(&dst_p,message.data(),message.size());
//     memcpy(data, &dst_p, sizeof(xxxxx));
// }

bool can_esc_7_lost = true;
//me11 车速解包
void a_me11_cmx_v01_esc_7_func_status_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        pm.updateCPropertyValue("ec_vcu_vehicle_spd",-1024);
        pm.updateCPropertyValue("ec_esc_tcs_active_sta", 0);
        pm.updateCPropertyValue("ec_esc_esp_active_sta", 0);
        pm.updateCPropertyValue("ec_esc_es_cor_tcs_failed", 0);
        pm.updateCPropertyValue("ec_esc_esc_disable", -1024);
        pm.updateCPropertyValue("cc_abs_abs_fail_status", -1024);
        pm.updateCPropertyValue("ec_vcu_brak_sys_warn", -1024);
        pm.updateCPropertyValue("ec_esc_ebd_failed", -1024);
        pm.updateCPropertyValue("ec_esc_rmi_active_sta", 0);
        LOG(INFO) << "me11_cmx_v01_esc_7_func_status reset done";
        can_esc_7_lost = true;
        return;
    }
    can_esc_7_lost = false;
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_esc_7_func_status_bac_t dst_p;
    std::string value;
	me11_cmx_baccanfd_ots_car_v02_07_20240520_esc_7_func_status_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_esc_7_func_status_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("dxy--speed%s %d %d %d", __func__,__LINE__,dst_p.esc_vehicle_speed,dst_p.esc_esc_disable);
    }
    if(dst_p.esc_vehicle_speed_v) {
        pm.updateCPropertyValue("ec_vcu_vehicle_spd", dst_p.esc_vehicle_speed);
    } else {
        pm.updateCPropertyValue("ec_vcu_vehicle_spd", -1024);
    }
    pm.updateCPropertyValue("ec_esc_esc_disable", dst_p.esc_esc_disable);
    pm.updateCPropertyValue("cc_abs_abs_fail_status",dst_p.esc_abs_failed);
    pm.updateCPropertyValue("ec_vcu_brak_sys_warn", dst_p.esc_hmi_warning_on);
    pm.updateCPropertyValue("ec_esc_ebd_failed", dst_p.esc_ebd_failed);
    pm.updateCPropertyValue("ec_esc_esp_active_sta", dst_p.esc_esp_active_sta);
    pm.updateCPropertyValue("ec_esc_tcs_active_sta", dst_p.esc_tcs_active_sta);
    pm.updateCPropertyValue("ec_esc_es_cor_tcs_failed", dst_p.esc_es_cor_tcs_failed);
/////////////////////////////////////////////////////////////////////////////////////////////////
    pm.updateCPropertyValue("ec_esc_rmi_active_sta", dst_p.esc_rmi_active_sta);
    pm.updateCPropertyValue("ec_esc_slope_percent_v", dst_p.esc_slope_percent_v);
    pm.updateCPropertyValue("ec_esc_slope_percent", dst_p.esc_slope_percent);
    pm.updateCPropertyValue("ec_esc_abs_active_sta", dst_p.esc_abs_active_sta);
    pm.updateCPropertyValue("ec_esc_req_brake_light_on", dst_p.esc_req_brake_light_on);
    pm.updateCPropertyValue("ec_esc_ebd_active_sta", dst_p.esc_ebd_active_sta);
    pm.updateCPropertyValue("ec_esc_ess_active", dst_p.esc_ess_active);
    pm.updateCPropertyValue("ec_esc_cdp_active", dst_p.esc_cdp_active);
}

//me11 实际档位解包
void a_me11_cmx_v01_vcu_6_drive_state_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    if(!is_valid_data(message)) {
        pm.updateCPropertyValue("ec_vcu_actual_gear",16);
        pm.updateCPropertyValue("ec_vcu_creep_sts", -1024);
        pm.updateCPropertyValue("ec_vcu_drive_mode", 0);
        pm.updateCPropertyValue("ec_vcu_12_v_bat_low_vol_sta", -1024);
        pm.updateCPropertyValue("ec_vcu_pt_st", 0);
        pm.updateCPropertyValue("bc_bcm_position_light_status", 0);
        pm.updateCPropertyValue("ec_bcm_back_ladjval_fb", -1024);
        pm.updateCPropertyValue("ec_vcu_pcu_fail_sta", -1024);
        pm.updateCPropertyValue("ec_vcu_drive_pwr_lim", -1024);
        pm.updateCPropertyValue("bc_bcm_power_sts", -1024);
        pm.updateCPropertyValue("ec_vcu_hdc_status", 0);
        LOG(INFO) << "me11_cmx_v01_vcu_6_drive_state reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_ccu_veh_info_bac_t dst_p;
    std::string value;
	me11_cmx_baccanfd_ots_car_v02_07_20240520_ccu_veh_info_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_ccu_veh_info_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d %d %d %d", __func__,__LINE__,dst_p.vcu_vhcl_actl_gear_pos_v,dst_p.bcm_sys_pwr_mode,dst_p.vcu_veh_act_drv_mod1,dst_p.vcu_prstls_sta_fb);
    }
    if(dst_p.vcu_vhcl_actl_gear_pos_v) {
        pm.updateCPropertyValue("ec_vcu_actual_gear", dst_p.vcu_vhcl_actl_gear_pos);
    } else {
        pm.updateCPropertyValue("ec_vcu_actual_gear", -1024);
    }
    if(dst_p.bcm_sys_pwr_mode_v) {
        pm.updateCPropertyValue("bc_bcm_power_sts", dst_p.bcm_sys_pwr_mode);
    } else {
        pm.updateCPropertyValue("bc_bcm_power_sts", 0);
    }
    pm.updateCPropertyValue("ec_vcu_drive_mode", dst_p.vcu_veh_act_drv_mod1);
    pm.updateCPropertyValue("ec_vcu_creep_sts", dst_p.vcu_prstls_sta_fb);
    pm.updateCPropertyValue("ec_vcu_pt_st", dst_p.vcu_pt_actl_oprt_mode);
    pm.updateCPropertyValue("ec_vcu_drive_pwr_lim", dst_p.vcu_limp_home_sta);
    ///////////////////////////////////////////////////////////////////Add new prop
    pm.updateCPropertyValue("ec_vcu_12_v_bat_low_vol_sta", dst_p.vcu_12_v_bat_low_vol_sta);
    pm.updateCPropertyValue("ec_vcu_hdc_status", dst_p.vcu_hdc_status);
    pm.updateCPropertyValue("bc_bcm_position_light_status", dst_p.bcm_pos_lamp_sta);       // 左右位置灯
////////////////////////////////////////////////////////////////////////////////////////////
    pm.updateCPropertyValue("ec_vcu_vhcl_actl_gear_pos_v", dst_p.vcu_vhcl_actl_gear_pos_v);
    pm.updateCPropertyValue("ec_bcm_sys_pwr_mode_v", dst_p.bcm_sys_pwr_mode_v);
    pm.updateCPropertyValue("ec_bcm_sys_pwr_mode", dst_p.bcm_sys_pwr_mode);
    pm.updateCPropertyValue("ec_bcm_sys_power_sts", dst_p.bcm_sys_power_sts);
    pm.updateCPropertyValue("ec_bcm_central_lock_sts", dst_p.bcm_central_lock_sts);
    pm.updateCPropertyValue("ec_bcm_hazard_lamp_sts", dst_p.bcm_hazard_lamp_sts);
    pm.updateCPropertyValue("ec_vcu_pcu_fail_sta", dst_p.vcu_pcu_fail_sta);
    pm.updateCPropertyValue("ec_bcm_door_lock_sta_fl", dst_p.bcm_door_lock_sta_fl);
    pm.updateCPropertyValue("ec_vcu_pwr_recup_intensity", dst_p.vcu_pwr_recup_intensity);
    pm.updateCPropertyValue("ec_ccu_diag_tester_online", dst_p.ccu_diag_tester_online);
    pm.updateCPropertyValue("ec_bcm_back_ladjval_fb", dst_p.bcm_back_ladjval_fb);
    pm.updateCPropertyValue("ec_bcm_brake_pedal_sta", dst_p.bcm_brake_pedal_sta);

}

// me11 驻车状态解包
void a_me11_cmx_v01_ehb_a_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        pm.updateCPropertyValue("ec_vcu_park_state", 1);
        pm.updateCPropertyValue("ec_ehb_avh_lamp_req", 1);
        pm.updateCPropertyValue("ec_ehb_epb_warn_lamp_sta", -1024);
        pm.updateCPropertyValue("ec_ehb_brake_liquid_level_warn", -1024);
        pm.updateCPropertyValue("ec_ehb_avh_appld_st", -1024);
        pm.updateCPropertyValue("ec_ehb_avh_disp_msg", -1024);
        pm.updateCPropertyValue("ec_ehb_epb_display_msg_id", -1024);
        pm.updateCPropertyValue("ec_ehb_epb_system_state", -1024);
        LOG(INFO) << "me11_cmx_v01_ehb_a_bac status reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_ehb_a_bac_t dst_p;
    std::string value;
	me11_cmx_baccanfd_ots_car_v02_07_20240520_ehb_a_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_ehb_a_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d %d %d %d", __func__,__LINE__,dst_p.ehb_epb_park_lamp_sta,dst_p.ehb_avh_appld_st,dst_p.ehb_epb_system_state,dst_p.ehb_brk_dis_wip_ac);
    }
    pm.updateCPropertyValue("ec_vcu_park_state", dst_p.ehb_epb_park_lamp_sta);
    pm.updateCPropertyValue("ec_ehb_avh_appld_st", dst_p.ehb_avh_appld_st);
    pm.updateCPropertyValue("ec_ehb_epb_system_state", dst_p.ehb_epb_system_state);
    pm.updateCPropertyValue("ec_ehb_brk_dis_wip_ac", dst_p.ehb_brk_dis_wip_ac);
    pm.updateCPropertyValue("ec_ehb_brake_liquid_level_warn", dst_p.ehb_brake_liquid_level_warn);
    pm.updateCPropertyValue("ec_ehb_epb_warn_lamp_sta", dst_p.ehb_epb_warn_lamp_sta);
    pm.updateCPropertyValue("ec_ehb_avh_disp_msg", dst_p.ehb_avh_disp_msg);
    pm.updateCPropertyValue("ec_ehb_epb_display_msg_id", dst_p.ehb_epb_display_msg_id);
    pm.updateCPropertyValue("ec_ehb_avh_lamp_req", dst_p.ehb_avh_lamp_req);
    pm.updateCPropertyValue("bc_bcm_brakelight_st", dst_p.ehb_epb_park_lamp_sta);                  //驻车灯
    //pm.updateCPropertyValue("ec_ehb_avh_lamp_req", dst_p.ehb_avh_lamp_req);
///////////////////////////////////////////////////////////////////////////////////////////////////
    pm.updateCPropertyValue("ec_ehb_epb_flt_sts", dst_p.ehb_epb_flt_sts);
    pm.updateCPropertyValue("ec_ehb_epb_switch_sts", dst_p.ehb_epb_switch_sts);
    pm.updateCPropertyValue("ec_ehb_failure_sts", dst_p.ehb_failure_sts);
    pm.updateCPropertyValue("ec_ehb_epb_actuator_st_l", dst_p.ehb_epb_actuator_st_l);
    pm.updateCPropertyValue("ec_ehb_hhc_active_sta", dst_p.ehb_hhc_active_sta);
    pm.updateCPropertyValue("ec_ehb_rgn_brk_st", dst_p.ehb_rgn_brk_st);
    pm.updateCPropertyValue("ec_ehb_ext_req_status", dst_p.ehb_ext_req_status);
    pm.updateCPropertyValue("ec_ehb_bla", dst_p.ehb_bla);
    pm.updateCPropertyValue("ec_ehb_hmi_warning_on", dst_p.ehb_hmi_warning_on);

}

void a_me11_cmx_v01_bcm_b_package_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        pm.updateCPropertyValue("bc_bcm_fl_door_ajar_status", 0);  //左前门
        pm.updateCPropertyValue("bc_bcm_fr_door_ajar_status", 0);  //右前门
        pm.updateCPropertyValue("bc_bcm_rr_door_ajar_status", 0);  //隔板门
        pm.updateCPropertyValue("bc_bcm_tr_cls_st", 0);              //后背门
        //pm.updateCPropertyValue("bc_bcm_turn_indcr_le",         0);  //左转向灯
        //pm.updateCPropertyValue("bc_bcm_turn_indcr_ri",         0);  //右转向灯
        pm.updateCPropertyValue("bc_bcm_low_beam_status",       0);  //近光灯
        pm.updateCPropertyValue("bc_bcm_high_beam_status",      0);  //远光灯
        pm.updateCPropertyValue("bc_bcm_rear_fog_light_status", 0);  //后雾灯
        pm.updateCPropertyValue("bc_bcm_brakelight_st",       0);    //驻车灯
        //pm.updateCPropertyValue("bc_bcm_hazard_light_status", 0);    //危险指示灯
        pm.updateCPropertyValue("bc_bcm_front_fog_light_status",0);
        pm.updateCPropertyValue("bc_bcm_warn_key_out_reminder", 0);
        pm.updateCPropertyValue("bc_tmps_abnm_prsr_warn", -1024);
        pm.updateCPropertyValue("bc_bcm_wipr_intl_time_set_fb", -1024);
        pm.updateCPropertyValue("bc_bcm_flwr_me_hm_ctrl_fb", -1024);
        pm.updateCPropertyValue("bc_bcm_mai_drvr_seat_sts", -1024); 
        pm.updateCPropertyValue("bc_bcm_steer_whl_heat_sts", -1024);
        pm.updateCPropertyValue("bc_bcm_park_auto_unlock_set_fb", -1024);
        pm.updateCPropertyValue("bc_bcm_lock_horn_on_st_fb", -1024);
        pm.updateCPropertyValue("bc_bcm_warn_no_key_found", 0);
        pm.updateCPropertyValue("bc_bcm_key_lo_batt_lvl_warn", 0);
        pm.updateCPropertyValue("bc_bcm_indication_key_closer", 0);
        pm.updateCPropertyValue("bc_bcm_indication_shiftto_neutral", 0);
        pm.updateCPropertyValue("bc_bcm_park_un_lock_ack", -1024);
        pm.updateCPropertyValue("bc_bcm_avalueble_key_in_car", 0);
        pm.updateCPropertyValue("bc_bcm_auto_lamp_status", 0);
        pm.updateCPropertyValue("ec_bcm_brake_lamp_sta", 0);
        pm.updateCPropertyValue("bc_bcm_carriage_light_sta", -1024);
        LOG(INFO) << "me11_cmx_v01_bcm_b_package status reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bcm_b_package_bac_t dst_p;
    std::string value;
    me11_cmx_baccanfd_ots_car_v02_07_20240520_bcm_b_package_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bcm_b_package_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d %d %d %d %d %d", __func__,__LINE__,dst_p.bcm_wipr_intl_time_set_fb,dst_p.bcm_flwr_me_hm_ctrl_fb,dst_p.bcm_park_auto_unlock_set_fb,dst_p.bcm_lock_horn_on_st_fb,dst_p.bcm_turn_lamp_sta_left,dst_p.bcm_turn_lamp_sta_right);
    }
    pm.updateCPropertyValue("bc_bcm_wipr_intl_time_set_fb", dst_p.bcm_wipr_intl_time_set_fb);
    //pm.updateCPropertyValue("bc_bcm_chg_wiper_mt_md_sw_set_fb", dst_p.bcm_chg_wiper_mt_md_sw_set_fb);
    pm.updateCPropertyValue("bc_bcm_flwr_me_hm_ctrl_fb", dst_p.bcm_flwr_me_hm_ctrl_fb);
    pm.updateCPropertyValue("bc_bcm_park_un_lock_ack", dst_p.bcm_park_auto_unlock_set_fb);
    pm.updateCPropertyValue("bc_bcm_lock_horn_on_st_fb", dst_p.bcm_lock_horn_on_st_fb);
//    pm.updateCPropertyValue("bc_bcm_wipr_intl_time_set_fb", dst_p.bcm_wipr_intl_time_set_fb);
    //pm.updateCPropertyValue("bc_bcm_turn_indcr_le", dst_p.bcm_turn_lamp_sta_left);  //左转向灯
    //pm.updateCPropertyValue("bc_bcm_turn_indcr_ri", dst_p.bcm_turn_lamp_sta_right);  //右转向灯
    pm.updateCPropertyValue("bc_bcm_fl_door_ajar_status", dst_p.bcm_door_ajar_sta_fl);  //左前门
    pm.updateCPropertyValue("bc_bcm_fr_door_ajar_status", dst_p.bcm_door_ajar_sta_fr);  //右前门
    pm.updateCPropertyValue("bc_bcm_rr_door_ajar_status", dst_p.bcm_door_ajar_sta_rr);  //隔板门
    pm.updateCPropertyValue("bc_bcm_rl_door_ajar_status", dst_p.bcm_door_ajar_sta_rl);
    pm.updateCPropertyValue("bc_bcm_tr_cls_st", dst_p.bcm_ajar_sta_trunk);              //后背门
    pm.updateCPropertyValue("bc_bcm_low_beam_status", dst_p.bcm_low_beam_sta);                   // 近光灯
    pm.updateCPropertyValue("bc_bcm_high_beam_status", dst_p.bcm_high_beam_sta);                 // 远光灯
    pm.updateCPropertyValue("bc_bcm_rear_fog_light_status", dst_p.bcm_fog_lamp_sta_rear);       // 后雾灯
    //pm.updateCPropertyValue("bc_bcm_position_light_status", dst_p.bcm_pos_lamp_sta);       // 左右位置灯
    //pm.updateCPropertyValue("bc_bcm_brakelight_st", dst_p.bcm_brake_lamp_sta);                  //驻车灯
    //pm.updateCPropertyValue("bc_bcm_hazard_light_status", dst_p.bcm_hazard_lamp_sts);                 //危险指示灯
///////////////////////////////////////////////////////////////new add props
    pm.updateCPropertyValue("bc_bcm_front_fog_light_status", dst_p.bcm_fog_lamp_sta_front);       // 前雾灯
    pm.updateCPropertyValue("bc_bcm_key_lo_batt_lvl_warn", dst_p.bcm_key_lo_batt_lvl_warn);       //钥匙电量低警告
    pm.updateCPropertyValue("bc_bcm_warn_no_key_found", dst_p.bcm_warn_no_key_found);             //钥匙未找到警告
    pm.updateCPropertyValue("bc_bcm_auto_lamp_status", dst_p.bcm_auto_lamp_status);               //自动远近光灯状态
    pm.updateCPropertyValue("bc_bcm_indication_shiftto_neutral", dst_p.bcm_indication_shiftto_neutral); //请挂驻车
    pm.updateCPropertyValue("bc_bcm_indication_key_closer", dst_p.bcm_indication_key_closer);
    pm.updateCPropertyValue("bc_bcm_warn_key_out_reminder", dst_p.bcm_warn_key_out_reminder);
    pm.updateCPropertyValue("bc_bcm_light_intensity", dst_p.bcm_light_intensity);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    pm.updateCPropertyValue("bc_bcm_light_sw_sig", dst_p.bcm_light_sw_sig);
    //pm.updateCPropertyValue("bc_bcm_flwr_me_hm_ctrl_fb", dst_p.bcm_flwr_me_hm_ctrl_fb);
    pm.updateCPropertyValue("bc_bcm_ajar_sta_hood", dst_p.bcm_ajar_sta_hood);
    pm.updateCPropertyValue("bc_tmps_abnm_prsr_warn", dst_p.tmps_abnm_prsr_warn);
    pm.updateCPropertyValue("bc_bcm_tpms_reset_sts", dst_p.bcm_tpms_reset_sts);
    pm.updateCPropertyValue("bc_bcm_b_w_iper_window_chksum", dst_p.bcm_b_w_iper_window_chksum);
    pm.updateCPropertyValue("bc_bcm_fr_windowmotor_flags", dst_p.bcm_fr_windowmotor_flags);
    pm.updateCPropertyValue("bc_bcm_fl_windowmotor_flags", dst_p.bcm_fl_windowmotor_flags);
    pm.updateCPropertyValue("bc_bcm_wash_liquid_level_warn", dst_p.bcm_wash_liquid_level_warn);
    pm.updateCPropertyValue("bc_bcm_front_wash_sts", dst_p.bcm_front_wash_sts);
    pm.updateCPropertyValue("bc_bcm_pas_wdw_init", dst_p.bcm_pas_wdw_init);
    pm.updateCPropertyValue("bc_bcm_drv_pass_win_sw_sig", dst_p.bcm_drv_pass_win_sw_sig);
    pm.updateCPropertyValue("bc_bcm_body_warn_sts", dst_p.bcm_body_warn_sts);
    pm.updateCPropertyValue("bc_bcm_key_in_car_sta", dst_p.bcm_key_in_car_sta);
    //pm.updateCPropertyValue("bc_bcm_rke_req", dst_p.bcm_rke_req);
    pm.updateCPropertyValue("bc_bcm_ps_authent_res", dst_p.bcm_ps_authent_res);
    pm.updateCPropertyValue("bc_bcm_trunk_lock_sw_ctrl_fb", dst_p.bcm_trunk_lock_sw_ctrl_fb);
    pm.updateCPropertyValue("bc_bcm_trunk_lock_sta", dst_p.bcm_trunk_lock_sta);
    pm.updateCPropertyValue("bc_bcm_overspd_cntrl_lock_set_fb", dst_p.bcm_overspd_cntrl_lock_set_fb);
    pm.updateCPropertyValue("bc_bcm_park_auto_unlock_set_fb", dst_p.bcm_park_auto_unlock_set_fb);
    pm.updateCPropertyValue("bc_bcm_chg_wiper_mt_md_sw_set_fb", dst_p.bcm_chg_wiper_mt_md_sw_set_fb);
    pm.updateCPropertyValue("bc_drv_lf_ant_diag_status", dst_p.drv_lf_ant_diag_status);
    pm.updateCPropertyValue("bc_bcm_lf_init_sta", dst_p.bcm_lf_init_sta);
    pm.updateCPropertyValue("bc_bcm_door_lock_operation_record", dst_p.bcm_door_lock_operation_record);
    pm.updateCPropertyValue("bc_int_lf_ant_diag_status", dst_p.int_lf_ant_diag_status);
    pm.updateCPropertyValue("bc_pass_lf_ant_diag_status", dst_p.pass_lf_ant_diag_status);
    pm.updateCPropertyValue("bc_bcm_key_lo_batt_lvl_warn", dst_p.bcm_key_lo_batt_lvl_warn);
    pm.updateCPropertyValue("bc_bcm_illmnd_entry_sts_fb", dst_p.bcm_illmnd_entry_sts_fb);
    //pm.updateCPropertyValue("bc_bcm_illmnd_lock_req", dst_p.bcm_illmnd_lock_req);
    //pm.updateCPropertyValue("bc_bcm_illmnd_lamp_req", dst_p.bcm_illmnd_lamp_req);
    pm.updateCPropertyValue("bc_bcm_steer_whl_temp", dst_p.bcm_steer_whl_temp);
    pm.updateCPropertyValue("bc_bcm_steer_whl_heat_sts", dst_p.bcm_steer_whl_heat_sts);
    pm.updateCPropertyValue("bc_bcm_rear_mirror_heat_sts", dst_p.bcm_rear_mirror_heat_sts);
    pm.updateCPropertyValue("bc_bcm_mai_drvr_seat_temp", dst_p.bcm_mai_drvr_seat_temp);
    pm.updateCPropertyValue("bc_bcm_mai_drvr_seat_sts", dst_p.bcm_mai_drvr_seat_sts);
    pm.updateCPropertyValue("bc_bcm_logo_active_flag", dst_p.bcm_logo_active_flag);
    pm.updateCPropertyValue("bc_bcm_le_turn_li_fault_fb", dst_p.bcm_le_turn_li_fault_fb);
    pm.updateCPropertyValue("bc_bcm_ri_turn_li_fault_fb", dst_p.bcm_ri_turn_li_fault_fb);
    pm.updateCPropertyValue("bc_bcm_rf_init_sta", dst_p.bcm_rf_init_sta);
    pm.updateCPropertyValue("bc_bcm_rear_fog_li_fault_fb", dst_p.bcm_rear_fog_li_fault_fb);
    pm.updateCPropertyValue("bc_bcm_front_fog_li_fault_fb", dst_p.bcm_front_fog_li_fault_fb);
    pm.updateCPropertyValue("bc_bcm_pos_li_fault_fb", dst_p.bcm_pos_li_fault_fb);
    pm.updateCPropertyValue("bc_bcm_back_li_fault_fb", dst_p.bcm_back_li_fault_fb);
    pm.updateCPropertyValue("bc_bcm_brake_li_fault_fb", dst_p.bcm_brake_li_fault_fb);
    pm.updateCPropertyValue("bc_bcm_reverse_li_fault_fb", dst_p.bcm_reverse_li_fault_fb);
    pm.updateCPropertyValue("bc_bcm_day_run_li_fault_fb", dst_p.bcm_day_run_li_fault_fb);
    pm.updateCPropertyValue("bc_bcm_avalueble_key_in_car", dst_p.bcm_avalueble_key_in_car);
    pm.updateCPropertyValue("bc_bcm_carriage_light_sta", dst_p.bcm_carriage_light_sta);
    pm.updateCPropertyValue("bc_bcm_rke_req", dst_p.bcm_rke_req);
    pm.updateCPropertyValue("ec_bcm_high_beam_li_fault_fb", dst_p.bcm_high_beam_li_fault_fb);
    pm.updateCPropertyValue("ec_bcm_low_beam_li_fault_fb", dst_p.bcm_low_beam_li_fault_fb);
    pm.updateCPropertyValue("ec_bcm_vhcl_seek", dst_p.bcm_vhcl_seek);
    pm.updateCPropertyValue("ec_bcm_illmnd_sts", dst_p.bcm_illmnd_sts);
    pm.updateCPropertyValue("ec_bcm_key_frbdn", dst_p.bcm_key_frbdn);
    pm.updateCPropertyValue("ec_bcm_frbdn_key_nr", dst_p.bcm_frbdn_key_nr);
    pm.updateCPropertyValue("ec_bcm_start_req", dst_p.bcm_start_req);
    pm.updateCPropertyValue("ec_bcm_drv_pe_authent_acsd", dst_p.bcm_drv_pe_authent_acsd);
    pm.updateCPropertyValue("ec_bcm_frpe_authent_acsd", dst_p.bcm_frpe_authent_acsd);
    pm.updateCPropertyValue("ec_bcm_ps_req", dst_p.bcm_ps_req);
    pm.updateCPropertyValue("ec_bcm_frnt_le_door_hndl_swt", dst_p.bcm_frnt_le_door_hndl_swt);
    pm.updateCPropertyValue("ec_bcm_frnt_ri_door_hndl_swt", dst_p.bcm_frnt_ri_door_hndl_swt);
    pm.updateCPropertyValue("ec_bcm_save_power_sta", dst_p.bcm_save_power_sta);
    pm.updateCPropertyValue("ec_bcm_front_wiper_motor_sts", dst_p.bcm_front_wiper_motor_sts);
    pm.updateCPropertyValue("ec_bcm_fl_window_sts", dst_p.bcm_fl_window_sts);
    pm.updateCPropertyValue("ec_bcm_fr_window_sts", dst_p.bcm_fr_window_sts);
    pm.updateCPropertyValue("ec_bcm_fl_windowmotor_sts", dst_p.bcm_fl_windowmotor_sts);
    pm.updateCPropertyValue("ec_bcm_fr_windowmotor_sts", dst_p.bcm_fr_windowmotor_sts);
    pm.updateCPropertyValue("ec_bcm_wiper_ctrl_front", dst_p.bcm_wiper_ctrl_front);
    pm.updateCPropertyValue("ec_bcm_drv_seat_occupant_sensor_sts", dst_p.bcm_drv_seat_occupant_sensor_sts);
    pm.updateCPropertyValue("ec_bcm_heavy_rain_ind", dst_p.bcm_heavy_rain_ind);
    pm.updateCPropertyValue("ec_tpms_pressure_value_rr", dst_p.tpms_pressure_value_rr);
    pm.updateCPropertyValue("ec_tpms_pressure_value_rl", dst_p.tpms_pressure_value_rl);
    pm.updateCPropertyValue("ec_tpms_pressure_sta_rl", dst_p.tpms_pressure_sta_rl);
    pm.updateCPropertyValue("ec_tpms_pressure_sta_rr", dst_p.tpms_pressure_sta_rr);
    pm.updateCPropertyValue("ec_tpms_pressure_value_fr", dst_p.tpms_pressure_value_fr);
    pm.updateCPropertyValue("ec_tpms_pressure_value_fl", dst_p.tpms_pressure_value_fl);
    pm.updateCPropertyValue("ec_tpms_pressure_sta_fl", dst_p.tpms_pressure_sta_fl);
    pm.updateCPropertyValue("ec_tpms_pressure_sta_fr", dst_p.tpms_pressure_sta_fr);
    pm.updateCPropertyValue("ec_bcm_dome_light_sta", dst_p.bcm_dome_light_sta);
    pm.updateCPropertyValue("ec_bcm_back_lamp_sta", dst_p.bcm_back_lamp_sta);
    pm.updateCPropertyValue("ec_bcm_bass_horn_req", dst_p.bcm_bass_horn_req);
    pm.updateCPropertyValue("ec_bcm_ajar_sta_v_hood", dst_p.bcm_ajar_sta_v_hood);
    pm.updateCPropertyValue("ec_bcm_frnt_fog_li_sw_sig", dst_p.bcm_frnt_fog_li_sw_sig);
    pm.updateCPropertyValue("ec_bcm_rear_fog_li_sw_sig", dst_p.bcm_rear_fog_li_sw_sig);
    pm.updateCPropertyValue("ec_bcm_day_run_light_sta", dst_p.bcm_day_run_light_sta);
    pm.updateCPropertyValue("ec_bcm_reverse_lamp_sta", dst_p.bcm_reverse_lamp_sta);
    pm.updateCPropertyValue("ec_bcm_brake_lamp_sta", dst_p.bcm_brake_lamp_sta);
    pm.updateCPropertyValue("ec_bcm_turn_li_sts", dst_p.bcm_turn_li_sts);
    pm.updateCPropertyValue("ec_bcm_authent_key_nr", dst_p.bcm_authent_key_nr);
    pm.updateCPropertyValue("ec_bcm_key_sta", dst_p.bcm_key_sta);
    pm.updateCPropertyValue("ec_bcm_wiper_stop_pos_sta", dst_p.bcm_wiper_stop_pos_sta);
}

// void a_me11_cmx_v01_vcu_info_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
//     struct me11_cmx_v01_vcu_info_t dst_p;
//     std::string value;
// 	me11_cmx_v01_vcu_info_unpack(&dst_p,message.data(),message.size());
//     memcpy(data, &dst_p, sizeof(me11_cmx_v01_vcu_info_t));
//     ALOGI("%s %d %d", __func__,__LINE__,dst_p.vcu_wltp_drvg_rng_dst);
//     pm.updateCPropertyValueNoCheck("ec_vcu_cltc_rema_range", dst_p.vcu_wltp_drvg_rng_dst);
// }

// me11 剩余电量电池SOC解包
void a_me11_cmx_v01_bms_2_bat_state_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        pm.updateCPropertyValue("ec_bms_packvoltage", -1024);
        //pm.updateCPropertyValue("ec_bms_batterysoc", -1024);
        LOG(INFO) << "me11_cmx_v01_bms_2_bat_state_unpack status reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_2_bat_state_bac_t dst_p;
    std::string value;
	me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_2_bat_state_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_2_bat_state_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d %d", __func__,__LINE__,dst_p.bms_bat_soc_rpt, dst_p.bms_hv_bat_actl_voltage_v);
    }
    pm.updateCPropertyValue("ec_bms_batterysoc", dst_p.bms_bat_soc_rpt);
    if(dst_p.bms_hv_bat_actl_voltage_v) {
        pm.updateCPropertyValue("ec_bms_packvoltage", dst_p.bms_hv_bat_actl_voltage);
    } else {
        pm.updateCPropertyValue("ec_bms_packvoltage", -1024);
    }
    pm.updateCPropertyValue("ec_bms_packvoltage_v", dst_p.bms_hv_bat_actl_voltage_v);
    pm.updateCPropertyValue("ec_bms_bat_soh", dst_p.bms_bat_soh);
}

//void a_me11_cmx_v01_vcu_7_drv_range_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
//    struct me11_cmx_v01_vcu_7_drv_range_t dst_p;
//    std::string value;
//    me11_cmx_v01_vcu_7_drv_range_unpack(&dst_p,message.data(),message.size());
//    memcpy(data, &dst_p, sizeof(me11_cmx_v01_vcu_7_drv_range_t));
//    ALOGI("%s %d %d", __func__,__LINE__,dst_p.vcu_drv_range_dist_est);
//    pm.updateCPropertyValueNoCheck("ec_bms_batterysoc", dst_p.vcu_drv_range_dist_est);
//}

void a_me11_cmx_v01_ac_ac_statuts_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        LOG(INFO) << "me11_cmx_v01_ac_ac_statuts_bac status reset done";
        pm.updateCPropertyValue("bc_ac_havc_state", -1024);
        pm.updateCPropertyValue("bc_ac_left_set_temperature", -1024);
        pm.updateCPropertyValue("bc_ac_blower_level", -1024);
        pm.updateCPropertyValue("bc_ac_a_cstate", -1024);
        pm.updateCPropertyValue("bc_ac_ac_sta_fb", -1024);
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_ac_ac_statuts_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_ac_ac_statuts_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_ac_ac_statuts_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d %d %d %d %d %d %d", __func__,__LINE__,dst_p.ac_ac_power_fb,dst_p.ac_manual_defrost_fb,dst_p.ac_air_inlet_mode_sta,dst_p.ac_ac_auto_fb,dst_p.ac_left_temp_value, dst_p.ac_front_fan_spd_value, dst_p.ac_hybrid_eco_mode_sta);
    }
    pm.updateCPropertyValue("bc_ac_havc_state", dst_p.ac_ac_power_fb);
    pm.updateCPropertyValue("bc_ac_front_defrost_state", dst_p.ac_manual_defrost_fb);
    //pm.updateCPropertyValue("bc_ac_heat_state", dst_p.ac_heat_state);
    pm.updateCPropertyValue("bc_ac_fresh_reci_state", dst_p.ac_air_inlet_mode_sta);
    pm.updateCPropertyValue("bc_ac_a_cstate", dst_p.ac_ac_auto_fb);
    if(!dst_p.ac_ac_auto_fb) {
        pm.updateCPropertyValue("bc_ac_ai_rdistribution_mode", dst_p.ac_blower_mode_sta);
    } else {
        pm.updateCPropertyValue("bc_ac_ai_rdistribution_mode", dst_p.ac_auto_blower_mode_sta);
    }
    pm.updateCPropertyValue("bc_ac_left_set_temperature", dst_p.ac_left_temp_value);
    pm.updateCPropertyValue("bc_ac_blower_level", dst_p.ac_front_fan_spd_value);
    pm.updateCPropertyValue("bc_ac_hybrid_eco_mode_sta", dst_p.ac_hybrid_eco_mode_sta);
    pm.updateCPropertyValue("bc_ac_auto_blower_mode_sta", dst_p.ac_auto_blower_mode_sta);
    pm.updateCPropertyValue("ec_bcm_rear_view_mirr_heating_sts", dst_p.bcm_rear_view_mirr_heating_sts);
    pm.updateCPropertyValue("bc_ac_ac_sta_fb", dst_p.ac_ac_sta_fb);
}

void a_me11_cmx_v01_ac_3_state_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        pm.updateCPropertyValue("bc_ac_envir_temp_v", 0);
        pm.updateCPropertyValue("bc_ac_envir_temp", -1024);
        LOG(INFO) << "me11_cmx_v01_ac_3_state_bac status reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_ac_3_state_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_ac_3_state_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_ac_3_state_bac_t));
    pm.updateCPropertyValue("bc_ac_envir_temp_v", dst_p.ac_envir_temp_v);
    if (dst_p.ac_envir_temp_v) {
        pm.updateCPropertyValue("bc_ac_envir_temp", dst_p.ac_envir_temp);
    } else {
        pm.updateCPropertyValue("bc_ac_envir_temp", -1024);
    }
////////////////////////////////////////////////////////////////////////////////////////////////
    pm.updateCPropertyValue("bc_ac_extd_defrost_actv_req", dst_p.ac_extd_defrost_actv_req);
    pm.updateCPropertyValue("bc_ac_indoor_temp", dst_p.ac_indoor_temp);
    pm.updateCPropertyValue("ec_tms_ohx_out_tmp", dst_p.tms_ohx_out_tmp);
    pm.updateCPropertyValue("ec_tms_evap_tmp", dst_p.tms_evap_tmp);
    pm.updateCPropertyValue("ec_tms_evap_out_tmp", dst_p.tms_evap_out_tmp);
    pm.updateCPropertyValue("ec_tms_chiller_out_tmp", dst_p.tms_chiller_out_tmp);
}

void a_me11_cmx_v01_vcu_disp_info_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        pm.updateCPropertyValue("ec_vcu_average_energy_consumption",-1024);
        pm.updateCPropertyValue("ec_vcu_instant_energy_consumption",-1024);
        pm.updateCPropertyValue("ec_vcu_motor_high_temp_wrning", -1024);
        pm.updateCPropertyValue("ec_vcu_mcu_high_temp_wrning", -1024);
        pm.updateCPropertyValue("ec_vcu_vhcl_p_gear_error", -1024);
        pm.updateCPropertyValue("bc_bcm_illmnd_unlock_sts_fb", -1024);
        pm.updateCPropertyValue("bc_bcm_illmnd_lock_sts_fb", -1024);
        pm.updateCPropertyValue("bc_ac_ac_unlock_vent_set_fb", -1024);
        pm.updateCPropertyValue("bc_vcu_ac_chrg_max_curr", -1024);
        pm.updateCPropertyValue("ec_vcu_mcu_err", -1024);
        pm.updateCPropertyValue("ec_vcu_motor_actual_power_type", -1024);
        pm.updateCPropertyValue("bc_vcu_info_disp", 0);
        pm.updateCPropertyValue("ec_vcu_sibs_kl30_voltage", -1024);
        pm.updateCPropertyValue("ec_vcu_obc_oprt_cmd_to_icu", 0);
        pm.updateCPropertyValue("bc_vcu_sibs_kl30_voltage", -1024);
        pm.updateCPropertyValue("ec_vcu_chraging_mode_sp", -1024);
        LOG(INFO) << "me11_cmx_v01_vcu_disp_info status reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_vcu_disp_info_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_vcu_disp_info_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_vcu_disp_info_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d %d %d %d", __func__,__LINE__,dst_p.bcm_illmnd_unlock_sts_fb,dst_p.vcu_veh_act_recuperate_pwr,dst_p.vcu_veh_act_driving_pwr,dst_p.vcu_ac_chrg_mode);
    }
    pm.updateCPropertyValue("bc_bcm_illmnd_unlock_sts_fb", dst_p.bcm_illmnd_unlock_sts_fb);
    pm.updateCPropertyValue("bc_veh_act_recuperate_pwr", dst_p.vcu_veh_act_recuperate_pwr);
    pm.updateCPropertyValue("ec_vehicle_driving_pwr", dst_p.vcu_veh_act_driving_pwr);
    pm.updateCPropertyValue("ec_vcu_average_energy_consumption", dst_p.vcu_vhcl_pwr_cnsmp_avrg); // 平均电耗
    LOG(INFO)<<"ec_vcu_average_energy_consumption:"<<dst_p.vcu_vhcl_pwr_cnsmp_avrg;
    pm.updateCPropertyValue("ec_vcu_instant_energy_consumption", dst_p.vcu_vhcl_actl_cnsm_pwr); // 瞬时电耗
    pm.updateCPropertyValue("ec_vcu_mcu_err", dst_p.vcu_drv_motor_sta);
    pm.updateCPropertyValue("ec_mcu_igbt_overheat_warning", dst_p.vcu_mcu_high_temp_wrning);
    pm.updateCPropertyValue("ec_mcu_motor_overheat_warning", dst_p.vcu_motor_high_temp_wrning);
    /////////////////////////////////////////////
    pm.updateCPropertyValue("ec_vcu_cltc_rema_range", dst_p.vcu_drv_range_dist_est);
    pm.updateCPropertyValue("ec_vcu_actual_rema_range", dst_p.vcu_wltp_drvg_rng_dst);
    pm.updateCPropertyValue("ec_vcu_cltc_rema_range_vd", dst_p.vcu_drv_range_dist_est_v);
    pm.updateCPropertyValue("ec_vcu_actual_rema_range_vd", dst_p.vcu_wltp_drvg_rng_dst_vldty);
    pm.updateCPropertyValue("ec_vcu_sibs_kl30_voltage", dst_p.vcu_sibs_kl30_voltage);
    pm.updateCPropertyValue("ec_vcu_motor_actual_power_type", dst_p.vcu_motor_actual_power_type);
    pm.updateCPropertyValue("ec_vcu_motor_actual_power", dst_p.vcu_motor_actual_power);
    pm.updateCPropertyValue("ec_vcu_reserve_power", dst_p.vcu_reserve_power);
    pm.updateCPropertyValue("ec_vcu_obc_oprt_cmd_to_icu", dst_p.vcu_obc_oprt_cmd_to_icu);
    pm.updateCPropertyValue("ec_vcu_motor_high_temp_wrning", dst_p.vcu_motor_high_temp_wrning);
    pm.updateCPropertyValue("ec_vcu_mcu_high_temp_wrning", dst_p.vcu_mcu_high_temp_wrning);
    pm.updateCPropertyValue("ec_vcu_vhcl_p_gear_error", dst_p.vcu_vhcl_p_gear_error);
    pm.updateCPropertyValue("bc_bcm_illmnd_lock_sts_fb", dst_p.bcm_illmnd_lock_sts_fb);
    pm.updateCPropertyValue("bc_ccu_remote_car_search", dst_p.ccu_remote_car_search_fb);
    pm.updateCPropertyValue("bc_ccu_remote_lock", dst_p.ccu_remote_lock_fb);
/////////////////////////////////////////////////////////////////////////////////////////////////////////
    pm.updateCPropertyValue("bc_vcu_info_disp", dst_p.vcu_info_disp);
    pm.updateCPropertyValue("bc_vcu_vhcl_speed_fast_warn", dst_p.vcu_vhcl_speed_fast_warn);
    pm.updateCPropertyValue("bc_vcu_drive_mode_key_stagnation", dst_p.vcu_drive_mode_key_stagnation);
    pm.updateCPropertyValue("bc_vcu_cc_key_stagnation", dst_p.vcu_cc_key_stagnation);
    pm.updateCPropertyValue("bc_vcu_shift_key_stagnation", dst_p.vcu_shift_key_stagnation);
    pm.updateCPropertyValue("bc_vcu_lock_veh_warm", dst_p.vcu_lock_veh_warm);
    pm.updateCPropertyValue("bc_vcu_sibs_kl30_voltage", dst_p.vcu_sibs_kl30_voltage);
    pm.updateCPropertyValue("bc_vcu_sibs_rtcwu_chg_num", dst_p.vcu_sibs_rtcwu_chg_num);
    pm.updateCPropertyValue("bc_vcu_sibs_chg_num_allow", dst_p.vcu_sibs_chg_num_allow);
    pm.updateCPropertyValue("bc_vcu_sibs_rtcwu_chg_fail_num", dst_p.vcu_sibs_rtcwu_chg_fail_num);
    pm.updateCPropertyValue("bc_vcu_sibs_rtc_chrg_st", dst_p.vcu_sibs_rtc_chrg_st);
    pm.updateCPropertyValue("bc_vcu_sibs_wup_volt_allow", dst_p.vcu_sibs_wup_volt_allow);
    pm.updateCPropertyValue("bc_vcu_sibs_bms_bat_soc_allow", dst_p.vcu_sibs_bms_bat_soc_allow);
    pm.updateCPropertyValue("bc_vcu_sibs_fb_rtc_wup_flg", dst_p.vcu_sibs_fb_rtc_wup_flg);
    pm.updateCPropertyValue("bc_vcu_ac_chrg_max_curr", dst_p.vcu_ac_chrg_max_curr);
    pm.updateCPropertyValue("bc_vcu_ac_chrg_mode", dst_p.vcu_ac_chrg_mode);
    pm.updateCPropertyValue("bc_ccu_open_ventilation_fb", dst_p.ccu_open_ventilation_fb);
    pm.updateCPropertyValue("bc_ac_ac_unlock_vent_set_fb", dst_p.ac_ac_unlock_vent_set_fb);
    pm.updateCPropertyValue("bc_ac_remote_defrost_ctl_fb", dst_p.ac_remote_defrost_ctl_fb);
    pm.updateCPropertyValue("bc_ac_temp_set_fb", dst_p.ac_temp_set_fb);
    pm.updateCPropertyValue("bc_ac_remt_ctrl_fb", dst_p.ac_remt_ctrl_fb);
    pm.updateCPropertyValue("bc_ccu_remote_power_lock_fb", dst_p.ccu_remote_power_lock_fb);
    pm.updateCPropertyValue("bc_vcu_speed_limit_level_fb", dst_p.vcu_speed_limit_level_fb);
    pm.updateCPropertyValue("bc_vcu_speed_limit_level_set_fb", dst_p.vcu_speed_limit_level_set_fb);
    pm.updateCPropertyValue("bc_vcu_operat_licence_fb", dst_p.vcu_operat_licence_fb);
    pm.updateCPropertyValue("bc_vcu_version_fb", dst_p.vcu_version_fb);
    pm.updateCPropertyValue("bc_vcu_vesion_set_fb", dst_p.vcu_vesion_set_fb);
    pm.updateCPropertyValue("bc_vcu_wgh_confdence", dst_p.vcu_wgh_confdence);
    pm.updateCPropertyValue("bc_vcu_wgh_est", dst_p.vcu_wgh_est);
    pm.updateCPropertyValue("bc_vcu_wgh_distb", dst_p.vcu_wgh_distb);
    pm.updateCPropertyValue("bc_vcu_cruise_ctrl_sta_for_bac", dst_p.vcu_cruise_ctrl_sta_for_bac);
    pm.updateCPropertyValue("bc_vcu_cruise_ctr_tgt_spd_for_bac", dst_p.vcu_cruise_ctr_tgt_spd_for_bac);
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    pm.updateCPropertyValue("ec_ccu_remt_steer_whl_heat_fb", dst_p.ccu_remt_steer_whl_heat_fb);
    pm.updateCPropertyValue("ec_ccu_remt_power_ctrl_fb", dst_p.ccu_remt_power_ctrl_fb);
    pm.updateCPropertyValue("ec_ccu_remt_mai_dr_seat_heat_fb", dst_p.ccu_remt_mai_dr_seat_heat_fb);

    pm.updateCPropertyValue("ec_vcu_remt_ac_ctrl_st", dst_p.vcu_remt_ac_ctrl_st);
    pm.updateCPropertyValue("ec_vcu_remt_ac_defrostl_req", dst_p.vcu_remt_ac_defrostl_req);
    pm.updateCPropertyValue("ec_vcu_remt_ac_ctrl_req", dst_p.vcu_remt_ac_ctrl_req);
    pm.updateCPropertyValue("ec_vcu_charging_conditions_fb", dst_p.vcu_charging_conditions_fb);
    pm.updateCPropertyValue("ec_vcu_chraging_mode_sp", dst_p.vcu_ac_chrg_mode);
}


void a_me11_cmx_v01_eps_1_status_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        LOG(INFO) << "me11_cmx_v01_eps_1_status status reset done";
        pm.updateCPropertyValue("cc_eps_run_state", -1024);
        pm.updateCPropertyValue("ec_elec_steer_mode_sta", -1024);
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_eps_1_status_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_eps_1_status_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_eps_1_status_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__,dst_p.eps_elec_steer_mode_sta);
    }
    pm.updateCPropertyValue("ec_elec_steer_mode_sta", dst_p.eps_elec_steer_mode_sta);
    pm.updateCPropertyValue("cc_eps_run_state", dst_p.eps_fault_lvl);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    pm.updateCPropertyValue("cc_eps_basic_elec_steer_failed", dst_p.eps_basic_elec_steer_failed);
    pm.updateCPropertyValue("cc_eps_mode_set_inhibit_fb", dst_p.eps_mode_set_inhibit_fb);
    pm.updateCPropertyValue("cc_eps_sws_hod_hands_off_sts", dst_p.eps_sws_hod_hands_off_sts);
    pm.updateCPropertyValue("ec_eps_torsion_bar_torque", dst_p.eps_torsion_bar_torque);
    pm.updateCPropertyValue("ec_eps_torsion_bar_torque_dir", dst_p.eps_torsion_bar_torque_dir);
    pm.updateCPropertyValue("ec_eps_torsion_bar_torque_valid", dst_p.eps_torsion_bar_torque_valid);
    pm.updateCPropertyValue("ec_eps_elect_power_consumption", dst_p.eps_elect_power_consumption);
    pm.updateCPropertyValue("ec_eps_advanced_mode_status", dst_p.eps_advanced_mode_status);
}

void a_me11_cmx_v01_icu_2_odo_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        LOG(INFO) << "me11_cmx_v01_icu_2_odo status reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_icu_2_odo_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_icu_2_odo_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_icu_2_odo_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__,dst_p.icu_icu_total_odometer);
    }
    pm.updateCPropertyValue("ec_total_odometer", dst_p.icu_icu_total_odometer);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    pm.updateCPropertyValue("ec_icu_icu_trip_a_odometer", dst_p.icu_icu_trip_a_odometer);
    pm.updateCPropertyValue("ec_icu_icu_trip_b_odometer", dst_p.icu_icu_trip_b_odometer);
}

void a_me11_cmx_v01_icu_info_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        LOG(INFO) << "me11_cmx_v01_icu_info_bac status reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_icu_info_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_icu_info_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_icu_info_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d %d", __func__,__LINE__,dst_p.icu_vehicle_speed_displayed, dst_p.icu_vehicle_speed_displayed_v);
    }
    if((dst_p.icu_vehicle_speed_displayed_v !=1) || (can_esc_7_lost&&dst_p.icu_vehicle_speed_displayed==0)){
        pm.updateCPropertyValue("sc_ccm_vehicle_spd_dsp", 0xffffffff);
    }else{
        pm.updateCPropertyValue("sc_ccm_vehicle_spd_dsp", dst_p.icu_vehicle_speed_displayed);
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////
    pm.updateCPropertyValue("ec_icu_vehicle_speed_displayed_v", dst_p.icu_vehicle_speed_displayed_v);
}

void a_me11_cmx_v01_bms_10_dc_charge_states_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        pm.updateCPropertyValue("ec_bms_remainingchargetime", -1024);
        pm.updateCPropertyValue("ec_bms_dcs_inlet_connect_sta", 0);
        pm.updateCPropertyValue("ec_bms_dcs_act_oprt_mode", 0);
        LOG(INFO) << "me11_cmx_v01_bms_10_dc_charge_states_bac status reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_10_dc_charge_states_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_10_dc_charge_states_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_10_dc_charge_states_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d %d", __func__,__LINE__,dst_p.bms_dc_ac_rem_chrg_time, dst_p.bms_dcs_actl_chrg_voltage);
    }
    pm.updateCPropertyValue("ec_bms_remainingchargetime", dst_p.bms_dc_ac_rem_chrg_time);
    /////////////////New add prop
    pm.updateCPropertyValue("ec_bms_dcs_inlet_connect_sta", dst_p.bms_dcs_inlet_connect_sta);
    pm.updateCPropertyValue("ec_bms_dcs_act_oprt_mode", dst_p.bms_dcs_act_oprt_mode);
////////////////////////////////////////////////////////////////////////////////////////////////////
    pm.updateCPropertyValue("ec_bms_dcs_actl_chrg_voltage", dst_p.bms_dcs_actl_chrg_voltage);
    pm.updateCPropertyValue("ec_bms_dcs_actl_chrg_current", dst_p.bms_dcs_actl_chrg_current);
    pm.updateCPropertyValue("ec_bms_dcs_actl_chrg_power", dst_p.bms_dcs_actl_chrg_power);
}

void a_me11_cmx_v01_bms_1_main_state_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
   if(!is_valid_data(message)) {
        LOG(INFO) << "me11_cmx_v01_bms_1_main_state_bac status reset done";
        pm.updateCPropertyValue("ec_bms_batterycurrent", -1024);
        pm.updateCPropertyValue("ec_bms_thermalrunaway_wrn", -1024);
        pm.updateCPropertyValue("ec_bms_pwr_train_connect_sta", -1024);
        pm.updateCPropertyValue("ec_vcu_hv_batt_err", -1024);
        pm.updateCPropertyValue("ec_bms_run_state", 0);
        pm.updateCPropertyValue("ec_bms_lowinsres", -1024);
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_1_main_state_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_1_main_state_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_1_main_state_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.bms_sys_fail_sta);
    }
    pm.updateCPropertyValue("ec_bms_batterycurrent", dst_p.bms_actl_exchg_curnt);
    pm.updateCPropertyValue("ec_vcu_hv_batt_err", dst_p.bms_sys_fail_sta);
    pm.updateCPropertyValue("ec_bms_lowinsres", dst_p.bms_insulation_wrning_distgsh);
    pm.updateCPropertyValue("ec_bms_pwr_train_connect_sta", dst_p.bms_pwr_train_connect_sta);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    pm.updateCPropertyValue("ec_bms_insulation_wrning_distgsh", dst_p.bms_insulation_wrning_distgsh);
    pm.updateCPropertyValue("ec_bms_thermalrunaway_wrn", dst_p.bms_thermalrunaway_wrn);    
    pm.updateCPropertyValue("ec_bms_run_state", dst_p.bms_run_state);
    pm.updateCPropertyValue("ec_bms_contactor_state_negative", dst_p.bms_contactor_state_negative);
    pm.updateCPropertyValue("ec_bms_contactor_state_positive", dst_p.bms_contactor_state_positive);
    pm.updateCPropertyValue("ec_bms_contactor_state_precharge", dst_p.bms_contactor_state_precharge);
    pm.updateCPropertyValue("ec_bms_supply_voltage_status", dst_p.bms_supply_voltage_status);
    pm.updateCPropertyValue("ec_bms_emeshutdown_req", dst_p.bms_emeshutdown_req);
    pm.updateCPropertyValue("ec_bms_charge_interlock_sta", dst_p.bms_charge_interlock_sta);
    pm.updateCPropertyValue("ec_bms_dis_charge_interlock_sta", dst_p.bms_dis_charge_interlock_sta);
    pm.updateCPropertyValue("ec_bms_batt_balance_sta", dst_p.bms_batt_balance_sta);
    pm.updateCPropertyValue("ec_bms_therm_runaway_snsr_flt", dst_p.bms_therm_runaway_snsr_flt);
    pm.updateCPropertyValue("ec_bms_therm_runaway_vlv_st", dst_p.bms_therm_runaway_vlv_st);
}

void a_me11_cmx_v01_srs_1_status_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
   if(!is_valid_data(message)) {
        pm.updateCPropertyValue("ec_srs_drv_seatbelt_buckle_valid", 1);
        pm.updateCPropertyValue("mc_main_seat_safety_belt_status", -1024);
        pm.updateCPropertyValue("ec_srs_airbag_warning_lamp_sts", -1024);
        pm.updateCPropertyValue("ec_srs_mid_seat_belt_bucklestatus", -1024);
        pm.updateCPropertyValue("ec_srs_pass_seatbelt_bucklestatus", -1024);
        LOG(INFO) << "me11_cmx_v01_bms_1_main_state_bac status reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_srs_1_status_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_srs_1_status_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_srs_1_status_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__,dst_p.srs_passenger_airbag_sta_rsvr);
    }
    pm.updateCPropertyValue("ec_srs_passenger_airbag_sta_rsvr", dst_p.srs_passenger_airbag_sta_rsvr); //安全气囊故障有效位new add prop
    pm.updateCPropertyValue("ec_srs_airbag_warning_lamp_sts", dst_p.srs_airbag_warning_lamp_sts);//安全气囊故障指示灯
    pm.updateCPropertyValue("ec_srs_pass_seat_occupant_sensor_sts", dst_p.srs_pass_seat_occupant_sensor_sts);
    pm.updateCPropertyValue("ec_srs_pass_seatbelt_buckle_valid", dst_p.srs_pass_seatbelt_buckle_valid);
    pm.updateCPropertyValue("ec_srs_pass_seatbelt_bucklestatus", dst_p.srs_pass_seatbelt_bucklestatus);
    pm.updateCPropertyValue("ec_srs_drv_seatbelt_buckle_valid", dst_p.srs_drv_seatbelt_buckle_valid);
    pm.updateCPropertyValue("mc_main_seat_safety_belt_status", dst_p.srs_drv_seatbelt_bucklestatus);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
    pm.updateCPropertyValue("ec_srs_crash_output_sts", dst_p.srs_crash_output_sts);
    pm.updateCPropertyValue("ec_srs_mid_seat_belt_bucklestatus", dst_p.srs_mid_seat_belt_bucklestatus);
    pm.updateCPropertyValue("ec_srs_mid_seat_belt_buckle_valid", dst_p.srs_mid_seat_belt_buckle_valid);
    pm.updateCPropertyValue("ec_srs_mid_seat_occupant_sensor_sts", dst_p.srs_mid_seat_occupant_sensor_sts);

}


void a_me11_cmx_v01_ipu_obc_1_state_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
   if(!is_valid_data(message)) {
        pm.updateCPropertyValue("ec_ipu_obc_inlet_connect_sta", 0);
        pm.updateCPropertyValue("ec_ipu_obc_actl_oprt_mode", 0);
        pm.updateCPropertyValue("ec_ipu_obc_sys_fail_sta", 0);
        LOG(INFO) << "me11_cmx_v01_ipu_obc_1_state_bac status reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_obc_1_state_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_obc_1_state_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_obc_1_state_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__,dst_p.ipu_obc_inlet_connct_sta);
    }
    pm.updateCPropertyValue("ec_vcu_chrg_cnctr_sts", dst_p.ipu_obc_inlet_connct_sta);
    pm.updateCPropertyValue("ec_ipu_obc_actl_oprt_mode", dst_p.ipu_obc_actl_oprt_mode);
    pm.updateCPropertyValue("ec_ipu_obc_inlet_connect_sta", dst_p.ipu_obc_inlet_connct_sta);
///////////////////////////////////////////////////////////////////////////////////////////////
    pm.updateCPropertyValue("ec_ipu_obc_inlelt_actl_power", dst_p.ipu_obc_inlelt_actl_power);
    pm.updateCPropertyValue("ec_ipu_obc_hvdc_actl_cur", dst_p.ipu_obc_hvdc_actl_cur);
    pm.updateCPropertyValue("ec_ipu_obc_connect_main_sta", dst_p.ipu_obc_connect_main_sta);
    pm.updateCPropertyValue("ec_ipu_obc_request", dst_p.ipu_obc_request);
    pm.updateCPropertyValue("ec_ipu_obc_sys_fail_sta", dst_p.ipu_obc_sys_fail_sta);
    pm.updateCPropertyValue("ec_ipu_obc_hvdc_actl_vol", dst_p.ipu_obc_hvdc_actl_vol);
    pm.updateCPropertyValue("ec_ipu_obc_inlelt_actl_power", dst_p.ipu_obc_inlelt_actl_power);
    pm.updateCPropertyValue("ec_ipu_obc_actl_oprt_mode", dst_p.ipu_obc_actl_oprt_mode);
    pm.updateCPropertyValue("ec_ipu_obc_errbit", dst_p.ipu_obc_errbit);

}

void a_me11_cmx_v02_fcm_b_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
   if(!is_valid_data(message)) {
        pm.updateCPropertyValue("ec_fcm_act_drv_asst_sts", -1024);
        pm.updateCPropertyValue("fcm_elk_sts", -1024);
        LOG(INFO) << "a_me11_cmx_v02_fcm_b_bac_unpack status reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_fcm_b_bac_t dst_p;
    std::string value;
    me11_cmx_baccanfd_ots_car_v02_07_20240520_fcm_b_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_fcm_b_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__,dst_p.fcm_act_drv_asst_sts);
    }
    pm.updateCPropertyValue("fcm_elk_sts", dst_p.fcm_elk_sts); 
    pm.updateCPropertyValue("fcm_aeb_status", dst_p.fcm_aeb_status);
    pm.updateCPropertyValue("ec_fcm_lgt_fct_actv_flg", dst_p.fcm_lgt_fct_actv_flg); 
    pm.updateCPropertyValue("ec_fcm_eps_set_steer_whl_agl", dst_p.fcm_eps_set_steer_whl_agl);
    pm.updateCPropertyValue("ec_fcm_steer_tq_req_active", dst_p.fcm_steer_tq_req_active);
    pm.updateCPropertyValue("ec_fcm_eps_ctrl_req", dst_p.fcm_eps_ctrl_req);
    pm.updateCPropertyValue("ec_fcm_drvr_warn_vib_req", dst_p.fcm_drvr_warn_vib_req);
    pm.updateCPropertyValue("ec_fcm_daeps_mode_req", dst_p.fcm_daeps_mode_req);
    pm.updateCPropertyValue("ec_fcm_act_drv_asst_sts", dst_p.fcm_act_drv_asst_sts);
    pm.updateCPropertyValue("ec_fcm_overtake_ass_sys_sts", dst_p.fcm_overtake_ass_sys_sts);
    pm.updateCPropertyValue("ec_fcm_turn_lamp_req", dst_p.fcm_turn_lamp_req);
    pm.updateCPropertyValue("ec_fcm_tjaica_supps_abortn", dst_p.fcm_tjaica_supps_abortn);
    pm.updateCPropertyValue("ec_fcm_lka_torq_fact_req", dst_p.fcm_lka_torq_fact_req);
    pm.updateCPropertyValue("ec_fcm_hma_supps_abortn", dst_p.fcm_hma_supps_abortn);
    pm.updateCPropertyValue("ec_fcm_lkaldw_supps_abortn", dst_p.fcm_lkaldw_supps_abortn);
    pm.updateCPropertyValue("ec_fcm_flt_sts", dst_p.fcm_flt_sts);
    pm.updateCPropertyValue("ec_fcm_elk_supps_abortn", dst_p.fcm_elk_supps_abortn);
    pm.updateCPropertyValue("ec_fcm_acc_whl_tq_req_val", dst_p.fcm_acc_whl_tq_req_val);
    pm.updateCPropertyValue("ec_fcm_acc_whl_tq_req_actv", dst_p.fcm_acc_whl_tq_req_actv);
    pm.updateCPropertyValue("ec_fcm_acc_whl_dece", dst_p.fcm_acc_whl_dece);
    pm.updateCPropertyValue("ec_fcm_acc_tar_accrn", dst_p.fcm_acc_tar_accrn);
    pm.updateCPropertyValue("ec_fcm_acc_supp_abort", dst_p.fcm_acc_supp_abort);
    pm.updateCPropertyValue("ec_fcm_acc_tar_accrn_req_actv", dst_p.fcm_acc_tar_accrn_req_actv);
    pm.updateCPropertyValue("ec_fcm_epb_req_v", dst_p.fcm_epb_req_v);
    pm.updateCPropertyValue("ec_fcm_epb_req", dst_p.fcm_epb_req);
    pm.updateCPropertyValue("ec_fcm_acc_shutdown_mode", dst_p.fcm_acc_shutdown_mode);
    pm.updateCPropertyValue("ec_fcm_ahd_obj_id", dst_p.fcm_ahd_obj_id);
    pm.updateCPropertyValue("ec_fcm_acc_tgt_ax", dst_p.fcm_acc_tgt_ax);
    pm.updateCPropertyValue("ec_fcm_aeb_tgt_ax", dst_p.fcm_aeb_tgt_ax);
    pm.updateCPropertyValue("ec_fcm_acc_mode", dst_p.fcm_acc_mode);
    pm.updateCPropertyValue("ec_fcm_acc_req_standstill", dst_p.fcm_acc_req_standstill);
    pm.updateCPropertyValue("ec_fcm_aeb_req", dst_p.fcm_aeb_req);
    pm.updateCPropertyValue("ec_fcm_aeb_standstill_sts", dst_p.fcm_aeb_standstill_sts);
    pm.updateCPropertyValue("ec_fcm_aeb_standstill_sts_v", dst_p.fcm_aeb_standstill_sts_v);
    pm.updateCPropertyValue("ec_fcm_aba_level", dst_p.fcm_aba_level);
    pm.updateCPropertyValue("ec_fcm_awb_level", dst_p.fcm_awb_level);
    pm.updateCPropertyValue("ec_fcm_aba_req", dst_p.fcm_aba_req);
    pm.updateCPropertyValue("ec_fcm_eba_req", dst_p.fcm_eba_req);
    pm.updateCPropertyValue("ec_fcm_awb_req", dst_p.fcm_awb_req);
    pm.updateCPropertyValue("ec_fcm_abp_req", dst_p.fcm_abp_req);
    pm.updateCPropertyValue("ec_fcm_acc_tgt_ax_req", dst_p.fcm_acc_tgt_ax_req);
    pm.updateCPropertyValue("ec_fcm_crv_decel_sta", dst_p.fcm_crv_decel_sta);
    pm.updateCPropertyValue("ec_fcm_acc_req_drive_off", dst_p.fcm_acc_req_drive_off);
}

void a_me11_cmx_v01_fcm_a_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
   if(!is_valid_data(message)) {
        pm.updateCPropertyValue("bc_fcm_hma_sts", 0);
        pm.updateCPropertyValue("bc_fcm_lka_sts", -1024);
        pm.updateCPropertyValue("bc_fcm_ldw_set_fb", -1024);
        pm.updateCPropertyValue("bc_fcm_ldw_alert_method_set_fb", -1024);
        pm.updateCPropertyValue("bc_fcm_ica_set_fb", -1024);
        pm.updateCPropertyValue("bc_fcm_ica_truck_set_fb", -1024);
        pm.updateCPropertyValue("bc_fcm_ldw_set_fb", -1024);
        pm.updateCPropertyValue("bc_fcm_lka_set_fb", -1024);
        pm.updateCPropertyValue("bc_fcm_elk_set_fb", -1024);
        pm.updateCPropertyValue("bc_fcm_fcw_set_fb", -1024);
        pm.updateCPropertyValue("bc_fcm_aeb_set_fb", -1024);
        pm.updateCPropertyValue("bc_fcm_slif_set_fb", -1024);
        pm.updateCPropertyValue("bc_fcm_slwf_set_fb", -1024);
        pm.updateCPropertyValue("bc_fcm_acc_mode_hmi", -1024);
        pm.updateCPropertyValue("bc_fcm_act_drv_asst_sts_hmi", -1024);
        pm.updateCPropertyValue("bc_fcm_v_set_dis", -1024);
        pm.updateCPropertyValue("sc_fcm_acc_take_over_req", 0);  // fix bug 9902
        pm.updateCPropertyValue("bc_fcm_pcw_pre_warn", 0); //fix bug 9904
        pm.updateCPropertyValue("bc_fcm_hma_set_fb", -1024); //fix bug 9901
        pm.updateCPropertyValue("bc_fcm_aeb_recreq", 0);  //fix bug 9929
        pm.updateCPropertyValue("bc_fcm_tar_obj_acc", 0);
        pm.updateCPropertyValue("fcm_tjatakeoverreq", 0); //fix bug 9902
        LOG(INFO) << "me11_cmx_v01_fcm_a_bac status reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_fcm_a_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_fcm_a_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_fcm_a_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__,dst_p.fcm_hma_sts);
    }
    pm.updateCPropertyValue("bc_fcm_elk_left_mode", dst_p.fcm_elk_left_mode);
    pm.updateCPropertyValue("bc_fcm_elk_right_mode", dst_p.fcm_elk_right_mode);
    pm.updateCPropertyValue("bc_fcm_fault_text_info", dst_p.fcm_fault_text_info);
    pm.updateCPropertyValue("bc_fcm_le_line_color_fct", dst_p.fcm_le_line_color_fct);
    pm.updateCPropertyValue("bc_fcm_ri_line_color_fct", dst_p.fcm_ri_line_color_fct);
    pm.updateCPropertyValue("bc_fcm_hma_sts", dst_p.fcm_hma_sts);
///////////////////////////////////////////////////////////////////////////////////////////////////////
    pm.updateCPropertyValue("bc_fcm_elk_set_fb", dst_p.fcm_elk_set_fb); 
    pm.updateCPropertyValue("bc_fcm_aeb_set_fb", dst_p.fcm_aeb_set_fb); 
    pm.updateCPropertyValue("bc_fcm_hma_set_fb", dst_p.fcm_hma_set_fb); 
    pm.updateCPropertyValue("bc_fcm_tsr_spd_lim_warn", dst_p.fcm_tsr_spd_lim_warn); 
    pm.updateCPropertyValue("bc_fcm_tsr_spd_lim_warn_au", dst_p.fcm_tsr_spd_lim_warn_au); 
    pm.updateCPropertyValue("bc_fcm_tsr_spd_lim_sts", dst_p.fcm_tsr_spd_lim_sts); 
    pm.updateCPropertyValue("bc_fcm_slwf_set_fb", dst_p.fcm_slwf_set_fb); 
    pm.updateCPropertyValue("bc_fcm_fcm_intialization", dst_p.fcm_fcm_intialization);
    pm.updateCPropertyValue("bc_fcm_slif_set_fb", dst_p.fcm_slif_set_fb); 
    pm.updateCPropertyValue("bc_fcm_trfc_sign", dst_p.fcm_trfc_sign); 
    pm.updateCPropertyValue("bc_fcm_tsr_lgt_dst", dst_p.fcm_tsr_lgt_dst); 
    pm.updateCPropertyValue("bc_fcm_tsr_lat_dst", dst_p.fcm_tsr_lat_dst); 
    pm.updateCPropertyValue("bc_fcm_tsr_set_fb", dst_p.fcm_tsr_set_fb); 
    pm.updateCPropertyValue("bc_fcm_tsr_spd_lim", dst_p.fcm_tsr_spd_lim);  
    pm.updateCPropertyValue("bc_fcm_slif_oper_sts", dst_p.fcm_slif_oper_sts);  
    pm.updateCPropertyValue("bc_fcm_audio_warn_hands_off", dst_p.fcm_audio_warn_hands_off);  
    pm.updateCPropertyValue("bc_fcm_lka_audio_warning", dst_p.fcm_lka_audio_warning);  
    pm.updateCPropertyValue("bc_fcm_act_drv_asst_sts_hmi", dst_p.fcm_act_drv_asst_sts_hmi);  
    pm.updateCPropertyValue("bc_fcm_tar_obj_acc", dst_p.fcm_tar_obj_acc);  
    pm.updateCPropertyValue("bc_fcm_adas_not_avl_reas", dst_p.fcm_adas_not_avl_reas); 
    pm.updateCPropertyValue("bc_fcm_acc_start", dst_p.fcm_acc_start);  
    pm.updateCPropertyValue("bc_fcm_ica_truck_set_fb", dst_p.fcm_ica_truck_set_fb);  
    pm.updateCPropertyValue("bc_fcm_ica_set_fb", dst_p.fcm_ica_set_fb);  
    pm.updateCPropertyValue("bc_fcm_adas_quit_reas", dst_p.fcm_adas_quit_reas);  
    pm.updateCPropertyValue("bc_fcm_acc_mode_hmi", dst_p.fcm_acc_mode_hmi);  
    pm.updateCPropertyValue("bc_fcm_pcw_pre_warn", dst_p.fcm_pcw_pre_warn);  
    pm.updateCPropertyValue("bc_fcm_pcw_latent_warn", dst_p.fcm_pcw_latent_warn);  
    pm.updateCPropertyValue("bc_fcm_text_info_lka1", dst_p.fcm_text_info_lka1);  
    pm.updateCPropertyValue("bc_fcm_ldw_sts", dst_p.fcm_ldw_sts);  
    pm.updateCPropertyValue("bc_fcm_ldw_set_fb", dst_p.fcm_ldw_set_fb);  
    pm.updateCPropertyValue("bc_fcm_lka_sts", dst_p.fcm_lka_sts);  
    pm.updateCPropertyValue("bc_fcm_lka_set_fb", dst_p.fcm_lka_set_fb);  
    pm.updateCPropertyValue("bc_fcm_ldw_alert_method_set_fb", dst_p.fcm_ldw_alert_method_set_fb);  
    pm.updateCPropertyValue("bc_fcm_v_set_dis", dst_p.fcm_v_set_dis);   
    pm.updateCPropertyValue("bc_fcm_aeb_obj_id", dst_p.fcm_aeb_obj_id);
    pm.updateCPropertyValue("bc_fcm_raeb_work_sts", dst_p.fcm_raeb_work_sts);
    pm.updateCPropertyValue("bc_fcm_ldw_audio_warn", dst_p.fcm_ldw_audio_warn);
    pm.updateCPropertyValue("bc_fcm_laterallimit", dst_p.fcm_laterallimit);
    pm.updateCPropertyValue("bc_fcm_fcw_set_fb", dst_p.fcm_fcw_set_fb);
    pm.updateCPropertyValue("sc_fcm_acc_take_over_req", dst_p.fcm_acc_take_over_req);
    pm.updateCPropertyValue("fcm_tjatakeoverreq", dst_p.fcm_tja_takeover_req);
    pm.updateCPropertyValue("bc_fcm_aeb_recreq", dst_p.fcm_aeb_rec_req);
    pm.updateCPropertyValue("fcm_adas_drvr_req", dst_p.fcm_adas_drvr_req);
    pm.updateCPropertyValue("ec_fcm_tau_gap_set", dst_p.fcm_tau_gap_set);
    pm.updateCPropertyValue("ec_fcm_hma_highbeam_req", dst_p.fcm_hma_highbeam_req);
}

int32_t getDistanceRangeFromRule(int32_t dist) {
    int32_t value = -1;
    if (dist == 0) {
        value = 0;
    } else if(dist > 0 && dist <= 40) {
        value = 1;
    } else if(dist > 40 && dist <= 50) {
        value = 2;
    } else if(dist > 50 && dist <= 60) {
        value = 3;
    } else if(dist > 60 && dist <= 80) {
        value = 4;
    } else if(dist > 80 && dist <= 100) {
        value = 5;
    } else if(dist > 100 && dist <= 120) {
        value = 6;
    } else if(dist > 120 && dist <= 150) {
        value = 7;
    } else {
        value = dist;
    }
    ALOGI("%s %d %d %d", __func__,__LINE__, value, dist);
    return value;
}

void a_me11_cmx_v01_pp_inform_and_status_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
   if(!is_valid_data(message)) {
        pm.updateCPropertyValue("bc_pp_rl_dist", -1024);
        pm.updateCPropertyValue("bc_pp_rlm_dist", -1024);
        pm.updateCPropertyValue("bc_pp_rrm_dist", -1024);
        pm.updateCPropertyValue("bc_pp_rr_dist", -1024);
        pm.updateCPropertyValue("bc_pp_sdwfsr_dist", -1024);
        pm.updateCPropertyValue("bc_pp_sdwrsl_dist", -1024);
        pm.updateCPropertyValue("bc_pp_sdwrsl_mid_dist", -1024);
        pm.updateCPropertyValue("bc_pp_sdwrsr_mid_dist", -1024);
        pm.updateCPropertyValue("bc_pdc1_distance_rr", -1024);
        pm.updateCPropertyValue("bc_pp_fl_dist", -1024);
        pm.updateCPropertyValue("bc_pp_fml_dist", -1024);
        pm.updateCPropertyValue("bc_pp_fmr_dist", -1024);
        pm.updateCPropertyValue("bc_pp_fr_dist", -1024);
        pm.updateCPropertyValue("bc_pp_sdwfsl_dist", -1024);
        pm.updateCPropertyValue("bc_pp_sdwfsl_mid_dist", -1024);
        pm.updateCPropertyValue("bc_pp_sdwfsr_mid_dist", -1024);
        pm.updateCPropertyValue("bc_pp_sdwrsr_dist", -1024);
        pm.updateCPropertyValue("bc_pp_sta", -1024);
        pm.updateCPropertyValue("bc_pdc1_mode", -1024);
        LOG(INFO) << "me11_cmx_v01_pp_inform_and_status_bac status reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_pp_inform_and_status_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_pp_inform_and_status_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_pp_inform_and_status_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__,dst_p.pp_sta);
    }
    pm.updateCPropertyValue("bc_pdc1_buzzer_alarm_pattern", dst_p.pp_buzzer_alarm_pattern);
    pm.updateCPropertyValue("bc_pdc1_mode", dst_p.pp_sta);
    pm.updateCPropertyValue("bc_pp_sensor_fault_sts_rl", dst_p.pp_sensor_fault_sts_rl);
    pm.updateCPropertyValue("bc_pp_sensor_fault_sts_rlm", dst_p.pp_sensor_fault_sts_rlm);
    pm.updateCPropertyValue("bc_pp_sensor_fault_sts_rrm", dst_p.pp_sensor_fault_sts_rrm);
    pm.updateCPropertyValue("bc_pp_sensor_fault_sts_rr", dst_p.pp_sensor_fault_sts_rr);
    pm.updateCPropertyValue("bc_pp_rl_dist", dst_p.pp_rl_dist);
    pm.updateCPropertyValue("bc_pp_rlm_dist", dst_p.pp_rlm_dist);
    pm.updateCPropertyValue("bc_pp_rrm_dist", dst_p.pp_rrm_dist);
    pm.updateCPropertyValue("bc_pp_rr_dist", dst_p.pp_rr_dist);
    pm.updateCPropertyValue("bc_pp_sdwfsr_dist", dst_p.pp_sdwfsr_dist);
    pm.updateCPropertyValue("bc_pp_sdwrsl_dist", dst_p.pp_sdwrsl_dist);
    pm.updateCPropertyValue("bc_pp_sdwrsl_mid_dist", dst_p.pp_sdwrsl_mid_dist);
    pm.updateCPropertyValue("bc_pp_sdwrsr_mid_dist", dst_p.pp_sdwrsr_mid_dist);
    pm.updateCPropertyValue("bc_pdc1_distance_rr", dst_p.pp_sdwrsr_dist);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    pm.updateCPropertyValue("bc_pp_fl_dist", dst_p.pp_fl_dist);
    pm.updateCPropertyValue("bc_pp_fml_dist", dst_p.pp_fml_dist);
    pm.updateCPropertyValue("bc_pp_fmr_dist", dst_p.pp_fmr_dist);
    pm.updateCPropertyValue("bc_pp_fr_dist", dst_p.pp_fr_dist);
    pm.updateCPropertyValue("bc_pp_sdwfsl_dist", dst_p.pp_sdwfsl_dist);
    pm.updateCPropertyValue("bc_pp_sdwfsl_mid_dist", dst_p.pp_sdwfsl_mid_dist);
    pm.updateCPropertyValue("bc_pp_sdwfsr_mid_dist", dst_p.pp_sdwfsr_mid_dist);
    pm.updateCPropertyValue("bc_pp_sensor_fault_sts_frm", dst_p.pp_sensor_fault_sts_frm);
    pm.updateCPropertyValue("bc_pp_sensor_fault_sts_fr", dst_p.pp_sensor_fault_sts_fr);
    pm.updateCPropertyValue("bc_pp_sdwrsr_dist", dst_p.pp_sdwrsr_dist);
    pm.updateCPropertyValue("bc_pp_sta", dst_p.pp_sta);
    pm.updateCPropertyValue("bc_pp_sw_fdbk", dst_p.pp_sw_fdbk);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    pm.updateCPropertyValue("ec_pp_raeb_set_fb", dst_p.pp_raeb_set_fb);
    pm.updateCPropertyValue("ec_pp_raeb_work_sts", dst_p.pp_raeb_work_sts);
    pm.updateCPropertyValue("ec_pp_raeb_brkg_req", dst_p.pp_raeb_brkg_req);
    pm.updateCPropertyValue("ec_pp_raeb_brkg_req_val", dst_p.pp_raeb_brkg_req_val);
    pm.updateCPropertyValue("ec_pp_raeb_distance_to_collision", dst_p.pp_raeb_distance_to_collision);
    pm.updateCPropertyValue("ec_pp_sensor_fault_sts_flm", dst_p.pp_sensor_fault_sts_flm);
    pm.updateCPropertyValue("ec_pp_sensor_fault_sts_fl", dst_p.pp_sensor_fault_sts_fl);
}

void a_me11_cmx_v02_bms_a_gb_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    if(!is_valid_data(message)){
        LOG(INFO) << "a_me11_cmx_v02_bms_a_gb_bac_unpack reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_a_gb_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_a_gb_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_a_gb_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__,dst_p.bms_temp_differ_wrning);
    }
    pm.updateCPropertyValue("ec_bms_deltatemp", dst_p.bms_temp_differ_wrning);
    pm.updateCPropertyValue("ec_bms_overtemp", dst_p.bms_high_temp_wrning);
    pm.updateCPropertyValue("ec_bms_packovervolt", dst_p.bms_battery_over_vol_wrning);
    pm.updateCPropertyValue("ec_bms_packudvolt", dst_p.bms_battery_underr_vol_wrning);
    pm.updateCPropertyValue("ec_bms_packlowsoc", dst_p.bms_low_soc_wrning);
    pm.updateCPropertyValue("ec_bms_cellovervolt", dst_p.bms_cell_over_vol_wrning);
    pm.updateCPropertyValue("ec_bms_celludvolt", dst_p.bms_cell_underr_vol_wrning);
    pm.updateCPropertyValue("ec_bms_packhighsoc", dst_p.bms_high_soc_wrning);
    pm.updateCPropertyValue("ec_bms_packfastchgsoc", dst_p.bms_abnormall_soc_wrning);
    pm.updateCPropertyValue("ec_bms_batsysnotmtc", dst_p.bms_batteryt_sys_mismatching_wrning);
    pm.updateCPropertyValue("ec_bms_ucellincon", dst_p.bms_cell_inconsistency_wrning);
    pm.updateCPropertyValue("ec_bms_h_vilfault", dst_p.bms_high_vol_inter_lock_wrning);
    pm.updateCPropertyValue("ec_bms_packovercharge", dst_p.bms_battery_over_chrg_wrning);
    pm.updateCPropertyValue("ec_bms_current_over_wrning", dst_p.bms_current_over_wrning);
    pm.updateCPropertyValue("ec_bms_insulation_wrning",dst_p.bms_insulation_wrning);
}

void a_me11_cmx_v02_bms_b_gb_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_b_gb_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_b_gb_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_b_gb_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.bms_battery_sub_sys_cod);
    }
    pm.updateCPropertyValue("ec_bms_battery_sub_sys_num", dst_p.bms_battery_sub_sys_num);
    pm.updateCPropertyValue("ec_bms_battery_sub_sys_cod", dst_p.bms_battery_sub_sys_cod);
    pm.updateCPropertyValue("ec_bms_battery_dtc_num", dst_p.bms_battery_dtc_num);
    pm.updateCPropertyValue("ec_bms_battery_dtc_list", dst_p.bms_battery_dtc_list);
    pm.updateCPropertyValue("ec_bms_battery_dtc_index", dst_p.bms_battery_dtc_index);
}

void a_me11_cmx_v02_bms_c_gb_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_c_gb_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_c_gb_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_c_gb_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__,dst_p.bms_total_cell_no);
    }
    pm.updateCPropertyValue("ec_bms_packtotalcellnum", dst_p.bms_total_cell_no);
    pm.updateCPropertyValue("ec_b2_v_v_frame_no", dst_p.bms_cell_index);
    pm.updateCPropertyValue("ec_b2_v_v_cell_volt_n1", dst_p.bms_cell_vol0);
    pm.updateCPropertyValue("ec_b2_v_v_cell_volt_n2", dst_p.bms_cell_vol1);
    pm.updateCPropertyValue("ec_b2_v_v_cell_volt_n3", dst_p.bms_cell_vol2);
}

void a_me11_cmx_v02_bms_d_gb_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_d_gb_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_d_gb_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_d_gb_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__,dst_p.bms_total_temp_probe);
    }
    pm.updateCPropertyValue("ec_bms_packtotaltempnum", dst_p.bms_total_temp_probe);
    pm.updateCPropertyValue("ec_b2_v_t_frame_no", dst_p.bms_temp_probe_index);
    pm.updateCPropertyValue("ec_b2_v_t_cell_temp_n1", dst_p.bms_temp_probe0);
    pm.updateCPropertyValue("ec_b2_v_t_cell_temp_n2", dst_p.bms_temp_probe1);
    pm.updateCPropertyValue("ec_b2_v_t_cell_temp_n3", dst_p.bms_temp_probe2);
    pm.updateCPropertyValue("ec_b2_v_t_cell_temp_n4", dst_p.bms_temp_probe3);
    pm.updateCPropertyValue("ec_b2_v_t_cell_temp_n5", dst_p.bms_temp_probe4);
    pm.updateCPropertyValue("ec_b2_v_t_cell_temp_n6", dst_p.bms_temp_probe5);
}

void a_me11_cmx_v02_vcu_b_accl_pedal_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_vcu_b_accl_pedal_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_vcu_b_accl_pedal_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_vcu_b_accl_pedal_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d %d", __func__,__LINE__,dst_p.vcu_err_amount,dst_p.vcu_err_bit);
    }
    pm.updateCPropertyValue("ec_vcu_err_amount", dst_p.vcu_err_amount);
    pm.updateCPropertyValue("ec_vcu_err_bit", dst_p.vcu_err_bit);
    pm.updateCPropertyValue("ec_vcu_sys_fail_sta", dst_p.vcu_sys_fail_sta);
}

void a_me11_cmx_v02_tpms_info_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_tpms_info_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_tpms_info_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_tpms_info_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__,dst_p.tpms_receiverstate);
    }
    // 当前胎温的轮胎号
    pm.updateCPropertyValue("bc_tpms_transducer_location",dst_p.tpms_transducer_location);
    // 当前胎温度
    pm.updateCPropertyValue("bc_tpms_tyretemperature",dst_p.tpms_tyretemperature);
    pm.updateCPropertyValue("bc_tpms_receiverstate",dst_p.tpms_receiverstate); //胎压学习状态

    //胎压故障
    pm.updateCPropertyValue("bc_tpms_tyrestate_fr",dst_p.tpms_tyrestate_fr);
    pm.updateCPropertyValue("bc_tpms_tyrestate_fl",dst_p.tpms_tyrestate_fl);
    pm.updateCPropertyValue("bc_tpms_tyrestate_rl",dst_p.tpms_tyrestate_rl);
    pm.updateCPropertyValue("bc_tpms_tyrestate_rr",dst_p.tpms_tyrestate_rr);

    //胎压
    pm.updateCPropertyValue("bc_tpms_tyrepressure_fl", dst_p.tpms_tyrepressure_fl);
    pm.updateCPropertyValue("bc_tpms_tyrepressure_fr", dst_p.tpms_tyrepressure_fr);
    pm.updateCPropertyValue("bc_tpms_tyrepressure_rl", dst_p.tpms_tyrepressure_rl);
    pm.updateCPropertyValue("bc_tpms_tyrepressure_rr", dst_p.tpms_tyrepressure_rr);
}

void a_me11_cmx_v02_inv_1_value_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_inv_1_value_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_inv_1_value_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_inv_1_value_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.inv_antithft_auth_rslt);
    }
    pm.updateCPropertyValue("ec_inv_antithft_auth_rslt", dst_p.inv_antithft_auth_rslt);
    pm.updateCPropertyValue("ec_inv_1_rolling_counter", dst_p.inv_1_rolling_counter);
    pm.updateCPropertyValue("ec_mcu_motorspd", dst_p.inv_1_mot_speed);
    pm.updateCPropertyValue("ec_mcu_motortorque", dst_p.inv_1_act_torque);
    //pm.updateCPropertyValue("ec_", dst_p.);
}

void a_me11_cmx_v02_inv_2_value_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_inv_2_value_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_inv_2_value_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_inv_2_value_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.inv_2_motor_max_tem);
    }
    pm.updateCPropertyValue("ec_mcu_motortemp", dst_p.inv_2_motor_max_tem);
    pm.updateCPropertyValue("ec_mcu_igbttemp", dst_p.inv_2_igbt_max_tem);
    pm.updateCPropertyValue("ec_mcu_busvolt", dst_p.inv_2_inp_voltage);
    pm.updateCPropertyValue("ec_mcu_buscurrent", dst_p.inv_2_inp_current);
}

void a_me11_cmx_v02_inv_4_value_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_inv_4_value_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_inv_4_value_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_inv_4_value_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.inv_4_rolling_counter);
    }
    pm.updateCPropertyValue("ec_inv_4_err_bits", dst_p.inv_4_err_bits);
    pm.updateCPropertyValue("ec_inv_4_rolling_counter", dst_p.inv_4_rolling_counter);
}

static uint32_t left_1_up = 0;
static uint32_t left_2_up = 0;
static uint32_t left_3_up = 0;
static uint32_t left_4_up = 0;
static uint32_t left_5_up = 0;
static uint32_t left_6_up = 0;
static uint32_t left_7_up = 0;
static uint32_t left_8_up = 0;
static uint32_t right_11_up = 0;
static uint32_t right_12_up = 0;
static uint32_t right_13_up = 0;
static uint32_t right_14_up = 0;
static uint32_t right_15_up = 0;
static uint32_t right_16_up = 0;

static uint32_t left_1_down = 0;
static uint32_t left_2_down = 0;
static uint32_t left_3_down = 0;
static uint32_t left_4_down = 0;
static uint32_t left_5_down = 0;
static uint32_t left_6_down = 0;
static uint32_t left_7_down = 0;
static uint32_t left_8_down = 0;
static uint32_t right_11_down = 0;
static uint32_t right_12_down = 0;
static uint32_t right_13_down = 0;
static uint32_t right_14_down = 0;
static uint32_t right_15_down = 0;
static uint32_t right_16_down = 0;

static const uint8_t disturbCount = 2;

static uint8_t getReversalValue(uint8_t value) {
    if(value == 0){
        return 1;
    }else{
        return 0;
    }
}

void a_me11_cmx_v02_scs_ri_switch_sts_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    if(!is_valid_data(message)){
        pm.updateCPropertyValue("ec_scs_right_11_swt", -1024);
        pm.updateCPropertyValue("ec_scs_right_12_swt", -1024);
        pm.updateCPropertyValue("ec_scs_right_13_swt", -1024);
        pm.updateCPropertyValue("ec_scs_gear_shift_lever_pst_req", -1024);
        LOG(INFO) << "a_me11_cmx_v02_scs_ri_switch_sts_bac_unpack reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_scs_ri_switch_sts_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_scs_ri_switch_sts_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_scs_ri_switch_sts_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.scs_right_16_swt);
    }
    pm.updateCPropertyValue("ec_scs_right_swt_vd", dst_p.scs_right_swt_vd);
    pm.updateCPropertyValue("ec_scs_right_11_swt", dst_p.scs_right_11_swt);
    pm.updateCPropertyValue("ec_scs_right_12_swt", dst_p.scs_right_12_swt);
    pm.updateCPropertyValue("ec_scs_right_13_swt", dst_p.scs_right_13_swt);
    pm.updateCPropertyValue("ec_scs_right_14_swt", dst_p.scs_right_14_swt);
    pm.updateCPropertyValue("ec_scs_right_15_swt", dst_p.scs_right_15_swt);
    pm.updateCPropertyValue("ec_scs_right_16_swt", dst_p.scs_right_16_swt);
    if(dst_p.scs_right_11_swt == 1)   //按下
    {
        right_11_up++;
        if (right_11_up > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_distance", getReversalValue(dst_p.scs_right_11_swt));
        }
        right_11_down = 0;
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac up scs_right_11_swt:%d right_11_up:%d, right_11_down:%d", dst_p.scs_right_11_swt,right_11_up,right_11_down);
        }
    }
    else if(dst_p.scs_right_11_swt == 0 && right_11_up > 0){ //抬起
        right_11_down++;
        if (right_11_down > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_distance", getReversalValue(dst_p.scs_right_11_swt));
            right_11_up = 0;
        }
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac down scs_right_11_swt:%d right_11_up:%d, right_11_down:%d", dst_p.scs_right_11_swt,right_11_up,right_11_down);
        }
    }
    if(dst_p.scs_right_12_swt == 1)   //按下
    {
        right_12_up++;
        if (right_12_up > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_speedup", getReversalValue(dst_p.scs_right_12_swt));
        }
        right_12_down = 0;
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac up scs_right_12_swt:%d right_12_up:%d, right_12_down:%d", dst_p.scs_right_12_swt,right_12_up,right_12_down);
        }
    }
    else if(dst_p.scs_right_12_swt == 0 && right_12_up > 0){ //抬起
        right_12_down++;
        if (right_12_down > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_speedup", getReversalValue(dst_p.scs_right_12_swt));
            right_12_up = 0;
        }
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac down scs_right_12_swt:%d right_12_up:%d, right_12_down:%d", dst_p.scs_right_12_swt,right_12_up,right_12_down);
        }
    }
    if(dst_p.scs_right_13_swt == 1)   //按下
    {
        right_13_up++;
        if (right_13_up > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_speeddown", getReversalValue(dst_p.scs_right_13_swt));
        }
        right_13_down = 0;
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac up scs_right_13_swt:%d right_13_up:%d, right_13_down:%d", dst_p.scs_right_13_swt,right_13_up,right_13_down);
        }
    }
    else if(dst_p.scs_right_13_swt == 0 && right_13_up > 0){ //抬起
        right_13_down++;
        if (right_13_down > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_speeddown", getReversalValue(dst_p.scs_right_13_swt));
            right_13_up = 0;
        }
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac down scs_right_13_swt:%d right_13_up:%d, right_13_down:%d", dst_p.scs_right_13_swt,right_13_up,right_13_down);
        }
    }
    if(dst_p.scs_right_14_swt == 1)   //按下
    {
        right_14_up++;
        if (right_14_up > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_pagedown", getReversalValue(dst_p.scs_right_14_swt));
        }
        right_14_down = 0;
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac up scs_right_14_swt:%d right_14_up:%d, right_14_down:%d", dst_p.scs_right_14_swt,right_16_up,right_14_down);
        }
    }
    else if(dst_p.scs_right_14_swt == 0 && right_14_up > 0){ //抬起
        right_14_down++;
        if (right_14_down > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_pagedown", getReversalValue(dst_p.scs_right_14_swt));
            right_14_up = 0;
        }
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac down scs_right_14_swt:%d right_14_up:%d, right_14_down:%d", dst_p.scs_right_14_swt,right_14_up,right_14_down);
        }
    }
    if(dst_p.scs_right_15_swt == 1)   //按下
    {
        right_15_up++;
        if (right_15_up > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_pageup", getReversalValue(dst_p.scs_right_15_swt));
        }
        right_15_down = 0;
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac up scs_right_15_swt:%d right_15_up:%d, right_15_down:%d", dst_p.scs_right_15_swt,right_15_up,right_15_down);
        }
    }
    else if(dst_p.scs_right_15_swt == 0 && right_15_up > 0){ //抬起
        right_15_down++;
        if (right_15_down > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_pageup", getReversalValue(dst_p.scs_right_15_swt));
            right_15_up = 0;
        }
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac down scs_right_15_swt:%d right_15_up:%d, right_15_down:%d", dst_p.scs_right_15_swt,right_15_up,right_15_down);
        }
    }
    if(dst_p.scs_right_16_swt == 1)   //按下
    {
        right_16_up++;
        if (right_16_up > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_okay", getReversalValue(dst_p.scs_right_16_swt));
        }
        right_16_down = 0;
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac up scs_right_16_swt:%d right_16_up:%d, right_16_down:%d", dst_p.scs_right_16_swt,right_16_up,right_16_down);
        }
    }
    else if(dst_p.scs_right_16_swt == 0 && right_16_up > 0){ //抬起
        right_16_down++;
        if (right_16_down > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_okay", getReversalValue(dst_p.scs_right_16_swt));
            right_16_up = 0;
        }
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac down scs_right_16_swt:%d right_16_up:%d, right_16_down:%d", dst_p.scs_right_16_swt,right_16_up,right_16_down);
        }
    }
    pm.updateCPropertyValue("ec_scs_gear_shift_lever_pst_req", dst_p.scs_gear_shift_lever_pst_req);
    pm.updateCPropertyValue("ec_scs_gear_shift_lever_pst_req_vd", dst_p.scs_gear_shift_lever_pst_req_vd);
}

void a_me11_cmx_v02_crrr_a_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    if(!is_valid_data(message)){
        pm.updateCPropertyValue("ec_crrr_rcw_warning", -1024);
        pm.updateCPropertyValue("ec_crrr_bsd_set_fb", -1024);
        pm.updateCPropertyValue("ec_crrr_dow_set_fb", -1024);
        pm.updateCPropertyValue("ec_crrr_rcw_set_fb", -1024);
        pm.updateCPropertyValue("ec_crrr_rcta_set_fb", -1024);
        LOG(INFO) << "a_me11_cmx_v02_crrr_a_bac_unpack reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_crrr_a_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_crrr_a_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_crrr_a_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.crrr_bsd_warn_right);
    }
    pm.updateCPropertyValue("ec_crrr_rcta_warn_right", dst_p.crrr_rcta_warn_right);
    pm.updateCPropertyValue("ec_crrr_rcta_warn_left", dst_p.crrr_rcta_warn_left);
    pm.updateCPropertyValue("ec_crrr_rear_corner_failure", dst_p.crrr_rear_corner_failure);
    pm.updateCPropertyValue("ec_crrr_bsd_warn_right", dst_p.crrr_bsd_warn_right);
    pm.updateCPropertyValue("ec_crrr_bsd_warn_left", dst_p.crrr_bsd_warn_left);
    pm.updateCPropertyValue("ec_crrr_bsd_set_fb", dst_p.crrr_bsd_set_fb);
    pm.updateCPropertyValue("ec_crrr_rcta_set_fb", dst_p.crrr_rcta_set_fb);
    pm.updateCPropertyValue("ec_crrr_rcw_warning", dst_p.crrr_rcw_warning);
    pm.updateCPropertyValue("ec_crrr_rcw_set_fb", dst_p.crrr_rcw_set_fb);
    pm.updateCPropertyValue("ec_crrr_rear_corner_blindness", dst_p.crrr_rear_corner_blindness);
    pm.updateCPropertyValue("ec_crrr_dow_set_fb", dst_p.crrr_dow_set_fb);
    pm.updateCPropertyValue("ec_crrr_dow_warn_fr", dst_p.crrr_dow_warn_fr);
    pm.updateCPropertyValue("ec_crrr_dow_warn_fl", dst_p.crrr_dow_warn_fl);
    pm.updateCPropertyValue("ec_crrr_dow_warn_rr", dst_p.crrr_dow_warn_rr);
    pm.updateCPropertyValue("ec_crrr_dow_warn_rl", dst_p.crrr_dow_warn_rl);
    pm.updateCPropertyValue("ec_crrr_dow_warn_right", dst_p.crrr_dow_warn_right);
    pm.updateCPropertyValue("ec_crrr_dow_warn_left", dst_p.crrr_dow_warn_left);
    pm.updateCPropertyValue("ec_crrr_rctb_decelaration", dst_p.crrr_rctb_decelaration);
    pm.updateCPropertyValue("ec_crrr_rctb_req", dst_p.crrr_rctb_req);
    pm.updateCPropertyValue("ec_crrr_rctb_brkg_vld", dst_p.crrr_rctb_brkg_vld);
    pm.updateCPropertyValue("ec_crrr_fov_chg_set_fb", dst_p.crrr_fov_chg_set_fb);

}

void a_me11_cmx_v02_fcm_23_obj0109_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    if(!is_valid_data(message)){
        pm.updateCPropertyValue("ec_fcm_obj_typ1", 0);
        LOG(INFO) << "a_me11_cmx_v02_fcm_23_obj0109_bac_unpack reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_fcm_23_obj0109_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_fcm_23_obj0109_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_fcm_23_obj0109_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.fcm_obj_typ1);
    }
    pm.updateCPropertyValue("ec_fcm_obj_typ1", dst_p.fcm_obj_typ1);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst1", dst_p.fcm_obj_lat_dst1);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst1", dst_p.fcm_obj_lgt_dst1);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl1", dst_p.fcm_obj_posn_agl1);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd1", dst_p.fcm_lgt_rel_spd1);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd7", dst_p.fcm_lgt_rel_spd7);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir1", dst_p.fcm_obj_movmt_dir1);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd8", dst_p.fcm_lgt_rel_spd8);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_sta1", dst_p.fcm_obj_movmt_sta1);
    pm.updateCPropertyValue("ec_fcm_obj_typ2", dst_p.fcm_obj_typ2);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst2", dst_p.fcm_obj_lat_dst2);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst2", dst_p.fcm_obj_lgt_dst2);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl2", dst_p.fcm_obj_posn_agl2);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd2", dst_p.fcm_lgt_rel_spd2);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd9", dst_p.fcm_lgt_rel_spd9);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir2", dst_p.fcm_obj_movmt_dir2);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_sta2", dst_p.fcm_obj_movmt_sta2);
    pm.updateCPropertyValue("ec_fcm_obj_typ3", dst_p.fcm_obj_typ3);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst3", dst_p.fcm_obj_lat_dst3);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst3", dst_p.fcm_obj_lgt_dst3);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl3", dst_p.fcm_obj_posn_agl3);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd3", dst_p.fcm_lgt_rel_spd3);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir3", dst_p.fcm_obj_movmt_dir3);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_sta3", dst_p.fcm_obj_movmt_sta3);
    pm.updateCPropertyValue("ec_fcm_obj_typ4", dst_p.fcm_obj_typ4);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst4", dst_p.fcm_obj_lat_dst4);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst4", dst_p.fcm_obj_lgt_dst4);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl4", dst_p.fcm_obj_posn_agl4);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd4", dst_p.fcm_lgt_rel_spd4);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir4", dst_p.fcm_obj_movmt_dir4);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir7", dst_p.fcm_obj_movmt_dir7);
    pm.updateCPropertyValue("ec_fcm_obj_typ5", dst_p.fcm_obj_typ5);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst5", dst_p.fcm_obj_lat_dst5);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst5", dst_p.fcm_obj_lgt_dst5);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl5", dst_p.fcm_obj_posn_agl5);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd5", dst_p.fcm_lgt_rel_spd5);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir5", dst_p.fcm_obj_movmt_dir5);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_sta5", dst_p.fcm_obj_movmt_sta5);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_sta9", dst_p.fcm_obj_movmt_sta9);
    pm.updateCPropertyValue("ec_fcm_obj_typ6", dst_p.fcm_obj_typ6);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst6", dst_p.fcm_obj_lat_dst6);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst6", dst_p.fcm_obj_lgt_dst6);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl6", dst_p.fcm_obj_posn_agl6);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd6", dst_p.fcm_lgt_rel_spd6);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir9", dst_p.fcm_obj_movmt_dir9);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_sta6", dst_p.fcm_obj_movmt_sta6);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir6", dst_p.fcm_obj_movmt_dir6);
    pm.updateCPropertyValue("ec_fcm_obj_typ7", dst_p.fcm_obj_typ7);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst7", dst_p.fcm_obj_lat_dst7);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst7", dst_p.fcm_obj_lgt_dst7);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl7", dst_p.fcm_obj_posn_agl7);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_sta7", dst_p.fcm_obj_movmt_sta7);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst8", dst_p.fcm_obj_lat_dst8);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst8", dst_p.fcm_obj_lgt_dst8);
    pm.updateCPropertyValue("ec_fcm_obj_typ8", dst_p.fcm_obj_typ8);
    pm.updateCPropertyValue("ec_fcm_obj_typ9", dst_p.fcm_obj_typ9);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_sta8", dst_p.fcm_obj_movmt_sta8);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl8", dst_p.fcm_obj_posn_agl8);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst9", dst_p.fcm_obj_lat_dst9);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst9", dst_p.fcm_obj_lgt_dst9);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl9", dst_p.fcm_obj_posn_agl9);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir8", dst_p.fcm_obj_movmt_dir8);
}

void a_me11_cmx_v02_fcm_24_obj1018_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    if(!is_valid_data(message)){
        LOG(INFO) << "a_me11_cmx_v02_fcm_24_obj1018_bac_unpack reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_fcm_24_obj1018_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_fcm_24_obj1018_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_fcm_24_obj1018_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.fcm_obj_typ10);
    }
    pm.updateCPropertyValue("ec_fcm_obj_typ10", dst_p.fcm_obj_typ10);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst10", dst_p.fcm_obj_lat_dst10);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst10", dst_p.fcm_obj_lgt_dst10);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl10", dst_p.fcm_obj_posn_agl10);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_sta10", dst_p.fcm_obj_movmt_sta10);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst11", dst_p.fcm_obj_lat_dst11);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst11", dst_p.fcm_obj_lgt_dst11);
    pm.updateCPropertyValue("ec_fcm_obj_typ11", dst_p.fcm_obj_typ11);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl11", dst_p.fcm_obj_posn_agl11);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_sta11", dst_p.fcm_obj_movmt_sta11);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst12", dst_p.fcm_obj_lat_dst12);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst12", dst_p.fcm_obj_lgt_dst12);
    pm.updateCPropertyValue("ec_fcm_obj_typ12", dst_p.fcm_obj_typ12);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir11", dst_p.fcm_obj_movmt_dir11);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl12", dst_p.fcm_obj_posn_agl12);
    pm.updateCPropertyValue("ec_fcm_obj_typ13", dst_p.fcm_obj_typ13);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst13", dst_p.fcm_obj_lat_dst13);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst13", dst_p.fcm_obj_lgt_dst13);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_sta13", dst_p.fcm_obj_movmt_sta13);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl13", dst_p.fcm_obj_posn_agl13);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst14", dst_p.fcm_obj_lat_dst14);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst14", dst_p.fcm_obj_lgt_dst14);
    pm.updateCPropertyValue("ec_fcm_obj_typ14", dst_p.fcm_obj_typ14);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_sta14", dst_p.fcm_obj_movmt_sta14);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl14", dst_p.fcm_obj_posn_agl14);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst15", dst_p.fcm_obj_lat_dst15);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst15", dst_p.fcm_obj_lgt_dst15);
    pm.updateCPropertyValue("ec_fcm_obj_typ15", dst_p.fcm_obj_typ15);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir14", dst_p.fcm_obj_movmt_dir14);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl15", dst_p.fcm_obj_posn_agl15);
    pm.updateCPropertyValue("ec_fcm_obj_typ16", dst_p.fcm_obj_typ16);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst16", dst_p.fcm_obj_lat_dst16);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst16", dst_p.fcm_obj_lgt_dst16);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst17", dst_p.fcm_obj_lat_dst17);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst17", dst_p.fcm_obj_lgt_dst17);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_sta16", dst_p.fcm_obj_movmt_sta16);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir16", dst_p.fcm_obj_movmt_dir16);
    pm.updateCPropertyValue("ec_fcm_obj_typ17", dst_p.fcm_obj_typ17);
    pm.updateCPropertyValue("ec_fcm_obj_typ18", dst_p.fcm_obj_typ18);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir17", dst_p.fcm_obj_movmt_dir17);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_sta17", dst_p.fcm_obj_movmt_sta17);
    pm.updateCPropertyValue("ec_fcm_obj_lat_dst18", dst_p.fcm_obj_lat_dst18);
    pm.updateCPropertyValue("ec_fcm_obj_lgt_dst18", dst_p.fcm_obj_lgt_dst18);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl18", dst_p.fcm_obj_posn_agl18);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir18", dst_p.fcm_obj_movmt_dir18);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_sta18", dst_p.fcm_obj_movmt_sta18);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd10", dst_p.fcm_lgt_rel_spd10);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl16", dst_p.fcm_obj_posn_agl16);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd11", dst_p.fcm_lgt_rel_spd11);
    pm.updateCPropertyValue("ec_fcm_obj_posn_agl17", dst_p.fcm_obj_posn_agl17);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd12", dst_p.fcm_lgt_rel_spd12);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd13", dst_p.fcm_lgt_rel_spd13);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir10", dst_p.fcm_obj_movmt_dir10);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd14", dst_p.fcm_lgt_rel_spd14);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_sta12", dst_p.fcm_obj_movmt_sta12);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd15", dst_p.fcm_lgt_rel_spd15);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir12", dst_p.fcm_obj_movmt_dir12);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd16", dst_p.fcm_lgt_rel_spd16);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir13", dst_p.fcm_obj_movmt_dir13);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_sta15", dst_p.fcm_obj_movmt_sta15);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd17", dst_p.fcm_lgt_rel_spd17);
    pm.updateCPropertyValue("ec_fcm_lgt_rel_spd18", dst_p.fcm_lgt_rel_spd18);
    pm.updateCPropertyValue("ec_fcm_obj_movmt_dir15", dst_p.fcm_obj_movmt_dir15);

}

void a_me11_cmx_v02_fcm_25_line123_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    if(!is_valid_data(message)){
        LOG(INFO) << "a_me11_cmx_v02_fcm_25_line123_bac_unpack reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_fcm_25_line123_bac_t dst_p;
    std::string value;
       me11_cmx_baccanfd_ots_car_v02_07_20240520_fcm_25_line123_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_fcm_25_line123_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.fcm_nr_of_line);
    }
    pm.updateCPropertyValue("ec_fcm_nr_of_line", dst_p.fcm_nr_of_line);
    pm.updateCPropertyValue("ec_fcm_le_line_typ", dst_p.fcm_le_line_typ);
    pm.updateCPropertyValue("ec_fcm_le_line_rd", dst_p.fcm_le_line_rd);
    pm.updateCPropertyValue("ec_fcm_le_line_dst", dst_p.fcm_le_line_dst);
    pm.updateCPropertyValue("ec_fcm_le_line_c1", dst_p.fcm_le_line_c1);
    pm.updateCPropertyValue("ec_fcm_le_line_c2", dst_p.fcm_le_line_c2);
    pm.updateCPropertyValue("ec_fcm_ri_line_typ", dst_p.fcm_ri_line_typ);
    pm.updateCPropertyValue("ec_fcm_le_line_c3", dst_p.fcm_le_line_c3);
    pm.updateCPropertyValue("ec_fcm_ri_line_rd", dst_p.fcm_ri_line_rd);
    pm.updateCPropertyValue("ec_fcm_ri_line_dst", dst_p.fcm_ri_line_dst);
    pm.updateCPropertyValue("ec_fcm_ri_line_c1", dst_p.fcm_ri_line_c1);
    pm.updateCPropertyValue("ec_fcm_ri_line_c3", dst_p.fcm_ri_line_c3);
    pm.updateCPropertyValue("ec_fcm_ri_line_c2", dst_p.fcm_ri_line_c2);
    pm.updateCPropertyValue("ec_fcm_le_le_line_typ", dst_p.fcm_le_le_line_typ);
    pm.updateCPropertyValue("ec_fcm_le_le_line_c1", dst_p.fcm_le_le_line_c1);
    pm.updateCPropertyValue("ec_fcm_le_le_line_c3", dst_p.fcm_le_le_line_c3);
    pm.updateCPropertyValue("ec_fcm_le_le_line_rd", dst_p.fcm_le_le_line_rd);
    pm.updateCPropertyValue("ec_fcm_le_le_line_dst", dst_p.fcm_le_le_line_dst);
    pm.updateCPropertyValue("ec_fcm_ri_ri_line_c1", dst_p.fcm_ri_ri_line_c1);
    pm.updateCPropertyValue("ec_fcm_ri_ri_line_c3", dst_p.fcm_ri_ri_line_c3);
    pm.updateCPropertyValue("ec_fcm_ri_ri_line_rd", dst_p.fcm_ri_ri_line_rd);
    pm.updateCPropertyValue("ec_fcm_ri_ri_line_dst", dst_p.fcm_ri_ri_line_dst);
    pm.updateCPropertyValue("ec_fcm_ri_ri_line_typ", dst_p.fcm_ri_ri_line_typ);
    pm.updateCPropertyValue("ec_fcm_le_le_line_c2", dst_p.fcm_le_le_line_c2);
    pm.updateCPropertyValue("ec_fcm_ri_ri_line_c2", dst_p.fcm_ri_ri_line_c2);
}

void a_me11_cmx_v02_scs_le_switch_sts_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    if(!is_valid_data(message)){
        pm.updateCPropertyValue("mc_scs_frnt_wipr_swt_st", -1024);
        LOG(INFO) << "a_me11_cmx_v02_scs_le_switch_sts_bac_unpack reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_scs_le_switch_sts_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_scs_le_switch_sts_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_scs_le_switch_sts_bac_t));

    pm.updateCPropertyValue("mc_scs_frnt_wipr_swt_st", dst_p.scs_frnt_wipr_swt_st);
    pm.updateCPropertyValue("mc_scs_low_beam_swt_st", dst_p.scs_low_beam_swt_st);
    pm.updateCPropertyValue("mc_scs_hi_beam_swt_st", dst_p.scs_hi_beam_swt_st);
    pm.updateCPropertyValue("mc_scs_over_veh_beam_swt_st", dst_p.scs_over_veh_beam_swt_st);
    pm.updateCPropertyValue("mc_scs_lamp_auto_st", dst_p.scs_lamp_auto_st);
    pm.updateCPropertyValue("mc_scs_le_turn_lmp_swt_st", dst_p.scs_le_turn_lmp_swt_st);
    pm.updateCPropertyValue("mc_scs_ri_turn_lmp_swt_st", dst_p.scs_ri_turn_lmp_swt_st);
    if(dst_p.scs_left_1_swt == 1)   //按下
    {
        left_1_up++;
        if (left_1_up > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_previous_song", getReversalValue(dst_p.scs_left_1_swt));
        }
        left_1_down = 0;
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac up scs_left_1_swt:%d left_1_up:%d, left_1_down:%d", dst_p.scs_left_1_swt,left_1_up,left_1_down);
        }
    }
    else if(dst_p.scs_left_1_swt == 0 && left_1_up > 0){ //抬起
        left_1_down++;
        if (left_1_down > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_previous_song", getReversalValue(dst_p.scs_left_1_swt));
            left_1_up = 0;
        }
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac down scs_left_1_swt:%d left_1_up:%d, left_1_down:%d", dst_p.scs_left_1_swt,left_1_up,left_1_down);
        }
    }
    
    if(dst_p.scs_left_2_swt == 1)   //按下
    {
        left_2_up++;
        if (left_2_up > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_next_song", getReversalValue(dst_p.scs_left_2_swt));
        }
        left_2_down = 0;
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac up scs_left_2_swt:%d left_2_up:%d, left_2_down:%d", dst_p.scs_left_2_swt,left_2_up,left_2_down);
        }
    }
    else if(dst_p.scs_left_2_swt == 0 && left_2_up > 0){ //抬起
        left_2_down++;
        if (left_2_down > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_next_song", getReversalValue(dst_p.scs_left_2_swt));
            left_2_up = 0;
        }
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac down scs_left_2_swt:%d left_2_up:%d, left_2_down:%d", dst_p.scs_left_2_swt,left_2_up,left_2_down);
        }
    }


    if(dst_p.scs_left_3_swt == 1)   //按下
    {
        left_3_up++;
        if (left_3_up > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_volume_up", getReversalValue(dst_p.scs_left_3_swt));
        }
        left_3_down = 0;
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac up scs_left_3_swt:%d left_3_up:%d, left_3_down:%d", dst_p.scs_left_3_swt,left_3_up,left_3_down);
        }
    }
    else if(dst_p.scs_left_3_swt == 0 && left_3_up > 0){ //抬起
        left_3_down++;
        if (left_3_down > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_volume_up", getReversalValue(dst_p.scs_left_3_swt));
            left_3_up = 0;
        }
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac down scs_left_3_swt:%d left_3_up:%d, left_3_down:%d", dst_p.scs_left_3_swt,left_3_up,left_3_down);
        }
    }


    if(dst_p.scs_left_4_swt == 1)   //按下
    {
        left_4_up++;
        if (left_4_up > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_volume_dowm", getReversalValue(dst_p.scs_left_4_swt));
        }
        left_4_down = 0;
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac up scs_left_4_swt:%d left_4_up:%d, left_4_down:%d", dst_p.scs_left_4_swt,left_4_up,left_4_down);
        }
    }
    else if(dst_p.scs_left_4_swt == 0 && left_4_up > 0){ //抬起
        left_4_down++;
        if (left_4_down > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_volume_dowm", getReversalValue(dst_p.scs_left_4_swt));
            left_4_up = 0;
        }
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac down scs_left_4_swt:%d left_4_up:%d, left_4_down:%d", dst_p.scs_left_4_swt,left_4_up,left_4_down);
        }
    }
    // pm.updateCPropertyValueNoCheck("mc_key_custom", getReversalValue(dst_p.scs_left_5_swt));
    if(dst_p.scs_left_5_swt == 1)   //按下
    {
        left_5_up++;
        if (left_5_up > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_custom", getReversalValue(dst_p.scs_left_5_swt));
        }
        left_5_down = 0;
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac up scs_left_5_swt:%d left_5_up:%d, left_5_down:%d", dst_p.scs_left_5_swt,left_5_up,left_5_down);
        }
    }
    else if(dst_p.scs_left_5_swt == 0 && left_5_up > 0){ //抬起
        left_5_down++;
        if (left_5_down > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_custom", getReversalValue(dst_p.scs_left_5_swt));
            left_5_up = 0;
        }
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac down scs_left_5_swt:%d left_5_up:%d, left_5_down:%d", dst_p.scs_left_5_swt,left_5_up,left_5_down);
        }
    }


    // pm.updateCPropertyValueNoCheck("mc_key_telephone", getReversalValue(dst_p.scs_left_6_swt));
    if(dst_p.scs_left_6_swt == 1)   //按下
    {
        left_6_up++;
        if (left_6_up > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_telephone", getReversalValue(dst_p.scs_left_6_swt));
        }
        left_6_down = 0;
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac up scs_left_6_swt:%d left_6_up:%d, left_6_down:%d", dst_p.scs_left_6_swt,left_6_up,left_6_down);
        }
    }
    else if(dst_p.scs_left_6_swt == 0 && left_6_up > 0){ //抬起
        left_6_down++;
        if (left_6_down > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_telephone", getReversalValue(dst_p.scs_left_6_swt));
            left_6_up = 0;
        }
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac down scs_left_6_swt:%d left_6_up:%d, left_6_down:%d", dst_p.scs_left_6_swt,left_6_up,left_6_down);
        }
    }


    // pm.updateCPropertyValueNoCheck("mc_key_voice", getReversalValue(dst_p.scs_left_7_swt));
    if(dst_p.scs_left_7_swt == 1)   //按下
    {
        left_7_up++;
        if (left_7_up > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_voice", getReversalValue(dst_p.scs_left_7_swt));
        }
        left_7_down = 0;
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac up scs_left_7_swt:%d left_7_up:%d, left_7_down:%d", dst_p.scs_left_7_swt,left_7_up,left_7_down);
        }
    }
    else if(dst_p.scs_left_7_swt == 0 && left_7_up > 0){ //抬起
        left_7_down++;
        if (left_7_down > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_voice", getReversalValue(dst_p.scs_left_7_swt));
            left_7_up = 0;
        }
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac down scs_left_7_swt:%d left_7_up:%d, left_7_down:%d", dst_p.scs_left_7_swt,left_7_up,left_7_down);
        }
    }


    // pm.updateCPropertyValueNoCheck("mc_key_mode_mute", getReversalValue(dst_p.scs_left_8_swt));
    if(dst_p.scs_left_8_swt == 1)   //按下
    {
        left_8_up++;
        if (left_8_up > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_mode_mute", getReversalValue(dst_p.scs_left_8_swt));
        }
        left_8_down = 0;
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac up scs_left_8_swt:%d left_8_up:%d, left_8_down:%d", dst_p.scs_left_8_swt,left_8_up,left_8_down);
        }
    }
    else if(dst_p.scs_left_8_swt == 0 && left_8_up > 0){ //抬起
        left_8_down++;
        if (left_8_down > disturbCount)
        {
            pm.updateCPropertyValueNoCheck("mc_key_mode_mute", getReversalValue(dst_p.scs_left_8_swt));
            left_8_up = 0;
        }
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("switch_sts_bac down scs_left_8_swt:%d left_8_up:%d, left_8_down:%d", dst_p.scs_left_8_swt,left_8_up,left_8_down);
        }
    }
}

void a_me11_cmx_v02_avas_state_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    if(!is_valid_data(message)) {
        pm.updateCPropertyValue("bc_avas_state", -1024);
        return;
    }

    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_avas_state_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_avas_state_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_avas_state_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.avas_state);
    }
    pm.updateCPropertyValue("ec_avas_state_rolling_counter", dst_p.avas_state_rolling_counter);
    pm.updateCPropertyValue("bc_avas_state", dst_p.avas_state);
}

void a_me11_cmx_v03_ipu_dcc_1_state_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_dcc_1_state_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_dcc_1_state_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_dcc_1_state_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.ipu_dcc_actl_oprt_mode);
    }
    pm.updateCPropertyValue("ec_ipu_dcc_actl_oprt_mode", dst_p.ipu_dcc_actl_oprt_mode);
    pm.updateCPropertyValue("ec_ipu_over_temperature_fail", dst_p.ipu_over_temperature_fail);
    pm.updateCPropertyValue("ec_ipu_dcc_sys_fail_sta", dst_p.ipu_dcc_sys_fail_sta);
    pm.updateCPropertyValue("ec_ipu_dcc_low_vol_actl_vol", dst_p.ipu_dcc_low_vol_actl_vol);
    pm.updateCPropertyValue("ec_ipu_dcc_low_vol_actl_cur", dst_p.ipu_dcc_low_vol_actl_cur);
    pm.updateCPropertyValue("ec_ipu_dcc_precharge_complete", dst_p.ipu_dcc_precharge_complete);
    pm.updateCPropertyValue("ec_ipu_dcc_derating_oprt_sta", dst_p.ipu_dcc_derating_oprt_sta);
    pm.updateCPropertyValue("ec_ipu_hvil_fail_sta", dst_p.ipu_hvil_fail_sta);
    pm.updateCPropertyValue("ec_ipu_dcc_errbit", dst_p.ipu_dcc_errbit);
}

void a_me11_cmx_v03_vcu_d_status_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    if(!is_valid_data(message)){
        pm.updateCPropertyValue("ec_vcu_cruise_ctrl_sta", -1024);
        pm.updateCPropertyValue("ec_vcu_cruise_ctr_tgt_spd", 0);
        LOG(INFO) << "a_me11_cmx_v03_vcu_d_status_bac_unpack reset done";
        return;
    }
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_vcu_d_status_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_vcu_d_status_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_vcu_d_status_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d %d", __func__,__LINE__, dst_p.vcu_accl_pedal_pos, dst_p.vcu_brk_pedal_pos);
    }
    pm.updateCPropertyValue("ec_vcu_accl_pedal_pos", dst_p.vcu_accl_pedal_pos);
    pm.updateCPropertyValue("ec_vcu_brk_pedal_pos", dst_p.vcu_brk_pedal_pos);
    pm.updateCPropertyValue("ec_vcu_accl_pedal_pos_snsr_sta", dst_p.vcu_accl_pedal_pos_snsr_sta);
    pm.updateCPropertyValue("ec_vcu_req_epb", dst_p.vcu_req_epb);
    pm.updateCPropertyValue("ec_vcu_acc_trq_ctrl_avl", dst_p.vcu_acc_trq_ctrl_avl);
    pm.updateCPropertyValue("ec_vcu_acc_standstill_req", dst_p.vcu_acc_standstill_req);
    pm.updateCPropertyValue("ec_vcu_acc_tgt_ax", dst_p.vcu_acc_tgt_ax);
    pm.updateCPropertyValue("ec_vcu_acc_tgt_ax_req", dst_p.vcu_acc_tgt_ax_req);
    pm.updateCPropertyValue("ec_vcu_drv_req_torq_override_fcm", dst_p.vcu_drv_req_torq_override_fcm);
    pm.updateCPropertyValue("ec_vcu_cruise_ctr_tgt_spd", dst_p.vcu_cruise_ctr_tgt_spd);
    pm.updateCPropertyValue("ec_vcu_cruise_ctrl_sta", dst_p.vcu_cruise_ctrl_sta);
    pm.updateCPropertyValue("ec_vcu_bra_tor_req", dst_p.vcu_bra_tor_req);
}

void a_me11_cmx_v03_bms_4_ac_charge_state_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_4_ac_charge_state_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_4_ac_charge_state_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_4_ac_charge_state_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d %d", __func__,__LINE__, dst_p.bms_ac_chrg_atcl_sta, dst_p.bms_iso_resistance);
    }
    pm.updateCPropertyValue("ec_bms_iso_resistance", dst_p.bms_iso_resistance);
    //pm.updateCPropertyValue("ec_bms_ac_chrg_atcl_sta", dst_p.bms_ac_chrg_atcl_sta);
    pm.updateCPropertyValue("ec_bms_ac_chrg_atcl_sta", dst_p.bms_ac_chrg_atcl_sta);
    pm.updateCPropertyValue("ec_bms_outlet_coolant_actl_temp", dst_p.bms_outlet_coolant_actl_temp);
    pm.updateCPropertyValue("ec_bms_outlet_coolant_actl_temp_valid", dst_p.bms_outlet_coolant_actl_temp_valid);
    pm.updateCPropertyValue("ec_bms_inlet_coolant_actl_temp", dst_p.bms_inlet_coolant_actl_temp);
    pm.updateCPropertyValue("ec_bms_inlet_coolant_actl_temp_valid", dst_p.bms_inlet_coolant_actl_temp_valid);
    pm.updateCPropertyValue("ec_bms_max_chrg_voltage_ac", dst_p.bms_max_chrg_voltage_ac);
    pm.updateCPropertyValue("ec_bms_max_chrg_current_ac", dst_p.bms_max_chrg_current_ac);
}

void a_me11_cmx_v03_ehb_b_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_ehb_b_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_ehb_b_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_ehb_b_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d %d", __func__,__LINE__, dst_p.ehb_brk_pedl_val, dst_p.ehb_brk_pedl_val_vld);
    }
    pm.updateCPropertyValue("ec_ehb_brk_pedl_val", dst_p.ehb_brk_pedl_val);
    pm.updateCPropertyValue("ec_ehb_brk_pedl_val_vld", dst_p.ehb_brk_pedl_val_vld);
    pm.updateCPropertyValue("ec_ehb_brk_pedl_val_perc", dst_p.ehb_brk_pedl_val_perc);
    pm.updateCPropertyValue("ec_ehb_brk_pedl_val_perc_vld", dst_p.ehb_brk_pedl_val_perc_vld);
    pm.updateCPropertyValue("ec_ehb_epb_dynamic_apply_sta", dst_p.ehb_epb_dynamic_apply_sta);
    pm.updateCPropertyValue("ec_ehb_pedl_trvl_snsr_fail_pdf", dst_p.ehb_pedl_trvl_snsr_fail_pdf);
    pm.updateCPropertyValue("ec_ehb_req_brk_li_on", dst_p.ehb_req_brk_li_on);
    pm.updateCPropertyValue("ec_ehb_pedl_trvl_snsr_fail_pdt", dst_p.ehb_pedl_trvl_snsr_fail_pdt);
    pm.updateCPropertyValue("ec_ehb_p_runout_pressure", dst_p.ehb_p_runout_pressure);
    pm.updateCPropertyValue("ec_ehb_brake_pedal_applied", dst_p.ehb_brake_pedal_applied);
    pm.updateCPropertyValue("ec_ehb_brake_pedal_applied_q", dst_p.ehb_brake_pedal_applied_q);
    pm.updateCPropertyValue("ec_ehb_cdd_temp_off", dst_p.ehb_cdd_temp_off);
    //pm.updateCPropertyValue("ec_ehb_s_output_rod_hydraulic_target", dst_p.ehb_s_output_rod_hydraulic_target);
    pm.updateCPropertyValue("ec_ehb_s_output_rod_driver_perc", dst_p.ehb_s_output_rod_driver_perc);
    pm.updateCPropertyValue("ec_ehb_epb_actuator_st_r", dst_p.ehb_epb_actuator_st_r);
    pm.updateCPropertyValue("ec_ehb_aba_active_sta", dst_p.ehb_aba_active_sta);
    pm.updateCPropertyValue("ec_ehb_aba_available_sta", dst_p.ehb_aba_available_sta);
    pm.updateCPropertyValue("ec_ehb_awb_active_sta", dst_p.ehb_awb_active_sta);
    pm.updateCPropertyValue("ec_ehb_awb_available_sta", dst_p.ehb_awb_available_sta);
    pm.updateCPropertyValue("ec_ehb_aeb_active_sta", dst_p.ehb_aeb_active_sta);
    pm.updateCPropertyValue("ec_ehb_aeb_available_sta", dst_p.ehb_aeb_available_sta);
    pm.updateCPropertyValue("ec_ehb_abp_active_sta", dst_p.ehb_abp_active_sta);
    pm.updateCPropertyValue("ec_ehb_abp_available_sta", dst_p.ehb_abp_available_sta);
    pm.updateCPropertyValue("ec_ehb_cdp_request", dst_p.ehb_cdp_request);
}

void a_me11_cmx_v03_bms_7_cell_volt_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_7_cell_volt_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_7_cell_volt_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_7_cell_volt_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d %d", __func__,__LINE__, dst_p.bms_max_vol_cell_cod, dst_p.bms_cell_vol_max);
    }
    pm.updateCPropertyValue("ec_bms_maxvoltmonomercell", dst_p.bms_max_vol_cell_cod);
    pm.updateCPropertyValue("ec_bms_maxvoltcell", dst_p.bms_cell_vol_max);
    pm.updateCPropertyValue("ec_bms_minvoltmonomercell", dst_p.bms_min_vol_cell_cod);
    pm.updateCPropertyValue("ec_bms_minvoltcell", dst_p.bms_cell_vol_min);
    pm.updateCPropertyValue("ec_bms_cell_vol_ave", dst_p.bms_cell_vol_ave);
}

void a_me11_cmx_v03_bms_5_bat_temp_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_5_bat_temp_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_5_bat_temp_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_5_bat_temp_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d %d", __func__,__LINE__, dst_p.bms_max_temp_probe_cod, dst_p.bms_min_temp_probe_cod);
    }
    pm.updateCPropertyValue("ec_bms_maxtempcell", dst_p.bms_hv_bat_cell_temp_max);
    pm.updateCPropertyValue("ec_bms_mintempcell", dst_p.bms_hv_bat_cell_temp_min);
    pm.updateCPropertyValue("ec_bms_maxtempmonomercell", dst_p.bms_max_temp_probe_cod);
    pm.updateCPropertyValue("ec_bms_mintempmonomercell", dst_p.bms_min_temp_probe_cod);
    pm.updateCPropertyValue("ec_bms_hv_bat_cell_temp_ave", dst_p.bms_hv_bat_cell_temp_ave);
}

void a_me11_cmx_v03_bms_hmi_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    if(!is_valid_data(message)) {
        pm.updateCPropertyValue("chraging_target_soc", -1024);
        LOG(INFO) << "a_me11_cmx_v03_bms_hmi_bac_unpack reset done";
        return;
    }

    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_hmi_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_hmi_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_hmi_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.bms_chrg_target_soc);
    }
    pm.updateCPropertyValue("chraging_target_soc", dst_p.bms_chrg_target_soc);
}

void a_me11_cmx_v04_eps_3_lat_drv_and_eps_sts_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_eps_3_lat_drv_and_eps_sts_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_eps_3_lat_drv_and_eps_sts_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_eps_3_lat_drv_and_eps_sts_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
       ALOGI("%s %d %d", __func__,__LINE__, dst_p.eps_mot_tq);
    }
    pm.updateCPropertyValue("ec_eps_mot_tq", dst_p.eps_mot_tq);
    pm.updateCPropertyValue("ec_eps_lat_drv_lvl12_srv_avl", dst_p.eps_lat_drv_lvl12_srv_avl);
    pm.updateCPropertyValue("ec_eps_lat_mc_hptc_wrn_srv_avl", dst_p.eps_lat_mc_hptc_wrn_srv_avl);
    pm.updateCPropertyValue("ec_eps_pscm_int_rpc_max_trq", dst_p.eps_pscm_int_rpc_max_trq);
    pm.updateCPropertyValue("ec_eps_torsion_bar_torque_qf", dst_p.eps_torsion_bar_torque_qf);
    pm.updateCPropertyValue("ec_eps_pscm_int_rpc_min_trq", dst_p.eps_pscm_int_rpc_min_trq);
    pm.updateCPropertyValue("ec_eps_lat_mc_steer_tq_active", dst_p.eps_lat_mc_steer_tq_active);
    pm.updateCPropertyValue("ec_eps_lat_mc_hptc_wrn_active", dst_p.eps_lat_mc_hptc_wrn_active);
    pm.updateCPropertyValue("ec_eps_max_allwd_rot_spd", dst_p.eps_max_allwd_rot_spd);
    pm.updateCPropertyValue("ec_eps_lks_agl_sys_sts", dst_p.eps_lks_agl_sys_sts);
    pm.updateCPropertyValue("ec_eps_max_allwd_agl", dst_p.eps_max_allwd_agl);
    pm.updateCPropertyValue("ec_eps_steer_tq", dst_p.eps_steer_tq);
    pm.updateCPropertyValue("ec_eps_lks_abort_fb", dst_p.eps_lks_abort_fb);
    pm.updateCPropertyValue("ec_eps_lks_agl_ctrl_avl_sts", dst_p.eps_lks_agl_ctrl_avl_sts);
}

void a_me11_cmx_v04_eps_2_str_whl_ang_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_eps_2_str_whl_ang_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_eps_2_str_whl_ang_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_eps_2_str_whl_ang_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.eps_steer_wheel_angle);
    }
    pm.updateCPropertyValue("ec_eps_steer_wheel_angle", dst_p.eps_steer_wheel_angle);
    pm.updateCPropertyValue("ec_eps_steer_wheel_angle_valid", dst_p.eps_steer_wheel_angle_valid);
    pm.updateCPropertyValue("ec_eps_rot_speed", dst_p.eps_rot_speed);
    pm.updateCPropertyValue("ec_eps_electric_power_sta", dst_p.eps_electric_power_sta);
    pm.updateCPropertyValue("ec_eps_steer_wheel_angle_failure", dst_p.eps_steer_wheel_angle_failure);
    pm.updateCPropertyValue("ec_eps_steer_wheel_angle_calibrated", dst_p.eps_steer_wheel_angle_calibrated);
}

void a_me11_cmx_v03_bms_9_batt_info_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_9_batt_info_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_9_batt_info_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_9_batt_info_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.bms_battery_type);
    }
    pm.updateCPropertyValue("ec_bms_battery_type", dst_p.bms_battery_type);
    pm.updateCPropertyValue("ec_bms_hw_version", dst_p.bms_hw_version);
    pm.updateCPropertyValue("ec_bms_pack_identity", dst_p.bms_pack_identity);
    pm.updateCPropertyValue("ec_bms_sw_version", dst_p.bms_sw_version);
    pm.updateCPropertyValue("ec_bms_nom_cell_cap", dst_p.bms_nom_cell_cap);
    pm.updateCPropertyValue("ec_bms_bat_capacity", dst_p.bms_bat_capacity);
}

void a_me11_cmx_v03_esc_a_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_esc_a_bac_t  dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_esc_a_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_esc_a_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.esc_dtc_active_sts);
    }
    pm.updateCPropertyValue("ec_esc_brake_temp_over_heat", dst_p.esc_brake_temp_over_heat);
    pm.updateCPropertyValue("ec_esc_dtc_active_sts", dst_p.esc_dtc_active_sts);
    pm.updateCPropertyValue("ec_esc_vhcl_standstill_sta", dst_p.esc_vhcl_standstill_sta);
    pm.updateCPropertyValue("ec_esc_dtc_request_rbs_sts", dst_p.esc_dtc_request_rbs_sts);
    pm.updateCPropertyValue("ec_esc_dtc_failure_sts", dst_p.esc_dtc_failure_sts);
    pm.updateCPropertyValue("ec_esc_wheel_speed_kph_v_fl", dst_p.esc_wheel_speed_kph_v_fl);
    pm.updateCPropertyValue("ec_esc_wheel_direction_fl", dst_p.esc_wheel_direction_fl);
    pm.updateCPropertyValue("ec_esc_wheel_speed_kph_fl", dst_p.esc_wheel_speed_kph_fl);
    pm.updateCPropertyValue("ec_esc_wheel_speed_kph_v_fr", dst_p.esc_wheel_speed_kph_v_fr);
    pm.updateCPropertyValue("ec_esc_wheel_direction_fr", dst_p.esc_wheel_direction_fr);
    pm.updateCPropertyValue("ec_esc_wheel_speed_kph_fr", dst_p.esc_wheel_speed_kph_fr);
    pm.updateCPropertyValue("ec_esc_master_cylinder_presr", dst_p.esc_master_cylinder_presr);
    pm.updateCPropertyValue("ec_esc_master_cylinder_presr_v", dst_p.esc_master_cylinder_presr_v);
    pm.updateCPropertyValue("ec_esc_wheel_speed_kph_v_rl", dst_p.esc_wheel_speed_kph_v_rl);
    pm.updateCPropertyValue("ec_esc_wheel_direction_rl", dst_p.esc_wheel_direction_rl);
    pm.updateCPropertyValue("ec_esc_wheel_speed_kph_rl", dst_p.esc_wheel_speed_kph_rl);
    pm.updateCPropertyValue("ec_esc_wheel_speed_kph_v_rr", dst_p.esc_wheel_speed_kph_v_rr);
    pm.updateCPropertyValue("ec_esc_wheel_direction_rr", dst_p.esc_wheel_direction_rr);
    pm.updateCPropertyValue("ec_esc_wheel_speed_kph_rr", dst_p.esc_wheel_speed_kph_rr);
    pm.updateCPropertyValue("ec_esc_wheel_speed_rc_sts_fl", dst_p.esc_wheel_speed_rc_sts_fl);
    pm.updateCPropertyValue("ec_esc_wheel_speed_rc_sts_fr", dst_p.esc_wheel_speed_rc_sts_fr);
    pm.updateCPropertyValue("ec_esc_wheel_speed_rc_sts_rl", dst_p.esc_wheel_speed_rc_sts_rl);
    pm.updateCPropertyValue("ec_esc_wheel_speed_rc_sts_rr", dst_p.esc_wheel_speed_rc_sts_rr);
    pm.updateCPropertyValue("ec_esc_wheel_speed_rc_fl", dst_p.esc_wheel_speed_rc_fl);
    pm.updateCPropertyValue("ec_esc_wheel_speed_rc_fr", dst_p.esc_wheel_speed_rc_fr);
    pm.updateCPropertyValue("ec_esc_wheel_speed_rc_rl", dst_p.esc_wheel_speed_rc_rl);
    pm.updateCPropertyValue("ec_esc_wheel_speed_rc_rr", dst_p.esc_wheel_speed_rc_rr);
    pm.updateCPropertyValue("ec_esc_long_acc_sensor_value", dst_p.esc_long_acc_sensor_value);
    pm.updateCPropertyValue("ec_esc_long_acc_sensor_value_vld", dst_p.esc_long_acc_sensor_value_vld);
    pm.updateCPropertyValue("ec_esc_lat_acc_sensor_value_vld", dst_p.esc_lat_acc_sensor_value_vld);
    pm.updateCPropertyValue("ec_esc_lat_acc_sensor_value", dst_p.esc_lat_acc_sensor_value);
    pm.updateCPropertyValue("ec_esc_yaw_rate_vld", dst_p.esc_yaw_rate_vld);
    pm.updateCPropertyValue("ec_esc_yaw_rate", dst_p.esc_yaw_rate);
    pm.updateCPropertyValue("ec_esc_epb_right_current", dst_p.esc_epb_right_current);
    pm.updateCPropertyValue("ec_esc_epb_right_fault_state", dst_p.esc_epb_right_fault_state);
    pm.updateCPropertyValue("ec_esc_hbb_hbc_available", dst_p.esc_hbb_hbc_available);
    pm.updateCPropertyValue("ec_esc_epb_actuator_st_r", dst_p.esc_epb_actuator_st_r);
    pm.updateCPropertyValue("ec_esc_hbb_hbc_active", dst_p.esc_hbb_hbc_active);
    pm.updateCPropertyValue("ec_esc_whl_tq_dec_req_actv", dst_p.esc_whl_tq_dec_req_actv);
    pm.updateCPropertyValue("ec_esc_whl_tq_dec_req_val", dst_p.esc_whl_tq_dec_req_val);
    pm.updateCPropertyValue("ec_esc_whl_tq_inc_req_actv", dst_p.esc_whl_tq_inc_req_actv);
    pm.updateCPropertyValue("ec_esc_whl_tq_inc_req_val", dst_p.esc_whl_tq_inc_req_val);
}

void a_me11_cmx_v03_bms_3_dc_charge_state_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_3_dc_charge_state_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_3_dc_charge_state_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_3_dc_charge_state_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.bms_pcb_temp_max);
    }
    pm.updateCPropertyValue("ec_bms_temp_max_dc_charge_socket", dst_p.bms_temp_max_dc_charge_socket);
    pm.updateCPropertyValue("ec_bms_pcb_temp_max", dst_p.bms_pcb_temp_max);
    pm.updateCPropertyValue("ec_bms_max_allow_chrg_cur_dc", dst_p.bms_max_allow_chrg_cur_dc);
    pm.updateCPropertyValue("ec_bms_max_allow_chrg_vol_dc", dst_p.bms_max_allow_chrg_vol_dc);
    pm.updateCPropertyValue("ec_bms_electronic_lock_ctrl", dst_p.bms_electronic_lock_ctrl);
    pm.updateCPropertyValue("ec_bms_electronic_lock_state", dst_p.bms_electronic_lock_state);
    pm.updateCPropertyValue("ec_bms_contactor_state_dc_charger", dst_p.bms_contactor_state_dc_charger);
    pm.updateCPropertyValue("ec_bms_dcs_charge_request", dst_p.bms_dcs_charge_request);
    pm.updateCPropertyValue("ec_bms_dcs_chrg_a_plus", dst_p.bms_dcs_chrg_a_plus);
}


void a_me11_cmx_v03_vcu_10_torque_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_vcu_10_torque_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_vcu_10_torque_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_vcu_10_torque_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
       ALOGI("%s %d %d", __func__,__LINE__, dst_p.vcu_act_whl_tq);
    }
    pm.updateCPropertyValue("ec_vcu_act_whl_tq", dst_p.vcu_act_whl_tq);
    pm.updateCPropertyValue("ec_vcu_act_whl_tq_vld", dst_p.vcu_act_whl_tq_vld);
    pm.updateCPropertyValue("ec_vcu_drv_req_whl_tq", dst_p.vcu_drv_req_whl_tq);
    pm.updateCPropertyValue("ec_vcu_drv_whl_tq_allwd_max", dst_p.vcu_drv_whl_tq_allwd_max);
    pm.updateCPropertyValue("ec_vcu_pwr_recup_max_avlb_whl_tq_sts", dst_p.vcu_pwr_recup_max_avlb_whl_tq_sts);
    pm.updateCPropertyValue("ec_vcu_pwr_recup_actl_whl_tq_sts", dst_p.vcu_pwr_recup_actl_whl_tq_sts);
    pm.updateCPropertyValue("ec_vcu_brk_pwr_recup_max_avlb_whl_tq", dst_p.vcu_brk_pwr_recup_max_avlb_whl_tq);
    pm.updateCPropertyValue("ec_vcu_pwr_recup_act_whl_tq", dst_p.vcu_pwr_recup_act_whl_tq);
    pm.updateCPropertyValue("ec_vcu_brk_pwr_recup_act_whl_tq", dst_p.vcu_brk_pwr_recup_act_whl_tq);
    pm.updateCPropertyValue("ec_vcu_brk_pwr_recup_actl_whl_tq_sts", dst_p.vcu_brk_pwr_recup_actl_whl_tq_sts);
    pm.updateCPropertyValue("ec_vcu_acc_req_drive_off", dst_p.vcu_acc_req_drive_off);
    pm.updateCPropertyValue("ec_vcu_actl_motor_rotate_spd", dst_p.vcu_actl_motor_rotate_spd);
    pm.updateCPropertyValue("ec_vcu_actl_motor_rotate_spd_v", dst_p.vcu_actl_motor_rotate_spd_v);
    pm.updateCPropertyValue("ec_vcu_drv_req_whl_tq_vld", dst_p.vcu_drv_req_whl_tq_vld);
    pm.setCan071update();
}

void a_me11_cmx_v03_bms_6_dischrg_recup_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_6_dischrg_recup_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_6_dischrg_recup_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_6_dischrg_recup_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.bms_max_con_dis_curnt);
    }
    pm.updateCPropertyValue("ec_bms_max_con_dis_curnt", dst_p.bms_max_con_dis_curnt);
    pm.updateCPropertyValue("ec_bms_pwr_recup_max_con_chrg_curnt", dst_p.bms_pwr_recup_max_con_chrg_curnt);
    pm.updateCPropertyValue("ec_bms_max_instan_dis_curnt", dst_p.bms_max_instan_dis_curnt);
    pm.updateCPropertyValue("ec_bms_pwr_recup_max_instan_chrg_curnt", dst_p.bms_pwr_recup_max_instan_chrg_curnt);
}

void a_me11_cmx_v03_ipu_obc_3_cp_cc_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_obc_3_cp_cc_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_obc_3_cp_cc_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_obc_3_cp_cc_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.ipu_obc_cc_resisdent);
    }
    pm.updateCPropertyValue("ec_ipu_obc_c_cor_cp_cnctd_sts", dst_p.ipu_obc_c_cor_cp_cnctd_sts);
    pm.updateCPropertyValue("ec_ipu_obc_cp_duty", dst_p.ipu_obc_cp_duty);
    pm.updateCPropertyValue("ec_ipu_obc_cp_max_volt", dst_p.ipu_obc_cp_max_volt);
    pm.updateCPropertyValue("ec_ipu_obc_process_code", dst_p.ipu_obc_process_code);
    pm.updateCPropertyValue("ec_ipu_obc_cc_resisdent", dst_p.ipu_obc_cc_resisdent);
}

void a_me11_cmx_v03_bms_8_temp_volt_limit_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_8_temp_volt_limit_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_8_temp_volt_limit_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_8_temp_volt_limit_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.bms_contactor_temp_negative);
    }
    pm.updateCPropertyValue("ec_bms_contactor_temp_negative", dst_p.bms_contactor_temp_negative);
    pm.updateCPropertyValue("ec_bms_max_cell_temp_limit", dst_p.bms_max_cell_temp_limit);
    pm.updateCPropertyValue("ec_bms_max_cell_vol_limit", dst_p.bms_max_cell_vol_limit);
    pm.updateCPropertyValue("ec_bms_pwr_limd_flg", dst_p.bms_pwr_limd_flg);
    pm.updateCPropertyValue("ec_bms_min_cell_temp_limit", dst_p.bms_min_cell_temp_limit);
    pm.updateCPropertyValue("ec_bms_min_cell_vol_limit", dst_p.bms_min_cell_vol_limit);
    pm.updateCPropertyValue("ec_bms_err_tab_index", dst_p.bms_err_tab_index);
}

void a_me11_cmx_v03_ipu_dcc_2_chrg_in_value_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_dcc_2_chrg_in_value_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_dcc_2_chrg_in_value_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_dcc_2_chrg_in_value_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.ipu_dcc_high_volt_actl_cur);
    }
    pm.updateCPropertyValue("ec_ipu_dcc_high_volt_actl_cur", dst_p.ipu_dcc_high_volt_actl_cur);
    pm.updateCPropertyValue("ec_ipu_dcc_high_volt_actl_vol", dst_p.ipu_dcc_high_volt_actl_vol);
}

void a_me11_cmx_v03_bms_bem_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_bem_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_bem_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_bem_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.bem_spn2560_aa);
    }
    pm.updateCPropertyValue("ec_bem_spn2560_aa", dst_p.bem_spn2560_aa);
    pm.updateCPropertyValue("ec_bem_spn2560_00", dst_p.bem_spn2560_00);
    pm.updateCPropertyValue("ec_bem_spn3904_cro", dst_p.bem_spn3904_cro);
    pm.updateCPropertyValue("ec_bem_spn3903_cml_cts", dst_p.bem_spn3903_cml_cts);
    pm.updateCPropertyValue("ec_bem_spn3906_cst", dst_p.bem_spn3906_cst);
    pm.updateCPropertyValue("ec_bem_spn3905_ccs", dst_p.bem_spn3905_ccs);
    pm.updateCPropertyValue("ec_bem_spn3907_csd", dst_p.bem_spn3907_csd);
    pm.setCan30Aupdate();
}

void a_me11_cmx_v03_bms_cem_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_cem_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_cem_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_cem_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.cem_bms_identify_timout);
    }
    pm.updateCPropertyValue("ec_cem_bms_identify_timout", dst_p.cem_bms_identify_timout);
    pm.updateCPropertyValue("ec_cem_bms_finish_charge_timeout", dst_p.cem_bms_finish_charge_timeout);
    pm.updateCPropertyValue("ec_cem_battery_parameter_timout", dst_p.cem_battery_parameter_timout);
    pm.updateCPropertyValue("ec_cem_bms_abort_charge_timeout", dst_p.cem_bms_abort_charge_timeout);
    pm.updateCPropertyValue("ec_cem_battery_charge_require_timeout", dst_p.cem_battery_charge_require_timeout);
    pm.updateCPropertyValue("ec_cem_battery_state_timeout", dst_p.cem_battery_state_timeout);
    pm.updateCPropertyValue("ec_cem_bms_charge_statistics_timeout", dst_p.cem_bms_charge_statistics_timeout);
    pm.setCan30Bupdate();
}

void a_me11_cmx_v03_bms_bst_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_bst_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_bst_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_bst_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.bst_abort_achive_dcs);
    }
    pm.updateCPropertyValue("ec_bst_abort_achive_dcs", dst_p.bst_abort_achive_dcs);
    pm.updateCPropertyValue("ec_bst_abort_achive_single_volt", dst_p.bst_abort_achive_single_volt);
    pm.updateCPropertyValue("ec_bst_abort_achive_total_volt", dst_p.bst_abort_achive_total_volt);
    pm.updateCPropertyValue("ec_bst_abort_achive_soc", dst_p.bst_abort_achive_soc);
    pm.updateCPropertyValue("ec_bst_abort_fault_connector", dst_p.bst_abort_fault_connector);
    pm.updateCPropertyValue("ec_bst_abort_fault_bms_temp", dst_p.bst_abort_fault_bms_temp);
    pm.updateCPropertyValue("ec_bst_abort_fault_connector_temp", dst_p.bst_abort_fault_connector_temp);
    pm.updateCPropertyValue("ec_bst_abort_fault_insulation", dst_p.bst_abort_fault_insulation);
    pm.updateCPropertyValue("ec_bst_abort_fault_other", dst_p.bst_abort_fault_other);
    pm.updateCPropertyValue("ec_bst_abort_fault_voltage", dst_p.bst_abort_fault_voltage);
    pm.updateCPropertyValue("ec_bst_abort_fault_hv", dst_p.bst_abort_fault_hv);
    pm.updateCPropertyValue("ec_bst_abort_fault_battery_temp", dst_p.bst_abort_fault_battery_temp);
    pm.updateCPropertyValue("ec_bst_abort_error_voltage", dst_p.bst_abort_error_voltage);
    pm.updateCPropertyValue("ec_bst_abort_error_current", dst_p.bst_abort_error_current);
    pm.setCan30Cupdate();
}

void a_me11_cmx_v03_bms_cst_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_cst_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_cst_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_bms_cst_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.cst_abort_by_bms);
    }
    pm.updateCPropertyValue("ec_cst_abort_by_bms", dst_p.cst_abort_by_bms);
    pm.updateCPropertyValue("ec_cst_abort_by_fault", dst_p.cst_abort_by_fault);
    pm.updateCPropertyValue("ec_cst_abort_by_user", dst_p.cst_abort_by_user);
    pm.updateCPropertyValue("ec_cst_abort_achive_condition", dst_p.cst_abort_achive_condition);
    pm.updateCPropertyValue("ec_cst_abort_fault_energe_transmit", dst_p.cst_abort_fault_energe_transmit);
    pm.updateCPropertyValue("ec_cst_abort_fault_charger_inner_temp", dst_p.cst_abort_fault_charger_inner_temp);
    pm.updateCPropertyValue("ec_cst_abort_fault_connetor", dst_p.cst_abort_fault_connetor);
    pm.updateCPropertyValue("ec_cst_abort_fault_charger_temp", dst_p.cst_abort_fault_charger_temp);
    pm.updateCPropertyValue("ec_cst_abort_fault_other", dst_p.cst_abort_fault_other);
    pm.updateCPropertyValue("ec_cst_abort_fault_charger_stop", dst_p.cst_abort_fault_charger_stop);
    pm.updateCPropertyValue("ec_cst_abort_error_voltage", dst_p.cst_abort_error_voltage);
    pm.updateCPropertyValue("ec_cst_abort_error_current", dst_p.cst_abort_error_current);
}

void a_me11_cmx_v03_ipu_obc_2_inlet_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_obc_2_inlet_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_obc_2_inlet_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_obc_2_inlet_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.ipu_device_int_temp);
    }
    pm.updateCPropertyValue("ec_ipu_device_int_temp", dst_p.ipu_device_int_temp);
    pm.updateCPropertyValue("ec_ipu_obc_temp_inlet", dst_p.ipu_obc_temp_inlet);
    pm.updateCPropertyValue("ec_ipu_obc_ac_max_pw_allow", dst_p.ipu_obc_ac_max_pw_allow);
    pm.updateCPropertyValue("ec_ipu_obc_inlet_electronic_lock", dst_p.ipu_obc_inlet_electronic_lock);
    pm.updateCPropertyValue("ec_ipu_obc_s2_state", dst_p.ipu_obc_s2_state);
    pm.updateCPropertyValue("ec_ipu_obc_wake_up_mode", dst_p.ipu_obc_wake_up_mode);
    pm.updateCPropertyValue("ec_ipu_obc_voltage_ok_l1", dst_p.ipu_obc_voltage_ok_l1);
    pm.updateCPropertyValue("ec_ipu_obc_voltage_ok_l2", dst_p.ipu_obc_voltage_ok_l2);
    pm.updateCPropertyValue("ec_ipu_obc_voltage_ok_l3", dst_p.ipu_obc_voltage_ok_l3);
}

void a_me11_cmx_v03_ipu_obc_4_state_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_obc_4_state_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_obc_4_state_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_ipu_obc_4_state_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.ipu_obc_uac_act_l1);
    }
    pm.updateCPropertyValue("ec_ipu_obc_uac_act_l1", dst_p.ipu_obc_uac_act_l1);
    pm.updateCPropertyValue("ec_ipu_obc_uac_act_l2", dst_p.ipu_obc_uac_act_l2);
    pm.updateCPropertyValue("ec_ipu_obc_uac_act_l3", dst_p.ipu_obc_uac_act_l3);
    pm.updateCPropertyValue("ec_ipu_obc_iac_act_l1", dst_p.ipu_obc_iac_act_l1);
    pm.updateCPropertyValue("ec_ipu_obc_iac_act_l2", dst_p.ipu_obc_iac_act_l2);
    pm.updateCPropertyValue("ec_ipu_obc_iac_act_l3", dst_p.ipu_obc_iac_act_l3);
}

void a_me11_cmx_v03_nm_asr_ccu_bac_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm) {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_nm_asr_ccu_bac_t dst_p;
    std::string value;
        me11_cmx_baccanfd_ots_car_v02_07_20240520_nm_asr_ccu_bac_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(me11_cmx_baccanfd_ots_car_v02_07_20240520_nm_asr_ccu_bac_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("%s %d %d", __func__,__LINE__, dst_p.ccu_keep_awake_sta);
    }
    pm.updateCPropertyValue("ec_ccu_keep_awake_sta", dst_p.ccu_keep_awake_sta);
}

const std::unordered_map<uint16_t, unpackFunctionPtr> FunctionMap::unpack_ev_can_ = {
    {0x0071, &a_me11_cmx_v03_vcu_10_torque_bac_unpack},
    {0x0082, &a_me11_cmx_v02_vcu_b_accl_pedal_bac_unpack},
    {0x00A1, &a_me11_cmx_v04_eps_3_lat_drv_and_eps_sts_unpack},
    {0x00A2, &a_me11_cmx_v04_eps_2_str_whl_ang_unpack},
    {0x00F1, &a_me11_cmx_v02_inv_1_value_bac_unpack},
    {0x00F2, &a_me11_cmx_v02_inv_2_value_bac_unpack},
    {0x00F4, &a_me11_cmx_v02_inv_4_value_bac_unpack},
    {0x0106, &a_me11_cmx_v01_vcu_disp_info_unpack},
    {0x011D, &a_me11_cmx_v01_bms_1_main_state_bac_unpack},
    {0x0120, &a_me11_cmx_v03_esc_a_bac_unpack},
    {0x0125, &a_me11_cmx_v01_esc_7_func_status_unpack},
    {0x012A, &a_me11_cmx_v01_bms_2_bat_state_unpack},
    {0x012F, &a_me11_cmx_v01_icu_info_bac_unpack},
    {0x0138, &a_me11_cmx_v03_ehb_b_bac_unpack},
    {0x013A, &a_me11_cmx_v03_bms_3_dc_charge_state_bac_unpack},
    {0x0145, &a_me11_cmx_v03_bms_4_ac_charge_state_bac_unpack},
    {0x0150, &a_me11_cmx_v01_eps_1_status_unpack},
    {0x015A, &a_me11_cmx_v01_bms_10_dc_charge_states_bac_unpack},
    {0x0165, &a_me11_cmx_v01_ehb_a_unpack},
    {0x016A, &a_me11_cmx_v02_scs_ri_switch_sts_bac_unpack},
    {0x0174, &a_me11_cmx_v02_fcm_b_bac_unpack},
    {0x0175, &a_me11_cmx_v01_fcm_a_bac_unpack},
    {0x0180, &a_me11_cmx_v01_srs_1_status_bac_unpack},
    {0x0186, &a_me11_cmx_v01_pp_inform_and_status_bac_unpack},
    {0x0193, &a_me11_cmx_v02_crrr_a_bac_unpack},
    {0x01A0, &a_me11_cmx_v01_vcu_6_drive_state_unpack},
    {0x01B6, &a_me11_cmx_v03_vcu_d_status_bac_unpack},
    {0x01BA, &a_me11_cmx_v03_bms_5_bat_temp_bac_unpack},
    {0x01F8, &a_me11_cmx_v01_ac_ac_statuts_bac_unpack},
    {0x01FA, &a_me11_cmx_v02_scs_le_switch_sts_bac_unpack},
    {0x021A, &a_me11_cmx_v01_bcm_b_package_unpack},
    {0x021D, &a_me11_cmx_v03_bms_6_dischrg_recup_bac_unpack},
    {0x022A, &a_me11_cmx_v03_ipu_obc_3_cp_cc_bac_unpack},
    {0x022E, &a_me11_cmx_v03_bms_7_cell_volt_bac_unpack},
    {0x0230, &a_me11_cmx_v03_ipu_dcc_1_state_bac_unpack},
    {0x0238, &a_me11_cmx_v01_ipu_obc_1_state_bac_unpack},
    {0x0239, &a_me11_cmx_v03_bms_8_temp_volt_limit_bac_unpack},
    {0x026E, &a_me11_cmx_v01_icu_2_odo_unpack},
    {0x0271, &a_me11_cmx_v03_bms_9_batt_info_bac_unpack},
    {0x0280, &a_me11_cmx_v03_ipu_dcc_2_chrg_in_value_bac_unpack},
    {0x029F, &a_me11_cmx_v01_ac_3_state_bac_unpack},
    {0x02B8, &a_me11_cmx_v02_fcm_23_obj0109_bac_unpack},
    {0x02BE, &a_me11_cmx_v02_fcm_24_obj1018_bac_unpack},
    {0x02BF, &a_me11_cmx_v02_fcm_25_line123_bac_unpack},
    {0x02FD, &a_me11_cmx_v03_bms_hmi_bac_unpack},
    {0x030A, &a_me11_cmx_v03_bms_bem_bac_unpack},
    {0x030B, &a_me11_cmx_v03_bms_cem_bac_unpack},
    {0x030C, &a_me11_cmx_v03_bms_bst_bac_unpack},
    {0x030D, &a_me11_cmx_v03_bms_cst_bac_unpack},
    {0x0311, &a_me11_cmx_v02_avas_state_bac_unpack},
    {0x0329, &a_me11_cmx_v03_ipu_obc_2_inlet_bac_unpack},
    {0x032A, &a_me11_cmx_v03_ipu_obc_4_state_bac_unpack},
    {0x0332, &a_me11_cmx_v02_tpms_info_bac_unpack},
    {0x03FC, &a_me11_cmx_v02_bms_a_gb_bac_unpack},
    {0x03FD, &a_me11_cmx_v02_bms_b_gb_bac_unpack},
    {0x03FE, &a_me11_cmx_v02_bms_c_gb_bac_unpack},
    {0x03FF, &a_me11_cmx_v02_bms_d_gb_bac_unpack},
    {0x0421, &a_me11_cmx_v03_nm_asr_ccu_bac_unpack},
};

//body_can
const std::unordered_map<uint16_t, unpackFunctionPtr> FunctionMap::unpack_body_can_ = {
};

const std::unordered_map<uint16_t, unpackFunctionPtr> FunctionMap::unpack_chassise_can_ = {
};

//mcu
void a_mcu_info_001_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        pm.updateCPropertyValue("mc_low_vol_battery_vol", -1024);
        pm.updateCPropertyValue("mc_main_seat_safety_belt_status", -1024);
        LOG(INFO) << "mcu_info_001_unpack_time reset done";
        return;
    }
    struct mcu_info_001_t dst_p;
    std::string value;
	mcu_info_001_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(mcu_info_001_t));
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("hu0506 mcu_steering_info_002_unpack lamplet_status:%x, brake_level_alarm_status:%x, main_seat_pressure_status:%x, main_seat_safety_belt_status:%x,engine_status:%x", 
    dst_p.lamplet_status, dst_p.brake_level_alarm_status, dst_p.main_seat_pressure_status, dst_p.main_seat_safety_belt_status, dst_p.engine_status);
    }
    pm.updateCPropertyValue("mc_lamplet_status", dst_p.lamplet_status);
    pm.updateCPropertyValue("mc_brake_level_alarm_status", dst_p.brake_level_alarm_status);
    pm.updateCPropertyValue("mc_main_seat_pressure_status", dst_p.main_seat_pressure_status);
    pm.updateCPropertyValue("mc_main_seat_safety_belt_status", dst_p.main_seat_safety_belt_status);
    pm.updateCPropertyValue("mc_engine_status", dst_p.engine_status);
    pm.updateCPropertyValue("mc_low_vol_battery_vol", dst_p.low_vol_battery_vol);
}


void a_mcu_steering_info_002_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        return;
    }
    struct mcu_steering_info_002_t dst_p;
	mcu_steering_info_002_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(mcu_steering_info_002_t));

    uint8_t key_code = message[0];
    uint8_t key_action = message[1];
    ALOGI("hu0506 mcu_steering_info_002_unpack key_code:0x%02x, action:%d", key_code, key_action);

    
    switch (key_code)
    {
    case 0xb5:
        pm.updateCPropertyValueNoCheck("mc_key_previous_song", key_action);
        break;
    case 0xb7:
        pm.updateCPropertyValueNoCheck("mc_key_next_song", key_action);
        break;
    case 0x48:
        pm.updateCPropertyValueNoCheck("mc_key_volume_up", key_action);
        break;
    case 0x4a:
        pm.updateCPropertyValueNoCheck("mc_key_volume_dowm", key_action);
        break;
    case 0x90:
        pm.updateCPropertyValueNoCheck("mc_key_mode_mute", key_action);
        break;
    case 0x52:
        pm.updateCPropertyValueNoCheck("mc_key_telephone", key_action);
        break;
    case 0x03:
        pm.updateCPropertyValueNoCheck("mc_key_home", key_action);
        break;
    case 0x11:
        pm.updateCPropertyValueNoCheck("mc_key_custom", key_action);
        break;
    case 0xc0:
        pm.updateCPropertyValueNoCheck("mc_key_voice", key_action);
        break;
    default:
        ALOGE("mcu_steering_info error key_code:0x%02x, action:%d", key_code, key_action);
        break;
    }
}

void a_mcu_headlight_control_003_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        return;
    }
    struct mcu_headlight_control_003_t dst_p;
	mcu_headlight_control_003_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(mcu_headlight_control_003_t));
    ALOGI("hu0506 mcu_headlight_control_003_unpack light_status:%x", dst_p.light_status);
    pm.updatePropertyValues();
}


void a_mcu_screen_brightness_adjust_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        return;
    }
    struct mcu_screen_brightness_adjust dst_p;
	mcu_screen_brightness_adjust_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(mcu_screen_brightness_adjust));
    ALOGI("zhao0506 mcu_screen_brightness_adjust light_level:%x", dst_p.light_level);
    pm.updateCPropertyValue("mc_light_level",dst_p.light_level);
    pm.updateCPropertyValue("mc_auto_adjust_state",dst_p.auto_adjust_state);
}


void a_mcu_appointment_chraging_006_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    struct mcu_appointment_chraging_006_t dst_p;
	mcu_appointment_chraging_006_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(mcu_appointment_chraging_006_t));
    pm.updateCPropertyValue("appointment_mode", dst_p.appointment_mode);
    pm.updateCPropertyValue("chraging_begin_time_hour", dst_p.chraging_begin_time_hour);
    pm.updateCPropertyValue("chraging_begin_time_minute", dst_p.chraging_begin_time_minute);
    pm.updateCPropertyValue("chraging_end_time_hour", dst_p.chraging_end_time_hour);
    pm.updateCPropertyValue("chraging_end_time_minute", dst_p.chraging_end_time_minute);
    pm.updateCPropertyValue("sunday_loop", dst_p.sunday_loop);
    pm.updateCPropertyValue("monday_loop", dst_p.monday_loop);
    pm.updateCPropertyValue("tuesday_loop", dst_p.tuesday_loop);
    pm.updateCPropertyValue("wednesday_loop", dst_p.wednesday_loop);
    pm.updateCPropertyValue("thursday_loop", dst_p.thursday_loop);
    pm.updateCPropertyValue("friday_loop", dst_p.friday_loop);
    pm.updateCPropertyValue("saturday_loop", dst_p.saturday_loop);
    pm.updateCPropertyValue("chraging_target_soc", dst_p.chraging_target_soc);
    // ALOGI("a_mcu_appointment_chraging_006_unpack appointment_mode:%d, chraging_begin_time_hour:%d, chraging_begin_time_minute:%d, chraging_end_time_hour:%d, chraging_end_time_minute:%d", 
    // dst_p.appointment_mode , dst_p.chraging_begin_time_hour, dst_p.chraging_begin_time_minute, dst_p.chraging_end_time_hour, dst_p.chraging_end_time_minute);
    // ALOGI("a_mcu_appointment_chraging_006_unpack sunday_loop:%d, monday_loop:%d, tuesday_loop:%d, wednesday_loop:%d, thursday_loop:%d, friday_loop:%d, saturday_loop:%d, chraging_target_soc:%d", 
    // dst_p.sunday_loop , dst_p.monday_loop, dst_p.tuesday_loop, dst_p.wednesday_loop, dst_p.thursday_loop, dst_p.friday_loop, dst_p.saturday_loop, dst_p.chraging_target_soc);

}

// a_mcu_appointment_chraging_006_unpack
// void a_mcu_fm_channel_adjust_00B_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
//     struct mcu_fm_channel_adjust dst_p;
//     mcu_fm_channel_adjust_unpack(&dst_p,message.data(),message.size());
//     memcpy(data, &dst_p, sizeof(mcu_fm_channel_adjust));
//     //pm.updateCPropertyValue("channel_function",dst_p.channel_function);
//     //pm.updateCPropertyValue("radio_station",dst_p.radio_station);
// }


// void a_mcu_fm_list_0013_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
//     struct mcu_fm_list_0013 dst_p;
//     mcu_fm_list_0013_unpack(&dst_p,message.data(),message.size());
//     memcpy(data, &dst_p, sizeof(mcu_fm_channel_adjust));
//     //pm.updateCPropertyValue("channel_total",dst_p.channel_total);
//     //pm.updateCPropertyValue("current_frame_number",dst_p.current_frame_number);
//     // if(0 < static_cast<int>(dst_p.channel_data[0])){
//     //     pm.updateCPropertyValue("fm_return_channel_one",dst_p.channel_data[0]);
//     // }
//     // if(0 < static_cast<int>(dst_p.channel_data[1])){
//     //     pm.updateCPropertyValue("fm_return_channel_two",dst_p.channel_data[1]);
//     // }
//     // if(0 < static_cast<int>(dst_p.channel_data[2])){
//     //     pm.updateCPropertyValue("fm_return_channel_three",dst_p.channel_data[2]);
//     // }
// }

//mcu
void a_mcu_soc_power_status_00d_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
//        pm.updateCPropertyValue("mc_low_vol_battery_vol", 0);
        LOG(INFO) << "dxy a_mcu_soc_power_status_00d_unpack is invalid data";  
        return;
    }
    struct mcu_soc_power_status dst_p;
    // std::string value;
	mcu_soc_power_status_00d__unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(mcu_info_001_t));
    ALOGI("dxy-- mcu_soc_power_status mcu_power_status:%x, soc_power_status:%x", dst_p.mcu_power_status, dst_p.soc_power_status);
    pm.updateCPropertyValueNoCheck("gecko_bcm_mcu_power_status", dst_p.mcu_power_status);
    pm.updateCPropertyValueNoCheck("gecko_ccm_soc_power_status", dst_p.soc_power_status);
}

void a_muc_volume_00a_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        return;
    }
    struct muc_volume_00a_t dst_p;
	muc_volume_00a_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(muc_volume_00a_t));
    ALOGI("a_muc_volume_00a_unpack light_status:%x", dst_p.mcu_voiume_navigation);
    pm.updateCPropertyValue("gecko_mcu_voiume_navigation", dst_p.mcu_voiume_navigation);
    pm.updateCPropertyValue("gecko_mcu_voiume_voic", dst_p.mcu_voiume_voic);
    pm.updateCPropertyValue("gecko_mcu_voiume_multimedia", dst_p.mcu_voiume_multimedia);
    pm.updateCPropertyValue("gecko_mcu_voiume_phone", dst_p.mcu_voiume_phone);
    pm.updateCPropertyValue("gecko_mcu_voiume_alarm", dst_p.mcu_voiume_alarm);
    pm.updateCPropertyValue("gecko_mcu_voiume_key", dst_p.mcu_voiume_key);
}

void a_mcu_info_vin_0108_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        LOG(INFO) << "mcu_info_vin_0108_unpack_time reset done";
        return;
    }
    struct mcu_info_vin_0108_t dst_p;
    std::string value;
        mcu_info_vin_0108_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(mcu_info_vin_0108_t));
    std::vector<char> vector_of_chars(dst_p.mcu_info_vin1, dst_p.mcu_info_vin1 + sizeof(dst_p.mcu_info_vin1) -1 );
    std::string string_result(vector_of_chars.begin(), vector_of_chars.end());
    pm.updatePropertyValue("mc_mcu_info_vin1", string_result);
}

void a_mcu_info_vin_0208_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        LOG(INFO) << "mcu_info_vin_0208_unpack_time reset done";
        return;
    }
    struct mcu_info_vin_0208_t dst_p;
    std::string value;
        mcu_info_vin_0208_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(mcu_info_vin_0208_t));
    std::vector<char> vector_of_chars(dst_p.mcu_info_vin2, dst_p.mcu_info_vin2 + sizeof(dst_p.mcu_info_vin2) - 1);
    std::string string_result(vector_of_chars.begin(), vector_of_chars.end());
    pm.updatePropertyValue("mc_mcu_info_vin2", string_result);
}

void a_mcu_info_vin_0308_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        LOG(INFO) << "mcu_info_vin_0308_unpack_time reset done";
        return;
    }
    struct mcu_info_vin_0308_t dst_p;
    std::string value;
        mcu_info_vin_0308_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(mcu_info_vin_0308_t));
    std::vector<char> vector_of_chars(dst_p.mcu_info_vin3, dst_p.mcu_info_vin3 + sizeof(dst_p.mcu_info_vin3) - 6);
    std::string string_result(vector_of_chars.begin(), vector_of_chars.end());
    pm.updatePropertyValue("mc_mcu_info_vin3", string_result);
}

void a_mcu_info_sn_010D_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        LOG(INFO) << "mcu_info_sn_010D_unpack_time reset done";
        return;
    }
    struct mcu_info_sn_010D_t dst_p;
    std::string value;
        mcu_info_sn_010D_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(mcu_info_sn_010D_t));
    std::vector<char> vector_of_chars(dst_p.mcu_info_sn1, dst_p.mcu_info_sn1 + sizeof(dst_p.mcu_info_sn1) -1);
    std::string string_result(vector_of_chars.begin(), vector_of_chars.end());
    pm.updatePropertyValue("mc_mcu_info_sn1", string_result);
}

void a_mcu_info_sn_020D_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        LOG(INFO) << "mcu_info_sn_020D_unpack_time reset done";
        return;
    }
    struct mcu_info_sn_020D_t dst_p;
    std::string value;
        mcu_info_sn_020D_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(mcu_info_sn_020D_t));
    std::vector<char> vector_of_chars(dst_p.mcu_info_sn2, dst_p.mcu_info_sn2 + sizeof(dst_p.mcu_info_sn2) - 1);
    std::string string_result(vector_of_chars.begin(), vector_of_chars.end());
    pm.updatePropertyValue("mc_mcu_info_sn2", string_result);
}

void a_mcu_info_sn_030D_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        LOG(INFO) << "mcu_info_sn_030D_unpack_time reset done";
        return;
    }
    struct mcu_info_sn_030D_t dst_p;
    std::string value;
        mcu_info_sn_030D_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(mcu_info_sn_030D_t));
    std::vector<char> vector_of_chars(dst_p.mcu_info_sn3, dst_p.mcu_info_sn3 + sizeof(dst_p.mcu_info_sn3) - 1);
    std::string string_result(vector_of_chars.begin(), vector_of_chars.end());
    pm.updatePropertyValue("mc_mcu_info_sn3", string_result);
}

void a_mcu_info_version_000F_unpack(std::vector<uint8_t>& message, void *data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        LOG(INFO) << "mcu_info_version_000F_unpack_time reset done";
        return;
    }
    struct mcu_info_version_000F_t dst_p;
    std::string value;
        mcu_info_version_000F_unpack(&dst_p,message.data(),message.size());
    memcpy(data, &dst_p, sizeof(mcu_info_version_000F_t));
    std::vector<char> vector_of_chars(dst_p.mcu_info_version, dst_p.mcu_info_version + sizeof(dst_p.mcu_info_version));
    std::string string_result(vector_of_chars.begin(), vector_of_chars.end());
    LOG(INFO) << "mcu_info_version is " << string_result;
    pm.updatePropertyValue("mc_mcu_info_version", string_result);
}

void a_mcu_trav_info_long_0AF1_unpack(std::vector<uint8_t>& message, void* data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        LOG(INFO) << "a_mcu_trav_info_long_0AF1_unpack err data reset done";
        return;
    }
    struct mcu_trav_info_t dst_p;
    std::string value;
    mcu_trav_info_unpack(&dst_p,message.data(),message.size());
    LOG(INFO) << "mcu_trav_info_long_unpack maleage: "<<dst_p.maleage
              <<" time: "<<dst_p.time
              <<" speed: "<<dst_p.speed
              <<" avgengc: "<<dst_p.avgengc;
    memcpy(data, &dst_p, sizeof(mcu_trav_info_t));
    pm.updateCPropertyValue("mc_travel_range", dst_p.maleage);
    pm.updateCPropertyValue("mc_travel_time", dst_p.time);
    pm.updateCPropertyValue("mc_travel_average_speed", dst_p.speed);
    pm.updateCPropertyValue("mc_average_energy_consumption", dst_p.avgengc);
}
void a_mcu_trav_info_short_0AF2_unpack(std::vector<uint8_t>& message, void* data, PropertyManager& pm){
    if(!is_valid_data(message)) {
        LOG(INFO) << "a_mcu_trav_info_long_0AF1_unpack err data reset done";
        return;
    }
    struct mcu_trav_info_t dst_p;
    std::string value;
    mcu_trav_info_unpack(&dst_p,message.data(),message.size());
    LOG(INFO) <<"mcu_trav_info_short_unpack maleage: "<<dst_p.maleage
              <<" time: "<<dst_p.time
              <<" speed: "<<dst_p.speed
              <<" avgengc: "<<dst_p.avgengc;
    memcpy(data, &dst_p, sizeof(mcu_trav_info_t));
    pm.updateCPropertyValue("mcu_short_travel_mileage", dst_p.maleage);
    pm.updateCPropertyValue("mcu_short_travel_time", dst_p.time);
    pm.updateCPropertyValue("mcu_short_average_spd", dst_p.speed);
    pm.updateCPropertyValue("mcu_short_average_consum", dst_p.avgengc);
    //mHal->onPropertyValue(*Create_power_on_average_speedReq(-1024.0),true);
}

const std::unordered_map<uint16_t, unpackFunctionPtr> FunctionMap::unpack_mcu_ = {
    {0x0001, &a_mcu_info_001_unpack},
    {0x0002, &a_mcu_steering_info_002_unpack},
    {0x0003, &a_mcu_headlight_control_003_unpack},
    {0x0006, &a_mcu_appointment_chraging_006_unpack},
    {0x0009, &a_mcu_screen_brightness_adjust_unpack},
    //{0x00B,  &a_mcu_fm_channel_adjust_00B_unpack},
    //{0x0013, &a_mcu_fm_list_0013_unpack},
    {0x00D,  &a_mcu_soc_power_status_00d_unpack},
    {0x00A,  &a_muc_volume_00a_unpack},
    {0x0108, &a_mcu_info_vin_0108_unpack},
    {0x0208, &a_mcu_info_vin_0208_unpack},
    {0x0308, &a_mcu_info_vin_0308_unpack},
    {0x010D, &a_mcu_info_sn_010D_unpack},
    {0x020D, &a_mcu_info_sn_020D_unpack},
    {0x030D, &a_mcu_info_sn_030D_unpack},
    {0x000F, &a_mcu_info_version_000F_unpack},
    {0x0AF1, &a_mcu_trav_info_long_0AF1_unpack}, //旅行信息长期记忆
    {0x0AF2, &a_mcu_trav_info_short_0AF2_unpack}, //旅行信息短期记忆
};


// 根据ID调用相应的函数
void FunctionMap::unpackcallFunction(uint16_t id,std::vector<uint8_t>& dataContent, void *data, BusIdentifier busIdentifier,PropertyManager& pm) {
    switch(busIdentifier){
        case BusIdentifier::BODY_CAN:
            {
                RecordData::getInstance()->recordBody_CAN(dataContent);
                auto it = unpack_body_can_.find(id);
                if (it != unpack_body_can_.end()) {
                    unpackFunctionPtr function = it->second;
                    function(dataContent, data,pm);
                } else
                {
                    ALOGE("BODY_CAN unpackca err, id:%x not found!!", id);
                }
                break;
            }
        case BusIdentifier::EV_CAN:
            {
                RecordData::getInstance()->recordEVCAN(dataContent);
                auto it = unpack_ev_can_.find(id);
                if (it != unpack_ev_can_.end()) {
                    unpackFunctionPtr function = it->second;
                    function(dataContent, data,pm);
                } else
                {
                    ALOGE("EV_CAN unpackca err, id:%x not found!!", id);
                }
                break;
            }
        case BusIdentifier::CH_CAN:
            {
                RecordData::getInstance()->recordChassis_CAN(dataContent);
                auto it = unpack_chassise_can_.find(id);
                if (it != unpack_chassise_can_.end()) {
                    unpackFunctionPtr function = it->second;
                    function(dataContent, data,pm);
                } else
                {
                    ALOGE("CH_CAN unpackca err, id:%x not found!!", id);
                }
                break;
            }
        case BusIdentifier::MCU:
            {
                RecordData::getInstance()->recordMCU(dataContent);
                auto it = unpack_mcu_.find(id);
                if (it != unpack_mcu_.end()) {
                    unpackFunctionPtr function = it->second;
                    function(dataContent, data,pm);
                } else
                {
                    ALOGE("MCU unpackca err, id:%x not found!!", id);
                }
                break;
            }
        default:  ALOGE("unpackca err, Wrong  BUS id:%d", static_cast<int>(busIdentifier));;
    }
}

}
