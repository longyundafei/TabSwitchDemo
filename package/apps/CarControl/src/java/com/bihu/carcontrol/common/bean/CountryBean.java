package com.bihu.carcontrol.common.bean;

/**
 * FileName: CountryBean
 * Author: WangXu
 * Date: 2024/12/20 11:36
 * Description: 国家代码
 */
public class CountryBean {
    private String countryName;
    private int countryCode;

    public CountryBean(String countryName, int countryCode) {
        this.countryName = countryName;
        this.countryCode = countryCode;
    }

    public String getCountryName() {
        return countryName;
    }

    public void setCountryName(String countryName) {
        this.countryName = countryName;
    }

    public int getCountryCode() {
        return countryCode;
    }

    public void setCountryCode(int countryCode) {
        this.countryCode = countryCode;
    }
}
