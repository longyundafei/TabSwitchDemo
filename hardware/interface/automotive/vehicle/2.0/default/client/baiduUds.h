#ifndef BAIDU_UDS_H
#define BAIDU_UDS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum OTA_STATE_T
{
    OTA_STATE_OK = 0,
    OTA_STATE_ERROR
}OTA_STATE_T_E;

/**
* @brief UDS服务消息定义
*/
typedef struct {
   uint32_t addr;          // UDS请求地址,物理地址或功能地址
   uint32_t data_len;     // UDS数据Buffer长度
   unsigned char *data;     // UDS数据Buffer地址
} ota_uds_msg;


/**
* @brief UDS读取消息回调函数的原型定义
*
* @param resp_addr  
[IN] 响应地址
* @param resp_msg    
[IN] 响应消息buffer地址，由调用者分配内存
* @param resp_msg_len
[IN] 响应消息buffer长度
* @param cbdata          
[IN] 调用 @ota_uds_set_resp_handler 时传入的cbdata
参数
*                          
*/
typedef enum OTA_STATE_T (*fn_ota_uds_handle_response)(
	uint32_t resp_addr,
	unsigned char* resp_msg,
	uint32_t resp_msg_len,
	void* cbdata
);

/**
* @brief 用于操作UDS通道
*
* 此处只是一个声明，具体的结构体内容需要由CDC供应商根据需要进行定义
* 具体的实现在供应商所提供的动态库中定义，不用暴露在接口层面
*/
typedef struct ota_uds_client
{
	uint32_t phy_addr;
	uint32_t func_addr;
   	uint32_t resp_addr;
	fn_ota_uds_handle_response callback;
	void* cbdata;
}ota_uds_client_t;


/**
* @brief 用于创建 struct ota_uds_client* 类型句柄
*
* 所有后续的UDS相关的接口，都需要传入该接口创建的句柄
* 该接口需要在调用其他UDS相关接口之前进行调用
*
* @param phy_addr    
[IN] 请求物理地址
* @param func_addr
[IN] 功能地址
* @param resp_addr
[IN] 响应地址
* @param rtn_code        
[OUT] 此函数调用成功，则返回OTA_STATE_OK
*                         失败时返回错误码
*
* @return 返回非NULL指针表示成功，返回NULL表示失败
*/
struct ota_uds_client* ota_uds_client_create(
   uint32_t phy_addr,
   uint32_t func_addr,
   uint32_t resp_addr,
   enum OTA_STATE_T* rtn_code
   );


/**
* @brief 设置UDS读取消息回调函数
*
* 需要在 ota_uds_send_diagmsg 接口调用之前进行设置
* 所设置的回调函数，会在接收到响应消息的时候进行调用UDS的响应消息
*
* @param client           [IN] UDS client句柄
* @param callback         [IN] UDS读取消息回调函数
* @param cbdata           [IN] 额外参数，需要在调用回调函数时作为最后一个参数传入      
         
*
* @return OTA_STATE_OK 表示成功，返回错误码表示失败
*/
enum OTA_STATE_T ota_uds_set_resp_handler(
   struct ota_uds_client* client,
   fn_ota_uds_handle_response callback,
   void* cbdata
);


/**
* @brief 发送UDS服务消息
*
* 在调用该接口之前，用户需设置pmsg指向的结构体内容
* pmsg->addr为要发送到的请求地址，可以是功能地址也可以是物理地址
*
* 注意：CDC只有持续20s没有检测到OTA Master调用诊断接口，才能进入休眠状态
*
* @param client           [IN] UDS Client句柄
* @param pmsg             [IN] 需要发送的消息结构体指针
*
* @return OTA_STATE_OK 表示成功，返回错误码表示失败
*/
enum OTA_STATE_T ota_uds_send_diagmsg(struct ota_uds_client* client, ota_uds_msg* pmsg);

