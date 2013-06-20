#include "includes.h"
#include "define2060.h"
#include "interface.h"
#include <math.h>
extern INT32U				g_nStartTime = 0;
extern INT32U				g_nTickCount = 0;

extern Param_Table_12		g_12Param;//12模块参数,从12模块读取,不保存,由12congfig模块配置,360系统不可修改
extern Param_Table_83		g_83param;//83模块参数,存储在flash,可从上位机更新
//Run_Time_Flag				g_RunTimeFlag;//83运行状态信息

/*三种数据以所绑定的键相号作为内存划分的基础，从外到里按照键相号-触发时间-虚拟通道序号组织*/
/*提前计算出静态数据按照键相号-触发次序-虚拟通道序号的指针列表,用于快速从信号处理模块快速接收数据*/
/*静态和动态数据以报警时间作为一次采集,虚拟间数据以虚拟通道序号排序*/


/*静态数据所耗内存很小，采用静态分配，静态数据主要以报警方式触发，特定时间戳的数据到达时，一个通道报警，
与其同键相的通道一起进行采集,采集完成后所属键相的静态触发序号加1*/
extern Tst_Head_DCM_SigModuSampData_SYS		g_StaticOriginData[_MAX_STATIC_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT];//静态测量值队列
//根据12模块的键相-通道邦定列表创建,用来快速检索静态数据
extern Tst_Head_DCM_SigModuSampData_SYS*		g_pStaticChannelOrigin[_MAX_JKEY_CHANNEL_CNT][_MAX_STATIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
//extern Channel_Static_Value					g_tmpStaticData[_MAX_SIGNAL_CHANNEL_CNT];
extern Static_Triger_Table					g_StaticTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];


/*动态数据所耗内存较大，采用动态分配，动态数据主要以报警方式触发，特定时间戳的数据到达时，一个通道报警，
与其同键相的通道一起进行采集，每个时间戳下采集动态数据的通道数量是不同的,采集完成后所属键相的动态触发序号加1*/
extern Tst_Head_DCM_SigModuSampData_SYS		g_DynamicValue[_MAX_DYNAMIC_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT];//动态态测量值队列
//根据12模块的键相-通道邦定列表创建,用来快速检索动态数据头
extern Tst_Head_DCM_SigModuSampData_SYS*		g_pDynamicChannelValue[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
extern Dynamic_Triger_Table					g_DynamicTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];

//动态通道数据偏移表,初始化时生成,同于指示电子盘通道写入地址,也用于数据向上位机发送时快速定位数据在电子盘的位置
extern INT32U									g_nChannel_Dynamic_Wave_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
extern INT32U									g_nChannel_Dynamic_Key_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH];

/*瞬态数据所耗内存很大,采用动态分配,根据虚拟设备划分为不同设备数据区域,瞬态数据队列长度最大不超过_MAX_TRAN_ARRAY_LTH，
最少为1，称为组数，设备数据以组号为依据划分为同等大小的区域,组内再按照通道序号细分,通道内部以触发次序再次细分
*/
extern Tst_Head_DCM_SigModuSampData_SYS		g_TranValue[_MAX_TRAN_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT*_MAX_TRAN_GROUP];//瞬态测量值队列
//根据12模块的键相-通道邦定列表创建,用来快速检索瞬态态数据头
extern Tst_Head_DCM_SigModuSampData_SYS*		g_pTranChannelValue[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT][_MAX_TRAN_GROUP];
extern Tran_Triger_Table						g_TranTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];
//瞬态通道数据偏移表,初始化时生成,同于指示电子盘通道写入地址,也用于数据向上位机发送时快速定位数据在电子盘的位置
extern INT32U		g_nChannel_Tran_Wave_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT][_MAX_TRAN_GROUP];
extern INT32U		g_nChannel_Tran_Key_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_TRAN_GROUP];


extern INT32S		g_nStaticManualPick[_MAX_JKEY_CHANNEL_CNT];
extern INT32S		g_nDynamicManualPick[_MAX_JKEY_CHANNEL_CNT];
extern INT32S		g_nTranManualPick[_MAX_JKEY_CHANNEL_CNT];//不用,保留

INT32U				g_nIndexInGroupSequency = 0;
INT32U				g_DynamicSampleStepDuration[_MAX_JKEY_CHANNEL_CNT] = {0};

//Data_Limit_83				g_83DataLimit;//83模块的一些极限参数
extern INT32S				g_nChannelArrivalSequenceTable[_MAX_SIGNAL_CHANNEL_CNT];//到达通道次12通道序号表
extern INT32S				g_nArrivalDeviceNo[_MAX_SIGNAL_CHANNEL_CNT];//到达通道虚拟设备表
extern INT32S				g_nArrivalChannelNo[_MAX_SIGNAL_CHANNEL_CNT];//通道数据到达在所属设备的通道号

//INT32U		g_Signal_Channel_Dynamic_Write_Position[_MAX_SIGNAL_CHANNEL_CNT];//各个通道的动态数据写入位置
//INT32U		g_Signal_Channel_Tran_Write_Position[_MAX_SIGNAL_CHANNEL_CNT];//各个通道的瞬态数据写入位置
//INT32U		g_Key_Channel_Dynamic_Write_Position[_MAX_JKEY_CHANNEL_CNT];//各个通道的动态键相数据写入位置
//INT32U		g_Key_Channel_Tran_Write_Position[_MAX_JKEY_CHANNEL_CNT];//各个通道的瞬态键相数据写入位置

