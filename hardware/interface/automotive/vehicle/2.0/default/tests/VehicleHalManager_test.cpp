/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <unordered_map>
#include <iostream>

#include <android-base/macros.h>
#include <cutils/native_handle.h>
#include <utils/SystemClock.h>

#include <gtest/gtest.h>

#include "vhal_v2_0/VehicleHalManager.h"

#include "VehicleHalTestUtils.h"

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

// A simple helper class to expose 'cmdSetOneProperty' to the unit tests.
class VehicleHalManagerTestHelper {
  public:
    VehicleHalManagerTestHelper(VehicleHalManager* manager) { mManager = manager; }
    bool cmdSetOneProperty(int fd, const hidl_vec<hidl_string>& options) {
        return mManager->cmdSetOneProperty(fd, options);
    }

  public:
    VehicleHalManager* mManager;
};

namespace {

using namespace std::placeholders;

constexpr char kCarMake[] = "Default Car";
constexpr int kRetriablePropMockedAttempts = 3;

class MockedVehicleHal : public VehicleHal {
public:
    MockedVehicleHal() {
        mConfigs.assign(std::begin(kVehicleProperties),
                        std::end(kVehicleProperties));
    }

    std::vector<VehiclePropConfig> listProperties() override {
        return mConfigs;
    }

    VehiclePropValuePtr get(const VehiclePropValue& requestedPropValue,
             StatusCode* outStatus) override {
        *outStatus = StatusCode::OK;
        VehiclePropValuePtr pValue;
        auto property = static_cast<VehicleProperty>(requestedPropValue.prop);
        int32_t areaId = requestedPropValue.areaId;

        if (property == VehicleProperty::INFO_FUEL_CAPACITY) {
            if (fuelCapacityAttemptsLeft-- > 0) {
                // Emulate property not ready yet.
                *outStatus = StatusCode::TRY_AGAIN;
            } else {
                pValue = getValuePool()->obtainFloat(42.42);
            }
        } else {
            auto key = makeKey(requestedPropValue);
            if (mValues.count(key) == 0) {
                ALOGW("key not found\n");
                *outStatus = StatusCode::INVALID_ARG;
                return pValue;
            }
            pValue = getValuePool()->obtain(mValues[key]);
        }

        if (*outStatus == StatusCode::OK && pValue.get() != nullptr) {
            pValue->prop = toInt(property);
            pValue->areaId = areaId;
            pValue->timestamp = elapsedRealtimeNano();
        }

        return pValue;
    }

    StatusCode set(const VehiclePropValue& propValue) override {
        if (toInt(VehicleProperty::MIRROR_FOLD) == propValue.prop
                && mirrorFoldAttemptsLeft-- > 0) {
            return StatusCode::TRY_AGAIN;
        }
        mValues[makeKey(propValue)] = propValue;
        return StatusCode::OK;
    }

    StatusCode subscribe(int32_t /* property */,
                         float /* sampleRate */) override {
        return StatusCode::OK;
    }

    StatusCode unsubscribe(int32_t /* property */) override {
        return StatusCode::OK;
    }

    StatusCode udsSendBack(const hidl_vec<uint8_t>&) override {
        return StatusCode::OK;
    }

    void sendPropEvent(recyclable_ptr<VehiclePropValue> value) {
        doHalEvent(std::move(value));
    }

    void sendHalError(StatusCode error, int32_t property, int32_t areaId) {
        doHalPropertySetError(error, property, areaId);
    }

public:
    int fuelCapacityAttemptsLeft = kRetriablePropMockedAttempts;
    int mirrorFoldAttemptsLeft = kRetriablePropMockedAttempts;

private:
    int64_t makeKey(const VehiclePropValue& v) const {
        return makeKey(v.prop, v.areaId);
    }

    int64_t makeKey(int32_t prop, int32_t area) const {
        return (static_cast<int64_t>(prop) << 32) | area;
    }

private:
    std::vector<VehiclePropConfig> mConfigs;
    std::unordered_map<int64_t, VehiclePropValue> mValues;
};

class VehicleHalManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        hal.reset(new MockedVehicleHal);
        manager.reset(new VehicleHalManager(hal.get()));

        objectPool = hal->getValuePool();
    }

    void TearDown() override {
        manager.reset(nullptr);
        hal.reset(nullptr);
    }
