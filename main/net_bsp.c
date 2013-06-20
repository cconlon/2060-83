/*
*********************************************************************************************************
*                                              uC/TCP-IP
*                                      The Embedded TCP/IP Stack
*
*                            (c) Copyright 2004; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*
*                                BOARD SUPPORT PACKAGE (BSP) FUNCTIONS
*
*                                          AT91SAM9261
*
* Filename   : net_bsp.c
* Programmer : Brian Nagel
*            : Eric Shufro
* Version    : 1.0
* Date       : 01/02/2007
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
#include "netservices.h"


pHandler theRxISR_Handler;
pHandler theTxISR_Handler;

/*
*********************************************************************************************************
*                                            GLOBALS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                            Initialize the NIC hardware
*
* Description : This function is called by NetBSP_NIC_HW_Init() to configure the IO pins necessary
*             : to communicate between the AT91SAM9261 and the DM9000E.
*
* Argument(s) : none.
*
* Returns     : none.
*
* Notes(s)    : (1) I/O configuration for the IRQ input occurs in NetBSP_IntInit()
*********************************************************************************************************
*/

void  NetBSP_NIC_HW_Init (void)
{
}

/*
*********************************************************************************************************
*                              Clear NIC interrupts in the AIC
*
* Description : This function clears NIC interrupts.
*
* Arguments   : none.
*
* Returns     : none.
*
* Caller(s)   : NetNIC_ISR_Handler()
*********************************************************************************************************
*/

void  NetBSP_IntClr (void)
{
}



/*
*********************************************************************************************************
*                                        NetBSP_DlyU()
*
* Description : This function delays for a certain number of microseconds.
*
* Arguments   : us      the number of microseconds to delay
*
* Returns     : none
*********************************************************************************************************
*/

void  NetBSP_DlyU (CPU_INT16U us)
{
  CPU_INT32U  clks;
  CPU_INT32U  i;
  
  
  clks = (us * 1);
  
  for (i = 0; i < clks; i++) {
    ;
  }
}

/*
*********************************************************************************************************
*                                        NetBSP_DlyM()
*
* Description : This function delays for a certain number of milliseconds.
*
* Arguments   : ms      the number of milliseconds to delay
*
* Returns     : none
*********************************************************************************************************
*/

void  NetBSP_DlyM (CPU_INT16U ms)
{
  OSTimeDlyHMSM(0, 0, 0, ms);                                 /* Delay                                                    */
}

/*
*********************************************************************************************************
*                                          NetUtil_TS_Get()
*
* Description : Get current Internet Timestamp.
*
*               (1) "The Timestamp is a right-justified, 32-bit timestamp in milliseconds since midnight
*                    UT [Universal Time]" (RFC #791, Section 3.1 'Options : Internet Timestamp').
*
*               (2) The developer is responsible for providing a real-time clock with correct time-zone
*                   configuration to implement the Internet Timestamp.
*
*
* Argument(s) : none.
*
* Return(s)   : Internet Timestamp.
*
* Caller(s)   : various.
*
* Note(s)     : none.
*********************************************************************************************************
*/

NET_TS  NetUtil_TS_Get (void)
{
  NET_TS  ts;
  
  
  /* $$$$ Insert code to return Internet Timestamp (see Notes #1 & #2).   */
  
  ts = NET_TS_NONE;
  
  return (ts);
}

