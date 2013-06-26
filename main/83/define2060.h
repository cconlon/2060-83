#ifndef DEFINE2060_H
#define DEFINE2060_H

#include "common.h"

/*
*********************************************************************************************************
*                                          Basic Define for PCM2060/83
*
*                          (c) Copyright 2011-, Provibtech
*                                           All Rights Reserved
*
* File : define.h
* By   : Luzhankui
*********************************************************************************************************
*/

//修改键相数量、数据抽取步长其中默写代码不能自动适应，需要手动修改部分代码以/*manual*/标记
#define _RAMDISK_CLUSTER_LTH				4
#define _SIMU
#define _BASIC_DEVIEID					20600
#define _BASE_RAM_DISK_CHDATA_OFFSET	1024*16//通道数据在电子盘中的起始偏移地址
#define _EVENT_TRAN_SEND				4


#define _FPGA_GROUP_CHANNEL_CNT			6
#define _DEFAULT_HIGH_FREQUENCY_COF		5/2
#define _12_CHANNEL_SAMPLE_RATE			62500
#define _12_DATA_UPLOAD_TIME_GAP		100

#define _SIZE_TCP_SEND_BASIC			1024*4

#define _TRIGER_EVENT_ALERT				0
#define _TRIGER_EVENT_RPM				1
#define _TRIGER_EVENT_TIME				2
#define _TRIGER_EVENT_MANUAL			3


#define _EVENT_CH_ALERT					0
#define _EVENT_STATIC_PICK				1
#define _EVENT_DYNAMIC_PICK				2
#define _EVENT_TRAN_PICK				3
#define _EVENT_TRAN_SEND				4

//#define _ADVANCED_TRAN_MODE
#define _TRAN_FLAG_IDLE					0
#define _TRAN_FLAG_PICKING				1
#define _TRAN_FLAG_TRANSFERING			2

#define	_MAX_BUFFER_LTH					10
#define _MAX_SIGNAL_CHANNEL_CNT			48
#define _MAX_JKEY_CHANNEL_CNT			4
#define _MAX_JKEY_SPEED_ARRAY_LTH		50

#define _DEFAULT_FREQUENCY_RESPONSE_LOW		2000
#define _DEFAULT_FREQUENCY_RESPONSE_HIGH	4000000
#define _MAX_SIGNAL_DATA_SIZE				16384

#define _MAX_STATIC_ARRAY_LTH			200
#define _MAX_DYNAMIC_ARRAY_LTH			10
#define _MAX_TRAN_ARRAY_LTH				3
#define _MAX_TRAN_BUFFER_TIME			5

#define _DEFAULT_DYNAMIC_DATA_MIN_CYCLE		2
#define _DEFAULT_TRAN_DATA_MIN_CYCLE		3

#define _DEFAULT_DYNAMIC_ASYN_DATA_SAMPLE_STEP	6
#define _DEFAULT_TRAN_DATA_SAMPLE_STEP			3
#define _DEFAULT_DYNAMIC_SYN_DATA_SAMPLE_STEP	6

#define _DEFAULT_DYNAMIC_SYN_SAMPLE_LTH		256
#define _MAX_DYNAMIC_SYN_SAMPLE_LTH			1024
#define _DEFAULT_TRAN_SYN_SAMPLE_LTH		256
#define _MAX_TRAN_SYN_SAMPLE_LTH			1024

#define _MAX_TRAN_GROUP					200
#define _DEFAULT_DYNAMIC_DATA_LTH		65536
#define _DEFAULT_TRAN_DATA_LTH			_12_CHANNEL_SAMPLE_RATE*_12_DATA_UPLOAD_TIME_GAP/1000/_DEFAULT_TRAN_DATA_SAMPLE_STEP
#define _DEFAULT_RM_DATA_LTH			2048

#define _MIN_DYNAMIC_CHANNEL_DATA_SAMPLE_STEP	1//最小动态数据抽样间隔
#define _MAX_DYNAMIC_CHANNEL_DATA_SAMPLE_STEP	60//最大动态数据抽样间隔

#define _MIN_TRAN_CHANNEL_DATA_SAMPLE_STEP		1//最大瞬态数据抽样间隔，本版不用
#define _MAX_TRAN_CHANNEL_DATA_SAMPLE_STEP		6//最大瞬态数据抽样间隔，本版不用

#define _DEFAULT_STATIC_AUTO_PICK_FREQUENCY		300000/_12_DATA_UPLOAD_TIME_GAP
#define _DEFAULT_DYNAMIC_AUTO_PICK_FREQUENCY	600000/_12_DATA_UPLOAD_TIME_GAP

#define _MAX_TRAN_TRIGER_TIMESPAN		360000
#define _MAX_DYNAMIC_KEY_POINT_CNT		4800
#define _MAX_TRAN_KEY_POINT_CNT			100

#define	_MAX_RPM_LEVEL					6
#define	_DYNAMIC_ASYNCHRONOUS_MAX_PICK_TIME	4800
#define	_DYNAMIC_SYNCHRONOUS_MAX_PICK_TIME	4800
#define	_DYNAMIC_ASYNCHRONOUS_MIN_PICK_TIME	600
#define	_DYNAMIC_SYNCHRONOUS_MIN_PICK_TIME	600
#define _DYNAMIC_DATA_OVER_FLOW_LTH		8192
#define _TRAN_DATA_OVER_FLOW_LTH		8192

