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

//�޸ļ������������ݳ�ȡ��������Ĭд���벻���Զ���Ӧ����Ҫ�ֶ��޸Ĳ��ִ�����/*manual*/���
#define _RAMDISK_CLUSTER_LTH				4
#define _SIMU
#define _BASIC_DEVIEID					20600
#define _BASE_RAM_DISK_CHDATA_OFFSET	1024*16//ͨ�������ڵ������е���ʼƫ�Ƶ�ַ
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

#define _MIN_DYNAMIC_CHANNEL_DATA_SAMPLE_STEP	1//��С��̬���ݳ������
#define _MAX_DYNAMIC_CHANNEL_DATA_SAMPLE_STEP	60//���̬���ݳ������

#define _MIN_TRAN_CHANNEL_DATA_SAMPLE_STEP		1//���˲̬���ݳ�����������治��
#define _MAX_TRAN_CHANNEL_DATA_SAMPLE_STEP		6//���˲̬���ݳ�����������治��

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
  INT32S nMaxStaticGroup;//��̬���ݶ��г���,Ĭ��100
  INT32S nMaxDyammicGroup;//��̬���ݶ��г���,Ĭ��10
  INT32S nMaxTranGroup;//˲̬���ݶ��г���,�̶�Ϊ1
  INT32S nMaxTranBufferTime;//˲̬���ݻ���ʱ��,Ĭ��5s
}Data_Limit_83;

typedef struct tagParam_Point
{
  INT32S jkey;
  INT32S nChCnt;
  INT32S channel[2];
}Param_Point;

