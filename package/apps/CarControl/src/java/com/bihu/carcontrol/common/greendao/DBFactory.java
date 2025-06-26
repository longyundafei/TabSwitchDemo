package com.bihu.carcontrol.common.greendao;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;

import com.bihu.carcontrol.common.greendao.db.DaoMaster;
import com.bihu.carcontrol.common.greendao.db.DaoSession;

import org.greenrobot.greendao.database.Database;

/**
 * FileName: DBBase
 * Author: WangXu
 * Date: 2021/11/30 17:46
 * Description:greendao数据库初始化+版本更新
 */
public class DBFactory {
    public static final int DB_VERSION_1_0 = 1;

    public static final String DB_NAME = "car-control-db";
    private static DaoMaster.DevOpenHelper devOpenHelper;
    private static DaoSession sharedSession;

    public static void init(Context context) {
        if (devOpenHelper == null) {
            devOpenHelper = createDatabase(context);
        }
        if (sharedSession == null) {
            Database db = devOpenHelper.getWritableDb();
            DaoMaster daoMaster = new DaoMaster(db);
            sharedSession = daoMaster.newSession();
        }
    }

    private static DBOpenHelper createDatabase(Context context) {
        return new DBOpenHelper(context, DB_NAME, null);
    }

    public static DaoSession sharedSession() {
        if (sharedSession == null)
            throw new NullPointerException("please call init method before");
        return sharedSession;
    }

    private static class DBOpenHelper extends DaoMaster.DevOpenHelper {

        public DBOpenHelper(Context context, String name) {
            super(context, name);
        }

        public DBOpenHelper(Context context, String name, SQLiteDatabase.CursorFactory factory) {
            super(context, name, factory);
        }

        @Override
        public void onCreate(Database db) {
            super.onCreate(db);
        }

        @Override
        public void onUpgrade(Database db, int oldVersion, int newVersion) {

        }
    }
}