#define _DEFAULT_PRE_TRANTRIGER_TIME_GAP	1000
#define _DEFAULT_PRE_TRANTRIGER_CYCLE		_DEFAULT_PRE_TRANTRIGER_TIME_GAP/_12_DATA_UPLOAD_TIME_GAP
#define _DEFAULT_PRE_TRANTRIGER_TIMESPAN	50000
#define _DEFAULT_PRE_TRANTRIGER_CNT			_DEFAULT_PRE_TRANTRIGER_TIMESPAN/_12_DATA_UPLOAD_TIME_GAP

#define _START_KEY_CH					48
#define _CALIBRATION_DATA_LTH			500

#define	_MAX_KEY_PT_CNT					128
#define	_MAX_TRAN_EVENT					16

#define	_MAX_VALUE_TRANSFER_CNT			36

#define	_MAX_DATA_PIPE_CNT				4

#define _MAX_DYNAMIC_CHANNEL_DATA_BLOCK_CNT		48
#define _DEFAULT_DYNAMIC_FREQUENCY_ASERT_BLOCK_CNT	20
#define	_DYAMIC_BUFFER_LTH				131072+3*_MAX_DYNAMIC_CHANNEL_DATA_BLOCK_CNT

#define _MAX_TRAN_CHANNEL_DATA_BLOCK_CNT	5
#define	_TRAN_BUFFER_LTH				131072+3*_MAX_TRAN_CHANNEL_DATA_BLOCK_CNT
#define _BASE_DYNAMIC_CHANNEL_DATA_ADDRESS			1024*1024
#define _BASE_TRAN_CHANNEL_DATA_ADDRESS				_BASE_DYNAMIC_CHANNEL_DATA_ADDRESS+_DEFAULT_DYNAMIC_DATA_LTH*_MAX_SIGNAL_CHANNEL_CNT*_MAX_DYNAMIC_ARRAY_LTH	
#define ZeroArray(x)						memset(x,0,sizeof(x))
typedef struct tag83DataLimit
{
  INT32S nMaxStaticGroup;//静态数据队列长度,默认100
  INT32S nMaxDyammicGroup;//动态数据队列长度,默认10
  INT32S nMaxTranGroup;//瞬态数据队列长度,固定为1
  INT32S nMaxTranBufferTime;//瞬态数据缓冲时间,默认5s
}Data_Limit_83;

typedef struct tagParam_Point
{
  INT32S jkey;
  INT32S nChCnt;
  INT32S channel[2];
}Param_Point;

