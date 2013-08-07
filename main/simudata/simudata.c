#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "simudata.h"
#include "../83/define2060.h"
#include "os_cfg.h"
#include <stdio.h>
#include "lib_mem.h"
#include "ucos_ii.h"
#include "simudata.h"

#define _DEFAULT_DATA_LTH 660
INT32S	g_bDataValid;//是否存在有效数据
//extern INT32S	g_nCurWriteSit;//写入位置，由数据接收任务操作
//extern INT32S	g_nCurReadSit;//读取位置，由数据读取函数操作
extern INT32S		g_nGroupCnt;
extern struct tag12Param		g_12Param;
struct tagChData2060       g_pOutChData[_MAX_SIGNAL_CHANNEL_CNT*(sizeof(struct tagChData2060)+(_MAX_KEY_PT_CNT+_DEFAULT_DATA_LTH-1)*sizeof(INT32S))] @ "ChData2060_SECTION";
INT8S Send_Data_buffer[sizeof(struct tagChData2060)+(_MAX_KEY_PT_CNT+_DEFAULT_DATA_LTH-1)*sizeof(INT32S)] @ "Send_Data_SECTION";
INT8S Tran_buffer[(int)(3000*1.2*(sizeof(struct tagChData2060)+((_MAX_KEY_PT_CNT+_DEFAULT_DATA_LTH-1)*sizeof(INT32S))))] @ "TRAN_SECTION";


struct tagChData2060*		g_pOutChDataArray[_MAX_SIGNAL_CHANNEL_CNT];
INT32S	g_nCaltKeySpeed[_MAX_JKEY_CHANNEL_CNT];
struct tagChValue	g_pOutChValue[_MAX_SIGNAL_CHANNEL_CNT];

extern void receive_channel_data(void *arg);
INT32S	BuildSignalGroupTable(INT32S nChannelSit[]);
INT32S		g_SendTimeInterval=100;
INT32S		g_nChValueFlag=3;//0,writing;1,valid;2,reading,3,over;
INT32S		g_nChDataFlag=3;//0,writing;1,valid;2,reading,3,over;
INT32S		g_nAllcnt=7000;
INT32S		g_nBasicRPM=3000;
INT32S		g_nChcnt=_MAX_SIGNAL_CHANNEL_CNT;
INT32S		g_nDatalth=_DEFAULT_DATA_LTH;
INT32S		g_nTrigerRPM=20;
INT32S		g_nHz=10;
FP32		g_fLevel=100;
FP32		g_fLevel2=120;
INT32S		g_nSyccnt=256;
FP32		g_fPickFrequency=20000;
FP32		g_fCof1x=.50;
FP32		g_fCof2x=.30;
FP32		g_fCofP5x=.20;
INT32S		g_nKeycnt=4;

INT32S		g_nAlertcnt=4;
struct tagChValue  g_BasicValue;
FP32		g_fNormalAlp;

#define _START_KEY_CH	48
#define _ALERT_LAST		10

static INT32S g_nChKeyNo[]={
  48,48,48,48,48,48,48,48,48,48,
  49,49,49,49,49,49,49,49,49,49,
  50,50,50,50,50,50,50,50,50,50,
  51,51,51,51,51,51,51,51,51,51,
  48,48,48,48,48,48,48,48
};

INT32S		g_nKeySpeed[4][28000];

FP32		g_fMaxRpmCof=1.2;
//INT32U		g_nSimuTranPiecelth=-1;

struct tagChData2060*		g_pSimuTranPiece=0;
INT32S		g_nStartRpm=600;

INT32S		g_nSimuAlertMeasureCnt=-1;
struct tagChValue*	g_pSimuAlertMeasureValue=0;

static AlertNode	g_AlertNode[16];
static TranNode		g_TranNode[4];

INT32S g_nTigerTime_bak[][10]={
  {60+600,110+600,350+600,400+600,650+600,700+600,950+600,1000+600,1250+600,1300+600},//虚拟设备的瞬态计划
  {80+600,130+600,380+600,430+600,680+600,730+600,980+600,1030+600,1280+600,1330+600},
  {150+600,200+600,450+600,500+600,750+600,800+600,1050+600,1100+600,1350+600,1400+600},
  {250+600,300+600,550+600,0+600,850+600,900+600,1150+600,1200+600,1450+600,1500+600},
};

