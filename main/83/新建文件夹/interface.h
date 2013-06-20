
/*
   *********************************************************************************************************
   *                                          interface for PCM2060/83
   *
   *                          (c) Copyright 2011-, Provibtech
   *                                           All Rights Reserved
   *
   * File : define.h
   * By   : Luzhankui
   *********************************************************************************************************
 */
#ifndef INTERFACE_H
#define INTERFACE_H

#include "define2060.h"

#define BASIC_IP_ADRESS					206
extern INT32S	Read12Param(void* p12);
extern INT32S	Init83Param(void* p83,void* p12);
extern INT32S	ReadChValue(Tst_Head_DCM_SigModuSampData_SYS* p,INT32S nStartChannel,INT32S nChannelcnt);
extern INT32S	ReadChData(void* p[],INT32S ncnt[]);//读取原始数据，返回所读取到的组数
extern INT32S	Renew83(void* p83,void* p12);
extern INT32S	SetRamDiskChDataOffset(INT32S nStartChannel,INT32S nChannelcnt,INT32S pData[]);
extern INT32S	SetChDataSampleStep(INT32S nStartChannel,INT32S nChannelcnt,INT32S pData[]);
extern INT32S	BuildSignalGroupTable(INT32S nChannelSit[]);
extern INT32S	ReadKeySpeed(INT32U nChannelcnt,INT32U nChannel[],INT32S pData[]);
extern INT32S	ReadKeyPointCnt(INT32U nChannelcnt,INT32U nChannel[],INT32S pData[]);
extern void		InitInterfaceAddress();
extern INT32S	ReadRamDisk(INT32U nOffset,INT32S pData[],INT32S nLength);
extern INT32S	RenewParam(void* p83in,void* p12in);

#endif
