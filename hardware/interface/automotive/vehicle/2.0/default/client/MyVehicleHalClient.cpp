/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "automotive.vehicle@2.0-client"
#include <android/log.h>
#include <hidl/HidlTransportSupport.h>
#include <iostream>
#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include <utils/Log.h>
#include <unordered_set>
#include <hidl/ServiceManagement.h>
#include "stdio.h"
#include "stdint.h"
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include "stdlib.h"
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <cutils/properties.h>
#include <sys/stat.h>
#include "baiduUds.h"

using namespace android;
using namespace android::hardware;
using namespace android::hardware::automotive::vehicle::V2_0;
using ::android::sp;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;

sp<IVehicle> mVehicle; //对应service端的VehicleHalManager

struct ota_uds_client* currentClient = NULL;

cdc_ota_manager_t *cdc_ota_mamager = NULL;

uint8_t masterInitFlag = 0;

int createNextFile(char *getFileNme);

class VehicleOtaCallback : public IVehicleOtaCallback {
	Return<void> otaEvent(StatusCode status, uint32_t addr, const hidl_vec<uint8_t>& data, uint32_t len) {
		if(cdc_ota_mamager == NULL)
		{
		   ALOGE("zwt spi client VehicleOtaCallback otaEvent cdc_ota_mamager is NULL\r\n");
		   return Return<void>();
		}

		if(currentClient == NULL)
		{
			ALOGE("zwt spi client VehicleOtaCallback otaEvent currentClient is NULL\r\n");
			return Return<void>();
		}
		
		ALOGI("zwt spi client otaCallback status = %d,addr = 0x%x,data[0] = 0x%x,len = %d",status,addr,data[0],len);
		if(currentClient->callback != NULL)
		{
			currentClient->callback(addr,(unsigned char*)&data[0],len,currentClient->cbdata);
			
			if(cdc_ota_mamager->canRecordFlag == CAN_RECORD_ACTIVE)
			{
				ALOGI("zwt spi client otaEvent can record active\r\n");
				struct timeval tv_stop;
				gettimeofday(&tv_stop, NULL);
				long timestamp_stop = tv_stop.tv_sec;
				if(timestamp_stop > cdc_ota_mamager->canRecordStopTime)
				{
					if(cdc_ota_mamager->curWritefd != -1)
					{
						close(cdc_ota_mamager->curWritefd);
						cdc_ota_mamager->curWritefd = -1;	
					}
					return Return<void>();
				}
				
				ssize_t bytes_written;
				if(len > 64)
				{
					ALOGI("zwt spi client otaEvent msg len > 64\r\n");
					if(cdc_ota_mamager->curWritefd == -1)
					{
						ALOGE("zwt spi client otaEvent cdc_ota_mamager->curWritefd == -1\r\n");
					}
					
					char writeLog[256];
					struct timeval tv;
					gettimeofday(&tv, NULL);
					long timestamp = tv.tv_sec;
					struct tm timeinfo;
					localtime_r(&timestamp, &timeinfo);
					sprintf(writeLog,"RX:%04d%02d%02d_%02d%02d%02d:%04x ",timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec,addr);
					bytes_written = write(cdc_ota_mamager->curWritefd, writeLog, strlen(writeLog));
					if (bytes_written == -1) {
						ALOGE("zwt spi client write file %d error\r\n",cdc_ota_mamager->curWritefd);
						close(cdc_ota_mamager->curWritefd);
						cdc_ota_mamager->curWritefd = -1;
						return Return<void>();
					}
					
					uint32_t writeIdx = 0;
					while(writeIdx < len)
					{
						if(len - writeIdx > 64)
						{
							for(uint8_t i = 0; i<64;i++)
							{
								sprintf(&writeLog[0 + 3*i],"%02x ",data[writeIdx+i]);
							}
							bytes_written = write(cdc_ota_mamager->curWritefd, writeLog, strlen(writeLog));
							if (bytes_written == -1) {
								ALOGE("zwt spi client write file %d error\r\n",cdc_ota_mamager->curWritefd);
								close(cdc_ota_mamager->curWritefd);
								cdc_ota_mamager->curWritefd = -1;
								return Return<void>();
							}

							writeIdx += 64;

						}else{

							for(uint8_t i = 0; i<len-writeIdx;i++)
							{
								sprintf(&writeLog[0 + 3*i],"%02x ",data[writeIdx+i]);
							}
							bytes_written = write(cdc_ota_mamager->curWritefd, writeLog, strlen(writeLog));
							if (bytes_written == -1) {
								ALOGE("zwt spi client write file %d error\r\n",cdc_ota_mamager->curWritefd);
								close(cdc_ota_mamager->curWritefd);
								cdc_ota_mamager->curWritefd = -1;
								return Return<void>();
							}

							writeIdx += (len-writeIdx);

						}
						
						
					}

					char end = '\n';
					bytes_written = write(cdc_ota_mamager->curWritefd, &end, 1);
					if (bytes_written == -1) {
						ALOGE("zwt spi client write file %d error\r\n",cdc_ota_mamager->curWritefd);
						close(cdc_ota_mamager->curWritefd);
						cdc_ota_mamager->curWritefd = -1;
						return Return<void>();
					}
					
					struct stat buf;
			    	stat(cdc_ota_mamager->curWritefileName,&buf);
					if(buf.st_size >= cdc_ota_mamager->max_can_size)
					{
						close(cdc_ota_mamager->curWritefd);
						cdc_ota_mamager->curWritefd = createNextFile(cdc_ota_mamager->curWritefileName);
						if(cdc_ota_mamager->curWritefd == -1)
						{
							ALOGE("zwt spi client VehicleOtaCallback 0,createNextFile failed\r\n");
						}
					}
				
				}else{
					ALOGI("zwt spi client otaEvent msg len < 64\r\n");
					if(cdc_ota_mamager->curWritefd == -1)
					{
						ALOGE("zwt spi client otaEvent cdc_ota_mamager->curWritefd == -1\r\n");
					}
					
					char writeLog[256];
					struct timeval tv;
					gettimeofday(&tv, NULL);
					long timestamp = tv.tv_sec;
					struct tm timeinfo;
					localtime_r(&timestamp, &timeinfo);
					sprintf(writeLog,"RX:%04d%02d%02d_%02d%02d%02d:%04x ",timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec,addr);
					for(uint8_t i = 0; i<len;i++)
					{
						sprintf(&writeLog[24 + 3*i],"%02x ",data[i]);
					}
					//ALOGI("zwt spi client 11 writelog = %s\r\n",writeLog);
					bytes_written = write(cdc_ota_mamager->curWritefd, writeLog, strlen(writeLog));
					if (bytes_written == -1) {
						ALOGE("zwt spi client write file %d error\r\n",cdc_ota_mamager->curWritefd);
						close(cdc_ota_mamager->curWritefd);
						cdc_ota_mamager->curWritefd = -1;
						return Return<void>();
					}
					
					char end = '\n';
					bytes_written = write(cdc_ota_mamager->curWritefd, &end, 1);
					if (bytes_written == -1) {
						ALOGE("zwt spi client write file %d error\r\n",cdc_ota_mamager->curWritefd);
						close(cdc_ota_mamager->curWritefd);
						return Return<void>();
					}
					struct stat buf;
			    	stat(cdc_ota_mamager->curWritefileName,&buf);
					if(buf.st_size >= cdc_ota_mamager->max_can_size)
					{
						close(cdc_ota_mamager->curWritefd);
						cdc_ota_mamager->curWritefd = createNextFile(cdc_ota_mamager->curWritefileName);
						if(cdc_ota_mamager->curWritefd == -1)
						{
							ALOGE("zwt spi client VehicleOtaCallback 1,createNextFile failed\r\n");
						}
					}
				}
			}
			
		}else{
			ALOGI("zwt spi client ota_uds_client callback is NULL\r\n");
		}
		
		return Return<void>();
	}

};

