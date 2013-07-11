#include "lib_mem.h"
#include "define2060.h"
#include <string.h>
//#define	_DEFAULT_DATA_LTH	2048
#define MAX_SIGNAL_CHANNEL_PARA_ADDRESS_RANGE 40
#define	MAX_SLOTS		20
#define	MAX_RESERVES		20
#define _12MOUDLE_ID		21
#define _12MOUDLE_CHANNEL_CNT		4
#define _12MOUDLE_CHANNEL_TABLE_CNT	8
//extern INT32S	g_Signal_Channel_Dynamic_Data_Lth[];//各个通道的动态数据长度
//extern INT32S	g_Signal_Channel_Tran_Data_Lth[];//各个通道的瞬态态数据长度
//extern INT32U	g_nDynamicChDataOffset[_MAX_SIGNAL_CHANNEL_CNT];//动态数据电子盘当前写入位置
//extern INT32U	g_nTranhDataOffset[_MAX_SIGNAL_CHANNEL_CNT];//瞬态数据电子盘当前写入位置
INT32S	SetRamDiskChDataOffset(INT32S nStartChannel,INT32S nChannelcnt,INT32S pData[]);
INT32S  Read12Param(void* p12);
INT32S	Init83Param(void* p83in,void* p12in);
INT32S  CaltChannelParam(INT32S nKey,FP32 fFrequencyLow,FP32 fFrequencyHigh,INT32S nSampleSynMode,INT32S nSynSampleLth,
                         INT32S *pnPickDynamicFrequency,INT32S* pnDynamicChannelDataSampleStep,
                         INT32S* pnSignal_Channel_Dynamic_Data_Lth,INT32S* pnChDynamicBlockSize,
                         INT32S* pnChDynamicDataBlockCnt,INT32S* pnDynamicPickTime,INT32S* pnChDynamicDataSize,
                         INT32S* pnDynamicSynRPM);
INT32S	RenewParam(void* p83in,void* p12in);
INT32S	SetChDataSampleStep(INT32S nStartChannel,INT32S nChannelcnt,INT32S pData[]);
//arm调用读取通道数据头
INT32S ReadChValue(Tst_Head_DCM_SigModuSampData_SYS* p,INT32S nStartChannel,INT32S nChannelcnt);
//arm调用读取通道原始数据
INT32S ReadRamDisk(INT32U nOffset,INT32S pData[],INT32S nLength);
INT32S ReadKeyPointCnt(INT32U nChannelcnt,INT32U nChannel[],INT32S pData[]);
INT32S	BuildSignalGroupTable(INT32S nChannelSit[]);

extern INT32U		g_nArmDynamicChDataOffset[];
extern INT32U*		g_nArmDynamicKeyDataOffset;
extern INT32U*		g_nArmTranhDataOffset;
extern INT32U*		g_nArmTranhKeyDataOffset;

extern Key_Speed_Calt	g_DynamicSpeedCalt[];
extern INT32U		g_nFpgaDynamicChDataOffset[];
//extern INT32U*		g_nFpgaDynamicKeyDataOffset;
//extern INT32U*		g_nFpgaTranDataOffset;
//extern INT32U*		g_nFpgaTranKeyDataOffset;

extern INT32U		g_nFpgaDynamicChDataSampleStep[];
//extern INT32U*		g_nFpgaTranChDataSampleStep;
extern Tst_Head_DCM_SigModuSampData_SYS g_TempValue[_MAX_SIGNAL_CHANNEL_CNT];//来自于FPGA数据头的临时存储区域
extern INT8S* g_pChData;

extern INT32S		g_nCaltKeySpeed[_MAX_JKEY_CHANNEL_CNT];
INT32S Simu12Param(void* p12);
//#undef _SIMU
#ifndef _SIMU
typedef struct TagACTUAL_OTHER_PARA_TABLE_STRUCT
{
  INT32S nXXEnable;                       //XX是否可用。
  INT32S nXXAlertEnable;                  //XX Alert是否可用。
  INT32S nXXDangerEnable;                 //XX Danger是否可用。
  FP32 dXXFullScaleHigh;            //XX满量程高。
  FP32 dXXFullScaleLow;             //XX满量程低。
  FP32 dXXAlertHigh;                //XX报警高。
  FP32 dXXAlertHighHysteresis;      //XX报警高Hysteresis。
  FP32 dXXAlertLow;                 //XX报警低。
  FP32 dXXAlertLowHysteresis;       //XX报警低Hysteresis。
  FP32 dXXDangerHigh;               //XX危险高。
  FP32 dXXDangerHighHysteresis;     //XX危险高Hysteresis。
  FP32 dXXDangerLow;                //XX危险低。
  FP32 dXXDangerLowHysteresis;      //XX危险低Hysteresis。
}ACTUAL_OTHER_PARA_TABLE_STRUCT;
//##ModelId=484F74B9030E
typedef struct TagSIGNAL_MODULE_PARAMETER_STRUCT{
  INT32S nChannelTypeID;                    //通道类型ID。
  INT32S nSensorTypeID;                     //传感器类型ID。
  INT32S nChannelEnable;                    //通道是否可用。
  INT32S nAlermLatch;                       //通道是否报警锁存。
  FP32 dSensitivity;                     //传感器灵敏。
  FP32 dAlertDelay;                      //ALERT报警延时。s
  FP32 dDangerDelay;                     //DANGER报警延时。s
  INT32S nOneHundredMs;                       //100ms是否有效。0:无效；1有效。
  FP32 dTeethPerCycle;                   //每周齿数。
  FP32 dZeroPosition;                    //零点位置。v
  FP32 dHysteresis;                      //回差。v
  FP32 dTriggerVoltage;                  //触发电压。v
  FP32 dFullScaleHigh;                //满量程高。
  FP32 dFullScaleLow;                 //满量程低。
  INT32S nMeasurementTypeID;               //测量类型ID。
  INT32S nMeasurementUnitID;               //测量单位ID。
  INT32S nAlertMeasurementTypeID;          //选择ALERT报警所对应的测量形式ID。
  INT32S nPhaseReferenceID;                //键相参考ID。 
  FP32 dDangerHigh;                   //危险高。
  FP32 dAlertHigh;                    //报警高。
  FP32 dAlertLow;                     //报警低。
  FP32 dDangerLow;                    //危险低。
  FP32 dGapHigh;                      //间隙电压高。
  FP32 dGapLow;                       //间隙电压低。
  INT32S nTransducerDirection;             //传感器安装方向。0：正向；1：背向。
  INT32S nLeadingTransducer;               //主导传感器。0：通道1；2：通道2。
  INT32S nUpscaleDirection;                //Upscale方向。0：朝向；1：背向。
  INT32S nOneOfUpThreePara;                //以上三项只选其一。0：TransducerDirection；1：LeadingTransducer；2：UpscaleDirection。
  INT32S nThresholdType;                   //门槛类型。0：手动；1：自动。
  INT32S nBarrier;                         //是否使用防爆隔离栅。
  INT32S nSampleRate;                      //采样速率ID。
  FP32 dDangerHighHysteresis;         //危险高报警回差。
  FP32 dAlertHighHysteresis;          //报警高报警回差。
  FP32 dAlertLowHysteresis;           //报警低报警回差。
  FP32 dDangerLowHysteresis;          //危险低报警回差。
  FP32 dGapAlertHysteresis;         //GAP报警回差。
  FP32 dAngleOfSensorFix;           //传感器安装角度。
  FP32 dLowPassFilter;				//低通滤波频率。
  FP32 dHighPassFilter;				 //高通滤波频率。
  //LG:2009-09-09 针对12模块而增加的参数///////////////////////////////////////////////////////////////
  INT32S nOverallAlertEnable;              //Overall Alert是否可用。
  INT32S nOverallDangerEnable;             //Overall Danger是否可用。
  INT32S nModbusOutputTypeID;            //MODBUS OUTPUT TYPE ID
  INT32S n4To20mAOutputTypeID;              //4~20ma OUTPUT TYPE ID。
  
  INT32S nIntegrateEnable;                   //是否积分。
  
  INT32S nHighPassFilterEnable;            //高通滤波是否可用。
  INT32S nLowPassFilterEnable;            //低通滤波是否可用。
  
  ACTUAL_OTHER_PARA_TABLE_STRUCT struActualOtherParaTable[7]; //其它参数表。0：1XA；1：1XP；2：2XA；3:2XP; 4:NOT1X;5:SMAX;6:GAP。
  //LG:2009-09-09 针对12模块而增加的参数///////////////////////////////////////////////////////////////	
  INT32S nReserve[MAX_RESERVES];             //INT32S保留。
  FP32 dReserve[MAX_RESERVES];          //double保留。
  //LG:2009-09-09 针对12模块而增加的参数///////////////////////////////////////////////////////////////
}SIGNAL_MODULE_PARAMETER_STRUCT;
INT32S	g_nCurWriteSit=0;//写入位置，由数据接收任务操作
INT32S	g_nCurReadSit=0;//读取位置，由数据读取函数操作

