
#include "define2060.h"
#include "interface.h"
#include "transfer.h"
#include "ucos_ii.h"


extern INT32S					g_nSystemNo;//ϵͳ���
extern INT32S					g_nMgrNo;//������
extern INT32S					g_nMchNo[_MAX_SIGNAL_CHANNEL_CNT];//�������
extern INT32S					g_nDeviceNo[_MAX_SIGNAL_CHANNEL_CNT] ;//�����豸���
extern INT32S					g_nRTValueRequest[_MAX_SIGNAL_CHANNEL_CNT];//��㣨����ţ������б�
extern INT32S					g_nRTValueMrsPt[_MAX_SIGNAL_CHANNEL_CNT];//�����������ֵ�Ĳ�����
extern INT32S					g_nRTValueMrsPtcnt ;//�������ֵ�Ĳ������
extern INT32S					g_nRTValueMrsPtChcnt ;//���в���ֵ�����ͨ������
extern INT32S					g_nRTValueMrsPtChSit ;//����ͨ�����ۻ�����ֵָ��ƫ��
extern INT32S					g_nRTValueBlockcnt ;//����ֵ����
extern INT32S					g_nRTValueBlockLth[(_MAX_SIGNAL_CHANNEL_CNT-1)/_MAX_VALUE_TRANSFER_CNT+1] ;//��������ֵ�鳤��
extern INT32S					g_nRTValueBlockMrsptcnt[(_MAX_SIGNAL_CHANNEL_CNT-1)/_MAX_VALUE_TRANSFER_CNT+1];//ÿһ��ʵʱ����ֵ�еĲ����
extern INT32S					g_RTVBuffer[_MAX_SIGNAL_CHANNEL_CNT*sizeof(struct tagMrsptRTValue)/sizeof(INT32S)];
extern struct tagMrsptRTValue*  g_RTValue;
extern INT32S*					g_pRTValue[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ��ʵʱ����ֵָ��
extern INT32S					g_nRTDataRequest[_MAX_SIGNAL_CHANNEL_CNT];
extern struct tagMrsptRTData*	g_pRTData[_MAX_SIGNAL_CHANNEL_CNT];//���ʵʱԭʼ�ź�
extern INT32S					g_nRTCalibrationChannel;

extern INT32S					g_bRTRequestChanging ;//ʵʱ����ֵ�������ڱ��޸�
extern INT32S					g_bRTValueReady;//ʵʱ����ֵ����
extern INT32S					g_bRTDataReady[_MAX_SIGNAL_CHANNEL_CNT];//ʵʱԭʼ�źţ�0�����У�1��д���У�2���ź���Ч
extern INT32S					g_bRTCalibrationReady;//ʵʱУ�����ݿ���

extern INT32S					g_nStaticManualPick[_MAX_JKEY_CHANNEL_CNT];
extern INT32S					g_nDynamicManualPick[_MAX_JKEY_CHANNEL_CNT];
extern INT32S					g_nTranManualPick[_MAX_JKEY_CHANNEL_CNT];//����,����

extern Data_Limit_83			g_83DataLimit;//83ģ���һЩ���޲���
extern INT32S					g_nChannelArrivalSequenceTable[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ����12ͨ����ű�
extern INT32S					g_nArrivalDeviceNo[_MAX_SIGNAL_CHANNEL_CNT];//����ͨ�������豸��
extern INT32S					g_nArrivalChannelNo[_MAX_SIGNAL_CHANNEL_CNT];//ͨ�����ݵ����������豸��ͨ����
extern Param_Table_12			g_12Param;//12ģ�����,��12ģ���ȡ,������,��12congfigģ������,360ϵͳ�����޸�
extern Param_Table_83			g_83param;//83ģ�����,�洢��flash,�ɴ���λ������
extern Run_Time_Flag			g_RunTimeFlag;//83����״̬��Ϣ
extern INT32S					g_bStaticPipeValid;
extern INT32S					g_bDynamicPipeValid;
extern INT32S					g_bTranPipeValid;
extern INT32S					g_bDataPipeValid;
extern INT32S					g_bSignalPipeValid;

void command_analys(void* p)
{	
#if 0
    OS_ERR err;
    INT8S*  p8s = &(g_RTValue->nChValue);
    INT32S  sBuffer[1024];
    INT32S re=-1,i,j,k,l,m,cnt=0,nRmvMrsPtcnt=0,nRmvMrsPtChcnt=0,delta;
    INT32S beat=(1000/g_n12UpFrequency)/(1000/OS_CFG_TICK_RATE_HZ)*2;
    INT32S lth=(sizeof(INT32S));
    do {
        //		OSSchedLock();//��ֹ�����л� 
        cnt=GetCommand(sBuffer,0);//��������߶�ȡ����
        //	OSSchedUnlock();//���������л� 
        cnt=cnt;
        if(cnt>lth)
        {
            switch(sBuffer[0]) 
            {
            case 1://��λ������

                switch(sBuffer[1])
                {
                case 1://����ϵͳ���
                    if(g_RunTimeFlag.bRunning==0)//ֹͣ״̬
                    {
                        g_nSystemNo=sBuffer[2];//ϵͳ���
                        re=0;
                    }
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 2://���û��顢�����������豸���
                    if(g_RunTimeFlag.bRunning==0)//ֹͣ״̬
                    {
                        g_nMgrNo=sBuffer[2];//������
                        g_83param.nDeviceNo=sBuffer[4+sBuffer[3]];//83�豸��ž����׸������豸���
                        for(i=0;i<sBuffer[3];i++)
                        {
                            g_nMchNo[i]=sBuffer[4+i];//�������
                            g_nDeviceNo[i]=sBuffer[4+sBuffer[3]+i];//�����豸���
                        }
                        re=0;
                    }
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 3://����/ֹͣ83ģ��
                    if(sBuffer[2])//��������
                    {
                        if(g_RunTimeFlag.bRunning==0)//ֹͣ״̬
                        {
                            Start2060();
                            g_RunTimeFlag.bRunning=1;
                            re=0;
                        }		
                    }else//ֹͣ����
                    {
                        if(g_RunTimeFlag.bRunning==1)//����״̬
                        {
                            Stop2060();
                            g_RunTimeFlag.bRunning=0;
                            re=0;
                        }		
                    }
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 4://����/ֹͣ����ʵʱ����ֵ
                case 5://����/ֹͣ����ʵʱ���ԭʼ����
                case 6://����/ֹͣʵʱͨ��ԭʼ��������
                    {
                        if(g_RunTimeFlag.bRunning==1)//����״̬
                        {
                            for(i=0;i<sBuffer[3];i++)//����ͨ������
                            {
                                for(j=0;j<g_12Param.nMrsPtcnt;j++)//���ݲ����Ѱ�Ҳ�����
                                {
                                    if(g_12Param.nMrsPtTable[j]==sBuffer[4+i])
                                        break;
                                }
                                switch(sBuffer[1]) 
                                {//�������ͣ���Э��
                                case 4:
                                    g_nRTValueRequest[j]=sBuffer[2];//������ֵ������λ
                                    if(sBuffer[2])//����
                                    {	
                                        l=0;
                                        for(k=0;k<g_nRTValueMrsPtcnt;k++)//�쿴�������Ƿ��Ѿ�����
                                        {	
                                            if(g_nRTValueMrsPt[k]==j)
                                            {
                                                l=1;
                                                break;
                                            }
                                        }
                                        if(l==0)
                                        {
                                            g_nRTValueMrsPt[g_nRTValueMrsPtcnt]=sBuffer[4+i];//����ʵʱ����ֵ��������б�
                                            for(k=0;k<g_12Param.nSignChannelNum;k++)//��Ѱ���ڸò���ͨ��
                                            {
                                                if(g_12Param.nMrsPt[k]==sBuffer[4+i])//��ͨ�����ڱ����
                                                {//���ñ�ͨ��ʵʱ����ֵָ��
                                                    g_pRTValue[k]=p8s+g_nRTValueMrsPtChSit;//����ͨ����ʵʱ����ֵָ��ָ����ͬ
                                                    if(++l==g_12Param.nMrsPtChcnt[j])//����ͨ�������ҳ�
                                                        break;
                                                }
                                            }
                                            if(l==1)
                                            {
                                                g_nRTValueBlockLth[g_nRTValueBlockcnt]+=sizeof(struct tagRTChValue);
                                                g_nRTValueMrsPtChSit+=sizeof(struct tagRTChValue);
                                            }
                                            else
                                            {
                                                g_nRTValueMrsPtChSit+=2*sizeof(struct tagRTChValue)-sizeof(INT32S);
                                                g_nRTValueBlockLth[g_nRTValueBlockcnt]+=2*sizeof(struct tagRTChValue)-sizeof(INT32S);
                                            }
                                            g_nRTValueMrsPtChcnt+=g_12Param.nMrsPtChcnt[g_nRTValueMrsPt[g_nRTValueMrsPtcnt]];//�����ͨ��������
                                            g_nRTValueMrsPtcnt++;//������������
                                            if(g_nRTValueMrsPtChcnt>=(g_nRTValueBlockcnt+1)*_MAX_VALUE_TRANSFER_CNT)
                                                g_nRTValueBlockMrsptcnt[g_nRTValueBlockcnt++]=g_nRTValueMrsPtcnt;
                                        }
                                    }
                                    else//��������
                                    {
                                        g_bRTRequestChanging=1;//���ò���ֵ�޸ı�־
                                        nRmvMrsPtcnt=0;
                                        nRmvMrsPtChcnt=0;
                                        for(k=0;k<g_nRTValueMrsPtcnt;k++)//�쿴�������Ƿ��Ѿ�����
                                        {
                                            if(g_nRTValueMrsPt[k]==j)
                                            {												
                                                g_nRTValueMrsPt[k]=-1;
                                                for(l=0;l<g_12Param.nSignChannelNum;l++)//��Ѱ���ڸò���ͨ��
                                                {
                                                    if(g_12Param.nMrsPt[l]==j)//��ͨ�����ڱ����
                                                    {//��ԭͨ��ʵʱ����ֵָ��
                                                        g_pRTValue[l]=0;
                                                        nRmvMrsPtChcnt=g_12Param.nMrsPtChcnt[j];//����Ų��ͨ����
                                                        break;
                                                    }
                                                }
                                                nRmvMrsPtcnt=1;
                                                if(nRmvMrsPtChcnt==1)//����ƫ��ָ��
                                                    g_nRTValueMrsPtChSit-=sizeof(struct tagMrsptRTValue);
                                                else
                                                    g_nRTValueMrsPtChSit-=sizeof(struct tagMrsptRTValue)+sizeof(struct tagRTChValue);
                                            }else if(nRmvMrsPtcnt&&nRmvMrsPtcnt)
                                            {
                                                m=0;
                                                g_nRTValueMrsPt[k-nRmvMrsPtcnt]=g_nRTValueMrsPt[k];//��������в����λ
                                                for(l=0;l<g_12Param.nSignChannelNum;l++)//��Ѱ���ڸò���ͨ��
                                                {
                                                    if(g_nRTValueMrsPt[k]==g_12Param.nMrsPt[l])//��ͨ�����ڱ����
                                                    {	
                                                        //�����ͨ����ʵʱ����ֵָ����λ
                                                        if(nRmvMrsPtChcnt==1)
                                                            g_pRTValue[l]-=sizeof(struct tagMrsptRTValue);
                                                        else
                                                            g_pRTValue[l]-=sizeof(struct tagMrsptRTValue)+sizeof(struct tagRTChValue);
                                                        if(++m==nRmvMrsPtChcnt)//��λͨ�����Ͳ��ͨ������ͬ
                                                            break;
                                                    }
                                                }
                                            }
                                        }
                                        if(nRmvMrsPtcnt)
                                        {
                                            g_nRTValueMrsPtChcnt-=nRmvMrsPtChcnt;//�����ͨ������С
                                            g_nRTValueMrsPtcnt-=nRmvMrsPtcnt;//����������С
                                            if(g_nRTValueMrsPtChcnt<(g_nRTValueBlockcnt+1)*_MAX_VALUE_TRANSFER_CNT)
                                            {
                                                g_nRTValueBlockMrsptcnt[g_nRTValueBlockcnt-2]=\
                                                    (nRmvMrsPtChcnt-g_nRTValueBlockMrsptcnt[g_nRTValueMrsPtcnt-1]);
                                                if(nRmvMrsPtChcnt-g_nRTValueBlockMrsptcnt[g_nRTValueMrsPtcnt-1]==1)
                                                    g_nRTValueBlockLth[g_nRTValueBlockcnt]-=sizeof(struct tagMrsptRTValue);
                                                else
                                                    g_nRTValueBlockLth[g_nRTValueBlockcnt]-=sizeof(struct tagMrsptRTValue)+sizeof(struct tagRTChValue);
                                                g_nRTValueBlockcnt--;
                                            }else//����û�м�С
                                            {
                                                if(nRmvMrsPtChcnt==1)
                                                    g_nRTValueBlockLth[g_nRTValueBlockcnt]-=sizeof(struct tagMrsptRTValue);
                                                else
                                                    g_nRTValueBlockLth[g_nRTValueBlockcnt]-=sizeof(struct tagMrsptRTValue)+sizeof(struct tagRTChValue);
                                                g_nRTValueBlockMrsptcnt[g_nRTValueBlockcnt-1]-=nRmvMrsPtChcnt;
                                            }

                                        }
                                        g_bRTRequestChanging=0;//��ԭ����ֵ�޸ı�־
                                    }
                                    break;
                                case 5:								
                                    g_nRTDataRequest[j]=sBuffer[2];//���ԭʼ����������λ
                                    break;
                                default:
                                    if(sBuffer[2]>=0)//У��
                                        g_nRTCalibrationChannel=sBuffer[4+i];
                                    else//ȡ��У��
                                        g_nRTCalibrationChannel=-1;
                                    break;
                                }
                            }
                            re=0;
                        }	
                    }
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 7://�ֶ���̬���ݲɼ�
                    if(g_RunTimeFlag.bRunning==1)//����״̬
                    {
                        for(i=0;i<g_83param.nDevicecnt;i++)
                            g_nStaticManualPick[i]=1;
                        re=0;
                    }	
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 8://�ֶ���̬���ݲɼ�
                    if(g_RunTimeFlag.bRunning==1)//����״̬
                    {
                        for(i=0;i<g_83param.nDevicecnt;i++)
                            g_nDynamicManualPick[i]=1;
                        re=0;
                    }
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 9://12ģ���������
                    if(g_RunTimeFlag.bRunning==0)//����״̬
                    {
                        re=0;
                        //	SendCommand(&re,sizeof(INT32S));
                        //	OSTimeDly(beat);
                        //	do {

                        SendCommand(&g_12Param,sizeof(g_12Param));
                        //OSTimeDly(beat);
                        OSTimeDly(beat, OS_OPT_TIME_DLY, &err);
                        //		GetCommand(&re,sizeof(INT32S));//�Է�������ȷ�᷵��0								
                        //	} while(re);						
                    }else
                    {
                        re=-1;
                        SendCommand(&re,sizeof(INT32S));
                    }	
                    break;
                case 10://83ģ������ϴ�����
                    if(g_RunTimeFlag.bRunning==0)//����״̬
                    {
                        re=0;
                        //	SendCommand(&re,sizeof(INT32S));
                        //	OSTimeDly(beat);
                        //	do {
                        SendCommand(&g_83param,sizeof(g_83param));
                        OSTimeDly(beat, OS_OPT_TIME_DLY, &err);
                        //		GetCommand(&re,sizeof(INT32S));//�Է�������ȷ�᷵��0							
                        //	} while(re);
                    }else
                    {
                        re=-1;
                        SendCommand(&re,sizeof(INT32S));
                    }	
                    break;
                case 11://83ģ�������������
                    if(g_RunTimeFlag.bRunning==0)//����״̬
                    {
                        re=0;
                        SendCommand(&re,sizeof(INT32S));
                        OSTimeDly(beat, OS_OPT_TIME_DLY, &err);
                        do {
                            re=GetCommand(&g_83param,sizeof(g_83param));
                            if(re!=sizeof(g_83param))//�������Ȳ��Դ��󣬷��ش�����Ϣ����λ������������
                            {
                                re=-1;
                                SendCommand(&re,sizeof(INT32S));
                            }
                            else//��ȷ������0
                            {
                                RenewParam(&g_83param,&g_12Param);
                                re=0;
                                SendCommand(&re,sizeof(INT32S));
                                break;
                            }
                            OSTimeDly(beat, OS_OPT_TIME_DLY, &err);							
                        } while(1);					
                    }else
                    {
                        re=-1;
                        SendCommand(&re,sizeof(INT32S));
                    }	
                    break;
                case 12://����ͨ����Ч
                    {
                        switch(sBuffer[2]) 
                        {
                        case 0:
                            g_bStaticPipeValid=1;
                            g_bDataPipeValid=1;
                            re=0;
                            break;
                        case 1:
                            g_bDynamicPipeValid=1;
                            g_bDataPipeValid=1;
                            re=0;
                            break;
                        case 2:
                            g_bTranPipeValid=1;
                            g_bDataPipeValid=1;
                            re=0;
                            break;
                        case 3:					
                            re=0;
                            g_bSignalPipeValid=1;
                            break;
                        case 4:
                            re=0;
                            g_bSignalPipeValid=1;
                            break;
                        default:
                            re=sBuffer[2];
                            break;
                        }

                        SendCommand(&re,sizeof(INT32S));

                    }
                    re=re;
                    break;
                default:
                    break;
                }
                break;	
            case 5://����83ģ�����
                switch(sBuffer[1]) 
                {
                case 1://ͨ�����ñ�־
                    if(g_RunTimeFlag.bRunning==0)//������״̬
                    {
                        for(i=0;i<sBuffer[2];i++)
                            g_83param.Device[sBuffer[3+i*3]].nChannelMask[sBuffer[3+i*3+1]]=sBuffer[3+i*3+2];
                        re=0;
                        RenewParam(&g_83param,&g_12Param);
                    }	
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 2://�����豸��������
                    if(g_RunTimeFlag.bRunning==0)//������״̬
                    {
                        for(i=0;i<sBuffer[2];i++)
                            g_83param.Device[sBuffer[3+i*2]].bAlertTrigerStatic=sBuffer[3+i*2+1];
                        re=0;
                        RenewParam(&g_83param,&g_12Param);
                    }	
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 3://���-ͨ�����б�
                    if(g_RunTimeFlag.bRunning==0)//������״̬
                    {
                        for(i=0;i<sBuffer[2];i++)
                        {
                            /*����ͨ����������*/
                            g_83param.Device[sBuffer[3+i*4]].nMrsPtNo[sBuffer[3+i*4+1]]=sBuffer[3+i*4+2];
                            /*ͨ��������������е����*/
                            g_83param.Device[sBuffer[3+i*4]].nChIndexMrsPt[sBuffer[3+i*4+1]]=sBuffer[3+i*4+3];
                        }
                        re=0;
                        RenewParam(&g_83param,&g_12Param);
                    }
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 4://��㾲̬��������
                case 5://��㶯̬��������
                case 6://���˲̬��������
                    if(g_RunTimeFlag.bRunning==0)//������״̬
                    {
                        for(i=0;i<sBuffer[2];i++)//�����������
                        {
                            k=0;
                            for(i=0;i<g_83param.nDevicecnt;i++)//�������������豸
                            {
                                for(j=0;j<g_83param.Device[i].nSignChannelNum;j++)
                                {
                                    if(g_83param.Device[i].nMrsPtNo[j]==sBuffer[3+i*2])
                                    {
                                        switch(sBuffer[1]) {
                                        case 4:
                                            g_83param.Device[i].nStaticMask[j]=sBuffer[3+i*2+1];
                                            break;
                                        case 5:
                                            g_83param.Device[i].nDynamicMask[j]=sBuffer[3+i*2+1];
                                            break;
                                        default:
                                            g_83param.Device[i].nTranMask[j]=sBuffer[3+i*2+1];
                                            break;
                                        }
                                        k++;
                                        if(k==2)
                                            goto Find;
                                    }
                                }
                            }
Find:	
                            ;
                        }
                        re=0;
                    }	
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 7://˲̬���ݲ�������
                    if(g_RunTimeFlag.bRunning==0)//������״̬
                    {
                        for(i=0;i<sBuffer[2];i++)//�������������豸
                        {
                            g_83param.Device[sBuffer[3+i*6]].KeyChannel[0].nTrigerDRPM=sBuffer[3+i*6+1];
                            g_83param.Device[sBuffer[3+i*6]].KeyChannel[0].fRPMLow=sBuffer[3+i*6+2];
                            g_83param.Device[sBuffer[3+i*6]].KeyChannel[0].fRPMHigh=sBuffer[3+i*6+3];
                            g_83param.Device[sBuffer[3+i*6]].nPickTimeIntervalPre=sBuffer[3+i*6+4];
                            g_83param.Device[sBuffer[3+i*6]].nSampleRPMInterval=sBuffer[3+i*6+5];	
                            re=0;
                        }
                    }
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 8://��̬���ݲɼ�����
                case 9://��̬���ݲɼ�����
                    if(g_RunTimeFlag.bRunning==0)//������״̬
                    {
                        for(i=0;i<sBuffer[2];i++)//�������������豸
                        {
                            if(sBuffer[1]==8)
                                g_83param.Device[sBuffer[3+i*2]].nStaticAutoPickTime=sBuffer[3+i*2+1];
                            else
                                g_83param.Device[sBuffer[3+i*2]].nDynamicAutoPickTime=sBuffer[3+i*2+1];
                            re=0;
                        }
                    }
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 10://
                    break;	
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
        OSTimeDly(beat, OS_OPT_TIME_DLY, &err);	
    } while(1);	
#endif
}