sp<VehicleOtaCallback> vehicleOtaCallback;

//ecu update
struct ota_uds_client* ota_uds_client_create(
   uint32_t phy_addr,
   uint32_t func_addr,
   uint32_t resp_addr,
   enum OTA_STATE_T* rtn_code)
{
	if(mVehicle == nullptr)
	{
	   ALOGE("zwt spi client ota_uds_client_create mVehicle is NULL\r\n");
	   if(rtn_code != NULL)
	   {
		  *rtn_code = OTA_STATE_ERROR;
	   }
	   return NULL;
	}

	ALOGI("zwt spi client ota_uds_client_create\r\n");
	struct ota_uds_client *mOtaUdsClient;
	mOtaUdsClient = (struct ota_uds_client *)malloc(sizeof(struct ota_uds_client));
	if(mOtaUdsClient == NULL)
	{
		 ALOGE("zwt spi client mOtaUdsClient malloc error\r\n");
		 if(rtn_code != NULL)
		 {
		 	*rtn_code = OTA_STATE_ERROR;
		 }
		 return NULL;
	}

	mOtaUdsClient->phy_addr = phy_addr;
	mOtaUdsClient->func_addr = func_addr;
	mOtaUdsClient->resp_addr = resp_addr;

	if(rtn_code != NULL)
	{
		*rtn_code = OTA_STATE_OK;
	}

	currentClient = mOtaUdsClient;

	vehicleOtaCallback =  new VehicleOtaCallback();
	StatusCode status = mVehicle->setOtaCallBack(vehicleOtaCallback);
	ALOGI("zwt spi client setOtaCallBack status = %d",status);
	
	return mOtaUdsClient;
		
}

enum OTA_STATE_T ota_uds_set_resp_handler(
   struct ota_uds_client* client,
   fn_ota_uds_handle_response callback,
   void* cbdata)
{
	ALOGI("zwt spi client ota_uds_set_resp_handler\r\n");

	if(client == NULL)
	{
		ALOGE("zwt spi client ota_uds_set_resp_handler,client is NULL\r\n");
		return OTA_STATE_ERROR;

	}
	if(callback == NULL)
	{
		ALOGE("zwt spi client ota_uds_set_resp_handler,callback is NULL\r\n");
		return OTA_STATE_ERROR;
	}
	if(cbdata == NULL)
	{
		ALOGI("zwt spi client ota_uds_set_resp_handler,cbdata is NULL\r\n");
	}
	
	client->callback = callback;
	client->cbdata = cbdata;
	return OTA_STATE_OK;
}

//enum OTA_STATE_T ota_uds_send_diagmsg(struct ota_uds_client* /*client*/, ota_uds_msg* pmsg)
enum OTA_STATE_T ota_uds_send_diagmsg(struct ota_uds_client *client, ota_uds_msg* pmsg)
{
	(void)client;
    if(cdc_ota_mamager == NULL)
    {
	   ALOGE("zwt spi client ota_uds_send_diagmsg cdc_ota_mamager is NULL\r\n");
	   return OTA_STATE_ERROR;
    }

	if(mVehicle != nullptr)
	{
		std::vector<uint8_t> data;
		data.insert(data.begin(), pmsg->data, (pmsg->data) + pmsg->data_len);
		mVehicle->sendOtaData(pmsg->addr,data,pmsg->data_len);
		if(cdc_ota_mamager->canRecordFlag == CAN_RECORD_ACTIVE)
		{
			ALOGI("zwt spi client ota_uds_send_diagmsg can record active\r\n");
			struct timeval tv_stop;
			gettimeofday(&tv_stop, NULL);
			long timestamp_stop = tv_stop.tv_sec;
			if(timestamp_stop > cdc_ota_mamager->canRecordStopTime)
			{
				if(cdc_ota_mamager->curWritefd != -1)
				{
					close(cdc_ota_mamager->curWritefd);
					cdc_ota_mamager->curWritefd = -1;	
				}
				ALOGI("zwt spi client can record stop time %ld\r\n",timestamp_stop);
				return OTA_STATE_OK;
			}
			
			ssize_t bytes_written;
			if(pmsg->data_len > 64)
			{
				ALOGI("zwt spi client ota_uds_send_diagmsg msg len > 64\r\n");
				if(cdc_ota_mamager->curWritefd == -1)
				{
					ALOGE("zwt spi client ota_uds_send_diagmsg cdc_ota_mamager->curWritefd == -1\r\n");
				}
				
				char writeLog[256];
				struct timeval tv;
				gettimeofday(&tv, NULL);
				long timestamp = tv.tv_sec;
				struct tm timeinfo;
				localtime_r(&timestamp, &timeinfo);
				sprintf(writeLog,"TX:%04d%02d%02d_%02d%02d%02d:%04x ",timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec,pmsg->addr);
				bytes_written = write(cdc_ota_mamager->curWritefd, writeLog, strlen(writeLog));
				if (bytes_written == -1) {
					ALOGE("zwt spi client write file %d error\r\n",cdc_ota_mamager->curWritefd);
					close(cdc_ota_mamager->curWritefd);
					cdc_ota_mamager->curWritefd = -1;
					return OTA_STATE_ERROR;
				}
				
				uint32_t writeIdx = 0;
				while(writeIdx < pmsg->data_len)
				{
					//ALOGI("zwt spi client writeIdx = %d\r\n",writeIdx);
					memset(writeLog,0,256);
					if(pmsg->data_len - writeIdx > 64)
					{
						for(uint8_t i = 0; i<64;i++)
						{
							sprintf(&writeLog[0 + 3*i],"%02x ",pmsg->data[writeIdx+i]);
						}
						//ALOGI("zwt spi client 0 strlen(writeLog) = %zu\r\n",strlen(writeLog));
						bytes_written = write(cdc_ota_mamager->curWritefd, writeLog, strlen(writeLog));
						if (bytes_written == -1) {
							ALOGE("zwt spi client write file %d error\r\n",cdc_ota_mamager->curWritefd);
							close(cdc_ota_mamager->curWritefd);
							cdc_ota_mamager->curWritefd = -1;
							return OTA_STATE_ERROR;
						}

						writeIdx += 64;

					}else{

						for(uint8_t i = 0; i<pmsg->data_len-writeIdx;i++)
						{
							sprintf(&writeLog[0 + 3*i],"%02x ",pmsg->data[writeIdx+i]);
						}
						//ALOGI("zwt spi client 1 strlen(writeLog) = %zu\r\n",strlen(writeLog));
						bytes_written = write(cdc_ota_mamager->curWritefd, writeLog, strlen(writeLog));
						if (bytes_written == -1) {
							ALOGE("zwt spi client write file %d error\r\n",cdc_ota_mamager->curWritefd);
							close(cdc_ota_mamager->curWritefd);
							cdc_ota_mamager->curWritefd = -1;
							return OTA_STATE_ERROR;
						}

						writeIdx += (pmsg->data_len - writeIdx);

					}
					
					
				}

				char end = '\n';
				bytes_written = write(cdc_ota_mamager->curWritefd, &end, 1);
				if (bytes_written == -1) {
					ALOGE("zwt spi client write file %d error\r\n",cdc_ota_mamager->curWritefd);
					close(cdc_ota_mamager->curWritefd);
					cdc_ota_mamager->curWritefd = -1;
					return OTA_STATE_ERROR;
				}
				
				struct stat buf;
		    	stat(cdc_ota_mamager->curWritefileName,&buf);
				if(buf.st_size >= cdc_ota_mamager->max_can_size)
				{
					close(cdc_ota_mamager->curWritefd);
					cdc_ota_mamager->curWritefd = createNextFile(cdc_ota_mamager->curWritefileName);
					if(cdc_ota_mamager->curWritefd == -1)
					{
						ALOGE("zwt spi client ota_uds_send_diagmsg 0,createNextFile failed\r\n");
						return OTA_STATE_ERROR;
					}
				}
			
			}else{
				ALOGI("zwt spi client ota_uds_send_diagmsg msg len < 64\r\n");
				if(cdc_ota_mamager->curWritefd == -1)
				{
					ALOGE("zwt spi client ota_uds_send_diagmsg cdc_ota_mamager->curWritefd == -1\r\n");
				}
				
				char writeLog[256];
				struct timeval tv;
				gettimeofday(&tv, NULL);
				long timestamp = tv.tv_sec;
				struct tm timeinfo;
				localtime_r(&timestamp, &timeinfo);
				sprintf(writeLog,"TX:%04d%02d%02d_%02d%02d%02d:%04x ",timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec,pmsg->addr);
				for(uint8_t i = 0; i<pmsg->data_len;i++)
				{
					sprintf(&writeLog[24+3*i],"%02x ",pmsg->data[i]);
				}
				//ALOGI("zwt spi client writelog = %s\r\n",writeLog);
				ssize_t bytes_written = write(cdc_ota_mamager->curWritefd, writeLog, strlen(writeLog));
				if (bytes_written == -1) {
					ALOGE("zwt spi client ota write file %d error\r\n",cdc_ota_mamager->curWritefd);
					close(cdc_ota_mamager->curWritefd);
					cdc_ota_mamager->curWritefd = -1;
					return OTA_STATE_ERROR;
				}
				
				char end = '\n';
				bytes_written = write(cdc_ota_mamager->curWritefd, &end, 1);
				if (bytes_written == -1) {
					ALOGE("zwt spi client ota write file %d error\r\n",cdc_ota_mamager->curWritefd);
					close(cdc_ota_mamager->curWritefd);
					cdc_ota_mamager->curWritefd = -1;
					return OTA_STATE_ERROR;
				}
				
				struct stat buf;
		    	stat(cdc_ota_mamager->curWritefileName,&buf);
				if(buf.st_size >= cdc_ota_mamager->max_can_size)
				{
					close(cdc_ota_mamager->curWritefd);
					cdc_ota_mamager->curWritefd = createNextFile(cdc_ota_mamager->curWritefileName);
					if(cdc_ota_mamager->curWritefd == -1)
					{
						ALOGE("zwt spi client ota_uds_send_diagmsg 1,createNextFile failed\r\n");
						return OTA_STATE_ERROR;
					}
				}
			}
		}
	}
	
