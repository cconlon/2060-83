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
//extern INT32S	g_Signal_Channel_Dynamic_Data_Lth[];//����ͨ���Ķ�̬���ݳ���
//extern INT32S	g_Signal_Channel_Tran_Data_Lth[];//����ͨ����˲̬̬���ݳ���
//extern INT32U	g_nDynamicChDataOffset[_MAX_SIGNAL_CHANNEL_CNT];//��̬���ݵ����̵�ǰд��λ��
//extern INT32U	g_nTranhDataOffset[_MAX_SIGNAL_CHANNEL_CNT];//˲̬���ݵ����̵�ǰд��λ��
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
//arm���ö�ȡͨ������ͷ
INT32S ReadChValue(Tst_Head_DCM_SigModuSampData_SYS* p,INT32S nStartChannel,INT32S nChannelcnt);
//arm���ö�ȡͨ��ԭʼ����
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
extern Tst_Head_DCM_SigModuSampData_SYS g_TempValue[_MAX_SIGNAL_CHANNEL_CNT];//������FPGA����ͷ����ʱ�洢����
extern INT8S* g_pChData;

extern INT32S		g_nCaltKeySpeed[_MAX_JKEY_CHANNEL_CNT];
INT32S Simu12Param(void* p12);
//#undef _SIMU
#ifndef _SIMU
typedef struct TagACTUAL_OTHER_PARA_TABLE_STRUCT
{
  INT32S nXXEnable;                       //XX�Ƿ���á�
  INT32S nXXAlertEnable;                  //XX Alert�Ƿ���á�
  INT32S nXXDangerEnable;                 //XX Danger�Ƿ���á�
  FP32 dXXFullScaleHigh;            //XX�����̸ߡ�
  FP32 dXXFullScaleLow;             //XX�����̵͡�
  FP32 dXXAlertHigh;                //XX�����ߡ�
  FP32 dXXAlertHighHysteresis;      //XX������Hysteresis��
  FP32 dXXAlertLow;                 //XX�����͡�
  FP32 dXXAlertLowHysteresis;       //XX������Hysteresis��
  FP32 dXXDangerHigh;               //XXΣ�ոߡ�
  FP32 dXXDangerHighHysteresis;     //XXΣ�ո�Hysteresis��
  FP32 dXXDangerLow;                //XXΣ�յ͡�
  FP32 dXXDangerLowHysteresis;      //XXΣ�յ�Hysteresis��
}ACTUAL_OTHER_PARA_TABLE_STRUCT;
//##ModelId=484F74B9030E
typedef struct TagSIGNAL_MODULE_PARAMETER_STRUCT{
  INT32S nChannelTypeID;                    //ͨ������ID��
  INT32S nSensorTypeID;                     //����������ID��
  INT32S nChannelEnable;                    //ͨ���Ƿ���á�
  INT32S nAlermLatch;                       //ͨ���Ƿ񱨾����档
  FP32 dSensitivity;                     //������������
  FP32 dAlertDelay;                      //ALERT������ʱ��s
  FP32 dDangerDelay;                     //DANGER������ʱ��s
  INT32S nOneHundredMs;                       //100ms�Ƿ���Ч��0:��Ч��1��Ч��
  FP32 dTeethPerCycle;                   //ÿ�ܳ�����
  FP32 dZeroPosition;                    //���λ�á�v
  FP32 dHysteresis;                      //�زv
  FP32 dTriggerVoltage;                  //������ѹ��v
  FP32 dFullScaleHigh;                //�����̸ߡ�
  FP32 dFullScaleLow;                 //�����̵͡�
  INT32S nMeasurementTypeID;               //��������ID��
  INT32S nMeasurementUnitID;               //������λID��
  INT32S nAlertMeasurementTypeID;          //ѡ��ALERT��������Ӧ�Ĳ�����ʽID��
  INT32S nPhaseReferenceID;                //����ο�ID�� 
  FP32 dDangerHigh;                   //Σ�ոߡ�
  FP32 dAlertHigh;                    //�����ߡ�
  FP32 dAlertLow;                     //�����͡�
  FP32 dDangerLow;                    //Σ�յ͡�
  FP32 dGapHigh;                      //��϶��ѹ�ߡ�
  FP32 dGapLow;                       //��϶��ѹ�͡�
  INT32S nTransducerDirection;             //��������װ����0������1������
  INT32S nLeadingTransducer;               //������������0��ͨ��1��2��ͨ��2��
  INT32S nUpscaleDirection;                //Upscale����0������1������
  INT32S nOneOfUpThreePara;                //��������ֻѡ��һ��0��TransducerDirection��1��LeadingTransducer��2��UpscaleDirection��
  INT32S nThresholdType;                   //�ż����͡�0���ֶ���1���Զ���
  INT32S nBarrier;                         //�Ƿ�ʹ�÷�������դ��
  INT32S nSampleRate;                      //��������ID��
  FP32 dDangerHighHysteresis;         //Σ�ո߱����ز
  FP32 dAlertHighHysteresis;          //�����߱����ز
  FP32 dAlertLowHysteresis;           //�����ͱ����ز
  FP32 dDangerLowHysteresis;          //Σ�յͱ����ز
  FP32 dGapAlertHysteresis;         //GAP�����ز
  FP32 dAngleOfSensorFix;           //��������װ�Ƕȡ�
  FP32 dLowPassFilter;				//��ͨ�˲�Ƶ�ʡ�
  FP32 dHighPassFilter;				 //��ͨ�˲�Ƶ�ʡ�
  //LG:2009-09-09 ���12ģ������ӵĲ���///////////////////////////////////////////////////////////////
  INT32S nOverallAlertEnable;              //Overall Alert�Ƿ���á�
  INT32S nOverallDangerEnable;             //Overall Danger�Ƿ���á�
  INT32S nModbusOutputTypeID;            //MODBUS OUTPUT TYPE ID
  INT32S n4To20mAOutputTypeID;              //4~20ma OUTPUT TYPE ID��
  
  INT32S nIntegrateEnable;                   //�Ƿ���֡�
  
  INT32S nHighPassFilterEnable;            //��ͨ�˲��Ƿ���á�
  INT32S nLowPassFilterEnable;            //��ͨ�˲��Ƿ���á�
  
  ACTUAL_OTHER_PARA_TABLE_STRUCT struActualOtherParaTable[7]; //����������0��1XA��1��1XP��2��2XA��3:2XP; 4:NOT1X;5:SMAX;6:GAP��
  //LG:2009-09-09 ���12ģ������ӵĲ���///////////////////////////////////////////////////////////////	
  INT32S nReserve[MAX_RESERVES];             //INT32S������
  FP32 dReserve[MAX_RESERVES];          //double������
  //LG:2009-09-09 ���12ģ������ӵĲ���///////////////////////////////////////////////////////////////
}SIGNAL_MODULE_PARAMETER_STRUCT;
INT32S	g_nCurWriteSit=0;//д��λ�ã������ݽ����������
INT32S	g_nCurReadSit=0;//��ȡλ�ã������ݶ�ȡ��������

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
  //LG:2009-09-09 ���12ģ������ӵĲ���///////////////////////////////////////////////////////////////
  p->nOverallAlertEnable=0;              //Overall Alert�Ƿ���á�
  p->nOverallDangerEnable=0;             //Overall Danger�Ƿ���á�
  p->nModbusOutputTypeID=-1;            //MODBUS OUTPUT TYPE ID
  p->n4To20mAOutputTypeID=-1;              //4~20ma OUTPUT TYPE ID��
  
  p->nIntegrateEnable=0;                   //�Ƿ���֡�
  
  p->nHighPassFilterEnable=0;            //��ͨ�˲��Ƿ���á�
  p->nLowPassFilterEnable=0;            //��ͨ�˲��Ƿ���á�
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
      BOOL bNeedChange;//��30ģ��ĵ�λ�����⴦��
      TCHAR  szName[128];
      g_SignalChannelParam[nChannel].nSampleRate=dwPara[0];                         //40200�����ʡ�
      g_SignalChannelParam[nChannel].nMeasurementTypeID=dwPara[2];                  //40202�������͡�
      if(nUnitType == 1 && dwPara[3] == 9)
      {
        g_SignalChannelParam[nChannel].nMeasurementUnitID=11;                         //40203������λ��
        bNeedChange=TRUE;
      }
      else
      {
        g_SignalChannelParam[nChannel].nMeasurementUnitID=dwPara[3];                  //40203������λ��
        bNeedChange=FALSE;
      }
      g_SignalChannelParam[nChannel].nSensorTypeID=dwPara[4];                       //40204����������ID��
      
      switch(nSensorType)
      {
      case 0:
        g_SignalChannelParam[nChannel].dSensitivity=(INT16U)dwPara[5];                                   //40205 �����������ȡ�
        break;
      case 1:
        g_SignalChannelParam[nChannel].dSensitivity=(INT16U)dwPara[5]/1000.0f;									    //40205 �����������ȡ�
        break;
      case 2:
        g_SignalChannelParam[nChannel].dSensitivity=(INT16U)dwPara[5]/1000.0f;                                   //40205 �����������ȡ�
        break;
      case 3:
        g_SignalChannelParam[nChannel].dSensitivity=(INT16U)dwPara[5]/1000.0f;                                   //40205 �����������ȡ�
        break;
      case 4:
        g_SignalChannelParam[nChannel].dSensitivity=(INT16U)dwPara[5]/1000.0f;                                   //40205 �����������ȡ�
        break;
      }
      
      nExponent=(INT16S)dwPara[27];                                                             //40227 ���õ����ָ��ֵ��
      
      g_SignalChannelParam[nChannel].nTransducerDirection=dwPara[6];                                    //40206 ����������
      g_SignalChannelParam[nChannel].nAlertMeasurementTypeID=dwPara[7];                                 //40207 �����������͡�
      if(dwPara[3] == 7 || dwPara[3] == 8)
      {
        g_SignalChannelParam[nChannel].dDangerHigh=(INT16U)dwPara[8]*pow(10,nExponent);                              //40208 Σ�ոߡ�
        g_SignalChannelParam[nChannel].dDangerHighHysteresis=(INT16U)dwPara[9]*pow(10,nExponent);                    //40209 Σ�ո߻ز
        g_SignalChannelParam[nChannel].dDangerLow=(INT16U)dwPara[10]*pow(10,nExponent);                              //40210 Σ�յ͡�
        g_SignalChannelParam[nChannel].dDangerLowHysteresis=(INT16U)dwPara[11]*pow(10,nExponent);                    //40211 Σ�յͻز
        g_SignalChannelParam[nChannel].dAlertLow=(INT16U)dwPara[12]*pow(10,nExponent);                               //40212 �����͡�
        g_SignalChannelParam[nChannel].dAlertLowHysteresis=(INT16U)dwPara[13]*pow(10,nExponent);                     //40213 �����ͻز
        g_SignalChannelParam[nChannel].dAlertHigh=(INT16U)dwPara[14]*pow(10,nExponent);                              //40214 �����ߡ�
        g_SignalChannelParam[nChannel].dAlertHighHysteresis=(INT16U)dwPara[15]*pow(10,nExponent);                    //40215 �����߻ز
        g_SignalChannelParam[nChannel].dFullScaleHigh=(INT16U)dwPara[19]*pow(10,nExponent);                          //40219 �����̸ߡ�
        g_SignalChannelParam[nChannel].dFullScaleLow=(INT16U)dwPara[26]*pow(10,nExponent);                      //40226 �����̵͡�
      }
      else
      {
        if(bNeedChange == TRUE)
        {
          g_SignalChannelParam[nChannel].dDangerHigh=(INT16S)dwPara[8]*pow(10,nExponent)*1.8+32;                              //40208 Σ�ոߡ�
          g_SignalChannelParam[nChannel].dDangerHighHysteresis=(INT16S)dwPara[9]*pow(10,nExponent)*1.8+32;                    //40209 Σ�ո߻ز
          g_SignalChannelParam[nChannel].dDangerLow=(INT16S)dwPara[10]*pow(10,nExponent)*1.8+32;                              //40210 Σ�յ͡�
          g_SignalChannelParam[nChannel].dDangerLowHysteresis=(INT16S)dwPara[11]*pow(10,nExponent)*1.8+32;                    //40211 Σ�յͻز
          g_SignalChannelParam[nChannel].dAlertLow=(INT16S)dwPara[12]*pow(10,nExponent)*1.8+32;                               //40212 �����͡�
          g_SignalChannelParam[nChannel].dAlertLowHysteresis=(INT16S)dwPara[13]*pow(10,nExponent)*1.8+32;                     //40213 �����ͻز
          g_SignalChannelParam[nChannel].dAlertHigh=(INT16S)dwPara[14]*pow(10,nExponent)*1.8+32;                              //40214 �����ߡ�
          g_SignalChannelParam[nChannel].dAlertHighHysteresis=(INT16S)dwPara[15]*pow(10,nExponent)*1.8+32;                    //40215 �����߻ز
          g_SignalChannelParam[nChannel].dFullScaleHigh=(INT16S)dwPara[19]*pow(10,nExponent)*1.8+32;                          //40219 �����̸ߡ�
          g_SignalChannelParam[nChannel].dFullScaleLow=(INT16S)dwPara[26]*pow(10,nExponent)*1.8+32;                      //40226 �����̵͡�
        }
        else
        {
          g_SignalChannelParam[nChannel].dDangerHigh=(INT16S)dwPara[8]*pow(10,nExponent);                              //40208 Σ�ոߡ�
          g_SignalChannelParam[nChannel].dDangerHighHysteresis=(INT16S)dwPara[9]*pow(10,nExponent);                    //40209 Σ�ո߻ز
          g_SignalChannelParam[nChannel].dDangerLow=(INT16S)dwPara[10]*pow(10,nExponent);                              //40210 Σ�յ͡�
          g_SignalChannelParam[nChannel].dDangerLowHysteresis=(INT16S)dwPara[11]*pow(10,nExponent);                    //40211 Σ�յͻز
          g_SignalChannelParam[nChannel].dAlertLow=(INT16S)dwPara[12]*pow(10,nExponent);                               //40212 �����͡�
          g_SignalChannelParam[nChannel].dAlertLowHysteresis=(INT16S)dwPara[13]*pow(10,nExponent);                     //40213 �����ͻز
          g_SignalChannelParam[nChannel].dAlertHigh=(INT16S)dwPara[14]*pow(10,nExponent);                              //40214 �����ߡ�
          g_SignalChannelParam[nChannel].dAlertHighHysteresis=(INT16S)dwPara[15]*pow(10,nExponent);                    //40215 �����߻ز
          g_SignalChannelParam[nChannel].dFullScaleHigh=(INT16S)dwPara[19]*pow(10,nExponent);                          //40219 �����̸ߡ�
          g_SignalChannelParam[nChannel].dFullScaleLow=(INT16S)dwPara[26]*pow(10,nExponent);                      //40226 �����̵͡�
        }
      }
      /*
      //���߷� 2009-07-15 ���35ģ��� ����ͨ��(30) �� ��ѹͨ��(31)
      //////////////////////////
      if(dwPara[25]==30 || dwPara[25] == 31)
      {
      g_SignalChannelParam[nChannel].dDangerHigh=(DINT16U)dwPara[8]*pow(10,nExponent);                              //40208 Σ�ոߡ�
      g_SignalChannelParam[nChannel].dDangerHighHysteresis=(DINT16U)dwPara[9]*pow(10,nExponent);                    //40209 Σ�ո߻ز
      g_SignalChannelParam[nChannel].dDangerLow=(DINT16U)dwPara[10]*pow(10,nExponent);                              //40210 Σ�յ͡�
      g_SignalChannelParam[nChannel].dDangerLowHysteresis=(DINT16U)dwPara[11]*pow(10,nExponent);                    //40211 Σ�յͻز
      g_SignalChannelParam[nChannel].dAlertLow=(DINT16U)dwPara[12]*pow(10,nExponent);                               //40212 �����͡�
      g_SignalChannelParam[nChannel].dAlertLowHysteresis=(DINT16U)dwPara[13]*pow(10,nExponent);                     //40213 �����ͻز
      g_SignalChannelParam[nChannel].dAlertHigh=(DINT16U)dwPara[14]*pow(10,nExponent);                              //40214 �����ߡ�
      g_SignalChannelParam[nChannel].dAlertHighHysteresis=(DINT16U)dwPara[15]*pow(10,nExponent);                    //40215 �����߻ز
      g_SignalChannelParam[nChannel].dFullScaleHigh=(DINT16U)dwPara[19]*pow(10,nExponent);                          //40219 �����̸ߡ�
      g_SignalChannelParam[nChannel].dFullScaleLow=(DINT16U)dwPara[26]*pow(10,nExponent);                      //40226 �����̵͡�
    }
      /////////////////////////////
      */
      g_SignalChannelParam[nChannel].dGapHigh=(INT16S)dwPara[16]*pow(10,-3);                                         //40216 GAP�ߡ�
      g_SignalChannelParam[nChannel].dGapLow=(INT16S)dwPara[17]*pow(10,-3);                                          //40217 GAP�͡�
      g_SignalChannelParam[nChannel].dGapAlertHysteresis=(INT16S)dwPara[18]*pow(10,-3);                              //40218 GAP�ز
      g_SignalChannelParam[nChannel].dAlertDelay=(INT16U)dwPara[20];                                            //40220 Alert�����ӳ١�
      g_SignalChannelParam[nChannel].dDangerDelay=(INT16U)dwPara[21];                                           //40221 Danger�����ӳ١�
      g_SignalChannelParam[nChannel].nAlermLatch=dwPara[22];                                            //40222 ����������
      g_SignalChannelParam[nChannel].nChannelEnable=dwPara[23];                                         //40223 ͨ���Ƿ�����
      g_SignalChannelParam[nChannel].nPhaseReferenceID=dwPara[24];                                      //40224 ͨ��������ź�ID��
      g_SignalChannelParam[nChannel].nChannelTypeID=dwPara[25];                                         //40225 ͨ������ID��
      
      //LG:2009-08-28 31ģ������λ���ϴ������10
      /*	if(this->m_nModuleID == 16)//·ռ����2012-11-16
      g_SignalChannelParam[nChannel].dZeroPosition=(INT16S)dwPara[28]/10.0;                                     //40228 ���λ�á�
            else*/
      g_SignalChannelParam[nChannel].dZeroPosition=(INT16S)dwPara[28]/1000.0;                                     //40228 ���λ�á�
      
      g_SignalChannelParam[nChannel].dHysteresis=(INT16S)dwPara[29]/1000.0;                                       //40229 ���͵�ѹ��
      g_SignalChannelParam[nChannel].dTriggerVoltage=(INT16S)dwPara[30]/1000.0;                                   //40230 ��ת��ѹ��
      g_SignalChannelParam[nChannel].dTeethPerCycle=(INT16U)dwPara[31];                                         //40231 ÿ�ܳ�����
      g_SignalChannelParam[nChannel].nThresholdType=dwPara[32];                                         //40232 ������ƽ���Զ����ֶ�����
      g_SignalChannelParam[nChannel].dAngleOfSensorFix=dwPara[33];                                   //40233 �������뱻������ĽǶȡ�
      g_SignalChannelParam[nChannel].nBarrier=dwPara[34];                                               //40234 ��������դ��
      g_SignalChannelParam[nChannel].dLowPassFilter=dwPara[35];                               //40235 �˲����Ͷ˽�ֹƵ�ʡ�
      g_SignalChannelParam[nChannel].dHighPassFilter=dwPara[36];                              //40236 �˲����߶˽�ֹƵ�ʡ�
      
      //LG:2009-09-11 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if((dwPara[35] & 0xFFFF) == 0xFFFF)
      {
        //	g_SignalChannelParam[nChannel].nLowPassFilterEnable = FALSE;                                                                       //40235 �˲����Ͷ˽�ֹƵ�ʡ�
        g_SignalChannelParam[nChannel].dLowPassFilter=0;                               //40235 �˲����Ͷ˽�ֹƵ�ʡ�
      }
      else
      {
        //	g_SignalChannelParam[nChannel].nLowPassFilterEnable = TRUE;                                                                       //40235 �˲����Ͷ˽�ֹƵ�ʡ�
        g_SignalChannelParam[nChannel].dLowPassFilter=(INT16S)dwPara[35];                               //40235 �˲����Ͷ˽�ֹƵ�ʡ�
      }
      
      if((dwPara[36] & 0xFFFF) == 0x0000)//LG:2009-11-16 ��ԭ����0xFFFFFFF��Ϊ���ڵ�0x00   
      {
        //	g_SignalChannelParam[nChannel].nHighPassFilterEnable = FALSE;                                                                       //40235 �˲����Ͷ˽�ֹƵ�ʡ�
        g_SignalChannelParam[nChannel].dHighPassFilter = 0;                              //40236 �˲����߶˽�ֹƵ�ʡ�
      }
      else
      {
        //	g_SignalChannelParam[nChannel].nHighPassFilterEnable = TRUE;                                                                       //40235 �˲����Ͷ˽�ֹƵ�ʡ�
        g_SignalChannelParam[nChannel].dHighPassFilter = (INT16S)dwPara[36];                              //40236 �˲����߶˽�ֹƵ�ʡ�
      }
      
      //�������
      
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
      
      //�������
      //·ռ������ʱ�Ķ�������ΪЭ�鲻һ�¶��յģ������ƻ�����ṹ��2012-3-13
      /*	if(this->m_nModuleID == 21)//·ռ����2012-11-16
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
      //40227 ���õ����ָ��ֵ��
      if(dwPara[3] == 7 || dwPara[3] == 8 )		
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXDangerHigh=(INT16U)dwPara[8]*pow(10,nExponent);                              //40208 Σ�ոߡ�
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXDangerHighHysteresis=(INT16U)dwPara[9]*pow(10,nExponent);                    //40209 Σ�ո߻ز
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXDangerLow=(INT16U)dwPara[10]*pow(10,nExponent);                              //40210 Σ�յ͡�
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXDangerLowHysteresis=(INT16U)dwPara[11]*pow(10,nExponent);                    //40211 Σ�յͻز
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXAlertLow=(INT16U)dwPara[12]*pow(10,nExponent);                               //40212 �����͡�
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXAlertLowHysteresis=(INT16U)dwPara[13]*pow(10,nExponent);                     //40213 �����ͻز
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXAlertHigh=(INT16U)dwPara[14]*pow(10,nExponent);                              //40214 �����ߡ�
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXAlertHighHysteresis=(INT16U)dwPara[15]*pow(10,nExponent);                    //40215 �����߻ز
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXFullScaleHigh=(INT16U)dwPara[19]*pow(10,nExponent);                          //40219 �����̸ߡ�
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXFullScaleLow=(INT16U)dwPara[26]*pow(10,nExponent);                      //40226 �����̵͡�
      }
      else
      {
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXDangerHigh=(INT16S)dwPara[8]*pow(10,nExponent);                              //40208 Σ�ոߡ�
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXDangerHighHysteresis=(INT16S)dwPara[9]*pow(10,nExponent);                    //40209 Σ�ո߻ز
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXDangerLow=(INT16S)dwPara[10]*pow(10,nExponent);                              //40210 Σ�յ͡�
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXDangerLowHysteresis=(INT16S)dwPara[11]*pow(10,nExponent);                    //40211 Σ�յͻز
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXAlertLow=(INT16S)dwPara[12]*pow(10,nExponent);                               //40212 �����͡�
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXAlertLowHysteresis=(INT16S)dwPara[13]*pow(10,nExponent);                     //40213 �����ͻز
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXAlertHigh=(INT16S)dwPara[14]*pow(10,nExponent);                              //40214 �����ߡ�
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXAlertHighHysteresis=(INT16S)dwPara[15]*pow(10,nExponent);                    //40215 �����߻ز
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXFullScaleHigh=(INT16S)dwPara[19]*pow(10,nExponent);                          //40219 �����̸ߡ�
        g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].dXXFullScaleLow=(INT16S)dwPara[26]*pow(10,nExponent);                      //40226 �����̵͡�
      }
      
      g_SignalChannelParam[nChannel].struActualOtherParaTable[nIndex].nXXEnable = dwPara[23];                                         //40223 ͨ���Ƿ�����
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

INT32S  CaltChannelParam(INT32S nKey,FP32 fFrequencyLow,FP32 fFrequencyHigh,INT32S nSampleSynMode,INT32S nSynSampleLth,
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

INT32S	RenewParam(void* p83in,void* p12in)
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
    re=CaltChannelParam(i,p83->Device[i].nFrequencyLow,p83->Device[i].nFrequencyHigh,
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

//����ͨ��ԭʼ���ݵ�ǰƫ��
INT32S	SetRamDiskChDataOffset(INT32S nStartChannel,INT32S nChannelcnt,INT32S pData[])//arm ���ã�����fpga����ͨ�������ڵ����̵�д���ַ
{
  INT32S re=0;
  Mem_Copy(g_nFpgaDynamicChDataOffset+nStartChannel,pData,sizeof(INT32S)*nChannelcnt);
  return re;
}

//���ø���ͨ���ĳ���Ƶ��
INT32S	SetChDataSampleStep(INT32S nStartChannel,INT32S nChannelcnt,INT32S pData[])
{
  INT32S re=0;
  Mem_Copy(g_nFpgaDynamicChDataSampleStep+nStartChannel,pData,sizeof(INT32S)*nChannelcnt);
  return re;
}

//arm���ö�ȡͨ������ͷ
INT32S ReadChValue(Tst_Head_DCM_SigModuSampData_SYS* p,INT32S nStartChannel,INT32S nChannelcnt)
{
  INT32S re=0;
  Mem_Copy(p,g_TempValue+nStartChannel,sizeof(Tst_Head_DCM_SigModuSampData_SYS)*nChannelcnt);
  return re;
}

//arm���ö�ȡͨ��ԭʼ����
INT32S ReadRamDisk(INT32U nOffset,INT32S pData[],INT32S nLength)
{
  INT32S re=0;
  Mem_Copy(pData,g_pChData+nOffset,nLength);
  return re;
}

//simu��main83ģ��ֱ�����������Լ��ĵ������-ͨ����ַ�����飬������������е�ͨ�����
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

//arm���ö�ȡ����ͨ����ǰ�ٶ�
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
//arm���ö�ȡ����ͨ���²����ļ����,Ŀǰ����Ҫ������
INT32S ReadKeyPointCnt(INT32U nChannelcnt,INT32U nChannel[],INT32S pData[])
{
  INT32S re=0;
  
  return re;
}
