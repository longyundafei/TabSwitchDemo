package com.android.tabswitchdemo;

import android.app.Application;
import android.car.Car;
import android.car.hardware.CarPropertyValue;
import android.car.hardware.property.CarPropertyManager;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.car.VehiclePropertyIds;

import com.android.tabswitchdemo.model.TabState;
import com.android.tabswitchdemo.model.TabConfig;

public class TabSwitchViewModel {

    private static final String TAG = "TabSwitchViewModel";
    private static final int SWITCH_TIMEOUT_MS = 5000; // 5 seconds

    private int currentActiveTab;
    private Integer pendingTab;
    private boolean isWaitingForConfirmation;

    private OnStateChangeListener listener;
    private final Handler mainHandler = new Handler(Looper.getMainLooper());
    private final Handler timeoutHandler = new Handler(Looper.getMainLooper());

    private final Application mApplication;
    private Car mCar;
    private CarPropertyManager mCarPropertyManager;
    private final TabConfig[] tabConfigs;

    // 新增构造函数，支持传入tabConfigs
    public TabSwitchViewModel(Application application, TabConfig[] tabConfigs) {
        this.mApplication = application;
        this.tabConfigs = tabConfigs;
        this.currentActiveTab = TabState.TAB1.getId(); // 默认Tab 1
        initCarConnection();
    }

    // 兼容旧构造方式，默认全部为GECKO_VEHICLE_DRIVE_MODE
    public TabSwitchViewModel(Application application) {
        this(application, new TabConfig[]{
            new TabConfig(android.car.VehiclePropertyIds.GECKO_VEHICLE_DRIVE_MODE, android.car.VehiclePropertyIds.GECKO_VEHICLE_DRIVE_MODE),
            new TabConfig(android.car.VehiclePropertyIds.GECKO_VEHICLE_DRIVE_MODE, android.car.VehiclePropertyIds.GECKO_VEHICLE_DRIVE_MODE),
            new TabConfig(android.car.VehiclePropertyIds.GECKO_VEHICLE_DRIVE_MODE, android.car.VehiclePropertyIds.GECKO_VEHICLE_DRIVE_MODE)
        });
    }

    private final CarPropertyManager.CarPropertyEventCallback mCarPropertyEventCallback = new CarPropertyManager.CarPropertyEventCallback() {
        @Override
        public void onChangeEvent(CarPropertyValue value) {
            // 遍历所有tab配置，找到匹配的responsePropId
            for (int i = 0; i < tabConfigs.length; i++) {
                if (value.getPropertyId() == tabConfigs[i].responsePropId) {
                    int newValue = (Integer) value.getValue();
                    Log.d(TAG, "Property " + value.getPropertyId() + " changed to: " + newValue);

                    if (isWaitingForConfirmation && pendingTab != null && pendingTab == newValue) {
                        timeoutHandler.removeCallbacksAndMessages(null); // Cancel timeout
                        currentActiveTab = newValue;
                        pendingTab = null;
                        isWaitingForConfirmation = false;
                        notifyStateChange();
                    }
                }
            }
        }

        @Override
        public void onErrorEvent(int propId, int zone) {
            Log.w(TAG, "Received error car property event for propId=" + propId);
            for (TabConfig config : tabConfigs) {
                if (isWaitingForConfirmation && propId == config.responsePropId) {
                    handleTimeoutOrError("Error setting property");
                }
            }
        }
    };


    private void initCarConnection() {
        mCar = Car.createCar(mApplication.getApplicationContext(), null, Car.CAR_WAIT_TIMEOUT_WAIT_FOREVER, (car, ready) -> {
            if (ready) {
                mCarPropertyManager = (CarPropertyManager) car.getCarManager(Car.PROPERTY_SERVICE);
                // 注册所有responsePropId的回调
                for (TabConfig config : tabConfigs) {
                    mCarPropertyManager.registerCallback(mCarPropertyEventCallback, config.responsePropId, CarPropertyManager.SENSOR_RATE_ONCHANGE);
                }
                Log.d(TAG, "Car connected and callbacks registered");
            } else {
                Log.e(TAG, "Car not ready");
            }
        });
    }

