#ifndef GLOBEL83PARAM_H
#define GLOBEL83PARAM_H

#include "common.h"
#include "define2060.h"

INT8S						g_strHostIP[] = "192.168.1.116";

INT32U						g_nTcpPort = 20600;
INT32U						g_nUdpPort = 20610;
INT32S						g_nSystemNo = -1;//系统编号
INT32S						g_nMgrNo = -1;//机组编号
INT32S						g_nMchNo[_MAX_SIGNAL_CHANNEL_CNT] = {-1};//机器编号
INT32S						g_nDeviceNo[_MAX_SIGNAL_CHANNEL_CNT] = {-1};//虚拟设备编号
const INT32S				g_n12UpFrequency = 10;//12模块上传数据的频率,由海淀给定,不得修改
const INT32S				g_TempAddtionalcnt = 10;//附加临时数据组数
const INT32S				g_nMaxTranGroupDataSize = 1024*1024*250;
const INT32S				g_nMaxTranDataGroup = 4;
const INT32S				g_nMaxDataSize = 1024*1024*400;
const INT32S				g_nTranGroupcnt = _MAX_TRAN_GROUP;
/*
INT32U						g_nStartTime = 0;
INT32U						g_nTickCount = 0;
INT32U						g_nIndexInGroupSequency = 0;
INT32U						g_DynamicSampleStepDuration = 0;
*/
Data_Limit_83				g_83DataLimit;//83模块的一些极限参数
INT32S						g_nChannelArrivalSequenceTable[_MAX_SIGNAL_CHANNEL_CNT];//到达通道次12通道序号表
INT32S						g_nArrivalDeviceNo[_MAX_SIGNAL_CHANNEL_CNT];//到达通道虚拟设备表
INT32S						g_nArrivalChannelNo[_MAX_SIGNAL_CHANNEL_CNT];//通道数据到达在所属设备的通道号
Param_Table_12				g_12Param @ "MYSECTION1";//12模块参数,从12模块读取,不保存,由12congfig模块配置,360系统不可修改
Param_Table_83				g_83param @ "MYSECTION1";//83模块参数,存储在flash,可从上位机更新
Run_Time_Flag				g_RunTimeFlag;//83运行状态信息

/*三种数据以所绑定的键相号作为内存划分的基础，从外到里按照键相号-触发时间-虚拟通道序号组织*/
/*提前计算出静态数据按照键相号-触发次序-虚拟通道序号的指针列表,用于快速从信号处理模块快速接收数据*/
/*静态和动态数据以报警时间作为一次采集,虚拟间数据以虚拟通道序号排序*/


/*静态数据所耗内存很小，采用静态分配，静态数据主要以报警方式触发，特定时间戳的数据到达时，一个通道报警，
与其同键相的通道一起进行采集,采集完成后所属键相的静态触发序号加1*/
Tst_Head_DCM_SigModuSampData_SYS		g_StaticOriginData[_MAX_STATIC_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT] @ "MYSECTION1";//静态测量值队列
//根据12模块的键相-通道邦定列表创建,用来快速检索静态数据
Tst_Head_DCM_SigModuSampData_SYS*		g_pStaticChannelOrigin[_MAX_JKEY_CHANNEL_CNT][_MAX_STATIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT] @ "MYSECTION1";
//INT8S*									g_tmpStaticData;//待发送静态数据
INT8S  g_tmpStaticData[sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead)] @ "MYSECTION1";
ProtocolHead*						    g_tmpStaticDataHead;
Channel_Static_Value*					g_ptmpStaticData[_MAX_SIGNAL_CHANNEL_CNT];
Static_Triger_Table						g_StaticTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];

