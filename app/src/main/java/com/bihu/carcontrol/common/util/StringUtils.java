package com.bihu.carcontrol.common.util;

import java.math.BigDecimal;

/**
 * FileName: formatDecimal1
 * Author: WangXu
 * Date: 2023/11/21 18:29
 * Description:
 */
public class StringUtils {
    public static String formatDecimal1(float value) {
        BigDecimal bd = new BigDecimal(String.valueOf(value));
        bd = bd.setScale(1, BigDecimal.ROUND_HALF_UP);
        return String.valueOf(bd.doubleValue());
    }
}