typedef struct tag12SignChannel
{
  INT32U		nChNo;						//通道序号	0~47	不修改	转化为槽号和通道号	
  INT32U		nChType;					//通道类型	0~255	位移、速度、加速度、温度、键相，可以扩展		
  FP32		fChFullScaleLow;					//量程低,用于原始数据的归一化处理		
  FP32		fChFullScaleHigh;					//量程高,用于原始数据的归一化处理		
  FP32		fSensority;					//传感器灵敏度	测量值与电压间的线性因子		
  FP32		fZeroVolt;					//零点电压	测量值零点对应的电压		
  INT32U		nCalcMode;//传感器类型,用于上位机	否	INT32U 	0~2	不积分、一次积分、二次积分		
  INT32U		nOutputMode;//测量值类型	是	INT32U 	0~3	pk，pk-pk，rms，avr		
  ///测量值输出方式	否	INT32U 	0~2	opc，4~20ma设备，modbus，取消？		
  INT32U 		bAlert;						//报警允许	是	INT32U 	0或1	允许或不允许		
  INT32U 		bAlertLock;					//报警锁定	否	INT32U 	0或1	锁定或不锁定		
  FP32 		fAlertLevelLow;				//警报	是	FP32	？	alert下限		
  FP32 		fDangerLevelLow;				//危险	是	FP32	？	danger下限	
  FP32 		fAlertLevelHigh;				//警报	是	FP32	？	alert上限		
  FP32 		fDangerLevelHigh;			//危险	是	FP32	？	danger上限

  FP32		fFreqResponseLow;			//	频响下限	是	FP32	？	12模块已经规定，不修改		
  FP32		fFreqResponsehigh;			//频响上限	是	FP32	？	12模块已经规定，不修改

  INT32U 		nPickFrequency;	

  ////FP32		2滤波范围	是	FP32	？	?		
  INT32U		bSync;						//同步标志	是	INT32U 	0或1	1，同步；0，异步		
  FP32		fFreqencyRes;				//频域分辨率,异步模式下代表频域分辨率，同步模式为采样长度		
  INT32S		nJkeyNo;					//键相通道序号 	是	INT32U 	0~？			

  INT32U		nInstallAngle;				//传感器安装角度,	0~360
  //下列参数与几何因素相关，绘制某些视图时需要		
  INT32U		bCircleFlag;				//椭圆标志	否	INT32U 	0或1	0，非椭圆；1，椭圆		
  FP32		fAxa;						//椭圆长轴	否	FP32	？			
  FP32		fAxb;						//椭圆短轴	否	FP32	？			
  INT32U		byTrustDiretion;			//涡流探头推进方向	否	INT32U 	0或1	0，靠近；1，远离
  FP32		fGapLow;					//间隙低	否	FP32	？	
  FP32		fGapHigh;					//间隙高	否	FP32	？	
  INT32U		bEn;						//测量单位					
  /////			
}Signal_Channel_12;
//信号模块参数结构
typedef struct tag12Param
{
  INT32S  nProtol;			//用于向上位机上传参数
  INT32S  nSonProtol;			//用于向上位机上传参数

  INT32U	nDeviceNo;			//设备序号;
  INT32S	nDeviceProperty;	//设备属性；0 数据采集卡 6 过程量输入模块 4 电流模块 5 继电器模块；
  INT32S	nDeviceTypeID;	//设备类型ID;
  INT32S	nDeviceID;//设备ID;

  INT32U	nVersion;
  INT32U	nRenewID;//>0
  INT32S	nRenewTime;//byte 0~1,year,2,month,3,day

  INT32S	nChDeviceChNo[_MAX_SIGNAL_CHANNEL_CNT][2];//通道在所属测点中序号
  INT32S	nSignChannelNum;		//设备信号通道个数；
  INT32S	bWork[_MAX_SIGNAL_CHANNEL_CNT];


  INT32U  nSignalChannelDynamicDataSize[_MAX_SIGNAL_CHANNEL_CNT];//通道原始动态数据大小
  INT32U  nSignalChannelTranDataSize[_MAX_SIGNAL_CHANNEL_CNT];//通道原始瞬态数据大小

  INT32S	nMrsPt[_MAX_SIGNAL_CHANNEL_CNT];//通道所属测点号
  INT32S	nChIndexMrsPt[_MAX_SIGNAL_CHANNEL_CNT];//通道在所属测点中序号
  INT32S	nMrsPtIndex[_MAX_SIGNAL_CHANNEL_CNT];//通道所属测点序号
  INT32S  nMrsPtcnt;//测点数量
  INT32S  nMrsPtTable[_MAX_SIGNAL_CHANNEL_CNT];//测点列表
  INT32S	nMrsPtChcnt[_MAX_SIGNAL_CHANNEL_CNT];//各个测点通道数量

  INT32S	nChannelNumIn83[_MAX_SIGNAL_CHANNEL_CNT*2];//通道在83模块中的序号

  INT32S	nMrsPtValueSize[_MAX_SIGNAL_CHANNEL_CNT];//各个测点实时测量值大小
  INT32S	nMrsPtDataSize[_MAX_SIGNAL_CHANNEL_CNT];//各个测点实时通道数据大小
  INT32S	nMrsPtDataSize0[_MAX_SIGNAL_CHANNEL_CNT];//各个测点0通道实时通道数据大小
  Signal_Channel_12 SignalChannel[_MAX_SIGNAL_CHANNEL_CNT];
}Param_Table_12;

typedef struct tagKeyChannel
{
  INT32U		nChNo;						//通道序号	0~47	不修改	转化为槽号和通道号	
  INT32U		nChType;					//通道类型，255
  INT32U 		bMaster;					//0或1，1为主键相	
  INT32U 		nGearcnt;					//每转的键相点数，>=1，保留

  INT32U 		bTran;						//0或1，1允许转速报警


  INT32U		nTrigerDRPM;				//临界触发转速，单位为0.01转/分		
  INT32U 		nTrigerTimeScope;			//判断转速触发的时间窗宽度，单位为毫秒

  INT32U 		fWokeRPM;					//工作转速

  FP32		fRPMLow;					//允许转速下限	是	FP32	？	12模块已经规定，不修改		
  FP32		fRPMHigh;					//允许转速上限	是	FP32	？	12模块已经规定，不修改		

}Key_Channel;

