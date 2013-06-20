#ifndef GLOBEL83PARAM_H
#define GLOBEL83PARAM_H

#include "common.h"
#include "define2060.h"

INT8S						g_strHostIP[] = "192.168.1.116";

INT32U						g_nTcpPort = 20600;
INT32U						g_nUdpPort = 20610;
INT32S						g_nSystemNo = -1;//ϵͳ���
INT32S						g_nMgrNo = -1;//������
INT32S						g_nMchNo[_MAX_SIGNAL_CHANNEL_CNT] = {-1};//�������
INT32S						g_nDeviceNo[_MAX_SIGNAL_CHANNEL_CNT] = {-1};//�����豸���
const INT32S				g_n12UpFrequency = 10;//12ģ���ϴ����ݵ�Ƶ��,�ɺ������,�����޸�
const INT32S				g_TempAddtionalcnt = 10;//������ʱ��������
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
Data_Limit_83				g_83DataLimit;//83ģ���һЩ���޲���
INT32S						g_nChannelArrivalSequenceTable[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ����12ͨ����ű�
INT32S						g_nArrivalDeviceNo[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ�������豸��
INT32S						g_nArrivalChannelNo[_MAX_SIGNAL_CHANNEL_CNT];//ͨ�����ݵ����������豸��ͨ����
Param_Table_12				g_12Param @ "MYSECTION1";//12ģ�����,��12ģ���ȡ,������,��12congfigģ������,360ϵͳ�����޸�
Param_Table_83				g_83param @ "MYSECTION1";//83ģ�����,�洢��flash,�ɴ���λ������
Run_Time_Flag				g_RunTimeFlag;//83����״̬��Ϣ

/*�������������󶨵ļ������Ϊ�ڴ滮�ֵĻ��������⵽�ﰴ�ռ����-����ʱ��-����ͨ�������֯*/
/*��ǰ�������̬���ݰ��ռ����-��������-����ͨ����ŵ�ָ���б�,���ڿ��ٴ��źŴ���ģ����ٽ�������*/
/*��̬�Ͷ�̬�����Ա���ʱ����Ϊһ�βɼ�,���������������ͨ���������*/


/*��̬���������ڴ��С�����þ�̬���䣬��̬������Ҫ�Ա�����ʽ�������ض�ʱ��������ݵ���ʱ��һ��ͨ��������
����ͬ�����ͨ��һ����вɼ�,�ɼ���ɺ���������ľ�̬������ż�1*/
Tst_Head_DCM_SigModuSampData_SYS		g_StaticOriginData[_MAX_STATIC_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT] @ "MYSECTION1";//��̬����ֵ����
//����12ģ��ļ���-ͨ����б���,�������ټ�����̬����
Tst_Head_DCM_SigModuSampData_SYS*		g_pStaticChannelOrigin[_MAX_JKEY_CHANNEL_CNT][_MAX_STATIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT] @ "MYSECTION1";
//INT8S*									g_tmpStaticData;//�����;�̬����
INT8S  g_tmpStaticData[sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead)] @ "MYSECTION1";
ProtocolHead*						    g_tmpStaticDataHead;
Channel_Static_Value*					g_ptmpStaticData[_MAX_SIGNAL_CHANNEL_CNT];
Static_Triger_Table						g_StaticTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];

/*��̬���������ڴ�ϴ󣬲��ö�̬���䣬��̬������Ҫ�Ա�����ʽ�������ض�ʱ��������ݵ���ʱ��һ��ͨ��������
����ͬ�����ͨ��һ����вɼ���ÿ��ʱ����²ɼ���̬���ݵ�ͨ�������ǲ�ͬ��,�ɼ���ɺ���������Ķ�̬������ż�1*/
Tst_Head_DCM_SigModuSampData_SYS		g_DynamicValue[_MAX_DYNAMIC_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT] @ "MYSECTION1";//��̬̬����ֵ����
//����12ģ��ļ���-ͨ����б���,�������ټ�����̬����ͷ
Tst_Head_DCM_SigModuSampData_SYS*		g_pDynamicChannelValue[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT] @ "MYSECTION1";
Dynamic_Triger_Table					g_DynamicTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];
//INT8S*									g_tmpDynamicData;//�����Ͷ�̬����
//INT8S                                   g_tmpDynamicData[(sizeof(INT32S)*_DYAMIC_BUFFER_LTH+sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead))];
INT8S                                   g_tmpDynamicData[sizeof(INT32S)*_DYAMIC_BUFFER_LTH+sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead)] @ "MYSECTION1";