INT32S InitSignalMoudle(SIGNAL_MODULE_PARAMETER_STRUCT* p)
{
  p->nChannelTypeID=-1;                
  p->nSensorTypeID=-1;                 
  p->nChannelEnable=1;                 
  p->nAlermLatch=0;                
  p->dSensitivity=0.0;
  p->dAlertDelay=1.0;                
  p->dDangerDelay=1.0;          
  p->nOneHundredMs=0;
  p->dTeethPerCycle=1.0;                
  p->dZeroPosition=-10.0;            
  p->dHysteresis=1.0;                
  p->dTriggerVoltage=-10.0;          
  p->dFullScaleHigh=1;                
  p->dFullScaleLow=0;                 
  p->nMeasurementTypeID=-1;           
  p->nMeasurementUnitID=-1;           
  p->nAlertMeasurementTypeID=-1;                 
  p->nPhaseReferenceID=-1;            
  p->dDangerHigh=0.0;                 
  p->dAlertHigh=0.0;                  
  p->dAlertLow=0.0;                   
  p->dDangerLow=0.0;                  
  p->dGapHigh=0.0;                    
  p->dGapLow=0.0;   
  p->nTransducerDirection=0;          
  p->nLeadingTransducer=0;            
  p->nUpscaleDirection=0;              
  p->nOneOfUpThreePara=0;                     
  p->nThresholdType=1;                
  p->nBarrier=0;                      
  p->nSampleRate=0;                   
  p->dDangerHighHysteresis=0.0;       
  p->dAlertHighHysteresis=0.0;        
  p->dAlertLowHysteresis=0.0;         
  p->dDangerLowHysteresis=0.0;        
  p->dGapAlertHysteresis=0.0;   
  p->dAngleOfSensorFix=0.0;         
  p->dLowPassFilter=0.0;  
  p->dHighPassFilter=0.0; 
  
  ZeroArray(p->nReserve);
  ZeroArray(p->dReserve);
  //LG:2009-09-09 针对12模块而增加的参数///////////////////////////////////////////////////////////////
  p->nOverallAlertEnable=0;              //Overall Alert是否可用。
  p->nOverallDangerEnable=0;             //Overall Danger是否可用。
  p->nModbusOutputTypeID=-1;            //MODBUS OUTPUT TYPE ID
  p->n4To20mAOutputTypeID=-1;              //4~20ma OUTPUT TYPE ID。
  
  p->nIntegrateEnable=0;                   //是否积分。
  
  p->nHighPassFilterEnable=0;            //高通滤波是否可用。
  p->nLowPassFilterEnable=0;            //低通滤波是否可用。
  ZeroArray(p->struActualOtherParaTable);
}