public:
    void invokeGet(int32_t property, int32_t areaId) {
        VehiclePropValue requestedValue {};
        requestedValue.prop = property;
        requestedValue.areaId = areaId;

        invokeGet(requestedValue);
    }

    void invokeGet(const VehiclePropValue& requestedPropValue) {
        actualValue = VehiclePropValue {};  // reset previous values

        StatusCode refStatus;
        VehiclePropValue refValue;
        bool called = false;
        manager->get(requestedPropValue, [&refStatus, &refValue, &called]
            (StatusCode status, const VehiclePropValue& value) {
            refStatus = status;
            refValue = value;
            called = true;
        });
        ASSERT_TRUE(called) << "callback wasn't called for prop: "
                            << hexString(requestedPropValue.prop);

        actualValue = refValue;
        actualStatusCode = refStatus;
    }

    MockedVehicleHal::VehiclePropValuePtr getComplexProperty() {
        auto pValue = objectPool->obtainComplex();
        pValue->prop = kCustomComplexProperty;
        pValue->areaId = 0;
        pValue->value.int32Values = hidl_vec<int32_t>{10, 20};
        pValue->value.int64Values = hidl_vec<int64_t>{30, 40};
        pValue->value.floatValues = hidl_vec<float_t>{1.1, 2.2};
        pValue->value.bytes = hidl_vec<uint8_t>{1, 2, 3};
        pValue->value.stringValue = kCarMake;
        return pValue;
    }

public:
    VehiclePropValue actualValue;
    StatusCode actualStatusCode;

    VehiclePropValuePool* objectPool;
    std::unique_ptr<MockedVehicleHal> hal;
    std::unique_ptr<VehicleHalManager> manager;
};

TEST_F(VehicleHalManagerTest, getPropConfigs) {
    hidl_vec<int32_t> properties =
        { toInt(VehicleProperty::HVAC_FAN_SPEED),
          toInt(VehicleProperty::INFO_MAKE) };
    bool called = false;

    manager->getPropConfigs(properties,
            [&called] (StatusCode status,
                       const hidl_vec<VehiclePropConfig>& c) {
        ASSERT_EQ(StatusCode::OK, status);
        ASSERT_EQ(2u, c.size());
        called = true;
    });

    ASSERT_TRUE(called);  // Verify callback received.

    called = false;
    manager->getPropConfigs({ toInt(VehicleProperty::HVAC_FAN_SPEED) },
            [&called] (StatusCode status,
                       const hidl_vec<VehiclePropConfig>& c) {
        ASSERT_EQ(StatusCode::OK, status);
        ASSERT_EQ(1u, c.size());
        ASSERT_EQ(toString(kVehicleProperties[1]), toString(c[0]));
        called = true;
    });
    ASSERT_TRUE(called);  // Verify callback received.

    // TODO(pavelm): add case case when property was not declared.
}

TEST_F(VehicleHalManagerTest, getAllPropConfigs) {
    bool called = false;
    manager->getAllPropConfigs(
            [&called] (const hidl_vec<VehiclePropConfig>& propConfigs) {
        ASSERT_EQ(arraysize(kVehicleProperties), propConfigs.size());

        for (size_t i = 0; i < propConfigs.size(); i++) {
            ASSERT_EQ(toString(kVehicleProperties[i]),
                      toString(propConfigs[i]));
        }
        called = true;
    });
    ASSERT_TRUE(called);  // Verify callback received.
}

TEST_F(VehicleHalManagerTest, halErrorEvent) {
    const auto PROP = toInt(VehicleProperty::DISPLAY_BRIGHTNESS);

    sp<MockedVehicleCallback> cb = new MockedVehicleCallback();

    hidl_vec<SubscribeOptions> options = {
        SubscribeOptions{.propId = PROP, .flags = SubscribeFlags::EVENTS_FROM_CAR},
    };

    StatusCode res = manager->subscribe(cb, options);
    ASSERT_EQ(StatusCode::OK, res);

    hal->sendHalError(StatusCode::TRY_AGAIN, PROP, 0 /* area id*/);
}

