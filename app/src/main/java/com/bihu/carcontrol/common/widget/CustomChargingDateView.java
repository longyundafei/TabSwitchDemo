package com.bihu.carcontrol.common.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.alibaba.fastjson.JSONObject;
import com.android.car.client.manager.vehicle.CarVehicleManager;
import com.bihu.carcontrol.R;
import com.bihu.carcontrol.common.util.LogUtil;

import java.util.ArrayList;
import java.util.List;

/**
 * FileName: CustomChargingDateView
 * Author: WangXu
 * Date: 2024/6/7 16:29
 * Description:
 */
public class CustomChargingDateView extends LinearLayout implements View.OnClickListener {
    private static final String TAG = CustomChargingDateView.class.getSimpleName();
    private Context mContext;
    private boolean mSelectFriday;
    private boolean mSelectMonday;
    private boolean mSelectSaturday;
    private boolean mSelectSunday;
    private boolean mSelectThursday;
    private boolean mSelectTuesday;
    private boolean mSelectWednesday;
    private TextView mTvFriday;
    private TextView mTvMonday;
    private TextView mTvSaturday;
    private TextView mTvSunday;
    private TextView mTvThursday;
    private TextView mTvTuesday;
    private TextView mTvWednesday;
    private boolean mSingle = true;
    private boolean isChargingEdit = false;

    public CustomChargingDateView(Context context) {
        super(context);
//        this.mSelectMonday = true;
    }

    public CustomChargingDateView(Context context, AttributeSet attrs) {
        super(context, attrs);
//        this.mSelectMonday = true;
        initView(context);
    }

    public CustomChargingDateView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
//        this.mSelectMonday = true;
    }

    public CustomChargingDateView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