typedef struct tag12SignChannel
{
  INT32U		nChNo;						//ͨ�����	0~47	���޸�	ת��Ϊ�ۺź�ͨ����	
  INT32U		nChType;					//ͨ������	0~255	λ�ơ��ٶȡ����ٶȡ��¶ȡ����࣬������չ		
  FP32		fChFullScaleLow;					//���̵�,����ԭʼ���ݵĹ�һ������		
  FP32		fChFullScaleHigh;					//���̸�,����ԭʼ���ݵĹ�һ������		
  FP32		fSensority;					//������������	����ֵ���ѹ�����������		
  FP32		fZeroVolt;					//����ѹ	����ֵ����Ӧ�ĵ�ѹ		
  INT32U		nCalcMode;//����������,������λ��	��	INT32U 	0~2	�����֡�һ�λ��֡����λ���		
  INT32U		nOutputMode;//����ֵ����	��	INT32U 	0~3	pk��pk-pk��rms��avr		
  ///����ֵ�����ʽ	��	INT32U 	0~2	opc��4~20ma�豸��modbus��ȡ����		
  INT32U 		bAlert;						//��������	��	INT32U 	0��1	���������		
  INT32U 		bAlertLock;					//��������	��	INT32U 	0��1	����������		
  FP32 		fAlertLevelLow;				//����	��	FP32	��	alert����		
  FP32 		fDangerLevelLow;				//Σ��	��	FP32	��	danger����	
  FP32 		fAlertLevelHigh;				//����	��	FP32	��	alert����		
  FP32 		fDangerLevelHigh;			//Σ��	��	FP32	��	danger����

  FP32		fFreqResponseLow;			//	Ƶ������	��	FP32	��	12ģ���Ѿ��涨�����޸�		
  FP32		fFreqResponsehigh;			//Ƶ������	��	FP32	��	12ģ���Ѿ��涨�����޸�

  INT32U 		nPickFrequency;	

  ////FP32		2�˲���Χ	��	FP32	��	?		
  INT32U		bSync;						//ͬ����־	��	INT32U 	0��1	1��ͬ����0���첽		
  FP32		fFreqencyRes;				//Ƶ��ֱ���,�첽ģʽ�´���Ƶ��ֱ��ʣ�ͬ��ģʽΪ��������		
  INT32S		nJkeyNo;					//����ͨ����� 	��	INT32U 	0~��			

  INT32U		nInstallAngle;				//��������װ�Ƕ�,	0~360
  //���в����뼸��������أ�����ĳЩ��ͼʱ��Ҫ		
  INT32U		bCircleFlag;				//��Բ��־	��	INT32U 	0��1	0������Բ��1����Բ		
  FP32		fAxa;						//��Բ����	��	FP32	��			
  FP32		fAxb;						//��Բ����	��	FP32	��			
  INT32U		byTrustDiretion;			//����̽ͷ�ƽ�����	��	INT32U 	0��1	0��������1��Զ��
  FP32		fGapLow;					//��϶��	��	FP32	��	
  FP32		fGapHigh;					//��϶��	��	FP32	��	
  INT32U		bEn;						//������λ					
  /////			
}Signal_Channel_12;
//�ź�ģ������ṹ
typedef struct tag12Param
{
  INT32S  nProtol;			//��������λ���ϴ�����
  INT32S  nSonProtol;			//��������λ���ϴ�����

  INT32U	nDeviceNo;			//�豸���;
  INT32S	nDeviceProperty;	//�豸���ԣ�0 ���ݲɼ��� 6 ����������ģ�� 4 ����ģ�� 5 �̵���ģ�飻
  INT32S	nDeviceTypeID;	//�豸����ID;
  INT32S	nDeviceID;//�豸ID;

  INT32U	nVersion;
  INT32U	nRenewID;//>0
  INT32S	nRenewTime;//byte 0~1,year,2,month,3,day

  INT32S	nChDeviceChNo[_MAX_SIGNAL_CHANNEL_CNT][2];//ͨ����������������
  INT32S	nSignChannelNum;		//�豸�ź�ͨ��������
  INT32S	bWork[_MAX_SIGNAL_CHANNEL_CNT];


  INT32U  nSignalChannelDynamicDataSize[_MAX_SIGNAL_CHANNEL_CNT];//ͨ��ԭʼ��̬���ݴ�С
  INT32U  nSignalChannelTranDataSize[_MAX_SIGNAL_CHANNEL_CNT];//ͨ��ԭʼ˲̬���ݴ�С

  INT32S	nMrsPt[_MAX_SIGNAL_CHANNEL_CNT];//ͨ����������
  INT32S	nChIndexMrsPt[_MAX_SIGNAL_CHANNEL_CNT];//ͨ����������������
  INT32S	nMrsPtIndex[_MAX_SIGNAL_CHANNEL_CNT];//ͨ������������
  INT32S  nMrsPtcnt;//�������
  INT32S  nMrsPtTable[_MAX_SIGNAL_CHANNEL_CNT];//����б�
  INT32S	nMrsPtChcnt[_MAX_SIGNAL_CHANNEL_CNT];//�������ͨ������

  INT32S	nChannelNumIn83[_MAX_SIGNAL_CHANNEL_CNT*2];//ͨ����83ģ���е����

  INT32S	nMrsPtValueSize[_MAX_SIGNAL_CHANNEL_CNT];//�������ʵʱ����ֵ��С
  INT32S	nMrsPtDataSize[_MAX_SIGNAL_CHANNEL_CNT];//�������ʵʱͨ�����ݴ�С
  INT32S	nMrsPtDataSize0[_MAX_SIGNAL_CHANNEL_CNT];//�������0ͨ��ʵʱͨ�����ݴ�С
  Signal_Channel_12 SignalChannel[_MAX_SIGNAL_CHANNEL_CNT];
}Param_Table_12;

typedef struct tagKeyChannel
{
  INT32U		nChNo;						//ͨ�����	0~47	���޸�	ת��Ϊ�ۺź�ͨ����	
  INT32U		nChType;					//ͨ�����ͣ�255
  INT32U 		bMaster;					//0��1��1Ϊ������	
  INT32U 		nGearcnt;					//ÿת�ļ��������>=1������

  INT32U 		bTran;						//0��1��1����ת�ٱ���


  INT32U		nTrigerDRPM;				//�ٽ紥��ת�٣���λΪ0.01ת/��		
  INT32U 		nTrigerTimeScope;			//�ж�ת�ٴ�����ʱ�䴰��ȣ���λΪ����

  INT32U 		fWokeRPM;					//����ת��

  FP32		fRPMLow;					//����ת������	��	FP32	��	12ģ���Ѿ��涨�����޸�		
  FP32		fRPMHigh;					//����ת������	��	FP32	��	12ģ���Ѿ��涨�����޸�		

}Key_Channel;