/*
*********************************************************************************************************
*                                        NetTCP_InitTxSeqNbr()
*
* Description : Initialize the TCP Transmit Initial Sequence Counter, 'NetTCP_TxSeqNbrCtr'.
*
*               (1) Possible initialization methods include :
*
*                   (a) Time-based initialization is one preferred method since it more appropriately
*                       provides a pseudo-random initial sequence number.
*                   (b) Hardware-generated random number initialization is NOT a preferred method since it
*                       tends to produce a discrete set of pseudo-random initial sequence numbers--often
*                       the same initial sequence number.
*                   (c) Hard-coded initial sequence number is NOT a preferred method since it is NOT random.
*
*                   See also 'net_tcp.h  MACRO'S  Note #2'.
*
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : NetTCP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#ifdef  NET_TCP_MODULE_PRESENT
void  NetTCP_InitTxSeqNbr (void)
{
  NetTCP_TxSeqNbrCtr = NET_TCP_SEQ_NBR_NONE;
}
#endif

/*$PAGE*/
/*
*********************************************************************************************************
*                                     NetTCP_TxConnRTT_GetTS_ms()
*
* Description : Get a TCP Round-Trip Time (RTT) timestamp.
*
*               (1) (a) (1) Although RFC #2988, Section 4 states that "there is no requirement for the
*                           clock granularity G used for computing RTT measurements ... experience has
*                           shown that finer clock granularities (<= 100 msec) perform somewhat better
*                           than more coarse granularities".
*
*                       (2) (A) RFC #2988, Section 2.4 states that "whenever RTO is computed, if it is
*                               less than 1 second then the RTO SHOULD be rounded up to 1 second".
*
*                           (B) RFC #1122, Section 4.2.3.1 states that "the recommended ... RTO ... upper
*                               bound should be 2*MSL" where RFC #793, Section 3.3 'Sequence Numbers :
*                               Knowing When to Keep Quiet' states that "the Maximum Segment Lifetime
*                               (MSL) is ... to be 2 minutes".
*
*                               Therefore, the required upper bound is :
*
*                                   2 * MSL = 2 * 2 minutes = 4 minutes = 240 seconds
*
*                   (b) Therefore, the developer is responsible for providing a timestamp clock with
*                       adequate resolution to satisfy the clock granularity (see Note #1a1) & adequate
*                       range to satisfy the minimum/maximum RTO values (see Note #1a2).
*
*
* Argument(s) : none.
*
* Return(s)   : TCP RTT timestamp, in milliseconds.
*
* Caller(s)   : NetTCP_RxPktValidate(),
*               NetTCP_TxPktPrepareHdr().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#ifdef  NET_TCP_MODULE_PRESENT
NET_TCP_TX_RTT_TS_MS  NetTCP_TxConnRTT_GetTS_ms (void)
{
#if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
  CPU_SR                cpu_sr;
#endif
  CPU_INT32U            clk_tick;
  CPU_INT32U            clk_tick_th;
  NET_TCP_TX_RTT_TS_MS  rtt_ts_ms;
  
  
  CPU_CRITICAL_ENTER();
  clk_tick         =  OSTime;
  CPU_CRITICAL_EXIT();
  
  if (OS_TICKS_PER_SEC > 0) {
    clk_tick_th  = (NET_TCP_TX_RTT_TS_MAX /                 /* MUST cfg clk tick th ...                             */
                    DEF_TIME_NBR_mS_PER_SEC);               /* ... as multiple of max ts (see Note #2b).            */
    clk_tick    %=  clk_tick_th;                            /* Truncate clk ticks to avoid ovf (see Note #2a).      */
    
    rtt_ts_ms    = (NET_TCP_TX_RTT_TS_MS)((clk_tick * DEF_TIME_NBR_mS_PER_SEC) / OS_TICKS_PER_SEC);
  } else {
    rtt_ts_ms    = (NET_TCP_TX_RTT_TS_MS)clk_tick;
  }
  
  return (rtt_ts_ms);
}
#endif
/*----------------------------------------------------------------------------
*        Local variables
*----------------------------------------------------------------------------*/
/** EMAC packet processing offset */
#define EMAC_RCV_OFFSET     0

/** Delay before a link check */
#define EMAC_LINK_CHECK_DELAY       1000000

/** Number of buffer for RX */
#define RX_BUFFERS  16
/** Number of buffer for TX */
#define TX_BUFFERS  32

#if !defined(EMAC)
#define EMAC       EMAC0
#define ID_EMAC    ID_EMAC0
#endif

/** EMAC power control pin */
#if !defined(BOARD_EMAC_POWER_ALWAYS_ON)
static const Pin emacPwrDn[] = {BOARD_EMAC_PIN_PWRDN};
#endif

/** The PINs' on PHY reset */
static const Pin emacRstPins[] = {BOARD_EMAC_RST_PINS};

/** The PINs for EMAC */
static const Pin emacPins[] = {BOARD_EMAC_RUN_PINS};

/** The MAC address used for demo */
static uint8_t MacAddress[6] = {0x00, 0x45, 0x56, 0x78, 0x9a, 0xbc};

