#include "common.h"

#include "interface.h"
#include "define2060.h"
//#include "net.h"
#include "ucos_ii.h"
#include "lib_mem.h"

#define PACKETSIZE 4096
extern Static_Triger_Table      g_StaticTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];
extern Dynamic_Triger_Table	    g_DynamicTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];
extern Tran_Triger_Table	    g_TranTrigerEventArray[_MAX_JKEY_CHANNEL_CNT];

extern Tst_Head_DCM_SigModuSampData_SYS*		g_pStaticChannelOrigin[_MAX_JKEY_CHANNEL_CNT][_MAX_STATIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
extern Tst_Head_DCM_SigModuSampData_SYS*		g_pDynamicChannelValue[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
extern Tst_Head_DCM_SigModuSampData_SYS*		g_pTranChannelValue[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT][_MAX_TRAN_GROUP];
extern INT32U		g_nChannel_Dynamic_Wave_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT];
extern INT32U       g_nChannel_Dynamic_Key_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_DYNAMIC_ARRAY_LTH];
extern INT32U		g_nChannel_Tran_Wave_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_SIGNAL_CHANNEL_CNT][_MAX_TRAN_GROUP];
extern INT32U		g_nChannel_Tran_Key_Data_Offset[_MAX_JKEY_CHANNEL_CNT][_MAX_TRAN_ARRAY_LTH][_MAX_TRAN_GROUP];
extern Param_Table_83		 g_83param;
extern Param_Table_12	     g_12Param;



//static int32_t StaticBuffer   [1<<20];
//static int32_t DynamicBuffer  [1<<20];
//static int32_t RealValueBuffer[1<<20];
//static int32_t RealDataBuffer [1<<20];
//static int32_t tranBuffer   [1<<25];
void send_data(int32_t pipe, int8_t *buffer, int32_t size);
void read_RamDisk(int32_t *nbuffer, const uint32_t addr, int32_t *buffer, int32_t len);
#define WAIT_TRIGGER(jkey, rArray, EventArray, ms) \
while(1) {\
  rArray = EventArray[jkey].ulnTrigerRead; \
    if(rArray == EventArray[jkey].ulnTrigerWrite) { \
      sleep_ms(ms); \
        if(_MAX_JKEY_CHANNEL_CNT == ++jkey) jkey = 0; \
          continue; \
    } \
    else { \
      break; \
    } \
}\
  
  
void realvalue_transfer(void * pParam);
void realdata_transfer(void * pParam);

