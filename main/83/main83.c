#include "define2060.h"
#include "ucos_ii.h"
#include "common.h"
#include "interface.h"
#include "lib_mem.h"
#include "main83.h"
#include "netio.h"
#include <string.h>
//#include "../simudata//simudata.h"
//#include <sys/stat.h>
//
extern INT8S						g_strHostIP[];
extern INT32U						g_nTcpPort;
extern INT32U						g_nUdpPort;
extern INT32S						g_nSystemNo;//ϵͳ���
extern INT32S						g_nMgrNo;//������
extern INT32S						g_nMchNo[_MAX_SIGNAL_CHANNEL_CNT];//�������
extern INT32S						g_nDeviceNo[_MAX_SIGNAL_CHANNEL_CNT];//�����豸���
extern const INT32S				    g_n12UpFrequency;//12ģ���ϴ����ݵ�Ƶ��,�ɺ������,�����޸�
extern const INT32S				    g_TempAddtionalcnt;//������ʱ��������
extern const INT32S				    g_nMaxTranGroupDataSize;
extern const INT32S				    g_nMaxTranDataGroup;
extern const INT32S				    g_nMaxDataSize;
extern const INT32S				    g_nTranGroupcnt;
//
extern Tst_Head_DCM_SigModuSampData_SYS		g_StaticOriginData[_MAX_STATIC_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT];//��̬����ֵ����
extern Tst_Head_DCM_SigModuSampData_SYS*	g_pStaticChannelOrigin[_MAX_JKEY_CHANNEL_CNT][_MAX_STATIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
extern Static_Triger_Table					g_StaticTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];
//
extern INT32U									g_nChannel_Dynamic_Wave_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
extern INT32U									g_nChannel_Dynamic_Key_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH];
//
extern Data_Limit_83				g_83DataLimit;//83ģ���һЩ���޲���
extern INT32S						g_nChannelArrivalSequenceTable[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ����12ͨ����ű�
extern INT32S						g_nArrivalDeviceNo[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ�������豸��
extern INT32S						g_nArrivalChannelNo[_MAX_SIGNAL_CHANNEL_CNT];//ͨ�����ݵ����������豸��ͨ����
extern Param_Table_12				g_12Param;//12ģ�����,��12ģ���ȡ,������,��12congfigģ������,360ϵͳ�����޸�
extern Param_Table_83				g_83param;//83ģ�����,�洢��flash,�ɴ���λ������
extern Run_Time_Flag				g_RunTimeFlag;//83����״̬��Ϣ
//
extern Tst_Head_DCM_SigModuSampData_SYS		g_StaticOriginData[_MAX_STATIC_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT];
extern Tst_Head_DCM_SigModuSampData_SYS*		g_pStaticChannelOrigin[_MAX_JKEY_CHANNEL_CNT][_MAX_STATIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
extern INT8S  g_tmpStaticData[sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead)];
extern ProtocolHead*						    g_tmpStaticDataHead;
extern Channel_Static_Value*					g_ptmpStaticData[_MAX_SIGNAL_CHANNEL_CNT];
extern Static_Triger_Table						g_StaticTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];
//
extern Tst_Head_DCM_SigModuSampData_SYS		g_DynamicValue[_MAX_DYNAMIC_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT];
extern Tst_Head_DCM_SigModuSampData_SYS*		g_pDynamicChannelValue[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
extern Dynamic_Triger_Table					g_DynamicTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];
//extern INT8S                                   g_tmpDynamicData[(sizeof(INT32S)*_DYAMIC_BUFFER_LTH+sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead))];;
extern INT8S                                   g_tmpDynamicData[sizeof(INT32S)*_DYAMIC_BUFFER_LTH+sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead)];

extern ProtocolHead*					     	g_tmpDynamicDataHead;
extern Channel_Dynamic_Value*					g_ptmpDynamicValue[_MAX_SIGNAL_CHANNEL_CNT];//�����Ͷ�̬���ݲ���ֵ
extern Channel_Dynamic_Wave_Data*				g_ptmpDynamicData[_MAX_SIGNAL_CHANNEL_CNT];//�����Ͷ�̬ͨ��ԭʼ����
extern INT32U									g_nChannel_Dynamic_Wave_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
extern INT32U									g_nChannel_Dynamic_Key_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH];
//
extern Tst_Head_DCM_SigModuSampData_SYS		    g_TranValue[_MAX_TRAN_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT*_MAX_TRAN_GROUP];//˲̬����ֵ����
//����12ģ��ļ���-ͨ����б���,�������ټ���˲̬̬����ͷ
extern Tst_Head_DCM_SigModuSampData_SYS*		g_pTranChannelValue[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT][_MAX_TRAN_GROUP];
extern Tran_Triger_Table						g_TranTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];
//extern INT8S*									g_tmpTranData;//������˲̬����
extern INT8S  g_tmpTranData[(sizeof(INT32S)*_TRAN_BUFFER_LTH+sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead))];


extern ProtocolHead*							g_tmpTranDataHead;
extern Channel_Tran_Value*						g_ptmpTranValue[_MAX_SIGNAL_CHANNEL_CNT];//�����Ͷ�̬���ݲ���ֵ
extern Channel_Tran_Wave_Data*					g_ptmpTranData[_MAX_SIGNAL_CHANNEL_CNT];
//
extern INT32U		g_nChannel_Tran_Wave_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT][_MAX_TRAN_GROUP];
extern INT32U		g_nChannel_Tran_Key_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_TRAN_GROUP];