SIGNAL_MODULE_PARAMETER_STRUCT g_SignalChannelParam[_MAX_SIGNAL_CHANNEL_CNT];
void ParsePara(INT32S nChannel,INT32U dwPara[], INT32S nCount,INT32S nType)
{
  INT32S nSensorType=0;//?
  INT32S nUnitType = 1;//?
  INT32S nExponent=0;
  INT8U	btAlertAlarm=0,btDangerAlarm=0;
  INT32S w=0;
  if(nCount!=MAX_SIGNAL_CHANNEL_PARA_ADDRESS_RANGE)
    return;
  switch(nType)
  {
  case 0:
    {
      BOOL bNeedChange;//对30模块的单位的特殊处理。
      TCHAR  szName[128];
      g_SignalChannelParam[nChannel].nSampleRate=dwPara[0];                         //40200采样率。
      g_SignalChannelParam[nChannel].nMeasurementTypeID=dwPara[2];                  //40202测量类型。
      if(nUnitType == 1 && dwPara[3] == 9)
      {
        g_SignalChannelParam[nChannel].nMeasurementUnitID=11;                         //40203测量单位。
        bNeedChange=TRUE;
      }
      else
      {
        g_SignalChannelParam[nChannel].nMeasurementUnitID=dwPara[3];                  //40203测量单位。
        bNeedChange=FALSE;
      }
      g_SignalChannelParam[nChannel].nSensorTypeID=dwPara[4];                       //40204传感器类型ID。
      
      switch(nSensorType)
      {
      case 0:
        g_SignalChannelParam[nChannel].dSensitivity=(INT16U)dwPara[5];                                   //40205 传感器灵敏度。
        break;
      case 1:
        g_SignalChannelParam[nChannel].dSensitivity=(INT16U)dwPara[5]/1000.0f;									    //40205 传感器灵敏度。
        break;
      case 2:
        g_SignalChannelParam[nChannel].dSensitivity=(INT16U)dwPara[5]/1000.0f;                                   //40205 传感器灵敏度。
        break;
      case 3:
        g_SignalChannelParam[nChannel].dSensitivity=(INT16U)dwPara[5]/1000.0f;                                   //40205 传感器灵敏度。
        break;
      case 4:
        g_SignalChannelParam[nChannel].dSensitivity=(INT16U)dwPara[5]/1000.0f;                                   //40205 传感器灵敏度。
        break;
      }
      
      nExponent=(INT16S)dwPara[27];                                                             //40227 配置点参数指数值。
      
      g_SignalChannelParam[nChannel].nTransducerDirection=dwPara[6];                                    //40206 传感器方向。
      g_SignalChannelParam[nChannel].nAlertMeasurementTypeID=dwPara[7];                                 //40207 报警测量类型。
      if(dwPara[3] == 7 || dwPara[3] == 8)
      {
        g_SignalChannelParam[nChannel].dDangerHigh=(INT16U)dwPara[8]*pow(10,nExponent);                              //40208 危险高。
        g_SignalChannelParam[nChannel].dDangerHighHysteresis=(INT16U)dwPara[9]*pow(10,nExponent);                    //40209 危险高回差。
        g_SignalChannelParam[nChannel].dDangerLow=(INT16U)dwPara[10]*pow(10,nExponent);                              //40210 危险低。
        g_SignalChannelParam[nChannel].dDangerLowHysteresis=(INT16U)dwPara[11]*pow(10,nExponent);                    //40211 危险低回差。
        g_SignalChannelParam[nChannel].dAlertLow=(INT16U)dwPara[12]*pow(10,nExponent);                               //40212 报警低。
        g_SignalChannelParam[nChannel].dAlertLowHysteresis=(INT16U)dwPara[13]*pow(10,nExponent);                     //40213 报警低回差。
        g_SignalChannelParam[nChannel].dAlertHigh=(INT16U)dwPara[14]*pow(10,nExponent);                              //40214 报警高。
        g_SignalChannelParam[nChannel].dAlertHighHysteresis=(INT16U)dwPara[15]*pow(10,nExponent);                    //40215 报警高回差。
        g_SignalChannelParam[nChannel].dFullScaleHigh=(INT16U)dwPara[19]*pow(10,nExponent);                          //40219 满量程高。
        g_SignalChannelParam[nChannel].dFullScaleLow=(INT16U)dwPara[26]*pow(10,nExponent);                      //40226 满量程低。
      }
      else
      {
        if(bNeedChange == TRUE)
        {
          g_SignalChannelParam[nChannel].dDangerHigh=(INT16S)dwPara[8]*pow(10,nExponent)*1.8+32;                              //40208 危险高。
          g_SignalChannelParam[nChannel].dDangerHighHysteresis=(INT16S)dwPara[9]*pow(10,nExponent)*1.8+32;                    //40209 危险高回差。
          g_SignalChannelParam[nChannel].dDangerLow=(INT16S)dwPara[10]*pow(10,nExponent)*1.8+32;                              //40210 危险低。
          g_SignalChannelParam[nChannel].dDangerLowHysteresis=(INT16S)dwPara[11]*pow(10,nExponent)*1.8+32;                    //40211 危险低回差。
          g_SignalChannelParam[nChannel].dAlertLow=(INT16S)dwPara[12]*pow(10,nExponent)*1.8+32;                               //40212 报警低。
          g_SignalChannelParam[nChannel].dAlertLowHysteresis=(INT16S)dwPara[13]*pow(10,nExponent)*1.8+32;                     //40213 报警低回差。
          g_SignalChannelParam[nChannel].dAlertHigh=(INT16S)dwPara[14]*pow(10,nExponent)*1.8+32;                              //40214 报警高。
          g_SignalChannelParam[nChannel].dAlertHighHysteresis=(INT16S)dwPara[15]*pow(10,nExponent)*1.8+32;                    //40215 报警高回差。
          g_SignalChannelParam[nChannel].dFullScaleHigh=(INT16S)dwPara[19]*pow(10,nExponent)*1.8+32;                          //40219 满量程高。
          g_SignalChannelParam[nChannel].dFullScaleLow=(INT16S)dwPara[26]*pow(10,nExponent)*1.8+32;                      //40226 满量程低。
        }
        else
        {
          g_SignalChannelParam[nChannel].dDangerHigh=(INT16S)dwPara[8]*pow(10,nExponent);                              //40208 危险高。
          g_SignalChannelParam[nChannel].dDangerHighHysteresis=(INT16S)dwPara[9]*pow(10,nExponent);                    //40209 危险高回差。
          g_SignalChannelParam[nChannel].dDangerLow=(INT16S)dwPara[10]*pow(10,nExponent);                              //40210 危险低。
          g_SignalChannelParam[nChannel].dDangerLowHysteresis=(INT16S)dwPara[11]*pow(10,nExponent);                    //40211 危险低回差。
          g_SignalChannelParam[nChannel].dAlertLow=(INT16S)dwPara[12]*pow(10,nExponent);                               //40212 报警低。
          g_SignalChannelParam[nChannel].dAlertLowHysteresis=(INT16S)dwPara[13]*pow(10,nExponent);                     //40213 报警低回差。
          g_SignalChannelParam[nChannel].dAlertHigh=(INT16S)dwPara[14]*pow(10,nExponent);                              //40214 报警高。
          g_SignalChannelParam[nChannel].dAlertHighHysteresis=(INT16S)dwPara[15]*pow(10,nExponent);                    //40215 报警高回差。
          g_SignalChannelParam[nChannel].dFullScaleHigh=(INT16S)dwPara[19]*pow(10,nExponent);                          //40219 满量程高。
          g_SignalChannelParam[nChannel].dFullScaleLow=(INT16S)dwPara[26]*pow(10,nExponent);                      //40226 满量程低。
        }
      }
      /*
      //郭高峰 2009-07-15 针对35模块的 电流通道(30) 和 电压通道(31)
      //////////////////////////
      if(dwPara[25]==30 || dwPara[25] == 31)
      {
      g_SignalChannelParam[nChannel].dDangerHigh=(DINT16U)dwPara[8]*pow(10,nExponent);                              //40208 危险高。
      g_SignalChannelParam[nChannel].dDangerHighHysteresis=(DINT16U)dwPara[9]*pow(10,nExponent);                    //40209 危险高回差。
      g_SignalChannelParam[nChannel].dDangerLow=(DINT16U)dwPara[10]*pow(10,nExponent);                              //40210 危险低。
      g_SignalChannelParam[nChannel].dDangerLowHysteresis=(DINT16U)dwPara[11]*pow(10,nExponent);                    //40211 危险低回差。
      g_SignalChannelParam[nChannel].dAlertLow=(DINT16U)dwPara[12]*pow(10,nExponent);                               //40212 报警低。
      g_SignalChannelParam[nChannel].dAlertLowHysteresis=(DINT16U)dwPara[13]*pow(10,nExponent);                     //40213 报警低回差。
      g_SignalChannelParam[nChannel].dAlertHigh=(DINT16U)dwPara[14]*pow(10,nExponent);                              //40214 报警高。
      g_SignalChannelParam[nChannel].dAlertHighHysteresis=(DINT16U)dwPara[15]*pow(10,nExponent);                    //40215 报警高回差。
      g_SignalChannelParam[nChannel].dFullScaleHigh=(DINT16U)dwPara[19]*pow(10,nExponent);                          //40219 满量程高。
      g_SignalChannelParam[nChannel].dFullScaleLow=(DINT16U)dwPara[26]*pow(10,nExponent);                      //40226 满量程低。
    }
      /////////////////////////////
      */
      g_SignalChannelParam[nChannel].dGapHigh=(INT16S)dwPara[16]*pow(10,-3);                                         //40216 GAP高。
      g_SignalChannelParam[nChannel].dGapLow=(INT16S)dwPara[17]*pow(10,-3);                                          //40217 GAP低。
      g_SignalChannelParam[nChannel].dGapAlertHysteresis=(INT16S)dwPara[18]*pow(10,-3);                              //40218 GAP回差。
      g_SignalChannelParam[nChannel].dAlertDelay=(INT16U)dwPara[20];                                            //40220 Alert报警延迟。
      g_SignalChannelParam[nChannel].dDangerDelay=(INT16U)dwPara[21];                                           //40221 Danger报警延迟。
      g_SignalChannelParam[nChannel].nAlermLatch=dwPara[22];                                            //40222 报警自锁。
      g_SignalChannelParam[nChannel].nChannelEnable=dwPara[23];                                         //40223 通道是否工作。
      g_SignalChannelParam[nChannel].nPhaseReferenceID=dwPara[24];                                      //40224 通道板键相信号ID。
      g_SignalChannelParam[nChannel].nChannelTypeID=dwPara[25];                                         //40225 通道类型ID。
      
      //LG:2009-08-28 31模块的零点位置上传后除以10
      /*	if(this->m_nModuleID == 16)//路占奎，2012-11-16
      g_SignalChannelParam[nChannel].dZeroPosition=(INT16S)dwPara[28]/10.0;                                     //40228 零点位置。
            else*/
      g_SignalChannelParam[nChannel].dZeroPosition=(INT16S)dwPara[28]/1000.0;                                     //40228 零点位置。
      
      g_SignalChannelParam[nChannel].dHysteresis=(INT16S)dwPara[29]/1000.0;                                       //40229 迟滞电压。
      g_SignalChannelParam[nChannel].dTriggerVoltage=(INT16S)dwPara[30]/1000.0;                                   //40230 翻转电压。
      g_SignalChannelParam[nChannel].dTeethPerCycle=(INT16U)dwPara[31];                                         //40231 每周齿数。
      g_SignalChannelParam[nChannel].nThresholdType=dwPara[32];                                         //40232 触发电平（自动或手动）。
      g_SignalChannelParam[nChannel].dAngleOfSensorFix=dwPara[33];                                   //40233 传感器与被测物体的角度。
      g_SignalChannelParam[nChannel].nBarrier=dwPara[34];                                               //40234 防爆隔离栅。
      g_SignalChannelParam[nChannel].dLowPassFilter=dwPara[35];                               //40235 滤波器低端截止频率。
      g_SignalChannelParam[nChannel].dHighPassFilter=dwPara[36];                              //40236 滤波器高端截止频率。
      
      //LG:2009-09-11 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if((dwPara[35] & 0xFFFF) == 0xFFFF)
      {
        //	g_SignalChannelParam[nChannel].nLowPassFilterEnable = FALSE;                                                                       //40235 滤波器低端截止频率。
        g_SignalChannelParam[nChannel].dLowPassFilter=0;                               //40235 滤波器低端截止频率。
      }
      else
      {
        //	g_SignalChannelParam[nChannel].nLowPassFilterEnable = TRUE;                                                                       //40235 滤波器低端截止频率。
        g_SignalChannelParam[nChannel].dLowPassFilter=(INT16S)dwPara[35];                               //40235 滤波器低端截止频率。
      }
      
      if((dwPara[36] & 0xFFFF) == 0x0000)//LG:2009-11-16 由原来的0xFFFFFFF改为现在的0x00   
      {
        //	g_SignalChannelParam[nChannel].nHighPassFilterEnable = FALSE;                                                                       //40235 滤波器低端截止频率。
        g_SignalChannelParam[nChannel].dHighPassFilter = 0;                              //40236 滤波器高端截止频率。
      }
      else
      {
        //	g_SignalChannelParam[nChannel].nHighPassFilterEnable = TRUE;                                                                       //40235 滤波器低端截止频率。
        g_SignalChannelParam[nChannel].dHighPassFilter = (INT16S)dwPara[36];                              //40236 滤波器高端截止频率。
      }
      
      //报警组合
      
      btAlertAlarm=dwPara[38] & 0xFF;
      btDangerAlarm=(dwPara[38] & 0xFF00) >> 8;
      
      g_SignalChannelParam[nChannel].nOverallAlertEnable=0;
      g_SignalChannelParam[nChannel].nOverallDangerEnable=0;
      for(w=0;w<7;w++)
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[w].nXXAlertEnable=0;
        g_SignalChannelParam[nChannel].struActualOtherParaTable[w].nXXDangerEnable=0;
      }
      if((btAlertAlarm & 0x1)== 0x1)
      {
        g_SignalChannelParam[nChannel].nOverallAlertEnable = 1;
      }
      if((btAlertAlarm & 0x4)== 0x4)
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[0].nXXAlertEnable = 1;
      }
      if((btAlertAlarm & 0x10)== 0x10)
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[1].nXXAlertEnable = 1;
      }
      if((btAlertAlarm & 0x8) == 0x8)
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[2].nXXAlertEnable = 1;
      }
      if((btAlertAlarm & 0x20) == 0x20)
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[3].nXXAlertEnable = 1;
      }
      if((btAlertAlarm & 0x40) == 0x40)
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[4].nXXAlertEnable = 1;
      }
      if((btAlertAlarm & 0x80) == 0x80)
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[5].nXXAlertEnable = 1;
      }
      if((btAlertAlarm & 0x2) == 0x2)
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[6].nXXAlertEnable = 1;
      }
      
      if((btDangerAlarm & 0x1) == 0x1)
      {
        g_SignalChannelParam[nChannel].nOverallDangerEnable = 1;
      }
      if((btDangerAlarm & 0x4) == 0x4)
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[0].nXXDangerEnable = 1;
      }
      if((btDangerAlarm & 0x10) == 0x10)
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[1].nXXDangerEnable = 1;
      }
      if((btDangerAlarm & 0x8) == 0x8)
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[2].nXXDangerEnable = 1;
      }
      if((btDangerAlarm & 0x20) == 0x20)
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[3].nXXDangerEnable = 1;
      }
      if((btDangerAlarm & 0x40) == 0x40)
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[4].nXXDangerEnable = 1;
      }
      if((btDangerAlarm & 0x80) == 0x80)
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[5].nXXDangerEnable = 1;
      }
      if((btDangerAlarm & 0x2) == 0x2)
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[6].nXXDangerEnable = 1;
      }
      
      //输出类型
      //路占奎，临时改动，这是为协议不一致而凑的，严重破坏程序结构，2012-3-13
      /*	if(this->m_nModuleID == 21)//路占奎，2012-11-16
      {*/
      g_SignalChannelParam[nChannel].nModbusOutputTypeID = dwPara[39] & 0xFF;
      g_SignalChannelParam[nChannel].n4To20mAOutputTypeID = (dwPara[39] & 0xFF00) >> 8;
      /*}else
      {
      g_SignalChannelParam[nChannel].nModbusOutputTypeID = 1;//OverAll
      g_SignalChannelParam[nChannel].n4To20mAOutputTypeID = 1;//OverAll
    }*/
      //LG:2009-09-11 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    break;
  case 1://1XA
  case 3://2XA
  case 5://NOT1X
  case 6://SMAX
  case 2://1XP
  case 4://2XP
  case 7://GAP
    {
      INT32S nIndex=nType-1;
      INT32S nExponent=(INT16S)dwPara[27];
      if(nType==1)
      {
        nIndex=6;
      }else
      {
        nIndex=nType-1-1;
      }
      //40227 配置点参数指数值。
      if(dwPara[3] == 7 || dwPara[3] == 8 )		
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXDangerHigh=(INT16U)dwPara[8]*pow(10,nExponent);                              //40208 危险高。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXDangerHighHysteresis=(INT16U)dwPara[9]*pow(10,nExponent);                    //40209 危险高回差。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXDangerLow=(INT16U)dwPara[10]*pow(10,nExponent);                              //40210 危险低。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXDangerLowHysteresis=(INT16U)dwPara[11]*pow(10,nExponent);                    //40211 危险低回差。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXAlertLow=(INT16U)dwPara[12]*pow(10,nExponent);                               //40212 报警低。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXAlertLowHysteresis=(INT16U)dwPara[13]*pow(10,nExponent);                     //40213 报警低回差。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXAlertHigh=(INT16U)dwPara[14]*pow(10,nExponent);                              //40214 报警高。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXAlertHighHysteresis=(INT16U)dwPara[15]*pow(10,nExponent);                    //40215 报警高回差。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXFullScaleHigh=(INT16U)dwPara[19]*pow(10,nExponent);                          //40219 满量程高。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXFullScaleLow=(INT16U)dwPara[26]*pow(10,nExponent);                      //40226 满量程低。
      }
      else
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXDangerHigh=(INT16S)dwPara[8]*pow(10,nExponent);                              //40208 危险高。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXDangerHighHysteresis=(INT16S)dwPara[9]*pow(10,nExponent);                    //40209 危险高回差。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXDangerLow=(INT16S)dwPara[10]*pow(10,nExponent);                              //40210 危险低。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXDangerLowHysteresis=(INT16S)dwPara[11]*pow(10,nExponent);                    //40211 危险低回差。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXAlertLow=(INT16S)dwPara[12]*pow(10,nExponent);                               //40212 报警低。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXAlertLowHysteresis=(INT16S)dwPara[13]*pow(10,nExponent);                     //40213 报警低回差。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXAlertHigh=(INT16S)dwPara[14]*pow(10,nExponent);                              //40214 报警高。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXAlertHighHysteresis=(INT16S)dwPara[15]*pow(10,nExponent);                    //40215 报警高回差。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXFullScaleHigh=(INT16S)dwPara[19]*pow(10,nExponent);                          //40219 满量程高。
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXFullScaleLow=(INT16S)dwPara[26]*pow(10,nExponent);                      //40226 满量程低。
      }
      
      g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].nXXEnable = dwPara[23];                                         //40223 通道是否工作。
    }
    break;
  default:
    break;
  }
};
INT32S GetMoudleType(INT32S nSlot)
{
  INT32S re=0;
  return re;
}
INT32S Get12MoudleParam(INT32S nSlot,INT32S nChannel,INT32S nTable,INT32U dwPara)
{
  INT32S re=0;
  return re;
}
INT32S ReadSignalChannelParam(void* p12)
{
  INT32S re=0;
  INT32S i=0,j=0,k=0;
  INT32S nChannel;
  INT32U dwPara[MAX_SIGNAL_CHANNEL_PARA_ADDRESS_RANGE];
  INT32S nCount;
  INT32S nType;
  for(i=0;i<MAX_SLOTS;i++)
  {
    if(_12MOUDLE_ID==GetMoudleType(i))
    {
      for(j=0;j<_12MOUDLE_CHANNEL_CNT;j++)
      {
        for(k=0;k<_12MOUDLE_CHANNEL_TABLE_CNT;k++)
        {
          if(Get12MoudleParam(i,j,k,dwPara))
          {					
            ParsePara(j,dwPara+k,MAX_SIGNAL_CHANNEL_PARA_ADDRESS_RANGE,k);
          }
          else
            return -1;
        }
      }
    }
  }
  return re;
}
#else
INT32S Simu12Param(void* p12)
{
  INT32S re=0,i;
  //INT32S nSize=sizeof(struct tagChData2060)+(_MAX_KEY_PT_CNT+_DEFAULT_DATA_LTH-1)*sizeof(INT32S);
  struct tag12Param* p=p12;
  //memset(p->nMrsPtDataSize, 0, sizeof(INT32S)*_MAX_SIGNAL_CHANNEL_CNT);
  Mem_Clr(p->nMrsPtDataSize,  sizeof(INT32S)*_MAX_SIGNAL_CHANNEL_CNT);
  Mem_Clr(p->nMrsPtDataSize0,  sizeof(INT32S)*_MAX_SIGNAL_CHANNEL_CNT);

  p->nDeviceNo=206;	//设备序号;
  p->nDeviceProperty=2060;	//设备属性；0 数据采集卡 6 过程量输入模块 4 电流模块 5 继电器模块；
  p->nDeviceTypeID=2060;	//设备类型ID;
  p->nDeviceID=20601;//设备ID;
  
  p->nVersion=1<<8;//0.1版
  p->nRenewID=1;//>0
  p->nRenewTime=(2101<<16)+(7<<8)+(8);//2011-7-8
  
  p->nSignChannelNum=48;
  p->nMrsPtcnt=0;//设置测点数量
  for(i=0;i<_MAX_SIGNAL_CHANNEL_CNT;i++)
  {
    p->bWork[i]=1;//启动所有通道
    p->nMrsPt[i]=-1;
    p->nMrsPtTable[i]=-1;
  }
  for(i=0;i<10;i++)
  {
    //p->SignalChannel[i].nDatalth=_DEFAULT_DATA_LTH;
    //p->nSignalChannelDataSize[i]=nSize;
    p->SignalChannel[i].nChNo=i;//通道序号	0~47	不修改	转化为槽号和通道号	
    p->SignalChannel[i].nChType=2;	//通道类型	0~255	位移、速度、加速度、温度、键相，可以扩展
    p->SignalChannel[i].fChFullScaleLow=-300;	//量程低,用于原始数据的归一化处理
    p->SignalChannel[i].fChFullScaleHigh=300;	//量程高,用于原始数据的归一化处理
    p->SignalChannel[i].fSensority=8;	//传感器灵敏度	测量值与电压间的线性因子
    p->SignalChannel[i].fZeroVolt=0;	//零点电压	测量值零点对应的电压
    ///////	传感器类型	否	p->SignalChannel[i]. 	0~2	不积分、一次积分、二次积分
    ///////	测量值类型	是	p->SignalChannel[i]. 	0~3	pk，pk-pk，rms，avr
    ///测量值输出方式	否	p->SignalChannel[i]. 	0~2	opc，4~20ma设备，modbus，取消？
    p->SignalChannel[i]. bAlert=1;//报警允许	是	p->SignalChannel[i]. 	0或1	允许或不允许
    p->SignalChannel[i]. bAlertLock=1;	//报警锁定	否	p->SignalChannel[i]. 	0或1	锁定或不锁定
    p->SignalChannel[i]. fAlertLevelLow=-200;//警报	是	p->SignalChannel[i].	？	alert下限
    p->SignalChannel[i]. fDangerLevelLow=-250;//危险	是	p->SignalChannel[i].	？	danger下限
    p->SignalChannel[i]. fAlertLevelHigh=200;//警报	是	p->SignalChannel[i].	？	alert下限
    p->SignalChannel[i]. fDangerLevelHigh=250;//危险	是	p->SignalChannel[i].	？	danger下限
    p->SignalChannel[i]. nPickFrequency=20000;//采集频率	12模块将原始通道数据抽样得到指定频率下的数据
    p->SignalChannel[i].fFreqResponseLow=1;	//	频响下限	是	p->SignalChannel[i].	？	12模块已经规定，不修改
    p->SignalChannel[i].fFreqResponsehigh=1000;	//频响上限	是	p->SignalChannel[i].	？	12模块已经规定，不修改
    ////p->SignalChannel[i].2滤波范围	是	p->SignalChannel[i].	？	?
    p->SignalChannel[i].bSync=1;//同步标志	是	p->SignalChannel[i]. 	0或1	1，同步；0，异步
    p->SignalChannel[i].fFreqencyRes=1;//频域分辨率,异步模式下代表频域分辨率，同步模式为采样长度
    p->SignalChannel[i].nJkeyNo=0;	//键相通道序号 	是	p->SignalChannel[i]. 	0~？	
    
    p->SignalChannel[i].nInstallAngle=90*(i-i/2*2);//传感器安装角度,	0~360
    //下列参数与几何因素相关，绘制某些视图时需要
    p->SignalChannel[i].bCircleFlag=0;//椭圆标志	否	p->SignalChannel[i]. 	0或1	0，非椭圆；1，椭圆
    p->SignalChannel[i].fAxa=400;//椭圆长轴	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].fAxb=300;//椭圆短轴	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].byTrustDiretion=0;	//涡流探头推进方向	否	p->SignalChannel[i]. 	0或1	0，靠近；1，远离
    p->SignalChannel[i].fGapLow=600;	//间隙低	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].fGapHigh=100;	//间隙高	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].bEn=0;//测量
    p->SignalChannel[i].nCalcMode=0;//传感器类型,用于上位机	否	INT32U 	0~2	不积分、一次积分、二次积分		
    p->SignalChannel[i].nOutputMode=1;
  }
  
  for(i=10;i<20;i++)
  {
    //		p->SignalChannel[i].nDatalth=_DEFAULT_DATA_LTH;
    //	p->nSignalChannelDataSize[i]=nSize;
    p->SignalChannel[i].nChNo=i;//通道序号	0~47	不修改	转化为槽号和通道号	
    p->SignalChannel[i].nChType=1;	//通道类型	0~255	位移、速度、加速度、温度、键相，可以扩展
    p->SignalChannel[i].fChFullScaleLow=0;	//量程低,用于原始数据的归一化处理
    p->SignalChannel[i].fChFullScaleHigh=300;	//量程高,用于原始数据的归一化处理
    p->SignalChannel[i].fSensority=8;	//传感器灵敏度	测量值与电压间的线性因子
    p->SignalChannel[i].fZeroVolt=0;	//零点电压	测量值零点对应的电压
    ///////	传感器类型	否	p->SignalChannel[i]. 	0~2	不积分、一次积分、二次积分
    ///////	测量值类型	是	p->SignalChannel[i]. 	0~3	pk，pk-pk，rms，avr
    ///测量值输出方式	否	p->SignalChannel[i]. 	0~2	opc，4~20ma设备，modbus，取消？
    p->SignalChannel[i]. bAlert=1;//报警允许	是	p->SignalChannel[i]. 	0或1	允许或不允许
    p->SignalChannel[i]. bAlertLock=1;	//报警锁定	否	p->SignalChannel[i]. 	0或1	锁定或不锁定
    p->SignalChannel[i]. fAlertLevelLow=-200;//警报	是	p->SignalChannel[i].	？	alert下限
    p->SignalChannel[i]. fDangerLevelLow=-250;//危险	是	p->SignalChannel[i].	？	danger下限
    p->SignalChannel[i]. fAlertLevelHigh=200;//警报	是	p->SignalChannel[i].	？	alert下限
    p->SignalChannel[i]. fDangerLevelHigh=250;//危险	是	p->SignalChannel[i].	？	danger下限
    p->SignalChannel[i]. nPickFrequency=20000;//采集频率	12模块将原始通道数据抽样得到指定频率下的数据
    p->SignalChannel[i].fFreqResponseLow=1;	//	频响下限	是	p->SignalChannel[i].	？	12模块已经规定，不修改
    p->SignalChannel[i].fFreqResponsehigh=1000;	//频响上限	是	p->SignalChannel[i].	？	12模块已经规定，不修改
    ////p->SignalChannel[i].2滤波范围	是	p->SignalChannel[i].	？	?
    p->SignalChannel[i].bSync=1;//同步标志	是	p->SignalChannel[i]. 	0或1	1，同步；0，异步
    p->SignalChannel[i].fFreqencyRes=1;//频域分辨率,异步模式下代表频域分辨率，同步模式为采样长度
    p->SignalChannel[i].nJkeyNo=1;	//键相通道序号 	是	p->SignalChannel[i]. 	0~？	
    
    p->SignalChannel[i].nInstallAngle=90*(i-i/2*2);//传感器安装角度,	0~360
    //下列参数与几何因素相关，绘制某些视图时需要
    p->SignalChannel[i].bCircleFlag=0;//椭圆标志	否	p->SignalChannel[i]. 	0或1	0，非椭圆；1，椭圆
    p->SignalChannel[i].fAxa=400;//椭圆长轴	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].fAxb=300;//椭圆短轴	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].byTrustDiretion=0;	//涡流探头推进方向	否	p->SignalChannel[i]. 	0或1	0，靠近；1，远离
    p->SignalChannel[i].fGapLow=600;	//间隙低	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].fGapHigh=100;	//间隙高	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].bEn=0;//测量
    p->SignalChannel[i].nCalcMode=0;//传感器类型,用于上位机	否	INT32U 	0~2	不积分、一次积分、二次积分		
    p->SignalChannel[i].nOutputMode=0;
  }
  
  for(i=20;i<30;i++)
  {
    //	p->SignalChannel[i].nDatalth=_DEFAULT_DATA_LTH;
    //	p->nSignalChannelDataSize[i]=nSize;
    p->SignalChannel[i].nChNo=i;//通道序号	0~47	不修改	转化为槽号和通道号	
    p->SignalChannel[i].nChType=0;	//通道类型	0~255	位移、速度、加速度、温度、键相，可以扩展
    p->SignalChannel[i].fChFullScaleLow=0;	//量程低,用于原始数据的归一化处理
    p->SignalChannel[i].fChFullScaleHigh=12;	//量程高,用于原始数据的归一化处理
    p->SignalChannel[i].fSensority=100;	//传感器灵敏度	测量值与电压间的线性因子
    p->SignalChannel[i].fZeroVolt=0;	//零点电压	测量值零点对应的电压
    ///////	传感器类型	否	p->SignalChannel[i]. 	0~2	不积分、一次积分、二次积分
    ///////	测量值类型	是	p->SignalChannel[i]. 	0~3	pk，pk-pk，rms，avr
    ///测量值输出方式	否	p->SignalChannel[i]. 	0~2	opc，4~20ma设备，modbus，取消？
    p->SignalChannel[i]. bAlert=1;//报警允许	是	p->SignalChannel[i]. 	0或1	允许或不允许
    p->SignalChannel[i]. bAlertLock=1;	//报警锁定	否	p->SignalChannel[i]. 	0或1	锁定或不锁定
    p->SignalChannel[i]. fAlertLevelHigh=8;//警报	是	p->SignalChannel[i].	？	alert下限
    p->SignalChannel[i]. fDangerLevelHigh=10;//危险	是	p->SignalChannel[i].	？	danger下限
    p->SignalChannel[i]. nPickFrequency=20000;//采集频率	12模块将原始通道数据抽样得到指定频率下的数据
    p->SignalChannel[i].fFreqResponseLow=1;	//	频响下限	是	p->SignalChannel[i].	？	12模块已经规定，不修改
    p->SignalChannel[i].fFreqResponsehigh=1000;	//频响上限	是	p->SignalChannel[i].	？	12模块已经规定，不修改
    ////p->SignalChannel[i].2滤波范围	是	p->SignalChannel[i].	？	?
    p->SignalChannel[i].bSync=1;//同步标志	是	p->SignalChannel[i]. 	0或1	1，同步；0，异步
    p->SignalChannel[i].fFreqencyRes=1;//频域分辨率,异步模式下代表频域分辨率，同步模式为采样长度
    p->SignalChannel[i].nJkeyNo=2;	//键相通道序号 	是	p->SignalChannel[i]. 	0~？	
    
    p->SignalChannel[i].nInstallAngle=90*(i-i/2*2);//传感器安装角度,	0~360
    //下列参数与几何因素相关，绘制某些视图时需要
    p->SignalChannel[i].bCircleFlag=0;//椭圆标志	否	p->SignalChannel[i]. 	0或1	0，非椭圆；1，椭圆
    p->SignalChannel[i].fAxa=400;//椭圆长轴	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].fAxb=300;//椭圆短轴	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].byTrustDiretion=0;	//涡流探头推进方向	否	p->SignalChannel[i]. 	0或1	0，靠近；1，远离
    p->SignalChannel[i].fGapLow=600;	//间隙低	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].fGapHigh=100;	//间隙高	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].bEn=0;//测量
    p->SignalChannel[i].nCalcMode=0;//传感器类型,用于上位机	否	INT32U 	0~2	不积分、一次积分、二次积分		
    p->SignalChannel[i].nOutputMode=0;
  }
  for(i=30;i<40;i++)
  {
    //	p->SignalChannel[i].nDatalth=_DEFAULT_DATA_LTH;
    //	p->nSignalChannelDataSize[i]=nSize;
    p->SignalChannel[i].nChNo=i;//通道序号	0~47	不修改	转化为槽号和通道号	
    p->SignalChannel[i].nChType=0;	//通道类型	0~255	位移、速度、加速度、温度、键相，可以扩展
    p->SignalChannel[i].fChFullScaleLow=-300;	//量程低,用于原始数据的归一化处理
    p->SignalChannel[i].fChFullScaleHigh=300;	//量程高,用于原始数据的归一化处理
    p->SignalChannel[i].fSensority=8;	//传感器灵敏度	测量值与电压间的线性因子
    p->SignalChannel[i].fZeroVolt=0;	//零点电压	测量值零点对应的电压
    ///////	传感器类型	否	p->SignalChannel[i]. 	0~2	不积分、一次积分、二次积分
    ///////	测量值类型	是	p->SignalChannel[i]. 	0~3	pk，pk-pk，rms，avr
    ///测量值输出方式	否	p->SignalChannel[i]. 	0~2	opc，4~20ma设备，modbus，取消？
    p->SignalChannel[i]. bAlert=1;//报警允许	是	p->SignalChannel[i]. 	0或1	允许或不允许
    p->SignalChannel[i]. bAlertLock=1;	//报警锁定	否	p->SignalChannel[i]. 	0或1	锁定或不锁定
    p->SignalChannel[i]. fAlertLevelLow=-200;//警报	是	p->SignalChannel[i].	？	alert下限
    p->SignalChannel[i]. fDangerLevelLow=-250;//危险	是	p->SignalChannel[i].	？	danger下限
    p->SignalChannel[i]. fAlertLevelHigh=200;//警报	是	p->SignalChannel[i].	？	alert下限
    p->SignalChannel[i]. fDangerLevelHigh=250;//危险	是	p->SignalChannel[i].	？	danger下限
    p->SignalChannel[i]. nPickFrequency=_12_CHANNEL_SAMPLE_RATE;//采集频率	12模块将原始通道数据抽样得到指定频率下的数据
    p->SignalChannel[i].fFreqResponseLow=1;	//	频响下限	是	p->SignalChannel[i].	？	12模块已经规定，不修改
    p->SignalChannel[i].fFreqResponsehigh=1000;	//频响上限	是	p->SignalChannel[i].	？	12模块已经规定，不修改
    ////p->SignalChannel[i].2滤波范围	是	p->SignalChannel[i].	？	?
    p->SignalChannel[i].bSync=1;//同步标志	是	p->SignalChannel[i]. 	0或1	1，同步；0，异步
    p->SignalChannel[i].fFreqencyRes=1;//频域分辨率,异步模式下代表频域分辨率，同步模式为采样长度
    p->SignalChannel[i].nJkeyNo=3;	//键相通道序号 	是	p->SignalChannel[i]. 	0~？	
    
    p->SignalChannel[i].nInstallAngle=90*(i-i/2*2);//传感器安装角度,	0~360
    //下列参数与几何因素相关，绘制某些视图时需要
    p->SignalChannel[i].bCircleFlag=0;//椭圆标志	否	p->SignalChannel[i]. 	0或1	0，非椭圆；1，椭圆
    p->SignalChannel[i].fAxa=400;//椭圆长轴	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].fAxb=300;//椭圆短轴	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].byTrustDiretion=0;	//涡流探头推进方向	否	p->SignalChannel[i]. 	0或1	0，靠近；1，远离
    p->SignalChannel[i].fGapLow=600;	//间隙低	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].fGapHigh=100;	//间隙高	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].bEn=0;//测量
    p->SignalChannel[i].nCalcMode=0;//传感器类型,用于上位机	否	INT32U 	0~2	不积分、一次积分、二次积分		
    p->SignalChannel[i].nOutputMode=0;
  }
  
  for(i=40;i<p->nSignChannelNum;i++)//温度
  {
    //	p->SignalChannel[i].nDatalth=_DEFAULT_DATA_LTH;
    //	p->nSignalChannelDataSize[i]=nSize;
    p->SignalChannel[i].nChNo=i;//通道序号	0~47	不修改	转化为槽号和通道号	
    p->SignalChannel[i].nChType=2;	//通道类型	0~255	位移、速度、加速度、温度、键相，可以扩展
    p->SignalChannel[i].fChFullScaleLow=-300;	//量程低,用于原始数据的归一化处理
    p->SignalChannel[i].fChFullScaleHigh=300;	//量程高,用于原始数据的归一化处理
    p->SignalChannel[i].fSensority=8;	//传感器灵敏度	测量值与电压间的线性因子
    p->SignalChannel[i].fZeroVolt=0;	//零点电压	测量值零点对应的电压
    ///////	传感器类型	否	p->SignalChannel[i]. 	0~2	不积分、一次积分、二次积分
    ///////	测量值类型	是	p->SignalChannel[i]. 	0~3	pk，pk-pk，rms，avr
    ///测量值输出方式	否	p->SignalChannel[i]. 	0~2	opc，4~20ma设备，modbus，取消？
    p->SignalChannel[i]. bAlert=1;//报警允许	是	p->SignalChannel[i]. 	0或1	允许或不允许
    p->SignalChannel[i]. bAlertLock=1;	//报警锁定	否	p->SignalChannel[i]. 	0或1	锁定或不锁定
    p->SignalChannel[i]. fAlertLevelLow=-200;//警报	是	p->SignalChannel[i].	？	alert下限
    p->SignalChannel[i]. fDangerLevelLow=-250;//危险	是	p->SignalChannel[i].	？	danger下限
    p->SignalChannel[i]. fAlertLevelHigh=200;//警报	是	p->SignalChannel[i].	？	alert下限
    p->SignalChannel[i]. fDangerLevelHigh=250;//危险	是	p->SignalChannel[i].	？	danger下限
    p->SignalChannel[i]. nPickFrequency=20000;//采集频率	12模块将原始通道数据抽样得到指定频率下的数据
    p->SignalChannel[i].fFreqResponseLow=1;	//	频响下限	是	p->SignalChannel[i].	？	12模块已经规定，不修改
    p->SignalChannel[i].fFreqResponsehigh=1000;	//频响上限	是	p->SignalChannel[i].	？	12模块已经规定，不修改
    ////p->SignalChannel[i].2滤波范围	是	p->SignalChannel[i].	？	?
    p->SignalChannel[i].bSync=1;//同步标志	是	p->SignalChannel[i]. 	0或1	1，同步；0，异步
    p->SignalChannel[i].fFreqencyRes=1;//频域分辨率,异步模式下代表频域分辨率，同步模式为采样长度
    p->SignalChannel[i].nJkeyNo=0;	//键相通道序号 	是	p->SignalChannel[i]. 	0~？	
    
    p->SignalChannel[i].nInstallAngle=90*(i-i/2*2);//传感器安装角度,	0~360
    //下列参数与几何因素相关，绘制某些视图时需要
    p->SignalChannel[i].bCircleFlag=0;//椭圆标志	否	p->SignalChannel[i]. 	0或1	0，非椭圆；1，椭圆
    p->SignalChannel[i].fAxa=400;//椭圆长轴	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].fAxb=300;//椭圆短轴	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].byTrustDiretion=0;	//涡流探头推进方向	否	p->SignalChannel[i]. 	0或1	0，靠近；1，远离
    p->SignalChannel[i].fGapLow=600;	//间隙低	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].fGapHigh=100;	//间隙高	否	p->SignalChannel[i].	？	
    p->SignalChannel[i].bEn=0;//测量
    p->SignalChannel[i].nCalcMode=0;//传感器类型,用于上位机	否	INT32U 	0~2	不积分、一次积分、二次积分		
    p->SignalChannel[i].nOutputMode=1;
  }
  return re;
}
#endif

