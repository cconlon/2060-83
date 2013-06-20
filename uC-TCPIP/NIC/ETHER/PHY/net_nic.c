#define   NET_NIC_MODULE
#include  <net.h>
#include  <bsp.h>

#include  <include/SAM9X35.h>
#include  <net_nic.h>
extern sEmacd gEmacd;
static  void NetNIC_IntInit(void);
static  void NetNIC_ISR_Handler(void);
static  void NetNIC_RxISR_Handler(void);               /* ISR for RX interrupts.                              */
static  void NetNIC_TxISR_Handler(void);               /* ISR for TX interrupts.                              */
static  void NetNIC_TxPktDiscard(NET_ERR *perr);

void  NetNIC_Init (NET_ERR  *perr)
{
  /* ----------------- PERFORM NIC/OS INIT ------------------ */
  NetOS_NIC_Init(perr);                                               /* Create NIC objs.                                         */
  if (*perr != NET_OS_ERR_NONE) {
    return;
  }
  /* ------------------- INIT NIC STATUS -------------------- */
  NetNIC_ConnStatus           =  DEF_OFF;
  /* --------------- INIT NIC STAT & ERR CTRS --------------- */
#if (NET_CTR_CFG_STAT_EN        == DEF_ENABLED)
  NetNIC_StatRxPktCtr         =  0;
  NetNIC_StatTxPktCtr         =  0;
#endif
  
#if (NET_CTR_CFG_ERR_EN         == DEF_ENABLED)
  NetNIC_ErrRxPktDiscardedCtr =  0;
  NetNIC_ErrTxPktDiscardedCtr =  0;
#endif
  /* ---------------------- INIT EMAC --------------------- */
  
  NET_EMAC_PHY_Init(perr);
  
#if (NET_NIC_CFG_INT_CTRL_EN == DEF_ENABLED)
  NetNIC_IntInit();                                                   /* See Note #5.                                        */
#endif
  NetNIC_ConnStatus = DEF_YES;
  *perr = NET_NIC_ERR_NONE;
}

static void NetNIC_IntInit(void)
{
  NET_EMAC_IntInit(NetNIC_ISR_Handler);
}

void  NetNIC_IntEn (NET_ERR  *perr)
{
  NET_EMAC_IntEn();
  *perr = NET_NIC_ERR_NONE;
}

void NetNIC_ISR_Handler(void)
{
  OS_CPU_SR  cpu_sr;
  
  OS_ENTER_CRITICAL()
    ;
  OSIntEnter();
  OS_EXIT_CRITICAL()
    ;
  EMACD_Handler2(&gEmacd, NetNIC_RxISR_Handler, NetNIC_TxISR_Handler);
#if (NET_NIC_CFG_INT_CTRL_EN == DEF_ENABLED)
  AIC->AIC_ICCR = 1 << ID_EMAC;
#endif
  
  OSIntExit();
}

static void  NetNIC_RxISR_Handler (void)
{
  NET_ERR     err;
  NetOS_IF_RxTaskSignal(&err);                                    /* Signal the Rx task to process the recieved packet        */
}

static void NetNIC_TxISR_Handler(void)
{
  NetOS_NIC_TxRdySignal();
}


void  NetNIC_RxPkt (void        *ppkt,
                    CPU_INT16U   size,
                    NET_ERR     *perr)
{
#if (NET_CTR_CFG_STAT_EN     == DEF_ENABLED)
#if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
  CPU_SR   cpu_sr;
#endif
#endif
  
  size = size;
  if (Net_InitDone != DEF_YES) {                                      /* If init NOT complete, exit rx (see Note #2).             */
    *perr = NET_ERR_INIT_INCOMPLETE;
    return;
  }
  
#if (NET_ERR_CFG_ARG_CHK_DBG_EN == DEF_ENABLED)                         /* --------------------- VALIDATE PTR --------------------- */
  if (ppkt == (void *)0) {
    *perr = NET_NIC_ERR_NULL_PTR;
    return;
  }
  
  /* --------------------- VALIDATE SIZE -------------------- */
  if (size < NET_IF_FRAME_MIN_SIZE) {
    *perr = NET_NIC_ERR_INVALID_SIZE;
    return;
  }
#endif
  
  if(NET_EMAC_Poll(ppkt))
  {
    NetOS_IF_RxTaskSignal(perr);
    if(NET_IF_ERR_NONE != *perr)
      return;
  }
  *perr         =  NET_NIC_ERR_NONE;
  
  NET_CTR_STAT_INC(NetNIC_StatRxPktCtr);
  
  *perr = NET_NIC_ERR_NONE;
}