	return OTA_STATE_OK;
}

enum OTA_STATE_T ota_uds_client_release(struct ota_uds_client* client)
{
	ALOGI("zwt spi client ota_uds_client_release\r\n");

	if(client != NULL)
	{
		free(client);
		client = NULL;
		currentClient = NULL;
	}

	if(vehicleOtaCallback != nullptr)
	{
		//delete vehicleOtaCallback; ?? cannot delete expression of type 'sp<VehicleOtaCallback>'
		vehicleOtaCallback = nullptr;
	}

	StatusCode status = mVehicle->setOtaCallBack(NULL);
	ALOGI("zwt spi client setOtaCallBack status = %d",status);
	
	return OTA_STATE_OK;
}



//cdc update
enum OTA_STATE_T ota_get_cdc_info(cdc_info* info)
{
	if(cdc_ota_mamager == NULL)
	{
		ALOGE("zwt spi client ota_get_cdc_info cdc_ota_mamager is NULL\r\n");
		return OTA_STATE_ERROR;
	}

	pthread_mutex_lock(&(cdc_ota_mamager->mutexotaGetCdcInfo)); 
	
	ALOGI("zwt spi client ota_get_cdc_info\r\n");

	if(info == NULL)
	{
		ALOGE("zwt spi client ota_get_cdc_info param is null\r\n");
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexotaGetCdcInfo)); 
		return OTA_STATE_ERROR;
	}

	//ecu_name
	char ecu_name[4] = "CDC";
	memcpy(info->ecu_name,ecu_name,sizeof(ecu_name));
	
	//part_num
	char partNum[11] = "7102010106";
	memcpy(info->part_num,partNum,sizeof(partNum));
	//NA

	//sup_id
	char sup_id[6] = "GECKO";
	memcpy(info->sup_id,sup_id,sizeof(sup_id));

	char propValue[128] = "\0";
	memset(propValue, 0, sizeof(propValue));
	if(property_get("gecko.mcu.version", propValue, nullptr)!=0){
		ALOGI("zwt spi client ota_get_cdc_info gecko.mcu.version:%s\r\n",propValue);
	} else {
		ALOGE("zwt spi client ota_get_cdc_info gecko.mcu.version error\r\n");
		//return OTA_STATE_ERROR;
	}

	memset(propValue, 0, sizeof(propValue));
	if(property_get("gsm.version.baseband", propValue, nullptr)!=0){
		ALOGI("zwt spi client ota_get_cdc_info gsm.version.baseband:%s\r\n",propValue);
	} else {
		ALOGE("zwt spi client ota_get_cdc_info gsm.version.baseband error\r\n");
		//return OTA_STATE_ERROR;
	}

	//sw_ver
	memset(propValue, 0, sizeof(propValue));
	if(property_get("ro.build.id", propValue, nullptr)!=0){
		ALOGI("zwt spi client ota_get_cdc_info gecko.soc.version:%s\r\n",propValue);
	} else {
		ALOGE("zwt spi client ota_get_cdc_info gecko.soc.version error\r\n");
		char sw_ver[] = "SW:01.00.00";
		memcpy(propValue,sw_ver,strlen(sw_ver));
		//return OTA_STATE_ERROR;
	}
	if(strlen(propValue) <= 21)
	{	char vehicleNum[] = "7102010106_";
		memcpy(info->sw_ver,vehicleNum,11);
		memcpy(info->sw_ver+11,propValue,strlen(propValue)-3);
	}else{
		ALOGE("zwt spi client ota_get_cdc_info gecko.soc.version too long:%zu\r\n",strlen(propValue));
	}

	//hw_ver
	memset(propValue, 0, sizeof(propValue));
	if(property_get("gecko.soc.hardware.version", propValue, nullptr)!=0){
		ALOGI("zwt spi client ota_get_cdc_info gecko.soc.hardware.version:%s\r\n",propValue);
	} else {
		ALOGE("zwt spi client ota_get_cdc_info gecko.soc.hardware.version error\r\n");
		char hw_ver[] = "HW:C.01.00";
		memcpy(propValue,hw_ver,strlen(hw_ver));
		//return OTA_STATE_ERROR;
	}
	if(strlen(propValue) <= 21)
	{
		char vehicleNum[] = "7102010106_";
		memcpy(info->hw_ver,vehicleNum,11);
		memcpy(info->hw_ver+11,propValue,strlen(propValue));
	}else{
		ALOGE("zwt spi client ota_get_cdc_info gecko.soc.hardware.version too long:%zu\r\n",strlen(propValue));
	}
	
	pthread_mutex_unlock(&(cdc_ota_mamager->mutexotaGetCdcInfo)); 
	
	return OTA_STATE_OK;
}