INT32S Read12Param(void* p12)
{
  INT32S re=0;
#ifdef _SIMU
  re=Simu12Param(p12);
#else
  re=ReadSignalChannelParam(p12);
#endif
  return re;
}

INT32S	Init83Param(void* p83in,void* p12in)
{
  INT32S	re=0,i=0,j=0,k=0;
  struct tag83Param* p=p83in;
  struct tag83Param* p83=p83in;
  struct tag12Param* p12=p12in;
  
  
  //初始化
  p->nDeviceNo=10000;	//设备序号;
  p->nDeviceProperty=2060;	//设备属性；0 数据采集卡 6 过程量输入模块 4 电流模块 5 继电器模块；
  p->nDeviceTypeID=2060;	//设备类型ID;
  p->nDeviceID=_BASIC_DEVIEID+1;//设备ID;
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
    p->Device[i].nDeviceNo=0;//设备序号;
    p->Device[i].nDeviceProperty=p->nDeviceProperty+i;//设备属性；0 数据采集卡 6 过程量输入模块 4 电流模块 5 继电器模块；
    p->Device[i].nDeviceTypeID=p->nDeviceTypeID+i;//设备类型ID;
    p->Device[i].nDeviceID=p->nDeviceID+i;//设备ID;
    
    p->Device[i].nKeyChno=i;
    p->Device[i].bAlertTrigerStatic=1;//报警触发静态，1，报警触发，0，无效
    p->Device[i].nStaticAutoPickTime=_DEFAULT_STATIC_AUTO_PICK_FREQUENCY	;//自动静态采集时间间隔，大于0有效
    p->Device[i].bAlertTrigerDynamic=1;//报警触发动态，1，报警触发，0，无效
    p->Device[i].nDynamicAutoPickTime=_DEFAULT_DYNAMIC_AUTO_PICK_FREQUENCY	;//自动动态采集时间间隔，大于0有效
    p->Device[i].nPickTimeScopePre=_DEFAULT_PRE_TRANTRIGER_TIMESPAN;//触发前采样时间长度，单位为毫秒
    
    p->Device[i].nSampleRPMInterval=5;//抽样转速间隔，单位为1转/分
    p->Device[i].nSampleTimeInterval=100;//不知道
    p->Device[i].nPickTimeIntervalPre=_DEFAULT_PRE_TRANTRIGER_TIME_GAP;//临时数据默认每一秒钟抽样10次，即1抽1
    
    p->Device[i].nTranPreTrigerGroupcnt=p->Device[i].nPickTimeScopePre/p->Device[i].nPickTimeIntervalPre;
    
    p->Device[i].nMaxTranSampleTime=_MAX_TRAN_TRIGER_TIMESPAN;//最大瞬态采集时间,按转速抽取时超过时间停止采集
    p->Device[i].nMaxTranSampleCnt=_MAX_TRAN_GROUP;//最大瞬态采集组数，目前为200
    
    p->Device[i].nSignChannelNum=0;//设备信号通道个数；
    
    p->Device[i].nAllDynamicChDataSize=0;
    p->Device[i].nAllTranChDataSize=0;
    
    p83->Device[i].nSignal_Channel_Dynamic_Data_Lth=16384;
    p83->Device[i].nSignal_Channel_Tran_Data_Lth=2048;
    p83->Device[i].nTranSampleMode=0;
  }
  for(i=0;i<_MAX_JKEY_CHANNEL_CNT;i++)//
  {
    p->Device[i].nFrequencyLow =_DEFAULT_FREQUENCY_RESPONSE_LOW;//上位机设定的频响下限
    p->Device[i].nFrequencyHigh =_DEFAULT_FREQUENCY_RESPONSE_HIGH;
    p->Device[i].nDynamicSampleMode =0;//0，异步采集；1，同步采集
    p->Device[i].nDynamicSynDataCntPerCycle =256;
    p->Device[i].nTranSynSampleMode =0;//0，异步采集；1，同步采集
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
  
  //从12模块参数初始化83模块参数,建立通道映射表
  
  p83->nDevicecnt=_MAX_JKEY_CHANNEL_CNT;//虚拟设备数量，同虚拟机器数量
  
  //	p83->nAllChDataSize=0;
  for(i=0;i<p12->nSignChannelNum;i++)//映射12通道号到83通道号,并根据12通道设置使能标志
  {
    for(j=0;j<p83->nDevicecnt;j++)
    {
      p83->Device[j].nKeyChno=j;//固定，不能修改
      
      if(p83->Device[j].nKeyChno==p12->SignalChannel[i].nJkeyNo)
      {
        if(p12->bWork[i])//该通道允许工作,映射到83通道表
        {
          p83->Device[j].nChannelNumIn12[p83->Device[j].nSignChannelNum]=i;//虚拟通道在12中的序号
          p12->nChannelNumIn83[i*2]=j;
          p12->nChannelNumIn83[i*2+1]=p83->Device[j].nSignChannelNum;
          p83->Device[j].nChannelMask[p83->Device[j].nSignChannelNum]=1;//1,允许，0禁止
          
          p83->nSignChannelNum++;//所有通道数加1
          p83->Device[j].nSignChannelNum++;//虚拟设备的通道数加1
          if(p83->Device[j].bAlertTrigerStatic)
          {
            p83->nStaticChannelNum++;//83的静态通道加1
            p83->Device[j].nStaticChannelNum++;//虚拟设备的静态通道数加1
          }
          if(p83->Device[j].bAlertTrigerDynamic&&\
            p83->Device[j].nSignal_Channel_Dynamic_Data_Lth)
          {
            p83->nDynamicChannelNum++;//83的动态通道加1
            p83->Device[j].nDynamicChannelNum++;//虚拟设备的动态通道数加1
          }
          if(p83->Device[j].KeyChannel[0].nTrigerDRPM&&
             p83->Device[j].nSignal_Channel_Tran_Data_Lth)//瞬态
          {
            p83->nTranChannelNum++;
            p83->Device[j].nTranChannelNum++;
          }
        }
        break;
      }
    }
  }
  //根据频响范围确定三种数据大小、抽样频率、采集时间等参数，生成地址计算用的各个参量	
  p83->nAllChValueSize=0;
  p83->nAllChStaticValueSize=0;//初始化三种测量值尺寸
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
    
    p83->Device[i].nChValueSize=sizeof(Tst_Head_DCM_SigModuSampData_SYS);//虚拟通道的测量值尺寸
    for(j=0;j<p83->Device[i].nSignChannelNum;j++)
    {
      p12->nChIndexMrsPt[p83->Device[i].nChannelNumIn12[j]]=p83->Device[i].nChIndexMrsPt[j];
      p12->nChDeviceChNo[p83->Device[i].nChannelNumIn12[j]][0]=i;
      p12->nChDeviceChNo[p83->Device[i].nChannelNumIn12[j]][1]=j;	
      p12->nMrsPt[p83->Device[i].nChannelNumIn12[j]]=p83->Device[i].nMrsPtNo[j];//虚拟通道所属测点
      
      p83->Device[i].nAllChValueSize+=p83->Device[i].nChValueSize;//虚拟设备的测量值尺寸	
      p83->Device[i].nChTranDataBlockCnt =\
        p83->Device[i].nSignal_Channel_Tran_Data_Lth /p83->Device[i].nChTranBlockSize ;	
      
      p83->Device[i].nAllDynamicChDataSize+=p83->Device[i].nChDynamicDataSize;
      p83->Device[i].nAllTranChDataSize+=p83->Device[i].nChTranDataSize;
      //	p12->nSignalChannelDynamicDataSize[p83->Device[i].nChannelNumIn12[j]]=\
      p83->Device[i].nSignal_Channel_Dynamic_Data_Lth[j];
      //	p12->nSignalChannelTranDataSize[p83->Device[i].nChannelNumIn12[j]]=\
      p83->Device[i].nSignal_Channel_Tran_Data_Lth[j];
    }
    //计算虚拟设备的三种数据测量值尺寸
    p83->Device[i].nAllChStaticValueSize=p83->Device[i].nStaticChannelNum*sizeof(Tst_Head_DCM_SigModuSampData_SYS);
    p83->Device[i].nAllChDynamicValueSize=p83->Device[i].nDynamicChannelNum*sizeof(Tst_Head_DCM_SigModuSampData_SYS);
    p83->Device[i].nAllChTranValueSize=p83->Device[i].nTranChannelNum*sizeof(Tst_Head_DCM_SigModuSampData_SYS);
    p83->Device[i].nAllChValueSize=p83->Device[i].nAllChStaticValueSize;
    
    //计算83的三种数据测量值尺寸
    p83->nAllChStaticValueSize+=p83->Device[i].nAllChStaticValueSize;
    p83->nAllChDynamicValueSize+=p83->Device[i].nAllChDynamicValueSize;
    p83->nAllChTranValueSize+=p83->Device[i].nAllChTranValueSize;
    p83->nAllChValueSize+=p83->Device[i].nAllChValueSize;//83模块的测量值尺寸
    
    //		p83->Device[i].nAllChDataSize=p83->Device[i].nAllDynamicChDataSize+p83->Device[i].nAllTranChDataSize;
    //		p83->nAllChDataSize+=p83->Device[i].nAllChDataSize;
    p83->nAllDynamicChDataSize+=p83->Device[i].nAllDynamicChDataSize;
    p83->nAllTranChDataSize+=p83->Device[i].nAllTranChDataSize;
  }
  
  /*设置12模块通道参数中与测点相关的参数*/
  p12->nMrsPtcnt=0;//设置测点数量
  
  for(i=0;i<p->nSignChannelNum;i++)//各虚拟设备键相通道号
  {
    re=0;
    p12->nMrsPt[i]=i/2;
    p12->nChIndexMrsPt[i]=i-i/2*2;
    for(j=0;j<p12->nMrsPtcnt;j++)//该测点是否已经在已知列表中
    {
      if(p12->nMrsPt[i]==j)//测点序号
      {
        re=1;//已知测点，将其索引影射到12通道测点索引列表中
        p12->nMrsPtIndex[i]=j;
        if(p12->nChIndexMrsPt[i]==0)
          p12->nMrsPtDataSize0[j]=_DEFAULT_RM_DATA_LTH+sizeof(struct tagChValue);
        p12->nMrsPtDataSize[j]+=_DEFAULT_RM_DATA_LTH+sizeof(struct tagChValue);
        p12->nMrsPtChcnt[j]++;//测点通道数加１
        break;
      }
    }
    if(re==0)//新测点
    {
      p12->nMrsPtTable[p12->nMrsPtcnt]=p12->nMrsPt[i];//测点加入到测点列表中
      if(p12->nChIndexMrsPt[i]==0)//是0号通道
        p12->nMrsPtDataSize0[p12->nMrsPtcnt]=_DEFAULT_RM_DATA_LTH+sizeof(struct tagChValue);
      p12->nMrsPtIndex[i]=p12->nMrsPtcnt;//索引影射到12通道测点索引列表中，测点数加1
      //测点数据尺寸初始化为通道数据尺寸
      p12->nMrsPtDataSize[p12->nMrsPtcnt]=_DEFAULT_RM_DATA_LTH+sizeof(struct tagChValue);
      p12->nMrsPtChcnt[p12->nMrsPtcnt]=1;//测点通道数初始化为1
      p12->nMrsPtcnt++;
    }
  }
  return re;
}