/*һ��83�����豸����ĳһ������������Чͨ���ļ��ϣ����һ��ͨ����12ģ���б���־Ϊ��Ч�������\
���������豸�г���*/
typedef struct tag83Device
{
  INT32U	nDeviceNo;				//�豸���;
  INT32S	nDeviceProperty;		//�豸���ԣ�0 ���ݲɼ��� 6 ����������ģ�� 4 ����ģ�� 5 �̵���ģ�飻
  INT32S	nDeviceTypeID;			//�豸����ID;
  INT32S	nDeviceID;				//�豸ID;

  FP32 	nFrequencyLow;//��λ���趨��Ƶ������,�Ŵ�100��
  FP32 	nFrequencyHigh;//��λ���趨��Ƶ�����ޣ��Ŵ�100��

  INT32U	nPickDynamicFrequency;//���������ֻ�Ƶ�ʣ��Ŵ�100��
  INT32U	nPickTranFrequency;//���������ֻ�Ƶ�ʣ��Ŵ�100��
  INT32U	nDynamicPickTime;//��̬�ɼ�����ʱ��
  INT32U	nTranPickTime;//˲̬�ɼ�����ʱ��
  INT32S	nDynamicChannelDataSampleStep;//����ͨ���Ķ�̬��ȡ����
  INT32S	nTranChannelDataSampleStep;//����ͨ����˲̬��ȡ����
  INT32S	nSignal_Channel_Dynamic_Data_Lth;//����ͨ���Ķ�̬���ݳ���
  INT32S	nSignal_Channel_Tran_Data_Lth;//����ͨ����˲̬̬���ݳ���

  INT32S	nChannelMask[_MAX_SIGNAL_CHANNEL_CNT];//1,����0��ֹ
  INT32S	nStaticMask[_MAX_SIGNAL_CHANNEL_CNT];//ȫ���̻�Ϊ1�����޸�
  INT32S	nDynamicMask[_MAX_SIGNAL_CHANNEL_CNT];//��ѡ��Ĵ�
  INT32S	nTranMask[_MAX_SIGNAL_CHANNEL_CNT];//��ѡ��Ĵ�

  INT32S	bAlertTrigerStatic;		//����������̬��1������������0����Ч���̶�Ϊ1
  INT32S	bAlertTrigerDynamic;	//����������̬��1������������0����Ч

  INT32S	nStaticAutoPickTime;	//�Զ���̬�ɼ�ʱ����������0��Ч
  INT32S	nDynamicAutoPickTime;	//�Զ���̬�ɼ�ʱ����������0��Ч
  INT32U	nDynamicSampleMode;//0���첽�ɼ���1��ͬ���ɼ�
  INT32U	nDynamicSynDataCntPerCycle;//��̬ͬ������ÿ������С����
  INT32U	nDynamicSynRPM;//��̬ͬ����Ƶ�ٽ�ת��

  INT32U 	nPickTimeScopePre;		//δ����ʱ�ݴ�ʱ�䣬��λΪ����

  INT32U	nTranSampleMode;			//0��ʱ����������1��ת�ټ������
  INT32U 	nPickTimeIntervalPre;	//δ����ʱ�ݴ�ʱ��������λΪ����	
  INT32U 	nSampleTimeInterval;	//����ʱ����ʱ��������λΪ����
  INT32U	nSampleRPMInterval;		//����ת�ټ������λΪ1ת/��	
  INT32U	nRealSampleTimeInterval;//�����ݲ����������˲̬������������
  INT32U	nRealPickTimeIntervalPre;//�����ݲ����������˲̬����ǰ�������

  INT32U	nMaxTranSampleTime;		//���˲̬�ɼ�ʱ��
  INT32U	nMaxTranSampleCnt;		//���˲̬�ɼ�������ĿǰΪ200
  INT32U  nTranPreTrigerGroupcnt;		//����ǰ˲̬����Ԥ������
  INT32U	nMinPreTranTickcnt;
  INT32U	nTranSynSampleMode;//0���첽�ɼ���1��ͬ���ɼ������汾�̶�Ϊ0
  INT32U	nTranSynDataCntPerCycle;//˲̬ͬ������ÿ������С���������汾��֧��

  INT32S	nMrsPtNo[_MAX_SIGNAL_CHANNEL_CNT];//�������
  INT32S  nPtMrsNo[_MAX_SIGNAL_CHANNEL_CNT][3];//����ͨ��
  INT32S	nChIndexMrsPt[_MAX_SIGNAL_CHANNEL_CNT];//ͨ����������������
  INT32S	nWorkChNo[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ����Ŷ�Ӧ��ͨ����

  INT32S  nPreTranTrigerTime;		//�Ƿ�ͨ��˳�����

  INT32S	nKeyChno;				//����ͨ����	

  INT32S	nSignChannelNum;		//�豸�ź�ͨ��������
  INT32S	nStaticChannelNum;		//�豸�ź�ͨ��������
  INT32S	nDynamicChannelNum;		//�豸�ź�ͨ��������
  INT32S	nTranChannelNum;		//�豸�ź�ͨ��������

  INT32S	nKeyChannelNum;		//�豸����ͨ��������
  Key_Channel KeyChannel[1];

  INT32S	nAllChValueSize;			//����ͨ���Ĳ���ֵ�ܳ���
  INT32S	nAllChStaticValueSize;			//����ͨ���ľ�̬����ֵ�ܳ���
  INT32S	nAllChDynamicValueSize;			//����ͨ���Ķ�̬����ֵ�ܳ���
  INT32S	nAllChTranValueSize;			//����ͨ����˲̬����ֵ�ܳ���
  //	INT32S	nAllChDataSize;			//����ͨ���������ܳ���
  INT32S	nAllDynamicChDataSize;			//����ͨ���������ܳ���
  INT32S	nAllTranChDataSize;			//����ͨ���������ܳ���
  INT32S	nChannelNumIn12[_MAX_SIGNAL_CHANNEL_CNT];//ͨ����12ģ���е����
  INT32S	nChValueSize;//ͨ������ֵ����
  INT32S	nChDynamicDataSize;//ͨ�����ݳ���
  INT32S	nChTranDataSize;//ͨ�����ݳ���
  INT32S	nChDynamicDataBlockCnt;//ͨ����������
  INT32S	nChTranDataBlockCnt;//ͨ�����ݳ�������
  INT32S	nChDynamicBlockSize;//ͨ�����γ������ݿ鳤��
  INT32S	nChTranBlockSize;//ͨ�����γ������ݿ鳤��
  INT32S	nChKeyBlockSize;//�����ߴ�
  //	INT32S	nDatalth[_MAX_SIGNAL_CHANNEL_CNT];//ͨ��ԭʼ���ݳ���
}Param_Table_Device;
/*83�����豸���ǽ�12����ͨ���������󶨵ļ�����л��֣���Ϊ�����豸�����һ������ͨ����12ģ���б���־Ϊ��Ч�����\
�򲻲��������豸�г���*/

typedef struct tag83Param
{
  INT32S  nProtol;				//��������λ���ϴ�����
  INT32S  nSonProtol;				//��������λ���ϴ�����
  INT32S	nSignChannelNum;		//
  INT32S	nStaticChannelNum;		//�豸�ź�ͨ��������
  INT32S	nDynamicChannelNum;		//�豸�ź�ͨ��������
  INT32S	nTranChannelNum;		//�豸�ź�ͨ��������
  INT32S	nAllChValueSize;		//����ͨ���Ĳ���ֵ�ܳ���
  INT32S	nAllChStaticValueSize;	//����ͨ���ľ�̬����ֵ�ܳ���
  INT32S	nAllChDynamicValueSize;	//����ͨ���Ķ�̬����ֵ�ܳ���
  INT32S	nAllChTranValueSize;	//����ͨ����˲̬����ֵ�ܳ���
  //	INT32S	nAllChDataSize;		//����ͨ���������ܳ���
  INT32S	nAllDynamicChDataSize;	//����ͨ���������ܳ���
  INT32S	nAllTranChDataSize;		//����ͨ���������ܳ���
  INT32U	nDeviceNo;				//�豸���;
  INT32S	nDeviceProperty;		//�豸���ԣ�0 ���ݲɼ��� 6 ����������ģ�� 4 ����ģ�� 5 �̵���ģ�飻
  INT32S	nDeviceTypeID;			//�豸����ID;
  INT32S	nDeviceID;				//�豸ID;
  INT32S	nDevicecnt;				//�豸����

  INT32S	nStaticAutoPickTime;	//�Զ���̬�ɼ�ʱ����������0��Ч
  INT32S	nDynamicAutoPickTime;	//�Զ���̬�ɼ�ʱ����������0��Ч

  Param_Point  nPtMrsNo[_MAX_SIGNAL_CHANNEL_CNT];//�������ͨ��
  INT32S  nMrsPtcnt; //�������
  INT32S  nMrsPtTable[_MAX_SIGNAL_CHANNEL_CNT];//����б�
  Param_Table_Device	Device[_MAX_JKEY_CHANNEL_CNT];
}Param_Table_83;
typedef struct tagRunTimeFlag
{
  INT32S	bRunning;//0,ֹͣ��1������
  INT32S	nAlertFlag[_MAX_JKEY_CHANNEL_CNT];//������־����������ʱ���ݲɼ�ģ�����ã�0���ޱ�����1��������2��Σ��
  INT32S	nTranFlag[_MAX_JKEY_CHANNEL_CNT];//ת�ٱ�����־���ޱ�����1��������
  INT32S	nPickFlag[_MAX_JKEY_CHANNEL_CNT];//���ݲɼ���־��0���޲ɼ���1����̬��2����̬��3��˲̬
  INT32S	nValidDataType;//��Ч�������ͣ����ݲɼ�ģ�����ã�1����̬��2����̬��3��˲̬
  INT32S	nVaildStaticStart;//��̬��Ч��Χ�����ݲɼ�ģ�����ã���ʼ���
  INT32S	nVaildStaticcnt;//��̬��Ч��Χ�����ݲɼ�ģ������,��������
  INT32S	nVaildDynamicStart;//��̬��Ч��Χ�����ݲɼ�ģ�����ã���ʼ���
  INT32S	nVaildDynamiccnt;//��̬��Ч��Χ�����ݲɼ�ģ������,��������
  INT32S	nValidTranNo;//˲̬��Ч����  ���ݲɼ�ģ�����ã�0��1
  INT32S	nInvaildStaticStart;//��̬��Ч��Χ�����ݲɼ�ģ�����ã���ʼ���
  INT32S	nInvaildStaticcnt;//��̬��Ч��Χ�����ݲɼ�ģ������,��������
  INT32S	nInvaildDynamicStart;//��̬��Ч��Χ�����ݲɼ�ģ�����ã���ʼ���
  INT32S	nInvaildDynamiccnt;//��̬��Ч��Χ�����ݲɼ�ģ������,��������
  INT32S	nInvalidTranNo;//˲̬��Ч����  ͨѶģ�����ã�0��1
  INT32S	nTransitFlag;//���ݴ����־��ͨѶģ�����ã�1����̬���䣬2����̬���䣬3��˲̬����
  INT32S	nOuterAlert;//�ⲿ������־��0���ޱ�����1��������2��Σ��
  INT32S	nMasterCommand;//PCM360�ⲿ���ͨѶģ�����ã����ݲɼ�ģ����Ӧ����ԭ
  /*1��	����ֵ���룬���ȼ���4
  2��	ʵʱ�������룬���ȼ���5��
  100��״̬��ѯ�����ȼ���1
  101���ֶ���̬�ɼ������ȼ���3
  102���ֶ���̬�ɼ������ȼ���2
  103��ֹͣ˲̬�ɼ������ȼ���1
  200������PT2060�����ȼ���0
  201��ֹͣPT2060�����ȼ���0
  202��PT2060�ϴ����������ȼ���6
  203��PT2060���ز��������ȼ���*/

}Run_Time_Flag;

typedef struct tagTst_Head_DCM_SigModuSampData_SYS
{ 
  union
  {
    struct
    {
      INT16U   unModuleType;      	// ģ������
      union
      {
        INT16U   unVal_PT2060;     // PT2060
        // ���������ϵͳ��ͨ����Žṹ
      }tunChNo;                    // ͨ�����
    }tstVal;
    INT32U  ulnVal;  
  }tunChInfo;      // ͨ����Ϣ

  INT32U   ulnWaveDataLen;         //��̬�������ݳ��ȡ� 0 �C ����������Ч������ �C ��Чֵ

  INT32U   ulnTmStamp_SYN1;         // ���ݲ���ͬ���ź�SYN1ʱ���
  INT32U   ulnTmStamp_LastWaveData;  // ���1����������ʱ���
  INT32U   ulnWaveDataSampFreq;		// �������ݲ���Ƶ�ʡ���17λΪ��������15λΪС������λHz
  INT32U   ulnWaveDataUnitage;		    // �������ݵ�λ������λ��nV

  union
  {
    struct
    { 
      INT16U  unFlag_RUN :1;          // ͨ����������ϵͳ״̬��־�� 1 == ��Ч��0 == ��Ч
      INT16U  unFlag_ChEnable :1;      // ͨ��ʹ�ܱ�־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_1XAEnable :1;     // 1XAʹ�ܱ�־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_1XPEnable :1;     // 1X Pʹ�ܱ�־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_2XAEnable :1;     // 2XAʹ�ܱ�־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_2XPEnable :1;     // 2XPʹ�ܱ�־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_Not1XEnable :1;   // Not1Xʹ�ܱ�־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_SMAXEnable :1;   // SMAXʹ�ܱ�־��1 == ��Ч��0 == ��Ч

      INT16U  unFlag_NotOK  :1;       // ͨ��NotOK��־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_ChCfgResoOK :1;   // ͨ�����ý����ɹ���־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_Danger  :1;       // ͨ�����Σ�ձ�����־�� 1 == ��Ч��0== ��Ч
      INT16U  unFlag_Alert    :1;       // ͨ����Ͼ��汨����־�� 1 == ��Ч��0 == ��Ч
      INT16U  unFlag_Bypass  :1;       // ͨ��������·��־�� 1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlarmMult :1;     // ͨ������������־�� 1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlarmLatch :1;    // ���������־��     1 == ���棬0 == ������
      INT16U  unSpace115 :1;        // ����   ȱʡֵΪ0
    }tstVal;
    INT16U  unVal;
  }tunStaticFlag1;

  union
  {
    struct
    { 
      INT16U  unFlag_Valid_GAP     :1;    // GAPֵ��Ч��־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_Valid_DIRECT  :1;    // DIRECTֵ��Ч��־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_Valid_1XA     :1;    // 1XAֵ��Ч��־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_Valid_1XP     :1;    // 1XPֵ��Ч��־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_Valid_2XA     :1;    // 2XAֵ��Ч��־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_Valid_2XP     :1;    // 2XPֵ��Ч��־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_Valid_NOT1X  :1;    // NOT1Xֵ��Ч��־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_Valid_SMAX   :1;    // SMAXֵ��Ч��־��1 == ��Ч��0 == ��Ч
      INT16U  unSpace8_15          :8;    // ���� ȱʡֵΪ0
    }tstVal;
    INT16U  unVal;
  }tunStaticFlag2;

  union
  {
    struct
    { 
      INT16U  unFlag_DangerH_GAP:1;   // GAP�߶�Σ�ձ�����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlertH_GAP:1;    // GAP�߶˾��汨����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlertL_GAP :1;    // GAP�Ͷ˾��汨����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_DangerL_GAP :1;  // GAP�Ͷ�Σ�ձ�����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_DangerH_DIRECT:1;   // DIRECT�߶�Σ�ձ�����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlertH_DIRECT:1;    // DIRECT�߶˾��汨����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlertL_DIRECT :1;    // DIRECT�Ͷ˾��汨����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_DangerL_DIRECT :1;  // DIRECT�Ͷ�Σ�ձ�����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_DangerH_1XA:1;   // 1XA�߶�Σ�ձ�����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlertH_1XA:1;    // 1XA�߶˾��汨����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlertL_1XA :1;    // 1XA�Ͷ˾��汨����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_DangerL_1XA :1;  // 1XA�Ͷ�Σ�ձ�����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_DangerH_1XP:1;   // 1XP�߶�Σ�ձ�����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlertH_1XP:1;    // 1XP�߶˾��汨����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlertL_1XP :1;    // 1XP�Ͷ˾��汨����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_DangerL_1XP :1;  // 1XP�Ͷ�Σ�ձ�����־��1 == ��Ч��0 == ��Ч
    }tstVal;
    INT16U  unVal;
  }tunStatic3;


  union
  {
    struct
    { 
      INT16U  unFlag_DangerH_2XA:1;   // 2XA�߶�Σ�ձ�����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlertH_2XA:1;    // 2XA�߶˾��汨����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlertL_2XA :1;    // 2XA�Ͷ˾��汨����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_DangerL_2XA :1;  // 2XA�Ͷ�Σ�ձ�����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_DangerH_2XP:1;   // 2XP�߶�Σ�ձ�����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlertH_2XP:1;    // 2XP�߶˾��汨����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlertL_2XP :1;    // 2XP�Ͷ˾��汨����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_DangerL_2XP :1;  // 2XP�Ͷ�Σ�ձ�����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_DangerH_Not1X:1;   // Not1X�߶�Σ�ձ�����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlertH_Not1X:1;    // Not1X �߶˾��汨����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlertL_Not1X :1;    // Not1X�Ͷ˾��汨����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_DangerL_Not1X :1;  // Not1X�Ͷ�Σ�ձ�����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_DangerH_SMAX:1;   // SMAX�߶�Σ�ձ�����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlertH_SMAX:1;    // SMAX�߶˾��汨����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_AlertL_SMAX :1;    // SMAX�Ͷ˾��汨����־��1 == ��Ч��0 == ��Ч
      INT16U  unFlag_DangerL_SMAX :1;  // SMAX�Ͷ�Σ�ձ�����־��1 == ��Ч��0 == ��Ч
    }tstVal;
    INT16U  unVal;
  }tunStaticFlag4;

  INT16U  unFS_Range;		// �����̷�Χ
  INT16S  nGapVal;			// ͨ��GAPֵ����ѹֵ����λ��mV

  union
  { 
    INT16U  unVal;
    INT16S  nVal;
  }tunDirect;			  // ͨ������ֵ
  INT16U   un1XA;		  // 1XAֵ

  INT16U   un1XP;		  // 1XPֵ
  INT16U   un2XA;		  // 2XAֵ

  INT16U   un2XP;		  // 2XPֵ
  INT16U   unNoT1X;	  // Not1Xֵ

  INT16U   unSMAX;	  // SMAXֵ
  INT16U   unSpace;	  // �������Ա����32λ��ȱʡֵΪ0

  INT32U   ulnaSpace[6];	  // �������Ա����20��32λ��ȱʡֵΪ0

  INT32U	 ulnRPM;
  INT32U	 ulnKeyPoint;

}Tst_Head_DCM_SigModuSampData_SYS;

typedef  struct tagChValue//����ֵ
{
  INT32U nTrigerTime;//ʱ���
  INT32S nChNo;//ͨ�����
  INT32S nAlertFlag;//	0��1��2	0���ޱ�����1��warning��2��danger
  INT32S nRpmFlag;//ת�ٱ���״̬	0��1	1��ת�ٱ���
  INT32S nRPM;//	����ֵ	ת��	rpm	
  INT32U nAllMeasureValue;//����ֵ	ͨƵ��ֵ	all	
  INT32U nAllPhase;//	����ֵ	ͨƵ��λ	all	
  INT32U n1xV;	//��Ƶ��ֵ	1X	
  INT32U n1xP;//��Ƶ��λ	1X	
  INT32U n2xV;//��Ƶ��ֵ	1X	
  INT32U n2xP;	//��Ƶ��λ	1X
  INT32U n05xV;//��Ƶ��ֵ	1X	
  INT32U n05xP;//��Ƶ��λ	1X	
  INT32U nn1xA;//��Ƶ��ֵ	1X	
  INT32U nn1xP;//��Ƶ��λ	1X	

}Channel_Static_Value,Channel_Dynamic_Value,Channel_Tran_Value;

typedef  struct tagChData2060//ԭʼ����
{
  INT32U nTrigerTime;//ʱ���
  INT32S nChNo;//ͨ�����
  INT32S nFrequency;//����Ƶ��
  INT32S nPiecelen;//��������
  INT32S nKeycnt;//�������
  //�����Ǽ����λ�ú�ԭʼ����
  //	INT32S* pnData;//������ǰ���ź��ں�
  INT32S pData[1];//
}Channel_Dynamic_Wave_Data,Channel_Tran_Wave_Data;
struct tagAlertEvent
{
  INT32U nAlertTime;//ʱ���
  INT32S nAlertChcnt;//������������
  INT32S nAlertChNo[_MAX_JKEY_CHANNEL_CNT];//���������
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
    INT32S nAlertFlag;     //0��1��2	0���ޱ�����1��warning��2��danger
    FP32 fAllMeasureValue; //����ֵ	ͨƵ��ֵ	all	
  }data[1];
}RTChValue;

