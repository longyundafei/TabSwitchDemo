// packages/apps/TabSwitchDemo/src/com/android/tabswitchdemo/MainActivity.java
package com.android.tabswitchdemo;

import android.app.Activity; // Changed from androidx.appcompat.app.AppCompatActivity
import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.os.Bundle;


/**
 * MainActivity 是应用的入口 Activity。
 * 它负责加载 TabSwitchFragment 到其布局中。
 */
public class MainActivity extends Activity { // Changed from AppCompatActivity

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // 设置 Activity 的布局
        setContentView(R.layout.activity_main);

        // 检查 savedInstanceState 是否为 null，以避免在配置更改时重复添加 Fragment
        if (savedInstanceState == null) {
            // 创建 TabSwitchFragment 实例
            TabSwitchFragment tabSwitchFragment = new TabSwitchFragment();

            // 获取 FragmentManager 并开始事务
            getFragmentManager().beginTransaction() // Changed from getSupportFragmentManager()
                    .replace(R.id.fragment_container, tabSwitchFragment) // 将 Fragment 添加到布局中
                    .commit(); // 提交事务
        }
    }
}

