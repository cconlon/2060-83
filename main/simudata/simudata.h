#ifndef SIMUDATA_H
#define SIMUDATA_H

#include <os_cpu.h>
#define _MAX_CHCNT 64
#define _MAX_KEYCNT 4
#define _START_KEY_CH 48
#define _ALERT_LAST 10
#define _TRAN_DELTA_STEP 2


typedef struct tagAlertNode
{
  INT32S nTigerTime;
  INT32S nChcnt;
  INT32S nChNo[_MAX_CHCNT];
  FP32 fCof[_MAX_CHCNT];	
}AlertNode,*PAlertNode;

//INT32S InsertAlert(struct AlertNode* p,INT32S nChno,FP32 fcof);

#define _MAX_TRANCNT 5

typedef struct tagTranNode
{
  INT32S nChNo;
  INT32S nEventcnt;
  INT32S nTigerTime[_MAX_TRANCNT*2];
  FP32 fDeltaRPM[_MAX_TRANCNT];
}TranNode,*PTranNode;

//INT32S  InsertTran(struct TranNode* p,INT32S nTigerTime1, INT32S nTigerTime2,FP32 fdeltaRpm);

INT32S	Simu_Read_ChValue(void* pp[],int cnt[]);
INT32S	Simu_Read_ChData(void* pp[],int cnt[]);

INT32S Simu_Send_Data(void* ps);
INT32S Simu_BuildRamDisk(void);
INT32S Simu_BuildChSpeed(void);
INT32S Simu_Build_AlertPlan(void);
INT32S Simu_Build_TranPlan(void);
INT32S BuildBasicSimuData(void);
INT32S Simu_BulidAlert(void);
INT32S Simu_BulidTran(void);
INT32S CheckSimuData(void);
INT32S Simu_Build(void);
INT32S Simu_BulidTran(void);
INT32S	Simu_Begin_Read(void);
INT32S	Simu_End_Read(void);
INT32S WriteRamDisk(INT32U nOffset,INT32S pData[],INT32S nLength);
INT32S	Simu_Write_ChData(INT32S nLength[],INT32S nChNo[],INT32S nCnt,
                          INT32U nDynamicChDataOffset[],INT32U nDynamicKeyOffset[],INT32U nDynamicStep[],
                          INT32U nTranChDataOffset[],INT32U nTranKeyOffset[],INT32U nTranStep[]);

INT32S	OutputChValue(void);

#endif