TEST_F(VehicleHalManagerTest, subscribe) {
    const auto PROP = toInt(VehicleProperty::DISPLAY_BRIGHTNESS);

    sp<MockedVehicleCallback> cb = new MockedVehicleCallback();

    hidl_vec<SubscribeOptions> options = {
        SubscribeOptions{.propId = PROP, .flags = SubscribeFlags::EVENTS_FROM_CAR}};

    StatusCode res = manager->subscribe(cb, options);
    ASSERT_EQ(StatusCode::OK, res);

    auto unsubscribedValue = objectPool->obtain(VehiclePropertyType::INT32);
    unsubscribedValue->prop = toInt(VehicleProperty::HVAC_FAN_SPEED);

    hal->sendPropEvent(std::move(unsubscribedValue));
    auto& receivedEnvents = cb->getReceivedEvents();

    ASSERT_TRUE(cb->waitForExpectedEvents(0)) << " Unexpected events received: "
                                              << receivedEnvents.size()
                                              << (receivedEnvents.size() > 0
                                                  ? toString(receivedEnvents.front()[0]) : "");

    auto subscribedValue = objectPool->obtain(VehiclePropertyType::INT32);
    subscribedValue->prop = PROP;
    subscribedValue->value.int32Values[0] = 42;

    cb->reset();
    VehiclePropValue actualValue(*subscribedValue.get());
    hal->sendPropEvent(std::move(subscribedValue));

    ASSERT_TRUE(cb->waitForExpectedEvents(1)) << "Events received: "
                                              << receivedEnvents.size();

    ASSERT_EQ(toString(actualValue),
              toString(cb->getReceivedEvents().front()[0]));
}

TEST_F(VehicleHalManagerTest, subscribe_WriteOnly) {
    const auto PROP = toInt(VehicleProperty::HVAC_SEAT_TEMPERATURE);

    sp<MockedVehicleCallback> cb = new MockedVehicleCallback();

    hidl_vec<SubscribeOptions> options = {
        SubscribeOptions{.propId = PROP, .flags = SubscribeFlags::EVENTS_FROM_CAR},
    };

    StatusCode res = manager->subscribe(cb, options);
    // Unable to subscribe on Hal Events for write-only properties.
    ASSERT_EQ(StatusCode::INVALID_ARG, res);

    options[0].flags = SubscribeFlags::EVENTS_FROM_ANDROID;

    res = manager->subscribe(cb, options);
    // OK to subscribe on SET method call for write-only properties.
    ASSERT_EQ(StatusCode::OK, res);
}

TEST_F(VehicleHalManagerTest, get_Complex) {
    ASSERT_EQ(StatusCode::OK, hal->set(*getComplexProperty().get()));

    invokeGet(kCustomComplexProperty, 0);

    ASSERT_EQ(StatusCode::OK, actualStatusCode);
    ASSERT_EQ(kCustomComplexProperty, actualValue.prop);

    ASSERT_EQ(3u, actualValue.value.bytes.size());
    ASSERT_EQ(1, actualValue.value.bytes[0]);
    ASSERT_EQ(2, actualValue.value.bytes[1]);
    ASSERT_EQ(3, actualValue.value.bytes[2]);

    ASSERT_EQ(2u, actualValue.value.int32Values.size());
    ASSERT_EQ(10, actualValue.value.int32Values[0]);
    ASSERT_EQ(20, actualValue.value.int32Values[1]);

    ASSERT_EQ(2u, actualValue.value.floatValues.size());
    ASSERT_FLOAT_EQ(1.1, actualValue.value.floatValues[0]);
    ASSERT_FLOAT_EQ(2.2, actualValue.value.floatValues[1]);

    ASSERT_EQ(2u, actualValue.value.int64Values.size());
    ASSERT_FLOAT_EQ(30, actualValue.value.int64Values[0]);
    ASSERT_FLOAT_EQ(40, actualValue.value.int64Values[1]);

    ASSERT_STREQ(kCarMake, actualValue.value.stringValue.c_str());
}

TEST_F(VehicleHalManagerTest, get_StaticString) {
    auto pValue = objectPool->obtainString(kCarMake);
    pValue->prop = toInt(VehicleProperty::INFO_MAKE);
    pValue->areaId = 0;
    ASSERT_EQ(StatusCode::OK, hal->set(*pValue.get()));

    invokeGet(toInt(VehicleProperty::INFO_MAKE), 0);

    ASSERT_EQ(StatusCode::OK, actualStatusCode);
    ASSERT_EQ(toInt(VehicleProperty::INFO_MAKE), actualValue.prop);
    ASSERT_STREQ(kCarMake, actualValue.value.stringValue.c_str());
}

