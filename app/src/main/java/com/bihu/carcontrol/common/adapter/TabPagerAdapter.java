package com.bihu.carcontrol.common.adapter;

import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentPagerAdapter;

import java.util.ArrayList;

/**
 * FileName: TabPagerAdapter
 * Author: WangXu
 * Date: 2023/6/13 11:11
 * Description: 车控首页切换适配器
 */
public class TabPagerAdapter extends FragmentPagerAdapter {
    private ArrayList<Fragment> fragments;

    public TabPagerAdapter(FragmentManager fm, ArrayList<Fragment> fragments) {
        super(fm);
        this.fragments = fragments;
    }

    @Override // androidx.fragment.app.FragmentPagerAdapter
    public Fragment getItem(int position) {
        return this.fragments.get(position);
    }

    @Override // androidx.viewpager.widget.PagerAdapter
    public int getCount() {
        return this.fragments.size();
    }
}