/** TX descriptors list */
#ifdef __ICCARM__          /* IAR */
#pragma data_alignment=8   /* IAR */
#endif                     /* IAR */
static sEmacTxDescriptor gTxDs[TX_BUFFERS];
/** TX callbacks list */
static fEmacdTransferCallback gTxCbs[TX_BUFFERS];
/** RX descriptors list */
#ifdef __ICCARM__          /* IAR */
#pragma data_alignment=8   /* IAR */
#endif                     /* IAR */
static sEmacRxDescriptor gRxDs[RX_BUFFERS];
/** Send Buffer */
/* Section 3.6 of AMBA 2.0 spec states that burst should not cross 1K Boundaries.
Receive buffer manager writes are burst of 2 words => 3 lsb bits of the address
shall be set to 0 */
#ifdef __ICCARM__          /* IAR */
#pragma data_alignment=8   /* IAR */
#endif                     /* IAR */
static uint8_t pTxBuffer[TX_BUFFERS * EMAC_TX_UNITSIZE] __attribute__((aligned(8)));

#ifdef __ICCARM__          /* IAR */
#pragma data_alignment=8   /* IAR */
#endif                     /* IAR */
/** Receive Buffer */
static uint8_t pRxBuffer[RX_BUFFERS * EMAC_RX_UNITSIZE] __attribute__((aligned(8)));

/** The EMAC driver instance */
sEmacd gEmacd;

/** The MACB driver instance */
static Macb gMacb;

int NET_EMAC_PHY_Init(NET_ERR *perr)
{   
  uint32_t errCount = 0;
  sEmacd    *pEmacd = &gEmacd;
  Macb      *pMacb  = &gMacb;
  
#if !defined(BOARD_EMAC_POWER_ALWAYS_ON)
  /* clear PHY power down mode */   
  PIO_Configure(emacPwrDn, 1);
#endif
  /* Init EMAC driver structure */
  EMACD_Init(pEmacd, EMAC, ID_EMAC, 0, 0);
  //EMAC_DiscardFCSEnable(gEmacd.pHw, 1);
  EMACD_InitTransfer(pEmacd,
                     pRxBuffer, gRxDs, RX_BUFFERS,
                     pTxBuffer, gTxDs, gTxCbs, TX_BUFFERS);
  EMAC_SetAddress(gEmacd.pHw, 0, MacAddress);
  NetIF_MAC_AddrSet(MacAddress, perr);
  
  /* Init MACB driver */
  MACB_Init(pMacb, pEmacd, BOARD_EMAC_PHY_ADDR);

  /* PHY initialize */
  if (!MACB_InitPhy(pMacb, BOARD_MCK,
                    emacRstPins, PIO_LISTSIZE(emacRstPins),
                    emacPins, PIO_LISTSIZE(emacPins)))
  {
    printf("P: PHY Initialize ERROR!\n\r");
    return -1;
  }
  
  /* Auto Negotiate, work in RMII mode */
  if (!MACB_AutoNegotiate(pMacb, 1))
  {
    printf("P: Auto Negotiate ERROR!\n\r");
    return -1;
  }
  
  while( MACB_GetLinkSpeed(pMacb, 1) == 0 )
  {
    errCount++;
  }
  NetNIC_ConnStatus = DEF_YES;
  return 0;
}

void  NET_EMAC_TxPktPrepare (void        *ppkt,
                             uint16_t     size,
                             NET_ERR     *perr)
{  
  Mem_Copy((void *)gEmacd.pTxD[gEmacd.wTxHead].addr, ppkt, size);
  *perr = NET_NIC_ERR_NONE;
}

void NET_EMAC_IntInit(pHandler pISR_Handler)
{
  /* Setup EMAC buffers and interrupts */
  IRQ_ConfigureIT(ID_EMAC, (0x2 << 5), pISR_Handler);
  IRQ_EnableIT(ID_EMAC);
}

void NET_EMAC_IntEn(void)
{
  EMAC_EnableIt(gEmacd.pHw, EMAC_IER_RCOMP | EMAC_IER_ROVR | EMAC_IER_TCOMP);
  //*AT91C_EMACB_IER = AT91C_EMAC_RCOMP | AT91C_EMAC_ROVR | AT91C_EMAC_TCOMP;
}

uint8_t NET_EMAC_Poll(void *ppkt)
{
  uint32_t frmSize = 0;
  uint8_t status = EMACD_Poll(&gEmacd, ppkt, 1518/*EMAC_FRAME_LENTGH_MAX*/, &frmSize);
  if(EMACD_OK == status)
  {
    return frmSize;
  }
  return 0;
  //return EMACD_Poll(&gEmacd, ppkt, EMAC_FRAME_LENTGH_MAX, &frmSize);
}
