// packages/apps/TabSwitchDemo/src/com/android/tabswitchdemo/TabSwitchFragment.java
package com.android.tabswitchdemo;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.Spinner;
import android.widget.ArrayAdapter;
import android.car.VehiclePropertyIds;

import com.android.tabswitchdemo.model.TabState;
import com.android.tabswitchdemo.model.TabConfig;

public class TabSwitchFragment extends Fragment implements TabSwitchViewModel.OnStateChangeListener {

    private TabSwitchViewModel viewModel;

    private Button buttonTab1, buttonTab2, buttonTab3, buttonTab4;
    private TextView textCurrentTab, textWaitingConfirmation;
    private Spinner setPropSpinner, responsePropSpinner;
    private int selectedSetPropId = VehiclePropertyIds.GECKO_VEHICLE_DRIVE_MODE;
    private int selectedResponsePropId = VehiclePropertyIds.GECKO_VEHICLE_DRIVE_MODE;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_tab_switch, container, false);
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        setPropSpinner = view.findViewById(R.id.spinner_set_prop);
        responsePropSpinner = view.findViewById(R.id.spinner_response_prop);

        // 属性名和ID映射
        final String[] propNames = {"GECKO_VEHICLE_DRIVE_MODE", "GECKO_POWER_STEERING_MODE"};
        final int[] propIds = {VehiclePropertyIds.GECKO_VEHICLE_DRIVE_MODE, VehiclePropertyIds.GECKO_POWER_STEERING_MODE};

        ArrayAdapter<String> adapter = new ArrayAdapter<>(getContext(), android.R.layout.simple_spinner_item, propNames);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        setPropSpinner.setAdapter(adapter);
        responsePropSpinner.setAdapter(adapter);

        setPropSpinner.setSelection(0);
        responsePropSpinner.setSelection(0);

        setPropSpinner.setOnItemSelectedListener(new android.widget.AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(android.widget.AdapterView<?> parent, View v, int position, long id) {
                selectedSetPropId = propIds[position];
                recreateViewModel();
            }
            @Override
            public void onNothingSelected(android.widget.AdapterView<?> parent) {}
        });
        responsePropSpinner.setOnItemSelectedListener(new android.widget.AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(android.widget.AdapterView<?> parent, View v, int position, long id) {
                selectedResponsePropId = propIds[position];
                recreateViewModel();
            }
            @Override
            public void onNothingSelected(android.widget.AdapterView<?> parent) {}
        });

        buttonTab1 = view.findViewById(R.id.button_tab1);
        buttonTab2 = view.findViewById(R.id.button_tab2);
        buttonTab3 = view.findViewById(R.id.button_tab3);
        buttonTab4 = view.findViewById(R.id.button_tab4);
        textCurrentTab = view.findViewById(R.id.text_current_tab);
        textWaitingConfirmation = view.findViewById(R.id.text_waiting_confirmation);

        // 初次创建ViewModel
        recreateViewModel();

        buttonTab1.setOnClickListener(v -> viewModel.onTabClicked(TabState.TAB1.getId()));
        buttonTab2.setOnClickListener(v -> viewModel.onTabClicked(TabState.TAB2.getId()));
        buttonTab3.setOnClickListener(v -> viewModel.onTabClicked(TabState.TAB3.getId()));
        buttonTab4.setOnClickListener(v -> viewModel.getTemperature());
    }

    // 动态重建ViewModel
    private void recreateViewModel() {
        if (viewModel != null) {
            viewModel.destroy();
        }
        TabConfig[] configs = new TabConfig[] {
            new TabConfig(selectedSetPropId, selectedResponsePropId),
            new TabConfig(selectedSetPropId, selectedResponsePropId),
            new TabConfig(selectedSetPropId, selectedResponsePropId)
        };
        viewModel = new TabSwitchViewModel(getActivity().getApplication(), configs);
        viewModel.setOnStateChangeListener(this);
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        if (viewModel != null) {
            viewModel.destroy();
        }
    }

    @Override
    public void onTabStateChanged(int currentActiveTab, Integer pendingTab, boolean isWaitingForConfirmation) {
        updateTabUI(currentActiveTab);

        textWaitingConfirmation.setVisibility(isWaitingForConfirmation ? View.VISIBLE : View.GONE);
        buttonTab1.setEnabled(!isWaitingForConfirmation);
        buttonTab2.setEnabled(!isWaitingForConfirmation);
        buttonTab3.setEnabled(!isWaitingForConfirmation);
        buttonTab4.setEnabled(!isWaitingForConfirmation);
    }

    @Override
    public void onShowMessage(String message) {
        if (getActivity() != null && message != null && !message.isEmpty()) {
            Toast.makeText(getActivity(), message, Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public void onTemperatureChanged(String temperature) {
        if (getActivity() != null) {
            Toast.makeText(getActivity(), temperature, Toast.LENGTH_SHORT).show();
        }
    }

    private void updateTabUI(int activeTabId) {
        TabState state = TabState.fromId(activeTabId);
        if (state != null) {
            textCurrentTab.setText(String.format("Current Tab: %s", state.name()));
            updateSingleTabButton(buttonTab1, TabState.TAB1.getId(), activeTabId);
            updateSingleTabButton(buttonTab2, TabState.TAB2.getId(), activeTabId);
            updateSingleTabButton(buttonTab3, TabState.TAB3.getId(), activeTabId);
        } else {
            android.util.Log.e("TabSwitchDemo", "Received an invalid tab ID: " + activeTabId);
        }
    }

    private void updateSingleTabButton(Button button, int buttonTabId, int activeTabId) {
        boolean isActive = (buttonTabId == activeTabId);
        int colorRes = isActive ? getActiveColorRes(buttonTabId) : getInactiveColorRes(buttonTabId);
        button.setBackgroundColor(getResources().getColor(colorRes, null));
    }

    private int getActiveColorRes(int tabId) {
        if (tabId == TabState.TAB1.getId()) return R.color.tab1_active;
        if (tabId == TabState.TAB2.getId()) return R.color.tab2_active;
        return R.color.tab3_active;
    }

    private int getInactiveColorRes(int tabId) {
        if (tabId == TabState.TAB1.getId()) return R.color.tab1_inactive;
        if (tabId == TabState.TAB2.getId()) return R.color.tab2_inactive;
        return R.color.tab3_inactive;
    }
}