/*一个83虚拟设备就是某一个键相所绑定有效通道的集合（如果一个通道在12模块中被标志为无效或不输出，\
则不在虚拟设备中出现*/
typedef struct tag83Device
{
  INT32U	nDeviceNo;				//设备序号;
  INT32S	nDeviceProperty;		//设备属性；0 数据采集卡 6 过程量输入模块 4 电流模块 5 继电器模块；
  INT32S	nDeviceTypeID;			//设备类型ID;
  INT32S	nDeviceID;				//设备ID;

  FP32 	nFrequencyLow;//上位机设定的频响下限,放大100倍
  FP32 	nFrequencyHigh;//上位机设定的频响上限，放大100倍

  INT32U	nPickDynamicFrequency;//期望的数字化频率，放大100倍
  INT32U	nPickTranFrequency;//期望的数字化频率，放大100倍
  INT32U	nDynamicPickTime;//动态采集持续时间
  INT32U	nTranPickTime;//瞬态采集持续时间
  INT32S	nDynamicChannelDataSampleStep;//各个通道的动态抽取因子
  INT32S	nTranChannelDataSampleStep;//各个通道的瞬态抽取因子
  INT32S	nSignal_Channel_Dynamic_Data_Lth;//各个通道的动态数据长度
  INT32S	nSignal_Channel_Tran_Data_Lth;//各个通道的瞬态态数据长度

  INT32S	nChannelMask[_MAX_SIGNAL_CHANNEL_CNT];//1,允许，0禁止
  INT32S	nStaticMask[_MAX_SIGNAL_CHANNEL_CNT];//全部固化为1，不修改
  INT32S	nDynamicMask[_MAX_SIGNAL_CHANNEL_CNT];//有选择的打开
  INT32S	nTranMask[_MAX_SIGNAL_CHANNEL_CNT];//有选择的打开

  INT32S	bAlertTrigerStatic;		//报警触发静态，1，报警触发，0，无效；固定为1
  INT32S	bAlertTrigerDynamic;	//报警触发动态，1，报警触发，0，无效

  INT32S	nStaticAutoPickTime;	//自动静态采集时间间隔，大于0有效
  INT32S	nDynamicAutoPickTime;	//自动动态采集时间间隔，大于0有效
  INT32U	nDynamicSampleMode;//0，异步采集；1，同步采集
  INT32U	nDynamicSynDataCntPerCycle;//动态同步数据每周期最小数量
  INT32U	nDynamicSynRPM;//动态同步变频临界转速

  INT32U 	nPickTimeScopePre;		//未触发时暂存时间，单位为毫秒

  INT32U	nTranSampleMode;			//0，时间间隔抽样；1，转速间隔抽样
  INT32U 	nPickTimeIntervalPre;	//未触发时暂存时间间隔，单位为毫秒	
  INT32U 	nSampleTimeInterval;	//触发时抽样时间间隔，单位为毫秒
  INT32U	nSampleRPMInterval;		//抽样转速间隔，单位为1转/分	
  INT32U	nRealSampleTimeInterval;//按数据产生次数算的瞬态触发后抽样间隔
  INT32U	nRealPickTimeIntervalPre;//按数据产生次数算的瞬态触发前抽样间隔

  INT32U	nMaxTranSampleTime;		//最大瞬态采集时间
  INT32U	nMaxTranSampleCnt;		//最大瞬态采集组数，目前为200
  INT32U  nTranPreTrigerGroupcnt;		//触发前瞬态数据预抽样数
  INT32U	nMinPreTranTickcnt;
  INT32U	nTranSynSampleMode;//0，异步采集；1，同步采集，本版本固定为0
  INT32U	nTranSynDataCntPerCycle;//瞬态同步数据每周期最小数量，本版本不支持

  INT32S	nMrsPtNo[_MAX_SIGNAL_CHANNEL_CNT];//所属测点
  INT32S  nPtMrsNo[_MAX_SIGNAL_CHANNEL_CNT][3];//所属通道
  INT32S	nChIndexMrsPt[_MAX_SIGNAL_CHANNEL_CNT];//通道在所属测点中序号
  INT32S	nWorkChNo[_MAX_SIGNAL_CHANNEL_CNT];//允许通道序号对应的通道号

  INT32S  nPreTranTrigerTime;		//是否通道顺序混乱

  INT32S	nKeyChno;				//键相通道号	

  INT32S	nSignChannelNum;		//设备信号通道个数；
  INT32S	nStaticChannelNum;		//设备信号通道个数；
  INT32S	nDynamicChannelNum;		//设备信号通道个数；
  INT32S	nTranChannelNum;		//设备信号通道个数；

  INT32S	nKeyChannelNum;		//设备键相通道个数；
  Key_Channel KeyChannel[1];

  INT32S	nAllChValueSize;			//所有通道的测量值总长度
  INT32S	nAllChStaticValueSize;			//所有通道的静态测量值总长度
  INT32S	nAllChDynamicValueSize;			//所有通道的动态测量值总长度
  INT32S	nAllChTranValueSize;			//所有通道的瞬态测量值总长度
  //	INT32S	nAllChDataSize;			//所有通道的数据总长度
  INT32S	nAllDynamicChDataSize;			//所有通道的数据总长度
  INT32S	nAllTranChDataSize;			//所有通道的数据总长度
  INT32S	nChannelNumIn12[_MAX_SIGNAL_CHANNEL_CNT];//通道在12模块中的序号
  INT32S	nChValueSize;//通道测量值长度
  INT32S	nChDynamicDataSize;//通道数据长度
  INT32S	nChTranDataSize;//通道数据长度
  INT32S	nChDynamicDataBlockCnt;//通道数据数量
  INT32S	nChTranDataBlockCnt;//通道数据长度数量
  INT32S	nChDynamicBlockSize;//通道单次抽样数据块长度
  INT32S	nChTranBlockSize;//通道单次抽样数据块长度
  INT32S	nChKeyBlockSize;//键相块尺寸
  //	INT32S	nDatalth[_MAX_SIGNAL_CHANNEL_CNT];//通道原始数据长度
}Param_Table_Device;
/*83虚拟设备就是将12所有通道按其所绑定的键相进行划分，成为虚拟设备（如果一个键相通道在12模块中被标志为无效或不输出\
则不产生虚拟设备中出现*/