/**
* @brief 释放UDS Client句柄
*
2.2.3 接口说明
OTA_STATE_T 类型由CDC供应商定义，成功为OTA_STATE_OK(0)，其他全部为错误码，需要供应
商根据实际情况添加错误码定义；
接口定义中 struct ota_uds_client 是一个结构体声明，具体接口体定义需要车机供应商根据需
要在动态库内部实现，实现细节不需要暴露到接口层面；
ota_uds_client_create 接口用于创建一个 struct ota_uds_client * 类型句柄，所有UDS相
关其他接口都使用此句柄作为第一个参数；
ota_uds_client_create 接口的 rtn_code 参数为返回参数，用于在创建句柄失败时返回错误
码；
若UDS的使用还需要其他操作，应共同协商制定新的接口。
2.3 CDC自升级相关接口
2.3.1 接口定义
* @param client
[IN] UDS Client句柄
*
* @return OTA_STATE_OK 表示成功，返回错误码表示失败
*/
enum OTA_STATE_T ota_uds_client_release(struct ota_uds_client* client);






/*****************************************************************/
/**
* @brief CDC升级相关信息存储结构体定义
*/
typedef struct
{
   char ecu_name[32];      // CDC零件名称
   char sw_ver[32];        // CDC软件版本号
   char hw_ver[32];        // CDC硬件版本号
   char part_num[32];      // CDC零件号
   char sup_id[32];        // CDC供应商代码
} cdc_info;


/**
* @brief 获取CDC升级相关信息
*
* 1、所有CDC提供的接口，需要加锁保证线程同步；
* 2、所获取到的CDC相关信息，会存储在所传入的 @ref cdc_INFO 结构体参数中；
*
* @param info             [OUT] 存储获取到的CDC升级相关信息
*
* @return OTA_STATE_OK 表示成功，返回错误码表示失败
*/
enum OTA_STATE_T ota_get_cdc_info(cdc_info* info);


/**
* @brief 返回升级进度
*
* @param prog        
[IN] 升级进度，取值0-100
* @param user_data    
[IN] 在设置回调接口中传入的参数，透传给回调接口
* @return 默认返回0
2.3.2 接口说明
OTA Master程序会调用 ota_get_cdc_info 接口获取CDC升级所需相关信息并上报，该上报信息
后续会用于升级任务匹配；
请求到升级任务后，OTA Master程序会将CDC自升级的升级包下载并存储到本地，并将存储升级文
件的路径传给CDC提供的自升级接口 ota_cdc_update 进行CDC自升级处理；
升级过程中如果需要重启CDC，则在下次启动时，OTA Master会通过检查CDC版本号查询上次升级
是否成功；
OTA程序存放目录需要CDC供应商分配，所有OTA涉及到的文件存储都需要存放于此目录内部；
2.4 信息获取接口
2.4.1 接口定义
*/ 
typedef int(*fn_update_progress)(int prog, void* user_data);


/**
* @brief CDC自升级接口
*
* 1、所有CDC提供的接口，需要加锁保证线程同步；
* 2、使用参数pkg_path指定升级包路径进行升级；
* 3、pkg_path路径所指向的文件是供应商所提供的原始升级文件包；
* 4、此升级接口为同步阻塞接口，升级完成后返回；
* 5、如果升级过程中会重启则在函数返回前直接重启；
* 6、OTA Master重启后通过CDC版本号判断升级结果；
*
* @param pkg_path     [IN] 存储升级包的完整路径
* @param func_ptr     [IN] 升级进度回调
* @param user_data     [IN] 在设置回调接口中传入的参数，透传给进度回调
*
* @return OTA_STATE_OK 表示升级成功，返回错误码表示失败
*/
enum OTA_STATE_T ota_cdc_update(
   const char* pkg_path,
   fn_update_progress func_ptr,
   void* user_data);


/***********************************************************************/

/**
* @brief KL15上电状态定义，既低压上电(Key-ON)状态
*/
//typedef enum {
//   KL15_STATUS_OFF = 0,        // KL15（低压）未上电
//   KL15_STATUS_ON = 1,         // KL15（低压）已上电
//} KL15_STATUS;