TEST_F(VehicleHalManagerTest, get_NegativeCases) {
    // Write-only property must fail.
    invokeGet(toInt(VehicleProperty::HVAC_SEAT_TEMPERATURE), 0);
    ASSERT_EQ(StatusCode::ACCESS_DENIED, actualStatusCode);

    // Unknown property must fail.
    invokeGet(toInt(VehicleProperty::MIRROR_Z_MOVE), 0);
    ASSERT_EQ(StatusCode::INVALID_ARG, actualStatusCode);
}

TEST_F(VehicleHalManagerTest, get_Retriable) {
    actualStatusCode = StatusCode::TRY_AGAIN;
    int attempts = 0;
    while (StatusCode::TRY_AGAIN == actualStatusCode && ++attempts < 10) {
        invokeGet(toInt(VehicleProperty::INFO_FUEL_CAPACITY), 0);

    }
    ASSERT_EQ(StatusCode::OK, actualStatusCode);
    ASSERT_EQ(kRetriablePropMockedAttempts + 1, attempts);
    ASSERT_FLOAT_EQ(42.42, actualValue.value.floatValues[0]);
}

TEST_F(VehicleHalManagerTest, set_Basic) {
    const auto PROP = toInt(VehicleProperty::DISPLAY_BRIGHTNESS);
    const auto VAL = 7;

    auto expectedValue = hal->getValuePool()->obtainInt32(VAL);
    expectedValue->prop = PROP;
    expectedValue->areaId = 0;

    actualStatusCode = manager->set(*expectedValue.get());
    ASSERT_EQ(StatusCode::OK, actualStatusCode);

    invokeGet(PROP, 0);
    ASSERT_EQ(StatusCode::OK, actualStatusCode);
    ASSERT_EQ(PROP, actualValue.prop);
    ASSERT_EQ(VAL, actualValue.value.int32Values[0]);
}

TEST_F(VehicleHalManagerTest, set_DifferentAreas) {
    const auto PROP = toInt(VehicleProperty::HVAC_FAN_SPEED);
    const auto VAL1 = 1;
    const auto VAL2 = 2;
    const auto AREA1 = toInt(VehicleAreaSeat::ROW_1_LEFT);
    const auto AREA2 = toInt(VehicleAreaSeat::ROW_1_RIGHT);

    {
        auto expectedValue1 = hal->getValuePool()->obtainInt32(VAL1);
        expectedValue1->prop = PROP;
        expectedValue1->areaId = AREA1;
        actualStatusCode = manager->set(*expectedValue1.get());
        ASSERT_EQ(StatusCode::OK, actualStatusCode);

        auto expectedValue2 = hal->getValuePool()->obtainInt32(VAL2);
        expectedValue2->prop = PROP;
        expectedValue2->areaId = AREA2;
        actualStatusCode = manager->set(*expectedValue2.get());
        ASSERT_EQ(StatusCode::OK, actualStatusCode);
    }

    {
        invokeGet(PROP, AREA1);
        ASSERT_EQ(StatusCode::OK, actualStatusCode);
        ASSERT_EQ(PROP, actualValue.prop);
        ASSERT_EQ(AREA1, actualValue.areaId);
        ASSERT_EQ(VAL1, actualValue.value.int32Values[0]);

        invokeGet(PROP, AREA2);
        ASSERT_EQ(StatusCode::OK, actualStatusCode);
        ASSERT_EQ(PROP, actualValue.prop);
        ASSERT_EQ(AREA2, actualValue.areaId);
        ASSERT_EQ(VAL2, actualValue.value.int32Values[0]);
    }
}

TEST_F(VehicleHalManagerTest, set_Retriable) {
    const auto PROP = toInt(VehicleProperty::MIRROR_FOLD);

    auto v = hal->getValuePool()->obtainBoolean(true);
    v->prop = PROP;
    v->areaId = 0;

    actualStatusCode = StatusCode::TRY_AGAIN;
    int attempts = 0;
    while (StatusCode::TRY_AGAIN == actualStatusCode && ++attempts < 10) {
        actualStatusCode = manager->set(*v.get());
    }

    ASSERT_EQ(StatusCode::OK, actualStatusCode);
    ASSERT_EQ(kRetriablePropMockedAttempts + 1, attempts);

    invokeGet(PROP, 0);
    ASSERT_EQ(StatusCode::OK, actualStatusCode);
    ASSERT_TRUE(actualValue.value.int32Values[0]);
}

