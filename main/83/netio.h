#ifndef NETIO_H
#define NETIO_H
#include <os_cpu.h>
#include <stdint.h>

/*
   *********************************************************************************************************
   *                                          transfer for PCM2060/83
   *
   *                          (c) Copyright 2011-, Provibtech
   *                                           All Rights Reserved
   *
   * File : transfer.h
   * By   : Luzhankui
   *********************************************************************************************************
 */
#define _COMMAND_CHNO			0
#define _REALTIME_VALUE_CHNO	5
#define _REALTIME_WAVE_CHNO		4
#define _STATIC_CHNO			3
#define _DYNAMIC_CHNO			2
#define _TRAN_CHNO				1
#define _MAX_TCP_BLOCK_SIZE		512
#define _MAX_TRANSFER_SIZE_PER_BEAT	819200*20
extern INT32S	InitNet(INT8S cIP[],INT32U nTcpPort,INT32S nUdpPort);
extern INT32S	UnInitNet();

extern INT32S	OpenStaticDataPipe();//�򿪾�̬���ݴ���ܵ�
extern INT32S	CloseStaticDataPipe();//�رվ�̬����ܵ�
extern INT32S	OpenDynamicDataPipe();//��̬
extern INT32S	CloseDynamicDataPipe();
extern INT32S	OpenTranDataPipe();//˲̬
extern INT32S	CloseTranDataPipe();
extern INT32S	OpenRealTimeValuePipe();//ʵʱ����ֵ
extern INT32S	CloseRealTimeValuePipe();
extern INT32S	OpenRealTimeWavePipe();//ʵʱ����ֵ
extern INT32S	CloseRealTimeWavePipe();


extern INT32S	SendCommand(void* p,INT32S cnt);//��������
extern INT32S	GetCommand(void* p,INT32S cnt);//��ȡ����
extern INT32S	BroadcastState(void* p,INT32S cnt);//��ȡ����

extern INT32S	SendStaticData(void* p,INT32S ncnt);//���;�̬����
extern INT32S	SendDynamicData(void* p,INT32S ncnt);//���Ͷ�̬����
extern INT32S	SendTranData(void* p,INT32S ncnt);//����˲̬����
extern INT32S	SendRealtimeValueData(void* p,int nCnt);//����ʵʱ����
extern INT32S	SendRealtimeWaveData(void* p,int nCnt);//����ʵʱ����

extern INT32S	ReadRealTimeValuePipe(INT8S pBuffer[]);//ʵʱ�ź�

extern int32_t	open_DataPipe(int32_t channel);
extern int32_t	close_DataPipe(int32_t channel);
extern int32_t	write_DataPipe(int32_t channel, int8_t* p,int32_t ncnt);

#endif