#pragma segment="STATIC_BLOCK"
void static_transfer(void * pParam)
{
  uint32_t jkey = 0;
  int32_t rArray;
  int32_t channel = 0;
  uint32_t sBuffer = 0;

  Tst_Head_DCM_SigModuSampData_SYS* pSigModuSampData;
  ProtocolHead *pProtocolHead;
  Channel_Static_Value *pValueHead;
  uint32_t *StaticBuffer = __section_begin("STATIC_BLOCK");
  
  while(1)
  {
    WAIT_TRIGGER(jkey, rArray, g_StaticTrigerEventArray, 10)
      ;
    
    OSSchedLock();
    sBuffer = sizeof(ProtocolHead) + (g_83param.Device[jkey].nSignChannelNum*sizeof(Channel_Static_Value));
    StaticBuffer[0] = sBuffer + 8;
    StaticBuffer[1] = sBuffer + 4;
    sBuffer += 8;
    pProtocolHead = (ProtocolHead *)(StaticBuffer+2);
    pValueHead  = (Channel_Static_Value *)(StaticBuffer + 2 + (sizeof(ProtocolHead)>>2));
    
    /* head */
    pProtocolHead->nProtol = 2;
    pProtocolHead->nSonProtol = 3;
    pProtocolHead->nJkey = jkey;
    pProtocolHead->nTrigerTime = g_StaticTrigerEventArray[jkey].ulnTrigerTime[rArray];
    pProtocolHead->nTrigerType = g_StaticTrigerEventArray[jkey].ulnTrigerReason[rArray];
    pProtocolHead->nChcnt = g_83param.Device[jkey].nSignChannelNum;
    pProtocolHead->nGroupcnt = 0;
    
    /* sample */
    for (channel=0; channel<g_83param.Device[jkey].nSignChannelNum; channel++)
    {
      pSigModuSampData = g_pStaticChannelOrigin[jkey][rArray][channel];
      pValueHead->nTrigerTime = pSigModuSampData->ulnTmStamp_SYN1;
      pValueHead->n05xV = pSigModuSampData->unNoT1X;
      pValueHead->n1xP = pSigModuSampData->un1XP;
      pValueHead->n1xV = pSigModuSampData->un1XA;
      pValueHead->n2xP = pSigModuSampData->un2XP;
      pValueHead->n2xV = pSigModuSampData->un2XA;
      pValueHead->nAlertFlag = pSigModuSampData->tunStaticFlag1.tstVal.unFlag_Danger +
        pSigModuSampData->tunStaticFlag1.tstVal.unFlag_Alert;
      pValueHead->nAllMeasureValue = pSigModuSampData->tunDirect.nVal;
      pValueHead->nChNo = pSigModuSampData->tunChInfo.tstVal.tunChNo.unVal_PT2060;
      pValueHead->nRPM = pSigModuSampData->ulnRPM;
      ++pValueHead;
    }
    
    if(_MAX_STATIC_ARRAY_LTH == ++rArray)
      rArray = 0;
    g_StaticTrigerEventArray[jkey].ulnTrigerRead = rArray;
    OSSchedUnlock();
    
    OSSchedLock();
    //send_data(_STATIC_CHNO, (int8_t *)StaticBuffer, sBuffer);
    OSSchedUnlock();
    sleep_ms(20);
  }
}

