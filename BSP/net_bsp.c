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

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define AT91C_DM9000_NWE_SETUP	        (1  << 0)		                /* Note: these timings are based on MCK, which is assumed   */
#define AT91C_DM9000_NWE_PULSE 	        (3  << 0)		                /* to be 96MHz                                              */
#define AT91C_DM9000_NWE_CYCLE 	        (4  << 0)		

#define AT91C_DM9000_NRD_SETUP	        (1  << 16)		    
#define AT91C_DM9000_NRD_PULSE	        (3  << 16)		
#define AT91C_DM9000_NRD_CYCLE	        (4  << 16)		
    
#define AT91C_DM9000_NCS_WR_SETUP	    (0  << 8)		
#define AT91C_DM9000_NCS_WR_PULSE	    (4  << 8)		
    
#define AT91C_DM9000_NCS_RD_SETUP	    (0  << 24)		
#define AT91C_DM9000_NCS_RD_PULSE	    (4  << 24)		  
  
#define AT91C_DM9000_TDF	            (1  << 16)		

/*
*********************************************************************************************************
*                                            PROTOTYPES
*********************************************************************************************************
*/

extern  void  NetNIC_ISR_Handler(void);

/*
*********************************************************************************************************
*                                            GLOBALS
*********************************************************************************************************
*/

static  OS_EVENT  *DM9000_LockSem;                                      /* Semaphore uses to provide exclusive access to the DM9000 */

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
    AT91C_BASE_SMC->SMC_SETUP2 = (AT91C_DM9000_NWE_SETUP              | /* Configure the SMC for use with the Davicom DM9000E       */ 
                                  AT91C_DM9000_NCS_WR_SETUP           | /* Consule the DM9000E datasheet for timing diagrams and    */
				                  AT91C_DM9000_NRD_SETUP              | /* see the AT91SAM9261 datasheet section 21.9.1 and below   */
                                  AT91C_DM9000_NCS_RD_SETUP);           /* for additional information on how to generate these      */
                                                                        /* timings should MCK run at a frequency other than 96MHz   */
    AT91C_BASE_SMC->SMC_PULSE2 = (AT91C_DM9000_NWE_PULSE              | 
                                  AT91C_DM9000_NCS_WR_PULSE           |   
                                  AT91C_DM9000_NRD_PULSE              |   
                                  AT91C_DM9000_NCS_RD_PULSE);
  
    AT91C_BASE_SMC->SMC_CYCLE2 = (AT91C_DM9000_NWE_CYCLE              | 
                                  AT91C_DM9000_NRD_CYCLE);
    
    AT91C_BASE_SMC->SMC_CTRL2  = (AT91C_SMC_READMODE                  | 
                                  AT91C_SMC_WRITEMODE                 | 
                                  AT91C_SMC_NWAITM_NWAIT_DISABLE      |
			                      AT91C_SMC_BAT_BYTE_WRITE            | 
                                  AT91C_SMC_DBW_WIDTH_SIXTEEN_BITS    | 
                                  AT91C_DM9000_TDF);
  
    AT91C_BASE_PMC->PMC_PCER   = (1 << AT91C_ID_SYS);                   /* Ensure clocks for the SMC are enabled                    */
    AT91C_BASE_PMC->PMC_PCER   = (1 << AT91C_ID_PIOC);                  /* Enable PIOC clock                                        */
    AT91C_BASE_SYS->PIOC_PER   = (1 << 10);                             /* Configure the reset signal (PC10) as a GPIO pin          */
    AT91C_BASE_SYS->PIOC_OER   = (1 << 10);                             /* Configure the reset signal as an output pin              */
    AT91C_BASE_SYS->PIOC_CODR  = (1 << 10);                             /* Set the pin to a logic low level (not reset)             */
}

