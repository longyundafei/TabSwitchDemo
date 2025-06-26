package com.bihu.carcontrol.common.util;

import com.bihu.carcontrol.base.activity.BaseActivity;

import java.util.concurrent.CopyOnWriteArrayList;

public class ActivityStack {

    private final static String ACTIVITY_MAIN = "MAIN";
    private int activeActivityCount;
    private CopyOnWriteArrayList<ActivityRecord> mQueue = new CopyOnWriteArrayList<ActivityRecord>();

    public void add(BaseActivity a) {
        mQueue.add(new ActivityRecord(a));
    }

    public void addMain(BaseActivity main) {
        mQueue.add(new ActivityRecord(main, ACTIVITY_MAIN));
    }

    public void add(BaseActivity a, String tag) {
        mQueue.add(new ActivityRecord(a, tag));
    }

    public void remove(BaseActivity a) {

        for (ActivityRecord r : mQueue) {
            if (r.activity == a) {
                mQueue.remove(r);
                break;
            }
        }
    }

    public int getCount() {
        return mQueue.size();
    }

    public void startCount() {
        activeActivityCount++;
    }

    public void stopCount() {
        activeActivityCount--;
    }

    public int getActiveActivityCount() {
        return activeActivityCount;
    }

    public BaseActivity getTopActivity() {
        int lastIndex = mQueue.size() - 1;
        if (lastIndex >= 0) {
            try {
                return mQueue.get(lastIndex).activity;
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return null;
    }

    public boolean hasMain() {
        for (ActivityRecord r : mQueue) {
            if (ACTIVITY_MAIN.equals(r.tag)) {
                return true;
            }
        }
        return false;
    }

    public void finishByTag(String tag) {

        if (tag == null)
            return;

        for (ActivityRecord r : mQueue) {

            if (!tag.equals(r.tag)) {
                continue;
            }

            if (r.activity != null && !r.activity.isFinishing()) {
                r.activity.finish();
            }
            mQueue.remove(r);
        }
    }

    public void finishAll() {
        for (ActivityRecord r : mQueue) {
            if (r.activity != null && !r.activity.isFinishing()) {
                r.activity.finish();
            }
            mQueue.remove(r);
        }
    }

    public void finishExcept(final BaseActivity except) {

        for (ActivityRecord r : mQueue) {

            if (r.activity == except) {
                continue;
            }

            if (r.activity != null && !r.activity.isFinishing()) {
                r.activity.finish();
            }
            mQueue.remove(r);
        }
    }

    private class ActivityRecord {

        private ActivityRecord(BaseActivity activity) {
            this.activity = activity;
        }

        private ActivityRecord(BaseActivity activity, String tag) {
            this.activity = activity;
            this.tag = tag;
        }

        public String tag;
        public BaseActivity activity;
    }
}