void dynamic_transfer(void * pParam)
{
  uint32_t jkey = 0;
  int32_t rArray;
  uint32_t block = 0;
  uint32_t currBlock = 0;
  uint32_t headBlock = 0;
  
  int32_t channel = 0;
  uint32_t sJkeyBlock, sWaveBlock;
  uint32_t nValidBlocks = 0;
  //int8_t testdata[16];
  //uint32_t ii = 0;
  uint32_t JkeyAddrBase,   WaveAddrBase;
  uint32_t JkeyAddrOffset, WaveAddrOffset;
  
  uint32_t lastData;
  uint32_t sBuffer = 0;
  Channel_Dynamic_Wave_Data *pWaveData;
  //OS_ERR err;
  Tst_Head_DCM_SigModuSampData_SYS* pSigModuSampData;
  dynamic_vec32 helpBlocks[128];
  /*
  *len+2*4 len+1*4 ProtocolHead Channel_Dynamic_Value [head jkey wave][head jkey wave]...[head jkey wave]
  *
  */
  ProtocolHead *pProtocolHead;
  Channel_Dynamic_Value *pValueHead;
  int32_t *pBuffer;
  
  //_log("dynamic_transfer\n");
  //while(1)
  //{
  //    sleep_ms(1000);
  //    sprintf(testdata,"dynamic:%10d %6d\n",OSTimeGet(&err), ii++);
  //    _log(testdata);
  //    PC_DispStr(0, 1,  testdata, DISP_FGND_WHITE + DISP_BGND_GREEN);
  //}
#if 0
  while(1)
  {
    WAIT_TRIGGER(jkey, rArray, g_DynamicTrigerEventArray, 10)
      ;
    OSSchedLock(&err);
    sJkeyBlock = g_83param.Device[jkey].nChKeyBlockSize;
    sWaveBlock = g_83param.Device[jkey].nChDynamicBlockSize;
    nValidBlocks = g_DynamicTrigerEventArray[jkey].ChannelSignalDataTable[rArray].ulnBlockCnt;
    headBlock = g_DynamicTrigerEventArray[jkey].ChannelSignalDataTable[rArray].lnHeadBlock;
    JkeyAddrOffset = headBlock * sJkeyBlock;
    WaveAddrOffset = headBlock * sWaveBlock;
    
    pBuffer = DynamicBuffer;
    pProtocolHead = (ProtocolHead *)(pBuffer + 2);
    pValueHead  = (Channel_Dynamic_Value *)(pBuffer + 2 + (sizeof(ProtocolHead)>>2));
    pBuffer += 2;
    
    /* head */
    pProtocolHead->nProtol = 2;
    pProtocolHead->nSonProtol = 4;
    pProtocolHead->nJkey = jkey;
    pProtocolHead->nTrigerTime = g_DynamicTrigerEventArray[jkey].ChannelSignalDataTable[rArray].ulnTrigerTime;
    pProtocolHead->nTrigerType = g_DynamicTrigerEventArray[jkey].ChannelSignalDataTable[rArray].ulnTrigerReason;
    pProtocolHead->nChcnt = g_83param.Device[jkey].nSignChannelNum;
    pProtocolHead->nGroupcnt = 0;
    pBuffer += sizeof(ProtocolHead)>>2;
    
    /* sample */
    for (channel=0; channel<g_83param.Device[jkey].nSignChannelNum; channel++)
    {
      pSigModuSampData = g_pDynamicChannelValue[jkey][rArray][channel];
      pValueHead->nTrigerTime = pSigModuSampData->ulnTmStamp_SYN1;
      pValueHead->n05xV = pSigModuSampData->unNoT1X;
      pValueHead->n1xP = pSigModuSampData->un1XP;
      pValueHead->n1xV = pSigModuSampData->un1XA;
      pValueHead->n2xP = pSigModuSampData->un2XP;
      pValueHead->n2xV = pSigModuSampData->un2XA;
      pValueHead->nAlertFlag = pSigModuSampData->tunStaticFlag1.tstVal.unFlag_Danger +
        pSigModuSampData->tunStaticFlag1.tstVal.unFlag_Alert;
      pValueHead->nAllMeasureValue = pSigModuSampData->tunDirect.nVal;
      pValueHead->nChNo = pSigModuSampData->tunChInfo.tstVal.tunChNo.unVal_PT2060;
      pValueHead->nRPM = pSigModuSampData->ulnRPM;
      ++pValueHead;
    }
    pBuffer += ((g_83param.Device[jkey].nSignChannelNum*sizeof(Channel_Dynamic_Value))>>2);
    
    /*
    [head jkey wave]...[head jkey wave]
    */
    for (channel=0; channel<g_83param.Device[jkey].nSignChannelNum; channel++)
    {
      uint32_t nBlock;
      int32_t *ptr_tmp;
      JkeyAddrBase = g_nChannel_Dynamic_Key_Data_Offset [jkey][rArray];
      WaveAddrBase = g_nChannel_Dynamic_Wave_Data_Offset[jkey][rArray][channel];
      
      pWaveData = (Channel_Dynamic_Wave_Data*)pBuffer;
      pWaveData->nChNo = channel;
      pWaveData->nFrequency = g_83param.Device[jkey].nPickDynamicFrequency;
      pWaveData->nTrigerTime = pProtocolHead->nTrigerTime;
      
      pBuffer += (sizeof(Channel_Dynamic_Wave_Data)>>2)-1;
      lastData = *(pBuffer-1);
      
      /* jkey */
      currBlock = headBlock;
      ptr_tmp = pBuffer;
      for(block=0; block<nValidBlocks; block++)
      {
        ReadRamDisk(JkeyAddrBase+JkeyAddrOffset, pBuffer-1, sJkeyBlock);
        nBlock = *(pBuffer-1);
        *(pBuffer-1) = lastData;
        lastData = *((int32_t *)(pBuffer+nBlock-1));
        if(0 < block)
        {
          helpBlocks[block].len = nBlock;
          helpBlocks[block].addr = pBuffer;
        }
        pBuffer += nBlock;
        JkeyAddrOffset += sJkeyBlock;
        if(nValidBlocks == ++currBlock)
        {
          currBlock = 0;
          JkeyAddrOffset = 0;
        }
      }
      pWaveData->nKeycnt = pBuffer-ptr_tmp;
      
      /* wave */
      currBlock = headBlock;
      ptr_tmp = pBuffer;
      for(block=0; block<nValidBlocks; block++)
      {
        ReadRamDisk(WaveAddrBase+WaveAddrOffset, pBuffer-1, sWaveBlock);
        nBlock = *(pBuffer-1);
        *(pBuffer-1) = lastData;
        lastData = *((int32_t *)(pBuffer+nBlock-1));
        if(0 < block)
        {
          while(helpBlocks[block].len--)
            *helpBlocks[block].addr++ += pBuffer-ptr_tmp;
        }
        pBuffer += nBlock;
        WaveAddrOffset += sWaveBlock;
        if(nValidBlocks == ++currBlock)
        {
          currBlock = 0;
          WaveAddrOffset = 0;
        }
      }
      pWaveData->nPiecelen = pBuffer-ptr_tmp;;
    }
    if(_MAX_DYNAMIC_ARRAY_LTH == ++rArray)
      rArray = 0;
    g_DynamicTrigerEventArray[jkey].ulnTrigerRead = rArray;
    OSSchedUnlock(&err);
    
    sBuffer = (pBuffer - DynamicBuffer - 2) << 2;//all data length by byte
    DynamicBuffer[0] = sBuffer + 8;
    DynamicBuffer[1] = sBuffer + 4;
    sBuffer += 8;
    OSSchedLock(&err);
    send_data(_DYNAMIC_CHNO, (int8_t *)DynamicBuffer, sBuffer);
    OSSchedUnlock(&err);
    sleep_ms(20);
  }
#endif
}

