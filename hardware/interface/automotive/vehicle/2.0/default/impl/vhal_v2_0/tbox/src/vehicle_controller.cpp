#include "vehicle_controller.h"
#include <android-base/logging.h>
#include <set>
#include "tinyxml2.h"

/*------------------------------------------------------
左值代表车机服务侧值
右值代表tbox服务需要的值
--------------------------------------------------------*/

std::vector<Item> VehicleController::mItemList;

VehicleController::VehicleController() {
    mVehicle = IVehicle::getService();
}
VehicleController::VehicleController(LockMap<int32_t, TboxDataDeclaration>& tboxdecl) {
    mVehicle = IVehicle::getService();
    Start(tboxdecl);
}
void VehicleController::Start(LockMap<int32_t, TboxDataDeclaration>& tboxdecl) {
//    int i = 0;
    hidl_vec<SubscribeOptions> options;
    options.resize(tboxdecl.size());
    mCallback = new VehicleCallback();
//    mCallback->mCarMessageQueue.settboxdecl(&tboxdecl);
//    mCallback->mCarMessageQueue.start();
//    for(auto & pair:tboxdecl){
//        options[i++] = pair.second.conf.opt;
//    }
//    mVehicle->subscribe(mCallback, options);
}
void VehicleController::Start() {
    tinyxml2::XMLDocument doc;
    doc.LoadFile("/vendor/etc/gecko-tbox-vehicle.xml");
    tinyxml2::XMLElement* optionsxml = doc.FirstChildElement("items");
    hidl_vec<SubscribeOptions> options;
    int i = 0;
    int propNum = 0;
    std::set<int> leftPropIds;
    for (tinyxml2::XMLElement* item = optionsxml->FirstChildElement("item"); item != nullptr;
         item = item->NextSiblingElement("item")) {
        Item itemData;
        tinyxml2::XMLElement* right = item->FirstChildElement("right");
        itemData.right_name = right->Attribute("name");
        itemData.right_areaId = static_cast<int32_t>(std::stoi(right->Attribute("areaId")));
        itemData.right_value_type = right->Attribute("type");

        tinyxml2::XMLElement* left = item->FirstChildElement("left");
        if (left != nullptr) {
            itemData.opt.propId = std::stoi(left->Attribute("propId"));
            LOG(INFO) << __func__ << __LINE__ << "--" << itemData.opt.propId;
            itemData.opt.sampleRate = std::stof(left->Attribute("frequency"));
            LOG(INFO) << __func__ << __LINE__ << "--" << itemData.opt.sampleRate;
            itemData.opt.flags = static_cast<SubscribeFlags>(std::stoi(left->Attribute("flags")));
            itemData.left_tranMode = atoi(left->FirstChildElement("tranMode")->GetText());
        }
        mItemList.push_back(itemData);
        if (leftPropIds.count(itemData.opt.propId) == 0) {
            propNum++;
            leftPropIds.insert(itemData.opt.propId);
        }
    }
    options.resize(propNum);

    // mVehicle = IVehicle::getService();
    mCallback = new VehicleCallback();
    //  hidl_vec<SubscribeOptions> options = {SubscribeOptions{.propId =
    //  static_cast<int>(VehicleProperty::PERF_VEHICLE_SPEED), 100.0, .flags =
    //  SubscribeFlags::EVENTS_FROM_CAR},
    //                                        SubscribeOptions{.propId =
    //                                        static_cast<int>(VehicleProperty::HVAC_TEMPERATURE_CURRENT),
    //                                        100.0, .flags = SubscribeFlags::EVENTS_FROM_CAR},};
    // 先将所有数据初值完成更新,再添加对应的订阅
    std::set<int> propIds;
    for (auto& it : mItemList) {
        // 处理item
        if (it.opt.propId != 0) {
            LOG(INFO) << __func__ << __LINE__ << it.opt.propId;
            // left不为空,处理left部分
            StatusCode refStatus;
            VehiclePropValue refValue;
            bool isCalled = false;
            VehiclePropValue requestedValue{};
            requestedValue.prop = (int)it.opt.propId;
            requestedValue.areaId = it.right_areaId;
            mVehicle->get(requestedValue,
                          [&refStatus, &refValue, &isCalled](StatusCode status,
                                                             const VehiclePropValue& value) {
                              refStatus = status;
                              refValue = value;
                              isCalled = true;
                          });
            LOG(INFO) << __func__ << __LINE__;
            if (sizeof(refValue.value) != 0) {
                it.right_value = left_to_right(it.right_value_type, refValue);
                if (propIds.count(it.opt.propId) == 0) {
                    // propId not repeat
                    options[i++] = it.opt;
                    propIds.insert(it.opt.propId);
                } else {
                    //std::cout << "yyy:options repeat,do not add" << std::endl;
                    LOG(INFO) << "yyy:options repeat,do not add";
                }
            }
        }
    }
    LOG(INFO) << __func__ << __LINE__ << options.size();
    mVehicle->subscribe(mCallback, options);
}