INT32S  CaltChannelParam(INT32S nKey,FP32 fFrequencyLow,FP32 fFrequencyHigh,INT32S nSampleSynMode,INT32S nSynSampleLth,
                         INT32S *pnPickDynamicFrequency,INT32S* pnDynamicChannelDataSampleStep,
                         INT32S* pnSignal_Channel_Dynamic_Data_Lth,INT32S* pnChDynamicBlockSize,
                         INT32S* pnChDynamicDataBlockCnt,INT32S* pnDynamicPickTime,INT32S* pnChDynamicDataSize,
                         INT32S* pnDynamicSynRPM)
{
  INT32S re=0;
  //	nSampleSynMode=1;
  if(nSampleSynMode==0)//异步模式
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
    pnChDynamicBlockSize[0] =_12_CHANNEL_SAMPLE_RATE*_12_DATA_UPLOAD_TIME_GAP/1000;//每次产生的新数据
    pnChDynamicBlockSize[0] =\
      pnChDynamicBlockSize[0]/pnDynamicChannelDataSampleStep[0]+1;//单次抽取数量
    pnChDynamicBlockSize[0] =\
      (pnChDynamicBlockSize[0] /_RAMDISK_CLUSTER_LTH+1)*_RAMDISK_CLUSTER_LTH;//按电子盘最小寻址块圆整
    pnDynamicPickTime[0] = 1000000/fFrequencyLow;//采集时间
    if(pnDynamicPickTime[0]>_DYNAMIC_ASYNCHRONOUS_MAX_PICK_TIME)
      pnDynamicPickTime[0]=_DYNAMIC_ASYNCHRONOUS_MAX_PICK_TIME;
    else if(pnDynamicPickTime[0]<_DYNAMIC_ASYNCHRONOUS_MIN_PICK_TIME)
      pnDynamicPickTime[0]=_DYNAMIC_ASYNCHRONOUS_MIN_PICK_TIME;
    pnChDynamicDataBlockCnt[0] =(pnDynamicPickTime[0]-_12_DATA_UPLOAD_TIME_GAP/2)/_12_DATA_UPLOAD_TIME_GAP+1;
    pnSignal_Channel_Dynamic_Data_Lth[0] = pnChDynamicBlockSize[0] * pnChDynamicDataBlockCnt[0];//动态长度圆整				
  }
  else//同步模式
  {
    pnDynamicChannelDataSampleStep[0]=_DEFAULT_DYNAMIC_SYN_DATA_SAMPLE_STEP;//同步默认抽样因子
    pnPickDynamicFrequency[0] = _12_CHANNEL_SAMPLE_RATE/pnDynamicChannelDataSampleStep[0];//同步默认采集频率
    
    pnChDynamicBlockSize[0] =_12_CHANNEL_SAMPLE_RATE*_12_DATA_UPLOAD_TIME_GAP/1000;//每次产生的新数据
    pnChDynamicBlockSize[0] =\
      pnChDynamicBlockSize[0]/pnDynamicChannelDataSampleStep[0];//单次抽取数量
    pnChDynamicBlockSize[0] =\
      (pnChDynamicBlockSize[0] /_RAMDISK_CLUSTER_LTH+1)*_RAMDISK_CLUSTER_LTH;//按电子盘最小寻址块圆整
    
    pnDynamicPickTime[0] = 1000000/fFrequencyLow;//采集时间
    if(pnDynamicPickTime[0]>_DYNAMIC_SYNCHRONOUS_MAX_PICK_TIME)
      pnDynamicPickTime[0]=_DYNAMIC_SYNCHRONOUS_MAX_PICK_TIME;
    else if(pnDynamicPickTime[0]<_DYNAMIC_SYNCHRONOUS_MIN_PICK_TIME)
      pnDynamicPickTime[0]=_DYNAMIC_SYNCHRONOUS_MIN_PICK_TIME;
    pnChDynamicDataBlockCnt[0] = pnDynamicPickTime[0]/_12_DATA_UPLOAD_TIME_GAP;//数据块数
    pnDynamicSynRPM[0]=(1000/_12_DATA_UPLOAD_TIME_GAP)*60*_MAX_DYNAMIC_SYN_SAMPLE_LTH/nSynSampleLth;//极限变频转速
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

INT32S	RenewParam(void* p83in,void* p12in)
{
  INT32S	re=0,i=0,j=0,k=0,temp;
  struct tag83Param* p83=p83in;
  struct tag12Param* p12=p12in;
  p83->nAllChValueSize=0;
  p83->nAllChStaticValueSize=0;//初始化三种测量值尺寸
  p83->nAllChDynamicValueSize=0;
  p83->nAllChTranValueSize=0;
  for(i=0;i<p83->nDevicecnt;i++)
  {
    p83->Device[i].nDeviceNo=i;
    //		p83->Device[i].nAllChDataSize=0;
    p83->Device[i].nAllDynamicChDataSize=0;
    p83->Device[i].nAllTranChDataSize=0;////瞬态模型跟随采集频率变化时需要打开，目前的模型无法实现瞬态同步采集功能
    p83->Device[i].nMinPreTranTickcnt=
      p83->Device[i].nPickTimeIntervalPre/_12_DATA_UPLOAD_TIME_GAP*p83->Device[i].nTranPreTrigerGroupcnt;
    re=CaltChannelParam(i,p83->Device[i].nFrequencyLow,p83->Device[i].nFrequencyHigh,
                        p83->Device[i].nDynamicSampleMode,p83->Device[i].nDynamicSynDataCntPerCycle,
                        &p83->Device[i].nPickDynamicFrequency,&p83->Device[i].nDynamicChannelDataSampleStep,
                        &p83->Device[i].nSignal_Channel_Dynamic_Data_Lth,&p83->Device[i].nChDynamicBlockSize,
                        &p83->Device[i].nChDynamicDataBlockCnt,&p83->Device[i].nDynamicPickTime,
                        &p83->Device[i].nChDynamicDataSize,&p83->Device[i].nDynamicSynRPM);
    //为瞬态同步采集预留的算法,可以调整根据要求频率调整抽取因子
    
    temp=p83->Device[i].nFrequencyHigh*_DEFAULT_HIGH_FREQUENCY_COF;
    p83->Device[i].nTranChannelDataSampleStep=_DEFAULT_TRAN_DATA_SAMPLE_STEP;
    p83->Device[i].nTranPickTime=100;
    p83->Device[i].nSignal_Channel_Tran_Data_Lth=_DEFAULT_TRAN_DATA_LTH;
#ifdef	_ADVANCED_TRAN_MODE //根据频响范围计算抽取因子和数据长度，为同步采集模式预留
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
      p12->nMrsPt[p83->Device[i].nChannelNumIn12[j]]=p83->Device[i].nMrsPtNo[j];//虚拟通道所属测点
      
      p83->Device[i].nChValueSize=sizeof(Tst_Head_DCM_SigModuSampData_SYS);//虚拟通道的测量值尺寸
      p83->Device[i].nAllChValueSize+=p83->Device[i].nChValueSize;//虚拟设备的测量值尺寸	
      
      p83->Device[i].nAllDynamicChDataSize+=p83->Device[i].nChDynamicDataSize;
      p83->Device[i].nAllTranChDataSize+=p83->Device[i].nChTranDataSize;
      //填充12数据通道长度
      //	p12->nSignalChannelDynamicDataSize[p83->Device[i].nChannelNumIn12[j]]=\
      p83->Device[i].nSignal_Channel_Dynamic_Data_Lth[j];
      //	p12->nSignalChannelTranDataSize[p83->Device[i].nChannelNumIn12[j]]=\
      p83->Device[i].nSignal_Channel_Tran_Data_Lth[j];
    }
    //计算虚拟设备的三种数据测量值尺寸
    p83->Device[i].nAllChStaticValueSize=p83->Device[i].nStaticChannelNum*sizeof(Tst_Head_DCM_SigModuSampData_SYS);
    p83->Device[i].nAllChDynamicValueSize=p83->Device[i].nDynamicChannelNum*sizeof(Tst_Head_DCM_SigModuSampData_SYS);
    p83->Device[i].nAllChTranValueSize=p83->Device[i].nTranChannelNum*sizeof(Tst_Head_DCM_SigModuSampData_SYS);
    p83->Device[i].nAllChValueSize=p83->Device[i].nAllChStaticValueSize;
    
    //计算83的三种数据测量值尺寸
    p83->nAllChStaticValueSize+=p83->Device[i].nAllChStaticValueSize;
    p83->nAllChDynamicValueSize+=p83->Device[i].nAllChDynamicValueSize;
    p83->nAllChTranValueSize+=p83->Device[i].nAllChTranValueSize;
    p83->nAllChValueSize+=p83->Device[i].nAllChValueSize;//83模块的测量值尺寸
    
    //	p83->Device[i].nAllChDataSize=p83->Device[i].nAllDynamicChDataSize+p83->Device[i].nAllTranChDataSize;
    //	p83->nAllChDataSize+=p83->Device[i].nAllChDataSize;
    p83->nAllDynamicChDataSize+=p83->Device[i].nAllDynamicChDataSize;
    p83->nAllTranChDataSize+=p83->Device[i].nAllTranChDataSize;
    
  }
  
  /*12模块测点-通道映射相关的参数设置*/
  p12->nMrsPtcnt=0;//设置测点数量
  
  for(i=0;i<p12->nSignChannelNum;i++)//遍历所有12模块通道
  {
    re=0;
    
    for(j=0;j<p12->nMrsPtcnt;j++)//该测点是否已经在已知测点列表中
    {
      if(p12->nMrsPt[i]==p12->nMrsPtTable[j])//通道绑定的测点号在测点列表中
      {//已知测点，将其索引影射到12通道测点索引列表中
        re=1;
        p12->nMrsPtIndex[i]=j;
        if(p12->nChIndexMrsPt[i]==0)
          p12->nMrsPtDataSize0[j]=_DEFAULT_RM_DATA_LTH+sizeof(struct tagChValue);
        p12->nMrsPtDataSize[j]+=_DEFAULT_RM_DATA_LTH+sizeof(struct tagChValue);
        p12->nMrsPtChcnt[j]++;//测点通道数加１
        break;
      }
    }
    if(re==0)//新测点
    {
      p12->nMrsPtTable[p12->nMrsPtcnt]=p12->nMrsPt[i];//测点加入到测点列表中
      if(p12->nChIndexMrsPt[i]==0)//是0号通道
        p12->nMrsPtDataSize0[p12->nMrsPtcnt]=_DEFAULT_RM_DATA_LTH+sizeof(struct tagChValue);
      p12->nMrsPtIndex[i]=p12->nMrsPtcnt;//索引影射到12通道测点索引列表中，测点数加1
      //测点数据尺寸初始化为通道数据尺寸
      p12->nMrsPtDataSize[p12->nMrsPtcnt]=_DEFAULT_RM_DATA_LTH+sizeof(struct tagChValue);
      p12->nMrsPtChcnt[p12->nMrsPtcnt]=1;//测点通道数初始化为1
      p12->nMrsPtcnt++;
    }
  }
  return re;
}