void tran_transfer(void * pParam)
{
  uint32_t jkey = 0;
  uint32_t rArray = 0;
  uint32_t block = 0;
  uint32_t currBlock = 0;
  uint32_t nBlock = 0;
  int32_t  channel = 0;
  uint32_t group = 0;
  uint32_t sJkeyBlock, sWaveBlock;
  uint32_t nValidBlocks = 0;
  uint32_t JkeyAddrBase, WaveAddrBase;
  Channel_Tran_Wave_Data *pWaveData;
  int32_t  headGroup, currGroup;
  uint32_t lastData;
  uint32_t sBuffer = 0;
  Tst_Head_DCM_SigModuSampData_SYS* pSigModuSampData;
  int8_t ret = 0;
  ProtocolHead *pProtocolHead;
  Channel_Tran_Value *pValueHead;
  int32_t *pBuffer;
  int32_t *tranBuffer;
  //_log("tran_transfer\n");
#if 0
  tranBuffer = (int32_t*)malloc(1<<28);
  while(1)
  {
    WAIT_TRIGGER(jkey, rArray, g_TranTrigerEventArray, 10)
      ;
    OSSchedLock(&err);
    sJkeyBlock = g_83param.Device[jkey].nChKeyBlockSize;
    sWaveBlock = g_83param.Device[jkey].nChTranBlockSize;
    headGroup = g_TranTrigerEventArray[jkey].lnGroupHeadSit[rArray];
    
    pBuffer = tranBuffer;
    pProtocolHead = (ProtocolHead *)(pBuffer + 2);
    pValueHead  = (Channel_Tran_Value *)(pBuffer + 2 + (sizeof(ProtocolHead)>>2));
    pBuffer += 2;
    
    /* head */
    pProtocolHead->nProtol = 2;
    pProtocolHead->nSonProtol = 5;
    pProtocolHead->nJkey = jkey;
    pProtocolHead->nTrigerTime = g_TranTrigerEventArray[jkey].ulnTrigerTime[rArray];
    pProtocolHead->nTrigerType = g_TranTrigerEventArray[jkey].ulnTrigerReason[rArray];
    pProtocolHead->nChcnt = g_83param.Device[jkey].nSignChannelNum;
    pProtocolHead->nGroupcnt = _MAX_TRAN_GROUP;
    pBuffer += sizeof(ProtocolHead)>>2;
    
    /* sample */
    
    currGroup = headGroup;
    for(group = 0; group < _MAX_TRAN_GROUP; ++group)
    {
      for (channel=0; channel < g_83param.Device[jkey].nSignChannelNum; ++channel)
      {
        pSigModuSampData = g_pTranChannelValue[jkey][rArray][channel][currGroup];
        pValueHead->nTrigerTime = pSigModuSampData->ulnTmStamp_SYN1;
        pValueHead->n05xV = pSigModuSampData->unNoT1X;
        pValueHead->n1xP = pSigModuSampData->un1XP;
        pValueHead->n1xV = pSigModuSampData->un1XA;
        pValueHead->n2xP = pSigModuSampData->un2XP;
        pValueHead->n2xV = pSigModuSampData->un2XA;
        pValueHead->nAlertFlag = pSigModuSampData->tunStaticFlag1.tstVal.unFlag_Danger +
          pSigModuSampData->tunStaticFlag1.tstVal.unFlag_Alert;
        pValueHead->nAllMeasureValue = pSigModuSampData->tunDirect.nVal;
        pValueHead->nChNo = pSigModuSampData->tunChInfo.tstVal.tunChNo.unVal_PT2060;
        pValueHead->nRPM = pSigModuSampData->ulnRPM;
        ++pValueHead;
      }
      if(_MAX_TRAN_GROUP == ++currGroup)
        currGroup = 0;
    }
    pBuffer += ((g_83param.Device[jkey].nSignChannelNum*sizeof(Channel_Dynamic_Value)*_MAX_TRAN_GROUP)>>2);
    
    /*
    [head jkey wave]...[head jkey wave]
    */
    currGroup = headGroup;
    for (group=0; group<_MAX_TRAN_GROUP; group++) {
      for(channel=0; channel<g_83param.Device[jkey].nSignChannelNum; channel++) {
        pWaveData = (Channel_Tran_Wave_Data*)pBuffer;
        pWaveData->nChNo = channel;
        pWaveData->nFrequency = g_83param.Device[jkey].nPickDynamicFrequency;
        pWaveData->nTrigerTime = g_pTranChannelValue[jkey][rArray][channel][currGroup]->ulnTmStamp_SYN1;
        pBuffer += (sizeof(Channel_Tran_Wave_Data)>>2)-1;
        lastData = *(pBuffer-1);
        /* Jkey */
        JkeyAddrBase = g_nChannel_Tran_Key_Data_Offset[jkey][rArray][currGroup];
        ReadRamDisk(JkeyAddrBase, pBuffer-1, sJkeyBlock);
        nBlock = *(pBuffer-1);
        *(pBuffer-1) = lastData;
        lastData = *((uint32_t *)(pBuffer+nBlock-1));
        pBuffer += nBlock;
        pWaveData->nKeycnt = nBlock;
        /* Wave */
        WaveAddrBase = g_nChannel_Tran_Wave_Data_Offset[jkey][rArray][channel][currGroup];
        ReadRamDisk(WaveAddrBase, pBuffer-1, sWaveBlock);
        nBlock = *(pBuffer-1);
        *(pBuffer-1) = lastData;
        lastData = *((uint32_t *)(pBuffer+nBlock-1));
        pBuffer += nBlock;
        pWaveData->nPiecelen = nBlock;
      }
      if(_MAX_TRAN_GROUP == ++currGroup)
        currGroup = 0;
    }
    if(_MAX_TRAN_ARRAY_LTH == ++rArray)
      rArray = 0;
    g_TranTrigerEventArray[jkey].ulnTrigerRead = rArray;
    OSSchedUnlock(&err);
    
    sBuffer = (pBuffer - tranBuffer - 2) << 2;
    tranBuffer[0] = sBuffer + 8;
    tranBuffer[1] = sBuffer + 4;
    sBuffer += 8;
    
    OSSchedLock(&err);
    send_data(_TRAN_CHNO, (int8_t *)tranBuffer, sBuffer);
    OSSchedUnlock(&err);
    sleep_ms(20);
  }
#endif
}