/*动态数据所耗内存较大，采用动态分配，动态数据主要以报警方式触发，特定时间戳的数据到达时，一个通道报警，
与其同键相的通道一起进行采集，每个时间戳下采集动态数据的通道数量是不同的,采集完成后所属键相的动态触发序号加1*/
Tst_Head_DCM_SigModuSampData_SYS		g_DynamicValue[_MAX_DYNAMIC_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT] @ "MYSECTION1";//动态态测量值队列
//根据12模块的键相-通道邦定列表创建,用来快速检索动态数据头
Tst_Head_DCM_SigModuSampData_SYS*		g_pDynamicChannelValue[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT] @ "MYSECTION1";
Dynamic_Triger_Table					g_DynamicTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];
//INT8S*									g_tmpDynamicData;//待发送动态数据
//INT8S                                   g_tmpDynamicData[(sizeof(INT32S)*_DYAMIC_BUFFER_LTH+sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead))];
INT8S                                   g_tmpDynamicData[sizeof(INT32S)*_DYAMIC_BUFFER_LTH+sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead)] @ "MYSECTION1";

ProtocolHead*						    g_tmpDynamicDataHead;
Channel_Dynamic_Value*					g_ptmpDynamicValue[_MAX_SIGNAL_CHANNEL_CNT];//待发送动态数据测量值
Channel_Dynamic_Wave_Data*				g_ptmpDynamicData[_MAX_SIGNAL_CHANNEL_CNT];//待发送动态通道原始数据
//动态通道数据偏移表,初始化时生成,同于指示电子盘通道写入地址,也用于数据向上位机发送时快速定位数据在电子盘的位置
INT32U									g_nChannel_Dynamic_Wave_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
INT32U									g_nChannel_Dynamic_Key_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH];

/*瞬态数据所耗内存很大,采用动态分配,根据虚拟设备划分为不同设备数据区域,瞬态数据队列长度最大不超过_MAX_TRAN_ARRAY_LTH，
最少为1，称为组数，设备数据以组号为依据划分为同等大小的区域,组内再按照通道序号细分,通道内部以触发次序再次细分
*/
Tst_Head_DCM_SigModuSampData_SYS		g_TranValue[_MAX_TRAN_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT*_MAX_TRAN_GROUP] @ "MYSECTION1";//瞬态测量值队列
//根据12模块的键相-通道邦定列表创建,用来快速检索瞬态态数据头
Tst_Head_DCM_SigModuSampData_SYS*		g_pTranChannelValue[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT][_MAX_TRAN_GROUP] @ "MYSECTION1";
Tran_Triger_Table						g_TranTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];
//INT8S*									g_tmpTranData;//待发送瞬态数据
INT8S  g_tmpTranData[(sizeof(INT32S)*_TRAN_BUFFER_LTH+sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead))];


ProtocolHead*							g_tmpTranDataHead;
Channel_Tran_Value*						g_ptmpTranValue[_MAX_SIGNAL_CHANNEL_CNT];//待发送动态数据测量值
Channel_Tran_Wave_Data*					g_ptmpTranData[_MAX_SIGNAL_CHANNEL_CNT];
//瞬态通道数据偏移表,初始化时生成,同于指示电子盘通道写入地址,也用于数据向上位机发送时快速定位数据在电子盘的位置
INT32U		g_nChannel_Tran_Wave_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT][_MAX_TRAN_GROUP];
INT32U		g_nChannel_Tran_Key_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_TRAN_GROUP];

