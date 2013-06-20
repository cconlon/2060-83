/*
*********************************************************************************************************
*                                              uC/TCP-IP
*                                      The Embedded TCP/IP Suite
*
*                          (c) Copyright 2003-2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/TCP-IP is provided in source form for FREE evaluation, for educational
*               use or peaceful research.  If you plan on using uC/TCP-IP in a commercial
*               product you need to contact Micrium to properly license its use in your
*               product.  We provide ALL the source code for your convenience and to help
*               you experience uC/TCP-IP.  The fact that the source code is provided does
*               NOT mean that you can use it without paying a licensing fee.
*
*               Network Interface Card (NIC) port files provided, as is, for FREE and do
*               NOT require any additional licensing or licensing fee.
*
*               Knowledge of the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        NETWORK PHYSICAL LAYER
*
*                                               BCM5221
*
* Filename      : net_phy.h
* Version       : V1.91
* Programmer(s) : CL
*********************************************************************************************************
* Note(s)       : (1) Supports Broadcom BCM5221 Fast Ethernet Transceiver' as described in
*
*                         Broadcom Corporation (Broadcom; http://www.broadcom.com)
*                         (a) BCM5221          (10/100BASE-TX Single-Channel Signi-PHY™ Transceiver
*                                                   5221-DS08-R  02-17-2005;
*                                               http://www.broadcom.com/products/Enterprise-Small-Office/
*                                                   Fast-Ethernet-Transceivers/BCM5221)
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  BCM5221_INIT_AUTO_NEG_RETRIES                     3


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void         NetNIC_PhyInit          (NET_ERR *perr);

void         NetNIC_PhyAutoNeg       (void);                    /* Do link auto-negotiation                             */


                                                                /* ------------------- STATUS FNCTS ------------------- */
CPU_BOOLEAN  NetNIC_PhyAutoNegState  (void);                    /* Get PHY auto-negotiation state                       */

CPU_BOOLEAN  NetNIC_PhyLinkState     (void);                    /* Get PHY link state                                   */


void         NetNIC_PhyISR_Handler   (void);                    /* Decode & handle PHY ISRs.                            */


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                                    DEFINED IN PRODUCT'S  net_bsp.c
*********************************************************************************************************
*/

void         NetNIC_LinkUp           (void);                    /* Message from NIC that the ethernet link is up.       */
                                                                /* Called in interruption context most of the time.     */

void         NetNIC_LinkDown         (void);                    /* Message from NIC that the ethernet link is down.     */
                                                                /* Called in interruption context most of the time.     */


void         BCM5221_DlyReset        (void);                    /* Implement 50 ms dly. Function in net_bsp.c           */
                                                                /*   MAY  use OS dly to preempt or tmr loop.            */

void         BCM5221_DlyAutoNegAck   (void);                    /* Implement 1.5 s dly.  Function in net_bsp.c          */
                                                                /*   MUST use OS dly to preempt.                        */


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                                  DEFINED IN PRODUCT's  net_isr_a.s
*********************************************************************************************************
*/

void         NetNIC_PhyISR           (void);


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*                                    DEFINED IN PRODUCT'S  net_isr.c
*********************************************************************************************************
*/

#if (NET_NIC_CFG_INT_CTRL_EN == DEF_ENABLED)
void         NetNIC_PhyIntInit       (void);                    /* Init int ctrl'r.                                     */

void         NetNIC_PhyIntClr        (void);                    /* Clr  int ctrl'r src(s).                              */
#endif


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/

#ifndef  NET_NIC_CFG_INT_CTRL_EN
#error   NET_NIC_CFG_INT_CTRL_EN           not #define'd in 'net_cfg.h'
#error                               [MUST be  DEF_DISABLED]
#error                               [     ||  DEF_ENABLED ]
#elif  ((NET_NIC_CFG_INT_CTRL_EN != DEF_DISABLED) && \
        (NET_NIC_CFG_INT_CTRL_EN != DEF_ENABLED ))
#error   NET_NIC_CFG_INT_CTRL_EN     illegally #define'd in 'net_cfg.h'
#error                               [MUST be  DEF_DISABLED]
#error                               [     ||  DEF_ENABLED ]
#endif