ProtocolHead*						    g_tmpDynamicDataHead;
Channel_Dynamic_Value*					g_ptmpDynamicValue[_MAX_SIGNAL_CHANNEL_CNT];//�����Ͷ�̬���ݲ���ֵ
Channel_Dynamic_Wave_Data*				g_ptmpDynamicData[_MAX_SIGNAL_CHANNEL_CNT];//�����Ͷ�̬ͨ��ԭʼ����
//��̬ͨ������ƫ�Ʊ�,��ʼ��ʱ����,ͬ��ָʾ������ͨ��д���ַ,Ҳ������������λ������ʱ���ٶ�λ�����ڵ����̵�λ��
INT32U									g_nChannel_Dynamic_Wave_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
INT32U									g_nChannel_Dynamic_Key_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH];

/*˲̬���������ڴ�ܴ�,���ö�̬����,���������豸����Ϊ��ͬ�豸��������,˲̬���ݶ��г�����󲻳���_MAX_TRAN_ARRAY_LTH��
����Ϊ1����Ϊ�������豸���������Ϊ���ݻ���Ϊͬ�ȴ�С������,�����ٰ���ͨ�����ϸ��,ͨ���ڲ��Դ��������ٴ�ϸ��
*/
Tst_Head_DCM_SigModuSampData_SYS		g_TranValue[_MAX_TRAN_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT*_MAX_TRAN_GROUP] @ "MYSECTION1";//˲̬����ֵ����
//����12ģ��ļ���-ͨ����б���,�������ټ���˲̬̬����ͷ
Tst_Head_DCM_SigModuSampData_SYS*		g_pTranChannelValue[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT][_MAX_TRAN_GROUP] @ "MYSECTION1";
Tran_Triger_Table						g_TranTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];
//INT8S*									g_tmpTranData;//������˲̬����
INT8S  g_tmpTranData[(sizeof(INT32S)*_TRAN_BUFFER_LTH+sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead))];


ProtocolHead*							g_tmpTranDataHead;
Channel_Tran_Value*						g_ptmpTranValue[_MAX_SIGNAL_CHANNEL_CNT];//�����Ͷ�̬���ݲ���ֵ
Channel_Tran_Wave_Data*					g_ptmpTranData[_MAX_SIGNAL_CHANNEL_CNT];
//˲̬ͨ������ƫ�Ʊ�,��ʼ��ʱ����,ͬ��ָʾ������ͨ��д���ַ,Ҳ������������λ������ʱ���ٶ�λ�����ڵ����̵�λ��
INT32U		g_nChannel_Tran_Wave_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT][_MAX_TRAN_GROUP];
INT32U		g_nChannel_Tran_Key_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_TRAN_GROUP];

//INT32U		g_Signal_Channel_Dynamic_Write_Position[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ���Ķ�̬����д��λ��
//INT32U		g_Signal_Channel_Tran_Write_Position[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ����˲̬����д��λ��
//INT32U		g_Key_Channel_Dynamic_Write_Position[_MAX_JKEY_CHANNEL_CNT];//����ͨ���Ķ�̬��������д��λ��
//INT32U		g_Key_Channel_Tran_Write_Position[_MAX_JKEY_CHANNEL_CNT];//����ͨ����˲̬��������д��λ��
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
/*manual*///�޸ļ������������ݳ�ȡ����ʱ����ȫ�ֱ����ĳ�ʼ����Ҫ�ֹ��޸�
//Key_Speed_Calt	g_DynamicSpeedCalt[_MAX_JKEY_CHANNEL_CNT];//���������ٶȶ�̬����ṹ
//Key_Speed_Calt	g_TranSpeedCalt[_MAX_JKEY_CHANNEL_CNT];//���������ٶ�˲̬����ṹ