typedef struct tagMrsptRTValue //���ʵʱ����ֵ��
{
  INT32U nPickTime;
  INT32S nMrsPtcnt;
  struct tagRTChValue nChValue[1];
}MrsptRTValue;

typedef struct tagMrsptRTData  //���ʵʱԭʼ�ź�
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
  INT32U ulnBlockCnt;//��Ч��������̬����Ϊ���
  //	INT32S ulnBlockLth;//�鳤��
  //	INT32S ulnDataHeadSit[_MAX_DYNAMIC_CHANNEL_DATA_BLOCK_CNT];//����
  //	INT32S ulDataLength[_MAX_DYNAMIC_CHANNEL_DATA_BLOCK_CNT];//����Ч���ݳ���
  INT32S lnHeadBlock;//��ʼ��
  INT32U ulnBlockStep;//�鲽��

  INT32S lnCurBlock;//��ǰ��
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
  INT32U ulnBlockCnt;//��Ч������˲̬Ŀǰʵ���ϲ���Ҫ����Ϊֻ��һ��
  //	INT32S ulnBlockLth;//�鳤��,����
  //	INT32S ulnDataHeadSit[_MAX_TRAN_CHANNEL_DATA_BLOCK_CNT];//����
  //	INT32S ulDataLength[_MAX_TRAN_CHANNEL_DATA_BLOCK_CNT];//����Ч���ݳ���
  INT32S lnHeadBlock;//��ʼ��
  INT32U ulnBlockStep;//�鲽��

  INT32S lnCurBlock;//��ǰ��
}Tran_Channel_Data_table;
typedef  struct tagTranTrigerTable
{	
  volatile INT32U ulnTrigerRead;//��ǰ��ȡλ��
  volatile INT32U ulnTrigerWrite;//��ǰд��λ��
  INT32U ulnTrigerTime[_MAX_TRAN_ARRAY_LTH];//����ʱ�����
  INT32U ulnTrigerReason[_MAX_TRAN_ARRAY_LTH];//����ԭ�� ����
  Tran_Channel_Data_table ChannelSignalDataTable[_MAX_TRAN_ARRAY_LTH][_MAX_TRAN_GROUP];
  INT32U ulnGroupCnt[_MAX_TRAN_ARRAY_LTH];
  INT32S lnGroupHeadSit[_MAX_TRAN_ARRAY_LTH];//	���δ�����˲̬��ʼ��λ��

  INT32U ulnTrigerTimeSpan;//˲̬��������ʱ��
  INT32U nPreTranTickCnt;//˲̬����ǰ��������
  INT32U ulnTrigerFlag;//0,δ����;1,�Ѿ�����;
  INT32U ulnCurWriteGroup;//˲̬��ǰ����

  INT32U ulnPreAlertFlag;//ǰ�εı���״̬,ʵ���������ж��Ƿ񴥷������ɼ�
  INT32U ulnPreRPM;//	��һ�����ݵ�ת��
  INT32U ulnAccumulateCnt;//�ϴα������ݺ���Ѿ���������

  INT32U ulnPick;//���״ε���ͨ�������Ƿ�����־
}Tran_Triger_Table;
typedef struct tagKeySpeedCalt
{
  INT32U		nTrigerLevel;//���������ۻ����ٶ�����
  INT32S		nSpeedArray[_MAX_JKEY_SPEED_ARRAY_LTH];//�ٶ���ʷ����
  INT32S		nPreAcumulateSpeed;//ǰһ�ζ����ٶȵĺ�
  INT32U		nCurLevel;
  INT32U		nRPMLevel[_MAX_RPM_LEVEL];//ͬ����ȡʱ����ת��
  INT32U		nSampleStep[_MAX_RPM_LEVEL];//ͬ����ȡʱ���׳�ȡ����
  INT32U		nSynBlockCnt[_MAX_RPM_LEVEL];//ͬ����ȡʱ���׳�ȡ����
  INT32U		nSynBlockIncreaseStep[_MAX_RPM_LEVEL];//ͬ����ȡʱ�������ݿ�ƫ�Ƶ�������
  //INT32S		nSpeed;//���ζ�ȡ���ٶ�	
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
//arm���ö�ȡ����ͨ���²����ļ����
INT32S ReadKeyPoint(INT32U nChannel,INT32S pData[],INT32S nCnt);
//arm���ö�ȡ����ͨ����ǰ�ٶ�
INT32S ReadKeySpeed(INT32U nChannelcnt,INT32U nChannel[],INT32S pData[]);
/////////��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��/////////
/*��������һ��δ�޸�ǰ�Ĺ�����*/
/////////��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��/////////
/////////��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��//��Ҫɾ��/////////
///////// 

#endif