void realvalue_transfer(void * pParam)
{
  uint32_t jkey = 0;
  uint32_t rArray = 0;
  int32_t  channel = 0;
  int32_t  point = 0;
  uint32_t currWriteGroup = 0;
  uint32_t nData;
  MrsptRTValue   *pValueHead;
  ProtocolHead   *pProtocolHead;
  
  uint32_t sBuffer = 0;
  Tst_Head_DCM_SigModuSampData_SYS* pSigModuSampData;
  
  int8_t ret = 0;
  int32_t time_cnt = 0;
  //_log("real_transfer\n");
#if 0
  while(1)
  {
    OSSchedLock( );
    
    rArray = g_TranTrigerEventArray[0].ulnTrigerRead;
    currWriteGroup = g_TranTrigerEventArray[0].ulnCurWriteGroup;
    pProtocolHead = (ProtocolHead *)(RealValueBuffer + 2);
    pValueHead  = (MrsptRTValue *)(RealValueBuffer + 2 + (sizeof(ProtocolHead)>>2));
    
    /* head */
    pProtocolHead->nProtol = 2;
    pProtocolHead->nSonProtol = 6;
    pProtocolHead->nJkey = 0;
    pProtocolHead->nTrigerTime = ++time_cnt;//g_TranTrigerEventArray[0].ulnTrigerTime[rArray];
    pProtocolHead->nTrigerType = g_TranTrigerEventArray[0].ulnTrigerReason[rArray];
    pProtocolHead->nGroupcnt = 1;
    pProtocolHead->nMrsPtcnt = g_83param.nMrsPtcnt;
    for(jkey=0; jkey<_MAX_JKEY_CHANNEL_CNT; ++jkey) {
      pProtocolHead->nChcnt += g_83param.Device[jkey].nSignChannelNum;
    }
    
    /* head */
    pSigModuSampData = g_pTranChannelValue[0][rArray][0][currWriteGroup];
    pValueHead->nPickTime = pSigModuSampData->ulnTmStamp_SYN1;
    pValueHead->nMrsPtcnt = g_83param.nMrsPtcnt;
    
    /* data */
    nData = 0;
    for(point=0; point < pValueHead->nMrsPtcnt; ++point) {
      int i;
      for(i=0; i<g_83param.nPtMrsNo[point].nChCnt; ++i) {
        jkey = g_83param.nPtMrsNo[point].jkey;
        rArray = g_TranTrigerEventArray[jkey].ulnTrigerRead;
        channel = g_83param.nPtMrsNo[point].channel[i];
        currWriteGroup = g_TranTrigerEventArray[jkey].ulnCurWriteGroup;
        pSigModuSampData = g_pTranChannelValue[jkey][rArray][channel][currWriteGroup];
        pValueHead->nChValue[nData].nMrsPt = g_83param.nMrsPtTable[point];
        pValueHead->nChValue[nData].data[0].fAllMeasureValue = pSigModuSampData->tunDirect.nVal;
        pValueHead->nChValue[nData].data[0].nAlertFlag = 
          pSigModuSampData->tunStaticFlag1.tstVal.unFlag_Danger +                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
            pSigModuSampData->tunStaticFlag1.tstVal.unFlag_Alert;
        ++nData;
      }
    }
    //assert(0 < nData);
    OSSchedUnlock( );
    
    sBuffer = sizeof(ProtocolHead) + sizeof(MrsptRTValue) + (nData-1)*sizeof(RTChValue);
    RealValueBuffer[0] = sBuffer + 8;
    RealValueBuffer[1] = sBuffer + 4;
    sBuffer += 8;
    
    OSSchedLock( );
    //send_data(_REALTIME_VALUE_CHNO, (int8_t *)RealValueBuffer, sBuffer);
    OSSchedUnlock( );
    
    sleep_ms(40);
  }
#endif
}