INT32S g_nTigerTime[][10]={
  {60+600,110+600,350+600,400+600,650+600,700+600,950+600,1000+600,1250+600,1300+600},//虚拟设备的瞬态计划
  {80+600,130+600,380+600,430+600,680+600,730+600,980+600,1030+600,1280+600,1330+600},
  {150+600,200+600,450+600,500+600,750+600,800+600,1050+600,1100+600,1350+600,1400+600},
  {250+600,300+600,550+600,0+600,850+600,900+600,1150+600,1200+600,1450+600,1500+600},
};

INT32S g_nTigerTime2[][10]={
  {60+600, 110+600, 250+600, 300+600, 350+600, 400+600, 450+600, 500+600, 550+600, 600+600},//虚拟设备的瞬态计划
  {80+600,130+600,180+600,230+600,680+600,730+600,980+600,1030+600,1280+600,1330+600},
  {150+600,200+600,450+600,500+600,750+600,800+600,1050+600,1100+600,1350+600,1400+600},
  {250+600,300+600,550+600,0+600,850+600,900+600,1150+600,1200+600,1450+600,1500+600},
};

//INT32S g_nGrouplth=_FPGA_GROUP_CHANNEL_CNT;//每次fpga数据到的通道数量
INT32S g_nSit[_MAX_SIGNAL_CHANNEL_CNT]={
  0,1,2,3,4,5,6,7,8,9,
  10,11,12,13,14,15,16,17,18,19,
  20,21,22,23,24,25,26,27,28,29,
  30,31,32,33,34,35,36,37,38,39,
  40,41,42,43,44,45,46,47,
};//通道编码

INT32S g_nLength[_MAX_SIGNAL_CHANNEL_CNT];
INT32S g_nChNo[_MAX_SIGNAL_CHANNEL_CNT];

INT32U		g_nFpgaDynamicChDataOffset[_MAX_SIGNAL_CHANNEL_CNT*2+_MAX_JKEY_CHANNEL_CNT*2];
INT32U*		g_nFpgaDynamicKeyDataOffset=g_nFpgaDynamicChDataOffset+_MAX_SIGNAL_CHANNEL_CNT;
INT32U*		g_nFpgaTranDataOffset=g_nFpgaDynamicChDataOffset+_MAX_SIGNAL_CHANNEL_CNT+_MAX_JKEY_CHANNEL_CNT;
INT32U*		g_nFpgaTranKeyDataOffset=g_nFpgaDynamicChDataOffset+_MAX_SIGNAL_CHANNEL_CNT*2+_MAX_JKEY_CHANNEL_CNT;

INT32U		g_nFpgaDynamicChDataSampleStep[_MAX_SIGNAL_CHANNEL_CNT*2];
INT32U*		g_nFpgaTranDataSampleStep=g_nFpgaDynamicChDataSampleStep+_MAX_SIGNAL_CHANNEL_CNT;

Tst_Head_DCM_SigModuSampData_SYS g_TempValue[_MAX_SIGNAL_CHANNEL_CNT];
//INT8S* g_pChData=NULL;
INT8S g_pChData[0x1000000] @ "ChData_SECTION";

INT32S Simu_BuildRamDisk()
{
  INT32S re=0,nChDataSize;
  INT8S *p;
  //	BuildSignalGroupTable(g_nSit);
  for(re=0;re<_MAX_SIGNAL_CHANNEL_CNT;re++)
  {
    g_nLength[re]=g_nDatalth;
    g_nChNo[re]=re;
  }
  memset(g_pChData, 0, sizeof(g_pChData));
  //Mem_Clr(g_pChData, sizeof(g_pChData));
  nChDataSize=sizeof(struct tagChData2060)+(_MAX_KEY_PT_CNT+g_nDatalth-1)*sizeof(INT32S);
  //g_pOutChData=malloc(_MAX_SIGNAL_CHANNEL_CNT*nChDataSize);
  Mem_Clr(g_pOutChData,sizeof(g_pOutChData));
  g_pOutChDataArray[0]=g_pOutChData;
  
  for(re=1;re<_MAX_SIGNAL_CHANNEL_CNT;re++)
  {
    p=(INT8S *)g_pOutChDataArray[re-1];
    g_pOutChDataArray[re]=(struct tagChData2060 *)(p+nChDataSize);
  }
  return re;
}

