#ifndef VEHICLESTATUSINFO_H
#define VEHICLESTATUSINFO_H

#include <condition_variable>
#include "GB32960MessageParser.h"
#include "messageproductstate.h"
#include "vehicle_controller.h"
#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
using namespace android::hardware::automotive::vehicle::V2_0;
//#define CELL_COUNTS 104
//#define PROB_COUNTS 8
class VehicleStatusInfo {
    public:
        VehicleStatusInfo(VehicleController& vehicle);
        int initVehicleStatusInfoTask();

    public:
        std::vector<uint16_t> cellVotageList;
        std::vector<uint8_t> probeTemperatureList;
        std::vector<int> rechargeableFaultList;
        std::vector<int> driveMotorFaultList;
        std::vector<int> engineFaultList;
        std::vector<int> otherFaultList;
        std::vector<uint16_t> errTabIndexList;
        //std::mutex mutexVehicleInfoGet;
        std::mutex mutexUpdateRechargeableFault;
        std::mutex mutexUpdateDriveMotorFault;
        std::mutex mutexUpdateOtherFault;
        std::mutex mutexUpdateErrTabIndex;
        MessageProductState mMsgProductState = MessageProductState::Normal;
        int mSleepTime;
        int totalCellNum = 0;
        int probNum = 0;

    private:
        std::vector<std::thread> mThreads;
        VehicleController mVehicle;
    private:
        int getpacktotalcellnumValue();
        int getpacktotaltempnumValue();
        int getframeCellCountValue();
        int getvoltage1Value();
        int getvoltage2Value();
        int getvoltage3Value();
        int gettemperatureFrameNo();
        int gettemperature1Value();
        int gettemperature2Value();
        int gettemperature3Value();
        int gettemperature4Value();
        int gettemperature5Value();
        int gettemperature6Value();
#ifdef EV48
        int gettemperature7Value();
#endif
        int getrechargeableFaultCountValue();
        int getrechargeableFaultCodesValue();
        int getOtherFaultCountValue();
        int getdriveMotorFaultCountValue();
        int getdriveMotorFaultCodesValue();
        int getOtherFaultCodesValue();
        int getBMSErrTabIndexValue();
        //int updateVehicleStatusInfoThread();
        void updateCellVoltageThread();
        void updateProbTemperatureThread();
        void updateRechargeableFaultCodesThread();
        void updatedriveMotorFaultCodesThread();
        void updateOtherFaultCodesThread();
        void updateBMSErrorTabThread();

        /** Returns underlying (integer) value for given enum. */
        template<typename ENUM, typename U = typename std::underlying_type<ENUM>::type>
        inline constexpr U toInt(ENUM const value) {
            return static_cast<U>(value);
        }
};
#endif