enum OTA_STATE_T ota_cdc_update(const char* pkg_path,fn_update_progress func_ptr,void* user_data)
{
	if(cdc_ota_mamager == NULL)
	{
		ALOGE("zwt spi client ota_cdc_update cdc_ota_mamager is NULL\r\n");
		return OTA_STATE_ERROR;
	}

	pthread_mutex_lock(&(cdc_ota_mamager->mutexOtaCdcUpdate)); 

	ALOGI("zwt spi client ota_cdc_update\n");

	if(pkg_path == NULL)
	{
		ALOGE("zwt spi client ota_cdc_update pkg_path is null\r\n");
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaCdcUpdate)); 
		return OTA_STATE_ERROR;
	}

	if(func_ptr == NULL)
	{
		ALOGE("zwt spi client ota_cdc_update func_ptr is null\r\n");
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaCdcUpdate)); 
		return OTA_STATE_ERROR;
	}

	ALOGI("zwt spi client ota_cdc_update,pkg_path = %s\n",pkg_path);
	int retPrSet = 0;
    retPrSet = property_set("ota.cdc.pkgpath", pkg_path);
	if(retPrSet == 0)
	{
		ALOGI("zwt spi client ota_cdc_update,property_set success\n");
	}else{
		retPrSet = property_set("ota.cdc.pkgpath", pkg_path);
		if(retPrSet == 0)
		{
			ALOGI("zwt spi client ota_cdc_update,property_set again success\n");
		}else{
			ALOGI("zwt spi client ota_cdc_update,property_set again failed\n");
			pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaCdcUpdate)); 
			return OTA_STATE_ERROR;
		}
	}


//获取升级进度    
	int otaProgValue = 0;
	char otaProgArrayValue[128] = "\0";
	std::string strOtaProgValue;
	do{
		memset(otaProgArrayValue, 0, sizeof(otaProgArrayValue));
		property_get("ota.cdc.progvalue", otaProgArrayValue, nullptr);
		strOtaProgValue = otaProgArrayValue;
		if(strOtaProgValue.size()!=0){
		   ALOGI("zwt spi client otaProgArrayValue = %s\r\n",otaProgArrayValue);
		   otaProgValue = std::stof(strOtaProgValue)*100;
		   func_ptr(otaProgValue,user_data);
		}
		
		usleep(1000*1000); //1s回调一次
	}while(otaProgValue != 100);

	pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaCdcUpdate)); 
	return OTA_STATE_OK;
}

void getPropertybyPropId(int propId,int areaId,VehiclePropValue& refValue,StatusCode& refStatus) 
{
	if(mVehicle == nullptr)
	{
		ALOGE("zwt spi client getPropertybyPropId mVehicle is NULL\r\n");
		return;
	}
	
	bool isCalled = false;
    VehiclePropValue requestedValue{};
    requestedValue.areaId = areaId;
    requestedValue.prop = propId;
    mVehicle->get(requestedValue, [&refStatus, &refValue, &isCalled](
                                          StatusCode status, const VehiclePropValue& value) {
        refStatus = status;
        refValue = value;
        isCalled = true;
    });
	
	return;
}

enum OTA_STATE_T ota_get_car_status(struct car_status* status)
{
	if(cdc_ota_mamager == NULL)
	{
		ALOGE("zwt spi client ota_get_car_status cdc_ota_mamager is NULL\r\n");
		return OTA_STATE_ERROR;
	}

	pthread_mutex_lock(&(cdc_ota_mamager->mutexOtaGetCarStatus)); 

	ALOGI("zwt spi client ota_get_car_status\r\n");
	if(mVehicle == nullptr)
	{
		ALOGE("zwt spi client ota_get_car_status mVehicle is NULL\r\n");
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaGetCarStatus)); 
		return OTA_STATE_ERROR;
	}

	if(status == NULL)
	{
		ALOGE("zwt spi client ota_get_car_status status is null\r\n");
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaGetCarStatus)); 
		return OTA_STATE_ERROR;
	}

    StatusCode 		 rS_Speed,rS_CURRENT_GEAR,rS_EV_BATTERY_LEVEL,rS_GECKO_LOW_VOL_BATTERY_VOL,rS_GECKO_VCU_CHRG_STS,rS_GECKO_BCM_SYSPOWERSTS;
    VehiclePropValue rV_Speed,rV_CURRENT_GEAR,rV_EV_BATTERY_LEVEL,rV_GECKO_LOW_VOL_BATTERY_VOL,rV_GECKO_VCU_CHRG_STS,rV_GECKO_BCM_SYSPOWERSTS;

	getPropertybyPropId((int)VehicleProperty::CURRENT_GEAR, 0, rV_CURRENT_GEAR, rS_CURRENT_GEAR);
    int32_t gear = static_cast<int32_t>(rV_CURRENT_GEAR.value.int32Values[0]);
	ALOGI("zwt spi client ota_get_car_status gear = 0x%x\r\n",gear);
	if(gear == 0)
	{
		status->gear = GEAR_STATUS_OTHE;
		status->handbreak_status = HANDBREAK_RELEASE;
	}else if(gear == 0x0001){
		status->gear = GEAR_STATUS_N;
		status->handbreak_status = HANDBREAK_RELEASE;
	}else if(gear == 0x0002){
		status->gear = GEAR_STATUS_R;
		status->handbreak_status = HANDBREAK_RELEASE;
	}else if(gear == 0x0004){
		status->gear = GEAR_STATUS_P;
		status->handbreak_status = HANDBREAK_HOLD;
	}else if(gear == 0x0008){
		status->gear = GEAR_STATUS_D;
		status->handbreak_status = HANDBREAK_RELEASE;
	}else{
		status->gear = GEAR_STATUS_OTHE;
		status->handbreak_status = HANDBREAK_RELEASE;
	}
	

	getPropertybyPropId((int)VehicleProperty::PERF_VEHICLE_SPEED_DISPLAY, 0, rV_Speed, rS_Speed);
    float speed = static_cast<float>(rV_Speed.value.floatValues[0]);
	ALOGI("zwt spi client ota_get_car_status speed = %f\r\n",speed);
	status->car_speed = (int)speed;

	getPropertybyPropId((int)VehicleProperty::EV_BATTERY_LEVEL, 0, rV_EV_BATTERY_LEVEL, rS_EV_BATTERY_LEVEL);
    float batteryLevel = static_cast<float>(rV_EV_BATTERY_LEVEL.value.floatValues[0]);
	ALOGI("zwt spi client ota_get_car_status batteryLevel = %f\r\n",batteryLevel);
	status->high_volt_batt_soc = (int)batteryLevel;

	getPropertybyPropId((int)VehicleProperty::GECKO_12V_BAT_LOW_VOL_STA, 0, rV_GECKO_LOW_VOL_BATTERY_VOL, rS_GECKO_LOW_VOL_BATTERY_VOL);
    int32_t voltag = static_cast<int32_t>(rV_GECKO_LOW_VOL_BATTERY_VOL.value.int32Values[0]);
	ALOGI("zwt spi client ota_get_car_status voltag = %d\r\n",voltag);
	status->low_volt_batt_volt = (double)voltag;

	getPropertybyPropId((int)VehicleProperty::GECKO_BMS_DCS_ACT_OPRT_MODE, 0, rV_GECKO_VCU_CHRG_STS, rS_GECKO_VCU_CHRG_STS);
    int32_t chrgSts = static_cast<int32_t>(rV_GECKO_VCU_CHRG_STS.value.int32Values[0]);
	ALOGI("zwt spi client ota_get_car_status chrgSts = %d\r\n",chrgSts);
	if(chrgSts == 0x05)
	{
		status->charge_status = CHARGE_STATUS_CHARGING;
	}else{
		status->charge_status = CHARGE_STATUS_IDLE; 
	}
	
	getPropertybyPropId((int)VehicleProperty::GECKO_BCM_SYSPOWERSTS, 0, rV_GECKO_BCM_SYSPOWERSTS, rS_GECKO_BCM_SYSPOWERSTS);
    int32_t lampletStatus = static_cast<int32_t>(rV_GECKO_BCM_SYSPOWERSTS.value.int32Values[0]);
	ALOGI("zwt spi client ota_get_car_status lampletStatus = %d\r\n",lampletStatus);
	if(lampletStatus == 0x05)
	{
		status->power_status = POWER_STATUS_LOW_VOLT_ON;
	}else if(lampletStatus == 0x03) {
		status->power_status = POWER_STATUS_HIGH_VOLT_ON;
	}else{
		status->power_status = POWER_STATUS_OFF;
	}

	pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaGetCarStatus)); 

	return OTA_STATE_OK;
}