INT32U		g_nArmDynamicChDataOffset[_MAX_SIGNAL_CHANNEL_CNT*2+_MAX_JKEY_CHANNEL_CNT*2];
INT32U*		g_nArmDynamicKeyDataOffset = g_nArmDynamicChDataOffset+_MAX_SIGNAL_CHANNEL_CNT;
INT32U*		g_nArmTranChDataOffset = g_nArmDynamicChDataOffset+_MAX_SIGNAL_CHANNEL_CNT+_MAX_JKEY_CHANNEL_CNT;
INT32U*		g_nArmTranKeyDataOffset = g_nArmDynamicChDataOffset+_MAX_SIGNAL_CHANNEL_CNT*2+_MAX_JKEY_CHANNEL_CNT;
INT32S		g_nGroupCnt = _MAX_SIGNAL_CHANNEL_CNT/_FPGA_GROUP_CHANNEL_CNT;

INT32U		g_nArmDynamicChDataSampleStep[_MAX_SIGNAL_CHANNEL_CNT*2];
INT32U*		g_nArmTranChDataSampleStep=g_nArmDynamicChDataSampleStep+_MAX_SIGNAL_CHANNEL_CNT;

INT32U		g_nTickTime = 0;
//INT32S		g_nFPGATickcnt = 0;
/*manual*///修改键相数量和数据抽取步长时下面全局变量的初始化需要手工修改
Key_Speed_Calt	g_DynamicSpeedCalt[_MAX_JKEY_CHANNEL_CNT];//各个键相速度动态计算结构
static Key_Speed_Calt	g_TranSpeedCalt[_MAX_JKEY_CHANNEL_CNT];//各个键相速度瞬态计算结构

static const INT32U g_nArmDataOffsetInvalid[_MAX_SIGNAL_CHANNEL_CNT*2+_MAX_JKEY_CHANNEL_CNT*2]={0};//无效电子盘偏移地址
static INT32S g_nKeyValidFlag[_MAX_JKEY_CHANNEL_CNT] = {1,1,1,1};//本次键相数据是否有效
static INT32S g_nKeyStaticTrigerFlag[_MAX_JKEY_CHANNEL_CNT] = {0};//本次各个键相是否触发静态报警采集
static INT32S g_nKeyDynamicTrigerFlag[_MAX_JKEY_CHANNEL_CNT] = {0};//本次各个键相是否触发静态报警采集
static INT32S g_nCurSpeed[]={0,1,2,3};
static INT32S g_nSpeedArrayIndex[4]={0};
static INT32U g_nStaticTickCount[4]  = {0};
static INT32U g_nDynamicTickCount[4] = {0};

void receive_channel_data(void);
void InitInterfaceAddress(void);
void InitInterfaceAddress()
{	
  INT32S re = 0,i,j,l,nOffsetJKey = 0,nOffsetTriger = 0,nOffsetChannel = 0,nOffsetGroup = 0;
  INT8S* p = 0;
  FP32   fValue,fValue1;//
  for(i = 0;i<_MAX_SIGNAL_CHANNEL_CNT;i++)//初始化电子盘信号通道数据写入偏移表和通道抽取因子
  {
    nOffsetJKey = g_12Param.nChannelNumIn83[i*2];//所属虚拟设备号
    nOffsetChannel = g_12Param.nChannelNumIn83[i*2+1];//通道号
    g_nArmDynamicChDataOffset[i] = g_nChannel_Dynamic_Wave_Data_Offset[nOffsetJKey][0][nOffsetChannel];//动态起始偏移
    g_nArmTranChDataOffset[i] = g_nChannel_Tran_Wave_Data_Offset[nOffsetJKey][0][nOffsetChannel][0];//瞬态起始偏移
    g_nArmDynamicChDataSampleStep[i] = g_83param.Device[nOffsetJKey].nDynamicChannelDataSampleStep/*[nOffsetChannel]*/;//动态抽取因子
    g_nArmTranChDataSampleStep[i] = g_83param.Device[nOffsetJKey].nTranChannelDataSampleStep/*[nOffsetChannel]*/;//瞬态抽取因子
  }
  for(i = 0;i<_MAX_JKEY_CHANNEL_CNT;i++)
  {
    g_83param.Device[i].nChKeyBlockSize=_MAX_DYNAMIC_KEY_POINT_CNT*sizeof(INT32S)/_MAX_DYNAMIC_CHANNEL_DATA_BLOCK_CNT;
    g_nDynamicTickCount[i]=0;
    g_nStaticTickCount[i]=0;
    g_nArmDynamicKeyDataOffset[i] = g_nChannel_Dynamic_Key_Data_Offset[i][0];
    g_nArmTranKeyDataOffset[i] = g_nChannel_Tran_Key_Data_Offset[i][0][0];
  }
  for(i = 0;i<_MAX_JKEY_CHANNEL_CNT;i++)
  {
    g_DynamicSpeedCalt[i].nTrigerLevel=g_83param.Device[i].KeyChannel[0].nTrigerDRPM*_MAX_JKEY_SPEED_ARRAY_LTH;
    g_DynamicSpeedCalt[i].nSampleStep[0]=g_83param.Device[i].nDynamicChannelDataSampleStep;
    g_DynamicSpeedCalt[i].nSynBlockCnt[0]=g_83param.Device[i].nChDynamicDataBlockCnt;
    g_DynamicSpeedCalt[i].nSynBlockIncreaseStep[0]=1;
    for(j=1;j<g_83param.Device[i].nDynamicChannelDataSampleStep;j++)
    {
      g_DynamicSpeedCalt[i].nSampleStep[j]=g_83param.Device[i].nDynamicChannelDataSampleStep-j;
      g_DynamicSpeedCalt[i].nSynBlockIncreaseStep[j]=\
        (g_83param.Device[i].nDynamicChannelDataSampleStep-1)/g_DynamicSpeedCalt[i].nSampleStep[j]+1;
      g_DynamicSpeedCalt[i].nSynBlockCnt[j]=
        g_83param.Device[i].nDynamicChannelDataSampleStep/g_DynamicSpeedCalt[i].nSynBlockIncreaseStep[j];
    }
    g_TranTrigerEventArray[i].ulnPreRPM = 0;
  }

  //设置FPGA的通道数据起始偏移
  re = SetRamDiskChDataOffset(0,(_MAX_SIGNAL_CHANNEL_CNT+_MAX_JKEY_CHANNEL_CNT)*2,g_nArmDynamicChDataOffset);
  //设置FPGA的通道数据抽取因子
  re |= SetChDataSampleStep(0,_MAX_SIGNAL_CHANNEL_CNT*2,g_nArmDynamicChDataSampleStep);
}

