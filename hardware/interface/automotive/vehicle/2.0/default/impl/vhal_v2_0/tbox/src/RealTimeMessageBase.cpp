#include "RealTimeMessageBase.h"
#include <android-base/logging.h>
#include <iostream>

RealTimeMessageBase::RealTimeMessageBase(LockMap<int32_t, TboxDataDeclaration>& tboxdecl,
                                       VehicleController& vehicle) {
    //LOG(INFO) << tboxdecl[0].tvalue.type;
    mTboxdecl = &tboxdecl;
    mVehicle = vehicle;
}

uint32_t RealTimeMessageBase::tboxReturnValue(int propId){
    uint32_t value = 0;
    if((*mTboxdecl)[propId].conf.tbox_value_size==1){
        value = 0xff;
    } else if((*mTboxdecl)[propId].conf.tbox_value_size==2){
        value = 0xffff;
    } else if((*mTboxdecl)[propId].conf.tbox_value_size==4){
        value = 0xffffffff;
    } else {
        value = 0xff;
    }
    return value;
}
uint32_t RealTimeMessageBase::getValue(int propId) {
    StatusCode refStatus;
    VehiclePropValue refValue;
    uint32_t value = 0;
    double temp_value = 0;
    //LOG(INFO) << __func__ << __LINE__ << propId << "-" << (*mTboxdecl)[propId].conf.areaid;
    mVehicle.getPropertybyPropId(propId, (*mTboxdecl)[propId].conf.areaid, refValue, refStatus);
    if (refStatus != StatusCode::OK) {
        value = tboxReturnValue(propId);
    }
    if((*mTboxdecl)[propId].conf.type=="int32") {
        if((*mTboxdecl)[propId].conf.TboxRflex.size()>=1){
            // 使用 find 查找元素
            auto it = (*mTboxdecl)[propId].conf.TboxRflex.find(refValue.value.int32Values[0]);
            if (it != (*mTboxdecl)[propId].conf.TboxRflex.end()) {
                value = atoi((*mTboxdecl)[propId].conf.TboxRflex[refValue.value.int32Values[0]].c_str());
                //LOG(INFO) << "get " << propId << " tbox value:"
                          //<< (*mTboxdecl)[propId].conf.TboxRflex[refValue.value.int32Values[0]];
            } else {
                //LOG(INFO) << "Out of Range error: ";
                value = 0xff;
            }
        } else {
            if(refValue.value.int32Values[0] == -1024){
                value = tboxReturnValue(propId);
            } else {
//            value = refValue.value.int32Values[0] * (*mTboxdecl)[propId].conf.tbox_value_muls + (*mTboxdecl)[propId].conf.tbox_value_offset;
                temp_value = refValue.value.int32Values[0] * (*mTboxdecl)[propId].conf.prop_res + (*mTboxdecl)[propId].conf.prop_offset;
                //LOG(INFO) << "temp_value: " << temp_value;
                temp_value = (temp_value -  (*mTboxdecl)[propId].conf.tbox_value_offset) *  (*mTboxdecl)[propId].conf.tbox_value_muls;
                //LOG(INFO) << "temp_value: " << temp_value;
                if((temp_value < (*mTboxdecl)[propId].conf.tbox_value_min)||(temp_value > (*mTboxdecl)[propId].conf.tbox_value_max)) {
                    value = tboxReturnValue(propId);
                } else {
                    value = temp_value;
                }
            }
            //LOG(INFO) << "value: " << value;
        }

    } else if ((*mTboxdecl)[propId].conf.type=="float") {
        if(refValue.value.floatValues[0] == -1024){
            value = tboxReturnValue(propId);
        } else {
//        value = (int)(refValue.value.floatValues[0] * (*mTboxdecl)[propId].conf.tbox_value_muls + (*mTboxdecl)[propId].conf.tbox_value_offset);
            temp_value = refValue.value.floatValues[0] * (*mTboxdecl)[propId].conf.prop_res + (*mTboxdecl)[propId].conf.prop_offset;
            //LOG(INFO) << __LINE__ << " temp_value: " << temp_value;
            temp_value = (temp_value -  (*mTboxdecl)[propId].conf.tbox_value_offset) *  (*mTboxdecl)[propId].conf.tbox_value_muls;
            //LOG(INFO) << __LINE__ << " temp_value: " << temp_value;
            value = static_cast<uint32_t>(std::round(temp_value));
            //value = temp_value;
            //LOG(INFO) << __LINE__ << " value: " << value;
            }
            if((value < (*mTboxdecl)[propId].conf.tbox_value_min)||(value > (*mTboxdecl)[propId].conf.tbox_value_max)) {
                value = tboxReturnValue(propId);
            }
        }
    //LOG(INFO) << __func__ << __LINE__ << ": " << value;
    return value;
}


