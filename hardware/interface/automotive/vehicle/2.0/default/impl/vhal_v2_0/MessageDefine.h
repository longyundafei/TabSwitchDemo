#ifndef MESSAGEDEFINE_H
#define MESSAGEDEFINE_H

enum BusIdentifier {
    BODY_CAN = 0x01, //Can Data Report
    EV_CAN = 0x02,  //ME11 canfd busidentifier
    MCU_UPGRADE_START = 0x03, //MCU Upgrade Start
    MCU = 0x04,    //MCU Upgrade Data
    MCU_UPGRADE_END =  0x05,  //MCU Upgrade End
    SOC_GET = 0x06,   //SOC Get Data/Config
    SOC_SET = 0x07,   //SOC Set Data/Config
    CH_CAN = 0x08,   //
    UDS_CAN = 0x10,
    ME11_CAN_FD = 0x09  // ME11 canfd busidentifier
};

#endif // MESSAGEDEFINE_H