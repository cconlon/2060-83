#include "define2060.h"
#include "ucos_ii.h"
#include "common.h"
#include "interface.h"
#include "lib_mem.h"
#include "main83.h"
#include "netio.h"
#include <string.h>
#include "app_cfg.h"

extern INT8S					                g_strHostIP[];
extern INT32U					                g_nTcpPort;
extern INT32U					                g_nUdpPort;
extern INT32S					                g_nSystemNo;//系统编号
extern INT32S					                g_nMgrNo;   //机组编号
extern INT32S					                g_nMchNo[_MAX_SIGNAL_CHANNEL_CNT];      //机器编号
extern INT32S					                g_nDeviceNo[_MAX_SIGNAL_CHANNEL_CNT];   //虚拟设备编号
extern const INT32S				            g_n12UpFrequency;                   //12模块上传数据的频率,由海淀给定,不得修改
extern const INT32S				            g_TempAddtionalcnt;                 //附加临时数据组数
extern const INT32S				            g_nMaxTranGroupDataSize;
extern const INT32S				            g_nMaxTranDataGroup;
extern const INT32S				            g_nMaxDataSize;
extern const INT32S				            g_nTranGroupcnt;
//
extern Tst_Head_DCM_SigModuSampData_SYS		g_StaticOriginData[_MAX_STATIC_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT];//静态测量值队列
extern Tst_Head_DCM_SigModuSampData_SYS*	    g_pStaticChannelOrigin[_MAX_JKEY_CHANNEL_CNT][_MAX_STATIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
extern Static_Triger_Table					    g_StaticTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];
//
extern INT32U									g_nChannel_Dynamic_Wave_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
extern INT32U									g_nChannel_Dynamic_Key_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH];
//
extern Data_Limit_83			                g_83DataLimit;//83模块的一些极限参数
extern INT32S					                g_nChannelArrivalSequenceTable[_MAX_SIGNAL_CHANNEL_CNT];//到达通道次12通道序号表
extern INT32S					                g_nArrivalDeviceNo[_MAX_SIGNAL_CHANNEL_CNT];//到达通道虚拟设备表
extern INT32S					                g_nArrivalChannelNo[_MAX_SIGNAL_CHANNEL_CNT];//通道数据到达在所属设备的通道号
extern Param_Table_12			                g_12Param;//12模块参数,从12模块读取,不保存,由12congfig模块配置,360系统不可修改
extern Param_Table_83			                g_83param;//83模块参数,存储在flash,可从上位机更新
extern Run_Time_Flag			                g_RunTimeFlag;//83运行状态信息
//
extern Tst_Head_DCM_SigModuSampData_SYS		g_StaticOriginData[_MAX_STATIC_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT];
extern Tst_Head_DCM_SigModuSampData_SYS*		g_pStaticChannelOrigin[_MAX_JKEY_CHANNEL_CNT][_MAX_STATIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
extern INT8S g_tmpStaticData[sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead)];
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
extern Channel_Dynamic_Value*					g_ptmpDynamicValue[_MAX_SIGNAL_CHANNEL_CNT];//待发送动态数据测量值
extern Channel_Dynamic_Wave_Data*				g_ptmpDynamicData[_MAX_SIGNAL_CHANNEL_CNT];//待发送动态通道原始数据
extern INT32U									g_nChannel_Dynamic_Wave_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
extern INT32U									g_nChannel_Dynamic_Key_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH];
//
extern Tst_Head_DCM_SigModuSampData_SYS		g_TranValue[_MAX_TRAN_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT*_MAX_TRAN_GROUP];//瞬态测量值队列
//根据12模块的键相-通道邦定列表创建,用来快速检索瞬态态数据头
extern Tst_Head_DCM_SigModuSampData_SYS*		g_pTranChannelValue[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT][_MAX_TRAN_GROUP];
extern Tran_Triger_Table						g_TranTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];
//extern INT8S*									g_tmpTranData;//待发送瞬态数据
extern INT8S  g_tmpTranData[(sizeof(INT32S)*_TRAN_BUFFER_LTH+sizeof(struct tagChValue)*_MAX_SIGNAL_CHANNEL_CNT+sizeof(ProtocolHead))];


extern ProtocolHead*							g_tmpTranDataHead;
extern Channel_Tran_Value*						g_ptmpTranValue[_MAX_SIGNAL_CHANNEL_CNT];//待发送动态数据测量值
extern Channel_Tran_Wave_Data*					g_ptmpTranData[_MAX_SIGNAL_CHANNEL_CNT];
//
extern INT32U		g_nChannel_Tran_Wave_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT][_MAX_TRAN_GROUP];
extern INT32U		g_nChannel_Tran_Key_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_TRAN_GROUP];

extern INT32U		g_Signal_Channel_Dynamic_Write_Position[_MAX_SIGNAL_CHANNEL_CNT];//各个通道的动态数据写入位置
extern INT32U		g_Signal_Channel_Tran_Write_Position[_MAX_SIGNAL_CHANNEL_CNT];//各个通道的瞬态数据写入位置
extern INT32U		g_Key_Channel_Dynamic_Write_Position[_MAX_JKEY_CHANNEL_CNT];//各个通道的动态键相数据写入位置
extern INT32U		g_Key_Channel_Tran_Write_Position[_MAX_JKEY_CHANNEL_CNT];//各个通道的瞬态键相数据写入位置
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

//Data_Limit_83				g_83DataLimit;//83模块的一些极限参数
extern INT32S				g_nChannelArrivalSequenceTable[_MAX_SIGNAL_CHANNEL_CNT];//到达通道次12通道序号表
extern INT32S				g_nArrivalDeviceNo[_MAX_SIGNAL_CHANNEL_CNT];//到达通道虚拟设备表
extern INT32S				g_nArrivalChannelNo[_MAX_SIGNAL_CHANNEL_CNT];//通道数据到达在所属设备的通道号