INT32S Simu_BuildChSpeed()//根据瞬态计划创建各个键项的按时间顺序的转速数组
{
  INT32S re=0;
  INT32S nKeyChindex=0;
  INT32S i=0,j=0;
  //_log("Start Build Speed Array\n");
  for(i=0;i<g_nKeycnt;i++)
  {
    INT32S nEventIndex=0;
    //if(g_nKeySpeed[i])
    //  free(g_nKeySpeed[i]);
    //g_nKeySpeed[i]=malloc(sizeof(INT32S)*g_nAllcnt);	
    g_nKeySpeed[nKeyChindex][0]=g_nStartRpm;
    for(j=1;j<g_nAllcnt;j++)
    {
      if(j<g_TranNode[i].nTigerTime[nEventIndex*2])
      {
        g_nKeySpeed[nKeyChindex][j]=g_nKeySpeed[nKeyChindex][j-1];
        
      }else if(j>=g_TranNode[i].nTigerTime[nEventIndex*2]&&j<=g_TranNode[i].nTigerTime[nEventIndex*2+1])
      {
        g_nKeySpeed[nKeyChindex][j]=g_nKeySpeed[nKeyChindex][g_TranNode[i].nTigerTime[nEventIndex*2]-1]+\
          1.0*(j-g_TranNode[i].nTigerTime[nEventIndex*2])*\
            g_TranNode[i].fDeltaRPM[nEventIndex]/(g_TranNode[i].nTigerTime[nEventIndex*2+1]-g_TranNode[i].nTigerTime[nEventIndex*2]);
        if(j==g_TranNode[i].nTigerTime[nEventIndex*2+1])
        {
          nEventIndex++;
        }
      }else
        g_nKeySpeed[nKeyChindex][j]=g_nKeySpeed[nKeyChindex][j-1];
    }
    nKeyChindex++;
  }
  //_log("End Build Speed Array\n");
  return re;
}

INT32S Simu_Build_AlertPlan()//创建报警计划表
{
  INT32S i=0,j=0,nJKeycnt=7;
  INT32S nTrigerTime[]={50,100,140,340,440,670,740};//报警时间
  INT32S nChcnt[]={10,10,10,10,10,10,10};//报警的通道数
  INT32S nChStart[]={0,10,20,30,20,30,0};//起始通道号
  //INT32S nChStart[]={0,0,0,0,0,0,0};//起始通道号
  FP32	fCof[]={1.3,1.5,1.7,1.9,1.2,1.9,1.44};//报警值对于标准测量值的倍数
  //_log("Start Build Alert Plan\n");
  g_nAlertcnt=nJKeycnt;
  for(j=0;j<g_nAlertcnt;j++)
  {	
    g_AlertNode[j].nTigerTime = nTrigerTime[j];//报警时间
    g_AlertNode[j].nChcnt = nChcnt[j];//报警通道熟；iang 
    
    for(i=0;i<g_AlertNode[j].nChcnt;i++)
    {
      g_AlertNode[j].nChNo[i] = i+nChStart[j];//报警通道列表
      g_AlertNode[j].fCof[i]  = fCof[j];	//报警值
    }
  }
  //_log("End Build Alert Plan\n");
  return i;
}

INT32S Simu_Build_TranPlan()//创建瞬态计划
{
  INT32S i=0,j=0,nKeycnt=4;
  
  INT32S nDeltaRPM[][5]={
    {600,600,600,600,600},
    {600,600,600,600,600},
    {600,600,600,600,600},
    {600,600,600,600,600},
  };
  //	INT32S nEventcnt[]={5,5,5,5};//瞬态事件数量
  INT32S nEventcnt[]={3,3,3,3};//瞬态事件数量
  INT32S nChNo[]={48,49,50,51};
  
  //_log("Start Build Tran Plan\n");
  for(j=0;j<nKeycnt;j++)
  {	
    g_TranNode[j].nEventcnt = nEventcnt[j];
    g_TranNode[j].nChNo = nChNo[j];
    
    for(i=0;i<g_TranNode[j].nEventcnt;i++)
    {
      g_TranNode[j].nTigerTime[2*i] = g_nTigerTime[j][2*i];
      g_TranNode[j].nTigerTime[2*i+1] = g_nTigerTime[j][2*i+1];
      g_TranNode[j].fDeltaRPM[i]  = nDeltaRPM[j][i];	
    }
  }
  //_log("End Build Tran Plan\n");
  return i;
}