typedef struct tag83Param
{
  INT32S  nProtol;				//用于向上位机上传参数
  INT32S  nSonProtol;				//用于向上位机上传参数
  INT32S	nSignChannelNum;		//
  INT32S	nStaticChannelNum;		//设备信号通道个数；
  INT32S	nDynamicChannelNum;		//设备信号通道个数；
  INT32S	nTranChannelNum;		//设备信号通道个数；
  INT32S	nAllChValueSize;		//所有通道的测量值总长度
  INT32S	nAllChStaticValueSize;	//所有通道的静态测量值总长度
  INT32S	nAllChDynamicValueSize;	//所有通道的动态测量值总长度
  INT32S	nAllChTranValueSize;	//所有通道的瞬态测量值总长度
  //	INT32S	nAllChDataSize;		//所有通道的数据总长度
  INT32S	nAllDynamicChDataSize;	//所有通道的数据总长度
  INT32S	nAllTranChDataSize;		//所有通道的数据总长度
  INT32U	nDeviceNo;				//设备序号;
  INT32S	nDeviceProperty;		//设备属性；0 数据采集卡 6 过程量输入模块 4 电流模块 5 继电器模块；
  INT32S	nDeviceTypeID;			//设备类型ID;
  INT32S	nDeviceID;				//设备ID;
  INT32S	nDevicecnt;				//设备数量

  INT32S	nStaticAutoPickTime;	//自动静态采集时间间隔，大于0有效
  INT32S	nDynamicAutoPickTime;	//自动动态采集时间间隔，大于0有效

  Param_Point  nPtMrsNo[_MAX_SIGNAL_CHANNEL_CNT];//所属测点通道
  INT32S  nMrsPtcnt; //测点数量
  INT32S  nMrsPtTable[_MAX_SIGNAL_CHANNEL_CNT];//测点列表
  Param_Table_Device	Device[_MAX_JKEY_CHANNEL_CNT];
}Param_Table_83;
typedef struct tagRunTimeFlag
{
  INT32S	bRunning;//0,停止，1，运行
  INT32S	nAlertFlag[_MAX_JKEY_CHANNEL_CNT];//报警标志：报警产生时数据采集模块设置，0，无报警；1，警报；2，危险
  INT32S	nTranFlag[_MAX_JKEY_CHANNEL_CNT];//转速报警标志：无报警；1，警报；
  INT32S	nPickFlag[_MAX_JKEY_CHANNEL_CNT];//数据采集标志：0，无采集，1，静态；2，动态，3，瞬态
  INT32S	nValidDataType;//有效数据类型：数据采集模块设置，1，静态；2，动态，3，瞬态
  INT32S	nVaildStaticStart;//静态有效范围：数据采集模块设置，起始序号
  INT32S	nVaildStaticcnt;//静态有效范围：数据采集模块设置,数据组数
  INT32S	nVaildDynamicStart;//动态有效范围：数据采集模块设置，起始序号
  INT32S	nVaildDynamiccnt;//动态有效范围：数据采集模块设置,数据组数
  INT32S	nValidTranNo;//瞬态有效区：  数据采集模块设置，0或1
  INT32S	nInvaildStaticStart;//静态无效范围：数据采集模块设置，起始序号
  INT32S	nInvaildStaticcnt;//静态无效范围：数据采集模块设置,数据组数
  INT32S	nInvaildDynamicStart;//动态无效范围：数据采集模块设置，起始序号
  INT32S	nInvaildDynamiccnt;//动态无效范围：数据采集模块设置,数据组数
  INT32S	nInvalidTranNo;//瞬态无效区：  通讯模块设置，0或1
  INT32S	nTransitFlag;//数据传输标志：通讯模块设置，1，静态传输，2，动态传输，3，瞬态传输
  INT32S	nOuterAlert;//外部报警标志：0，无报警；1，警报；2，危险
  INT32S	nMasterCommand;//PCM360外部命令：通讯模块设置，数据采集模块响应并复原
  /*1，	测量值申请，优先级，4
  2，	实时数据申请，优先级，5；
  100，状态查询，优先级，1
  101，手动静态采集，优先级，3
  102，手动动态采集，优先级，2
  103，停止瞬态采集，优先级，1
  200，启动PT2060，优先级，0
  201，停止PT2060，优先级，0
  202，PT2060上传参数，优先级，6
  203，PT2060下载参数，优先级，*/

}Run_Time_Flag;