/*
*********************************************************************************************************
*                            Initialize NIC interrupts in the AIC
*
* Description : This function configures the AIC for NIC interrupts.
*
* Arguments   : none.
*
* Returns     : none.
*
* Caller(s)   : DM9000EP_Init()
*
* Notes       : 1) NetNIC_ISR_Handler() would potential be called for any PIOC pin that generates
*                  an interrupt. If other PIOC pin interrupts are required, then a dispatch PIOC 
*                  interrupt handler will need to be created.
*********************************************************************************************************
*/

void  NetBSP_IntInit (void)
{
    volatile CPU_INT32U  dummy;
    
    
    AT91C_BASE_AIC->AIC_IDCR    = (1 << AT91C_ID_PIOC);                 /* Temporarily disable PIOC interrupts on the AIC level     */

    AT91C_BASE_PIOC->PIO_PER    =  AT91C_PIO_PC11;                      /* Enable GPIO control of pin PC11                          */
    AT91C_BASE_PIOC->PIO_IFER   =  AT91C_PIO_PC11;                      /* Enable input glitch filter on PC11                       */
    AT91C_BASE_PIOC->PIO_IER    =  AT91C_PIO_PC11;                      /* Enable PIOC interrupts                                   */
    dummy                       =  AT91C_BASE_PIOC->PIO_ISR;            /* Clear the status register flags by reading PIOC ISR      */
    
                                                                        /* Setup AIC interrupt controller                           */
    AT91C_BASE_AIC->AIC_SVR[AT91C_ID_PIOC] = (CPU_INT32U)NetNIC_ISR_Handler;
    AT91C_BASE_AIC->AIC_SMR[AT91C_ID_PIOC] =  AT91C_AIC_SRCTYPE_EXT_HIGH_LEVEL | AT91C_AIC_PRIOR_LOWEST;
   
    AT91C_BASE_AIC->AIC_ICCR    = (1 << AT91C_ID_PIOC);
    AT91C_BASE_AIC->AIC_IECR    = (1 << AT91C_ID_PIOC);                 /* Re-enable PIOC interrupts on the AIC level               */
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
    volatile CPU_INT32U  dummy;

    
    dummy                       =  AT91C_BASE_PIOC->PIO_ISR;            /* Clear the status register flags by reading PIOC ISR      */    
    AT91C_BASE_AIC->AIC_ICCR    = (1 << AT91C_ID_PIOC);                 /* Clear the AIC PIOC interrupt source                      */    
}

/*
*********************************************************************************************************
*                                   WRITE 8 BITS TO A NIC REGISTER
*
* Description : This function is called from functions in net_nic.c to write to a NIC register.
*
* Argument(s) : reg     is the register offset of the register to which data will be written.
*               data    is the data to write to the register.
*
* Returns     : none.
*********************************************************************************************************
*/

#if NET_NIC_CFG_RD_WR_SEL == NET_NIC_RD_WR_SEL_FNCT
void  NetNIC_WrReg_8 (CPU_INT08U reg, CPU_INT08U data)
{
    SMC_INDX16 = (CPU_INT16U)reg;
    SMC_DATA16 = (CPU_INT16U)data;
}
#endif

/*
*********************************************************************************************************
*                               READ 8 BITS FROM A NIC REGISTER
*
* Description : This function is called from functions in net_nic.c to read from the NIC
*
* Argument(s) : reg     is the register offset of the register from which data will be read.
*
* Returns     : The data from the register.
*********************************************************************************************************
*/

CPU_INT08U  NetNIC_RdReg_8 (CPU_INT08U reg)
{
    CPU_INT08U  rd_value;

    
    SMC_INDX16 = (CPU_INT16U)reg;
    rd_value   = (CPU_INT08U)SMC_DATA16;

    return (rd_value);
}