//设置通道原始数据当前偏移
INT32S	SetRamDiskChDataOffset(INT32S nStartChannel,INT32S nChannelcnt,INT32S pData[])//arm 调用，设置fpga各个通道数据在电子盘的写入地址
{
  INT32S re=0;
  Mem_Copy(g_nFpgaDynamicChDataOffset+nStartChannel,pData,sizeof(INT32S)*nChannelcnt);
  return re;
}

//设置各个通道的抽样频率
INT32S	SetChDataSampleStep(INT32S nStartChannel,INT32S nChannelcnt,INT32S pData[])
{
  INT32S re=0;
  Mem_Copy(g_nFpgaDynamicChDataSampleStep+nStartChannel,pData,sizeof(INT32S)*nChannelcnt);
  return re;
}

//arm调用读取通道数据头
INT32S ReadChValue(Tst_Head_DCM_SigModuSampData_SYS* p,INT32S nStartChannel,INT32S nChannelcnt)
{
  INT32S re=0;
  Mem_Copy(p,g_TempValue+nStartChannel,sizeof(Tst_Head_DCM_SigModuSampData_SYS)*nChannelcnt);
  return re;
}

//arm调用读取通道原始数据
INT32S ReadRamDisk(INT32U nOffset,INT32S pData[],INT32S nLength)
{
  INT32S re=0;
  Mem_Copy(pData,g_pChData+nOffset,nLength);
  return re;
}