TEST(HalClientVectorTest, basic) {
    HalClientVector clients;
    sp<IVehicleCallback> callback1 = new MockedVehicleCallback();

    sp<HalClient> c1 = new HalClient(callback1);
    sp<HalClient> c2 = new HalClient(callback1);

    clients.addOrUpdate(c1);
    clients.addOrUpdate(c1);
    clients.addOrUpdate(c2);
    ASSERT_EQ(2u, clients.size());
    ASSERT_FALSE(clients.isEmpty());
    ASSERT_LE(0, clients.indexOf(c1));
    ASSERT_LE(0, clients.remove(c1));
    ASSERT_GT(0, clients.indexOf(c1));  // c1 was already removed
    ASSERT_GT(0, clients.remove(c1));   // attempt to remove c1 again
    ASSERT_LE(0, clients.remove(c2));

    ASSERT_TRUE(clients.isEmpty());
}

TEST_F(VehicleHalManagerTest, debug) {
    hidl_handle fd = {};
    fd.setTo(native_handle_create(/*numFds=*/1, /*numInts=*/0), /*shouldOwn=*/true);

    // Because debug function returns void, so no way to check return value.
    manager->debug(fd, {});
    manager->debug(fd, {"--help"});
    manager->debug(fd, {"--list"});
    manager->debug(fd, {"--get"});
    manager->debug(fd, {"--set"});
    manager->debug(fd, {"invalid"});
}

struct SetPropTestCase {
    std::string test_name;
    const hidl_vec<hidl_string> configs;
    bool success;
};

class VehicleHalManagerSetPropTest : public VehicleHalManagerTest,
                                     public testing::WithParamInterface<SetPropTestCase> {};

TEST_P(VehicleHalManagerSetPropTest, cmdSetOneProperty) {
    const SetPropTestCase& tc = GetParam();
    VehicleHalManagerTestHelper helper(manager.get());
    ASSERT_EQ(tc.success, helper.cmdSetOneProperty(STDERR_FILENO, tc.configs));
}

std::vector<SetPropTestCase> GenSetPropParams() {
    char infoMakeProperty[100] = {};
    snprintf(infoMakeProperty, sizeof(infoMakeProperty), "%d", toInt(VehicleProperty::INFO_MAKE));
    return {
            {"success_set_string", {"--set", infoMakeProperty, "-s", kCarMake}, true},
            {"success_set_bytes", {"--set", infoMakeProperty, "-b", "0xdeadbeef"}, true},
            {"success_set_bytes_caps", {"--set", infoMakeProperty, "-b", "0xDEADBEEF"}, true},
            {"success_set_int", {"--set", infoMakeProperty, "-i", "2147483647"}, true},
            {"success_set_ints",
             {"--set", infoMakeProperty, "-i", "2147483647", "0", "-2147483648"},
             true},
            {"success_set_int64",
             {"--set", infoMakeProperty, "-i64", "-9223372036854775808"},
             true},
            {"success_set_int64s",
             {"--set", infoMakeProperty, "-i64", "-9223372036854775808", "0",
              "9223372036854775807"},
             true},
            {"success_set_float", {"--set", infoMakeProperty, "-f", "1.175494351E-38"}, true},
            {"success_set_floats",
             {"--set", infoMakeProperty, "-f", "-3.402823466E+38", "0", "3.402823466E+38"},
             true},
            {"success_set_area", {"--set", infoMakeProperty, "-a", "2147483647"}, true},
            {"fail_no_options", {}, false},
            {"fail_less_than_4_options", {"--set", infoMakeProperty, "-i"}, false},
            {"fail_unknown_options", {"--set", infoMakeProperty, "-s", kCarMake, "-abcd"}, false},
            {"fail_invalid_property", {"--set", "not valid", "-s", kCarMake}, false},
            {"fail_duplicate_string",
             {"--set", infoMakeProperty, "-s", kCarMake, "-s", kCarMake},
             false},
            {"fail_multiple_strings", {"--set", infoMakeProperty, "-s", kCarMake, kCarMake}, false},
            {"fail_no_string_value", {"--set", infoMakeProperty, "-s", "-a", "1234"}, false},
            {"fail_duplicate_bytes",
             {"--set", infoMakeProperty, "-b", "0xdeadbeef", "-b", "0xdeadbeef"},
             false},
            {"fail_multiple_bytes",
             {"--set", infoMakeProperty, "-b", "0xdeadbeef", "0xdeadbeef"},
             false},
            {"fail_invalid_bytes", {"--set", infoMakeProperty, "-b", "0xgood"}, false},
            {"fail_invalid_bytes_no_prefix", {"--set", infoMakeProperty, "-b", "deadbeef"}, false},
            {"fail_invalid_int", {"--set", infoMakeProperty, "-i", "abc"}, false},
            {"fail_int_out_of_range", {"--set", infoMakeProperty, "-i", "2147483648"}, false},
            {"fail_no_int_value", {"--set", infoMakeProperty, "-i", "-s", kCarMake}, false},
            {"fail_invalid_int64", {"--set", infoMakeProperty, "-i64", "abc"}, false},
            {"fail_int64_out_of_range",
             {"--set", infoMakeProperty, "-i64", "-9223372036854775809"},
             false},
            {"fail_no_int64_value", {"--set", infoMakeProperty, "-i64", "-s", kCarMake}, false},
            {"fail_invalid_float", {"--set", infoMakeProperty, "-f", "abc"}, false},
            {"fail_float_out_of_range",
             {"--set", infoMakeProperty, "-f", "-3.402823466E+39"},
             false},
            {"fail_no_float_value", {"--set", infoMakeProperty, "-f", "-s", kCarMake}, false},
            {"fail_multiple_areas", {"--set", infoMakeProperty, "-a", "2147483648", "0"}, false},
            {"fail_invalid_area", {"--set", infoMakeProperty, "-a", "abc"}, false},
            {"fail_area_out_of_range", {"--set", infoMakeProperty, "-a", "2147483648"}, false},
            {"fail_no_area_value", {"--set", infoMakeProperty, "-a", "-s", kCarMake}, false},
    };
}

