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

    public TabSwitchViewModel(Application application) {
        this.mApplication = application;
        this.currentActiveTab = TabState.TAB1.getId(); // Default to Tab 1
        initCarConnection();
    }

    private final CarPropertyManager.CarPropertyEventCallback mCarPropertyEventCallback = new CarPropertyManager.CarPropertyEventCallback() {
        @Override
        public void onChangeEvent(CarPropertyValue value) {
            if (value.getPropertyId() == VehiclePropertyIds.GECKO_VEHICLE_DRIVE_MODE) {
                int newDriveMode = (Integer) value.getValue();
                Log.d(TAG, "Drive mode changed to: " + newDriveMode);

                if (isWaitingForConfirmation && pendingTab != null && pendingTab == newDriveMode) {
                    timeoutHandler.removeCallbacksAndMessages(null); // Cancel timeout
                    currentActiveTab = newDriveMode;
                    pendingTab = null;
                    isWaitingForConfirmation = false;
                    notifyStateChange();
                }
            }
        }

        @Override
        public void onErrorEvent(int propId, int zone) {
            Log.w(TAG, "Received error car property event for propId=" + propId);
            if (isWaitingForConfirmation && propId == VehiclePropertyIds.GECKO_VEHICLE_DRIVE_MODE) {
                handleTimeoutOrError("Error setting property");
            }
        }
    };

    private void initCarConnection() {
        mCar = Car.createCar(mApplication.getApplicationContext(), null, Car.CAR_WAIT_TIMEOUT_WAIT_FOREVER, (car, ready) -> {
            if (ready) {
                mCarPropertyManager = (CarPropertyManager) car.getCarManager(Car.PROPERTY_SERVICE);
                mCarPropertyManager.registerCallback(mCarPropertyEventCallback, VehiclePropertyIds.GECKO_VEHICLE_DRIVE_MODE, CarPropertyManager.SENSOR_RATE_ONCHANGE);
                Log.d(TAG, "Car connected and callback registered");
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
            Log.e(TAG, "CarPropertyManager not available. Cannot set drive mode.");
            if (listener != null) listener.onShowMessage("Error: Car service not connected");
            return;
        }

        pendingTab = tabId;
        isWaitingForConfirmation = true;
        if (listener != null) listener.onShowMessage("Waiting for confirmation...");
        notifyStateChange();

        // Set a timeout
        timeoutHandler.postDelayed(() -> handleTimeoutOrError("Switch timed out"), SWITCH_TIMEOUT_MS);

        Log.d(TAG, "Setting drive mode to: " + tabId);
        mCarPropertyManager.setIntProperty(VehiclePropertyIds.GECKO_VEHICLE_DRIVE_MODE, 0, tabId);
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
            mCarPropertyManager.unregisterCallback(mCarPropertyEventCallback);
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