void  NetNIC_RxPktDiscard (CPU_INT16U   size,
                           NET_ERR     *perr)
{
  if (Net_InitDone != DEF_YES) {                                      /* If init NOT complete, exit discard (see Note #1).        */
    *perr = NET_ERR_INIT_INCOMPLETE;
    return;
  }
  
  if (size > 0) {
    EMACD_RxDiscard(&gEmacd);
  }
  
  NET_CTR_ERR_INC(NetNIC_ErrRxPktDiscardedCtr);
  
  *perr = NET_NIC_ERR_NONE;
}

void  NetNIC_TxPkt (void        *ppkt,
                    CPU_INT16U   size,
                    NET_ERR     *perr)
{
#if (NET_CTR_CFG_STAT_EN     == DEF_ENABLED)
#if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
  CPU_SR   cpu_sr;
#endif
#endif
  
  
  if (Net_InitDone != DEF_YES) {                                      /* If init NOT complete, exit tx (see Note #1).             */
    *perr = NET_ERR_INIT_INCOMPLETE;
    return;
  }
  
#if (NET_ERR_CFG_ARG_CHK_DBG_EN == DEF_ENABLED)                         /* --------------------- VALIDATE PTR --------------------- */
  if (ppkt == (void *)0) {
    NetNIC_TxPktDiscard(perr);
    return;
  }
#endif
  
  EMACD_Send2(&gEmacd, size);
  
  if (*perr != NET_NIC_ERR_NONE) {
  NetNIC_TxPktDiscard(perr);
    return;
  }
    
  NET_CTR_STAT_INC(NetNIC_StatTxPktCtr);
  
  *perr = NET_NIC_ERR_NONE;
}
  
static void  NetNIC_TxPktDiscard (NET_ERR  *perr)
{
NET_CTR_ERR_INC(NetNIC_ErrTxPktDiscardedCtr);
  *perr = NET_ERR_TX;
}
  
void  NetNIC_TxPktPrepare (void        *ppkt,
CPU_INT16U   size,
NET_ERR     *perr)
{
  if (Net_InitDone != DEF_YES) {                                      /* If init NOT complete, exit tx (see Note #1).        */
    *perr = NET_ERR_INIT_INCOMPLETE;
    return;
  }
  
#if (NET_ERR_CFG_ARG_CHK_DBG_EN == DEF_ENABLED)                         /* ------------------- VALIDATE PTR ------------------ */
  if (ppkt == (void *)0) {
    NetNIC_TxPktDiscard(perr);
    return;
  }
#endif
  
  EMACD_TxPrepare(&gEmacd, ppkt, size);                     /* Preparte to tx pkt to AT91RM9200.                   */
  *perr = NET_NIC_ERR_NONE;
  if (*perr != NET_NIC_ERR_NONE) {
    NetNIC_TxPktDiscard(perr);
    return;
  }
  *perr = NET_NIC_ERR_NONE;
}

/*
*********************************************************************************************************
*                                        NetNIC_ConnStatusGet()
*
* Description : Get NIC's network connection status.
*
*               (1) Obtaining the NIC's network connection status is encapsulated in this function for the
*                   possibility that obtaining a NIC's connection status requires a non-trivial procedure.
*
*
* Argument(s) : None.
*
* Return(s)   : NIC network connection status :
*
*                   DEF_OFF     Network connection DOWN.
*                   DEF_ON      Network connection UP.
*
* Caller(s)   : NetIF_Pkt_Tx().
*********************************************************************************************************
*/

CPU_BOOLEAN  NetNIC_ConnStatusGet (void)
{
  return (NetNIC_ConnStatus);
}

/*
*********************************************************************************************************
*                                             DumpRegs()
*
* Description : Prints the content of all DM9000 internal registers (NIC and PHY) to the IAR console
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  DumpRegs (void)
{
}