enum OTA_STATE_T ota_get_vin_code(char* buffer, unsigned int buff_len)
{
	if(cdc_ota_mamager == NULL)
	{
		ALOGE("zwt spi client ota_get_vin_code cdc_ota_mamager is NULL\r\n");
		return OTA_STATE_ERROR;
	}

	pthread_mutex_lock(&(cdc_ota_mamager->mutexOtaGetVinCode)); 

	ALOGI("zwt spi client ota_get_vin_code\r\n");

	if(buffer == NULL)
	{
		ALOGE("zwt spi client ota_get_vin_code buffer is null\r\n");
		return OTA_STATE_ERROR;
	}

	char propValue[256] = "\0";
	memset(propValue, 0, sizeof(propValue));
	if(property_get("gecko.vin", propValue, nullptr)!=0){
		ALOGI("zwt spi client ota_get_vin_code vin:%s\r\n",propValue);
	} else {
		ALOGE("zwt spi client ota_get_vin_code vin error\r\n");
		return OTA_STATE_ERROR;
	}

	if(buff_len < strlen(propValue))
	{
		return OTA_STATE_ERROR;
	}else{
		memcpy(buffer,propValue,buff_len);
	}

	pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaGetVinCode)); 

	return OTA_STATE_OK;
}

enum OTA_STATE_T ota_get_sn_code(char* buffer, unsigned int buff_len)
{
	if(cdc_ota_mamager == NULL)
	{
		ALOGE("zwt spi client ota_get_sn_code cdc_ota_mamager is NULL\r\n");
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaGetSnCode)); 
		return OTA_STATE_ERROR;
	}

	pthread_mutex_lock(&(cdc_ota_mamager->mutexOtaGetSnCode)); 

	ALOGI("zwt spi client ota_get_sn_code\r\n");

	if(buffer == NULL)
	{
		ALOGE("zwt spi client ota_get_sn_code buffer is null\r\n");
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaGetSnCode)); 
		return OTA_STATE_ERROR;
	}

	char propV[256] = "\0";
	memset(propV, 0, sizeof(propV));
    if(property_get("gecko.serial.number", propV, nullptr)!=0){
      	ALOGI("zwt spi client ota_get_sn_code sn:%s\r\n",propV);
    } else {
        ALOGE("zwt spi client ota_get_sn_code sn error\r\n");
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaGetSnCode)); 
		return OTA_STATE_ERROR;
    }

	if(buff_len < strlen(propV))
	{
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaGetSnCode)); 
		return OTA_STATE_ERROR;
	}else{
		memcpy(buffer,propV,buff_len);
	}

	pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaGetSnCode)); 

	return OTA_STATE_OK;

}

enum OTA_STATE_T ota_wakeup_vehicle_network(int mode)
{
	if(cdc_ota_mamager == NULL)
	{
		ALOGE("zwt spi client ota_wakeup_vehicle_network cdc_ota_mamager is NULL\r\n");
		return OTA_STATE_ERROR;
	}

	pthread_mutex_lock(&(cdc_ota_mamager->mutexOtaWakeupVehicleNetwork)); 
	ALOGI("zwt spi client ota_wakeup_vehicle_network,mode = %d\r\n",mode);

	if(mode == 1)
	{
		VehiclePropValue setVehiclePropValue;
		setVehiclePropValue.prop = (int)VehicleProperty::GECKO_CDC_ACTIVEWAKEUP;
		setVehiclePropValue.areaId = 0;
		setVehiclePropValue.value.int32Values.resize(1);
		setVehiclePropValue.value.int32Values[0] = 1;
		mVehicle->set(setVehiclePropValue);

		VehiclePropValue setVehiclePropValue1;
		setVehiclePropValue1.prop = (int)VehicleProperty::GECKO_CDC_WAKEUPREAS ;
		setVehiclePropValue1.areaId = 0;
		setVehiclePropValue1.value.int32Values.resize(1);
		setVehiclePropValue1.value.int32Values[0] = 3;
		mVehicle->set(setVehiclePropValue1);
	}else if(mode == 0) {
		VehiclePropValue setVehiclePropValue;
		setVehiclePropValue.prop = (int)VehicleProperty::GECKO_CDC_ACTIVEWAKEUP;
		setVehiclePropValue.areaId = 0;
		setVehiclePropValue.value.int32Values.resize(1);
		setVehiclePropValue.value.int32Values[0] = 0;
		mVehicle->set(setVehiclePropValue);
	}else{
		ALOGI("zwt spi client ota_wakeup_vehicle_network,mode = %d,mode error\r\n",mode);
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaWakeupVehicleNetwork));
		return OTA_STATE_ERROR;
	}
	
	pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaWakeupVehicleNetwork));
	return OTA_STATE_OK;
}

enum OTA_STATE_T ota_set_ota_mode(OTA_MODE mode, unsigned int duration_time_s)
{
	if(cdc_ota_mamager == NULL)
	{
		ALOGE("zwt spi client ota_set_ota_mode cdc_ota_mamager is NULL\r\n");
		return OTA_STATE_ERROR;
	}

	pthread_mutex_lock(&(cdc_ota_mamager->mutexOtaSetOtaMode)); 
	ALOGI("zwt spi client ota_set_ota_mode,mode = %d，duration_time_s = %d\r\n",mode,duration_time_s);
	if(mVehicle == nullptr)
	{
		ALOGE("zwt spi client ota_set_ota_mode mVehicle is NULL\r\n");
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaSetOtaMode));
		return OTA_STATE_ERROR;
	}

	if((mode != OTA_MODE_ACTIVATE) && (mode != OTA_MODE_DEACTIVATE))
	{
		ALOGI("zwt spi client ota_set_ota_mode,mode = %d,duration_time_s = %d,mode value is error\r\n",mode,duration_time_s);
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaSetOtaMode));
		return OTA_STATE_ERROR;
	}
	
    //禁止spi上json消息传输
	uint8_t jsonMsgFlag;
	if(mode == OTA_MODE_ACTIVATE)
	{
		jsonMsgFlag = 1;
		//stop json msg transfer
		mVehicle->sendOtaCtlCmd(jsonMsgFlag);
	}else if(mode == OTA_MODE_DEACTIVATE){
		jsonMsgFlag = 0;
		//resume json msg transfer
		mVehicle->sendOtaCtlCmd(jsonMsgFlag);
	}
	
	//设置属性
	VehiclePropValue setVehiclePropValue;
	setVehiclePropValue.prop = (int)VehicleProperty::GECKO_CDC_KEEPAWAKEREASDO;
	setVehiclePropValue.areaId = 0;
	setVehiclePropValue.value.int32Values.resize(2);
	if(mode == OTA_MODE_ACTIVATE)
	{
		setVehiclePropValue.value.int32Values[0] = 1;
	}else if(mode == OTA_MODE_DEACTIVATE) {
		setVehiclePropValue.value.int32Values[0] = 0;
	}
	setVehiclePropValue.value.int32Values[1] = duration_time_s;
	mVehicle->set(setVehiclePropValue);
	
	pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaSetOtaMode));
	return OTA_STATE_OK;
}