/**
* @brief 高压上电状态定义，针对电动车
*/
//typedef enum {
//   HIGH_VOLT_STATUS_OFF = 0,   // 高压没有上电
//   HIGH_VOLT_STATUS_ON = 1     // 高压已上电
//} HIGH_VOLT_STATUS;
/**
* @brief 上电状态定义
* POWER_STATUS_HIGH_VOLT_ON 对应 OTA 模式⾼压上电
* POWER_STATUS_LOW_VOLT_ON 对应 OTA 模式低压上电
* POWER_STATUS_OFF 对应 退出 OTA 模式
*/
typedef enum{
   POWER_STATUS_HIGH_VOLT_ON = 0, // 高压上电
   POWER_STATUS_LOW_VOLT_ON = 1, // 低压上电
   POWER_STATUS_OFF = 2 // 退出 OTA 模式
} POWER_STATUS;   

/**
* @brief 充电状态定义，针对电动车
*/
typedef enum {
   CHARGE_STATUS_IDLE = 0,     // 未充电
   CHARGE_STATUS_CHARGING = 1  // 充电中
} CHARGE_STATUS;
   
/**
* @brief 档位定义
*/
typedef enum {
   GEAR_STATUS_P = 0,          // P档
   GEAR_STATUS_N,              // N档
   GEAR_STATUS_D,              // D档
   GEAR_STATUS_R,              // R档
   GEAR_STATUS_OTHE            // 其他档位
} GEAR_STATUS;
   
/**
* @brief 手刹状态
*/
typedef enum {
   HANDBREAK_RELEASE = 0,      // 手刹释放
   HANDBREAK_HOLD              // 手刹拉起
} HANDBREAK_STATUS;
/**
* @brief 车辆状态结构体定义
*/
struct car_status
{
   int car_speed;             // 车速
   int high_volt_batt_soc;    // 动力电池电量百分比（取值：0-100）
   double low_volt_batt_volt; // 小电瓶电压
   GEAR_STATUS gear;          // 档位
   CHARGE_STATUS charge_status;     // 充电状态
   //KL15_STATUS kl15_status;         // KL15上电状态(低压上电状态)
  // HIGH_VOLT_STATUS high_volt_status;  // 高压上电状态
   HANDBREAK_STATUS handbreak_status;  // 手刹状态
   POWER_STATUS power_status; // 电压状态
};


/**
* @brief 获取车辆状态
*
* 1、所有CDC提供的接口，需要加锁保证线程同步；
* 2、所获取到的车辆状态，会存储在所传入的 struct car_status 结构体参数中；
*
* @param info             [OUT] 存储获取到的车辆状态，内存由调用者分配
*
* @return OTA_STATE_OK 表示成功，返回错误码表示失败
*/
enum OTA_STATE_T ota_get_car_status(struct car_status* status);



/**
2.5 OTA模式及电压控制接口
2.5.1 接口定义
* @brief 获取车辆VIN码
*
* 1、所有CDC提供的接口，需要加锁保证线程同步；
* 2、所获取到的车辆VIN码，会存储在传入的buffer参数中；
* 3、调用者需在调用此接口前分配好参数buffer的内存空间，如果车辆的VIN码长于buff_len，则会报
错；
*
* @param buffer           [OUT] 存储获取到的车辆VIN码，内存由调用者分配
* @param buff_len         [IN] buffer参数的内存空间大小，单位为字节
*
* @return OTA_STATE_OK 表示成功，返回错误码表示失败
*/
enum OTA_STATE_T ota_get_vin_code(char* buffer, unsigned int buff_len);



/**
* @brief 获取车辆SN码
*
* 1、所有CDC提供的接口，需要加锁保证线程同步；
* 2、所获取到的车辆SN码，会存储在传入的buffer参数中；
* 3、调用者需在调用此接口前分配好参数buffer的内存空间，如果车辆的SN码长于buff_len，则会报错；
*
* @param buffer           [OUT] 存储获取到的车辆SN码，内存由调用者分配
* @param buff_len         [IN] buffer参数的内存空间大小，单位为字节
*
* @return OTA_STATE_OK 表示成功，返回错误码表示失败
*/
enum OTA_STATE_T ota_get_sn_code(char* buffer, unsigned int buff_len);