typedef struct tagTst_Head_DCM_SigModuSampData_SYS
{ 
  union
  {
    struct
    {
      INT16U   unModuleType;      	// 模块类型
      union
      {
        INT16U   unVal_PT2060;     // PT2060
        // 添加其它子系统的通道序号结构
      }tunChNo;                    // 通道序号
    }tstVal;
    INT32U  ulnVal;  
  }tunChInfo;      // 通道信息

  INT32U   ulnWaveDataLen;         //动态波形数据长度。 0 – 波形数据无效；其它 – 有效值

  INT32U   ulnTmStamp_SYN1;         // 数据采样同步信号SYN1时间戳
  INT32U   ulnTmStamp_LastWaveData;  // 最后1个波形数据时间戳
  INT32U   ulnWaveDataSampFreq;		// 波形数据采样频率。高17位为整数，低15位为小数，单位Hz
  INT32U   ulnWaveDataUnitage;		    // 波形数据单位量。单位：nV

  union
  {
    struct
    { 
      INT16U  unFlag_RUN :1;          // 通道处于运行系统状态标志。 1 == 有效，0 == 无效
      INT16U  unFlag_ChEnable :1;      // 通道使能标志。1 == 有效，0 == 无效
      INT16U  unFlag_1XAEnable :1;     // 1XA使能标志。1 == 有效，0 == 无效
      INT16U  unFlag_1XPEnable :1;     // 1X P使能标志。1 == 有效，0 == 无效
      INT16U  unFlag_2XAEnable :1;     // 2XA使能标志。1 == 有效，0 == 无效
      INT16U  unFlag_2XPEnable :1;     // 2XP使能标志。1 == 有效，0 == 无效
      INT16U  unFlag_Not1XEnable :1;   // Not1X使能标志。1 == 有效，0 == 无效
      INT16U  unFlag_SMAXEnable :1;   // SMAX使能标志。1 == 有效，0 == 无效

      INT16U  unFlag_NotOK  :1;       // 通道NotOK标志。1 == 有效，0 == 无效
      INT16U  unFlag_ChCfgResoOK :1;   // 通道配置解析成功标志。1 == 有效，0 == 无效
      INT16U  unFlag_Danger  :1;       // 通道组合危险报警标志。 1 == 有效，0== 无效
      INT16U  unFlag_Alert    :1;       // 通道组合警告报警标志。 1 == 有效，0 == 无效
      INT16U  unFlag_Bypass  :1;       // 通道报警旁路标志。 1 == 有效，0 == 无效
      INT16U  unFlag_AlarmMult :1;     // 通道报警倍增标志。 1 == 有效，0 == 无效
      INT16U  unFlag_AlarmLatch :1;    // 报警锁存标志。     1 == 锁存，0 == 不锁存
      INT16U  unSpace115 :1;        // 保留   缺省值为0
    }tstVal;
    INT16U  unVal;
  }tunStaticFlag1;

  union
  {
    struct
    { 
      INT16U  unFlag_Valid_GAP     :1;    // GAP值有效标志。1 == 有效，0 == 无效
      INT16U  unFlag_Valid_DIRECT  :1;    // DIRECT值有效标志。1 == 有效，0 == 无效
      INT16U  unFlag_Valid_1XA     :1;    // 1XA值有效标志。1 == 有效，0 == 无效
      INT16U  unFlag_Valid_1XP     :1;    // 1XP值有效标志。1 == 有效，0 == 无效
      INT16U  unFlag_Valid_2XA     :1;    // 2XA值有效标志。1 == 有效，0 == 无效
      INT16U  unFlag_Valid_2XP     :1;    // 2XP值有效标志。1 == 有效，0 == 无效
      INT16U  unFlag_Valid_NOT1X  :1;    // NOT1X值有效标志。1 == 有效，0 == 无效
      INT16U  unFlag_Valid_SMAX   :1;    // SMAX值有效标志。1 == 有效，0 == 无效
      INT16U  unSpace8_15          :8;    // 保留 缺省值为0
    }tstVal;
    INT16U  unVal;
  }tunStaticFlag2;

  union
  {
    struct
    { 
      INT16U  unFlag_DangerH_GAP:1;   // GAP高端危险报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_AlertH_GAP:1;    // GAP高端警告报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_AlertL_GAP :1;    // GAP低端警告报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_DangerL_GAP :1;  // GAP低端危险报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_DangerH_DIRECT:1;   // DIRECT高端危险报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_AlertH_DIRECT:1;    // DIRECT高端警告报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_AlertL_DIRECT :1;    // DIRECT低端警告报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_DangerL_DIRECT :1;  // DIRECT低端危险报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_DangerH_1XA:1;   // 1XA高端危险报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_AlertH_1XA:1;    // 1XA高端警告报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_AlertL_1XA :1;    // 1XA低端警告报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_DangerL_1XA :1;  // 1XA低端危险报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_DangerH_1XP:1;   // 1XP高端危险报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_AlertH_1XP:1;    // 1XP高端警告报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_AlertL_1XP :1;    // 1XP低端警告报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_DangerL_1XP :1;  // 1XP低端危险报警标志。1 == 有效，0 == 无效
    }tstVal;
    INT16U  unVal;
  }tunStatic3;


  union
  {
    struct
    { 
      INT16U  unFlag_DangerH_2XA:1;   // 2XA高端危险报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_AlertH_2XA:1;    // 2XA高端警告报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_AlertL_2XA :1;    // 2XA低端警告报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_DangerL_2XA :1;  // 2XA低端危险报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_DangerH_2XP:1;   // 2XP高端危险报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_AlertH_2XP:1;    // 2XP高端警告报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_AlertL_2XP :1;    // 2XP低端警告报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_DangerL_2XP :1;  // 2XP低端危险报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_DangerH_Not1X:1;   // Not1X高端危险报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_AlertH_Not1X:1;    // Not1X 高端警告报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_AlertL_Not1X :1;    // Not1X低端警告报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_DangerL_Not1X :1;  // Not1X低端危险报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_DangerH_SMAX:1;   // SMAX高端危险报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_AlertH_SMAX:1;    // SMAX高端警告报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_AlertL_SMAX :1;    // SMAX低端警告报警标志。1 == 有效，0 == 无效
      INT16U  unFlag_DangerL_SMAX :1;  // SMAX低端危险报警标志。1 == 有效，0 == 无效
    }tstVal;
    INT16U  unVal;
  }tunStaticFlag4;

  INT16U  unFS_Range;		// 满量程范围
  INT16S  nGapVal;			// 通道GAP值，电压值，单位：mV

  union
  { 
    INT16U  unVal;
    INT16S  nVal;
  }tunDirect;			  // 通道类型值
  INT16U   un1XA;		  // 1XA值

  INT16U   un1XP;		  // 1XP值
  INT16U   un2XA;		  // 2XA值

  INT16U   un2XP;		  // 2XP值
  INT16U   unNoT1X;	  // Not1X值

  INT16U   unSMAX;	  // SMAX值
  INT16U   unSpace;	  // 保留，以便对齐32位，缺省值为0

  INT32U   ulnaSpace[6];	  // 保留，以便对齐20个32位，缺省值为0

  INT32U	 ulnRPM;
  INT32U	 ulnKeyPoint;

}Tst_Head_DCM_SigModuSampData_SYS;