enum OTA_STATE_T ota_get_ota_mode(OTA_MODE *mode)
{
	if(cdc_ota_mamager == NULL)
	{
		ALOGE("zwt spi client ota_get_ota_mode cdc_ota_mamager is NULL\r\n");
		return OTA_STATE_ERROR;
	}

	pthread_mutex_lock(&(cdc_ota_mamager->mutexOtaGetOtaMode));	

	if(mVehicle == nullptr)
	{
		ALOGE("zwt spi client ota_get_ota_mode mVehicle is NULL\r\n");
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaGetOtaMode));
		return OTA_STATE_ERROR;
	}
	
	ALOGI("zwt spi client ota_get_ota_mode\r\n");
	
	if(mode == NULL)
	{
		ALOGE("zwt spi client ota_get_ota_mode mode is null\r\n");
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaGetOtaMode));
		return OTA_STATE_ERROR;
	}

	StatusCode		 rS_GECKO_BCM_SYSPOWERSTS;
	VehiclePropValue rV_GECKO_BCM_SYSPOWERSTS;

	getPropertybyPropId((int)VehicleProperty::GECKO_BCM_SYSPOWERSTS, 0, rV_GECKO_BCM_SYSPOWERSTS, rS_GECKO_BCM_SYSPOWERSTS);
	int32_t bcmSyspowerSts = static_cast<int32_t>(rV_GECKO_BCM_SYSPOWERSTS.value.int32Values[0]);
	ALOGI("zwt spi client ota_get_ota_mode bcmSyspowerSts = %d\r\n",bcmSyspowerSts);
	if(bcmSyspowerSts == 0x03 || bcmSyspowerSts == 0x05)
	{
		*mode = OTA_MODE_ACTIVATE;
	}else{
		*mode = OTA_MODE_DEACTIVATE;
	}
	
	pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaGetOtaMode));

	return OTA_STATE_OK;

}

enum OTA_STATE_T ota_set_power_status(POWER_STATUS power_status)
{
	if(cdc_ota_mamager == NULL)
	{
		ALOGE("zwt spi client ota_set_power_status cdc_ota_mamager is NULL\r\n");
		return OTA_STATE_ERROR;
	}

	pthread_mutex_lock(&(cdc_ota_mamager->mutexotaSetPowerStatus));

	ALOGI("zwt spi client ota_set_power_status\r\n");

	if(mVehicle == nullptr)
	{
		ALOGE("zwt spi client ota_set_power_status mVehicle is NULL\r\n");
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexotaSetPowerStatus));
		return OTA_STATE_ERROR;
	}

	ALOGI("zwt spi client ota_set_power_status,power_status = %d\r\n",power_status);
	if( power_status == POWER_STATUS_HIGH_VOLT_ON ){
		ALOGI("zwt spi client ota_set_power_status,POWER_STATUS_HIGH_VOLT_ON\r\n");
		VehiclePropValue setVehiclePropValue;
		setVehiclePropValue.prop = (int)VehicleProperty::GECKO_BCM_OTA_MODE_SET;
		setVehiclePropValue.areaId = 0;
		setVehiclePropValue.value.int32Values.resize(1);
		setVehiclePropValue.value.int32Values[0] = 0x01;
		mVehicle->set(setVehiclePropValue);
	}else if(power_status == POWER_STATUS_LOW_VOLT_ON ){
		ALOGI("zwt spi client ota_set_power_status,POWER_STATUS_LOW_VOLT_ON\r\n");
		VehiclePropValue setVehiclePropValue;
		setVehiclePropValue.prop = (int)VehicleProperty::GECKO_BCM_OTA_MODE_SET;
		setVehiclePropValue.areaId = 0;
		setVehiclePropValue.value.int32Values.resize(1);
		setVehiclePropValue.value.int32Values[0] = 0x03;
		mVehicle->set(setVehiclePropValue);
	}else if(power_status == POWER_STATUS_OFF){
		ALOGI("zwt spi client ota_set_power_status,POWER_STATUS_OFF\r\n");
		VehiclePropValue setVehiclePropValue;
		setVehiclePropValue.prop = (int)VehicleProperty::GECKO_BCM_OTA_MODE_SET;
		setVehiclePropValue.areaId = 0;
		setVehiclePropValue.value.int32Values.resize(1);
		setVehiclePropValue.value.int32Values[0] = 0x02;
		mVehicle->set(setVehiclePropValue);
	}
	
	pthread_mutex_unlock(&(cdc_ota_mamager->mutexotaSetPowerStatus));

	return OTA_STATE_OK;
}

enum OTA_STATE_T ota_check_tester_exist(int *tester_exist_status)
{
	if(cdc_ota_mamager == NULL)
	{
		ALOGE("zwt spi client ota_check_tester_exist cdc_ota_mamager is NULL\r\n");
		return OTA_STATE_ERROR;
	}

	pthread_mutex_lock(&(cdc_ota_mamager->mutexOtaCheckTesterExist));
	ALOGI("zwt spi client ota_check_tester_exist\r\n");

	if(mVehicle == nullptr)
	{
		ALOGE("zwt spi client ota_check_tester_exist mVehicle is NULL\r\n");
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaCheckTesterExist));
		return OTA_STATE_ERROR;
	}
	
	if(tester_exist_status == NULL)
	{
		ALOGE("zwt spi client ota_check_tester_exist tester_exist_status is null\r\n");
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaCheckTesterExist));
		return OTA_STATE_ERROR;
	}

	StatusCode 		 rS_DIAGTESTERONLINE;
    VehiclePropValue rV_DIAGTESTERONLINE;

	getPropertybyPropId((int)VehicleProperty::GECKO_CCU_DIAGTESTERONLINE, 0, rV_DIAGTESTERONLINE, rS_DIAGTESTERONLINE);
    int32_t ccuDiagtOnline = static_cast<int32_t>(rV_DIAGTESTERONLINE.value.int32Values[0]);
	ALOGI("zwt spi client ota_check_tester_exist ccuDiagtOnline = %d\r\n",ccuDiagtOnline);
	*tester_exist_status = ccuDiagtOnline;
	
	pthread_mutex_unlock(&(cdc_ota_mamager->mutexOtaCheckTesterExist));


	return OTA_STATE_OK;
}

enum OTA_STATE_T ota_set_appointment_time(time_t unixtime_s)
{
	ALOGI("zwt spi client ota_set_appointment_time\r\n");

	if(mVehicle == nullptr)
	{
		ALOGE("zwt spi client ota_set_appointment_time mVehicle is NULL\r\n");
		return OTA_STATE_ERROR;
	}

	VehiclePropValue setVehiclePropValue;
	setVehiclePropValue.prop = (int)VehicleProperty::GECKO_OTA_RTC;
	setVehiclePropValue.areaId = 0;
	setVehiclePropValue.value.int32Values.resize(1);
	setVehiclePropValue.value.int32Values[0] = (int32_t)unixtime_s;
	mVehicle->set(setVehiclePropValue);

	return OTA_STATE_OK;
}