//INT32U		g_Signal_Channel_Dynamic_Write_Position[_MAX_SIGNAL_CHANNEL_CNT];//各个通道的动态数据写入位置
//INT32U		g_Signal_Channel_Tran_Write_Position[_MAX_SIGNAL_CHANNEL_CNT];//各个通道的瞬态数据写入位置
//INT32U		g_Key_Channel_Dynamic_Write_Position[_MAX_JKEY_CHANNEL_CNT];//各个通道的动态键相数据写入位置
//INT32U		g_Key_Channel_Tran_Write_Position[_MAX_JKEY_CHANNEL_CNT];//各个通道的瞬态键相数据写入位置
extern INT32S	g_nGroupCnt ;
/*
INT32U		g_nArmDynamicChDataOffset[_MAX_SIGNAL_CHANNEL_CNT*2+_MAX_JKEY_CHANNEL_CNT*2];
INT32U*		g_nArmDynamicKeyDataOffset = g_nArmDynamicChDataOffset+_MAX_SIGNAL_CHANNEL_CNT;
INT32U*		g_nArmTranChDataOffset = g_nArmDynamicChDataOffset+_MAX_SIGNAL_CHANNEL_CNT+_MAX_JKEY_CHANNEL_CNT;
INT32U*		g_nArmTranKeyDataOffset = g_nArmDynamicChDataOffset+_MAX_SIGNAL_CHANNEL_CNT*2+_MAX_JKEY_CHANNEL_CNT;
INT32U		g_nArmDynamicChDataSampleStep[_MAX_SIGNAL_CHANNEL_CNT*2];
INT32U*		g_nArmTranChDataSampleStep=g_nArmDynamicChDataSampleStep+_MAX_SIGNAL_CHANNEL_CNT;
*/
//INT32S		g_nFPGATickcnt = 0;
/*manual*///修改键相数量和数据抽取步长时下面全局变量的初始化需要手工修改
//Key_Speed_Calt	g_DynamicSpeedCalt[_MAX_JKEY_CHANNEL_CNT];//各个键相速度动态计算结构
//Key_Speed_Calt	g_TranSpeedCalt[_MAX_JKEY_CHANNEL_CNT];//各个键相速度瞬态计算结构

INT32S		g_nTempCurWriteSit[_MAX_JKEY_CHANNEL_CNT] = {0};//虚拟设备的当前瞬态写入位置
INT32S		g_nTempCurReadSit[_MAX_TRAN_ARRAY_LTH] = {0};
INT32S		g_nTempValidcntr[_MAX_TRAN_ARRAY_LTH] = {0};
/*临时测量值键相分组指针,用于瞬态抽样,第一所引为组号，第二所引为虚拟设备号*/
struct tagChValue*		g_TempChValuePtr[_MAX_TRAN_ARRAY_LTH][_MAX_JKEY_CHANNEL_CNT] = {0};
INT8S*					g_TempChDataPtr[_MAX_TRAN_ARRAY_LTH][_MAX_JKEY_CHANNEL_CNT] = {0};//瞬态数据键相分组指针,用于瞬态抽样

/*瞬态数据区标志，0，闲，1，正在采集，2，正在传输*/
INT32S					g_nTranFlag[_MAX_TRAN_ARRAY_LTH][_MAX_JKEY_CHANNEL_CNT] = {0};
/*触发时对应的瞬态数据区位置*/
INT32S					g_nTranTrigerSit[_MAX_TRAN_ARRAY_LTH][_MAX_JKEY_CHANNEL_CNT] = {0};
/*前触发数据起始位置*/
INT32S					g_nPreTranTrigerSit[_MAX_TRAN_ARRAY_LTH][_MAX_JKEY_CHANNEL_CNT] = {0};
/*已经采集的当前瞬态数据组数*/
INT32S					g_nTranPickcnt[_MAX_TRAN_ARRAY_LTH][_MAX_JKEY_CHANNEL_CNT];
INT32S					g_nPreAlertFlag[_MAX_JKEY_CHANNEL_CNT][_MAX_SIGNAL_CHANNEL_CNT];

INT32S					g_nStaticCurWriteSit = 0;
INT32S					g_nStaticCurReadSit = 0;
INT32S					g_nStaticValidcnt = 0;

INT32S					g_nDynamicCurWriteSit = 0;
INT32S					g_nDynamicCurReadSit = 0;
INT32S					g_nDynamicValidcnt = 0;

INT32S					g_nStaticType[_MAX_STATIC_ARRAY_LTH];
INT32S					g_nStaticTime[_MAX_STATIC_ARRAY_LTH];
INT32S					g_nStaticChcnt[_MAX_STATIC_ARRAY_LTH];