INT32S BuildBasicSimuData()//基础信号，同步采集点数量为256，基频50%，倍频30%，次频%20
{
  INT32S re=0,step;
  
  FP32 fv,fvmin=1000,fvmax=-1000;
  //_log("Start Build Basic SimuData\n");
  for(step=0;step<g_nDatalth;step++)
  {
    fv=sin(3.1416*step/g_nSyccnt)*g_fLevel*g_fCof1x+
      sin(6.2832*step/g_nSyccnt)*g_fLevel*g_fCof2x+
        sin(1.5708*step/g_nSyccnt)*g_fLevel*g_fCofP5x;
    fv/=2;
    if(fv>fvmax)
      fvmax=fv;
    if(fv<fvmin)
      fvmin=fv;
  }
  g_fNormalAlp=fvmax-fvmin;
  g_BasicValue.n05xP=0;
  g_BasicValue.n05xV=20;
  g_BasicValue.n1xP=0;
  g_BasicValue.n1xV=50;
  g_BasicValue.n2xV=30;
  g_BasicValue.n2xP=0;
  g_BasicValue.nAllMeasureValue=fvmax-fvmin;
  g_BasicValue.nAllPhase=0;
  g_BasicValue.nAlertFlag=0;
  g_BasicValue.nChNo=-1;
  g_BasicValue.nRPM=-1;
  g_BasicValue.nRpmFlag=0;
  g_BasicValue.nTrigerTime=-1;
  //_log("End Build Basic SimuData\n");
  return re;
}

INT32S Simu_BulidAlert()
{
  INT32S re=0,i,j;
  INT32S ccc=0;
  char p0[48*7*sizeof(struct tagChValue)];
  struct tagChValue* p;
  //_log("Start Build Alert SimuData\n");
  g_nSimuAlertMeasureCnt=g_nChcnt*g_nAlertcnt;
  
  //if(g_pSimuAlertMeasureValue)
  //  free(g_pSimuAlertMeasureValue);
  g_pSimuAlertMeasureValue=0;
  g_pSimuAlertMeasureValue=(struct tagChValue*)p0;
  p=g_pSimuAlertMeasureValue;
  
  for(i=0;i<g_nAlertcnt;i++)
  {
    ccc=0;
    for(j=0;j<g_nChcnt;j++)
    {
      *p=g_BasicValue;
      p->nTrigerTime=g_AlertNode[i].nTigerTime;//报警时间
      p->nChNo=j;//通道号
      if(j==g_AlertNode[i].nChNo[ccc]&&ccc<g_AlertNode[i].nChcnt)
      {
        p->nAlertFlag=1;//修改测量值
        p->nAllMeasureValue=g_fNormalAlp*g_AlertNode[i].fCof[ccc];
        p->n1xV=g_fLevel*g_fCof1x*g_AlertNode[i].fCof[ccc];
        p->n2xV=g_fLevel*g_fCof2x*g_AlertNode[i].fCof[ccc];
        p->n05xV=g_fLevel*g_fCofP5x*g_AlertNode[i].fCof[ccc];	
        ccc++;	
      }
      p++;					
    }
  }
  //_log("End Build Alert SimuData\n");
  return re;
}


INT32S Simu_BulidTran()//为各个转速创建原始数据，原始数据长度固定  
{
  INT32S re=0;
  //INT32S nOldrpm;
  INT32S l;
  INT32S nDatalth=sizeof(struct tagChData2060)+((_MAX_KEY_PT_CNT+g_nDatalth-1)*sizeof(INT32S));
  struct tagChData2060* p1;
  INT32S len = 3000*1.2*(sizeof(struct tagChData2060)+((_MAX_KEY_PT_CNT+_DEFAULT_DATA_LTH-1)*sizeof(INT32S)));
  INT8S *p = Tran_buffer;
  INT8S* p8=NULL;
  Mem_Clr(p,len);
  g_pSimuTranPiece=(struct tagChData2060*)p;
  p1=(struct tagChData2060 *)(((INT8S*)g_pSimuTranPiece)+g_nStartRpm*nDatalth);
  
  //nOldrpm=g_nStartRpm;
  
  for(l=g_nStartRpm;l<g_nBasicRPM*g_fMaxRpmCof-1;l++)
  {
    INT32S k;
    INT32S step;
    INT32S* p32s=&p1->nKeycnt+1;
    p1->nKeycnt=l*0.005208;
    p1->nPiecelen=g_nDatalth;
    p1->nFrequency=g_fPickFrequency;
    for(k=0;k<p1->nKeycnt;k++)
    {
      p32s[k]=k*g_nDatalth/p1->nKeycnt+781.25/p1->nKeycnt;
    }
    
    p32s+=p1->nKeycnt;

    for(step=0;step<g_nDatalth;step++)
    {
      FP32 v=0.000005236*l*step;
      p32s[step]=(sin(v)*g_fCof1x+sin(v*2)*g_fCof2x+sin(v/2)*g_fCofP5x)*g_fLevel;
      //p32s[step] = v*g_fCof1x;
      p32s[step]>>=2; 
    }					

    p8=(INT8S *)p1;
    p1=(struct tagChData2060 *)(p8+nDatalth);
  }	
  //_log("End Build Tran SimuData\n");
  return re;
}