/**********************************************************************************/


/**
* @brief 唤醒整车网络
*
* 1、所有CDC提供的接口，需要加锁保证线程同步
*
* @param mode
[IN] 1表示唤醒，0表示关闭唤醒
* @return OTA_STATE_OK 表示成功，返回错误码表示失败
*/
enum OTA_STATE_T ota_wakeup_vehicle_network(int mode);


/**
* @brief ota模式
*/
typedef enum {
   OTA_MODE_ACTIVATE = 0,      // ota模式激活状态
   OTA_MODE_DEACTIVATE        // ota模式非激活状态
} OTA_MODE;



/**
* @brief 设置ota模式
*
* 1、所有CDC提供的接口，需要加锁保证线程同步
2.5.2 接口说明
在OTA Master开始升级流程时，通知CDC启动OTA模式，此时CDC需实现以下功能：
除因电压过低外，不允许自动下电、或休眠、或kill OTA Master进程；
禁止其他业务进行诊断操作；
通知车端控制器不允许自动进行高低压控制，具体通知及实现方式需要CDC厂商与壁虎沟通确认；
* 2、调用一次，持续发相关报文维护ota模式一段时间（持续时间由参数指定）
*
* @param mode
[IN] ota激活模式
* @param duration_time_s   [IN] 网络管理报文发送持续时间，单位：秒
*
* @return OTA_STATE_OK 表示成功，返回错误码表示失败
*/
enum OTA_STATE_T ota_set_ota_mode(OTA_MODE mode, unsigned int duration_time_s);


/**
* @brief 获取当前ota模式
*
* 1、所有CDC提供的接口，需要加锁保证线程同步
*
* @param mode
[OUT] 当前ota模式
*
* @return OTA_STATE_OK 表示获取成功，返回错误码表示获取失败
*/
enum OTA_STATE_T ota_get_ota_mode(OTA_MODE *mode);



/**
*@brief 设置车辆电压状态
*
* 1、所有CDC提供的接口，需要加锁保证线程同步；
*
*@param kl15_status       [IN] 目标KL15状态
*@param high_volt_status   [IN] 目标高压状态
*
*@return OTA_STATE_OK 表示成功，返回错误码表示失败
*/
enum OTA_STATE_T ota_set_power_status(POWER_STATUS power_status);



/**
* @brief 检测诊断仪是否存在
*
* 1、所有CDC提供的接口，需要加锁保证线程同步；
* 2、该接口通过判断报文状态判断诊断仪是否存在；
* 3、CDC需要维护诊断仪是否存在的标识位，如果超过20秒没有检测到诊断仪是否存在报文，则该标志设置
为0，否则设置为1
*
* @param tester_exist_status [OUT] 诊断仪存在状态，1表示存在，0表示不存在
*
* @return OTA_STATE_OK 表示成功，返回错误码表示失败
*/
enum OTA_STATE_T ota_check_tester_exist(int *tester_exist_status);

/*********************************************************************************/



/**
* @brief 设置预约升级时间
*
* 通过该接口设置预约升级时间之后，CDC应该在指定时间定时唤醒，
* 并调起OTA Master进程（不需要使用任何参数）
* 如果时间到时OTA Master正在运行，则不需要进行额外操作
*
* @param unixtime_s       [IN] 需要唤醒的预约时间，形式为s为单位的UNIXTIME时间戳
*
* @return OTA_STATE_OK 表示成功，返回错误码表示失败
*/
enum OTA_STATE_T ota_set_appointment_time(time_t unixtime_s);





/*********************************************************************************/

typedef enum {
   CAN_RECORD_ACTIVE, // 开启录制CAN报文
   CAN_RECORD_DEACTIVE // 关闭录制CAN报文
} CAN_RECORD_MODE;


