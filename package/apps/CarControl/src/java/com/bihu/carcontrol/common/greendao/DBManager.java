package com.bihu.carcontrol.common.greendao;

import com.bihu.carcontrol.common.greendao.db.MaintainBeanDao;

/**
 * FileName: DBManager
 * Author: WangXu
 * Date: 2023/12/6 15:03
 * Description:
 */
public class DBManager {

    public static MaintainBeanDao getMaintainBeanDao() {
        return DBFactory.sharedSession().getMaintainBeanDao();
    }
}
