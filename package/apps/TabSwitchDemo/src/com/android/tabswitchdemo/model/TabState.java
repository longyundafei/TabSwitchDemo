// packages/apps/TabSwitchDemo/src/com/android/tabswitchdemo/model/TabState.java
package com.android.tabswitchdemo.model;

/**
 * TabState 枚举定义了 Tab 按钮的可能状态。
 * 每个 Tab 都有一个 ID 和与其关联的颜色资源。
 */
public enum TabState {
    // 定义三个 Tab 按钮及其 ID
    TAB1(1),
    TAB2(2),
    TAB3(3);

    private final int id;

    TabState(int id) {
        this.id = id;
    }

    public int getId() {
        return id;
    }

    /**
     * 根据 ID 获取对应的 TabState。
     *
     * @param id Tab 的整数 ID。
     * @return 对应的 TabState 枚举，如果找不到则返回 null。
     */
    public static TabState fromId(int id) {
        for (TabState state : values()) {
            if (state.id == id) {
                return state;
            }
        }
        return null;
    }
}