//simu和main83模块分别调用来构建自己的到达次序-通道地址表：分组，各组的依次排列的通道序号
INT32S	BuildSignalGroupTable(INT32S nChannelSit[])
{
  INT32S re=0,i,j;
  re=_MAX_SIGNAL_CHANNEL_CNT/_FPGA_GROUP_CHANNEL_CNT;
  for(i=0;i<re;i++)
  {
    for(j=0;j<_FPGA_GROUP_CHANNEL_CNT;j++)
    {
      nChannelSit[i*_FPGA_GROUP_CHANNEL_CNT+j]=j*re+i;
    }
  }
  return re;
}

//arm调用读取键相通道当前速度
INT32S ReadKeySpeed(INT32U nChannelcnt,INT32U nChannel[],INT32S pData[])
{
  INT32S re=0,i,j;
  for(i=0;i<nChannelcnt;i++)
  {
    for(j=0;j<_MAX_JKEY_CHANNEL_CNT;j++)
    {
      if(nChannel[i]==j)
      {
        pData[i]=g_nCaltKeySpeed[j];
        break;
      }
    }
  }
  return re;
}
//arm调用读取键相通道新产生的键相点,目前不需要，保留
INT32S ReadKeyPointCnt(INT32U nChannelcnt,INT32U nChannel[],INT32S pData[])
{
  INT32S re=0;
  
  return re;
}