uint32_t RealTimeMessageBase::getRawValue(int propId) {
    StatusCode refStatus;
    VehiclePropValue refValue;
    uint32_t value = 0;
    double temp_value = 0;
    //LOG(INFO) << __func__ << __LINE__ << propId << "-" << (*mTboxdecl)[propId].conf.areaid;
    mVehicle.getPropertybyPropId(propId, (*mTboxdecl)[propId].conf.areaid, refValue, refStatus);
    if (refStatus != StatusCode::OK) {
        value = tboxReturnValue(propId);
    }
    if((*mTboxdecl)[propId].conf.type=="int32") {
        temp_value = refValue.value.int32Values[0] * (*mTboxdecl)[propId].conf.prop_res + (*mTboxdecl)[propId].conf.prop_offset;
        //LOG(INFO) << "temp_value: " << temp_value;
        temp_value = (temp_value -  (*mTboxdecl)[propId].conf.tbox_value_offset) *  (*mTboxdecl)[propId].conf.tbox_value_muls;
        //LOG(INFO) << "temp_value: " << temp_value;
        value = temp_value;
        //LOG(INFO) << "value: " << value;

    } else if ((*mTboxdecl)[propId].conf.type=="float") {
        temp_value = refValue.value.floatValues[0] * (*mTboxdecl)[propId].conf.prop_res + (*mTboxdecl)[propId].conf.prop_offset;
        //LOG(INFO) << "temp_value: " << temp_value;
        temp_value = (temp_value -  (*mTboxdecl)[propId].conf.tbox_value_offset) *  (*mTboxdecl)[propId].conf.tbox_value_muls;
        //LOG(INFO) << "temp_value: " << temp_value;
        value = static_cast<uint32_t>(std::round(temp_value));
        //LOG(INFO) << "value: " << value;
    }
    //LOG(INFO) << __func__ << __LINE__ << ": " << value;
    return value;
}
uint32_t RealTimeMessageBase::getRawValue(int propId,int areaId) {
    StatusCode refStatus;
    VehiclePropValue refValue;
    uint32_t value = 0;
    double temp_value = 0;
    //LOG(INFO) << __func__ << __LINE__ << propId << "-" << areaId;
    mVehicle.getPropertybyPropId(propId, areaId, refValue, refStatus);
    if (refStatus != StatusCode::OK) {
        value = tboxReturnValue(propId);
    }
    if((*mTboxdecl)[propId].conf.type=="int32") {
        temp_value = refValue.value.int32Values[0] * (*mTboxdecl)[propId].conf.prop_res + (*mTboxdecl)[propId].conf.prop_offset;
        //LOG(INFO) << "temp_value: " << temp_value;
        temp_value = (temp_value -  (*mTboxdecl)[propId].conf.tbox_value_offset) *  (*mTboxdecl)[propId].conf.tbox_value_muls;
        //LOG(INFO) << "temp_value: " << temp_value;
        value = temp_value;
        //LOG(INFO) << "value: " << value;

    } else if ((*mTboxdecl)[propId].conf.type=="float") {
        temp_value = refValue.value.floatValues[0] * (*mTboxdecl)[propId].conf.prop_res + (*mTboxdecl)[propId].conf.prop_offset;
        //LOG(INFO) << "temp_value: " << temp_value;
        temp_value = (temp_value -  (*mTboxdecl)[propId].conf.tbox_value_offset) *  (*mTboxdecl)[propId].conf.tbox_value_muls;
        //LOG(INFO) << "temp_value: " << temp_value;
        value = static_cast<uint32_t>(std::round(temp_value));
        //LOG(INFO) << "value: " << value;
    }
    //LOG(INFO) << __func__ << __LINE__ << ": " << value;
    return value;
}

uint32_t RealTimeMessageBase::getRawPropValue(int propId) {
    StatusCode refStatus;
    VehiclePropValue refValue;
    uint32_t value = 0;
    double temp_value = 0;
    //LOG(INFO) << __func__ << __LINE__ << propId << "-" << (*mTboxdecl)[propId].conf.areaid;
    mVehicle.getPropertybyPropId(propId, (*mTboxdecl)[propId].conf.areaid, refValue, refStatus);
    if (refStatus != StatusCode::OK) {
        value = tboxReturnValue(propId);
    }
    if((*mTboxdecl)[propId].conf.type=="int32") {
        temp_value = refValue.value.int32Values[0];
        //LOG(INFO) << "temp_value: " << temp_value;
        value = temp_value;
        //LOG(INFO) << "value: " << value;
    } else if ((*mTboxdecl)[propId].conf.type=="float") {
        temp_value = refValue.value.floatValues[0];
        //LOG(INFO) << "temp_value: " << temp_value;
        value = static_cast<uint32_t>(std::round(temp_value));
        //LOG(INFO) << "value: " << value;
    }
    //LOG(INFO) << __func__ << __LINE__ << ": " << value;
    return value;
}

uint32_t RealTimeMessageBase::getRawPropFloatToIntValue(int propId,int areaId) {
    StatusCode refStatus;
    VehiclePropValue refValue;
    uint32_t value = 0;
    double temp_value = 0;
    //LOG(INFO) << __func__ << __LINE__ << propId << "-" << areaId;
    mVehicle.getPropertybyPropId(propId, areaId, refValue, refStatus);
    if (refStatus != StatusCode::OK) {
        value = tboxReturnValue(propId);
    }
    //LOG(INFO) << "prop float value: " << refValue.value.floatValues[0];
    if ((*mTboxdecl)[propId].conf.type=="float") {
        temp_value = refValue.value.floatValues[0] /(*mTboxdecl)[propId].conf.prop_res + (*mTboxdecl)[propId].conf.prop_offset;
        //LOG(INFO) << "temp_value: " << temp_value;
        value = static_cast<uint32_t>(std::round(temp_value));
        //LOG(INFO) << "value: " << value;
    }
    //LOG(INFO) << __func__ << __LINE__ << ": " << value;
    return value;
}