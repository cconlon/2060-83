
#include "define2060.h"
#include "interface.h"
#include "transfer.h"
#include "ucos_ii.h"


extern INT32S					g_nSystemNo;//系统编号
extern INT32S					g_nMgrNo;//机组编号
extern INT32S					g_nMchNo[_MAX_SIGNAL_CHANNEL_CNT];//机器编号
extern INT32S					g_nDeviceNo[_MAX_SIGNAL_CHANNEL_CNT] ;//虚拟设备编号
extern INT32S					g_nRTValueRequest[_MAX_SIGNAL_CHANNEL_CNT];//测点（按序号）请求列表
extern INT32S					g_nRTValueMrsPt[_MAX_SIGNAL_CHANNEL_CNT];//各个请求测量值的测点序号
extern INT32S					g_nRTValueMrsPtcnt ;//请求测量值的测点数量
extern INT32S					g_nRTValueMrsPtChcnt ;//所有测量值请求的通道数量
extern INT32S					g_nRTValueMrsPtChSit ;//所有通道的累积测量值指针偏移
extern INT32S					g_nRTValueBlockcnt ;//测量值块数
extern INT32S					g_nRTValueBlockLth[(_MAX_SIGNAL_CHANNEL_CNT-1)/_MAX_VALUE_TRANSFER_CNT+1] ;//各个测量值块长度
extern INT32S					g_nRTValueBlockMrsptcnt[(_MAX_SIGNAL_CHANNEL_CNT-1)/_MAX_VALUE_TRANSFER_CNT+1];//每一块实时测量值中的测点数
extern INT32S					g_RTVBuffer[_MAX_SIGNAL_CHANNEL_CNT*sizeof(struct tagMrsptRTValue)/sizeof(INT32S)];
extern struct tagMrsptRTValue*  g_RTValue;
extern INT32S*					g_pRTValue[_MAX_SIGNAL_CHANNEL_CNT];//各个通道实时测量值指针
extern INT32S					g_nRTDataRequest[_MAX_SIGNAL_CHANNEL_CNT];
extern struct tagMrsptRTData*	g_pRTData[_MAX_SIGNAL_CHANNEL_CNT];//测点实时原始信号
extern INT32S					g_nRTCalibrationChannel;

extern INT32S					g_bRTRequestChanging ;//实时测量值请求正在被修改
extern INT32S					g_bRTValueReady;//实时测量值空闲
extern INT32S					g_bRTDataReady[_MAX_SIGNAL_CHANNEL_CNT];//实时原始信号：0，空闲，1，写入中，2，信号有效
extern INT32S					g_bRTCalibrationReady;//实时校验数据空闲

extern INT32S					g_nStaticManualPick[_MAX_JKEY_CHANNEL_CNT];
extern INT32S					g_nDynamicManualPick[_MAX_JKEY_CHANNEL_CNT];
extern INT32S					g_nTranManualPick[_MAX_JKEY_CHANNEL_CNT];//不用,保留