typedef  struct tagChValue//测量值
{
  INT32U nTrigerTime;//时间戳
  INT32S nChNo;//通道序号
  INT32S nAlertFlag;//	0，1，2	0，无报警，1，warning，2，danger
  INT32S nRpmFlag;//转速报警状态	0或1	1，转速报警
  INT32S nRPM;//	测量值	转速	rpm	
  INT32U nAllMeasureValue;//测量值	通频幅值	all	
  INT32U nAllPhase;//	测量值	通频相位	all	
  INT32U n1xV;	//基频幅值	1X	
  INT32U n1xP;//基频相位	1X	
  INT32U n2xV;//倍频幅值	1X	
  INT32U n2xP;	//倍频相位	1X
  INT32U n05xV;//亚频幅值	1X	
  INT32U n05xP;//亚频相位	1X	
  INT32U nn1xA;//亚频幅值	1X	
  INT32U nn1xP;//亚频相位	1X	

}Channel_Static_Value,Channel_Dynamic_Value,Channel_Tran_Value;

typedef  struct tagChData2060//原始数据
{
  INT32U nTrigerTime;//时间戳
  INT32S nChNo;//通道序号
  INT32S nFrequency;//采样频率
  INT32S nPiecelen;//采样长度
  INT32S nKeycnt;//键相点数
  //后面是键相点位置和原始数据
  //	INT32S* pnData;//键相在前，信号在后，
  INT32S pData[1];//
}Channel_Dynamic_Wave_Data,Channel_Tran_Wave_Data;
struct tagAlertEvent
{
  INT32U nAlertTime;//时间戳
  INT32S nAlertChcnt;//报警键相数量
  INT32S nAlertChNo[_MAX_JKEY_CHANNEL_CNT];//报警键相号
};

struct tagTranEvent
{
  INT32S	nKeyNo;
  INT32S	nCurRead;
};

typedef struct tagProtocolHead
{
  INT32S	nProtol;
  INT32S	nSonProtol;
  INT32S    nJkey;
  INT32S	nTrigerTime;
  INT32S    nTrigerType;
  INT32S	nChcnt;
  INT32S	nGroupcnt;
  INT32S    nMrsPtcnt;
}ProtocolHead;

typedef struct tagRTChValue
{
  INT32S nMrsPt;
  struct
  {
    INT32S nAlertFlag;     //0，1，2	0，无报警，1，warning，2，danger
    FP32 fAllMeasureValue; //测量值	通频幅值	all	
  }data[1];
}RTChValue;

typedef struct tagMrsptRTValue //测点实时测量值组
{
  INT32U nPickTime;
  INT32S nMrsPtcnt;
  struct tagRTChValue nChValue[1];
}MrsptRTValue;

typedef struct tagMrsptRTData  //测点实时原始信号
{
  struct{
    struct tagChValue		Value;	
    struct tagChData2060	Data;
  }data[1];
}MrsptRTData;

struct FILEHEAD
{
  INT8S szFlag[10];		   //"SendFile0"
  INT32U dwFileSize;
  INT8S szFileName[32];
};

typedef  struct tagStaticTrigerTable
{	
  volatile INT32U ulnTrigerRead;
  volatile INT32U ulnTrigerWrite;
  INT32U ulnTrigerTime[_MAX_STATIC_ARRAY_LTH];
  INT32U ulnTrigerReason[_MAX_STATIC_ARRAY_LTH];
}Static_Triger_Table;

typedef struct tagDynamicChannelDataTable
{
  INT32U ulnTrigerTime;
  INT32U ulnTrigerReason;
  INT32U KeyDataBlockLth[_MAX_DYNAMIC_CHANNEL_DATA_BLOCK_CNT];
  INT32U ulnBlockCnt;//有效块数，动态数据为多块
  //	INT32S ulnBlockLth;//块长度
  //	INT32S ulnDataHeadSit[_MAX_DYNAMIC_CHANNEL_DATA_BLOCK_CNT];//块首
  //	INT32S ulDataLength[_MAX_DYNAMIC_CHANNEL_DATA_BLOCK_CNT];//块有效数据长度
  INT32S lnHeadBlock;//起始块
  INT32U ulnBlockStep;//块步长

  INT32S lnCurBlock;//当前块
}Dynamic_Channel_Data_table;