void realdata_transfer(void * pParam)
{
  uint32_t jkey = 0;
  uint32_t rArray = 0;
  int32_t  channel = 0;
  int32_t  point = 0;
  uint32_t currWriteGroup = 0;
  uint32_t group = 0;
  uint32_t sJkeyBlock, sWaveBlock;
  uint32_t JkeyAddrBase, WaveAddrBase;
  uint32_t nData  = 0;
  int32_t nBlock = 0;
  int32_t *pValueHead;
  ProtocolHead   *pProtocolHead;
  MrsptRTData *pMrsptRTData;
  uint32_t sBuffer = 0;
  Tst_Head_DCM_SigModuSampData_SYS* pSigModuSampData;
  int8_t ret = 0;
  int32_t time_cnt = 0;
  //_log("real_transfer\n");
  sleep_ms(5000);
  sleep_ms(5000);
  //len+8 len+4 ProtocolHead MrsPtTable value data
#if 0
  while(1)
  {
    OSSchedLock();
    rArray = g_TranTrigerEventArray[0].ulnTrigerRead;
    currWriteGroup = g_TranTrigerEventArray[0].ulnCurWriteGroup;
    pProtocolHead = (ProtocolHead *)(RealDataBuffer + 2);
    pValueHead  = (int32_t *)(RealDataBuffer + 2 + (sizeof(ProtocolHead)>>2));
    
    /* head */
    pProtocolHead->nProtol = 2;
    pProtocolHead->nSonProtol = 7;
    pProtocolHead->nJkey = 0;
    pProtocolHead->nTrigerTime = ++time_cnt;//g_TranTrigerEventArray[0].ulnTrigerTime[rArray];
    pProtocolHead->nTrigerType = g_TranTrigerEventArray[0].ulnTrigerReason[rArray];
    pProtocolHead->nGroupcnt = 1;
    pProtocolHead->nMrsPtcnt = g_83param.nMrsPtcnt;
    for(jkey=0; jkey<_MAX_JKEY_CHANNEL_CNT; ++jkey) {
      pProtocolHead->nChcnt += g_83param.Device[jkey].nSignChannelNum;
    }
    
    Mem_Copy(pValueHead, g_83param.nMrsPtTable, g_83param.nMrsPtcnt*sizeof(int32_t));
    sBuffer = sizeof(ProtocolHead) + g_83param.nMrsPtcnt*sizeof(int32_t);
    pMrsptRTData = (MrsptRTData *)(RealDataBuffer + 2 + (sBuffer>>2));
    /* value & data */
    nData = 0;
    for(point=0; point < pProtocolHead->nMrsPtcnt; ++point) {
      int i;
      for(i=0; i<g_83param.nPtMrsNo[point].nChCnt; ++i) {
        jkey = g_83param.nPtMrsNo[point].jkey;
        rArray = g_TranTrigerEventArray[jkey].ulnTrigerRead;
        channel = g_83param.nPtMrsNo[point].channel[i];
        group = g_TranTrigerEventArray[jkey].ulnCurWriteGroup;
        if(0 == group)
          group = _MAX_TRAN_GROUP-1;
        else
          --group;
        
        sJkeyBlock = g_83param.Device[jkey].nChKeyBlockSize;
        sWaveBlock = g_83param.Device[jkey].nChTranBlockSize;
        JkeyAddrBase = g_nChannel_Tran_Key_Data_Offset[jkey][rArray][group];
        WaveAddrBase = g_nChannel_Tran_Wave_Data_Offset[jkey][rArray][channel][group];
        pMrsptRTData = (MrsptRTData *)(RealDataBuffer + 2 + (sBuffer>>2));
        /* value */
        pSigModuSampData = g_pTranChannelValue[jkey][rArray][channel][group];
        pMrsptRTData->data[0].Value.nTrigerTime = pSigModuSampData->ulnTmStamp_SYN1;
        pMrsptRTData->data[0].Value.n05xV = pSigModuSampData->unNoT1X;
        pMrsptRTData->data[0].Value.n1xP = pSigModuSampData->un1XP;
        pMrsptRTData->data[0].Value.n1xV = pSigModuSampData->un1XA;
        pMrsptRTData->data[0].Value.n2xP = pSigModuSampData->un2XP;
        pMrsptRTData->data[0].Value.n2xV = pSigModuSampData->un2XA;
        pMrsptRTData->data[0].Value.nAlertFlag = pSigModuSampData->tunStaticFlag1.tstVal.unFlag_Danger +
          pSigModuSampData->tunStaticFlag1.tstVal.unFlag_Alert;
        pMrsptRTData->data[0].Value.nAllMeasureValue = pSigModuSampData->tunDirect.nVal;
        pMrsptRTData->data[0].Value.nChNo = pSigModuSampData->tunChInfo.tstVal.tunChNo.unVal_PT2060;
        pMrsptRTData->data[0].Value.nRPM = pSigModuSampData->ulnRPM;
        pMrsptRTData->data[0].Value.nRpmFlag = nData;
        sBuffer += sizeof(struct tagChValue);
        
        /* data head*/
        pMrsptRTData->data[0].Data.nChNo = channel;
        pMrsptRTData->data[0].Data.nFrequency = g_83param.Device[jkey].nPickDynamicFrequency;
        pMrsptRTData->data[0].Data.nTrigerTime = g_pTranChannelValue[jkey][rArray][channel][group]->ulnTmStamp_SYN1;
        sBuffer += sizeof(struct tagChData2060)-4;
        
        /* jkey data */
        read_RamDisk(&nBlock, JkeyAddrBase, pMrsptRTData->data[0].Data.pData, sJkeyBlock);
        pMrsptRTData->data[0].Data.nKeycnt = nBlock;
        sBuffer += nBlock*sizeof(int32_t);
        /* Wave data*/
        read_RamDisk(&nBlock, WaveAddrBase, pMrsptRTData->data[0].Data.pData + nBlock, sWaveBlock);
        pMrsptRTData->data[0].Data.nPiecelen = nBlock;
        sBuffer += nBlock*sizeof(int32_t);
        
        ++nData;
      }
    }
    
    OSSchedUnlock( );
    
    RealDataBuffer[0] = sBuffer + 8;
    RealDataBuffer[1] = sBuffer + 4;
    sBuffer += 8;
    
    OSSchedLock( );
    //send_data(_REALTIME_WAVE_CHNO, (int8_t *)RealDataBuffer, sBuffer);
    OSSchedUnlock( );
    
    sleep_ms(200);
  }
#endif
}

void send_data(int32_t pipe, int8_t *buffer, int32_t size) {
#if 0
  int l = 0;
  if(0 == open_DataPipe(pipe))
  {
    while (PACKETSIZE < size)
    {
      write_DataPipe(pipe, buffer + l, PACKETSIZE);
      l += PACKETSIZE;
      size -= PACKETSIZE;
    }
    write_DataPipe(pipe, buffer + l, size);
    close_DataPipe(pipe);
  }
#endif
}

//buffer-1 must be valid
void read_RamDisk(int32_t *nbuffer, const uint32_t addr, int32_t *buffer, int32_t len) {
  int32_t data = *(buffer-1);
  ReadRamDisk(addr, buffer-1, len);
  *nbuffer = *(buffer-1);
  *(buffer-1) = data;
}
