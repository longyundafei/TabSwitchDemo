package com.bihu.carcontrol.common.greendao.db;

import org.greenrobot.greendao.annotation.Entity;
import org.greenrobot.greendao.annotation.Id;
import org.greenrobot.greendao.annotation.Property;
import org.greenrobot.greendao.annotation.Generated;

/**
 * FileName: MaintainBean
 * Author: WangXu
 * Date: 2023/12/6 14:51
 * Description:保养信息
 */
@Entity(nameInDb = "DB_MAINTAIN")
public class MaintainBean {
    @Id(autoincrement = true)
    @Property(nameInDb = "ID")
    public Long id;
    //是否设置了保养时间
    @Property(nameInDb = "IS_MAINTAIN_TIME_SET")
    public boolean isMaintainTimeSet;
    //保养时间设置的时间
    @Property(nameInDb = "MAINTAIN_TIME_SET_TIME")
    public long maintainTimeSetTime;
    //保养时间设置的值
    @Property(nameInDb = "MAINTAIN_TIME_SET_VALUE")
    public int maintainTimeSetValue;
    //是否设置了保养里程
    @Property(nameInDb = "IS_MAINTAIN_MILEAGE_SET")
    public boolean isMaintainMileageSet;
    //保养里程设置的时间
    @Property(nameInDb = "MAINTAIN_MILEAGE_SET_TIME")
    public long maintainMileageSetTime;
    //保养里程设置的值
    @Property(nameInDb = "MAINTAIN_MILEAGE_SET_VALUE")
    public int maintainMileageSetValue;
    @Generated(hash = 1287510545)
    public MaintainBean(Long id, boolean isMaintainTimeSet,
            long maintainTimeSetTime, int maintainTimeSetValue,
            boolean isMaintainMileageSet, long maintainMileageSetTime,
            int maintainMileageSetValue) {
        this.id = id;
        this.isMaintainTimeSet = isMaintainTimeSet;
        this.maintainTimeSetTime = maintainTimeSetTime;
        this.maintainTimeSetValue = maintainTimeSetValue;
        this.isMaintainMileageSet = isMaintainMileageSet;
        this.maintainMileageSetTime = maintainMileageSetTime;
        this.maintainMileageSetValue = maintainMileageSetValue;
    }
    @Generated(hash = 1033343029)
    public MaintainBean() {
    }
    public Long getId() {
        return this.id;
    }
    public void setId(Long id) {
        this.id = id;
    }
    public boolean getIsMaintainTimeSet() {
        return this.isMaintainTimeSet;
    }
    public void setIsMaintainTimeSet(boolean isMaintainTimeSet) {
        this.isMaintainTimeSet = isMaintainTimeSet;
    }
    public long getMaintainTimeSetTime() {
        return this.maintainTimeSetTime;
    }
    public void setMaintainTimeSetTime(long maintainTimeSetTime) {
        this.maintainTimeSetTime = maintainTimeSetTime;
    }
    public int getMaintainTimeSetValue() {
        return this.maintainTimeSetValue;
    }
    public void setMaintainTimeSetValue(int maintainTimeSetValue) {
        this.maintainTimeSetValue = maintainTimeSetValue;
    }
    public boolean getIsMaintainMileageSet() {
        return this.isMaintainMileageSet;
    }
    public void setIsMaintainMileageSet(boolean isMaintainMileageSet) {
        this.isMaintainMileageSet = isMaintainMileageSet;
    }
    public long getMaintainMileageSetTime() {
        return this.maintainMileageSetTime;
    }
    public void setMaintainMileageSetTime(long maintainMileageSetTime) {
        this.maintainMileageSetTime = maintainMileageSetTime;
    }
    public int getMaintainMileageSetValue() {
        return this.maintainMileageSetValue;
    }
    public void setMaintainMileageSetValue(int maintainMileageSetValue) {
        this.maintainMileageSetValue = maintainMileageSetValue;
    }
}