int createNextFile(char *getFileNme)
{
	int writeFd = -1;
	char fileDir[128];
	struct stat st;

	if(cdc_ota_mamager == NULL)
	{
		ALOGE("zwt spi client createNextFile cdc_ota_mamager is NULL\r\n");
		return OTA_STATE_ERROR;
	}
	
	unsigned int fileMax = cdc_ota_mamager->max_file_num;
	unsigned int fileMaxSize = cdc_ota_mamager->max_can_size;
	memset(fileDir,0,sizeof(fileDir));
	memcpy(fileDir,cdc_ota_mamager->dir_path,128);

	uint8_t fileDirLen = strlen(fileDir);
	if(fileDir[fileDirLen-1] != '/')
	{
		fileDir[fileDirLen] = '/';
	}
 
    // mkdir
    if (stat(fileDir, &st) >= 0) {
        //dir exist
		//ALOGI("zwt spi client fileDir: %s exist\r\n",fileDir);
    } else {
       //dir not exist
     	if(mkdir(fileDir,0777) == 0)
     	{
			ALOGI("zwt spi client mkdir %s success\r\n",fileDir);
		}else{
			ALOGE("zwt spi client mkdir %s failed\r\n",fileDir);
			return OTA_STATE_ERROR;
		}
    }

	//opendir
	DIR *dir = opendir(fileDir);
	if (dir == NULL) {
		ALOGE("zwt spi client open dir %s failed\r\n",fileDir);
		return -1;
	}

	fileFind mfileFind[fileMax];
	struct dirent *entry;
	uint8_t fileIdx = 0;
	while((entry = readdir(dir)) != NULL)
	{
		if(entry->d_type == DT_REG)
		{ 
			char *ptr,c = '_'; 
			ptr = strchr(entry->d_name, c);
			if(ptr) 
			{
				uint8_t pos = (uint8_t)(ptr-entry->d_name);
				char fileNum[128];
				memset(fileNum,0,sizeof(fileNum));
				if(*(ptr+1) == 'c' && *(ptr+2) == 'a' && *(ptr+3) == 'n')
				{
					memcpy(fileNum,entry->d_name,pos);
					mfileFind[fileIdx].fileIdx = atoi(fileNum);
					memcpy(mfileFind[fileIdx].fileName,fileDir,strlen(fileDir));
					memcpy(mfileFind[fileIdx].fileName+strlen(fileDir),entry->d_name,128-strlen(fileDir));
					fileIdx++;
				}
			}
		}
	}
	closedir(dir);

	//ALOGE("zwt spi client dir has fileNum = %d\r\n",fileIdx);

	int minFileIdx = 0;
	int maxFileIdx = 0;
	if(fileIdx < fileMax)
	{
		if(fileIdx == 0)
		{
			//no file exist
			struct timeval tv;
			gettimeofday(&tv, NULL);
			long timestamp = tv.tv_sec;
			//ALOGE("zwt spi client Timestamp: %ld\n", timestamp);
			struct tm timeinfo;
			localtime_r(&timestamp, &timeinfo);
			//ALOGE("zwt spi client year:%4d,month:%2d,day:%2d,hour:%2d,min:%2d,sec:%2d\n", timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
			char fileName[128];
			sprintf(fileName,"%s%d_can_log_%04d%02d%02d_%02d%02d%02d.asc",fileDir,1,timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
			writeFd = open(fileName,O_RDWR | O_CREAT,0777);
			if(writeFd== -1)
			{
				ALOGE("zwt spi client open file %s error\r\n",fileName);
				return -1;
			}
			memcpy(getFileNme,fileName,128);

		}else{
			for(int i=0;i<fileIdx;i++)
			{
				if(mfileFind[i].fileIdx > maxFileIdx)
				{
					maxFileIdx = mfileFind[i].fileIdx;
				}
			}

			for(int i=0;i<fileIdx;i++)
			{
				if(mfileFind[i].fileIdx == maxFileIdx)
				{
					struct stat buf;
					stat(mfileFind[i].fileName,&buf);
					if(buf.st_size < fileMaxSize)
					{
						writeFd = open(mfileFind[i].fileName ,O_RDWR | O_CREAT,0777);
						if(writeFd == -1)
						{
							ALOGE("zwt spi client open file %s error\r\n",mfileFind[i].fileName);
							return -1;
						}
						memcpy(getFileNme,mfileFind[i].fileName,128);
					}else{
						struct timeval tv;
						gettimeofday(&tv, NULL);
						long timestamp = tv.tv_sec;
						//ALOGE("zwt spi client Timestamp: %ld\n", timestamp);
						struct tm timeinfo;
						localtime_r(&timestamp, &timeinfo);
						//ALOGE("zwt spi client year:%4d,month:%2d,day:%2d,hour:%2d,min:%2d,sec:%2d\n", timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
						char fileName[128];
						sprintf(fileName,"%s%d_can_log_%04d%02d%02d_%02d%02d%02d.asc",fileDir,mfileFind[i].fileIdx+1,timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
						writeFd = open(fileName,O_RDWR | O_CREAT,0777);
						if(writeFd == -1)
						{
							ALOGE("zwt spi client open file %s error\r\n",fileName);
							return -1;
						}
						memcpy(getFileNme,fileName,128);
					}
				
					break;
				 }
			 }
		}
		
	}else{
			//create max filenum file
			char tmpFileName[128];
			for(int i=0;i<fileIdx;i++)
			{
				if(mfileFind[i].fileIdx > maxFileIdx)
				{
					maxFileIdx = mfileFind[i].fileIdx;
					memset(tmpFileName,0,sizeof(tmpFileName));
					memcpy(tmpFileName,mfileFind[i].fileName,128);
				}
			}
	
			struct timeval tv;
			gettimeofday(&tv, NULL);
			long timestamp = tv.tv_sec;
			//ALOGE("zwt spi client Timestamp: %ld\n", timestamp);
			struct tm timeinfo;
			localtime_r(&timestamp, &timeinfo);
			//ALOGE("zwt spi client year:%4d,month:%2d,day:%2d,hour:%2d,min:%2d,sec:%2d\n", timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
			char fileName[128];
			sprintf(fileName,"%s%d_can_log_%04d%02d%02d_%02d%02d%02d.asc",fileDir,maxFileIdx+1,timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
			writeFd = open(fileName,O_RDWR | O_CREAT,0777);
			if(writeFd == -1)
			{
				ALOGE("zwt spi client open file %s error\r\n",fileName);
				return -1;
			}
			memcpy(getFileNme,fileName,128);

			//delete min filenum file
			minFileIdx = mfileFind[0].fileIdx;
			memset(tmpFileName,0,sizeof(tmpFileName));
			memcpy(tmpFileName,mfileFind[0].fileName,128);
			for(int i=1;i<fileIdx;i++)
			{
				if(mfileFind[i].fileIdx < minFileIdx)
				{
					minFileIdx = mfileFind[i].fileIdx;
					memset(tmpFileName,0,sizeof(tmpFileName));
					memcpy(tmpFileName,mfileFind[i].fileName,128);
				}
			}
			//ALOGE("zwt spi client minFileIdx = %d\r\n",minFileIdx);
			int result = unlink(tmpFileName);
			if (result == 0) {
				ALOGE("zwt spi client delete file: %s success\n", tmpFileName);
			} else {
				ALOGE("zwt spi client delete file: %s failed\n", tmpFileName);
			}
			
	}
	
	ALOGI("zwt spi client createNextFile %s,writeFd = %d\r\n",getFileNme,writeFd);
	return writeFd;

}


enum OTA_STATE_T ota_set_can_record(CAN_RECORD_MODE mode, unsigned int timeout_s)
{
	if(cdc_ota_mamager == NULL)
	{
		ALOGE("zwt spi client ota_set_can_record cdc_ota_mamager is NULL\r\n");
		return OTA_STATE_ERROR;
	}

	pthread_mutex_lock(&(cdc_ota_mamager->mutexCdcSetCanRecord));

	cdc_ota_mamager->canRecordFlag = mode;
	cdc_ota_mamager->timeout_s = timeout_s;

	ALOGI("zwt spi client ota_set_can_record,mode = %d,timeout_s = %d\r\n",mode,timeout_s);

	if(mode == CAN_RECORD_ACTIVE)
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		long timestamp = tv.tv_sec;
		cdc_ota_mamager->canRecordStartTime = timestamp;
		cdc_ota_mamager->canRecordStopTime = timestamp+timeout_s;
	
		cdc_ota_mamager->curWritefd = createNextFile(cdc_ota_mamager->curWritefileName);
		if(cdc_ota_mamager->curWritefd == -1)
		{
			ALOGE("zwt spi client ota_set_can_record,createNextFile failed\r\n");
		}
	}else if(mode == CAN_RECORD_DEACTIVE){
		if(cdc_ota_mamager->curWritefd != -1)
		{
			close(cdc_ota_mamager->curWritefd);
			cdc_ota_mamager->curWritefd = -1;
		}
	}

	pthread_mutex_unlock(&(cdc_ota_mamager->mutexCdcSetCanRecord));

	return OTA_STATE_OK;
}

enum OTA_STATE_T ota_set_can_record_file_param(const char *dir_path,unsigned int max_file_num,unsigned int max_can_size)
{
	if(cdc_ota_mamager == NULL)
	{
		ALOGE("zwt spi client ota_set_can_record_file_param cdc_ota_mamager is NULL\r\n");
		return OTA_STATE_ERROR;
	}
	
	pthread_mutex_lock(&(cdc_ota_mamager->mutexcanRecordFileParam));

	ALOGI("zwt spi client ota_set_can_record_file_param,dir_path = %s,max_file_num = %d,max_can_size = %d\r\n",dir_path,max_file_num,max_can_size);

	uint8_t dirLen;

	if(dir_path == NULL)
	{
		ALOGE("zwt spi client ota_set_can_record_file_param  dir_path is null\r\n");
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexcanRecordFileParam));
		return OTA_STATE_ERROR;
	}
	
	dirLen = strlen(dir_path)+1;
	if(dirLen > 128)
	{
		ALOGE("zwt spi client ota_set_can_record_file_param  dir_path len: %d is too long\r\n",dirLen);
		pthread_mutex_unlock(&(cdc_ota_mamager->mutexcanRecordFileParam));
		return OTA_STATE_ERROR;
	}
	memcpy(cdc_ota_mamager->dir_path,dir_path,dirLen);

	cdc_ota_mamager->max_file_num = max_file_num;

	cdc_ota_mamager->max_can_size = max_can_size;

	pthread_mutex_unlock(&(cdc_ota_mamager->mutexcanRecordFileParam));
	
	return OTA_STATE_OK;
}