//        this.mSelectMonday = true;
    }

    private void initView(Context context) {
        this.mContext = context;
        LayoutInflater.from(context).inflate(R.layout.precharging_date, (ViewGroup) this, true);
        this.mTvMonday = findViewById(R.id.tv_monday);
        this.mTvTuesday = findViewById(R.id.tv_tuesday);
        this.mTvWednesday = findViewById(R.id.tv_wednesday);
        this.mTvThursday = findViewById(R.id.tv_thursday);
        this.mTvFriday = findViewById(R.id.tv_friday);
        this.mTvSaturday = findViewById(R.id.tv_saturday);
        this.mTvSunday = findViewById(R.id.tv_sunday);
        this.mTvMonday.setOnClickListener(this);
        this.mTvTuesday.setOnClickListener(this);
        this.mTvWednesday.setOnClickListener(this);
        this.mTvThursday.setOnClickListener(this);
        this.mTvFriday.setOnClickListener(this);
        this.mTvSaturday.setOnClickListener(this);
        this.mTvSunday.setOnClickListener(this);
        restore();
    }

    @Override
    public void onClick(View view) {
        if (!isChargingEdit) {
            return;
        }
        int id = view.getId();
        switch (id) {
            case R.id.tv_monday:
                if (this.mSingle) {
                    selectSingleDate(0);
                } else {
                    this.mSelectMonday = !this.mSelectMonday;
                }
                updateUI();
                return;
            case R.id.tv_tuesday:
                if (this.mSingle) {
                    selectSingleDate(1);
                } else {
                    this.mSelectTuesday = !this.mSelectTuesday;
                }
                updateUI();
                return;
            case R.id.tv_wednesday:
                if (this.mSingle) {
                    selectSingleDate(2);
                } else {
                    this.mSelectWednesday = !this.mSelectWednesday;
                }
                updateUI();
                return;
            case R.id.tv_thursday:
                if (this.mSingle) {
                    selectSingleDate(3);
                } else {
                    this.mSelectThursday = !this.mSelectThursday;
                }
                updateUI();
                return;
            case R.id.tv_friday:
                if (this.mSingle) {
                    selectSingleDate(4);
                } else {
                    this.mSelectFriday = !this.mSelectFriday;
                }
                updateUI();
                return;
            case R.id.tv_saturday:
                if (this.mSingle) {
                    selectSingleDate(5);
                } else {
                    this.mSelectSaturday = !this.mSelectSaturday;
                }
                updateUI();
                return;
            case R.id.tv_sunday:
                if (this.mSingle) {
                    selectSingleDate(6);
                } else {
                    this.mSelectSunday = !this.mSelectSunday;
                }
                updateUI();
                return;
        }
    }

    private void updateUI() {
        this.mTvMonday.setTextColor(this.mSelectMonday ? getResources().getColor(R.color.white) : getResources().getColor(R.color.tab_unselect_text_color));
        this.mTvTuesday.setTextColor(this.mSelectTuesday ? getResources().getColor(R.color.white) : getResources().getColor(R.color.tab_unselect_text_color));
        this.mTvWednesday.setTextColor(this.mSelectWednesday ? getResources().getColor(R.color.white) : getResources().getColor(R.color.tab_unselect_text_color));
        this.mTvThursday.setTextColor(this.mSelectThursday ? getResources().getColor(R.color.white) : getResources().getColor(R.color.tab_unselect_text_color));
        this.mTvFriday.setTextColor(this.mSelectFriday ? getResources().getColor(R.color.white) : getResources().getColor(R.color.tab_unselect_text_color));
        this.mTvSaturday.setTextColor(this.mSelectSaturday ? getResources().getColor(R.color.white) : getResources().getColor(R.color.tab_unselect_text_color));
        this.mTvSunday.setTextColor(this.mSelectSunday ? getResources().getColor(R.color.white) : getResources().getColor(R.color.tab_unselect_text_color));
        this.mTvMonday.setBackgroundResource(this.mSelectMonday ? R.drawable.bg_btn_rect_select : R.drawable.bg_btn_rect_unselect);
        this.mTvTuesday.setBackgroundResource(this.mSelectTuesday ? R.drawable.bg_btn_rect_select : R.drawable.bg_btn_rect_unselect);
        this.mTvWednesday.setBackgroundResource(this.mSelectWednesday ? R.drawable.bg_btn_rect_select : R.drawable.bg_btn_rect_unselect);
        this.mTvThursday.setBackgroundResource(this.mSelectThursday ? R.drawable.bg_btn_rect_select : R.drawable.bg_btn_rect_unselect);
        this.mTvFriday.setBackgroundResource(this.mSelectFriday ? R.drawable.bg_btn_rect_select : R.drawable.bg_btn_rect_unselect);
        this.mTvSaturday.setBackgroundResource(this.mSelectSaturday ? R.drawable.bg_btn_rect_select : R.drawable.bg_btn_rect_unselect);
        this.mTvSunday.setBackgroundResource(this.mSelectSunday ? R.drawable.bg_btn_rect_select : R.drawable.bg_btn_rect_unselect);
    }

    public void restore() {
        List<Integer> chargingLoopList = CarVehicleManager.getInstance().requestVehicleChargingLoopData();
        if (chargingLoopList != null && chargingLoopList.size() == 7) {
            this.mSelectMonday = chargingLoopList.get(0) == 1;
            this.mSelectTuesday = chargingLoopList.get(1) == 1;
            this.mSelectWednesday = chargingLoopList.get(2) == 1;
            this.mSelectThursday = chargingLoopList.get(3) == 1;
            this.mSelectFriday = chargingLoopList.get(4) == 1;
            this.mSelectSaturday = chargingLoopList.get(5) == 1;
            this.mSelectSunday = chargingLoopList.get(6) == 1;
        } else {
            LogUtil.d("get charging date failed!");
        }
        updateUI();
    }

    public void save() {
        List<Integer> dateList = new ArrayList<>();
        for (int i = 0; i < 7; i++) {
            if (i == 0) {
                dateList.add(this.mSelectMonday ? 1 : 0);
            } else if (i == 1) {
                dateList.add(this.mSelectTuesday ? 1 : 0);
            } else if (i == 2) {
                dateList.add(this.mSelectWednesday ? 1 : 0);
            } else if (i == 3) {
                dateList.add(this.mSelectThursday ? 1 : 0);
            } else if (i == 4) {
                dateList.add(this.mSelectFriday ? 1 : 0);
            } else if (i == 5) {
                dateList.add(this.mSelectSaturday ? 1 : 0);
            } else if (i == 6) {
                dateList.add(this.mSelectSunday ? 1 : 0);
            }
        }
        CarVehicleManager.getInstance().updateVehicleChargingLoopData(dateList);
        LogUtil.d("设置预约充电开始星期列表日期： " + JSONObject.toJSONString(dateList));
    }

    public String getDateString() {
        StringBuilder stringBuilder = new StringBuilder();
        if (this.mSelectMonday) {
            stringBuilder.append(getResources().getString(R.string.tv_monday));
        }
        if (this.mSelectTuesday) {
            stringBuilder.append("  ").append(getResources().getString(R.string.tv_tuesday));
        }
        if (this.mSelectWednesday) {
            stringBuilder.append("  ").append(getResources().getString(R.string.tv_wednesday));
        }
        if (this.mSelectThursday) {
            stringBuilder.append("  ").append(getResources().getString(R.string.tv_thursday));
        }
        if (this.mSelectFriday) {
            stringBuilder.append("  ").append(getResources().getString(R.string.tv_friday));
        }
        if (this.mSelectSaturday) {
            stringBuilder.append("  ").append(getResources().getString(R.string.tv_saturday));
        }
        if (this.mSelectSunday) {
            stringBuilder.append("  ").append(getResources().getString(R.string.tv_sunday));
        }
        return stringBuilder.toString();
    }

    public boolean showWarning() {
        return (this.mSelectMonday || this.mSelectTuesday || this.mSelectWednesday || this.mSelectThursday || this.mSelectFriday || this.mSelectSaturday || this.mSelectSunday) ? false : true;
    }

    public void setSingleSelect(boolean single, boolean clickSingle) {
        this.mSingle = single;
        if (single && clickSingle) {//手动点击单次充电
            this.mSelectMonday = false;
            this.mSelectTuesday = false;
            this.mSelectWednesday = false;
            this.mSelectThursday = false;
            this.mSelectFriday = false;
            this.mSelectSaturday = false;
            this.mSelectSunday = false;
            updateUI();
        }
    }

    public void setEditState(boolean editing) {
        this.isChargingEdit = editing;
    }

    public void selectSingleDate(int index) {
        this.mSelectMonday = false;
        this.mSelectTuesday = false;
        this.mSelectWednesday = false;
        this.mSelectThursday = false;
        this.mSelectFriday = false;
        this.mSelectSaturday = false;
        this.mSelectSunday = false;
        for (int i = 0; i < 6; i++) {
            if (index == 0) {
                this.mSelectMonday = true;
            } else if (index == 1) {
                this.mSelectTuesday = true;
            } else if (index == 2) {
                this.mSelectWednesday = true;
            } else if (index == 3) {
                this.mSelectThursday = true;
            } else if (index == 4) {
                this.mSelectFriday = true;
            } else if (index == 5) {
                this.mSelectSaturday = true;
            } else if (index == 6) {
                this.mSelectSunday = true;
            }
        }
    }
}