INT32S CheckSimuData()
{
  INT32S re=0;
  INT32S l;
  INT32S nChDataSize=sizeof(struct tagChData2060)+(_MAX_KEY_PT_CNT+g_nDatalth-1)*sizeof(INT32S);
  struct tagChData2060*  pchData=0;
  for(l=g_nStartRpm;l<g_nBasicRPM*g_fMaxRpmCof-1;l++)
  {
    pchData=(struct tagChData2060 *)(((INT8S*)g_pSimuTranPiece)+l*nChDataSize);
    if(pchData->nTrigerTime!=0||pchData->nChNo!=0||pchData->nPiecelen!=_DEFAULT_DATA_LTH||
       pchData->nFrequency!=_12_CHANNEL_SAMPLE_RATE||pchData->nKeycnt>_MAX_TRAN_KEY_POINT_CNT)
      pchData->nTrigerTime=pchData->nTrigerTime;
  }
  return re;
}

INT32S Simu_Build()
{
  Simu_BuildRamDisk();
  BuildBasicSimuData();
  Simu_Build_TranPlan();
  Simu_BuildChSpeed();
  Simu_BulidTran();
  Simu_Build_AlertPlan();
  Simu_BulidAlert();
  return 0;
}

INT32S tttttt[][10]={
  {65,305,1600,1900,2700,3000,3800,4100,4900,5200},//虚拟设备的瞬态计划
  {850,1150,1950,2250,3050,3350,4150,4450,5250,5550},
  {1200,1500,2300,2600,3400,3700,4500,4800,5600,5900},
  {600,900,1700,2000,2800,3100,3900,4200,5000,5300},
};

INT32S WriteRamDisk(INT32U nOffset,INT32S pData[],INT32S nLength)
{
  INT32S re=0;
  memcpy(g_pChData+nOffset,pData,nLength << 2/**sizeof(INT32S)*/);
  return re;
}

INT32U g_nDynamicStart[_MAX_SIGNAL_CHANNEL_CNT]={0};
INT32U g_nTranStart[_MAX_SIGNAL_CHANNEL_CNT]={0};
static INT32S  g_nKeyChannSit[]={0,10,20,30};
INT32S  g_nDynamicResult[_DEFAULT_DATA_LTH]={0};
INT32S  g_nTranResult[_DEFAULT_DATA_LTH]={0};
INT32S  g_nKeyResult[_MAX_KEY_PT_CNT]={0};