typedef  struct tagDynamicTrigerTable
{	
  volatile INT32S ulnTrigerRead;
  volatile INT32S ulnTrigerWrite;	
  Dynamic_Channel_Data_table ChannelSignalDataTable[_MAX_DYNAMIC_ARRAY_LTH];//[_MAX_SIGNAL_CHANNEL_CNT];

}Dynamic_Triger_Table;
typedef struct tagTranChannelData
{
  INT32U KeyDataBlockLth[_MAX_TRAN_CHANNEL_DATA_BLOCK_CNT];
  INT32U ulnBlockCnt;//有效块数，瞬态目前实际上不需要，因为只有一块
  //	INT32S ulnBlockLth;//块长度,保留
  //	INT32S ulnDataHeadSit[_MAX_TRAN_CHANNEL_DATA_BLOCK_CNT];//块首
  //	INT32S ulDataLength[_MAX_TRAN_CHANNEL_DATA_BLOCK_CNT];//块有效数据长度
  INT32S lnHeadBlock;//起始块
  INT32U ulnBlockStep;//块步长

  INT32S lnCurBlock;//当前块
}Tran_Channel_Data_table;
typedef  struct tagTranTrigerTable
{	
  volatile INT32U ulnTrigerRead;//当前读取位置
  volatile INT32U ulnTrigerWrite;//当前写入位置
  INT32U ulnTrigerTime[_MAX_TRAN_ARRAY_LTH];//触发时间队列
  INT32U ulnTrigerReason[_MAX_TRAN_ARRAY_LTH];//触发原因 队列
  Tran_Channel_Data_table ChannelSignalDataTable[_MAX_TRAN_ARRAY_LTH][_MAX_TRAN_GROUP];
  INT32U ulnGroupCnt[_MAX_TRAN_ARRAY_LTH];
  INT32S lnGroupHeadSit[_MAX_TRAN_ARRAY_LTH];//	本次触发的瞬态起始组位置

  INT32U ulnTrigerTimeSpan;//瞬态触发持续时间
  INT32U nPreTranTickCnt;//瞬态触发前激发次数
  INT32U ulnTrigerFlag;//0,未触发;1,已经触发;
  INT32U ulnCurWriteGroup;//瞬态当前组数

  INT32U ulnPreAlertFlag;//前次的报警状态,实际上用来判断是否触发报警采集
  INT32U ulnPreRPM;//	上一组数据的转速
  INT32U ulnAccumulateCnt;//上次保留数据后的已经触发次数

  INT32U ulnPick;//非首次到达通道数据是否保留标志
}Tran_Triger_Table;
typedef struct tagKeySpeedCalt
{
  INT32U		nTrigerLevel;//各个键相累积的速度数量
  INT32S		nSpeedArray[_MAX_JKEY_SPEED_ARRAY_LTH];//速度历史队列
  INT32S		nPreAcumulateSpeed;//前一次队列速度的合
  INT32U		nCurLevel;
  INT32U		nRPMLevel[_MAX_RPM_LEVEL];//同步抽取时各阶转速
  INT32U		nSampleStep[_MAX_RPM_LEVEL];//同步抽取时各阶抽取因子
  INT32U		nSynBlockCnt[_MAX_RPM_LEVEL];//同步抽取时各阶抽取数量
  INT32U		nSynBlockIncreaseStep[_MAX_RPM_LEVEL];//同步抽取时各阶数据块偏移递增步长
  //INT32S		nSpeed;//本次读取的速度	
}Key_Speed_Calt;

extern const INT32S				g_n12UpFrequency;

INT32S Init2060();
INT32S Start2060();
INT32S Stop2060();
INT32S Uninit2060();
//void receive_channel_data();
void alert_pick(void* p);
void tran_pick(void* p);
void static_transfer(void* p);
void dynamic_transfer(void* p);
void tran_transfer(void* p);
void command_analys(void* p);
void real_time_show(void* p);
void real_time_show_pipe(void* p);
INT32S OutputTrigerEventToDisk(char strName[]);
INT32U moudle(INT32U nObject,INT32U nDevider);
//arm调用读取键相通道新产生的键相点
INT32S ReadKeyPoint(INT32U nChannel,INT32S pData[],INT32S nCnt);
//arm调用读取键相通道当前速度
INT32S ReadKeySpeed(INT32U nChannelcnt,INT32U nChannel[],INT32S pData[]);
/////////不要删除//不要删除//不要删除//不要删除//不要删除//不要删除//不要删除//不要删除//不要删除//不要删除/////////
/*内容在另一个未修改前的工程里*/
/////////不要删除//不要删除//不要删除//不要删除//不要删除//不要删除//不要删除//不要删除//不要删除//不要删除/////////
/////////不要删除//不要删除//不要删除//不要删除//不要删除//不要删除//不要删除//不要删除//不要删除//不要删除/////////
///////// 

#endif