INT32S					g_nDynamicType[_MAX_DYNAMIC_ARRAY_LTH];
INT32S					g_nDynamicAlert[_MAX_DYNAMIC_ARRAY_LTH][_MAX_JKEY_CHANNEL_CNT];
INT32S					g_nDynamicTime[_MAX_DYNAMIC_ARRAY_LTH];
INT32S					g_nDynamicChcnt[_MAX_DYNAMIC_ARRAY_LTH];

struct tagTranEvent		g_TranEventArray[_MAX_TRAN_EVENT];
struct tagTranEvent	   *g_pTranEventArray[_MAX_TRAN_EVENT];

INT32S					g_nStaticManualPick[_MAX_JKEY_CHANNEL_CNT] = {0};
INT32S					g_nDynamicManualPick[_MAX_JKEY_CHANNEL_CNT] = {0};
INT32S					g_nTranManualPick[_MAX_JKEY_CHANNEL_CNT] = {0};//不用,保留


//struct tagChannelCalibratioRTData	g_RTCalibrationData;//通道实时校验信号

INT32S					g_nRTValueRequest[_MAX_SIGNAL_CHANNEL_CNT];//测点（按序号）请求列表
INT32S					g_nRTValueMrsPt[_MAX_SIGNAL_CHANNEL_CNT];//各个请求测量值的测点序号
INT32S					g_nRTValueMrsPtcnt = 0;//请求测量值的测点数量
INT32S					g_nRTValueMrsPtChcnt = 0;//所有测量值请求的通道数量
INT32S					g_nRTValueMrsPtChSit = 0;//所有通道的累积测量值指针偏移
INT32S					g_nRTValueBlockcnt = 0;//测量值块数
INT32S					g_nRTValueBlockLth[(_MAX_SIGNAL_CHANNEL_CNT-1)/_MAX_VALUE_TRANSFER_CNT+1] = {0};//各个测量值块长度
INT32S					g_nRTValueBlockMrsptcnt[(_MAX_SIGNAL_CHANNEL_CNT-1)/_MAX_VALUE_TRANSFER_CNT+1] = {0};//每一块实时测量值中的测点数
INT32S					g_RTVBuffer[_MAX_SIGNAL_CHANNEL_CNT*sizeof(struct tagMrsptRTValue)/sizeof(INT32S)];
struct tagMrsptRTValue* g_RTValue = (struct tagMrsptRTValue*)g_RTVBuffer;
INT32S*					g_pRTValue[_MAX_SIGNAL_CHANNEL_CNT];//各个通道实时测量值指针
INT32S					g_nRTDataRequest[_MAX_SIGNAL_CHANNEL_CNT];
struct tagMrsptRTData*	g_pRTData[_MAX_SIGNAL_CHANNEL_CNT];//测点实时原始信号
INT32S					g_nRTCalibrationChannel = -1;

INT32S					g_bRTRequestChanging = 0;//实时测量值请求正在被修改
INT32S					g_bRTValueReady = 0;//实时测量值空闲
INT32S					g_bRTDataReady[_MAX_SIGNAL_CHANNEL_CNT] = {0};//实时原始信号：0，空闲，1，写入中，2，信号有效
INT32S					g_bRTCalibrationReady = 0;//实时校验数据空闲

INT32S					g_bStaticTransfering = 0;
INT32S					g_bDynamicTransfering = 0;
INT32S					g_bTranTransfering = 0;
INT32S					g_bStaticTransferingLinger = 0;
INT32S					g_bDynamicTransferingLinger = 0;
INT32S					g_bDataPipeValid = 1;
INT32S					g_bSignalPipeValid = 1;

INT32S					g_bStaticPipeValid = 0;
INT32S					g_bDynamicPipeValid = 0;
INT32S					g_bTranPipeValid = 0;

#endif