enum OTA_STATE_T ota_master_init()
{
	ALOGI("zwt spi client ota_master_init\r\n");

	if(masterInitFlag == 1)
	{
		ALOGI("zwt spi client ota_master_init has inited\r\n");
		return OTA_STATE_OK;
	}
	
	cdc_ota_mamager = (cdc_ota_manager_t *)malloc(sizeof(cdc_ota_manager_t));
	memset(cdc_ota_mamager,0,sizeof(cdc_ota_manager_t));
	cdc_ota_mamager->canRecordFlag = CAN_RECORD_DEACTIVE;
	cdc_ota_mamager->timeout_s = 0;
	cdc_ota_mamager->mutexCdcSetCanRecord = PTHREAD_MUTEX_INITIALIZER;
	cdc_ota_mamager->mutexcanRecordFileParam = PTHREAD_MUTEX_INITIALIZER;
	cdc_ota_mamager->mutexOtaCheckTesterExist = PTHREAD_MUTEX_INITIALIZER;
	cdc_ota_mamager->mutexotaSetPowerStatus = PTHREAD_MUTEX_INITIALIZER;
	cdc_ota_mamager->mutexOtaGetOtaMode = PTHREAD_MUTEX_INITIALIZER;
	cdc_ota_mamager->mutexOtaSetOtaMode = PTHREAD_MUTEX_INITIALIZER;
	cdc_ota_mamager->mutexOtaWakeupVehicleNetwork = PTHREAD_MUTEX_INITIALIZER;
	cdc_ota_mamager->mutexOtaGetSnCode = PTHREAD_MUTEX_INITIALIZER;
	cdc_ota_mamager->mutexOtaGetVinCode = PTHREAD_MUTEX_INITIALIZER;
	cdc_ota_mamager->mutexOtaGetCarStatus = PTHREAD_MUTEX_INITIALIZER;
	cdc_ota_mamager->mutexOtaCdcUpdate = PTHREAD_MUTEX_INITIALIZER;
	cdc_ota_mamager->mutexotaGetCdcInfo = PTHREAD_MUTEX_INITIALIZER;

	ALOGI("zwt spi client init hidl,get Vehicle service\r\n");
	mVehicle = IVehicle::getService("default");

	masterInitFlag = 1;
	
	return OTA_STATE_OK;
}

void ota_master_deinit(void)
{
	ALOGI("zwt spi client ota_master_deinit\r\n");

	if(masterInitFlag == 0)
	{
		ALOGI("zwt spi client ota_master_deinit not init\r\n");
	}

	if(cdc_ota_mamager != NULL)
	{
		free(cdc_ota_mamager);
		cdc_ota_mamager = NULL;
	}

	mVehicle = nullptr;

	masterInitFlag = 0;
	
	return;
}

enum OTA_STATE_T ota_get_cdc_storage_size(long long *storage_size)
{
	ALOGI("zwt spi client ota_get_cdc_storage_size\r\n");
	FILE *fp;
    char buffer[1035];
	char filesystem[128], size[128], used[128], avail[128], capacity[128], mounted_on[128];
	uint8_t idx = 0;
 
    // 执行df命令
    fp = popen("/bin/df -h /data", "r");
    if (fp == NULL) {
        ALOGE("zwt spi client failed to run df\n");
        return OTA_STATE_ERROR;
    }
 
    // 读取命令的输出
    while (fgets(buffer, sizeof(buffer), fp) != NULL) 
	{
		if(idx == 1)
		{
	        // 解析每一行的输出
	        if (sscanf(buffer, " %s %s %s %s %s %s", filesystem, size, used, avail, capacity, mounted_on) == 6) {
	         ALOGI("zwt spi client Filesystem: %s, Size:%s, Used: %s, Avail: %s, Capacity: %s, Mounted on: %s\n",
	                   filesystem, size, used, avail, capacity, mounted_on);
	        }
			break;
		}
		idx++;
    }
	ALOGI("zwt spi client avail = %s\n",avail);
	char tmpbuf[32];
	memset(tmpbuf,0,sizeof(tmpbuf));
	if(strlen(avail) < 32)
	{
		memcpy(tmpbuf,avail,strlen(avail)-1);
	}else{
		ALOGE("zwt spi client strlen(avail) too long\n");
		return OTA_STATE_ERROR;
	}
	if(avail[strlen(avail)-1] == 'G')
	{
		*storage_size = (long long)(atof(tmpbuf)*1024*1024*1024);
	}else if(avail[strlen(avail)-1] == 'M'){
		*storage_size = (long long)(atof(tmpbuf)*1024*1024);
	}else if(avail[strlen(avail)-1] == 'K'){
		*storage_size = (long long)(atof(tmpbuf)*1024);
	}
	
	ALOGI("zwt spi client *storage_size = %lld\n",*storage_size);
 
    // 关闭流
    pclose(fp);
	return OTA_STATE_OK;
}






