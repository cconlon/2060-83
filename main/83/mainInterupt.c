#include "includes.h"
#include "define2060.h"
#include "interface.h"
#include <math.h>
extern INT32U				g_nStartTime = 0;
extern INT32U				g_nTickCount = 0;

extern Param_Table_12		g_12Param;//12ģ�����,��12ģ���ȡ,������,��12congfigģ������,360ϵͳ�����޸�
extern Param_Table_83		g_83param;//83ģ�����,�洢��flash,�ɴ���λ������
//Run_Time_Flag				g_RunTimeFlag;//83����״̬��Ϣ

/*�������������󶨵ļ������Ϊ�ڴ滮�ֵĻ��������⵽�ﰴ�ռ����-����ʱ��-����ͨ�������֯*/
/*��ǰ�������̬���ݰ��ռ����-��������-����ͨ����ŵ�ָ���б�,���ڿ��ٴ��źŴ���ģ����ٽ�������*/
/*��̬�Ͷ�̬�����Ա���ʱ����Ϊһ�βɼ�,���������������ͨ���������*/


/*��̬���������ڴ��С�����þ�̬���䣬��̬������Ҫ�Ա�����ʽ�������ض�ʱ��������ݵ���ʱ��һ��ͨ��������
����ͬ�����ͨ��һ����вɼ�,�ɼ���ɺ���������ľ�̬������ż�1*/
extern Tst_Head_DCM_SigModuSampData_SYS		g_StaticOriginData[_MAX_STATIC_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT];//��̬����ֵ����
//����12ģ��ļ���-ͨ����б���,�������ټ�����̬����
extern Tst_Head_DCM_SigModuSampData_SYS*		g_pStaticChannelOrigin[_MAX_JKEY_CHANNEL_CNT][_MAX_STATIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
//extern Channel_Static_Value					g_tmpStaticData[_MAX_SIGNAL_CHANNEL_CNT];
extern Static_Triger_Table					g_StaticTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];


/*��̬���������ڴ�ϴ󣬲��ö�̬���䣬��̬������Ҫ�Ա�����ʽ�������ض�ʱ��������ݵ���ʱ��һ��ͨ��������
����ͬ�����ͨ��һ����вɼ���ÿ��ʱ����²ɼ���̬���ݵ�ͨ�������ǲ�ͬ��,�ɼ���ɺ���������Ķ�̬������ż�1*/
extern Tst_Head_DCM_SigModuSampData_SYS		g_DynamicValue[_MAX_DYNAMIC_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT];//��̬̬����ֵ����
//����12ģ��ļ���-ͨ����б���,�������ټ�����̬����ͷ
extern Tst_Head_DCM_SigModuSampData_SYS*		g_pDynamicChannelValue[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
extern Dynamic_Triger_Table					g_DynamicTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];

//��̬ͨ������ƫ�Ʊ�,��ʼ��ʱ����,ͬ��ָʾ������ͨ��д���ַ,Ҳ������������λ������ʱ���ٶ�λ�����ڵ����̵�λ��
extern INT32U									g_nChannel_Dynamic_Wave_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
extern INT32U									g_nChannel_Dynamic_Key_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH];

/*˲̬���������ڴ�ܴ�,���ö�̬����,���������豸����Ϊ��ͬ�豸��������,˲̬���ݶ��г�����󲻳���_MAX_TRAN_ARRAY_LTH��
����Ϊ1����Ϊ�������豸���������Ϊ���ݻ���Ϊͬ�ȴ�С������,�����ٰ���ͨ�����ϸ��,ͨ���ڲ��Դ��������ٴ�ϸ��
*/
extern Tst_Head_DCM_SigModuSampData_SYS		g_TranValue[_MAX_TRAN_ARRAY_LTH*_MAX_SIGNAL_CHANNEL_CNT*_MAX_TRAN_GROUP];//˲̬����ֵ����
//����12ģ��ļ���-ͨ����б���,�������ټ���˲̬̬����ͷ
extern Tst_Head_DCM_SigModuSampData_SYS*		g_pTranChannelValue[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT][_MAX_TRAN_GROUP];
extern Tran_Triger_Table						g_TranTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];
//˲̬ͨ������ƫ�Ʊ�,��ʼ��ʱ����,ͬ��ָʾ������ͨ��д���ַ,Ҳ������������λ������ʱ���ٶ�λ�����ڵ����̵�λ��
extern INT32U		g_nChannel_Tran_Wave_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT][_MAX_TRAN_GROUP];
extern INT32U		g_nChannel_Tran_Key_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_TRAN_GROUP];


extern INT32S		g_nStaticManualPick[_MAX_JKEY_CHANNEL_CNT];
extern INT32S		g_nDynamicManualPick[_MAX_JKEY_CHANNEL_CNT];
extern INT32S		g_nTranManualPick[_MAX_JKEY_CHANNEL_CNT];//����,����

INT32U				g_nIndexInGroupSequency = 0;
INT32U				g_DynamicSampleStepDuration[_MAX_JKEY_CHANNEL_CNT] = {0};

//Data_Limit_83				g_83DataLimit;//83ģ���һЩ���޲���
extern INT32S				g_nChannelArrivalSequenceTable[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ����12ͨ����ű�
extern INT32S				g_nArrivalDeviceNo[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ�������豸��
extern INT32S				g_nArrivalChannelNo[_MAX_SIGNAL_CHANNEL_CNT];//ͨ�����ݵ����������豸��ͨ����

//INT32U		g_Signal_Channel_Dynamic_Write_Position[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ���Ķ�̬����д��λ��
//INT32U		g_Signal_Channel_Tran_Write_Position[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ����˲̬����д��λ��
//INT32U		g_Key_Channel_Dynamic_Write_Position[_MAX_JKEY_CHANNEL_CNT];//����ͨ���Ķ�̬��������д��λ��
//INT32U		g_Key_Channel_Tran_Write_Position[_MAX_JKEY_CHANNEL_CNT];//����ͨ����˲̬��������д��λ��

INT32U		g_nArmDynamicChDataOffset[_MAX_SIGNAL_CHANNEL_CNT*2+_MAX_JKEY_CHANNEL_CNT*2];
INT32U*		g_nArmDynamicKeyDataOffset = g_nArmDynamicChDataOffset+_MAX_SIGNAL_CHANNEL_CNT;
INT32U*		g_nArmTranChDataOffset = g_nArmDynamicChDataOffset+_MAX_SIGNAL_CHANNEL_CNT+_MAX_JKEY_CHANNEL_CNT;
INT32U*		g_nArmTranKeyDataOffset = g_nArmDynamicChDataOffset+_MAX_SIGNAL_CHANNEL_CNT*2+_MAX_JKEY_CHANNEL_CNT;
INT32S		g_nGroupCnt = _MAX_SIGNAL_CHANNEL_CNT/_FPGA_GROUP_CHANNEL_CNT;

INT32U		g_nArmDynamicChDataSampleStep[_MAX_SIGNAL_CHANNEL_CNT*2];
INT32U*		g_nArmTranChDataSampleStep=g_nArmDynamicChDataSampleStep+_MAX_SIGNAL_CHANNEL_CNT;

INT32U		g_nTickTime = 0;
//INT32S		g_nFPGATickcnt = 0;
/*manual*///�޸ļ������������ݳ�ȡ����ʱ����ȫ�ֱ����ĳ�ʼ����Ҫ�ֹ��޸�
Key_Speed_Calt	g_DynamicSpeedCalt[_MAX_JKEY_CHANNEL_CNT];//���������ٶȶ�̬����ṹ
static Key_Speed_Calt	g_TranSpeedCalt[_MAX_JKEY_CHANNEL_CNT];//���������ٶ�˲̬����ṹ

static const INT32U g_nArmDataOffsetInvalid[_MAX_SIGNAL_CHANNEL_CNT*2+_MAX_JKEY_CHANNEL_CNT*2]={0};//��Ч������ƫ�Ƶ�ַ
static INT32S g_nKeyValidFlag[_MAX_JKEY_CHANNEL_CNT] = {1,1,1,1};//���μ��������Ƿ���Ч
static INT32S g_nKeyStaticTrigerFlag[_MAX_JKEY_CHANNEL_CNT] = {0};//���θ��������Ƿ񴥷���̬�����ɼ�
static INT32S g_nKeyDynamicTrigerFlag[_MAX_JKEY_CHANNEL_CNT] = {0};//���θ��������Ƿ񴥷���̬�����ɼ�
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
  for(i = 0;i<_MAX_SIGNAL_CHANNEL_CNT;i++)//��ʼ���������ź�ͨ������д��ƫ�Ʊ��ͨ����ȡ����
  {
    nOffsetJKey = g_12Param.nChannelNumIn83[i*2];//���������豸��
    nOffsetChannel = g_12Param.nChannelNumIn83[i*2+1];//ͨ����
    g_nArmDynamicChDataOffset[i] = g_nChannel_Dynamic_Wave_Data_Offset[nOffsetJKey][0][nOffsetChannel];//��̬��ʼƫ��
    g_nArmTranChDataOffset[i] = g_nChannel_Tran_Wave_Data_Offset[nOffsetJKey][0][nOffsetChannel][0];//˲̬��ʼƫ��
    g_nArmDynamicChDataSampleStep[i] = g_83param.Device[nOffsetJKey].nDynamicChannelDataSampleStep/*[nOffsetChannel]*/;//��̬��ȡ����
    g_nArmTranChDataSampleStep[i] = g_83param.Device[nOffsetJKey].nTranChannelDataSampleStep/*[nOffsetChannel]*/;//˲̬��ȡ����
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

  //����FPGA��ͨ��������ʼƫ��
  re = SetRamDiskChDataOffset(0,(_MAX_SIGNAL_CHANNEL_CNT+_MAX_JKEY_CHANNEL_CNT)*2,g_nArmDynamicChDataOffset);
  //����FPGA��ͨ�����ݳ�ȡ����
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
  INT32S nStaticTrigerWriteSit[] = //��ǰ�����豸������ͷд��λ��
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
  /*��ȡ˲̬�������е�ǰ��*/
  INT32S nTranCureWriteGroup[] = {
    g_TranTrigerEventArray[0].ulnCurWriteGroup,g_TranTrigerEventArray[1].ulnCurWriteGroup,
    g_TranTrigerEventArray[2].ulnCurWriteGroup,g_TranTrigerEventArray[3].ulnCurWriteGroup,
  };
  if(g_nIndexInGroupSequency == 0)//�������ٶȣ��ж�˲̬�Ƿ񴥷�,Ҫ��g_nGroupCnt����Ϊ2��ָ����/**/
  {
    ReadKeySpeed(_MAX_JKEY_CHANNEL_CNT,nCurSpeed,g_nCurSpeed);//��ȡ���м���ͨ���ٶ�
    for(nDevice = 0;nDevice<_MAX_JKEY_CHANNEL_CNT;nDevice++)//ѭ���豸
    {
      g_nKeyValidFlag[nDevice]=1;//��ʼ��ͨ����־�͸������౨����־
      //			g_uKeyAlertFlag[nDevice] = 0;//���θ�������ϳɱ���״̬
      tmp = g_TranTrigerEventArray[nDevice].nPreTranTickCnt;
      if(g_TranTrigerEventArray[nDevice].ulnTrigerFlag==0)//�жϴ˼����Ƿ񴥷�˲̬�ɼ�
      {		
        tmp1 = g_DynamicSpeedCalt[nDevice].nPreAcumulateSpeed;
        tmp2 = g_DynamicSpeedCalt[nDevice].nTrigerLevel;
        tmp3 = g_nCurSpeed[nDevice]*_MAX_JKEY_SPEED_ARRAY_LTH;
        //�ٶȶ������ݳ��㲢������ת�ٴ�������			
        if((_MAX_JKEY_SPEED_ARRAY_LTH<=g_nTickCount)&&(tmp3-tmp1 >= tmp2 || tmp1-tmp3 >= tmp2)&&
          g_nCurSpeed[nDevice]>=g_83param.Device[nDevice].KeyChannel->fRPMLow&&
          g_nCurSpeed[nDevice]<=g_83param.Device[nDevice].KeyChannel->fRPMHigh)
        {
          g_TranTrigerEventArray[nDevice].ulnTrigerFlag = 1;//���ô�����־
          //����˲̬������ʼƫ��
          if(tmp+1>g_83param.Device[nDevice].nMinPreTranTickcnt)//����ǰ�ۻ���������ǰ��������
          {	//����
            g_TranTrigerEventArray[nDevice].lnGroupHeadSit[nTranTrigerWriteSit[nDevice]] = \
              g_TranTrigerEventArray[nDevice].ulnCurWriteGroup-g_83param.Device[nDevice].nTranPreTrigerGroupcnt+1;
            if(g_TranTrigerEventArray[nDevice].lnGroupHeadSit[nTranTrigerWriteSit[nDevice]]<0)//С��0����ħ����
              g_TranTrigerEventArray[nDevice].lnGroupHeadSit[nTranTrigerWriteSit[nDevice]] += g_83param.Device[nDevice].nMaxTranSampleCnt;
            g_TranTrigerEventArray[nDevice].nPreTranTickCnt = g_83param.Device[nDevice].nTranPreTrigerGroupcnt;//��Ϊǰ��������
          }
          else//����ǰ�ۻ�����δ�ﵽǰ����������ȫ������
          {
            g_TranTrigerEventArray[nDevice].lnGroupHeadSit[nTranTrigerWriteSit[nDevice]] = 0;//ǰ�������ݲ���ʱ����ͷ����ʼ
            g_TranTrigerEventArray[nDevice].nPreTranTickCnt = g_TranTrigerEventArray[nDevice].ulnCurWriteGroup+1;
          }

          g_TranTrigerEventArray[nDevice].ulnTrigerTime[nTranTrigerWriteSit[nDevice]] = g_nTickTime;//��¼����ʱ���/**/
          g_TranTrigerEventArray[nDevice].ulnAccumulateCnt = 0;//ʱ���ȡ���ۻ�������
          g_TranTrigerEventArray[nDevice].ulnPreRPM = 0;//ǰһ�����ݵ�ת��
          g_TranTrigerEventArray[nDevice].ulnTrigerTimeSpan=0;//��մ�������ʱ��
        }else //δ����
        {	
          g_nKeyStaticTrigerFlag[nDevice] = 0;//���θ��������Ƿ񴥷���̬�����ɼ�
          g_nKeyDynamicTrigerFlag[nDevice]=0;
          if(g_83param.Device[nDevice].nDynamicSampleMode)//��̬ͬ���ɼ�ʱִ��
          {
            if(g_83param.Device[nDevice].nChDynamicDataBlockCnt!=g_DynamicSampleStepDuration[nDevice])//2s���ж�һ��
              g_DynamicSampleStepDuration[nDevice]++;
            else//��Ƶ����
            {
              for(tmp3=0;tmp<_MAX_RPM_LEVEL;tmp3++)
              {
                if(g_nCurSpeed[nDevice]<=g_DynamicSpeedCalt[nDevice].nRPMLevel[tmp3])
                {
                  if(g_DynamicSpeedCalt[nDevice].nCurLevel==tmp3)//�ض�ִ��
                    break;
                  g_DynamicSpeedCalt[nDevice].nCurLevel=tmp3;//����Ƶ��
                  nIndex = g_DynamicTrigerEventArray[nDevice].ulnTrigerWrite;//ȡ������Ķ�̬��ǰд������
                  pDCT = g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable+nIndex;
                  pDCT->ulnBlockCnt=0;
                  pDCT->lnCurBlock=0;
                  pDCT->lnHeadBlock=0;
                  p32S=g_83param.Device[nDevice].nChannelNumIn12;
                  tmp1=g_DynamicSpeedCalt[nDevice].nSampleStep[tmp3];
                  for(nChannel = g_83param.Device[nDevice].nSignChannelNum-1;nChannel>=0;nChannel--)//���������豸����ͨ��
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
      tmp++;//��������1
      if(g_nTickCount<_MAX_JKEY_SPEED_ARRAY_LTH)//�ٶȶ������ݲ���
      {
        g_DynamicSpeedCalt[nDevice].nPreAcumulateSpeed += g_nCurSpeed[nDevice];//�ۻ����ٶ�	
      }else
      {
        g_DynamicSpeedCalt[nDevice].nPreAcumulateSpeed += g_nCurSpeed[nDevice]
        -g_DynamicSpeedCalt[nDevice].nSpeedArray[g_nSpeedArrayIndex[nDevice]];//�ۻ��ٶȳ��ɸ���
      }
      if(g_DynamicSpeedCalt[nDevice].nPreAcumulateSpeed>=120000)
        tmp=tmp;
      //�ٶȶ��е�ǰд��λ��
      g_DynamicSpeedCalt[nDevice].nSpeedArray[g_nSpeedArrayIndex[nDevice]] = g_nCurSpeed[nDevice];//�����ٶ�
      g_nSpeedArrayIndex[nDevice]++;
      if(_MAX_JKEY_SPEED_ARRAY_LTH==g_nSpeedArrayIndex[nDevice])
        g_nSpeedArrayIndex[nDevice]=0;
      if(g_TranTrigerEventArray[nDevice].ulnTrigerFlag==0)
        g_TranTrigerEventArray[nDevice].nPreTranTickCnt = tmp;//���¼�����
    }
  }
  tmp1= g_nIndexInGroupSequency*_FPGA_GROUP_CHANNEL_CNT;
  for(nIndex = tmp1;nIndex<tmp1+_FPGA_GROUP_CHANNEL_CNT;nIndex++)//����������˲̬����ͷ����,���жϱ���
  {
    nDevice = g_nArrivalDeviceNo[nIndex];//�����豸��
    if(g_nKeyValidFlag[nDevice] == 0)//ͨ����Чʱ������
      continue;
    nChannel = g_nArrivalChannelNo[nIndex];
    pHT = g_pTranChannelValue[nDevice][nTranTrigerWriteSit[nDevice]][nChannel][nTranCureWriteGroup[nDevice]];//��ȡд���ַ
    tmp = ReadChValue(pHT,g_83param.Device[nDevice].nChannelNumIn12[nChannel],1);//��ȡ����ͷ
    pHT->ulnRPM=g_nCurSpeed[nDevice];//ʹ�ñ�����
    g_nKeyValidFlag[nDevice] &= pHT->tunStaticFlag1.tstVal.unFlag_RUN;//����ͨ�������Ƿ���Ч�������
    if(g_TranTrigerEventArray[nDevice].ulnTrigerFlag == 0&&g_nKeyValidFlag[nDevice])//˲̬δ����ʱ�����Ƿ��б�������
    {
      tmp = pHT->tunStaticFlag1.tstVal.unFlag_Danger+pHT->tunStaticFlag1.tstVal.unFlag_Alert;//�����ϳ�
      g_nKeyStaticTrigerFlag[nDevice] |= (tmp>g_TranTrigerEventArray[nDevice].ulnPreAlertFlag);//�ж��Ƿ񴥷������ɼ�
      g_TranTrigerEventArray[nDevice].ulnPreAlertFlag = tmp;//�´��ж�ʱ����
    }
    *g_pStaticChannelOrigin[nDevice][nStaticTrigerWriteSit[nDevice]][nChannel]=*pHT;
    *g_pDynamicChannelValue[nDevice][nDynamicTrigerWriteSit[nDevice]][nChannel]=*pHT;
  }
  if(g_nIndexInGroupSequency != g_nGroupCnt-1)//ͨ��û��ȫ�����룬����Ĳ�����
  {
    g_nIndexInGroupSequency++;
    return; 
  }
  ReadKeyPointCnt(_MAX_JKEY_CHANNEL_CNT,nCurKeyPointCnt,nCurKeyPointCnt);	
  for(nDevice = 0;nDevice<_MAX_JKEY_CHANNEL_CNT;nDevice++)//ѭ�������豸
  {
    if(g_nKeyValidFlag[nDevice])//����˲̬�������л�ȡǰ�θ�������ı���״̬��������Чʱ��ִ��
    {
      //���˲̬�������㳤��
      g_TranTrigerEventArray[nDevice].ChannelSignalDataTable[nTranTrigerWriteSit[nDevice]][nTranCureWriteGroup[nDevice]].KeyDataBlockLth[0]=
        nCurKeyPointCnt[nDevice];

      if(g_TranTrigerEventArray[nDevice].ulnTrigerFlag == 0)//δ����˲̬
      {
        //��䶯̬�������㳤��
        tmp=g_DynamicTrigerEventArray[nDevice].ulnTrigerWrite;
        tmp2=g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[tmp].lnCurBlock;
        g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[tmp].KeyDataBlockLth[tmp2]=nCurKeyPointCnt[nDevice];

        //˲̬ǰ�����ۻ�g_83param.Device[nDevice].nRealPickTimeIntervalPre������1��
        if(g_83param.Device[nDevice].nRealPickTimeIntervalPre-1 == g_TranTrigerEventArray[nDevice].ulnAccumulateCnt)
        {
          //����˲̬��ַ
          tmp2 = g_TranTrigerEventArray[nDevice].ulnCurWriteGroup+1;//������һ��λ��
          if(tmp2 == g_83param.Device[nDevice].nMaxTranSampleCnt)
            tmp2 = 0;
          tmp1=g_83param.Device[nDevice].nMaxTranSampleCnt;
          p32S=g_83param.Device[nDevice].nChannelNumIn12;
          nChannel = g_83param.Device[nDevice].nSignChannelNum-1;
          p32S1=g_nChannel_Tran_Wave_Data_Offset[nDevice][nTranTrigerWriteSit[nDevice]][nChannel]+tmp2;
          for(;nChannel>=0;nChannel--)//����������˲̬����ͷ����,���жϱ���
          {
            //ˢ��˲̬�ź�ͨ������д���ַ
            g_nArmTranChDataOffset[p32S[nChannel]]=*p32S1;
            p32S1-=tmp1;
          }
          //ˢ��˲̬����ͨ������д���ַ
          g_nArmTranKeyDataOffset[nDevice]=\
            g_nChannel_Tran_Key_Data_Offset[nDevice][nTranTrigerWriteSit[nDevice]][tmp2];

          g_TranTrigerEventArray[nDevice].ulnCurWriteGroup=tmp2;//�л�����һ��
          g_TranTrigerEventArray[nDevice].ulnAccumulateCnt=0;
        }else
          g_TranTrigerEventArray[nDevice].ulnAccumulateCnt++;
      }
      //˲̬�Ѿ�����
      else
      {			
        tmp1 = nTranTrigerWriteSit[nDevice];//����˲̬���ݵĵ�ǰд��λ��
        nIndex = g_TranTrigerEventArray[nDevice].ulnTrigerWrite;//ȡ�������˲̬��ǰд������
        pTCT = g_TranTrigerEventArray[nDevice].ChannelSignalDataTable[nIndex]+g_TranTrigerEventArray[nDevice].ulnCurWriteGroup;
        pTCT->ulnBlockCnt = 0;//��ԭ�������´ν���ʱ��ֵ�Ա���
        tmp = g_TranTrigerEventArray[nDevice].ulnPreRPM;
        tmp1 = g_83param.Device[nDevice].nSampleRPMInterval;
        tmp2 = g_TranTrigerEventArray[nDevice].ulnTrigerTime[nTranTrigerWriteSit[nDevice]];
        tmp3 = g_nCurSpeed[nDevice];
        g_TranTrigerEventArray[nDevice].ulnPick=0;//Ĭ��ʱ��δ��
        if(g_83param.Device[nDevice].nTranSampleMode)//ת�ٳ�ȡ
        {
          if(tmp-tmp3 >= tmp1 || tmp3-tmp >= tmp1)//�������
          {
            g_TranTrigerEventArray[nDevice].ulnPick = 1;
            g_TranTrigerEventArray[nDevice].ulnPreRPM = tmp3;
          }
        }else//ʱ���ȡ
        {
          if(g_TranTrigerEventArray[nDevice].ulnAccumulateCnt == g_83param.Device[nDevice].nRealSampleTimeInterval-1)
          {
            g_TranTrigerEventArray[nDevice].ulnPick = 1;
            g_TranTrigerEventArray[nDevice].ulnAccumulateCnt = 0;
          }
          else
            g_TranTrigerEventArray[nDevice].ulnAccumulateCnt++;
        }

        if(g_TranTrigerEventArray[nDevice].ulnPick)//��ȡ��������
        {
          //����˲̬��ַ
          tmp1=g_83param.Device[nDevice].nMaxTranSampleCnt;
          p32S=g_83param.Device[nDevice].nChannelNumIn12;
          nChannel = g_83param.Device[nDevice].nSignChannelNum-1;

          if(_MAX_TRAN_GROUP > g_TranTrigerEventArray[nDevice].nPreTranTickCnt&&//�������˻�ʱ�䵽��
            g_TranTrigerEventArray[nDevice].ulnTrigerTimeSpan < g_83param.Device[nDevice].nMaxTranSampleTime)//�ü���˲̬�ɼ�����
          {
            tmp2 = g_TranTrigerEventArray[nDevice].ulnCurWriteGroup+1;//������һ��λ��
            if(tmp2 == g_83param.Device[nDevice].nMaxTranSampleCnt)
              tmp2 = 0;

            p32U1=g_nChannel_Tran_Wave_Data_Offset[nDevice][nTranTrigerWriteSit[nDevice]][nChannel]+tmp2;
            for(;nChannel>=0;nChannel--)//����������˲̬����ͷ����,���жϱ���
            {
              //ˢ��˲̬�ź�ͨ������д���ַ
              g_nArmTranChDataOffset[p32S[nChannel]]=*p32U1;
              p32U1-=tmp1;
            }
            //ˢ��˲̬����ͨ������д���ַ
            g_nArmTranKeyDataOffset[nDevice]=\
              g_nChannel_Tran_Key_Data_Offset[nDevice][nTranTrigerWriteSit[nDevice]][tmp2];

            g_TranTrigerEventArray[nDevice].nPreTranTickCnt++;//��Ч������1	
            g_TranTrigerEventArray[nDevice].ulnCurWriteGroup=tmp2;//�л�����һ��
          }
          else{	//
            g_TranTrigerEventArray[nDevice].ulnGroupCnt[nTranTrigerWriteSit[nDevice]]=g_TranTrigerEventArray[nDevice].nPreTranTickCnt;
            g_TranTrigerEventArray[nDevice].nPreTranTickCnt = 0;//��ԭǰ������
            g_TranTrigerEventArray[nDevice].ulnTrigerFlag = 0;//��ԭ˲̬������־
            g_TranTrigerEventArray[nDevice].ulnCurWriteGroup=0;
            tmp3=nTranTrigerWriteSit[nDevice]+1;
            if(_MAX_TRAN_ARRAY_LTH == tmp3)
              tmp3 = 0;//��ͷ
            g_TranTrigerEventArray[nDevice].ulnTrigerWrite=tmp3;

            p32U1=g_nChannel_Tran_Wave_Data_Offset[nDevice][tmp3][nChannel]+0;
            for(;nChannel>=0;nChannel--)//����������˲̬����ͷ����,���жϱ���
            {
              //ˢ��˲̬�ź�ͨ������д���ַ
              g_nArmTranChDataOffset[p32S[nChannel]]=*p32U1;
              p32U1-=tmp1;
            }
            //ˢ��˲̬����ͨ������д���ַ
            g_nArmTranKeyDataOffset[nDevice]=\
              g_nChannel_Tran_Key_Data_Offset[nDevice][tmp3][0];
          }

        }					

        nIndex = nDynamicTrigerWriteSit[nDevice];//һ����������ջ����еĶ�̬����
        g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[nIndex].ulnBlockCnt = 0;//��Ч������ԭ
        g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[nIndex].lnCurBlock = 0;//��ǰ�鸴ԭ
        g_TranTrigerEventArray[nDevice].ulnTrigerTimeSpan+=_12_DATA_UPLOAD_TIME_GAP;
        continue;//˲̬һ���������򲻽��б����������жϺʹ���
      }	
    }
    else//�ü���������Чʱ���������ݶ�������̬���ݽ����»��壬�Ѿ�������˲̬���ݽ����������ε�
    {	nIndex = nDynamicTrigerWriteSit[nDevice];//һ����������ջ����еĶ�̬����
    g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[nIndex].ulnBlockCnt = 0;//��Ч������ԭ
    g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[nIndex].lnCurBlock = 0;//��ǰ�鸴ԭ
    continue;
    }

    if(g_83param.Device[nDevice].bAlertTrigerStatic)//��̬����������Ч
    {
      if(g_83param.Device[nDevice].bAlertTrigerDynamic)//��̬����������Ч
      {
        g_nKeyDynamicTrigerFlag[nDevice] = g_nKeyStaticTrigerFlag[nDevice];//�����򴥷�	
      }else
        g_nKeyDynamicTrigerFlag[nDevice] = 0;//��������̬
    }else
      g_nKeyStaticTrigerFlag[nDevice] = 0;//����ִ��

    //δ����ʱ�ж϶�ʱ�ɼ��Ƿ����
    if(g_83param.Device[nDevice].nDynamicAutoPickTime && g_nKeyDynamicTrigerFlag[nDevice] == 0)//��̬
    {
      if(g_nDynamicTickCount[nDevice]==g_83param.Device[nDevice].nDynamicAutoPickTime)
      {
        g_nDynamicTickCount[nDevice]=0;
        g_nKeyDynamicTrigerFlag[nDevice] = 1;
        g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[nDynamicTrigerWriteSit[nDevice]].ulnTrigerReason = 1;
      }else
        g_nDynamicTickCount[nDevice]++;
    }
    if(g_83param.Device[nDevice].nStaticAutoPickTime && g_nKeyStaticTrigerFlag[nDevice] == 0)//��̬
    {
      if(g_nStaticTickCount[nDevice]==g_83param.Device[nDevice].nStaticAutoPickTime)
      {
        g_nStaticTickCount[nDevice]=0;
        g_nKeyStaticTrigerFlag[nDevice] = 1;
        g_StaticTrigerEventArray[nDevice].ulnTrigerReason[nStaticTrigerWriteSit[nDevice]] = 1;	
      }else
        g_nStaticTickCount[nDevice]++;
    }
    //δ����ʱ�ж��ֶ��ɼ��Ƿ����
    if(g_nKeyDynamicTrigerFlag[nDevice] == 0)//��̬
    {
      g_nKeyDynamicTrigerFlag[nDevice] = g_nDynamicManualPick[nDevice];
      g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[nDynamicTrigerWriteSit[nDevice]].ulnTrigerReason = 2;
    }
    if(g_nKeyStaticTrigerFlag[nDevice] == 0)//��̬
    {
      g_nKeyStaticTrigerFlag[nDevice] = g_nStaticManualPick[nDevice];
      g_StaticTrigerEventArray[nDevice].ulnTrigerReason[nStaticTrigerWriteSit[nDevice]] = 2;
    }
    if(g_nKeyStaticTrigerFlag[nDevice])//��̬�ɼ�����
    {
      g_StaticTrigerEventArray[nDevice].ulnTrigerTime[nStaticTrigerWriteSit[nDevice]] = g_nTickTime;
      g_StaticTrigerEventArray[nDevice].ulnTrigerWrite++;//�л�����һ��
      if(_MAX_STATIC_ARRAY_LTH == g_StaticTrigerEventArray[nDevice].ulnTrigerWrite)//�Ƿ񵽴�β��
        g_StaticTrigerEventArray[nDevice].ulnTrigerWrite = 0;//��ͷ
    }
    if(g_nKeyDynamicTrigerFlag[nDevice])//��̬�ɼ�����
    {
      nIndex = g_DynamicTrigerEventArray[nDevice].ulnTrigerWrite;//ȡ������Ķ�̬��ǰд������
      pDCT = g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable+nIndex;
      pDCT->ulnTrigerTime = g_nTickTime;
      //���ƶ�̬������ʼ�����
      if(g_83param.Device[nDevice].nDynamicSampleMode==0)//�첽ʱ���������Ϊ1
      {
        pDCT->lnHeadBlock=pDCT->lnCurBlock+1;//�ܿ�������
      }
      else//ͬ��ʱ��ת���й�
      {
        tmp=g_DynamicSpeedCalt[nDevice].nCurLevel;//��ȡת�ټ���
        pDCT->lnHeadBlock=pDCT->lnCurBlock+g_DynamicSpeedCalt[nDevice].nSynBlockIncreaseStep[tmp];
        pDCT->ulnBlockCnt=g_DynamicSpeedCalt[nDevice].nSynBlockCnt[tmp];//���ݳ�����ת�ٽ����й�
      }
      if(pDCT->lnHeadBlock>=g_83param.Device[nDevice].nChDynamicDataBlockCnt)//����Խ�磬��ͷ��ʼ
        pDCT->lnHeadBlock=0;
      g_DynamicSampleStepDuration[nDevice]=g_83param.Device[nDevice].nChDynamicDataBlockCnt;//ǿ���´ο�ʼ��Ƶ�ж�
      g_DynamicTrigerEventArray[nDevice].ulnTrigerWrite++;//ָ����һ����̬������
      //���¶�̬��ַ
      tmp1 = g_DynamicTrigerEventArray[nDevice].ulnTrigerWrite;//��̬���ݶ���ƫ��
      p32S=g_nChannel_Dynamic_Wave_Data_Offset[nDevice][tmp1];
      for(nChannel = 0;nChannel<g_83param.Device[nDevice].nSignChannelNum;nChannel++)//����������˲̬����ͷ����,���жϱ���
      {
        //ȡ�ø�ͨ���ڣ����е����,ˢ�¶�̬�ź�ͨ������д���ַ
        g_nArmDynamicChDataOffset[g_83param.Device[nDevice].nChannelNumIn12[nChannel]]=p32S[nChannel];
      }
      //ˢ�¶�̬����ͨ������д���ַ
      g_nArmDynamicKeyDataOffset[nDevice]=g_nChannel_Dynamic_Key_Data_Offset[nDevice][tmp1];
      if(_MAX_DYNAMIC_ARRAY_LTH == g_DynamicTrigerEventArray[nDevice].ulnTrigerWrite)
        g_DynamicTrigerEventArray[nDevice].ulnTrigerWrite = 0;//��ͷ
    }
    else
    {
      tmp2 = g_DynamicSpeedCalt[nDevice].nCurLevel;
      tmp1 = g_DynamicTrigerEventArray[nDevice].ulnTrigerWrite;//��̬���ݶ���ƫ��
      tmp3 = g_83param.Device[nDevice].nChDynamicBlockSize;
      if(g_83param.Device[nDevice].nDynamicSampleMode)
        if(g_DynamicSpeedCalt[nDevice].nSynBlockIncreaseStep[tmp2]>1)
          tmp3*=g_DynamicSpeedCalt[nDevice].nSynBlockIncreaseStep[tmp2];
      //ȡ������Ķ�̬��ǰд������
      pDCT = g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable+nDynamicTrigerWriteSit[nDevice];
      tmp = 1;//�첽�ɼ�ʱ����1
      if(g_83param.Device[nDevice].nDynamicSampleMode)//ͬ���ɼ���Ƶʱ�����Ӵ�
        tmp = g_DynamicSpeedCalt[nDevice].nSynBlockIncreaseStep[tmp2];
      pDCT->lnCurBlock+=tmp;//�л�����һ�����ݿ�
      if(pDCT->lnCurBlock+tmp>g_83param.Device[nDevice].nChDynamicDataBlockCnt)//����������ݿ��Ƿ����㲽��Ҫ��
        pDCT->lnCurBlock=0;//��������ݿ鲻�㣬��ͷ��ʼ���
      if(g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[tmp1].lnCurBlock==0)
      {
        p32S=g_nChannel_Dynamic_Wave_Data_Offset[nDevice][tmp1];
        for(nChannel = 0;nChannel<g_83param.Device[nDevice].nSignChannelNum;nChannel++)//����������˲̬����ͷ����,���жϱ���
        {
          //ȡ�ø�ͨ���ڣ����е����,ˢ�¶�̬�ź�ͨ������д���ַ			
          g_nArmDynamicChDataOffset[g_83param.Device[nDevice].nChannelNumIn12[nChannel]]=p32S[nChannel];
        }
      }else
      {
        for(nChannel = 0;nChannel<g_83param.Device[nDevice].nSignChannelNum;nChannel++)//����������˲̬����ͷ����,���жϱ���
        {
          g_nArmDynamicChDataOffset[g_83param.Device[nDevice].nChannelNumIn12[nChannel]]+=tmp3;
        }
      }
      //ˢ�¶�̬����ͨ������д���ַ
      if(g_DynamicTrigerEventArray[nDevice].ChannelSignalDataTable[tmp1].lnCurBlock==0)
        g_nArmDynamicKeyDataOffset[nDevice]=g_nChannel_Dynamic_Key_Data_Offset[nDevice][tmp1];
      else
        g_nArmDynamicKeyDataOffset[nDevice]+=g_83param.Device[nDevice].nChKeyBlockSize;

    }		
  }
  //֪ͨFPGA�޸ĸ���ͨ���Ķ�̬��˲̬����RAMDISKд���ַ
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