INT32S		g_nTempCurWriteSit[_MAX_JKEY_CHANNEL_CNT] = {0};//�����豸�ĵ�ǰ˲̬д��λ��
INT32S		g_nTempCurReadSit[_MAX_TRAN_ARRAY_LTH] = {0};
INT32S		g_nTempValidcntr[_MAX_TRAN_ARRAY_LTH] = {0};
/*��ʱ����ֵ�������ָ��,����˲̬����,��һ����Ϊ��ţ��ڶ�����Ϊ�����豸��*/
struct tagChValue*		g_TempChValuePtr[_MAX_TRAN_ARRAY_LTH][_MAX_JKEY_CHANNEL_CNT] = {0};
INT8S*					g_TempChDataPtr[_MAX_TRAN_ARRAY_LTH][_MAX_JKEY_CHANNEL_CNT] = {0};//˲̬���ݼ������ָ��,����˲̬����

/*˲̬��������־��0���У�1�����ڲɼ���2�����ڴ���*/
INT32S					g_nTranFlag[_MAX_TRAN_ARRAY_LTH][_MAX_JKEY_CHANNEL_CNT] = {0};
/*����ʱ��Ӧ��˲̬������λ��*/
INT32S					g_nTranTrigerSit[_MAX_TRAN_ARRAY_LTH][_MAX_JKEY_CHANNEL_CNT] = {0};
/*ǰ����������ʼλ��*/
INT32S					g_nPreTranTrigerSit[_MAX_TRAN_ARRAY_LTH][_MAX_JKEY_CHANNEL_CNT] = {0};
/*�Ѿ��ɼ��ĵ�ǰ˲̬��������*/
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
INT32S					g_nTranManualPick[_MAX_JKEY_CHANNEL_CNT] = {0};//����,����


//struct tagChannelCalibratioRTData	g_RTCalibrationData;//ͨ��ʵʱУ���ź�

INT32S					g_nRTValueRequest[_MAX_SIGNAL_CHANNEL_CNT];//��㣨����ţ������б�
INT32S					g_nRTValueMrsPt[_MAX_SIGNAL_CHANNEL_CNT];//�����������ֵ�Ĳ�����
INT32S					g_nRTValueMrsPtcnt = 0;//�������ֵ�Ĳ������
INT32S					g_nRTValueMrsPtChcnt = 0;//���в���ֵ�����ͨ������
INT32S					g_nRTValueMrsPtChSit = 0;//����ͨ�����ۻ�����ֵָ��ƫ��
INT32S					g_nRTValueBlockcnt = 0;//����ֵ����
INT32S					g_nRTValueBlockLth[(_MAX_SIGNAL_CHANNEL_CNT-1)/_MAX_VALUE_TRANSFER_CNT+1] = {0};//��������ֵ�鳤��
INT32S					g_nRTValueBlockMrsptcnt[(_MAX_SIGNAL_CHANNEL_CNT-1)/_MAX_VALUE_TRANSFER_CNT+1] = {0};//ÿһ��ʵʱ����ֵ�еĲ����
INT32S					g_RTVBuffer[_MAX_SIGNAL_CHANNEL_CNT*sizeof(struct tagMrsptRTValue)/sizeof(INT32S)];
struct tagMrsptRTValue* g_RTValue = (struct tagMrsptRTValue*)g_RTVBuffer;
INT32S*					g_pRTValue[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ��ʵʱ����ֵָ��
INT32S					g_nRTDataRequest[_MAX_SIGNAL_CHANNEL_CNT];
struct tagMrsptRTData*	g_pRTData[_MAX_SIGNAL_CHANNEL_CNT];//���ʵʱԭʼ�ź�
INT32S					g_nRTCalibrationChannel = -1;

INT32S					g_bRTRequestChanging = 0;//ʵʱ����ֵ�������ڱ��޸�
INT32S					g_bRTValueReady = 0;//ʵʱ����ֵ����
INT32S					g_bRTDataReady[_MAX_SIGNAL_CHANNEL_CNT] = {0};//ʵʱԭʼ�źţ�0�����У�1��д���У�2���ź���Ч
INT32S					g_bRTCalibrationReady = 0;//ʵʱУ�����ݿ���

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