extern Data_Limit_83			g_83DataLimit;//83模块的一些极限参数
extern INT32S					g_nChannelArrivalSequenceTable[_MAX_SIGNAL_CHANNEL_CNT];//到达通道次12通道序号表
extern INT32S					g_nArrivalDeviceNo[_MAX_SIGNAL_CHANNEL_CNT];//到达通道虚拟设备表
extern INT32S					g_nArrivalChannelNo[_MAX_SIGNAL_CHANNEL_CNT];//通道数据到达在所属设备的通道号
extern Param_Table_12			g_12Param;//12模块参数,从12模块读取,不保存,由12congfig模块配置,360系统不可修改
extern Param_Table_83			g_83param;//83模块参数,存储在flash,可从上位机更新
extern Run_Time_Flag			g_RunTimeFlag;//83运行状态信息
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
        //		OSSchedLock();//禁止任务切换 
        cnt=GetCommand(sBuffer,0);//从命令管线读取数据
        //	OSSchedUnlock();//允许任务切换 
        cnt=cnt;
        if(cnt>lth)
        {
            switch(sBuffer[0]) 
            {
            case 1://上位机命令

                switch(sBuffer[1])
                {
                case 1://设置系统编号
                    if(g_RunTimeFlag.bRunning==0)//停止状态
                    {
                        g_nSystemNo=sBuffer[2];//系统编号
                        re=0;
                    }
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 2://设置机组、机器和虚拟设备编号
                    if(g_RunTimeFlag.bRunning==0)//停止状态
                    {
                        g_nMgrNo=sBuffer[2];//机组编号
                        g_83param.nDeviceNo=sBuffer[4+sBuffer[3]];//83设备编号就是首个虚拟设备编号
                        for(i=0;i<sBuffer[3];i++)
                        {
                            g_nMchNo[i]=sBuffer[4+i];//机器编号
                            g_nDeviceNo[i]=sBuffer[4+sBuffer[3]+i];//虚拟设备编号
                        }
                        re=0;
                    }
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 3://启动/停止83模块
                    if(sBuffer[2])//运行命令
                    {
                        if(g_RunTimeFlag.bRunning==0)//停止状态
                        {
                            Start2060();
                            g_RunTimeFlag.bRunning=1;
                            re=0;
                        }		
                    }else//停止命令
                    {
                        if(g_RunTimeFlag.bRunning==1)//运行状态
                        {
                            Stop2060();
                            g_RunTimeFlag.bRunning=0;
                            re=0;
                        }		
                    }
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 4://申请/停止申请实时测量值
                case 5://申请/停止申请实时测点原始数据
                case 6://申请/停止实时通道原始数据申请
                    {
                        if(g_RunTimeFlag.bRunning==1)//运行状态
                        {
                            for(i=0;i<sBuffer[3];i++)//测点或通道数量
                            {
                                for(j=0;j<g_12Param.nMrsPtcnt;j++)//根据测点编号寻找测点序号
                                {
                                    if(g_12Param.nMrsPtTable[j]==sBuffer[4+i])
                                        break;
                                }
                                switch(sBuffer[1]) 
                                {//请求类型，子协议
                                case 4:
                                    g_nRTValueRequest[j]=sBuffer[2];//测点测量值请求置位
                                    if(sBuffer[2])//请求
                                    {	
                                        l=0;
                                        for(k=0;k<g_nRTValueMrsPtcnt;k++)//察看这个测点是否已经请求
                                        {	
                                            if(g_nRTValueMrsPt[k]==j)
                                            {
                                                l=1;
                                                break;
                                            }
                                        }
                                        if(l==0)
                                        {
                                            g_nRTValueMrsPt[g_nRTValueMrsPtcnt]=sBuffer[4+i];//加入实时测量值测点请求列表
                                            for(k=0;k<g_12Param.nSignChannelNum;k++)//找寻属于该测点的通道
                                            {
                                                if(g_12Param.nMrsPt[k]==sBuffer[4+i])//该通道属于本测点
                                                {//设置本通道实时测量值指针
                                                    g_pRTValue[k]=p8s+g_nRTValueMrsPtChSit;//两个通道的实时测量值指针指向相同
                                                    if(++l==g_12Param.nMrsPtChcnt[j])//所有通道都被找出
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
                                            g_nRTValueMrsPtChcnt+=g_12Param.nMrsPtChcnt[g_nRTValueMrsPt[g_nRTValueMrsPtcnt]];//请求的通道数增家
                                            g_nRTValueMrsPtcnt++;//请求测点数增加
                                            if(g_nRTValueMrsPtChcnt>=(g_nRTValueBlockcnt+1)*_MAX_VALUE_TRANSFER_CNT)
                                                g_nRTValueBlockMrsptcnt[g_nRTValueBlockcnt++]=g_nRTValueMrsPtcnt;
                                        }
                                    }
                                    else//结束请求
                                    {
                                        g_bRTRequestChanging=1;//设置测量值修改标志
                                        nRmvMrsPtcnt=0;
                                        nRmvMrsPtChcnt=0;
                                        for(k=0;k<g_nRTValueMrsPtcnt;k++)//察看这个测点是否已经请求
                                        {
                                            if(g_nRTValueMrsPt[k]==j)
                                            {												
                                                g_nRTValueMrsPt[k]=-1;
                                                for(l=0;l<g_12Param.nSignChannelNum;l++)//找寻属于该测点的通道
                                                {
                                                    if(g_12Param.nMrsPt[l]==j)//该通道属于本测点
                                                    {//复原通道实时测量值指针
                                                        g_pRTValue[l]=0;
                                                        nRmvMrsPtChcnt=g_12Param.nMrsPtChcnt[j];//该序号测点通道数
                                                        break;
                                                    }
                                                }
                                                nRmvMrsPtcnt=1;
                                                if(nRmvMrsPtChcnt==1)//后移偏置指针
                                                    g_nRTValueMrsPtChSit-=sizeof(struct tagMrsptRTValue);
                                                else
                                                    g_nRTValueMrsPtChSit-=sizeof(struct tagMrsptRTValue)+sizeof(struct tagRTChValue);
                                            }else if(nRmvMrsPtcnt&&nRmvMrsPtcnt)
                                            {
                                                m=0;
                                                g_nRTValueMrsPt[k-nRmvMrsPtcnt]=g_nRTValueMrsPt[k];//请求队列中测点移位
                                                for(l=0;l<g_12Param.nSignChannelNum;l++)//找寻属于该测点的通道
                                                {
                                                    if(g_nRTValueMrsPt[k]==g_12Param.nMrsPt[l])//该通道属于本测点
                                                    {	
                                                        //测点中通道的实时测量值指针移位
                                                        if(nRmvMrsPtChcnt==1)
                                                            g_pRTValue[l]-=sizeof(struct tagMrsptRTValue);
                                                        else
                                                            g_pRTValue[l]-=sizeof(struct tagMrsptRTValue)+sizeof(struct tagRTChValue);
                                                        if(++m==nRmvMrsPtChcnt)//移位通道数和测点通道数相同
                                                            break;
                                                    }
                                                }
                                            }
                                        }
                                        if(nRmvMrsPtcnt)
                                        {
                                            g_nRTValueMrsPtChcnt-=nRmvMrsPtChcnt;//请求的通道数减小
                                            g_nRTValueMrsPtcnt-=nRmvMrsPtcnt;//请求测点数减小
                                            if(g_nRTValueMrsPtChcnt<(g_nRTValueBlockcnt+1)*_MAX_VALUE_TRANSFER_CNT)
                                            {
                                                g_nRTValueBlockMrsptcnt[g_nRTValueBlockcnt-2]=\
                                                    (nRmvMrsPtChcnt-g_nRTValueBlockMrsptcnt[g_nRTValueMrsPtcnt-1]);
                                                if(nRmvMrsPtChcnt-g_nRTValueBlockMrsptcnt[g_nRTValueMrsPtcnt-1]==1)
                                                    g_nRTValueBlockLth[g_nRTValueBlockcnt]-=sizeof(struct tagMrsptRTValue);
                                                else
                                                    g_nRTValueBlockLth[g_nRTValueBlockcnt]-=sizeof(struct tagMrsptRTValue)+sizeof(struct tagRTChValue);
                                                g_nRTValueBlockcnt--;
                                            }else//块数没有减小
                                            {
                                                if(nRmvMrsPtChcnt==1)
                                                    g_nRTValueBlockLth[g_nRTValueBlockcnt]-=sizeof(struct tagMrsptRTValue);
                                                else
                                                    g_nRTValueBlockLth[g_nRTValueBlockcnt]-=sizeof(struct tagMrsptRTValue)+sizeof(struct tagRTChValue);
                                                g_nRTValueBlockMrsptcnt[g_nRTValueBlockcnt-1]-=nRmvMrsPtChcnt;
                                            }

                                        }
                                        g_bRTRequestChanging=0;//复原测量值修改标志
                                    }
                                    break;
                                case 5:								
                                    g_nRTDataRequest[j]=sBuffer[2];//测点原始数据请求置位
                                    break;
                                default:
                                    if(sBuffer[2]>=0)//校验
                                        g_nRTCalibrationChannel=sBuffer[4+i];
                                    else//取消校验
                                        g_nRTCalibrationChannel=-1;
                                    break;
                                }
                            }
                            re=0;
                        }	
                    }
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 7://手动静态数据采集
                    if(g_RunTimeFlag.bRunning==1)//运行状态
                    {
                        for(i=0;i<g_83param.nDevicecnt;i++)
                            g_nStaticManualPick[i]=1;
                        re=0;
                    }	
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 8://手动动态数据采集
                    if(g_RunTimeFlag.bRunning==1)//运行状态
                    {
                        for(i=0;i<g_83param.nDevicecnt;i++)
                            g_nDynamicManualPick[i]=1;
                        re=0;
                    }
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 9://12模块参数申请
                    if(g_RunTimeFlag.bRunning==0)//运行状态
                    {
                        re=0;
                        //	SendCommand(&re,sizeof(INT32S));
                        //	OSTimeDly(beat);
                        //	do {

                        SendCommand(&g_12Param,sizeof(g_12Param));
                        //OSTimeDly(beat);
                        OSTimeDly(beat, OS_OPT_TIME_DLY, &err);
                        //		GetCommand(&re,sizeof(INT32S));//对方接收正确会返回0								
                        //	} while(re);						
                    }else
                    {
                        re=-1;
                        SendCommand(&re,sizeof(INT32S));
                    }	
                    break;
                case 10://83模块参数上传申请
                    if(g_RunTimeFlag.bRunning==0)//运行状态
                    {
                        re=0;
                        //	SendCommand(&re,sizeof(INT32S));
                        //	OSTimeDly(beat);
                        //	do {
                        SendCommand(&g_83param,sizeof(g_83param));
                        OSTimeDly(beat, OS_OPT_TIME_DLY, &err);
                        //		GetCommand(&re,sizeof(INT32S));//对方接收正确会返回0							
                        //	} while(re);
                    }else
                    {
                        re=-1;
                        SendCommand(&re,sizeof(INT32S));
                    }	
                    break;
                case 11://83模块参数下载申请
                    if(g_RunTimeFlag.bRunning==0)//运行状态
                    {
                        re=0;
                        SendCommand(&re,sizeof(INT32S));
                        OSTimeDly(beat, OS_OPT_TIME_DLY, &err);
                        do {
                            re=GetCommand(&g_83param,sizeof(g_83param));
                            if(re!=sizeof(g_83param))//参数长度不对错误，发回错误信息，上位机将重新下载
                            {
                                re=-1;
                                SendCommand(&re,sizeof(INT32S));
                            }
                            else//正确，返回0
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
                case 12://数据通道有效
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
            case 5://下载83模块参数
                switch(sBuffer[1]) 
                {
                case 1://通道启用标志
                    if(g_RunTimeFlag.bRunning==0)//非运行状态
                    {
                        for(i=0;i<sBuffer[2];i++)
                            g_83param.Device[sBuffer[3+i*3]].nChannelMask[sBuffer[3+i*3+1]]=sBuffer[3+i*3+2];
                        re=0;
                        RenewParam(&g_83param,&g_12Param);
                    }	
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 2://虚拟设备报警允许
                    if(g_RunTimeFlag.bRunning==0)//非运行状态
                    {
                        for(i=0;i<sBuffer[2];i++)
                            g_83param.Device[sBuffer[3+i*2]].bAlertTrigerStatic=sBuffer[3+i*2+1];
                        re=0;
                        RenewParam(&g_83param,&g_12Param);
                    }	
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 3://测点-通道绑定列表
                    if(g_RunTimeFlag.bRunning==0)//非运行状态
                    {
                        for(i=0;i<sBuffer[2];i++)
                        {
                            /*虚拟通道所属测点号*/
                            g_83param.Device[sBuffer[3+i*4]].nMrsPtNo[sBuffer[3+i*4+1]]=sBuffer[3+i*4+2];
                            /*通道在其所属测点中的序号*/
                            g_83param.Device[sBuffer[3+i*4]].nChIndexMrsPt[sBuffer[3+i*4+1]]=sBuffer[3+i*4+3];
                        }
                        re=0;
                        RenewParam(&g_83param,&g_12Param);
                    }
                    SendCommand(&re,sizeof(INT32S));
                    break;
                case 4://测点静态数据允许
                case 5://测点动态数据允许
                case 6://测点瞬态数据允许
                    if(g_RunTimeFlag.bRunning==0)//非运行状态
                    {
                        for(i=0;i<sBuffer[2];i++)//遍历各个测点
                        {
                            k=0;
                            for(i=0;i<g_83param.nDevicecnt;i++)//遍历各个虚拟设备
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
                case 7://瞬态数据采样参数
                    if(g_RunTimeFlag.bRunning==0)//非运行状态
                    {
                        for(i=0;i<sBuffer[2];i++)//遍历各个虚拟设备
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
                case 8://静态数据采集周期
                case 9://动态数据采集周期
                    if(g_RunTimeFlag.bRunning==0)//非运行状态
                    {
                        for(i=0;i<sBuffer[2];i++)//遍历各个虚拟设备
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
