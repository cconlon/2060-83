#include"netio.h"
#include"define2060.h"
#include "ucos_ii.h"
//#pragma comment(lib,"CIPDevice.lib")

typedef void ( *SOCKETAPICALLBACKFUNC)(const void* strRecvIp,INT32S *pnPort,void* pData,INT32S *pDataLen);

extern INT32S  OpenDevice();//初始化通讯模块
extern INT32S  CloseDevice();
extern INT32S  CreateDispatcher(INT8S strIP[],INT32U nPort,SOCKETAPICALLBACKFUNC func);/*创建调度器*/
/*等待一个管道连接请求连接进入,返回分配的管道编号,同时获得udp数据包的邮寄地址*/
extern INT32S  ConnectTcpPipe(int nPipe,int nTimeOut,int nSendBufferSize);
extern INT32S  DisconnectTcpPipe(int nPipe);
/*创建邮局*/
extern INT32S  CreateUdpPipe(INT8S strIP[],INT32U nPort,SOCKETAPICALLBACKFUNC func);
extern INT32S  CloseDispatcher();//关闭调度器
extern INT32S  CloseTcpPipe(INT32U nPipeID);//关闭一条管道
extern INT32S  CloseUdpPipe();//关闭邮局
extern INT32S  SendTcpCommand(INT8S* p,int cnt);
extern INT32S  GetTcpResponse(INT8S* p,int nLth);//从特定管道读取数
extern INT32S  SendTcpPipe(INT32U nPipe,INT8S* p,INT32S cnt);//向特定管道写入数据
extern INT32S  ReadTcpPipe(INT32U nPipe,INT8S* p,INT32S nLth);//从特定管道读取数据
extern INT32S  SendUdpPipe(INT32U nPipe,void* p,INT32S cnt);//邮寄,忽略管线号码，通讯机只支持一个套结字

static INT32S  g_nPipeFlag[16]={-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16};//各种数据的管道编号
extern INT32S g_bDynamicTransfering;
extern INT32S g_bTranTransfering;
static int g_nTimeOut[]={50,5000,5000,5000,5000,10000,50};
static int g_nSendBufferSize[]={1024,1024*1000,4*1024*1024,4*1024*1024,1024*1000,1024*1000,1024*1000};
INT32S	UnInitNet(void);
INT32S	OpenStaticDataPipe(void);
INT32S	CloseStaticDataPipe(void);
INT32S	OpenDynamicDataPipe(void);
INT32S	CloseDynamicDataPipe(void);
INT32S	OpenTranDataPipe(void);
INT32S	CloseTranDataPipe(void);
INT32S	OpenRealTimeValuePipe(void);

INT32S	CloseRealTimeValuePipe(void);
INT32S	OpenRealTimeWavePipe(void);
INT32S	ReadStaticResponse(void* p);
INT32S	CloseRealTimeWavePipe(void);
INT32S	ReadDynamicResponse(void* p);
INT32S	ReadTranResponse(void* p);

INT32S InitNet(INT8S cIP[],INT32U nTcpPort,INT32S nUdpPort)
{
  int re=0;
  //change by czx
  //OpenDevice();                    //打开通讯设备
  //CreateDispatcher(cIP,nTcpPort,0);//创建调度器
  //CreateUdpPipe(cIP,nUdpPort,0);   //创建UDP传输线
  return re;
}

INT32S	UnInitNet()
{
  INT32S re=0;
  //change by czx
//  CloseUdpPipe();
//  CloseDispatcher();
//  CloseDevice();
  return re;
}

int32_t	open_DataPipe(int32_t channel)//打开静态数据传输管道
{
  int32_t re=-1;
  re=ConnectTcpPipe(channel,g_nTimeOut[channel],g_nSendBufferSize[channel]);
  if(re==0)
  {
    g_nPipeFlag[channel]=channel;
  }
  return re;
}

int32_t	close_DataPipe(int32_t channel)
{
  INT32S re=0;
  if(g_nPipeFlag[channel]==channel)
  {
    re=DisconnectTcpPipe(g_nPipeFlag[channel]);
    if(re==0)
    {
      g_nPipeFlag[channel]=-channel;
    }
  }
  return re;
}

int32_t	write_DataPipe(int32_t channel, int8_t* p,int32_t ncnt)
{
  return SendTcpPipe(channel, p, ncnt);
}

INT32S	OpenStaticDataPipe()//打开静态数据传输管道
{
  INT32S re=-1;
  if(g_nPipeFlag[_STATIC_CHNO]<0&&g_nPipeFlag[_DYNAMIC_CHNO]<0&&g_nPipeFlag[_TRAN_CHNO]<0)//级别低于瞬态和动态
  {
    re=ConnectTcpPipe(_STATIC_CHNO,g_nTimeOut[_STATIC_CHNO],g_nSendBufferSize[_STATIC_CHNO]);
    if(re==0)
    {
      g_nPipeFlag[_STATIC_CHNO]=_STATIC_CHNO;
    }
  }
  return re;
}