/*
*********************************************************************************************************
*                                   WRITE A REGISTER INDEX TO THE DM900A
*
* Description : This function is called from functions in net_nic.c to write to a NIC register.
*
* Argument(s) : ix     is the index which will be written to the data bus.
*
* Returns     : none.
*
* Note(s)     : (1) This function is used with NetNIC_RdData_16() and NetNIC_WrData_16() or
*                   NetNIC_RdData_8() and NetNIC_WrData_8() to perform multiple read/write accesses
*                   after a single command (i.e., index specification).  Basically, a call to this
*                   function (specifying the register which will be accessed) is followed by multiple
*                   calls to NetNIC_RdData_16() or NetNIC_WrData_16() (reading/writing data to the NIC).
*********************************************************************************************************
*/

#if NET_NIC_CFG_RD_WR_SEL == NET_NIC_RD_WR_SEL_FNCT
void  NetNIC_WrIx_8 (CPU_INT08U ix)
{
    SMC_INDX16 = (CPU_INT16U)ix;
}
#endif

/*
*********************************************************************************************************
*                                 READ DATA FROM THE DM9000E
*
* Description : This function is called from functions in net_nic.c to read from the DM9000E data bus.
*
* Argument(s) : none.
*
* Returns     : The data from the register.
*
* Note(s)     : see Note (1) for NetNIC_WrCmd_8
*********************************************************************************************************
*/

#if NET_NIC_CFG_RD_WR_SEL == NET_NIC_RD_WR_SEL_FNCT
CPU_INT16U  NetNIC_RdData_16 (void)
{
    CPU_INT16U  rd_value;


    rd_value   = SMC_DATA16;

    return (rd_value);
}
#endif

/*
*********************************************************************************************************
*                                 WRITE DATA TO THE DM9000E
*
* Description : This function is called from functions in net_nic.c to write to the DM9000E data bus.
*
* Argument(s) : data    is the data to write to the DM9000E.
*
* Returns     : none.
*
* Note(s)     : see Note (1) for NetNIC_WrCmd_8
*********************************************************************************************************
*/

#if NET_NIC_CFG_RD_WR_SEL == NET_NIC_RD_WR_SEL_FNCT
void  NetNIC_WrData_16 (CPU_INT16U data)
{
    SMC_DATA16 = data;
}
#endif

/*
*********************************************************************************************************
*                                 READ DATA FROM THE DM9000E
*
* Description : This function is called from functions in net_nic.c to read from the DM9000E data bus.
*
* Argument(s) : none.
*
* Returns     : The data from the register.
*
* Note(s)     : see Note (1) for NetNIC_WrCmd_8
*********************************************************************************************************
*/

#if NET_NIC_CFG_RD_WR_SEL == NET_NIC_RD_WR_SEL_FNCT
CPU_INT08U  NetNIC_RdData_8 (void)
{
    CPU_INT08U  rd_value;

    
    rd_value   = (CPU_INT08U)SMC_DATA16;

    return (rd_value);
}
#endif

/*
*********************************************************************************************************
*                                 WRITE DATA TO THE DM9000E
*
* Description : This function is called from functions in net_nic.c to read from the DM9000E data bus.
*
* Argument(s) : data    is the data to write to the DM9000E.
*
* Returns     : none.
*
* Note(s)     : see Note (1) for NetNIC_WrCmd_8
*********************************************************************************************************
*/

#if NET_NIC_CFG_RD_WR_SEL == NET_NIC_RD_WR_SEL_FNCT
void  NetNIC_WrData_8 (CPU_INT08U data)
{
    SMC_DATA16 = (CPU_INT16U)data;
}
#endif

/*
*********************************************************************************************************
*                                        NetBSP_OS_X_Init()
*
* Description : This function creates and initializes the DM9000 lock semaphore
*
* Arguments   : none
*
* Returns     : 0 for no error, 1 on error.
*********************************************************************************************************
*/

CPU_INT08U  NetBSP_OS_X_Init (void) 
{
    DM9000_LockSem  = OSSemCreate(1);                                   /* Create a semaphore, assume DM9000 is ready for access    */
    if (DM9000_Lock !=  NULL)
        return  (0);                                                    /* No error creating the semaphore                          */
    else {
        return  (1);                                                    /* An error occured while creating the semaphore            */
    }
}