INT32S	Simu_Write_ChData(INT32S nLength[],INT32S nChNo[],INT32S nCnt,
                          INT32U nDynamicChDataOffset[],INT32U nDynamicKeyOffset[],INT32U nDynamicStep[],
                          INT32U nTranChDataOffset[],INT32U nTranKeyOffset[],INT32U nTranStep[])
{
  INT32S re=0;
  INT32S i=0,j,nLth,nSit;
  //INT8S* pData=(INT8S*)g_pOutChData;
  //INT32S nChDataSize=sizeof(struct tagChData2060)+(_MAX_KEY_PT_CNT+g_nDatalth-1)*sizeof(INT32S);
  INT32S noKey[]={0,0,0,0};
  
  for(i=0;i<_MAX_SIGNAL_CHANNEL_CNT;i++)
  {
    if(nChNo[re]==i)
    {	
      INT8S* p=(INT8S *)g_pOutChDataArray[i];
      INT32S* pChData=(INT32S *)(p+sizeof(struct tagChData2060)+((_MAX_KEY_PT_CNT-1)<<2)/**sizeof(INT32S)*/);
      memset(g_nDynamicResult,0,sizeof(g_nDynamicResult));
      nLth=(g_nDynamicStart[i]+_DEFAULT_DATA_LTH)/nDynamicStep[i];
      g_nDynamicResult[0]=nLth;
      nSit=g_nDynamicStart[i];
      for(j=1;j<nLth+1;j++)
      {
        g_nDynamicResult[j]=pChData[nSit];
        nSit+=nDynamicStep[i];
      }
      WriteRamDisk(nDynamicChDataOffset[i],g_nDynamicResult,nLth+1);
      g_nDynamicStart[i]=(g_nDynamicStart[i]+_DEFAULT_DATA_LTH)%nDynamicStep[i];
      
      memset(g_nTranResult,0,sizeof(g_nTranResult));
      nLth=(g_nTranStart[i]+_DEFAULT_DATA_LTH)/nTranStep[i];
      g_nTranResult[0]=nLth;
      nSit=g_nTranStart[i];
      for(j=1;j<nLth+1;j++)
      {
        g_nTranResult[j]=pChData[nSit];
        nSit+=nTranStep[i];
      }
      WriteRamDisk(nTranChDataOffset[i],g_nTranResult,nLth+1);
      g_nTranStart[i]=(g_nTranStart[i]+_DEFAULT_DATA_LTH)%nTranStep[i];
      re++;
      if(re==nCnt)
        break;
    }
  }
  
  for(i=0;i<_MAX_JKEY_CHANNEL_CNT;i++)
  {
    if(g_pOutChDataArray[g_nKeyChannSit[i]]->nKeycnt)
    {
      nLth=g_pOutChDataArray[g_nKeyChannSit[i]]->nKeycnt;
      g_nKeyResult[0]=nLth;
      nSit=g_nDynamicStart[i];
      memcpy(g_nKeyResult+1,g_pOutChDataArray[g_nKeyChannSit[i]]->pData,nLth << 2/**sizeof(INT32S)*/);
      for(j=1;j<nLth+1;j++)
      {
        g_nKeyResult[j]/=nDynamicStep[i];
      }
      WriteRamDisk(nDynamicKeyOffset[i],g_nKeyResult,nLth+1);
      memcpy(g_nKeyResult+1,g_pOutChDataArray[g_nKeyChannSit[i]]->pData,nLth << 2/**sizeof(INT32S)*/);
      for(j=1;j<nLth+1;j++)
      {
        g_nKeyResult[j]/=nTranStep[i];
      }
      WriteRamDisk(nTranKeyOffset[i],g_nKeyResult,nLth+1);
    }else
    {
      WriteRamDisk(nDynamicKeyOffset[i],noKey, 4/*sizeof(noKey)/sizeof(INT32S)*/);
      WriteRamDisk(nTranKeyOffset[i],noKey,4/*sizeof(noKey)/sizeof(INT32S)*/);
    }
  }
  
  return re;
}

INT32S	OutputChValue()
{
  INT32S re=0;
  Tst_Head_DCM_SigModuSampData_SYS* p=g_TempValue;
  int i=0;
  struct tagChValue* pv=g_pOutChValue;		
  for(i=0;i<_MAX_SIGNAL_CHANNEL_CNT;i++)
  {
    p[i].tunStaticFlag1.tstVal.unFlag_RUN=1;
    p[i].tunChInfo.tstVal.unModuleType=20612;
    p[i].tunChInfo.tstVal.tunChNo.unVal_PT2060=pv[i].nChNo;      // 通道信息
    
    p[i].ulnWaveDataLen=g_nDatalth;         //波形数据长度。 0 – 波形数据无效；其它 – 有效值
    
    p[i].ulnTmStamp_SYN1=pv[i].nTrigerTime;         // 数据采样同步信号SYN1时间戳
    p[i].ulnTmStamp_LastWaveData=pv[i].nTrigerTime;  // 最后1个波形数据时间戳
    p[i].ulnWaveDataSampFreq=pv[i].nTrigerTime;		// 波形数据采样频率。高17位为整数，低15位为小数，单位Hz
    
    p[i].ulnWaveDataUnitage=1;		    // 波形数据单位量。单位：nV
    p[i].tunDirect.nVal=pv[i].nAllMeasureValue;
    p[i].unNoT1X=pv[i].n05xV;
    p[i].un1XP=pv[i].n1xP;
    p[i].un1XA=pv[i].n1xV;
    p[i].un2XP=pv[i].n2xP;
    p[i].un2XA=pv[i].n2xV;
    p[i].tunStaticFlag1.tstVal.unFlag_Alert=(pv[i].nAlertFlag==1);
    p[i].tunStaticFlag1.tstVal.unFlag_Danger=(pv[i].nAlertFlag==2);
    p[i].ulnRPM=pv[i].nRPM;
  }
  
  return re;
}