INSTANTIATE_TEST_SUITE_P(
        VehicleHalManagerSetPropTests, VehicleHalManagerSetPropTest,
        testing::ValuesIn(GenSetPropParams()),
        [](const testing::TestParamInfo<VehicleHalManagerSetPropTest::ParamType>& info) {
            return info.param.test_name;
        });

TEST_F(VehicleHalManagerTest, SetComplexPropTest) {
    char infoMakeProperty[100] = {};
    snprintf(infoMakeProperty, sizeof(infoMakeProperty), "%d", toInt(VehicleProperty::INFO_MAKE));
    VehicleHalManagerTestHelper helper(manager.get());
    ASSERT_TRUE(helper.cmdSetOneProperty(
            STDERR_FILENO, {"--set", infoMakeProperty,      "-s",   kCarMake,
                            "-b",    "0xdeadbeef",          "-i",   "2147483647",
                            "0",     "-2147483648",         "-i64", "-9223372036854775808",
                            "0",     "9223372036854775807", "-f",   "-3.402823466E+38",
                            "0",     "3.402823466E+38",     "-a",   "123"}));
    StatusCode status = StatusCode::OK;
    VehiclePropValue requestProp;
    requestProp.prop = toInt(VehicleProperty::INFO_MAKE);
    requestProp.areaId = 123;
    auto value = hal->get(requestProp, &status);
    ASSERT_EQ(StatusCode::OK, status);
    ASSERT_EQ(value->prop, toInt(VehicleProperty::INFO_MAKE));
    ASSERT_EQ(value->areaId, 123);
    ASSERT_STREQ(kCarMake, value->value.stringValue.c_str());
    uint8_t bytes[] = {0xde, 0xad, 0xbe, 0xef};
    ASSERT_FALSE(memcmp(bytes, value->value.bytes.data(), sizeof(bytes)));
    ASSERT_EQ(3u, value->value.int32Values.size());
    ASSERT_EQ(2147483647, value->value.int32Values[0]);
    ASSERT_EQ(0, value->value.int32Values[1]);
    ASSERT_EQ(-2147483648, value->value.int32Values[2]);
    ASSERT_EQ(3u, value->value.int64Values.size());
    // -9223372036854775808 is not a valid literal since '-' and '9223372036854775808' would be two
    // tokens and the later does not fit in unsigned long long.
    ASSERT_EQ(-9223372036854775807 - 1, value->value.int64Values[0]);
    ASSERT_EQ(0, value->value.int64Values[1]);
    ASSERT_EQ(9223372036854775807, value->value.int64Values[2]);
    ASSERT_EQ(3u, value->value.floatValues.size());
    ASSERT_EQ(-3.402823466E+38f, value->value.floatValues[0]);
    ASSERT_EQ(0.0f, value->value.floatValues[1]);
    ASSERT_EQ(3.402823466E+38f, value->value.floatValues[2]);
}

}  // namespace anonymous

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android