void receive_channel_data()
{
  INT32S tmp,tmp1,tmp2,tmp3,nIndex,nDevice,nChannel,*p32S,*p32S1;	
  Dynamic_Channel_Data_table* pDCT;
  Tran_Channel_Data_table* pTCT;
  INT32U *p32U1;
  Tst_Head_DCM_SigModuSampData_SYS *pHS,*pHD,*pHT;
  INT32U nCurSpeed[]={0,1,2,3};INT32S nCurKeyPointCnt[]={0,1,2,3};
  INT32S nStaticTrigerWriteSit[] = //当前各个设备的数据头写入位置
  {
    g_StaticTrigerEventArray[0].ulnTrigerWrite,g_StaticTrigerEventArray[1].ulnTrigerWrite,
    g_StaticTrigerEventArray[2].ulnTrigerWrite,g_StaticTrigerEventArray[3].ulnTrigerWrite,
  };
  INT32S nDynamicTrigerWriteSit[] = 
  {
    g_DynamicTrigerEventArray[0].ulnTrigerWrite,g_DynamicTrigerEventArray[1].ulnTrigerWrite,
    g_DynamicTrigerEventArray[2].ulnTrigerWrite,g_DynamicTrigerEventArray[3].ulnTrigerWrite,
  };
  INT32S nTranTrigerWriteSit[] =
  {
    g_TranTrigerEventArray[0].ulnTrigerWrite,g_TranTrigerEventArray[1].ulnTrigerWrite,
    g_TranTrigerEventArray[2].ulnTrigerWrite,g_TranTrigerEventArray[3].ulnTrigerWrite,
  };
  /*获取瞬态触发队列当前组*/
  INT32S nTranCureWriteGroup[] = {
    g_TranTrigerEventArray[0].ulnCurWriteGroup,g_TranTrigerEventArray[1].ulnCurWriteGroup,
    g_TranTrigerEventArray[2].ulnCurWriteGroup,g_TranTrigerEventArray[3].ulnCurWriteGroup,
  };
  if(g_nIndexInGroupSequency == 0)//读键相速度，判断瞬态是否触发,要求g_nGroupCnt必须为2的指数倍/**/
  {
    ReadKeySpeed(_MAX_JKEY_CHANNEL_CNT,nCurSpeed,g_nCurSpeed);//读取所有键相通道速度
    for(nDevice = 0;nDevice<_MAX_JKEY_CHANNEL_CNT;nDevice++)//循环设备
    {
      g_nKeyValidFlag[nDevice]=1;//初始化通道标志和各个键相报警标志
      //			g_uKeyAlertFlag[nDevice] = 0;//本次各个键相合成报警状态
      tmp = g_TranTrigerEventArray[nDevice].nPreTranTickCnt;
      if(g_TranTrigerEventArray[nDevice].ulnTrigerFlag==0)//判断此键相是否触发瞬态采集
      {		
        tmp1 = g_DynamicSpeedCalt[nDevice].nPreAcumulateSpeed;
        tmp2 = g_DynamicSpeedCalt[nDevice].nTrigerLevel;
        tmp3 = g_nCurSpeed[nDevice]*_MAX_JKEY_SPEED_ARRAY_LTH;
        //速度队列数据充足并且满足转速触发条件			
        if((_MAX_JKEY_SPEED_ARRAY_LTH<=g_nTickCount)&&(tmp3-tmp1 >= tmp2 || tmp1-tmp3 >= tmp2)&&
          g_nCurSpeed[nDevice]>=g_83param.Device[nDevice].KeyChannel->fRPMLow&&
          g_nCurSpeed[nDevice]<=g_83param.Device[nDevice].KeyChannel->fRPMHigh)
        {
          g_TranTrigerEventArray[nDevice].ulnTrigerFlag = 1;//设置触发标志
          //本次瞬态数据起始偏移
          if(tmp+1>g_83param.Device[nDevice].nMinPreTranTickcnt)//触发前累积数量超过前触发组数
          {	//回退
            g_TranTrigerEventArray[nDevice].lnGroupHeadSit[nTranTrigerWriteSit[nDevice]] = \
              g_TranTrigerEventArray[nDevice].ulnCurWriteGroup-g_83param.Device[nDevice].nTranPreTrigerGroupcnt+1;
            if(g_TranTrigerEventArray[nDevice].lnGroupHeadSit[nTranTrigerWriteSit[nDevice]]<0)//小于0的驱魔处理
              g_TranTrigerEventArray[nDevice].lnGroupHeadSit[nTranTrigerWriteSit[nDevice]] += g_83param.Device[nDevice].nMaxTranSampleCnt;
            g_TranTrigerEventArray[nDevice].nPreTranTickCnt = g_83param.Device[nDevice].nTranPreTrigerGroupcnt;//置为前触发组数
          }
          else//触发前累积数量未达到前触发组数，全部保留
          {
            g_TranTrigerEventArray[nDevice].lnGroupHeadSit[nTranTrigerWriteSit[nDevice]] = 0;//前触发数据不足时，从头部开始
            g_TranTrigerEventArray[nDevice].nPreTranTickCnt = g_TranTrigerEventArray[nDevice].ulnCurWriteGroup+1;
          }

          g_TranTrigerEventArray[nDevice].ulnTrigerTime[nTranTrigerWriteSit[nDevice]] = g_nTickTime;//记录触发时间戳/**/
          g_TranTrigerEventArray[nDevice].ulnAccumulateCnt = 0;//时间抽取的累积数清零
          g_TranTrigerEventArray[nDevice].ulnPreRPM = 0;//前一组数据的转速
          g_TranTrigerEventArray[nDevice].ulnTrigerTimeSpan=0;//清空触发持续时间
        }else //未触发
        {	
          g_nKeyStaticTrigerFlag[nDevice] = 0;//本次各个键相是否触发静态报警采集
          g_nKeyDynamicTrigerFlag[nDevice]=0;
          if(g_83param.Device[nDevice].nDynamicSampleMode)//动态同步采集时执行
          {
            if(g_83param.Device[nDevice].nChDynamicDataBlockCnt!=g_DynamicSampleStepDuration[nDevice])//2s中判断一次
              g_DynamicSampleStepDuration[nDevice]++;
            else//变频处理
            {
              for(tmp3=0;tmp<_MAX_RPM_LEVEL;tmp3++)
              {
                if(g_nCurSpeed[nDevice]<=g_DynamicSpeedCalt[nDevice].nRPMLevel[tmp3])
                {
                  if(g_DynamicSpeedCalt[nDevice].nCurLevel==tmp3)//必定执行
                    break;
                  g_DynamicSpeedCalt[nDevice].nCurLevel=tmp3;//更新频阶
                  nIndex = g_DynamicTrigerEventArray[nDevice].ulnTrigerWrite;//取本键相的动态当前写入索引
                  pDCT = g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable+nIndex;
                  pDCT->ulnBlockCnt=0;
                  pDCT->lnCurBlock=0;
                  pDCT->lnHeadBlock=0;
                  p32S=g_83param.Device[nDevice].nChannelNumIn12;
                  tmp1=g_DynamicSpeedCalt[nDevice].nSampleStep[tmp3];
                  for(nChannel = g_83param.Device[nDevice].nSignChannelNum-1;nChannel>=0;nChannel--)//遍历虚拟设备各个通道
                  {
                    g_nArmDynamicChDataSampleStep[p32S[nChannel]]=tmp1;		
                  }
                  SetChDataSampleStep(0,_MAX_SIGNAL_CHANNEL_CNT, g_nArmDynamicChDataSampleStep);
                  break;
                }
              }
              g_DynamicSampleStepDuration[nDevice]=0;	
            }
          }
        }
      }
      tmp++;//激发数加1
      if(g_nTickCount<_MAX_JKEY_SPEED_ARRAY_LTH)//速度队列数据不足
      {
        g_DynamicSpeedCalt[nDevice].nPreAcumulateSpeed += g_nCurSpeed[nDevice];//累积新速度	
      }else
      {
        g_DynamicSpeedCalt[nDevice].nPreAcumulateSpeed += g_nCurSpeed[nDevice]
        -g_DynamicSpeedCalt[nDevice].nSpeedArray[g_nSpeedArrayIndex[nDevice]];//累积速度除旧更新
      }
      if(g_DynamicSpeedCalt[nDevice].nPreAcumulateSpeed>=120000)
        tmp=tmp;
      //速度队列当前写入位置
      g_DynamicSpeedCalt[nDevice].nSpeedArray[g_nSpeedArrayIndex[nDevice]] = g_nCurSpeed[nDevice];//更新速度
      g_nSpeedArrayIndex[nDevice]++;
      if(_MAX_JKEY_SPEED_ARRAY_LTH==g_nSpeedArrayIndex[nDevice])
        g_nSpeedArrayIndex[nDevice]=0;
      if(g_TranTrigerEventArray[nDevice].ulnTrigerFlag==0)
        g_TranTrigerEventArray[nDevice].nPreTranTickCnt = tmp;//更新激发数
    }
  }
  tmp1= g_nIndexInGroupSequency*_FPGA_GROUP_CHANNEL_CNT;
  for(nIndex = tmp1;nIndex<tmp1+_FPGA_GROUP_CHANNEL_CNT;nIndex++)//将数据移入瞬态数据头区域,并判断报警
  {
    nDevice = g_nArrivalDeviceNo[nIndex];//所属设备号
    if(g_nKeyValidFlag[nDevice] == 0)//通道无效时不处理
      continue;
    nChannel = g_nArrivalChannelNo[nIndex];
    pHT = g_pTranChannelValue[nDevice][nTranTrigerWriteSit[nDevice]][nChannel][nTranCureWriteGroup[nDevice]];//获取写入地址
    tmp = ReadChValue(pHT,g_83param.Device[nDevice].nChannelNumIn12[nChannel],1);//读取数据头
    pHT->ulnRPM=g_nCurSpeed[nDevice];//使用保留的
    g_nKeyValidFlag[nDevice] &= pHT->tunStaticFlag1.tstVal.unFlag_RUN;//键相通道数据是否有效，最大否决
    if(g_TranTrigerEventArray[nDevice].ulnTrigerFlag == 0&&g_nKeyValidFlag[nDevice])//瞬态未触发时检验是否有报警产生
    {
      tmp = pHT->tunStaticFlag1.tstVal.unFlag_Danger+pHT->tunStaticFlag1.tstVal.unFlag_Alert;//报警合成
      g_nKeyStaticTrigerFlag[nDevice] |= (tmp>g_TranTrigerEventArray[nDevice].ulnPreAlertFlag);//判断是否触发报警采集
      g_TranTrigerEventArray[nDevice].ulnPreAlertFlag = tmp;//下次中断时再用
    }
    *g_pStaticChannelOrigin[nDevice][nStaticTrigerWriteSit[nDevice]][nChannel]=*pHT;
    *g_pDynamicChannelValue[nDevice][nDynamicTrigerWriteSit[nDevice]][nChannel]=*pHT;
  }
  if(g_nIndexInGroupSequency != g_nGroupCnt-1)//通道没有全部到齐，后面的不处理
  {
    g_nIndexInGroupSequency++;
    return; 
  }
  ReadKeyPointCnt(_MAX_JKEY_CHANNEL_CNT,nCurKeyPointCnt,nCurKeyPointCnt);	
  for(nDevice = 0;nDevice<_MAX_JKEY_CHANNEL_CNT;nDevice++)//循环虚拟设备
  {
    if(g_nKeyValidFlag[nDevice])//设置瞬态触发表中获取前次各个键相的报警状态，键相有效时才执行
    {
      //填充瞬态缓冲键相点长度
      g_TranTrigerEventArray[nDevice].ChannelSignalDataTable[nTranTrigerWriteSit[nDevice]][nTranCureWriteGroup[nDevice]].KeyDataBlockLth[0]=
        nCurKeyPointCnt[nDevice];

      if(g_TranTrigerEventArray[nDevice].ulnTrigerFlag == 0)//未触发瞬态
      {
        //填充动态缓冲键相点长度
        tmp=g_DynamicTrigerEventArray[nDevice].ulnTrigerWrite;
        tmp2=g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[tmp].lnCurBlock;
        g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[tmp].KeyDataBlockLth[tmp2]=nCurKeyPointCnt[nDevice];

        //瞬态前触发累积g_83param.Device[nDevice].nRealPickTimeIntervalPre个后移1个
        if(g_83param.Device[nDevice].nRealPickTimeIntervalPre-1 == g_TranTrigerEventArray[nDevice].ulnAccumulateCnt)
        {
          //更新瞬态地址
          tmp2 = g_TranTrigerEventArray[nDevice].ulnCurWriteGroup+1;//计算下一组位置
          if(tmp2 == g_83param.Device[nDevice].nMaxTranSampleCnt)
            tmp2 = 0;
          tmp1=g_83param.Device[nDevice].nMaxTranSampleCnt;
          p32S=g_83param.Device[nDevice].nChannelNumIn12;
          nChannel = g_83param.Device[nDevice].nSignChannelNum-1;
          p32S1=g_nChannel_Tran_Wave_Data_Offset[nDevice][nTranTrigerWriteSit[nDevice]][nChannel]+tmp2;
          for(;nChannel>=0;nChannel--)//将数据移入瞬态数据头区域,并判断报警
          {
            //刷新瞬态信号通道数据写入地址
            g_nArmTranChDataOffset[p32S[nChannel]]=*p32S1;
            p32S1-=tmp1;
          }
          //刷新瞬态键相通道数据写入地址
          g_nArmTranKeyDataOffset[nDevice]=\
            g_nChannel_Tran_Key_Data_Offset[nDevice][nTranTrigerWriteSit[nDevice]][tmp2];

          g_TranTrigerEventArray[nDevice].ulnCurWriteGroup=tmp2;//切换到下一组
          g_TranTrigerEventArray[nDevice].ulnAccumulateCnt=0;
        }else
          g_TranTrigerEventArray[nDevice].ulnAccumulateCnt++;
      }
      //瞬态已经触发
      else
      {			
        tmp1 = nTranTrigerWriteSit[nDevice];//键相瞬态数据的当前写入位置
        nIndex = g_TranTrigerEventArray[nDevice].ulnTrigerWrite;//取本键相的瞬态当前写入索引
        pTCT = g_TranTrigerEventArray[nDevice].ChannelSignalDataTable[nIndex]+g_TranTrigerEventArray[nDevice].ulnCurWriteGroup;
        pTCT->ulnBlockCnt = 0;//复原，否则下次进入时此值仍保持
        tmp = g_TranTrigerEventArray[nDevice].ulnPreRPM;
        tmp1 = g_83param.Device[nDevice].nSampleRPMInterval;
        tmp2 = g_TranTrigerEventArray[nDevice].ulnTrigerTime[nTranTrigerWriteSit[nDevice]];
        tmp3 = g_nCurSpeed[nDevice];
        g_TranTrigerEventArray[nDevice].ulnPick=0;//默认时机未到
        if(g_83param.Device[nDevice].nTranSampleMode)//转速抽取
        {
          if(tmp-tmp3 >= tmp1 || tmp3-tmp >= tmp1)//间隔到了
          {
            g_TranTrigerEventArray[nDevice].ulnPick = 1;
            g_TranTrigerEventArray[nDevice].ulnPreRPM = tmp3;
          }
        }else//时间抽取
        {
          if(g_TranTrigerEventArray[nDevice].ulnAccumulateCnt == g_83param.Device[nDevice].nRealSampleTimeInterval-1)
          {
            g_TranTrigerEventArray[nDevice].ulnPick = 1;
            g_TranTrigerEventArray[nDevice].ulnAccumulateCnt = 0;
          }
          else
            g_TranTrigerEventArray[nDevice].ulnAccumulateCnt++;
        }

        if(g_TranTrigerEventArray[nDevice].ulnPick)//抽取本次数据
        {
          //更新瞬态地址
          tmp1=g_83param.Device[nDevice].nMaxTranSampleCnt;
          p32S=g_83param.Device[nDevice].nChannelNumIn12;
          nChannel = g_83param.Device[nDevice].nSignChannelNum-1;

          if(_MAX_TRAN_GROUP > g_TranTrigerEventArray[nDevice].nPreTranTickCnt&&//组数够了或时间到了
            g_TranTrigerEventArray[nDevice].ulnTrigerTimeSpan < g_83param.Device[nDevice].nMaxTranSampleTime)//该键相瞬态采集结束
          {
            tmp2 = g_TranTrigerEventArray[nDevice].ulnCurWriteGroup+1;//计算下一组位置
            if(tmp2 == g_83param.Device[nDevice].nMaxTranSampleCnt)
              tmp2 = 0;

            p32U1=g_nChannel_Tran_Wave_Data_Offset[nDevice][nTranTrigerWriteSit[nDevice]][nChannel]+tmp2;
            for(;nChannel>=0;nChannel--)//将数据移入瞬态数据头区域,并判断报警
            {
              //刷新瞬态信号通道数据写入地址
              g_nArmTranChDataOffset[p32S[nChannel]]=*p32U1;
              p32U1-=tmp1;
            }
            //刷新瞬态键相通道数据写入地址
            g_nArmTranKeyDataOffset[nDevice]=\
              g_nChannel_Tran_Key_Data_Offset[nDevice][nTranTrigerWriteSit[nDevice]][tmp2];

            g_TranTrigerEventArray[nDevice].nPreTranTickCnt++;//有效组数加1	
            g_TranTrigerEventArray[nDevice].ulnCurWriteGroup=tmp2;//切换到下一组
          }
          else{	//
            g_TranTrigerEventArray[nDevice].ulnGroupCnt[nTranTrigerWriteSit[nDevice]]=g_TranTrigerEventArray[nDevice].nPreTranTickCnt;
            g_TranTrigerEventArray[nDevice].nPreTranTickCnt = 0;//复原前触发数
            g_TranTrigerEventArray[nDevice].ulnTrigerFlag = 0;//复原瞬态触发标志
            g_TranTrigerEventArray[nDevice].ulnCurWriteGroup=0;
            tmp3=nTranTrigerWriteSit[nDevice]+1;
            if(_MAX_TRAN_ARRAY_LTH == tmp3)
              tmp3 = 0;//回头
            g_TranTrigerEventArray[nDevice].ulnTrigerWrite=tmp3;

            p32U1=g_nChannel_Tran_Wave_Data_Offset[nDevice][tmp3][nChannel]+0;
            for(;nChannel>=0;nChannel--)//将数据移入瞬态数据头区域,并判断报警
            {
              //刷新瞬态信号通道数据写入地址
              g_nArmTranChDataOffset[p32S[nChannel]]=*p32U1;
              p32U1-=tmp1;
            }
            //刷新瞬态键相通道数据写入地址
            g_nArmTranKeyDataOffset[nDevice]=\
              g_nChannel_Tran_Key_Data_Offset[nDevice][tmp3][0];
          }

        }					

        nIndex = nDynamicTrigerWriteSit[nDevice];//一旦触发，清空缓冲中的动态数据
        g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[nIndex].ulnBlockCnt = 0;//有效块数复原
        g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[nIndex].lnCurBlock = 0;//当前块复原
        g_TranTrigerEventArray[nDevice].ulnTrigerTimeSpan+=_12_DATA_UPLOAD_TIME_GAP;
        continue;//瞬态一旦触发，则不进行报警触发的判断和处理
      }	
    }
    else//该键相数据无效时，本次数据丢弃，动态数据将重新缓冲，已经触发的瞬态数据仅仅抛弃本次的
    {	nIndex = nDynamicTrigerWriteSit[nDevice];//一旦触发，清空缓冲中的动态数据
    g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[nIndex].ulnBlockCnt = 0;//有效块数复原
    g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[nIndex].lnCurBlock = 0;//当前块复原
    continue;
    }

    if(g_83param.Device[nDevice].bAlertTrigerStatic)//静态报警触发有效
    {
      if(g_83param.Device[nDevice].bAlertTrigerDynamic)//动态报警触发有效
      {
        g_nKeyDynamicTrigerFlag[nDevice] = g_nKeyStaticTrigerFlag[nDevice];//报警则触发	
      }else
        g_nKeyDynamicTrigerFlag[nDevice] = 0;//不触发动态
    }else
      g_nKeyStaticTrigerFlag[nDevice] = 0;//不会执行

    //未报警时判断定时采集是否成立
    if(g_83param.Device[nDevice].nDynamicAutoPickTime && g_nKeyDynamicTrigerFlag[nDevice] == 0)//动态
    {
      if(g_nDynamicTickCount[nDevice]==g_83param.Device[nDevice].nDynamicAutoPickTime)
      {
        g_nDynamicTickCount[nDevice]=0;
        g_nKeyDynamicTrigerFlag[nDevice] = 1;
        g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[nDynamicTrigerWriteSit[nDevice]].ulnTrigerReason = 1;
      }else
        g_nDynamicTickCount[nDevice]++;
    }
    if(g_83param.Device[nDevice].nStaticAutoPickTime && g_nKeyStaticTrigerFlag[nDevice] == 0)//静态
    {
      if(g_nStaticTickCount[nDevice]==g_83param.Device[nDevice].nStaticAutoPickTime)
      {
        g_nStaticTickCount[nDevice]=0;
        g_nKeyStaticTrigerFlag[nDevice] = 1;
        g_StaticTrigerEventArray[nDevice].ulnTrigerReason[nStaticTrigerWriteSit[nDevice]] = 1;	
      }else
        g_nStaticTickCount[nDevice]++;
    }
    //未触发时判断手动采集是否成立
    if(g_nKeyDynamicTrigerFlag[nDevice] == 0)//动态
    {
      g_nKeyDynamicTrigerFlag[nDevice] = g_nDynamicManualPick[nDevice];
      g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[nDynamicTrigerWriteSit[nDevice]].ulnTrigerReason = 2;
    }
    if(g_nKeyStaticTrigerFlag[nDevice] == 0)//静态
    {
      g_nKeyStaticTrigerFlag[nDevice] = g_nStaticManualPick[nDevice];
      g_StaticTrigerEventArray[nDevice].ulnTrigerReason[nStaticTrigerWriteSit[nDevice]] = 2;
    }
    if(g_nKeyStaticTrigerFlag[nDevice])//静态采集触发
    {
      g_StaticTrigerEventArray[nDevice].ulnTrigerTime[nStaticTrigerWriteSit[nDevice]] = g_nTickTime;
      g_StaticTrigerEventArray[nDevice].ulnTrigerWrite++;//切换到下一组
      if(_MAX_STATIC_ARRAY_LTH == g_StaticTrigerEventArray[nDevice].ulnTrigerWrite)//是否到达尾部
        g_StaticTrigerEventArray[nDevice].ulnTrigerWrite = 0;//回头
    }
    if(g_nKeyDynamicTrigerFlag[nDevice])//动态采集触发
    {
      nIndex = g_DynamicTrigerEventArray[nDevice].ulnTrigerWrite;//取本键相的动态当前写入索引
      pDCT = g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable+nIndex;
      pDCT->ulnTrigerTime = g_nTickTime;
      //逆推动态触发开始块序号
      if(g_83param.Device[nDevice].nDynamicSampleMode==0)//异步时块递增步长为1
      {
        pDCT->lnHeadBlock=pDCT->lnCurBlock+1;//总块数不变
      }
      else//同步时与转速有关
      {
        tmp=g_DynamicSpeedCalt[nDevice].nCurLevel;//获取转速级别
        pDCT->lnHeadBlock=pDCT->lnCurBlock+g_DynamicSpeedCalt[nDevice].nSynBlockIncreaseStep[tmp];
        pDCT->ulnBlockCnt=g_DynamicSpeedCalt[nDevice].nSynBlockCnt[tmp];//数据长度与转速阶数有关
      }
      if(pDCT->lnHeadBlock>=g_83param.Device[nDevice].nChDynamicDataBlockCnt)//块数越界，从头开始
        pDCT->lnHeadBlock=0;
      g_DynamicSampleStepDuration[nDevice]=g_83param.Device[nDevice].nChDynamicDataBlockCnt;//强制下次开始变频判断
      g_DynamicTrigerEventArray[nDevice].ulnTrigerWrite++;//指向下一个动态数据区
      //更新动态地址
      tmp1 = g_DynamicTrigerEventArray[nDevice].ulnTrigerWrite;//动态数据队列偏移
      p32S=g_nChannel_Dynamic_Wave_Data_Offset[nDevice][tmp1];
      for(nChannel = 0;nChannel<g_83param.Device[nDevice].nSignChannelNum;nChannel++)//将数据移入瞬态数据头区域,并判断报警
      {
        //取得该通道在１２中的序号,刷新动态信号通道数据写入地址
        g_nArmDynamicChDataOffset[g_83param.Device[nDevice].nChannelNumIn12[nChannel]]=p32S[nChannel];
      }
      //刷新动态键相通道数据写入地址
      g_nArmDynamicKeyDataOffset[nDevice]=g_nChannel_Dynamic_Key_Data_Offset[nDevice][tmp1];
      if(_MAX_DYNAMIC_ARRAY_LTH == g_DynamicTrigerEventArray[nDevice].ulnTrigerWrite)
        g_DynamicTrigerEventArray[nDevice].ulnTrigerWrite = 0;//回头
    }
    else
    {
      tmp2 = g_DynamicSpeedCalt[nDevice].nCurLevel;
      tmp1 = g_DynamicTrigerEventArray[nDevice].ulnTrigerWrite;//动态数据队列偏移
      tmp3 = g_83param.Device[nDevice].nChDynamicBlockSize;
      if(g_83param.Device[nDevice].nDynamicSampleMode)
        if(g_DynamicSpeedCalt[nDevice].nSynBlockIncreaseStep[tmp2]>1)
          tmp3*=g_DynamicSpeedCalt[nDevice].nSynBlockIncreaseStep[tmp2];
      //取本键相的动态当前写入索引
      pDCT = g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable+nDynamicTrigerWriteSit[nDevice];
      tmp = 1;//异步采集时总是1
      if(g_83param.Device[nDevice].nDynamicSampleMode)//同步采集升频时步长加大
        tmp = g_DynamicSpeedCalt[nDevice].nSynBlockIncreaseStep[tmp2];
      pDCT->lnCurBlock+=tmp;//切换到下一个数据块
      if(pDCT->lnCurBlock+tmp>g_83param.Device[nDevice].nChDynamicDataBlockCnt)//检测后面的数据块是否满足步长要求
        pDCT->lnCurBlock=0;//后面的数据块不足，从头开始存放
      if(g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[tmp1].lnCurBlock==0)
      {
        p32S=g_nChannel_Dynamic_Wave_Data_Offset[nDevice][tmp1];
        for(nChannel = 0;nChannel<g_83param.Device[nDevice].nSignChannelNum;nChannel++)//将数据移入瞬态数据头区域,并判断报警
        {
          //取得该通道在１２中的序号,刷新动态信号通道数据写入地址			
          g_nArmDynamicChDataOffset[g_83param.Device[nDevice].nChannelNumIn12[nChannel]]=p32S[nChannel];
        }
      }else
      {
        for(nChannel = 0;nChannel<g_83param.Device[nDevice].nSignChannelNum;nChannel++)//将数据移入瞬态数据头区域,并判断报警
        {
          g_nArmDynamicChDataOffset[g_83param.Device[nDevice].nChannelNumIn12[nChannel]]+=tmp3;
        }
      }
      //刷新动态键相通道数据写入地址
      if(g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[tmp1].lnCurBlock==0)
        g_nArmDynamicKeyDataOffset[nDevice]=g_nChannel_Dynamic_Key_Data_Offset[nDevice][tmp1];
      else
        g_nArmDynamicKeyDataOffset[nDevice]+=g_83param.Device[nDevice].nChKeyBlockSize;

    }		
  }
  //通知FPGA修改各个通道的动态和瞬态数据RAMDISK写入地址
  SetRamDiskChDataOffset(0,_MAX_SIGNAL_CHANNEL_CNT+_MAX_JKEY_CHANNEL_CNT,g_nArmDynamicChDataOffset);
  SetRamDiskChDataOffset(_MAX_SIGNAL_CHANNEL_CNT+_MAX_JKEY_CHANNEL_CNT,
    _MAX_SIGNAL_CHANNEL_CNT+_MAX_JKEY_CHANNEL_CNT,g_nArmTranChDataOffset);
  g_nIndexInGroupSequency++;
  if(g_nIndexInGroupSequency==g_nGroupCnt)
    g_nIndexInGroupSequency=0;
  g_nTickCount++;
  g_nTickTime+=_12_DATA_UPLOAD_TIME_GAP;
  
  //change by czx
  if((int)(g_pTranChannelValue[0][0][0][184])<1000)
    nDevice=nDevice;
}