INT32S	CloseStaticDataPipe()//关闭静态传输管道
{
  INT32S re=0;
  if(g_nPipeFlag[_STATIC_CHNO]==_STATIC_CHNO)
  {
    re=DisconnectTcpPipe(g_nPipeFlag[_STATIC_CHNO]);
    if(re==0)
    {
      g_nPipeFlag[_STATIC_CHNO]=-_STATIC_CHNO;
    }
  }
  return re;
}

INT32S	OpenDynamicDataPipe()//动态
{
  INT32S re=-1;
#if 0
  OS_ERR err;

  if(g_nPipeFlag[_DYNAMIC_CHNO]<0 && g_nPipeFlag[_TRAN_CHNO]<0)//级别低于瞬态
  {
    OSSchedLock(&err);
    re=ConnectTcpPipe(_DYNAMIC_CHNO,g_nTimeOut[_DYNAMIC_CHNO],g_nSendBufferSize[_DYNAMIC_CHNO]);
    OSSchedUnlock(&err);
    if(re==0)
    {
      g_nPipeFlag[_DYNAMIC_CHNO]=_DYNAMIC_CHNO;
    }
  }
#endif
  return re;
}

INT32S	CloseDynamicDataPipe()
{
  INT32S re=0;
#if 0
  OS_ERR err;
  if(g_nPipeFlag[_DYNAMIC_CHNO]==_DYNAMIC_CHNO)
  {
    OSSchedLock(&err);
    re=DisconnectTcpPipe(g_nPipeFlag[_DYNAMIC_CHNO]);
    OSSchedUnlock(&err);
    if(re==0)
    {
      g_nPipeFlag[_DYNAMIC_CHNO]=-_DYNAMIC_CHNO;
    }
  }
#endif
  return re;
}

INT32S	OpenTranDataPipe()//瞬态
{
  INT32S re=-1;
#if 0
  OS_ERR err;
  if(g_nPipeFlag[_TRAN_CHNO]<0)
  {
    OSSchedLock(&err);
    re=ConnectTcpPipe(_TRAN_CHNO,g_nTimeOut[_TRAN_CHNO],g_nSendBufferSize[_TRAN_CHNO]);
    OSSchedUnlock(&err);
    if(re==0)
    {
      g_nPipeFlag[_TRAN_CHNO]=_TRAN_CHNO;
    }
  }
#endif
  return re;
}
INT32S	CloseTranDataPipe()
{
  INT32S re=0;
#if 0
  OS_ERR err;
  if(g_nPipeFlag[_TRAN_CHNO]==_TRAN_CHNO)
  {
    OSSchedLock(&err);
    re=DisconnectTcpPipe(g_nPipeFlag[_TRAN_CHNO]);
    OSSchedUnlock(&err);
    if(re==0)
    {
      g_nPipeFlag[_TRAN_CHNO]=-_TRAN_CHNO;
    }
  }
#endif
  return re;
}

INT32S	OpenRealTimeValuePipe()//实时信号
{
  INT32S re=0;
  re=ConnectTcpPipe(_REALTIME_VALUE_CHNO,g_nTimeOut[_REALTIME_VALUE_CHNO],g_nSendBufferSize[_REALTIME_VALUE_CHNO]);
  if(re==0)
  {
    g_nPipeFlag[_REALTIME_VALUE_CHNO]=_REALTIME_VALUE_CHNO;
  }
  return re;
}

INT32S	ReadRealTimeValuePipe(INT8S pBuffer[])//实时信号
{
  INT32S re=0;
#if 0
  OS_ERR err;
  OSSchedLock(&err);//禁止任务切换 
  re=ReadTcpPipe(_REALTIME_VALUE_CHNO,pBuffer,1);
  OSSchedUnlock(&err);//允许任务切换 
#endif
  return re;
}

INT32S	CloseRealTimeValuePipe()
{
  INT32S re=0;
  if(g_nPipeFlag[_REALTIME_VALUE_CHNO]==_REALTIME_VALUE_CHNO)
  {
    re=DisconnectTcpPipe(g_nPipeFlag[_REALTIME_VALUE_CHNO]);
    if(re==0)
    {
      g_nPipeFlag[_REALTIME_VALUE_CHNO]=-_REALTIME_VALUE_CHNO;
    }
  }
  return re;
}

INT32S	OpenRealTimeWavePipe()//实时信号
{
  INT32S re=0;
  re=ConnectTcpPipe(_REALTIME_WAVE_CHNO,g_nTimeOut[_REALTIME_WAVE_CHNO],g_nSendBufferSize[_REALTIME_WAVE_CHNO]);
  if(re==0)
  {
    g_nPipeFlag[_REALTIME_WAVE_CHNO]=_REALTIME_WAVE_CHNO;
  }
  return re;
}