std::string VehicleController::left_to_right(std::string& value_type, VehiclePropValue& refvalue) {
    std::string msg;
    if (value_type == "float") {
        msg = toString(refvalue.value.floatValues[0]);
    } else if (value_type == "int") {
        msg = toString(refvalue.value.int32Values[0]);
    }
    return msg;
}
/*
void VehicleController::subscribeToProperties(const std::vector<int>& properties) {
  // 实现订阅属性
  std::cout << properties << std::endl;
}*/

void VehicleController::subscribeToProperty(int propID) {
    // 实现订阅属性
    LOG(INFO) << "subscribeToProperty";
    mCallback = new VehicleCallback();
    hidl_vec<SubscribeOptions> option = {
        SubscribeOptions{.propId = propID, 100.0, .flags = SubscribeFlags::EVENTS_FROM_CAR}};
    mVehicle->subscribe(mCallback, option);
}

void VehicleController::setProperty(const VehiclePropValue& value) {
    mVehicle->set(value);
}
void VehicleController::getPropertybyPropId(int propId,int areaId,VehiclePropValue& refValue,StatusCode& refStatus) {
    bool isCalled = false;
    VehiclePropValue requestedValue{};
    requestedValue.areaId = areaId;
    requestedValue.prop = propId;
    mVehicle->get(requestedValue, [&refStatus, &refValue, &isCalled](
                                          StatusCode status, const VehiclePropValue& value) {
        refStatus = status;
        refValue = value;
        isCalled = true;
    });
}
std::string VehicleController::getPropertybyPropId(int propId) {
    int32_t areaId = 0;
    std::string value_type;
    for (const auto& item : mItemList) {
        if (item.opt.propId == propId) {
            areaId = item.right_areaId;
            value_type = item.right_value_type;
        }
    }
    StatusCode refStatus;
    VehiclePropValue refValue;
    bool isCalled = false;
    VehiclePropValue requestedValue{};
    requestedValue.areaId = areaId;
    requestedValue.prop = propId;
    mVehicle->get(requestedValue, [&refStatus, &refValue, &isCalled](
                                          StatusCode status, const VehiclePropValue& value) {
        refStatus = status;
        refValue = value;
        isCalled = true;
    });
    std::string propIdValue;
    if (value_type == "int") {
        for (size_t i = 0; i < refValue.value.int32Values.size(); i++) {
            propIdValue[i * 2] = toString(refValue.value.int32Values[i])[0];
            propIdValue[i * 2 + 1] = toString(refValue.value.int32Values[i])[1];
        }
    } else if (value_type == "float") {
        propIdValue = toString(refValue.value.floatValues[0]);
    } else {
        propIdValue = "00";
    }
    //std::cout << "yyy:getvalue:" << propIdValue << std::endl;
    LOG(INFO) << "yyy:getvalue:" << propIdValue;
    for (auto& item : mItemList) {
        if (item.opt.propId == propId) {
            item.right_value = propIdValue;
        }
    }
    return propIdValue;
}

int VehicleController::getRightValueByPropId(int propId) {
    for (const auto& item : mItemList) {
        if (item.opt.propId == propId) {
            return std::stoi(item.right_value);
        }
    }
    return -1;
}

int32_t VehicleController::getareaIdByPropId(int propId) {
    for (const auto& item : mItemList) {
        if (item.opt.propId == propId) {
            return item.right_areaId;
        }
    }
    return -1;
}