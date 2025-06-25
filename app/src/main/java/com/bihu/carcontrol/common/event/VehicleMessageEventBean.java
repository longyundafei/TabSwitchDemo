package com.bihu.carcontrol.common.event;

public class VehicleMessageEventBean {
    private int code;
    private Object cmd;

    public VehicleMessageEventBean(int code, Object cmd) {
        this.code = code;
        this.cmd = cmd;
    }

    public Object getCmd() {
        return cmd;
    }

    private void setCmd(Object cmd) {
        this.cmd = cmd;
    }

    public int getCode() {
        return code;
    }

    private void setCode(int code) {
        this.code = code;
    }
}