    public void onTabClicked(int tabId) {
        if (isWaitingForConfirmation || tabId == currentActiveTab) {
            return; // Ignore clicks while waiting or on the active tab
        }
        if (mCarPropertyManager == null) {
            Log.e(TAG, "CarPropertyManager not available. Cannot set property.");
            if (listener != null) listener.onShowMessage("Error: Car service not connected");
            return;
        }

        pendingTab = tabId;
        isWaitingForConfirmation = true;
        if (listener != null) listener.onShowMessage("Waiting for confirmation...");
        notifyStateChange();

        // Set a timeout
        timeoutHandler.postDelayed(() -> handleTimeoutOrError("Switch timed out"), SWITCH_TIMEOUT_MS);

        // 找到对应tab的setPropId
        int tabIndex = tabId - 1; // 假设tabId从1开始
        if (tabIndex >= 0 && tabIndex < tabConfigs.length) {
            int setPropId = tabConfigs[tabIndex].setPropId;
            Log.d(TAG, "Setting property " + setPropId + " to: " + tabId);
            mCarPropertyManager.setIntProperty(setPropId, 0, tabId);
        } else {
            Log.e(TAG, "Invalid tab index for property setting");
        }
    }

    private void handleTimeoutOrError(String message) {
        timeoutHandler.removeCallbacksAndMessages(null);
        if (isWaitingForConfirmation) {
            pendingTab = null;
            isWaitingForConfirmation = false;
            if (listener != null) listener.onShowMessage(message);
            notifyStateChange();
        }
    }

    public void getTemperature() {
        if (mCarPropertyManager == null) {
            Log.e(TAG, "CarPropertyManager not available");
            if (listener != null) listener.onShowMessage("Error: Car service not connected");
            return;
        }
        try {
            CarPropertyValue<Float> carPropertyValue = mCarPropertyManager.getProperty(Float.class, VehiclePropertyIds.HVAC_TEMPERATURE_SET, 49);
            if (carPropertyValue != null && carPropertyValue.getStatus() == CarPropertyValue.STATUS_AVAILABLE) {
                if (listener != null) listener.onTemperatureChanged("Temp: " + carPropertyValue.getValue());
            } else {
                if (listener != null) listener.onTemperatureChanged("Temp: Not available");
            }
        } catch (Exception e) {
            Log.e(TAG, "Failed to get temperature", e);
            if (listener != null) listener.onTemperatureChanged("Temp: Error");
        }
    }

    private void notifyStateChange() {
        if (listener != null) {
            mainHandler.post(() -> listener.onTabStateChanged(currentActiveTab, pendingTab, isWaitingForConfirmation));
        }
    }

    public int getCurrentActiveTab() { return currentActiveTab; }
    public Integer getPendingTab() { return pendingTab; }
    public boolean isWaitingForConfirmation() { return isWaitingForConfirmation; }

    public void setOnStateChangeListener(OnStateChangeListener listener) {
        this.listener = listener;
        notifyStateChange(); // Notify with initial state
    }

    public void destroy() {
        timeoutHandler.removeCallbacksAndMessages(null);
        if (mCarPropertyManager != null) {
            for (TabConfig config : tabConfigs) {
                mCarPropertyManager.unregisterCallback(mCarPropertyEventCallback, config.responsePropId);
            }
        }
        if (mCar != null && mCar.isConnected()) {
            mCar.disconnect();
        }
    }

    public interface OnStateChangeListener {
        void onTabStateChanged(int currentActiveTab, Integer pendingTab, boolean isWaitingForConfirmation);
        void onShowMessage(String message);
        void onTemperatureChanged(String temperature);
    }
}