extern INT32U		g_Signal_Channel_Dynamic_Write_Position[_MAX_SIGNAL_CHANNEL_CNT];//各个通道的动态数据写入位置
extern INT32U		g_Signal_Channel_Tran_Write_Position[_MAX_SIGNAL_CHANNEL_CNT];   //各个通道的瞬态数据写入位置
extern INT32U		g_Key_Channel_Dynamic_Write_Position[_MAX_JKEY_CHANNEL_CNT];     //各个通道的动态键相数据写入位置
extern INT32U		g_Key_Channel_Tran_Write_Position[_MAX_JKEY_CHANNEL_CNT];        //各个通道的瞬态键相数据写入位置

extern INT32U		g_nArmDynamicChDataOffset[_MAX_SIGNAL_CHANNEL_CNT*2+_MAX_JKEY_CHANNEL_CNT*2];
extern INT32U*		g_nArmDynamicKeyDataOffset;
extern INT32U*		g_nArmTranChDataOffset;
extern INT32U*		g_nArmTranKeyDataOffset;
extern INT32S		g_nGroupCnt;

extern INT32U		g_nArmDynamicChDataSampleStep[_MAX_SIGNAL_CHANNEL_CNT*2];
extern INT32U*		g_nArmTranChDataSampleStep;
//

/*所有动态内存的分配和释放都在此函数进行，避免碎块产生*/

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

void MapChannel()
{
  INT32S i,j,tmp1,tmp2;
  for(i = 0;i<g_nGroupCnt;i++)//将数据移入瞬态数据头区域,并判断报警
  {
    tmp1 = i*_FPGA_GROUP_CHANNEL_CNT;//要求g_nGroupCnt必须为2的指数倍/**/
    for(j=0;j<_FPGA_GROUP_CHANNEL_CNT;j++)
    {
      tmp2 = g_nChannelArrivalSequenceTable[tmp1+j];
      g_nArrivalDeviceNo[tmp1+j] = g_12Param.nChannelNumIn83[tmp2+tmp2];//所属设备号
      g_nArrivalChannelNo[tmp1+j] = g_12Param.nChannelNumIn83[tmp2+tmp2+1];//所属设备的通道号
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
    pValue[nChannel].nRPM=pOriginValue[nChannel]->ulnRPM;//	测量值	转速	rpm	
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
    cnt = GetCommand(sBuffer,256*sizeof(INT32S));//从命令管线读取数据
    OSTimeDly(10);
  }while(1);
#endif
}

extern Key_Speed_Calt	g_DynamicSpeedCalt[];

INT32S AllocMemery(INT32S nDynamiccnt,INT32S nTrancnt);
INT32S AllocMemery(INT32S nDynamiccnt,INT32S nTrancnt)
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
  Read12Param(&g_12Param);
  Init83Param(&g_83param,&g_12Param);
  RenewParam(&g_83param,&g_12Param);
  re = InitNet(g_strHostIP,g_nTcpPort,g_nUdpPort);
  re = AllocMemery(_MAX_DYNAMIC_ARRAY_LTH,g_nTranGroupcnt);
  BuildSignalGroupTable(g_nChannelArrivalSequenceTable);
  MapChannel();
  /*czx add begin
  * init 测点
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

static CPU_STK static_transfer_stk[STATIC_TRANSFER_TASK_SIZE];
static CPU_STK Simu_Send_Data_stk[SIMU_SEND_DATA_TASK_SIZE];

INT32S Start2060()
{
  INT32S re = 0;
  RenewParam(&g_83param,&g_12Param);
  //OpenRealTimeValuePipe();
  //OpenStaticDataPipe();
  //OSTaskCreate(receive_channel_data, 0, &TaskStk[4][TASK_STK_SIZE-1], Task1_Prio+PICK_TASK_PRIO);
  
  OSTaskCreateExt(Simu_Send_Data, 0, 
                  &Simu_Send_Data_stk[SIMU_SEND_DATA_TASK_SIZE-1], SIMU_SEND_DATA_TASK_PRIO, 
                  SIMU_SEND_DATA_TASK_PRIO, &Simu_Send_Data_stk[0], SIMU_SEND_DATA_TASK_SIZE, 0, OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
     
 
  OSTaskCreateExt(static_transfer, 0, 
                  &static_transfer_stk[STATIC_TRANSFER_TASK_SIZE-1], STATIC_TRANSFER_TASK_PRIO, STATIC_TRANSFER_TASK_PRIO, 
                  &static_transfer_stk[0], STATIC_TRANSFER_TASK_SIZE, 0, OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

  
  //OSTaskCreate(dynamic_transfer, 0, &TaskStk[6][TASK_STK_SIZE-1], Task1_Prio+DYNAMIC_TRANSFER_TASK_PRIO);
  //OSTaskCreate(tran_transfer, 0, &TaskStk[7][TASK_STK_SIZE-1], Task1_Prio+TRAN_TRANSFER_TASK_PRIO);
  //OSTaskCreate(real_time_show, 0, &TaskStk[8][TASK_STK_SIZE-1], Task1_Prio+REAL_TIME_SHOW_TASK_PRIO);
  //OSTaskCreate(real_time_show_pipe, 0, &TaskStk[8][TASK_STK_SIZE-1], Task1_Prio+REAL_TIME_SHOW_TASK_PRIO);
  

  //OSTaskCreate(static_transfer, 0, &TaskStk[5][TASK_STK_SIZE-1], Task1_Prio+STATIC_TRANSFER_TASK_PRIO);
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