INT32S	CloseRealTimeWavePipe()
{
  INT32S re=0;
  if(g_nPipeFlag[_REALTIME_WAVE_CHNO]==_REALTIME_WAVE_CHNO)
  {
    re=DisconnectTcpPipe(g_nPipeFlag[_REALTIME_WAVE_CHNO]);
    if(re==0)
    {
      g_nPipeFlag[_REALTIME_WAVE_CHNO]=-_REALTIME_WAVE_CHNO;
    }
  }
  return re;
}

INT32S	SendCommand(void* p,INT32S cnt)
{
  INT32S re=0;
  //re=SendTcpCommand(0,p,cnt);
  return re;
}

INT32S	GetCommand(void* p,INT32S cnt)
{
  INT32S re=0;
  //re=GetTcpResponse(0,p,cnt);
  return re;
}

INT32S	SendStaticData(void* p,INT32S ncnt)
{
  INT32S re=0;
#if 0
  OS_ERR err;
  //	 while(g_bDynamicTransfering||g_bTranTransfering)//动态或瞬态数据正在传输
  //		 OSTimeDly(1);//休眠
  OSSchedLock(&err);//禁止任务切换 
  re=SendTcpPipe(_STATIC_CHNO,p,ncnt);
  OSSchedUnlock(&err);//允许任务切换 
#endif
  return re;
}

INT32S	ReadStaticResponse(void* p)
{
  INT32S re=0;
#if 0
  OS_ERR err;
  //	 while(g_bDynamicTransfering||g_bTranTransfering)//动态或瞬态数据正在传输
  //		 OSTimeDly(1);//休眠
  OSSchedLock(&err);//禁止任务切换 
  re=ReadTcpPipe(_STATIC_CHNO,p,1);
  OSSchedUnlock(&err);//允许任务切换 
#endif
  return re;
};
INT32S	SendDynamicData(void* p,INT32S ncnt)
{	
  INT32S re=0;
#if 0
  OS_ERR err;
  //	 while(g_bTranTransfering)//动态或瞬态数据正在传输
  //		 OSTimeDly(1);//休眠
  OSSchedLock(&err);//禁止任务切换 
  re=SendTcpPipe(_DYNAMIC_CHNO,p,ncnt);
  OSSchedUnlock(&err);//允许任务切换 
#endif
  return re;
}

INT32S	ReadDynamicResponse(void* p)
{
  INT32S re=0;
#if 0
  OS_ERR err;
  //	 while(g_bDynamicTransfering||g_bTranTransfering)//动态或瞬态数据正在传输
  //		 OSTimeDly(1);//休眠
  OSSchedLock(&err);//禁止任务切换 
  re=ReadTcpPipe(_DYNAMIC_CHNO,p,1);
  OSSchedUnlock(&err);//允许任务切换 
#endif
  return re;
}

INT32S	SendTranData(void* p,INT32S ncnt)
{
  INT32S re=0;
#if 0
  OS_ERR err;
  OSSchedLock(&err);//禁止任务切换 
  re=SendTcpPipe(_TRAN_CHNO,p,ncnt);
  OSSchedUnlock(&err);//允许任务切换 
#endif
  return re;
}

INT32S	ReadTranResponse(void* p)
{
  INT32S re=0;
#if 0
  OS_ERR err;
  //	 while(g_bDynamicTransfering||g_bTranTransfering)//动态或瞬态数据正在传输
  //		 OSTimeDly(1);//休眠
  OSSchedLock(&err);//禁止任务切换 
  re=ReadTcpPipe(_TRAN_CHNO,p,1);
  OSSchedUnlock(&err);//允许任务切换 
#endif
  return re;
};
INT32S	SendRealtimeValueData(void* p,int nCnt)
{
  INT32S re=0;
  #if 0
  OS_ERR err;
  //re=SendUdpPipe(-1,p,nCnt);
  OSSchedLock(&err);//禁止任务切换 
  re=SendTcpPipe(_REALTIME_VALUE_CHNO,p,nCnt);
  OSSchedUnlock(&err);//允许任务切换 
  #endif
  return re;
};
INT32S	SendRealtimeWaveData(void* p,int nCnt)
{
  INT32S re=0;
  #if 0
  OS_ERR err;
  //re=SendUdpPipe(-1,p,nCnt);
  OSSchedLock(&err);//禁止任务切换 
  re=SendTcpPipe(_REALTIME_WAVE_CHNO,p,nCnt);
  OSSchedUnlock(&err);//允许任务切换 
  #endif
  return re;
};
INT32S	BroadcastState(void* p,INT32S nCnt)//状态广播
{
  INT32S re=0;
  //re=SendUdpPipe(-1,p,nCnt);
  re=SendUdpPipe(-1,p,nCnt);
  return re;
}