/*
*********************************************************************************************************
*                                        DM9000_Lock()
*
* Description : This function attempts to acquire a semaphore. If the semaphore
*               is not available, it will block the calling thread. This is used
*               to provide exclusive access to the DM9000 internal registers and
*               databus to either user threads that transmit, or the receive task.
*
* Arguments   : none
*
* Returns     : uC/OS-II Semaphore Return Codes
*********************************************************************************************************
*/

CPU_INT08U  DM9000_Lock (void) 
{
    CPU_INT08U  err;
    
    
    OSSemPend(DM9000_LockSem, 0, &err);
    return (err);
}

/*
*********************************************************************************************************
*                                        DM9000_Unlock()
*
* Description : This function releases the DM9000 lock semaphore.
*
* Arguments   : none
*
* Returns     : uC/OS-II Semaphore Return Codes
*********************************************************************************************************
*/

CPU_INT08U  DM9000_Unlock (void) 
{
    CPU_INT08U  err;
    
    
    err  =  OSSemPost(DM9000_LockSem);
    return (err);
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
*                                            NetNIC_LinkUp()
*
* Description : Message from NIC that the ethernet link is up.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : WARNING: Called in interruption context most of the time.
*********************************************************************************************************
*/

void  NetNIC_LinkUp (void)
{
    NetNIC_ConnStatus = DEF_TRUE;
}

/*
*********************************************************************************************************
*                                           NetNIC_LinkDown()
*
* Description : Message from NIC that the ethernet link is down.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : WARNING: Called in interruption context most of the time.
*********************************************************************************************************
*/

void  NetNIC_LinkDown (void)
{
    NetNIC_ConnStatus = DEF_FALSE;
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
   NetTCP_TxSeqNbrCtr = 5000;
}
#endif

/*
*********************************************************************************************************
*                                     NetTCP_TxRTT_GetTS()
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
* Note(s)     : (2) (a) To avoid timestamp calculation overflow, a maximum clock tick threshold value
*                       MUST be configured to truncate all clock tick values.
*
*                   (b) Also, since the clock tick integer will periodically overflow; the maximum
*                       threshold MUST be a multiple of the clock tick to avoid a discontinuity in
*                       the timestamp calculation when the clock tick integer overflows.
*********************************************************************************************************
*/

#ifdef  NET_TCP_MODULE_PRESENT
NET_TCP_TX_RTT_TS_MS  NetTCP_TxRTT_GetTS (void)
{
#if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
    CPU_SR                cpu_sr;
#endif
    NET_TCP_TX_RTT_TS_MS  rtt_ts_ms;
    CPU_INT32U            clk_tick;
    CPU_INT32U            clk_tick_th;


    CPU_CRITICAL_ENTER();
    clk_tick = (CPU_INT32U)OSTime;
    CPU_CRITICAL_EXIT();

    if (OS_TICKS_PER_SEC > 0) {
        clk_tick_th  = (NET_TCP_TX_RTT_TS_MAX                   /* MUST cfg clk tick th ...                                 */
                     /  DEF_TIME_NBR_mS_PER_SEC);               /* ... as multiple of max ts (see Note #2b).                */
        clk_tick    %=  clk_tick_th;                            /* Truncate clk ticks to avoid ovf (see Note #2a).          */

        rtt_ts_ms    = (NET_TCP_TX_RTT_TS_MS)((clk_tick * DEF_TIME_NBR_mS_PER_SEC) / OS_TICKS_PER_SEC);

    } else {
        rtt_ts_ms    = (NET_TCP_TX_RTT_TS_MS)clk_tick;
    }


    return (rtt_ts_ms);
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
    NET_TCP_TX_RTT_TS_MS  rtt_ts_ms;


    /* $$$$ Insert code to return TCP RTT timestamp (see Note #1).  */

    rtt_ts_ms = NET_TCP_TX_RTT_TS_NONE;

    return (rtt_ts_ms);
}
#endif