INT32S Simu_Send_Data(void *ps)
{
  INT32S re=0,i,j,k,l,m,nKeyNo;
  INT32S nChDataSize=sizeof(struct tagChData2060)+(_MAX_KEY_PT_CNT+g_nDatalth-1)*sizeof(INT32S);
  float fSign=0;
  float fSignHelp = 0;
  //char inf[256]={0};
  //float fv[50];
  //INT8S* pData = (INT8S *)g_pOutChData;
  struct tagChData2060*  pchData=0;
  
  INT32S nCurSit[_MAX_JKEY_CHANNEL_CNT]={0};
  INT32S nSpeed[_MAX_JKEY_CHANNEL_CNT];
  //INT32S nLth[_MAX_JKEY_CHANNEL_CNT];
  
  PAlertNode pAlert=g_AlertNode;
  
  struct tagChValue *pAv=g_pSimuAlertMeasureValue;	
  
  //PTranNode pTran=g_TranNode;
  struct tagChValue chValue;
  
  INT32S nRpmFlag=0;
  
  pchData=(struct tagChData2060*)Send_Data_buffer;
  
  chValue.nTrigerTime=0;
  chValue.nChNo=0;
  chValue.nRPM=g_nBasicRPM;
  chValue.nAlertFlag=0;
  chValue.nAllMeasureValue=g_fNormalAlp;
  chValue.nAllPhase=0;
  chValue.n1xV=g_fLevel*g_fCof1x;
  chValue.n1xP=0;
  chValue.n2xV=g_fLevel*g_fCof2x;
  chValue.n2xP=0;
  chValue.n05xV=g_fLevel*g_fCofP5x;
  chValue.n05xP=0;
  chValue.nRpmFlag=0;
  i=0;
  do{
    //_log("Start Send SimuData\n");
    for(m=0;;m++)
    {		
      INT32S nChIndex=0;
      if(g_nChValueFlag!=3||g_nChDataFlag!=3)
      {
        //OSTimeDly(g_SendTimeInterval/(1000/OS_TICKS_PER_SEC));
        OSTimeDly(1);
        continue;
      }
      //OSSchedLock();//禁止任务切换 
      g_nChValueFlag=0;
      g_nChDataFlag=0;
      fSignHelp = 1+0.3*sin(0.62832*m);
      for(j=0;j<g_nChcnt;j++)
      {
        //fSign=(g_nSit[j]*48.0/g_nChcnt-24.0)*(1+0.3*sin(6.2832*(m/10.)));
        fSign = (g_nSit[j]-24.0)*fSignHelp;
        if(g_nChKeyNo[g_nSit[j]]>=_START_KEY_CH)
        {
          nKeyNo=g_nChKeyNo[g_nSit[j]]-_START_KEY_CH;
          nSpeed[nKeyNo]=g_nKeySpeed[nKeyNo][i];
          g_nCaltKeySpeed[nKeyNo]=nSpeed[nKeyNo];
        }
        else
          nSpeed[nKeyNo]=-1;
        nRpmFlag=0;
        for(k=0;k<g_TranNode[nKeyNo].nEventcnt;k++)
        {
          if(i>= g_TranNode[nKeyNo].nTigerTime[k<<1]&&i<= g_TranNode[nKeyNo].nTigerTime[(k<<1)+1])
          {
            nRpmFlag=1;
            break;
          }
        }
        //CheckSimuData();

        if(i==pAlert->nTigerTime)//报警时间
        {
          pAv->nRPM=nSpeed[nKeyNo];
          pAv->nRpmFlag=nRpmFlag;
          //////输出报警通道测量值pAv
          //g_pOutChValue=pAv;
          memcpy(g_pOutChValue+g_nSit[j],pAv,sizeof(struct tagChValue));
          pAv++;
          
        }
        else//非报警通道
        {
          chValue.nTrigerTime=i;
          chValue.nChNo=g_nSit[j];
          chValue.nRPM=nSpeed[nKeyNo];
          chValue.nRpmFlag=nRpmFlag;
          //g_pOutChValue=&chValue;
          memcpy(g_pOutChValue+g_nSit[j],&chValue,sizeof(struct tagChValue));
          g_pOutChValue[g_nSit[j]].nAllMeasureValue*=(100+fSign)/100.;
          g_pOutChValue[g_nSit[j]].n1xV*=(100+fSign)/100.;
          g_pOutChValue[g_nSit[j]].n2xV*=(100+fSign)/100.;
          g_pOutChValue[g_nSit[j]].n05xV*=(100+fSign)/100.;
          //fv[g_nSit[j]]=g_pOutChValue[g_nSit[j]].nAllMeasureValue;
          g_nChValueFlag=1;
          
          //输出正常通道测量值chValue
        }
        memcpy(pchData,((INT8S*)g_pSimuTranPiece)+nSpeed[nKeyNo]*nChDataSize,nChDataSize);
        //	CheckSimuData();
        // nLth[nKeyNo]=nChDataSize-(_MAX_KEY_PT_CNT-pchData->nKeycnt)*sizeof(INT32S);
        pchData->nTrigerTime=i;
        pchData->nChNo=g_nSit[j];
        
        //if(1 /* czx i==pAlert->nTigerTime&&g_nSit[j]==pAlert->nChNo[nChIndex]*/)//报警通道
        INT32S step;
        INT32S* p32s=(&pchData->nKeycnt)+1;
        p32s+=pchData->nKeycnt;
        if((i==pAlert->nTigerTime) && (g_nSit[j]==pAlert->nChNo[nChIndex]))
        {
          for(step=0;step<g_nDatalth;step++)//修改报警通道原始数据幅值
          {
            p32s[step]=p32s[step]*pAlert->fCof[nChIndex]*g_12Param.SignalChannel[g_nSit[j]].fSensority;
          }					
          nChIndex++;
        }
        else
        {
          //if(g_nSit[j]/2*2==g_nSit[j])
          if((g_nSit[j]^(g_nSit[j]-1)) != 1)
          {
            for(step=0;step<g_nDatalth;step++)//修改报警通道原始数据幅值
            {
              p32s[step]*=(100+fSign)/100.*g_12Param.SignalChannel[g_nSit[j]].fSensority;
              //p32s[step] = 0;
            }
          }
          else
          {
            for(step=0;step<g_nDatalth;step++)//修改报警通道原始数据幅值
            {
              p32s[step]*=(100-fSign)/100.*g_12Param.SignalChannel[g_nSit[j]].fSensority;
              //p32s[step] = 0;
            }
          }				
          nChIndex++;
        }
        memcpy(g_pOutChDataArray[g_nSit[j]],pchData,nChDataSize);
      }

      //输出所有通道数据头
      OutputChValue();	
      //输出所有通道原始数据

      Simu_Write_ChData(g_nLength,g_nChNo,_MAX_SIGNAL_CHANNEL_CNT,
                        g_nFpgaDynamicChDataOffset,g_nFpgaDynamicKeyDataOffset,g_nFpgaDynamicChDataSampleStep,
                        g_nFpgaTranDataOffset,g_nFpgaTranKeyDataOffset,g_nFpgaTranDataSampleStep);

      for(l=0;l<8 /*_MAX_SIGNAL_CHANNEL_CNT/_FPGA_GROUP_CHANNEL_CNT*/;l++) {
        OSSchedLock();
        receive_channel_data(0);
        OSSchedUnlock();
      }
      g_nChValueFlag=3;
      g_nChDataFlag=3;
      //OSTimeDly(g_SendTimeInterval/(1000/OS_TICKS_PER_SEC)/(_MAX_SIGNAL_CHANNEL_CNT/_FPGA_GROUP_CHANNEL_CNT));
      if(i>=pAlert->nTigerTime)
        pAlert++;
      //	_log("Deal Speed Too Slow,Data Will lose\n");
      g_bDataValid=1;
      
      for(l=0;l<_MAX_JKEY_CHANNEL_CNT;l++)
      {
        if(i==g_nTigerTime[l][nCurSit[l]])
        {
          nCurSit[l]++;
        }
      }
      i++;
      if(i==g_nAllcnt)
      {
        i=0;
        break;
      } 
      //OSSchedUnlock();//允许任务切换 
      //CheckSimuData();
      //printf("Simu_Send_Data\n");
      OSTimeDly(10);
    }
    //_log("End Send SimuData\n");
    OSTimeDly(20);
  }while(1);
  //free(p);	
  return re;
}
