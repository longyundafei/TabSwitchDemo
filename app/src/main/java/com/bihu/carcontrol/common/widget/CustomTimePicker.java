package com.bihu.carcontrol.common.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.ViewGroup;
import android.widget.NumberPicker;
import android.widget.RelativeLayout;

import com.android.car.client.manager.vehicle.CarVehicleManager;
import com.android.car.core.server.vehicle.VehicleType;
import com.bihu.carcontrol.R;

/**
 * FileName: CustomTimePicker
 * Author: WangXu
 * Date: 2023/8/17 14:21
 * Description:
 */
public class CustomTimePicker extends RelativeLayout {
    private Context mContext;
    private TextColorNumberPicker mHourNumberPicker;
    private TextColorNumberPicker mMinNumberPicker;

    public CustomTimePicker(Context context) {
        super(context);
    }

    public CustomTimePicker(Context context, AttributeSet attrs) {
        super(context, attrs);
        initView(context);
    }

    public CustomTimePicker(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    public CustomTimePicker(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
    }

    private void initView(Context context) {
        this.mContext = context;
        LayoutInflater.from(context).inflate(R.layout.custom_time_picker, (ViewGroup) this, true);
        this.mHourNumberPicker = findViewById(R.id.np_hour);
        this.mMinNumberPicker = findViewById(R.id.np_min);
        this.mHourNumberPicker.setMaxValue(23);
        this.mMinNumberPicker.setMaxValue(59);
        NumberPicker.Formatter formatter = new NumberPicker.Formatter() { // from class: com.mega.charging.ui.CustomTimePicker$$ExternalSyntheticLambda0
            @Override // android.widget.NumberPicker.Formatter
            public final String format(int i) {
//                String format;
//                format = String.format(TimeModel.ZERO_LEADING_NUMBER_FORMAT, Integer.valueOf(i));
//                return format;
                return String.valueOf(i);
            }
        };
        this.mHourNumberPicker.setFormatter(formatter);
        this.mMinNumberPicker.setFormatter(formatter);
        this.mHourNumberPicker.setPickerDividerColor(0);
        this.mMinNumberPicker.setPickerDividerColor(0);
    }

    public int getCurrentHourValue() {
        return this.mHourNumberPicker.getCurrentValue();
    }

    public int getCurrentMinValue() {
        return this.mMinNumberPicker.getCurrentValue();
    }

    public void save(String chargingType) {
//        MegaSettings.putStringForUserGlobal(this.mContext.getContentResolver(), hourData, String.valueOf(getCurrentHourValue()), 0);
//        MegaSettings.putStringForUserGlobal(this.mContext.getContentResolver(), minData, String.valueOf(getCurrentMinValue()), 0);
//        if (chargingType.equals(DataUtil.CHARGINGSTART)) {
//            CarVehicleManager.getInstance().updateChargingSetting(VehicleType.GECKO_CCM_CHRAGING_BEGIN_TIME_HOUR, getCurrentHourValue());
//            LogUtil.d("设置预约充电开始时间(时): " + getCurrentHourValue());
//            CarVehicleManager.getInstance().updateChargingSetting(VehicleType.GECKO_CCM_CHRAGING_BEGIN_TIME_MINUTE, getCurrentMinValue());
//            LogUtil.d("设置预约充电开始时间(分): " + getCurrentMinValue());
//        } else if (chargingType.equals(DataUtil.CHARGINGFINISH)) {
//            CarVehicleManager.getInstance().updateChargingSetting(VehicleType.GECKO_CCM_CHRAGING_END_TIME_HOUR, getCurrentHourValue());
//            LogUtil.d("设置预约充电结束时间(时): " + getCurrentHourValue());
//            CarVehicleManager.getInstance().updateChargingSetting(VehicleType.GECKO_CCM_CHRAGING_END_TIME_MINUTE, getCurrentMinValue());
//            LogUtil.d("设置预约充电结束时间(分): " + getCurrentMinValue());
//        }
    }

    public void restore(String chargingType) {
        int hour = 0;
        int min = 0;
//        if (chargingType.equals(DataUtil.CHARGINGSTART)) {
//            hour = CarVehicleManager.getInstance().requestGetChargingSetting(VehicleType.GECKO_CCM_CHRAGING_BEGIN_TIME_HOUR);
//            min = CarVehicleManager.getInstance().requestGetChargingSetting(VehicleType.GECKO_CCM_CHRAGING_BEGIN_TIME_MINUTE);
//            LogUtil.d("获取预约充电开始时间：" + hour + "时" + min + "分");
//        } else if (chargingType.equals(DataUtil.CHARGINGFINISH)) {
//            hour = CarVehicleManager.getInstance().requestGetChargingSetting(VehicleType.GECKO_CCM_CHRAGING_END_TIME_HOUR);
//            min = CarVehicleManager.getInstance().requestGetChargingSetting(VehicleType.GECKO_CCM_CHRAGING_END_TIME_MINUTE);
//            LogUtil.d("获取预约充电结束时间：" + hour + "时" + min + "分");
//        }
        this.mHourNumberPicker.setValue(hour);
        this.mMinNumberPicker.setValue(min);
    }
}