extern INT32U		g_Signal_Channel_Dynamic_Write_Position[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ���Ķ�̬����д��λ��
extern INT32U		g_Signal_Channel_Tran_Write_Position[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ����˲̬����д��λ��
extern INT32U		g_Key_Channel_Dynamic_Write_Position[_MAX_JKEY_CHANNEL_CNT];//����ͨ���Ķ�̬��������д��λ��
extern INT32U		g_Key_Channel_Tran_Write_Position[_MAX_JKEY_CHANNEL_CNT];//����ͨ����˲̬��������д��λ��
//
extern OS_EVENT				*g_EventStatic;
extern OS_EVENT				*g_EventDynamic;
//extern OS_EVENT				*g_EventTran;
extern OS_EVENT				*g_EventRTData;
//
//mainInterupt
extern INT32U				g_nDynamicRPMLevel[_MAX_JKEY_CHANNEL_CNT][_MAX_RPM_LEVEL];
extern INT32U				g_nIndexInGroupSequency;
extern INT32U				g_DynamicSampleStepDuration;

//Data_Limit_83				g_83DataLimit;//83ģ���һЩ���޲���
extern INT32S				g_nChannelArrivalSequenceTable[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ����12ͨ����ű�
extern INT32S				g_nArrivalDeviceNo[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ�������豸��
extern INT32S				g_nArrivalChannelNo[_MAX_SIGNAL_CHANNEL_CNT];//ͨ�����ݵ����������豸��ͨ����

extern INT32U		g_Signal_Channel_Dynamic_Write_Position[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ���Ķ�̬����д��λ��
extern INT32U		g_Signal_Channel_Tran_Write_Position[_MAX_SIGNAL_CHANNEL_CNT];   //����ͨ����˲̬����д��λ��
extern INT32U		g_Key_Channel_Dynamic_Write_Position[_MAX_JKEY_CHANNEL_CNT];     //����ͨ���Ķ�̬��������д��λ��
extern INT32U		g_Key_Channel_Tran_Write_Position[_MAX_JKEY_CHANNEL_CNT];        //����ͨ����˲̬��������д��λ��

extern INT32U		g_nArmDynamicChDataOffset[_MAX_SIGNAL_CHANNEL_CNT*2+_MAX_JKEY_CHANNEL_CNT*2];
extern INT32U*		g_nArmDynamicKeyDataOffset;
extern INT32U*		g_nArmTranChDataOffset;
extern INT32U*		g_nArmTranKeyDataOffset;
extern INT32S		g_nGroupCnt;

extern INT32U		g_nArmDynamicChDataSampleStep[_MAX_SIGNAL_CHANNEL_CNT*2];
extern INT32U*		g_nArmTranChDataSampleStep;
//

/*���ж�̬�ڴ�ķ�����ͷŶ��ڴ˺������У�����������*/
  CPU_STK	TaskStk[N_TASKS][TASK_STK_SIZE];

static INT32S InitRunTimeFlag(void);
static INT32S AllocMemery(INT32S nDynamiccnt,INT32S nTrancnt);
static void MapChannel(void);
static void ComposeValue(INT32U nReadSit,INT32U nDevice,INT32U nChannelCnt,Tst_Head_DCM_SigModuSampData_SYS* pOriginValue[],\
  Channel_Static_Value *pValue);

static void ComposeStaticData(void);
static void tff(void* pin);
static INT32S InitDataLimit(void);
static INT32S InitEvent(void);
static void ComposeStaticData(void);
static void ComposeDynamicData(void);
static void ComposeTranValue(void);

extern void static_transfer   (void * pParam);
extern void dynamic_transfer  (void * pParam);
extern void tran_transfer     (void * pParam);
extern void realvalue_transfer(void * pParam);
extern void realdata_transfer (void * pParam);
extern void InitInterfaceAddress(void);

INT32S  myRead12Param(void* p12);
INT32S  myInit83Param(void* p83in,void* p12in);
void myMapChannel(void);

OS_TCB Simu_Send_Data_tcb;

OS_TCB static_transfer_tcb;
OS_TCB dynamic_transfer_tcb;
OS_TCB tran_transfer_tcb;
OS_TCB realvalue_transfer_tcb;
OS_TCB realdata_transfer_tcb;

CPU_STK Simu_Send_Data_stk[TASK_STK_SIZE] @ "MYSECTION1";

CPU_STK static_transfer_stk   [TASK_STK_SIZE];
CPU_STK dynamic_transfer_stk  [TASK_STK_SIZE];
CPU_STK tran_transfer_stk     [TASK_STK_SIZE];
CPU_STK realvalue_transfer_stk[TASK_STK_SIZE];
CPU_STK realdata_transfer_stk [TASK_STK_SIZE];



void MapChannel()
{
  INT32S i,j,tmp1,tmp2;
  for(i = 0;i<g_nGroupCnt;i++)//����������˲̬����ͷ����,���жϱ���
  {
    tmp1 = i*_FPGA_GROUP_CHANNEL_CNT;//Ҫ��g_nGroupCnt����Ϊ2��ָ����/**/
    for(j=0;j<_FPGA_GROUP_CHANNEL_CNT;j++)
    {
      tmp2 = g_nChannelArrivalSequenceTable[tmp1+j];
      g_nArrivalDeviceNo[tmp1+j] = g_12Param.nChannelNumIn83[tmp2+tmp2];//�����豸��
      g_nArrivalChannelNo[tmp1+j] = g_12Param.nChannelNumIn83[tmp2+tmp2+1];//�����豸��ͨ����
    }
  }
  i=i;
}

static void ComposeValue(INT32U nReadSit,INT32U nDevice,INT32U nChannelCnt,Tst_Head_DCM_SigModuSampData_SYS* pOriginValue[],\
  Channel_Static_Value *pValue)
{
  INT32U nChannel;
  for(nChannel=0;nChannel<nChannelCnt;nDevice++)
  {
    pValue[nChannel].nTrigerTime=pOriginValue[nChannel]->ulnTmStamp_LastWaveData;
    pValue[nChannel].nChNo=pOriginValue[nChannel]->tunChInfo.tstVal.tunChNo.unVal_PT2060;
    pValue[nChannel].nAlertFlag=pOriginValue[nChannel]->tunStaticFlag1.tstVal.unFlag_Danger+\
      pOriginValue[nChannel]->tunStaticFlag1.tstVal.unFlag_Alert;
    pValue[nChannel].nRpmFlag=0;
    pValue[nChannel].nRPM=pOriginValue[nChannel]->ulnRPM;//	����ֵ	ת��	rpm	
    pValue[nChannel].nAllMeasureValue=pOriginValue[nChannel]->tunDirect.nVal;
    pValue[nChannel].nAllPhase=0;
    pValue[nChannel].n1xV=pOriginValue[nChannel]->un1XA;		
    pValue[nChannel].n1xP=pOriginValue[nChannel]->un1XP;
    pValue[nChannel].n2xV=pOriginValue[nChannel]->un2XA;
    pValue[nChannel].n2xP=pOriginValue[nChannel]->un2XP;
    pValue[nChannel].n05xV=0;
    pValue[nChannel].n05xP=0;
    pValue[nChannel].n05xV=pOriginValue[nChannel]->unNoT1X;
    pValue[nChannel].n05xP=0;
  }
}
void ComposeStaticData()
{
  INT32U nDevice,nCurReadSit;
  for(nDevice=0;nDevice<_MAX_JKEY_CHANNEL_CNT;nDevice++)
  {
    nCurReadSit=g_StaticTrigerEventArray[nDevice].ulnTrigerRead;
    if(nCurReadSit!=g_StaticTrigerEventArray[nDevice].ulnTrigerWrite)
    {
      ComposeValue(g_StaticTrigerEventArray[nDevice].ulnTrigerRead,nDevice,g_83param.Device[nDevice].nStaticChannelNum,\
        g_pStaticChannelOrigin[nDevice][nCurReadSit],(struct tagChValue *)g_tmpStaticData[nDevice]);
      g_StaticTrigerEventArray[nDevice].ulnTrigerRead++;
      if(_MAX_STATIC_ARRAY_LTH==g_StaticTrigerEventArray[nDevice].ulnTrigerRead)
        g_StaticTrigerEventArray[nDevice].ulnTrigerRead=0;
    }
  }
}
void ComposeDynamicData()
{
  INT32U nCurReadSit,nCurWriteSit,nDevice,nChannel;
  Tst_Head_DCM_SigModuSampData_SYS* pOriginValue;
  Channel_Dynamic_Value *pValue;
  for(nDevice=0;nDevice<_MAX_JKEY_CHANNEL_CNT;nDevice++)
  {
    nCurReadSit=g_DynamicTrigerEventArray[nDevice].ulnTrigerRead;
    nCurWriteSit=g_DynamicTrigerEventArray[nDevice].ulnTrigerWrite;
    if(nCurWriteSit!=nCurReadSit)
    {
      pOriginValue=(struct tagTst_Head_DCM_SigModuSampData_SYS *)(g_pDynamicChannelValue[nDevice][nCurReadSit]);
      pValue=(struct tagChValue *)g_tmpDynamicData[nDevice];
      ComposeValue(g_DynamicTrigerEventArray[nDevice].ulnTrigerRead,nDevice,g_83param.Device[nDevice].nDynamicChannelNum,\
        g_pDynamicChannelValue[nDevice][nCurReadSit],(struct tagChValue *)g_tmpDynamicData[nDevice]);
      g_DynamicTrigerEventArray[nDevice].ulnTrigerRead++;
      if(_MAX_DYNAMIC_ARRAY_LTH==g_DynamicTrigerEventArray[nDevice].ulnTrigerRead)
        g_DynamicTrigerEventArray[nDevice].ulnTrigerRead=0;
    }
  }
}

void ComposeTranValue()
{
  INT32U nCurReadSit,nCurWriteSit,nDevice,nChannel;
  Tst_Head_DCM_SigModuSampData_SYS* pOriginValue;
  Channel_Tran_Value *pValue;
  for(nDevice=0;nDevice<_MAX_JKEY_CHANNEL_CNT;nDevice++)
  {
    nCurReadSit=g_TranTrigerEventArray[nDevice].ulnTrigerRead;
    nCurWriteSit=g_TranTrigerEventArray[nDevice].ulnTrigerWrite;
    if(nCurWriteSit!=nCurReadSit)
    {
      pOriginValue=(struct tagTst_Head_DCM_SigModuSampData_SYS *)g_pTranChannelValue[nDevice][nCurReadSit];
      pValue=(struct tagChValue *)g_tmpTranData[nDevice];
      ComposeValue(g_TranTrigerEventArray[nDevice].ulnTrigerRead,nDevice,g_83param.Device[nDevice].nTranChannelNum,\
        (struct tagTst_Head_DCM_SigModuSampData_SYS**)g_pTranChannelValue[nDevice][nCurReadSit],(struct tagChValue *)g_tmpTranData[nDevice]);
      g_TranTrigerEventArray[nDevice].ulnTrigerRead++;
      if(_MAX_TRAN_ARRAY_LTH==g_TranTrigerEventArray[nDevice].ulnTrigerRead)
        g_TranTrigerEventArray[nDevice].ulnTrigerRead=0;
    }
  }
}

INT32S InitDataLimit()
{
  INT32S re = 0;
  g_83DataLimit.nMaxDyammicGroup = _MAX_DYNAMIC_ARRAY_LTH;
  g_83DataLimit.nMaxStaticGroup = _MAX_STATIC_ARRAY_LTH;
  g_83DataLimit.nMaxTranBufferTime = _MAX_TRAN_BUFFER_TIME;
  g_83DataLimit.nMaxTranGroup = 1;
  return re;
}

static INT32S InitEvent(void)
{
  INT32S re = 0;
  g_EventStatic  = OSSemCreate(0);
  g_EventDynamic = OSSemCreate(0);
  //g_EventTran    = OSQCreate(g_pTranEventArray, _MAX_TRAN_EVENT);
  g_EventRTData  = OSSemCreate(0);
  //OSSemCreate(&g_EventStatic, (CPU_CHAR *)"g_EventStatic", 0, &err);
  //OSSemCreate(&g_EventDynamic, (CPU_CHAR *)"g_EventDynamic", 0, &err);
  //OSQCreate(&g_EventTran, (CPU_CHAR *)"g_EventTran", _MAX_TRAN_EVENT, &err);
  //OSSemCreate(&g_EventRTData, (CPU_CHAR *)"g_EventRTData", 0, &err);

  return re;
}

INT32S InitRunTimeFlag()
{
  INT32S re = 0;
  g_RunTimeFlag.bRunning = 0;
  memset(g_RunTimeFlag.nAlertFlag, 0, sizeof(INT32S)*_MAX_JKEY_CHANNEL_CNT);
  memset(g_RunTimeFlag.nTranFlag,  0, sizeof(INT32S)*_MAX_JKEY_CHANNEL_CNT);
  memset(g_RunTimeFlag.nPickFlag,  0, sizeof(INT32S)*_MAX_JKEY_CHANNEL_CNT);
  
  g_RunTimeFlag.nInvaildDynamiccnt = 0;
  g_RunTimeFlag.nInvaildDynamicStart = 0;
  g_RunTimeFlag.nInvaildStaticcnt = 0;
  g_RunTimeFlag.nInvaildStaticStart = 0;
  g_RunTimeFlag.nMasterCommand = -1;
  g_RunTimeFlag.nOuterAlert = 0;
  g_RunTimeFlag.nTransitFlag = 0;
  g_RunTimeFlag.nVaildDynamiccnt = 0;
  g_RunTimeFlag.nVaildDynamicStart = 0;
  g_RunTimeFlag.nVaildStaticcnt = 0;
  g_RunTimeFlag.nVaildStaticStart = 0;
  g_RunTimeFlag.nValidDataType = -1;
  g_RunTimeFlag.nInvalidTranNo = 0;
  g_RunTimeFlag.nValidTranNo = 0;
  return re;
}

void tff(void* pin)
{
#if 0
  INT8S*  p8s = &(g_RTValue[1].nChValue);
  INT32S  sBuffer[1024];
  INT32S re = -1,i,j,k,l,m,cnt = 0,nRmvMrsPtcnt = 0,nRmvMrsPtChcnt = 0,delta;
  INT32S beat = (1000/g_n12UpFrequency)/(1000/OS_TICKS_PER_SEC)*2;
  
  do {
    cnt = GetCommand(sBuffer,256*sizeof(INT32S));//��������߶�ȡ����
    OSTimeDly(10);
  }while(1);
#endif
}

INT32S myRead12Param(void* p12)
{
  INT32S re=0,i;
  //INT32S nSize=sizeof(struct tagChData2060)+(_MAX_KEY_PT_CNT+_DEFAULT_DATA_LTH-1)*sizeof(INT32S);
  struct tag12Param* p=p12;
  //memset(p->nMrsPtDataSize, 0, sizeof(INT32S)*_MAX_SIGNAL_CHANNEL_CNT);
  memset(p->nMrsPtDataSize,  0, sizeof(INT32S)*_MAX_SIGNAL_CHANNEL_CNT);
  memset(p->nMrsPtDataSize0,  0, sizeof(INT32S)*_MAX_SIGNAL_CHANNEL_CNT);
  
  p->nDeviceNo=206;	//�豸���;
  p->nDeviceProperty=2060;	//�豸���ԣ�0 ���ݲɼ��� 6 ����������ģ�� 4 ����ģ�� 5 �̵���ģ�飻
  p->nDeviceTypeID=2060;	//�豸����ID;
  p->nDeviceID=20601;//�豸ID;
  
  p->nVersion=1<<8;//0.1��
  p->nRenewID=1;//>0
  p->nRenewTime=(2101<<16)+(7<<8)+(8);//2011-7-8
  
  p->nSignChannelNum=48;
  p->nMrsPtcnt=0;//���ò������
  for(i=0;i<_MAX_SIGNAL_CHANNEL_CNT;i++)
  {
    p->bWork[i]=1;//��������ͨ��
    p->nMrsPt[i]=-1;
    p->nMrsPtTable[i]=-1;
  }
  for(i=0;i<10;i++)
  {
    //p->SignalChannel[i].nDatalth=_DEFAULT_DATA_LTH;
    //p->nSignalChannelDataSize[i]=nSize;
    p->SignalChannel[i].nChNo=i;//ͨ�����	0~47	���޸�	ת��Ϊ�ۺź�ͨ����	
    p->SignalChannel[i].nChType=2;	//ͨ������	0~255	λ�ơ��ٶȡ����ٶȡ��¶ȡ����࣬������չ
    p->SignalChannel[i].fChFullScaleLow=-300;	//���̵�,����ԭʼ���ݵĹ�һ������
    p->SignalChannel[i].fChFullScaleHigh=300;	//���̸�,����ԭʼ���ݵĹ�һ������
    p->SignalChannel[i].fSensority=8;	//������������	����ֵ���ѹ�����������
    p->SignalChannel[i].fZeroVolt=0;	//����ѹ	����ֵ����Ӧ�ĵ�ѹ
    ///////	����������	��	p->SignalChannel[i]. 	0~2	�����֡�һ�λ��֡����λ���
    ///////	����ֵ����	��	p->SignalChannel[i]. 	0~3	pk��pk-pk��rms��avr
    ///����ֵ�����ʽ	��	p->SignalChannel[i]. 	0~2	opc��4~20ma�豸��modbus��ȡ����
    p->SignalChannel[i]. bAlert=1;//��������	��	p->SignalChannel[i]. 	0��1	���������
    p->SignalChannel[i]. bAlertLock=1;	//��������	��	p->SignalChannel[i]. 	0��1	����������
    p->SignalChannel[i]. fAlertLevelLow=-200;//����	��	p->SignalChannel[i].	��	alert����
    p->SignalChannel[i]. fDangerLevelLow=-250;//Σ��	��	p->SignalChannel[i].	��	danger����
    p->SignalChannel[i]. fAlertLevelHigh=200;//����	��	p->SignalChannel[i].	��	alert����
    p->SignalChannel[i]. fDangerLevelHigh=250;//Σ��	��	p->SignalChannel[i].	��	danger����
    p->SignalChannel[i]. nPickFrequency=20000;//�ɼ�Ƶ��	12ģ�齫ԭʼͨ�����ݳ����õ�ָ��Ƶ���µ�����
    p->SignalChannel[i].fFreqResponseLow=1;	//	Ƶ������	��	p->SignalChannel[i].	��	12ģ���Ѿ��涨�����޸�
    p->SignalChannel[i].fFreqResponsehigh=1000;	//Ƶ������	��	p->SignalChannel[i].	��	12ģ���Ѿ��涨�����޸�
    ////p->SignalChannel[i].2�˲���Χ	��	p->SignalChannel[i].	��	?
    p->SignalChannel[i].bSync=1;//ͬ����־	��	p->SignalChannel[i]. 	0��1	1��ͬ����0���첽
    p->SignalChannel[i].fFreqencyRes=1;//Ƶ��ֱ���,�첽ģʽ�´���Ƶ��ֱ��ʣ�ͬ��ģʽΪ��������
    p->SignalChannel[i].nJkeyNo=0;	//����ͨ����� 	��	p->SignalChannel[i]. 	0~��	
    
    p->SignalChannel[i].nInstallAngle=90*(i-i/2*2);//��������װ�Ƕ�,	0~360
    //���в����뼸��������أ�����ĳЩ��ͼʱ��Ҫ
    p->SignalChannel[i].bCircleFlag=0;//��Բ��־	��	p->SignalChannel[i]. 	0��1	0������Բ��1����Բ
    p->SignalChannel[i].fAxa=400;//��Բ����	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].fAxb=300;//��Բ����	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].byTrustDiretion=0;	//����̽ͷ�ƽ�����	��	p->SignalChannel[i]. 	0��1	0��������1��Զ��
    p->SignalChannel[i].fGapLow=600;	//��϶��	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].fGapHigh=100;	//��϶��	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].bEn=0;//����
    p->SignalChannel[i].nCalcMode=0;//����������,������λ��	��	INT32U 	0~2	�����֡�һ�λ��֡����λ���		
    p->SignalChannel[i].nOutputMode=1;
  }
  
  for(i=10;i<20;i++)
  {
    //		p->SignalChannel[i].nDatalth=_DEFAULT_DATA_LTH;
    //	p->nSignalChannelDataSize[i]=nSize;
    p->SignalChannel[i].nChNo=i;//ͨ�����	0~47	���޸�	ת��Ϊ�ۺź�ͨ����	
    p->SignalChannel[i].nChType=1;	//ͨ������	0~255	λ�ơ��ٶȡ����ٶȡ��¶ȡ����࣬������չ
    p->SignalChannel[i].fChFullScaleLow=0;	//���̵�,����ԭʼ���ݵĹ�һ������
    p->SignalChannel[i].fChFullScaleHigh=300;	//���̸�,����ԭʼ���ݵĹ�һ������
    p->SignalChannel[i].fSensority=8;	//������������	����ֵ���ѹ�����������
    p->SignalChannel[i].fZeroVolt=0;	//����ѹ	����ֵ����Ӧ�ĵ�ѹ
    ///////	����������	��	p->SignalChannel[i]. 	0~2	�����֡�һ�λ��֡����λ���
    ///////	����ֵ����	��	p->SignalChannel[i]. 	0~3	pk��pk-pk��rms��avr
    ///����ֵ�����ʽ	��	p->SignalChannel[i]. 	0~2	opc��4~20ma�豸��modbus��ȡ����
    p->SignalChannel[i]. bAlert=1;//��������	��	p->SignalChannel[i]. 	0��1	���������
    p->SignalChannel[i]. bAlertLock=1;	//��������	��	p->SignalChannel[i]. 	0��1	����������
    p->SignalChannel[i]. fAlertLevelLow=-200;//����	��	p->SignalChannel[i].	��	alert����
    p->SignalChannel[i]. fDangerLevelLow=-250;//Σ��	��	p->SignalChannel[i].	��	danger����
    p->SignalChannel[i]. fAlertLevelHigh=200;//����	��	p->SignalChannel[i].	��	alert����
    p->SignalChannel[i]. fDangerLevelHigh=250;//Σ��	��	p->SignalChannel[i].	��	danger����
    p->SignalChannel[i]. nPickFrequency=20000;//�ɼ�Ƶ��	12ģ�齫ԭʼͨ�����ݳ����õ�ָ��Ƶ���µ�����
    p->SignalChannel[i].fFreqResponseLow=1;	//	Ƶ������	��	p->SignalChannel[i].	��	12ģ���Ѿ��涨�����޸�
    p->SignalChannel[i].fFreqResponsehigh=1000;	//Ƶ������	��	p->SignalChannel[i].	��	12ģ���Ѿ��涨�����޸�
    ////p->SignalChannel[i].2�˲���Χ	��	p->SignalChannel[i].	��	?
    p->SignalChannel[i].bSync=1;//ͬ����־	��	p->SignalChannel[i]. 	0��1	1��ͬ����0���첽
    p->SignalChannel[i].fFreqencyRes=1;//Ƶ��ֱ���,�첽ģʽ�´���Ƶ��ֱ��ʣ�ͬ��ģʽΪ��������
    p->SignalChannel[i].nJkeyNo=1;	//����ͨ����� 	��	p->SignalChannel[i]. 	0~��	
    
    p->SignalChannel[i].nInstallAngle=90*(i-i/2*2);//��������װ�Ƕ�,	0~360
    //���в����뼸��������أ�����ĳЩ��ͼʱ��Ҫ
    p->SignalChannel[i].bCircleFlag=0;//��Բ��־	��	p->SignalChannel[i]. 	0��1	0������Բ��1����Բ
    p->SignalChannel[i].fAxa=400;//��Բ����	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].fAxb=300;//��Բ����	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].byTrustDiretion=0;	//����̽ͷ�ƽ�����	��	p->SignalChannel[i]. 	0��1	0��������1��Զ��
    p->SignalChannel[i].fGapLow=600;	//��϶��	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].fGapHigh=100;	//��϶��	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].bEn=0;//����
    p->SignalChannel[i].nCalcMode=0;//����������,������λ��	��	INT32U 	0~2	�����֡�һ�λ��֡����λ���		
    p->SignalChannel[i].nOutputMode=0;
  }
  
  for(i=20;i<30;i++)
  {
    //	p->SignalChannel[i].nDatalth=_DEFAULT_DATA_LTH;
    //	p->nSignalChannelDataSize[i]=nSize;
    p->SignalChannel[i].nChNo=i;//ͨ�����	0~47	���޸�	ת��Ϊ�ۺź�ͨ����	
    p->SignalChannel[i].nChType=0;	//ͨ������	0~255	λ�ơ��ٶȡ����ٶȡ��¶ȡ����࣬������չ
    p->SignalChannel[i].fChFullScaleLow=0;	//���̵�,����ԭʼ���ݵĹ�һ������
    p->SignalChannel[i].fChFullScaleHigh=12;	//���̸�,����ԭʼ���ݵĹ�һ������
    p->SignalChannel[i].fSensority=100;	//������������	����ֵ���ѹ�����������
    p->SignalChannel[i].fZeroVolt=0;	//����ѹ	����ֵ����Ӧ�ĵ�ѹ
    ///////	����������	��	p->SignalChannel[i]. 	0~2	�����֡�һ�λ��֡����λ���
    ///////	����ֵ����	��	p->SignalChannel[i]. 	0~3	pk��pk-pk��rms��avr
    ///����ֵ�����ʽ	��	p->SignalChannel[i]. 	0~2	opc��4~20ma�豸��modbus��ȡ����
    p->SignalChannel[i]. bAlert=1;//��������	��	p->SignalChannel[i]. 	0��1	���������
    p->SignalChannel[i]. bAlertLock=1;	//��������	��	p->SignalChannel[i]. 	0��1	����������
    p->SignalChannel[i]. fAlertLevelHigh=8;//����	��	p->SignalChannel[i].	��	alert����
    p->SignalChannel[i]. fDangerLevelHigh=10;//Σ��	��	p->SignalChannel[i].	��	danger����
    p->SignalChannel[i]. nPickFrequency=20000;//�ɼ�Ƶ��	12ģ�齫ԭʼͨ�����ݳ����õ�ָ��Ƶ���µ�����
    p->SignalChannel[i].fFreqResponseLow=1;	//	Ƶ������	��	p->SignalChannel[i].	��	12ģ���Ѿ��涨�����޸�
    p->SignalChannel[i].fFreqResponsehigh=1000;	//Ƶ������	��	p->SignalChannel[i].	��	12ģ���Ѿ��涨�����޸�
    ////p->SignalChannel[i].2�˲���Χ	��	p->SignalChannel[i].	��	?
    p->SignalChannel[i].bSync=1;//ͬ����־	��	p->SignalChannel[i]. 	0��1	1��ͬ����0���첽
    p->SignalChannel[i].fFreqencyRes=1;//Ƶ��ֱ���,�첽ģʽ�´���Ƶ��ֱ��ʣ�ͬ��ģʽΪ��������
    p->SignalChannel[i].nJkeyNo=2;	//����ͨ����� 	��	p->SignalChannel[i]. 	0~��	
    
    p->SignalChannel[i].nInstallAngle=90*(i-i/2*2);//��������װ�Ƕ�,	0~360
    //���в����뼸��������أ�����ĳЩ��ͼʱ��Ҫ
    p->SignalChannel[i].bCircleFlag=0;//��Բ��־	��	p->SignalChannel[i]. 	0��1	0������Բ��1����Բ
    p->SignalChannel[i].fAxa=400;//��Բ����	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].fAxb=300;//��Բ����	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].byTrustDiretion=0;	//����̽ͷ�ƽ�����	��	p->SignalChannel[i]. 	0��1	0��������1��Զ��
    p->SignalChannel[i].fGapLow=600;	//��϶��	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].fGapHigh=100;	//��϶��	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].bEn=0;//����
    p->SignalChannel[i].nCalcMode=0;//����������,������λ��	��	INT32U 	0~2	�����֡�һ�λ��֡����λ���		
    p->SignalChannel[i].nOutputMode=0;
  }
  for(i=30;i<40;i++)
  {
    //	p->SignalChannel[i].nDatalth=_DEFAULT_DATA_LTH;
    //	p->nSignalChannelDataSize[i]=nSize;
    p->SignalChannel[i].nChNo=i;//ͨ�����	0~47	���޸�	ת��Ϊ�ۺź�ͨ����	
    p->SignalChannel[i].nChType=0;	//ͨ������	0~255	λ�ơ��ٶȡ����ٶȡ��¶ȡ����࣬������չ
    p->SignalChannel[i].fChFullScaleLow=-300;	//���̵�,����ԭʼ���ݵĹ�һ������
    p->SignalChannel[i].fChFullScaleHigh=300;	//���̸�,����ԭʼ���ݵĹ�һ������
    p->SignalChannel[i].fSensority=8;	//������������	����ֵ���ѹ�����������
    p->SignalChannel[i].fZeroVolt=0;	//����ѹ	����ֵ����Ӧ�ĵ�ѹ
    ///////	����������	��	p->SignalChannel[i]. 	0~2	�����֡�һ�λ��֡����λ���
    ///////	����ֵ����	��	p->SignalChannel[i]. 	0~3	pk��pk-pk��rms��avr
    ///����ֵ�����ʽ	��	p->SignalChannel[i]. 	0~2	opc��4~20ma�豸��modbus��ȡ����
    p->SignalChannel[i]. bAlert=1;//��������	��	p->SignalChannel[i]. 	0��1	���������
    p->SignalChannel[i]. bAlertLock=1;	//��������	��	p->SignalChannel[i]. 	0��1	����������
    p->SignalChannel[i]. fAlertLevelLow=-200;//����	��	p->SignalChannel[i].	��	alert����
    p->SignalChannel[i]. fDangerLevelLow=-250;//Σ��	��	p->SignalChannel[i].	��	danger����
    p->SignalChannel[i]. fAlertLevelHigh=200;//����	��	p->SignalChannel[i].	��	alert����
    p->SignalChannel[i]. fDangerLevelHigh=250;//Σ��	��	p->SignalChannel[i].	��	danger����
    p->SignalChannel[i]. nPickFrequency=_12_CHANNEL_SAMPLE_RATE;//�ɼ�Ƶ��	12ģ�齫ԭʼͨ�����ݳ����õ�ָ��Ƶ���µ�����
    p->SignalChannel[i].fFreqResponseLow=1;	//	Ƶ������	��	p->SignalChannel[i].	��	12ģ���Ѿ��涨�����޸�
    p->SignalChannel[i].fFreqResponsehigh=1000;	//Ƶ������	��	p->SignalChannel[i].	��	12ģ���Ѿ��涨�����޸�
    ////p->SignalChannel[i].2�˲���Χ	��	p->SignalChannel[i].	��	?
    p->SignalChannel[i].bSync=1;//ͬ����־	��	p->SignalChannel[i]. 	0��1	1��ͬ����0���첽
    p->SignalChannel[i].fFreqencyRes=1;//Ƶ��ֱ���,�첽ģʽ�´���Ƶ��ֱ��ʣ�ͬ��ģʽΪ��������
    p->SignalChannel[i].nJkeyNo=3;	//����ͨ����� 	��	p->SignalChannel[i]. 	0~��	
    
    p->SignalChannel[i].nInstallAngle=90*(i-i/2*2);//��������װ�Ƕ�,	0~360
    //���в����뼸��������أ�����ĳЩ��ͼʱ��Ҫ
    p->SignalChannel[i].bCircleFlag=0;//��Բ��־	��	p->SignalChannel[i]. 	0��1	0������Բ��1����Բ
    p->SignalChannel[i].fAxa=400;//��Բ����	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].fAxb=300;//��Բ����	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].byTrustDiretion=0;	//����̽ͷ�ƽ�����	��	p->SignalChannel[i]. 	0��1	0��������1��Զ��
    p->SignalChannel[i].fGapLow=600;	//��϶��	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].fGapHigh=100;	//��϶��	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].bEn=0;//����
    p->SignalChannel[i].nCalcMode=0;//����������,������λ��	��	INT32U 	0~2	�����֡�һ�λ��֡����λ���		
    p->SignalChannel[i].nOutputMode=0;
  }
  
  for(i=40;i<p->nSignChannelNum;i++)//�¶�
  {
    //	p->SignalChannel[i].nDatalth=_DEFAULT_DATA_LTH;
    //	p->nSignalChannelDataSize[i]=nSize;
    p->SignalChannel[i].nChNo=i;//ͨ�����	0~47	���޸�	ת��Ϊ�ۺź�ͨ����	
    p->SignalChannel[i].nChType=2;	//ͨ������	0~255	λ�ơ��ٶȡ����ٶȡ��¶ȡ����࣬������չ
    p->SignalChannel[i].fChFullScaleLow=-300;	//���̵�,����ԭʼ���ݵĹ�һ������
    p->SignalChannel[i].fChFullScaleHigh=300;	//���̸�,����ԭʼ���ݵĹ�һ������
    p->SignalChannel[i].fSensority=8;	//������������	����ֵ���ѹ�����������
    p->SignalChannel[i].fZeroVolt=0;	//����ѹ	����ֵ����Ӧ�ĵ�ѹ
    ///////	����������	��	p->SignalChannel[i]. 	0~2	�����֡�һ�λ��֡����λ���
    ///////	����ֵ����	��	p->SignalChannel[i]. 	0~3	pk��pk-pk��rms��avr
    ///����ֵ�����ʽ	��	p->SignalChannel[i]. 	0~2	opc��4~20ma�豸��modbus��ȡ����
    p->SignalChannel[i]. bAlert=1;//��������	��	p->SignalChannel[i]. 	0��1	���������
    p->SignalChannel[i]. bAlertLock=1;	//��������	��	p->SignalChannel[i]. 	0��1	����������
    p->SignalChannel[i]. fAlertLevelLow=-200;//����	��	p->SignalChannel[i].	��	alert����
    p->SignalChannel[i]. fDangerLevelLow=-250;//Σ��	��	p->SignalChannel[i].	��	danger����
    p->SignalChannel[i]. fAlertLevelHigh=200;//����	��	p->SignalChannel[i].	��	alert����
    p->SignalChannel[i]. fDangerLevelHigh=250;//Σ��	��	p->SignalChannel[i].	��	danger����
    p->SignalChannel[i]. nPickFrequency=20000;//�ɼ�Ƶ��	12ģ�齫ԭʼͨ�����ݳ����õ�ָ��Ƶ���µ�����
    p->SignalChannel[i].fFreqResponseLow=1;	//	Ƶ������	��	p->SignalChannel[i].	��	12ģ���Ѿ��涨�����޸�
    p->SignalChannel[i].fFreqResponsehigh=1000;	//Ƶ������	��	p->SignalChannel[i].	��	12ģ���Ѿ��涨�����޸�
    ////p->SignalChannel[i].2�˲���Χ	��	p->SignalChannel[i].	��	?
    p->SignalChannel[i].bSync=1;//ͬ����־	��	p->SignalChannel[i]. 	0��1	1��ͬ����0���첽
    p->SignalChannel[i].fFreqencyRes=1;//Ƶ��ֱ���,�첽ģʽ�´���Ƶ��ֱ��ʣ�ͬ��ģʽΪ��������
    p->SignalChannel[i].nJkeyNo=0;	//����ͨ����� 	��	p->SignalChannel[i]. 	0~��	
    
    p->SignalChannel[i].nInstallAngle=90*(i-i/2*2);//��������װ�Ƕ�,	0~360
    //���в����뼸��������أ�����ĳЩ��ͼʱ��Ҫ
    p->SignalChannel[i].bCircleFlag=0;//��Բ��־	��	p->SignalChannel[i]. 	0��1	0������Բ��1����Բ
    p->SignalChannel[i].fAxa=400;//��Բ����	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].fAxb=300;//��Բ����	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].byTrustDiretion=0;	//����̽ͷ�ƽ�����	��	p->SignalChannel[i]. 	0��1	0��������1��Զ��
    p->SignalChannel[i].fGapLow=600;	//��϶��	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].fGapHigh=100;	//��϶��	��	p->SignalChannel[i].	��	
    p->SignalChannel[i].bEn=0;//����
    p->SignalChannel[i].nCalcMode=0;//����������,������λ��	��	INT32U 	0~2	�����֡�һ�λ��֡����λ���		
    p->SignalChannel[i].nOutputMode=1;
  }
  
  return re;
  
}

INT32S	myInit83Param(void* p83in,void* p12in)
{
  INT32S	re=0,i=0,j=0,k=0;
  struct tag83Param* p=p83in;
  struct tag83Param* p83=p83in;
  struct tag12Param* p12=p12in;
  
  
  //��ʼ��
  p->nDeviceNo=10000;	//�豸���;
  p->nDeviceProperty=2060;	//�豸���ԣ�0 ���ݲɼ��� 6 ����������ģ�� 4 ����ģ�� 5 �̵���ģ�飻
  p->nDeviceTypeID=2060;	//�豸����ID;
  p->nDeviceID=_BASIC_DEVIEID+1;//�豸ID;
  p->nDevicecnt=_MAX_JKEY_CHANNEL_CNT;
  p->nAllDynamicChDataSize=0;
  p->nAllTranChDataSize=0;
  for(i=0;i<p->nDevicecnt;i++)
  {
    for(j=0;j<1;j++)
    {
      p->Device[i].KeyChannel[j].bMaster=1;
      p->Device[i].KeyChannel[j].bTran=1;
      p->Device[i].KeyChannel[j].fRPMHigh=12000;
      p->Device[i].KeyChannel[j].fRPMLow=60;
      p->Device[i].KeyChannel[j].nChNo=_START_KEY_CH+i;
      p->Device[i].KeyChannel[j].nChType=255;
      p->Device[i].KeyChannel[j].nGearcnt=30;
      p->Device[i].KeyChannel[j].nTrigerDRPM=30;
      p->Device[i].KeyChannel[j].nTrigerTimeScope=5000;
      p->Device[i].KeyChannel[j].fWokeRPM=3000;
    }
  }
  
  for(i=0;i<p->nDevicecnt;i++)
  {
    p->Device[i].nPreTranTrigerTime=_DEFAULT_PRE_TRANTRIGER_TIMESPAN;
    p->Device[i].nDeviceNo=0;//�豸���;
    p->Device[i].nDeviceProperty=p->nDeviceProperty+i;//�豸���ԣ�0 ���ݲɼ��� 6 ����������ģ�� 4 ����ģ�� 5 �̵���ģ�飻
    p->Device[i].nDeviceTypeID=p->nDeviceTypeID+i;//�豸����ID;
    p->Device[i].nDeviceID=p->nDeviceID+i;//�豸ID;
    
    p->Device[i].nKeyChno=i;
    p->Device[i].bAlertTrigerStatic=1;//����������̬��1������������0����Ч
    p->Device[i].nStaticAutoPickTime=_DEFAULT_STATIC_AUTO_PICK_FREQUENCY	;//�Զ���̬�ɼ�ʱ����������0��Ч
    p->Device[i].bAlertTrigerDynamic=1;//����������̬��1������������0����Ч
    p->Device[i].nDynamicAutoPickTime=_DEFAULT_DYNAMIC_AUTO_PICK_FREQUENCY	;//�Զ���̬�ɼ�ʱ����������0��Ч
    p->Device[i].nPickTimeScopePre=_DEFAULT_PRE_TRANTRIGER_TIMESPAN;//����ǰ����ʱ�䳤�ȣ���λΪ����
    
    p->Device[i].nSampleRPMInterval=5;//����ת�ټ������λΪ1ת/��
    p->Device[i].nSampleTimeInterval=100;//��֪��
    p->Device[i].nPickTimeIntervalPre=_DEFAULT_PRE_TRANTRIGER_TIME_GAP;//��ʱ����Ĭ��ÿһ���ӳ���10�Σ���1��1
    
    p->Device[i].nTranPreTrigerGroupcnt=p->Device[i].nPickTimeScopePre/p->Device[i].nPickTimeIntervalPre;
    
    p->Device[i].nMaxTranSampleTime=_MAX_TRAN_TRIGER_TIMESPAN;//���˲̬�ɼ�ʱ��,��ת�ٳ�ȡʱ����ʱ��ֹͣ�ɼ�
    p->Device[i].nMaxTranSampleCnt=_MAX_TRAN_GROUP;//���˲̬�ɼ�������ĿǰΪ200
    
    p->Device[i].nSignChannelNum=0;//�豸�ź�ͨ��������
    
    p->Device[i].nAllDynamicChDataSize=0;
    p->Device[i].nAllTranChDataSize=0;
    
    p83->Device[i].nSignal_Channel_Dynamic_Data_Lth=16384;
    p83->Device[i].nSignal_Channel_Tran_Data_Lth=2048;
    p83->Device[i].nTranSampleMode=0;
  }
  for(i=0;i<_MAX_JKEY_CHANNEL_CNT;i++)//
  {
    p->Device[i].nFrequencyLow =_DEFAULT_FREQUENCY_RESPONSE_LOW;//��λ���趨��Ƶ������
    p->Device[i].nFrequencyHigh =_DEFAULT_FREQUENCY_RESPONSE_HIGH;
    p->Device[i].nDynamicSampleMode =0;//0���첽�ɼ���1��ͬ���ɼ�
    p->Device[i].nDynamicSynDataCntPerCycle =256;
    p->Device[i].nTranSynSampleMode =0;//0���첽�ɼ���1��ͬ���ɼ�
    p->Device[i].nTranSynDataCntPerCycle =256;
    for(j=0;j<_MAX_SIGNAL_CHANNEL_CNT;j++)
    {
      p->Device[i].nMrsPtNo[j]=(i*10+j)/2;
      p->Device[i].nChIndexMrsPt[j]=j%2;
      p->Device[i].nStaticMask[j]=1;
      p->Device[i].nDynamicMask[j]=1;
      p->Device[i].nTranMask[j]=1;
    }
    if(i==0)
    {
      p->Device[i].nMrsPtNo[10]=20;
      p->Device[i].nMrsPtNo[11]=20;
      p->Device[i].nMrsPtNo[12]=21;
      p->Device[i].nMrsPtNo[13]=21;
      p->Device[i].nMrsPtNo[14]=22;
      p->Device[i].nMrsPtNo[15]=22;
      p->Device[i].nMrsPtNo[16]=23;
      p->Device[i].nMrsPtNo[17]=23;
    }
  }
  
  //��12ģ�������ʼ��83ģ�����,����ͨ��ӳ���
  
  p83->nDevicecnt=_MAX_JKEY_CHANNEL_CNT;//�����豸������ͬ�����������
  
  //	p83->nAllChDataSize=0;
  for(i=0;i<p12->nSignChannelNum;i++)//ӳ��12ͨ���ŵ�83ͨ����,������12ͨ������ʹ�ܱ�־
  {
    for(j=0;j<p83->nDevicecnt;j++)
    {
      p83->Device[j].nKeyChno=j;//�̶��������޸�
      
      if(p83->Device[j].nKeyChno==p12->SignalChannel[i].nJkeyNo)
      {
        if(p12->bWork[i])//��ͨ��������,ӳ�䵽83ͨ����
        {
          p83->Device[j].nChannelNumIn12[p83->Device[j].nSignChannelNum]=i;//����ͨ����12�е����
          p12->nChannelNumIn83[i*2]=j;
          p12->nChannelNumIn83[i*2+1]=p83->Device[j].nSignChannelNum;
          p83->Device[j].nChannelMask[p83->Device[j].nSignChannelNum]=1;//1,����0��ֹ
          
          p83->nSignChannelNum++;//����ͨ������1
          p83->Device[j].nSignChannelNum++;//�����豸��ͨ������1
          if(p83->Device[j].bAlertTrigerStatic)
          {
            p83->nStaticChannelNum++;//83�ľ�̬ͨ����1
            p83->Device[j].nStaticChannelNum++;//�����豸�ľ�̬ͨ������1
          }
          if(p83->Device[j].bAlertTrigerDynamic&&\
            p83->Device[j].nSignal_Channel_Dynamic_Data_Lth)
          {
            p83->nDynamicChannelNum++;//83�Ķ�̬ͨ����1
            p83->Device[j].nDynamicChannelNum++;//�����豸�Ķ�̬ͨ������1
          }
          if(p83->Device[j].KeyChannel[0].nTrigerDRPM&&
             p83->Device[j].nSignal_Channel_Tran_Data_Lth)//˲̬
          {
            p83->nTranChannelNum++;
            p83->Device[j].nTranChannelNum++;
          }
        }
        break;
      }
    }
  }
  //����Ƶ�췶Χȷ���������ݴ�С������Ƶ�ʡ��ɼ�ʱ��Ȳ��������ɵ�ַ�����õĸ�������	
  p83->nAllChValueSize=0;
  p83->nAllChStaticValueSize=0;//��ʼ�����ֲ���ֵ�ߴ�
  p83->nAllChDynamicValueSize=0;
  p83->nAllChTranValueSize=0;
  for(i=0;i<p83->nDevicecnt;i++)
  {
    p83->Device[i].nDeviceNo=i;
    p83->Device[i].nMinPreTranTickcnt=
      p83->Device[i].nPickTimeIntervalPre/_12_DATA_UPLOAD_TIME_GAP*p83->Device[i].nTranPreTrigerGroupcnt;
    p83->Device[i].nDynamicChannelDataSampleStep =_DEFAULT_DYNAMIC_ASYN_DATA_SAMPLE_STEP;
    p83->Device[i].nPickDynamicFrequency =\
      _12_CHANNEL_SAMPLE_RATE/p83->Device[i].nDynamicChannelDataSampleStep;
    p83->Device[i].nDynamicPickTime=\
      1000*p83->Device[i].nSignal_Channel_Dynamic_Data_Lth/p83->Device[i].nPickDynamicFrequency;			
    p83->Device[i].nChDynamicBlockSize=_12_CHANNEL_SAMPLE_RATE*_12_DATA_UPLOAD_TIME_GAP/1000;
    p83->Device[i].nChDynamicBlockSize=\
      p83->Device[i].nChDynamicBlockSize/p83->Device[i].nDynamicChannelDataSampleStep+1;
    p83->Device[i].nChDynamicBlockSize=\
      ((p83->Device[i].nChDynamicBlockSize-1)/_RAMDISK_CLUSTER_LTH+1)*_RAMDISK_CLUSTER_LTH;
    p83->Device[i].nSignal_Channel_Dynamic_Data_Lth =\
      (_DEFAULT_DYNAMIC_DATA_LTH+1)/p83->Device[i].nChDynamicBlockSize *p83->Device[i].nChDynamicBlockSize ;
    p83->Device[i].nChDynamicDataBlockCnt =\
      p83->Device[i].nSignal_Channel_Dynamic_Data_Lth /p83->Device[i].nChDynamicBlockSize ;
    p83->Device[i].nChDynamicBlockSize*=sizeof(INT32S);
    p83->Device[i].nChDynamicDataSize =p83->Device[i].nSignal_Channel_Dynamic_Data_Lth *sizeof(INT32S);
    
    p83->Device[i].nTranChannelDataSampleStep =_DEFAULT_TRAN_DATA_SAMPLE_STEP;
    p83->Device[i].nPickTranFrequency =_12_CHANNEL_SAMPLE_RATE/p83->Device[i].nTranChannelDataSampleStep ;
    p83->Device[i].nTranPickTime =100;
    p83->Device[i].nChTranBlockSize =_12_CHANNEL_SAMPLE_RATE*_12_DATA_UPLOAD_TIME_GAP/1000;
    p83->Device[i].nChTranBlockSize =\
      p83->Device[i].nChTranBlockSize / p83->Device[i].nTranChannelDataSampleStep+1;
    p83->Device[i].nChTranBlockSize =\
      ((p83->Device[i].nChTranBlockSize-1) /_RAMDISK_CLUSTER_LTH+1)*_RAMDISK_CLUSTER_LTH;
    p83->Device[i].nSignal_Channel_Tran_Data_Lth =\
      (_DEFAULT_TRAN_DATA_LTH+1)/p83->Device[i].nChTranBlockSize *p83->Device[i].nChTranBlockSize ;
    p83->Device[i].nChTranBlockSize*=sizeof(INT32S);
    p83->Device[i].nChTranDataSize =p83->Device[i].nSignal_Channel_Tran_Data_Lth *sizeof(INT32S);
    
    p83->Device[i].nChValueSize=sizeof(Tst_Head_DCM_SigModuSampData_SYS);//����ͨ���Ĳ���ֵ�ߴ�
    for(j=0;j<p83->Device[i].nSignChannelNum;j++)
    {
      p12->nChIndexMrsPt[p83->Device[i].nChannelNumIn12[j]]=p83->Device[i].nChIndexMrsPt[j];
      p12->nChDeviceChNo[p83->Device[i].nChannelNumIn12[j]][0]=i;
      p12->nChDeviceChNo[p83->Device[i].nChannelNumIn12[j]][1]=j;	
      p12->nMrsPt[p83->Device[i].nChannelNumIn12[j]]=p83->Device[i].nMrsPtNo[j];//����ͨ���������
      
      p83->Device[i].nAllChValueSize+=p83->Device[i].nChValueSize;//�����豸�Ĳ���ֵ�ߴ�	
      p83->Device[i].nChTranDataBlockCnt =\
        p83->Device[i].nSignal_Channel_Tran_Data_Lth /p83->Device[i].nChTranBlockSize ;	
      
      p83->Device[i].nAllDynamicChDataSize+=p83->Device[i].nChDynamicDataSize;
      p83->Device[i].nAllTranChDataSize+=p83->Device[i].nChTranDataSize;
      //	p12->nSignalChannelDynamicDataSize[p83->Device[i].nChannelNumIn12[j]]=\
      p83->Device[i].nSignal_Channel_Dynamic_Data_Lth[j];
      //	p12->nSignalChannelTranDataSize[p83->Device[i].nChannelNumIn12[j]]=\
      p83->Device[i].nSignal_Channel_Tran_Data_Lth[j];
    }
    //���������豸���������ݲ���ֵ�ߴ�
    p83->Device[i].nAllChStaticValueSize=p83->Device[i].nStaticChannelNum*sizeof(Tst_Head_DCM_SigModuSampData_SYS);
    p83->Device[i].nAllChDynamicValueSize=p83->Device[i].nDynamicChannelNum*sizeof(Tst_Head_DCM_SigModuSampData_SYS);
    p83->Device[i].nAllChTranValueSize=p83->Device[i].nTranChannelNum*sizeof(Tst_Head_DCM_SigModuSampData_SYS);
    p83->Device[i].nAllChValueSize=p83->Device[i].nAllChStaticValueSize;
    
    //����83���������ݲ���ֵ�ߴ�
    p83->nAllChStaticValueSize+=p83->Device[i].nAllChStaticValueSize;
    p83->nAllChDynamicValueSize+=p83->Device[i].nAllChDynamicValueSize;
    p83->nAllChTranValueSize+=p83->Device[i].nAllChTranValueSize;
    p83->nAllChValueSize+=p83->Device[i].nAllChValueSize;//83ģ��Ĳ���ֵ�ߴ�
    
    //		p83->Device[i].nAllChDataSize=p83->Device[i].nAllDynamicChDataSize+p83->Device[i].nAllTranChDataSize;
    //		p83->nAllChDataSize+=p83->Device[i].nAllChDataSize;
    p83->nAllDynamicChDataSize+=p83->Device[i].nAllDynamicChDataSize;
    p83->nAllTranChDataSize+=p83->Device[i].nAllTranChDataSize;
  }
  
  /*����12ģ��ͨ��������������صĲ���*/
  p12->nMrsPtcnt=0;//���ò������
  
  for(i=0;i<p->nSignChannelNum;i++)//�������豸����ͨ����
  {
    re=0;
    p12->nMrsPt[i]=i/2;
    p12->nChIndexMrsPt[i]=i-i/2*2;
    for(j=0;j<p12->nMrsPtcnt;j++)//�ò���Ƿ��Ѿ�����֪�б���
    {
      if(p12->nMrsPt[i]==j)//������
      {
        re=1;//��֪��㣬��������Ӱ�䵽12ͨ����������б���
        p12->nMrsPtIndex[i]=j;
        if(p12->nChIndexMrsPt[i]==0)
          p12->nMrsPtDataSize0[j]=_DEFAULT_RM_DATA_LTH+sizeof(struct tagChValue);
        p12->nMrsPtDataSize[j]+=_DEFAULT_RM_DATA_LTH+sizeof(struct tagChValue);
        p12->nMrsPtChcnt[j]++;//���ͨ�����ӣ�
        break;
      }
    }
    if(re==0)//�²��
    {
      p12->nMrsPtTable[p12->nMrsPtcnt]=p12->nMrsPt[i];//�����뵽����б���
      if(p12->nChIndexMrsPt[i]==0)//��0��ͨ��
        p12->nMrsPtDataSize0[p12->nMrsPtcnt]=_DEFAULT_RM_DATA_LTH+sizeof(struct tagChValue);
      p12->nMrsPtIndex[i]=p12->nMrsPtcnt;//����Ӱ�䵽12ͨ����������б��У��������1
      //������ݳߴ��ʼ��Ϊͨ�����ݳߴ�
      p12->nMrsPtDataSize[p12->nMrsPtcnt]=_DEFAULT_RM_DATA_LTH+sizeof(struct tagChValue);
      p12->nMrsPtChcnt[p12->nMrsPtcnt]=1;//���ͨ������ʼ��Ϊ1
      p12->nMrsPtcnt++;
    }
  }
  return re;
}

extern Key_Speed_Calt	g_DynamicSpeedCalt[];
INT32S	myRenewParam(void* p83in,void* p12in);
INT32S  myCaltChannelParam(INT32S nKey,FP32 fFrequencyLow,FP32 fFrequencyHigh,INT32S nSampleSynMode,INT32S nSynSampleLth,
                         INT32S *pnPickDynamicFrequency,INT32S* pnDynamicChannelDataSampleStep,
                         INT32S* pnSignal_Channel_Dynamic_Data_Lth,INT32S* pnChDynamicBlockSize,
                         INT32S* pnChDynamicDataBlockCnt,INT32S* pnDynamicPickTime,INT32S* pnChDynamicDataSize,
                         INT32S* pnDynamicSynRPM);
INT32S  myCaltChannelParam(INT32S nKey,FP32 fFrequencyLow,FP32 fFrequencyHigh,INT32S nSampleSynMode,INT32S nSynSampleLth,
                         INT32S *pnPickDynamicFrequency,INT32S* pnDynamicChannelDataSampleStep,
                         INT32S* pnSignal_Channel_Dynamic_Data_Lth,INT32S* pnChDynamicBlockSize,
                         INT32S* pnChDynamicDataBlockCnt,INT32S* pnDynamicPickTime,INT32S* pnChDynamicDataSize,
                         INT32S* pnDynamicSynRPM)
{
  INT32S re=0;
  //	nSampleSynMode=1;
  if(nSampleSynMode==0)//�첽ģʽ
  {
    re=fFrequencyHigh*_DEFAULT_HIGH_FREQUENCY_COF/1000;
    pnDynamicChannelDataSampleStep[0] =re<_12_CHANNEL_SAMPLE_RATE?_12_CHANNEL_SAMPLE_RATE/re:1;
    if(pnDynamicChannelDataSampleStep[0] >_MAX_DYNAMIC_CHANNEL_DATA_SAMPLE_STEP)
      pnDynamicChannelDataSampleStep[0] =_MAX_DYNAMIC_CHANNEL_DATA_SAMPLE_STEP;
    else if(pnDynamicChannelDataSampleStep[0] <_MIN_DYNAMIC_CHANNEL_DATA_SAMPLE_STEP)
    {
      pnDynamicChannelDataSampleStep[0] =_MIN_DYNAMIC_CHANNEL_DATA_SAMPLE_STEP;
    }
    pnPickDynamicFrequency[0] =_12_CHANNEL_SAMPLE_RATE/pnDynamicChannelDataSampleStep[0];
    pnChDynamicBlockSize[0] =_12_CHANNEL_SAMPLE_RATE*_12_DATA_UPLOAD_TIME_GAP/1000;//ÿ�β�����������
    pnChDynamicBlockSize[0] =\
      pnChDynamicBlockSize[0]/pnDynamicChannelDataSampleStep[0]+1;//���γ�ȡ����
    pnChDynamicBlockSize[0] =\
      (pnChDynamicBlockSize[0] /_RAMDISK_CLUSTER_LTH+1)*_RAMDISK_CLUSTER_LTH;//����������СѰַ��Բ��
    pnDynamicPickTime[0] = 1000000/fFrequencyLow;//�ɼ�ʱ��
    if(pnDynamicPickTime[0]>_DYNAMIC_ASYNCHRONOUS_MAX_PICK_TIME)
      pnDynamicPickTime[0]=_DYNAMIC_ASYNCHRONOUS_MAX_PICK_TIME;
    else if(pnDynamicPickTime[0]<_DYNAMIC_ASYNCHRONOUS_MIN_PICK_TIME)
      pnDynamicPickTime[0]=_DYNAMIC_ASYNCHRONOUS_MIN_PICK_TIME;
    pnChDynamicDataBlockCnt[0] =(pnDynamicPickTime[0]-_12_DATA_UPLOAD_TIME_GAP/2)/_12_DATA_UPLOAD_TIME_GAP+1;
    pnSignal_Channel_Dynamic_Data_Lth[0] = pnChDynamicBlockSize[0] * pnChDynamicDataBlockCnt[0];//��̬����Բ��				
  }
  else//ͬ��ģʽ
  {
    pnDynamicChannelDataSampleStep[0]=_DEFAULT_DYNAMIC_SYN_DATA_SAMPLE_STEP;//ͬ��Ĭ�ϳ�������
    pnPickDynamicFrequency[0] = _12_CHANNEL_SAMPLE_RATE/pnDynamicChannelDataSampleStep[0];//ͬ��Ĭ�ϲɼ�Ƶ��
    
    pnChDynamicBlockSize[0] =_12_CHANNEL_SAMPLE_RATE*_12_DATA_UPLOAD_TIME_GAP/1000;//ÿ�β�����������
    pnChDynamicBlockSize[0] =\
      pnChDynamicBlockSize[0]/pnDynamicChannelDataSampleStep[0];//���γ�ȡ����
    pnChDynamicBlockSize[0] =\
      (pnChDynamicBlockSize[0] /_RAMDISK_CLUSTER_LTH+1)*_RAMDISK_CLUSTER_LTH;//����������СѰַ��Բ��
    
    pnDynamicPickTime[0] = 1000000/fFrequencyLow;//�ɼ�ʱ��
    if(pnDynamicPickTime[0]>_DYNAMIC_SYNCHRONOUS_MAX_PICK_TIME)
      pnDynamicPickTime[0]=_DYNAMIC_SYNCHRONOUS_MAX_PICK_TIME;
    else if(pnDynamicPickTime[0]<_DYNAMIC_SYNCHRONOUS_MIN_PICK_TIME)
      pnDynamicPickTime[0]=_DYNAMIC_SYNCHRONOUS_MIN_PICK_TIME;
    pnChDynamicDataBlockCnt[0] = pnDynamicPickTime[0]/_12_DATA_UPLOAD_TIME_GAP;//���ݿ���
    pnDynamicSynRPM[0]=(1000/_12_DATA_UPLOAD_TIME_GAP)*60*_MAX_DYNAMIC_SYN_SAMPLE_LTH/nSynSampleLth;//���ޱ�Ƶת��
    pnSignal_Channel_Dynamic_Data_Lth[0] = pnChDynamicDataBlockCnt[0]*pnChDynamicBlockSize[0];//
    for(re=0;re<_MAX_RPM_LEVEL;re++)
    {
      g_DynamicSpeedCalt[nKey].nRPMLevel[re]=pnDynamicSynRPM[0]*(re+1);
    }
  }
  pnChDynamicBlockSize[0]*=sizeof(INT32S);
  pnChDynamicDataSize[0] =pnSignal_Channel_Dynamic_Data_Lth[0] * sizeof(INT32S);
  return re;
}

INT32S	myRenewParam(void* p83in,void* p12in)
{
  INT32S	re=0,i=0,j=0,k=0,temp;
  struct tag83Param* p83=p83in;
  struct tag12Param* p12=p12in;
  p83->nAllChValueSize=0;
  p83->nAllChStaticValueSize=0;//��ʼ�����ֲ���ֵ�ߴ�
  p83->nAllChDynamicValueSize=0;
  p83->nAllChTranValueSize=0;
  for(i=0;i<p83->nDevicecnt;i++)
  {
    p83->Device[i].nDeviceNo=i;
    //		p83->Device[i].nAllChDataSize=0;
    p83->Device[i].nAllDynamicChDataSize=0;
    p83->Device[i].nAllTranChDataSize=0;////˲̬ģ�͸���ɼ�Ƶ�ʱ仯ʱ��Ҫ�򿪣�Ŀǰ��ģ���޷�ʵ��˲̬ͬ���ɼ�����
    p83->Device[i].nMinPreTranTickcnt=
      p83->Device[i].nPickTimeIntervalPre/_12_DATA_UPLOAD_TIME_GAP*p83->Device[i].nTranPreTrigerGroupcnt;
    re=myCaltChannelParam(i,p83->Device[i].nFrequencyLow,p83->Device[i].nFrequencyHigh,
                        p83->Device[i].nDynamicSampleMode,p83->Device[i].nDynamicSynDataCntPerCycle,
                        &p83->Device[i].nPickDynamicFrequency,&p83->Device[i].nDynamicChannelDataSampleStep,
                        &p83->Device[i].nSignal_Channel_Dynamic_Data_Lth,&p83->Device[i].nChDynamicBlockSize,
                        &p83->Device[i].nChDynamicDataBlockCnt,&p83->Device[i].nDynamicPickTime,
                        &p83->Device[i].nChDynamicDataSize,&p83->Device[i].nDynamicSynRPM);
    //Ϊ˲̬ͬ���ɼ�Ԥ�����㷨,���Ե�������Ҫ��Ƶ�ʵ�����ȡ����
    
    temp=p83->Device[i].nFrequencyHigh*_DEFAULT_HIGH_FREQUENCY_COF;
    p83->Device[i].nTranChannelDataSampleStep=_DEFAULT_TRAN_DATA_SAMPLE_STEP;
    p83->Device[i].nTranPickTime=100;
    p83->Device[i].nSignal_Channel_Tran_Data_Lth=_DEFAULT_TRAN_DATA_LTH;
#ifdef	_ADVANCED_TRAN_MODE //����Ƶ�췶Χ�����ȡ���Ӻ����ݳ��ȣ�Ϊͬ���ɼ�ģʽԤ��
    p83->Device[i].nTranChannelDataSampleStep=temp<_12_CHANNEL_SAMPLE_RATE?_12_CHANNEL_SAMPLE_RATE/temp:1;
    if(p83->Device[i].nTranChannelDataSampleStep>_MAX_TRAN_CHANNEL_DATA_SAMPLE_STEP)
      p83->Device[i].nTranChannelDataSampleStep=_MAX_TRAN_CHANNEL_DATA_SAMPLE_STEP;
    p83->Device[i].nSignal_Channel_Tran_Data_Lth=
      p83->Device[i].nPickTranFrequency*_DEFAULT_TRAN_DATA_MIN_CYCLE*100/p83->Device[i].nFrequencyLow;			
    p83->Device[i].nTranPickTime[i]=
      1000*p83->Device[i].nSignal_Channel_Tran_Data_Lth/p83->Device[i].nPickTranFrequency;
#endif
    p83->Device[i].nPickTranFrequency=_12_CHANNEL_SAMPLE_RATE/p83->Device[i].nTranChannelDataSampleStep;		
    
    p83->Device[i].nChTranBlockSize=_12_CHANNEL_SAMPLE_RATE*_12_DATA_UPLOAD_TIME_GAP/1000;
    p83->Device[i].nChTranBlockSize=\
      p83->Device[i].nChTranBlockSize/p83->Device[i].nTranChannelDataSampleStep+1;
    p83->Device[i].nChTranBlockSize=\
      (p83->Device[i].nChTranBlockSize/_RAMDISK_CLUSTER_LTH+1)*_RAMDISK_CLUSTER_LTH;
    p83->Device[i].nSignal_Channel_Tran_Data_Lth=\
      _DEFAULT_TRAN_DATA_LTH/p83->Device[i].nChTranBlockSize*p83->Device[i].nChTranBlockSize;
    
    if(p83->Device[i].nSignal_Channel_Tran_Data_Lth<p83->Device[i].nChTranBlockSize)
      p83->Device[i].nSignal_Channel_Tran_Data_Lth=p83->Device[i].nChTranBlockSize;
    p83->Device[i].nChTranDataBlockCnt=\
      p83->Device[i].nSignal_Channel_Tran_Data_Lth/p83->Device[i].nChTranBlockSize;
    p83->Device[i].nChTranBlockSize*=sizeof(INT32S);
    p83->Device[i].nChTranDataSize=p83->Device[i].nSignal_Channel_Tran_Data_Lth*sizeof(INT32S);	
    
    p83->Device[i].nRealPickTimeIntervalPre=p83->Device[i].nPickTimeIntervalPre/(1000/g_n12UpFrequency);
    p83->Device[i].nRealSampleTimeInterval=p83->Device[i].nSampleTimeInterval/(1000/g_n12UpFrequency);
    
    for(j=0;j<p83->Device[i].nSignChannelNum;j++)
    {
      p12->nChIndexMrsPt[p83->Device[i].nChannelNumIn12[j]]=p83->Device[i].nChIndexMrsPt[j];
      p12->nChDeviceChNo[p83->Device[i].nChannelNumIn12[j]][0]=i;
      p12->nChDeviceChNo[p83->Device[i].nChannelNumIn12[j]][1]=j;
      p12->nMrsPt[p83->Device[i].nChannelNumIn12[j]]=p83->Device[i].nMrsPtNo[j];//����ͨ���������
      
      p83->Device[i].nChValueSize=sizeof(Tst_Head_DCM_SigModuSampData_SYS);//����ͨ���Ĳ���ֵ�ߴ�
      p83->Device[i].nAllChValueSize+=p83->Device[i].nChValueSize;//�����豸�Ĳ���ֵ�ߴ�	
      
      p83->Device[i].nAllDynamicChDataSize+=p83->Device[i].nChDynamicDataSize;
      p83->Device[i].nAllTranChDataSize+=p83->Device[i].nChTranDataSize;
      //���12����ͨ������
      //	p12->nSignalChannelDynamicDataSize[p83->Device[i].nChannelNumIn12[j]]=\
      p83->Device[i].nSignal_Channel_Dynamic_Data_Lth[j];
      //	p12->nSignalChannelTranDataSize[p83->Device[i].nChannelNumIn12[j]]=\
      p83->Device[i].nSignal_Channel_Tran_Data_Lth[j];
    }
    //���������豸���������ݲ���ֵ�ߴ�
    p83->Device[i].nAllChStaticValueSize=p83->Device[i].nStaticChannelNum*sizeof(Tst_Head_DCM_SigModuSampData_SYS);
    p83->Device[i].nAllChDynamicValueSize=p83->Device[i].nDynamicChannelNum*sizeof(Tst_Head_DCM_SigModuSampData_SYS);
    p83->Device[i].nAllChTranValueSize=p83->Device[i].nTranChannelNum*sizeof(Tst_Head_DCM_SigModuSampData_SYS);
    p83->Device[i].nAllChValueSize=p83->Device[i].nAllChStaticValueSize;
    
    //����83���������ݲ���ֵ�ߴ�
    p83->nAllChStaticValueSize+=p83->Device[i].nAllChStaticValueSize;
    p83->nAllChDynamicValueSize+=p83->Device[i].nAllChDynamicValueSize;
    p83->nAllChTranValueSize+=p83->Device[i].nAllChTranValueSize;
    p83->nAllChValueSize+=p83->Device[i].nAllChValueSize;//83ģ��Ĳ���ֵ�ߴ�
    
    //	p83->Device[i].nAllChDataSize=p83->Device[i].nAllDynamicChDataSize+p83->Device[i].nAllTranChDataSize;
    //	p83->nAllChDataSize+=p83->Device[i].nAllChDataSize;
    p83->nAllDynamicChDataSize+=p83->Device[i].nAllDynamicChDataSize;
    p83->nAllTranChDataSize+=p83->Device[i].nAllTranChDataSize;
    
  }
  
  /*12ģ����-ͨ��ӳ����صĲ�������*/
  p12->nMrsPtcnt=0;//���ò������
  
  for(i=0;i<p12->nSignChannelNum;i++)//��������12ģ��ͨ��
  {
    re=0;
    
    for(j=0;j<p12->nMrsPtcnt;j++)//�ò���Ƿ��Ѿ�����֪����б���
    {
      if(p12->nMrsPt[i]==p12->nMrsPtTable[j])//ͨ���󶨵Ĳ����ڲ���б���
      {//��֪��㣬��������Ӱ�䵽12ͨ����������б���
        re=1;
        p12->nMrsPtIndex[i]=j;
        if(p12->nChIndexMrsPt[i]==0)
          p12->nMrsPtDataSize0[j]=_DEFAULT_RM_DATA_LTH+sizeof(struct tagChValue);
        p12->nMrsPtDataSize[j]+=_DEFAULT_RM_DATA_LTH+sizeof(struct tagChValue);
        p12->nMrsPtChcnt[j]++;//���ͨ�����ӣ�
        break;
      }
    }
    if(re==0)//�²��
    {
      p12->nMrsPtTable[p12->nMrsPtcnt]=p12->nMrsPt[i];//�����뵽����б���
      if(p12->nChIndexMrsPt[i]==0)//��0��ͨ��
        p12->nMrsPtDataSize0[p12->nMrsPtcnt]=_DEFAULT_RM_DATA_LTH+sizeof(struct tagChValue);
      p12->nMrsPtIndex[i]=p12->nMrsPtcnt;//����Ӱ�䵽12ͨ����������б��У��������1
      //������ݳߴ��ʼ��Ϊͨ�����ݳߴ�
      p12->nMrsPtDataSize[p12->nMrsPtcnt]=_DEFAULT_RM_DATA_LTH+sizeof(struct tagChValue);
      p12->nMrsPtChcnt[p12->nMrsPtcnt]=1;//���ͨ������ʼ��Ϊ1
      p12->nMrsPtcnt++;
    }
  }
  return re;
}

INT32S myAllocMemery(INT32S nDynamiccnt,INT32S nTrancnt);
INT32S myAllocMemery(INT32S nDynamiccnt,INT32S nTrancnt)
{
  INT32S re = 0,i,j,k,l,nOffsetJKey = 0,nOffsetTriger = 0,nOffsetChannel = 0,nOffsetGroup = 0;
  INT8S* p = 0;
  FP32   fValue;//
  int val1;
  int val2;
  //static data table init
  memset(g_StaticOriginData, 0, sizeof(g_StaticOriginData));
  memset(g_pStaticChannelOrigin, 0, sizeof(g_pStaticChannelOrigin));//[_MAX_JKEY_CHANNEL_CNT][_MAX_STATIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
  memset(g_StaticTrigerEventArray, 0, sizeof(g_StaticTrigerEventArray));//[_MAX_JKEY_CHANNEL_CNT];
  
  nOffsetJKey = 0;
  for(i = 0;i<g_83param.nDevicecnt;i++)
  {
    g_pStaticChannelOrigin[i][0][0] = g_StaticOriginData+nOffsetJKey;
    nOffsetTriger = 0;
    for(j = 0;j<_MAX_STATIC_ARRAY_LTH;j++)
    {
      g_pStaticChannelOrigin[i][j][0] = g_pStaticChannelOrigin[i][0][0]+nOffsetTriger;
      for(k = 0;k<g_83param.Device[i].nStaticChannelNum;k++)
      {
        g_pStaticChannelOrigin[i][j][k] = g_pStaticChannelOrigin[i][j][0]+k;	
      }
      nOffsetTriger += g_83param.Device[i].nStaticChannelNum;
    }
    nOffsetJKey += g_83param.Device[i].nStaticChannelNum*_MAX_STATIC_ARRAY_LTH;
  }
  
  ////////////////////////////////////////////////////////
  //g_tmpStaticData=malloc(sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead));
  g_tmpStaticDataHead=(ProtocolHead *)g_tmpStaticData;
  g_ptmpStaticData[0]=(struct tagChValue *)(g_tmpStaticData+sizeof(ProtocolHead));
  for(i=1;i<_MAX_SIGNAL_CHANNEL_CNT;i++)
  {
    g_ptmpStaticData[i]=g_ptmpStaticData[i-1]+1;
  }
  memset(g_tmpStaticData, 0, sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead));
  //Dynamic data table init
  //Value
  memset(g_DynamicValue, 0, sizeof(g_DynamicValue));
  memset(g_pDynamicChannelValue, 0, sizeof(g_pDynamicChannelValue));//[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
  memset(g_DynamicTrigerEventArray, 0,  sizeof(g_DynamicTrigerEventArray));//[_MAX_JKEY_CHANNEL_CNT];
  
  nOffsetJKey = 0;
  for(i = 0;i<g_83param.nDevicecnt;i++)
  {
    g_pDynamicChannelValue[i][0][0] = g_DynamicValue+nOffsetJKey;
    nOffsetTriger = 0;
    for(j = 0;j<_MAX_DYNAMIC_ARRAY_LTH;j++)
    {
      g_pDynamicChannelValue[i][j][0] = g_pDynamicChannelValue[i][0][0]+nOffsetTriger;
      for(k = 0;k<g_83param.Device[i].nDynamicChannelNum;k++)
      {
        g_pDynamicChannelValue[i][j][k] = g_pDynamicChannelValue[i][j][0]+k;	
      }
      nOffsetTriger += g_83param.Device[i].nDynamicChannelNum;
    }
    nOffsetJKey += g_83param.Device[i].nDynamicChannelNum*_MAX_DYNAMIC_ARRAY_LTH;
  }
  
  //Wave Form
  //g_tmpDynamicData = malloc(sizeof(INT32S)*_DYAMIC_BUFFER_LTH+sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead));
  memset(g_tmpDynamicData, 0, sizeof(INT32S)*_DYAMIC_BUFFER_LTH+sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead));
  g_tmpDynamicDataHead=(ProtocolHead *)g_tmpDynamicData;
  g_ptmpDynamicValue[0]=(struct tagChValue *)(g_tmpDynamicData+sizeof(ProtocolHead));
  for(i=1;i<_MAX_SIGNAL_CHANNEL_CNT;i++)
  {
    g_ptmpDynamicValue[i]=g_ptmpDynamicValue[i-1]+1;
  }
  g_ptmpDynamicData[0] = (struct tagChData2060 *)(g_tmpDynamicData+sizeof(ProtocolHead)+sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT);
  nOffsetJKey = _BASE_DYNAMIC_CHANNEL_DATA_ADDRESS;
  for(i = 0;i<g_83param.nDevicecnt;i++)
  {
    g_nChannel_Dynamic_Wave_Data_Offset[i][0][0] = nOffsetJKey;
    nOffsetTriger = 0; 
    for(j = 0;j<_MAX_DYNAMIC_ARRAY_LTH;j++)
    {
      g_nChannel_Dynamic_Wave_Data_Offset[i][j][0] = g_nChannel_Dynamic_Wave_Data_Offset[i][0][0]+nOffsetTriger;
      for(k = 1;k<g_83param.Device[i].nDynamicChannelNum;k++)
      {
        g_nChannel_Dynamic_Wave_Data_Offset[i][j][k] = g_nChannel_Dynamic_Wave_Data_Offset[i][j][k-1]+
          g_83param.Device[i].nChDynamicDataSize/*[k]*/;
      }
      nOffsetTriger += g_83param.Device[i].nAllDynamicChDataSize;
    }
    nOffsetJKey += g_83param.Device[i].nAllDynamicChDataSize*_MAX_DYNAMIC_ARRAY_LTH;
  }
  for(i = 0;i<g_83param.nDevicecnt;i++)
  {
    g_nChannel_Dynamic_Key_Data_Offset[i][0] = nOffsetJKey;
    g_DynamicTrigerEventArray[i].ChannelSignalDataTable[0].ulnBlockCnt=g_83param.Device[i].nChDynamicDataBlockCnt;
    g_DynamicTrigerEventArray[i].ChannelSignalDataTable[0].ulnBlockStep=1;
    for(j = 1;j<_MAX_DYNAMIC_ARRAY_LTH;j++)
    {
      g_nChannel_Dynamic_Key_Data_Offset[i][j] = 
        g_nChannel_Dynamic_Key_Data_Offset[i][j-1]+_MAX_DYNAMIC_KEY_POINT_CNT*sizeof(INT32S);
      
      g_DynamicTrigerEventArray[i].ChannelSignalDataTable[j].ulnBlockCnt=g_83param.Device[i].nChDynamicDataBlockCnt;
      g_DynamicTrigerEventArray[i].ChannelSignalDataTable[j].ulnBlockStep=1;
    }
    nOffsetJKey += _MAX_DYNAMIC_KEY_POINT_CNT*_MAX_DYNAMIC_ARRAY_LTH*sizeof(INT32S);
  }
  //Tran data table init
  //Value
  
  memset(g_pTranChannelValue, 0, sizeof(g_pTranChannelValue));//[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
  memset(g_TranValue, 0, sizeof(g_TranValue));
  memset(g_TranTrigerEventArray, 0, sizeof(g_TranTrigerEventArray));//[_MAX_JKEY_CHANNEL_CNT];
  nOffsetJKey = 0;
  for(i = 0;i<g_83param.nDevicecnt;i++)
  {
    g_pTranChannelValue[i][0][0][0] = g_TranValue+nOffsetJKey;
    nOffsetTriger = 0;
    for(j = 0;j<_MAX_TRAN_ARRAY_LTH;j++)
    {
      g_pTranChannelValue[i][j][0][0] = g_pTranChannelValue[i][0][0][0]+nOffsetTriger;
      nOffsetGroup=0;
      for(k = 0;k<g_83param.Device[i].nTranChannelNum;k++)
      {
        g_pTranChannelValue[i][j][k][0] = g_pTranChannelValue[i][j][0][0]+nOffsetGroup;
        for(l = 0;l<_MAX_TRAN_GROUP;l++)
        {
          g_pTranChannelValue[i][j][k][l] = g_pTranChannelValue[i][j][k][0]+l;
          g_TranTrigerEventArray[i].ChannelSignalDataTable[j][l].ulnBlockCnt=1;
          g_TranTrigerEventArray[i].ChannelSignalDataTable[j][l].ulnBlockStep=1;
          g_TranTrigerEventArray[i].ChannelSignalDataTable[j][l].lnHeadBlock=0;
          g_TranTrigerEventArray[i].ChannelSignalDataTable[j][l].lnCurBlock=0;
        }
        nOffsetGroup += _MAX_TRAN_GROUP;
      }
      nOffsetTriger += g_83param.Device[i].nTranChannelNum*_MAX_TRAN_GROUP;
    }
    nOffsetJKey += g_83param.Device[i].nTranChannelNum*_MAX_TRAN_ARRAY_LTH*_MAX_TRAN_GROUP;
  }
  //Wave Form
  //#define	_TRAN_BUFFER_LTH
  //g_tmpTranData = malloc(sizeof(INT32S)*_TRAN_BUFFER_LTH+sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead));
  memset(g_tmpTranData, 0, sizeof(INT32S)*_TRAN_BUFFER_LTH+sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead));
  g_tmpTranDataHead=(ProtocolHead *)g_tmpTranData;
  g_ptmpTranValue[0]=(struct tagChValue *)(g_tmpTranData+sizeof(ProtocolHead));
  memset(g_ptmpTranValue, 0, sizeof(g_ptmpTranValue));
  memset(g_ptmpTranData,0, sizeof(g_ptmpTranData));
  for(i=1;i<_MAX_SIGNAL_CHANNEL_CNT;i++)
  {
    g_ptmpTranValue[i]=g_ptmpTranValue[i-1]+1;
  }
  g_ptmpTranData[0] = (struct tagChData2060 *)(g_tmpTranData+sizeof(ProtocolHead)+sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT);
  nOffsetJKey = _BASE_TRAN_CHANNEL_DATA_ADDRESS;
  for(i = 0;i<g_83param.nDevicecnt;i++)
  {
    g_nChannel_Tran_Wave_Data_Offset[i][0][0][0] = nOffsetJKey;
    nOffsetTriger = 0;
    for(j = 0;j<_MAX_TRAN_ARRAY_LTH;j++)
    {
      g_nChannel_Tran_Wave_Data_Offset[i][j][0][0] = g_nChannel_Tran_Wave_Data_Offset[i][0][0][0]+nOffsetTriger;
      nOffsetGroup = 0;
      for(k = 0;k<g_83param.Device[i].nTranChannelNum;k++)
      {
        g_nChannel_Tran_Wave_Data_Offset[i][j][k][0] = g_nChannel_Tran_Wave_Data_Offset[i][j][0][0]+nOffsetGroup;
        for(l = 1;l<_MAX_TRAN_GROUP;l++)
        {
          g_nChannel_Tran_Wave_Data_Offset[i][j][k][l] = g_nChannel_Tran_Wave_Data_Offset[i][j][k][l-1]+
            g_83param.Device[i].nChTranDataSize/*[k]*/;
        }
        nOffsetGroup += g_83param.Device[i].nChTranDataSize/*[k]*/*_MAX_TRAN_GROUP;
      }
      nOffsetTriger += g_83param.Device[i].nAllTranChDataSize*_MAX_TRAN_GROUP;
    }
    nOffsetJKey += g_83param.Device[i].nAllTranChDataSize*_MAX_TRAN_GROUP*_MAX_TRAN_ARRAY_LTH;
  }
  
  for(i = 0;i<g_83param.nDevicecnt;i++)
  {
    g_nChannel_Tran_Key_Data_Offset[i][0][0] = nOffsetJKey;
    nOffsetTriger = 0;
    for(j = 0;j<_MAX_TRAN_ARRAY_LTH;j++)
    {
      g_nChannel_Tran_Key_Data_Offset[i][j][0] = g_nChannel_Tran_Key_Data_Offset[i][0][0]+nOffsetTriger;
      for(l = 1;l<_MAX_TRAN_GROUP;l++)
      {
        g_nChannel_Tran_Key_Data_Offset[i][j][l] = 
          g_nChannel_Tran_Key_Data_Offset[i][j][l-1]+_MAX_TRAN_KEY_POINT_CNT*sizeof(INT32S);
      }
      nOffsetTriger += _MAX_TRAN_KEY_POINT_CNT*_MAX_TRAN_GROUP*sizeof(INT32S);
    }
    nOffsetJKey += _MAX_TRAN_KEY_POINT_CNT*_MAX_TRAN_GROUP*_MAX_TRAN_ARRAY_LTH*sizeof(INT32S);
  }
  
  InitInterfaceAddress( );
  return re;
}


void myMapChannel(void)
{
  INT32S i,j,tmp1,tmp2;
  for(i = 0;i<g_nGroupCnt;i++)//����������˲̬����ͷ����,���жϱ���
  {
    tmp1 = i*_FPGA_GROUP_CHANNEL_CNT;//Ҫ��g_nGroupCnt����Ϊ2��ָ����/**/
    for(j=0;j<_FPGA_GROUP_CHANNEL_CNT;j++)
    {
      tmp2 = g_nChannelArrivalSequenceTable[tmp1+j];
      g_nArrivalDeviceNo[tmp1+j] = g_12Param.nChannelNumIn83[tmp2+tmp2];//�����豸��
      g_nArrivalChannelNo[tmp1+j] = g_12Param.nChannelNumIn83[tmp2+tmp2+1];//�����豸��ͨ����
    }
  }
  i=i;
}

INT32S Init2060()
{
  INT32S re = 0;
  int32_t point = 0;
  
  Param_Point k[] = {
    {0, 2, 0, 1}, {0, 2, 2, 3}, {0, 2, 4, 5}, {0, 2, 6, 7}, {0, 2, 8, 9}, {0, 2,10,11}, {0, 2,12,13}, {0, 2,14,15}, 
    {0, 2,16,17}, {1, 2, 0, 1}, {1, 2, 2, 3}, {1, 2, 4, 5}, {1, 2, 6, 7}, {1, 2, 8, 9}, {2, 2, 0, 1}, {2, 2, 2, 3}, 
    {2, 2, 4, 5}, {2, 2, 6, 7}, {2, 2, 8, 9}, {3, 2, 0, 1}, {3, 2, 2, 3}, {3, 2, 4, 5}, {3, 2, 6, 7}, {3, 2, 8, 9}};
  InitDataLimit();
  InitRunTimeFlag();
  InitEvent();
  //memset(g_12Param.nMrsPtDataSize, 0, sizeof(INT32S)*_MAX_SIGNAL_CHANNEL_CNT);
  //memset(g_12Param.nMrsPtDataSize,  sizeof(INT32S)*_MAX_SIGNAL_CHANNEL_CNT);
  //memset(g_12Param.nMrsPtDataSize0,  sizeof(INT32S)*_MAX_SIGNAL_CHANNEL_CNT);
  myRead12Param(&g_12Param);
  myInit83Param(&g_83param,&g_12Param);
  myRenewParam(&g_83param,&g_12Param);
  re = InitNet(g_strHostIP,g_nTcpPort,g_nUdpPort);
  re = myAllocMemery(_MAX_DYNAMIC_ARRAY_LTH,g_nTranGroupcnt);
  BuildSignalGroupTable(g_nChannelArrivalSequenceTable);
  myMapChannel();
  /*czx add begin
  * init ���
  */
  g_83param.nMrsPtcnt = 24;
  for(point=0; point<g_83param.nMrsPtcnt; ++point)
  {
    g_83param.nMrsPtTable[point] = point;
    g_83param.nPtMrsNo[point].jkey = k[point].jkey;
    g_83param.nPtMrsNo[point].nChCnt = k[point].nChCnt;
    g_83param.nPtMrsNo[point].channel[0] = k[point].channel[0];
    g_83param.nPtMrsNo[point].channel[1] = k[point].channel[1];
  }
  /* czx add end
  *
  */
  
  //OSTaskCreate(Simu_Send_Data, 0, &TaskStk[2][TASK_STK_SIZE-1], Task1_Prio+RECEIVE_12DATA_TASK_PRIO);
  /*OSTaskCreate(&Simu_Send_Data_tcb,
  (CPU_CHAR*)"Simu_Send_Data",
  (OS_TASK_PTR )Simu_Send_Data,
  (void       *)0,
  (OS_PRIO     )Task1_Prio+RECEIVE_12DATA_TASK_PRIO,
  (CPU_STK    *)&Simu_Send_Data_stk[0],
  (CPU_STK_SIZE)2048,
  (CPU_STK_SIZE)TASK_STK_SIZE,
  (OS_MSG_QTY  )0,
  (OS_TICK     )0,
  (void       *)0,
  (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
  (OS_ERR     *)&err);
  */
  //OSTaskCreate(receive_channel_data, 0, &TaskStk[4][TASK_STK_SIZE-1], Task1_Prio+PICK_TASK_PRIO);
  //OSTaskCreate(command_analys, 0, &TaskStk[3][TASK_STK_SIZE-1], Task1_Prio+COMMAND_ANALYS_TASK_PRIO);
  //OSTaskCreate(tff, 0, &TaskStk[3][TASK_STK_SIZE-1], Task1_Prio+COMMAND_ANALYS_TASK_PRIO);
  //OpenRealTimeValuePipe();
  return re;
}

INT32S Uninit2060()
{
  INT32S re = 0;
  //OS_ERR err;
  //OSTaskDel(Task1_Prio+COMMAND_ANALYS_TASK_PRIO, &err);
  //OSTaskDel(Task1_Prio+RECEIVE_12DATA_TASK_PRIO, &err);
  UnInitNet();
  return re;
}

extern void Simu_Send_Data(void* ps);
extern void receive_channel_data(void *arg);

INT32S Start2060()
{
  
  INT32S re = 0;
  RenewParam(&g_83param,&g_12Param);
  //OpenRealTimeValuePipe();
  //OpenStaticDataPipe();
  //OSTaskCreate(receive_channel_data, 0, &TaskStk[4][TASK_STK_SIZE-1], Task1_Prio+PICK_TASK_PRIO);
  //OSTaskCreate(static_transfer, 0, &TaskStk[5][TASK_STK_SIZE-1], Task1_Prio+STATIC_TRANSFER_TASK_PRIO);
  //OSTaskCreate(dynamic_transfer, 0, &TaskStk[6][TASK_STK_SIZE-1], Task1_Prio+DYNAMIC_TRANSFER_TASK_PRIO);
  //OSTaskCreate(tran_transfer, 0, &TaskStk[7][TASK_STK_SIZE-1], Task1_Prio+TRAN_TRANSFER_TASK_PRIO);
  //OSTaskCreate(real_time_show, 0, &TaskStk[8][TASK_STK_SIZE-1], Task1_Prio+REAL_TIME_SHOW_TASK_PRIO);
  //OSTaskCreate(real_time_show_pipe, 0, &TaskStk[8][TASK_STK_SIZE-1], Task1_Prio+REAL_TIME_SHOW_TASK_PRIO);
  
  OSTaskCreate(Simu_Send_Data, 0, &TaskStk[2][TASK_STK_SIZE-1], Task1_Prio+RECEIVE_12DATA_TASK_PRIO);
//  OSTaskCreateExt(Simu_Send_Data,
//                  (void           *) 0,
//                  (OS_STK         *)&Simu_Send_Data_stk[TASK_STK_SIZE - 1],
//                  (INT8U           ) Task1_Prio+RECEIVE_12DATA_TASK_PRIO,
//                  (INT16U          ) Task1_Prio+RECEIVE_12DATA_TASK_PRIO,
//                  (OS_STK         *)&Simu_Send_Data_stk[0],
//                  (INT32U          ) TASK_STK_SIZE,
//                  (void           *) 0,
//                  (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
  
  
  //  OSTaskCreate(&Simu_Send_Data_tcb,
  //               (CPU_CHAR*)"Simu_Send_Data",
  //               (OS_TASK_PTR )Simu_Send_Data,
  //               (void       *)0,
  //               (OS_PRIO     )Task1_Prio+RECEIVE_12DATA_TASK_PRIO,
  //               (CPU_STK    *)&Simu_Send_Data_stk[0],
  //               (CPU_STK_SIZE)2048,
  //               (CPU_STK_SIZE)TASK_STK_SIZE,
  //               (OS_MSG_QTY  )0,
  //               (OS_TICK     )0,
  //               (void       *)0,
  //               (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
  //               (OS_ERR     *)&err);
  //  
  //  OSTaskCreate(&static_transfer_tcb,
  //               (CPU_CHAR*)"static_transfer",
  //               (OS_TASK_PTR )static_transfer, 
  //               (void       *)0,
  //               (OS_PRIO     )Task1_Prio+STATIC_TRANSFER_TASK_PRIO,
  //               (CPU_STK    *)&static_transfer_stk[0],
  //               (CPU_STK_SIZE)2048,
  //               (CPU_STK_SIZE)TASK_STK_SIZE,
  //               (OS_MSG_QTY  )0,
  //               (OS_TICK     )0,
  //               (void       *)0,
  //               (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
  //               (OS_ERR     *)&err);
  //  
  //  OSTaskCreate(&dynamic_transfer_tcb,
  //               (CPU_CHAR*)"dynamic_transfer",
  //               (OS_TASK_PTR )dynamic_transfer, 
  //               (void       *)0,
  //               (OS_PRIO     )Task1_Prio+DYNAMIC_TRANSFER_TASK_PRIO,
  //               (CPU_STK    *)&dynamic_transfer_stk[0],
  //               (CPU_STK_SIZE)2048,
  //               (CPU_STK_SIZE)TASK_STK_SIZE,
  //               (OS_MSG_QTY  )0,
  //               (OS_TICK     )0,
  //               (void       *)0,
  //               (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
  //               (OS_ERR     *)&err);
  //  
  //  OSTaskCreate(&tran_transfer_tcb,
  //               (CPU_CHAR*)"tran_transfer",
  //               (OS_TASK_PTR )tran_transfer, 
  //               (void       *)0,
  //               (OS_PRIO     )Task1_Prio+TRAN_TRANSFER_TASK_PRIO,
  //               (CPU_STK    *)&tran_transfer_stk[0],
  //               (CPU_STK_SIZE)2048,
  //               (CPU_STK_SIZE)TASK_STK_SIZE,
  //               (OS_MSG_QTY  )0,
  //               (OS_TICK     )0,
  //               (void       *)0,
  //               (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
  //               (OS_ERR     *)&err);
  
  //OSTaskCreate(&realvalue_transfer_tcb,
  //    (CPU_CHAR*)"realvalue_transfer",
  //    (OS_TASK_PTR )realvalue_transfer, 
  //    (void       *)0,
  //    (OS_PRIO     )Task1_Prio+REALVALUE_TRANSFER_TASK_PRIO,
  //    (CPU_STK    *)&realvalue_transfer_stk[0],
  //    (CPU_STK_SIZE)2048,
  //    (CPU_STK_SIZE)TASK_STK_SIZE,
  //    (OS_MSG_QTY  )0,
  //    (OS_TICK     )0,
  //    (void       *)0,
  //    (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
  //    (OS_ERR     *)&err);
  //    
  //OSTaskCreate(&realdata_transfer_tcb,
  //    (CPU_CHAR*)"realdata_transfer",
  //    (OS_TASK_PTR )realdata_transfer, 
  //    (void       *)0,
  //    (OS_PRIO     )Task1_Prio+REALDATA_TRANSFER_TASK_PRIO,
  //    (CPU_STK    *)&realvalue_transfer_stk[0],
  //    (CPU_STK_SIZE)2048,
  //    (CPU_STK_SIZE)TASK_STK_SIZE,
  //    (OS_MSG_QTY  )0,
  //    (OS_TICK     )0,
  //    (void       *)0,
  //    (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
  //    (OS_ERR     *)&err);
  return re;
  
}

INT32S Stop2060()
{
  INT32S re = 0;
  //OSTaskDel(Task1_Prio+TRAN_TRANSFER_TASK_PRIO);
  //OSTaskDel(Task1_Prio+DYNAMIC_TRANSFER_TASK_PRIO);
  //OSTaskDel(Task1_Prio+STATIC_TRANSFER_TASK_PRIO);
  //OSTaskDel(Task1_Prio+PICK_TASK_PRIO);
  //OSTaskDel(Task1_Prio+REAL_TIME_SHOW_TASK_PRIO);
  //CloseRealTimeValuePipe();
  return re;
}

INT32S OutputTrigerEventToDisk(char strName[])
{
  INT32S re=0,nOffset,i,j,k,l;
#if 0
  struct stat statbuf;
  int len;
  FILE *pf=fopen(strName,"wb");
  re=fwrite(&g_12Param,sizeof(g_12Param),1,pf);
  re=fwrite(&g_83param,sizeof(g_83param),1,pf);
  re=fwrite(g_StaticOriginData,sizeof(g_StaticOriginData),1,pf);
  re=fwrite(g_StaticTrigerEventArray,sizeof(g_StaticTrigerEventArray),1,pf);
  re=fwrite(g_DynamicValue,sizeof(g_DynamicValue),1,pf);
  re=fwrite(g_DynamicTrigerEventArray,sizeof(g_DynamicTrigerEventArray),1,pf);
  re=fwrite(g_nChannel_Dynamic_Wave_Data_Offset,sizeof(g_nChannel_Dynamic_Wave_Data_Offset),1,pf);
  re=fwrite(g_nChannel_Dynamic_Key_Data_Offset,sizeof(g_nChannel_Dynamic_Key_Data_Offset),1,pf);
  re=fwrite(g_TranValue,sizeof(g_TranValue),1,pf);
  re=fwrite(g_TranTrigerEventArray,sizeof(g_TranTrigerEventArray),1,pf);
  re=fwrite(g_nChannel_Tran_Wave_Data_Offset,sizeof(g_nChannel_Tran_Wave_Data_Offset),1,pf);
  re=fwrite(g_nChannel_Tran_Key_Data_Offset,sizeof(g_nChannel_Tran_Key_Data_Offset),1,pf);
  for(i=0;i<_MAX_JKEY_CHANNEL_CNT;i++)
  {
    for(j=0;j<_MAX_STATIC_ARRAY_LTH;j++)
    {
      for(k=0;k<_MAX_SIGNAL_CHANNEL_CNT;k++)
      {
        INT32S nOffset=g_pStaticChannelOrigin[i][j][k]-g_StaticOriginData;
        re=fwrite(&nOffset,sizeof(INT32S*),1,pf);
      }
    }
  }
  for(i=0;i<_MAX_JKEY_CHANNEL_CNT;i++)
  {	
    for(j=0;j<_MAX_DYNAMIC_ARRAY_LTH;j++)
    {
      for(k=0;k<_MAX_SIGNAL_CHANNEL_CNT;k++)
      {
        INT32S nOffset=g_pDynamicChannelValue[i][j][k]-g_DynamicValue;
        re=fwrite(&nOffset,sizeof(INT32S),1,pf);
      }
    }
  }
  for(i=0;i<_MAX_JKEY_CHANNEL_CNT;i++)
  {
    for(j=0;j<_MAX_TRAN_ARRAY_LTH;j++)
      for(k=0;k<_MAX_SIGNAL_CHANNEL_CNT;k++)
        for(l=0;l<_MAX_TRAN_GROUP;l++)
        {
          INT32S nOffset=g_pTranChannelValue[i][j][k][l]-g_TranValue;
          re=fwrite(&nOffset,sizeof(INT32S),1,pf);
        }
  }
  
  fclose(pf);
  len = stat(strName, &statbuf);
  //pf=fopen(strName,"rb");
  //re=fread(&g_12Param,sizeof(g_12Param),1,pf);
  //re=fread(&g_83param,sizeof(g_83param),1,pf);
  //fclose(pf);
#endif
  return re;
}