/**
* @brief 设置是否开启CAN报文的录制
*
* 1、所有CDC提供的接口，需要加锁保证线程同步；
* 2、当开启CAN报文录制时，CDC应负责录制CAN报文
* 2、录制应持续到OTA Master关闭录制或者到达超时时间
*
* @param mode
[IN] CAN_RECORD_ACTIVE 开启；
CAN_RECORD_DEACTIVE关闭
* @param timeout_s
[IN] 超时时间，到达超时时间后CDC可以停止录制
* @return OTA_STATE_OK 表示成功；其他表示失败
*/
enum OTA_STATE_T ota_set_can_record(CAN_RECORD_MODE mode, unsigned int timeout_s);


/**
* @brief 设置保存CAN报文的路径
*
* 1、所有CDC提供的接口，需要加锁保证线程同步；
* 2、创建文件的命名规则"x_can_log_yyyyyyy_zzzzzz.asc",
*
x表示当前文件的序号，yyyyyy表示创建文件时的年月日，zzzzzz表示创建文件时的时分秒，
如"1_can_log_20240614_141721.asc"；
* 3、参数 @ref max_file_num 表示同时可存储can日志的最大数量；
* 4、can日志需要循环覆盖记录，最新的can日志文件要覆盖最老的can日志文件，
*
如设置can日志文件最大个数为3，当已经存在1_xxx.asc、2_xxx.asc、3_xxx.asc，
*
并且3_xxx.asc文件已经写到参数 @ref max_can_size 设置的最大值，后续记录can日志，将
删除1_xxx.asc，创建4_xxx.asc文件；
*
* @param cdir_path
[IN] 保存CAN报文的目录路径
* @param max_file_num
[IN] 保存最大can日志个数
* @param max_can_size
[IN] 单个文件最大存储的报文
* @return OTA_STATE_OK 表示成功；其他表示失败
*/
enum OTA_STATE_T ota_set_can_record_file_param(
   const char *dir_path,
   unsigned int max_file_num,
   unsigned int max_can_size
);

/****************************************************************************/

/**
* @brief 初始化接⼝，在OTA Master启动时调用一次
*
* @return OTA_OK 表示成功，返回错误码表示失败
*/
enum OTA_STATE_T ota_master_init();
/**
* @brief 资源销毁接⼝，在OTA Master退出时调⽤⼀次
*
*/
void ota_master_deinit(void);
/**
* @brief 获取CDC的存储空间
*
* @param storage_size [OUT] CDC的存储空间
* @return OTA_STATE_OK表示成功；其他表示失败
**/
enum OTA_STATE_T ota_get_cdc_storage_size(long long *storage_size);

typedef struct fileFind_t{
	int fileIdx;
	char fileName[128];
}fileFind;

typedef struct Cdc_Ota_Manager
{
	char dir_path[128];
	int curWritefd;
	char curWritefileName[128];
	unsigned int max_file_num;
	unsigned int max_can_size;
	CAN_RECORD_MODE canRecordFlag;
	unsigned int timeout_s;
	long canRecordStartTime;
	long canRecordStopTime;
	pthread_mutex_t mutexCdcSetCanRecord;
	pthread_mutex_t mutexcanRecordFileParam;
	pthread_mutex_t mutexOtaCheckTesterExist;
	pthread_mutex_t mutexotaSetPowerStatus;
	pthread_mutex_t mutexOtaGetOtaMode;
	pthread_mutex_t mutexOtaSetOtaMode;
	pthread_mutex_t mutexOtaWakeupVehicleNetwork;
	pthread_mutex_t mutexOtaGetSnCode;
	pthread_mutex_t mutexOtaGetVinCode;
	pthread_mutex_t mutexOtaGetCarStatus;
	pthread_mutex_t mutexOtaCdcUpdate;
	pthread_mutex_t mutexotaGetCdcInfo;
}cdc_ota_manager_t;


#ifdef __cplusplus
}
#endif


#endif

