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
* Filename      : net_phy.c
* Version       : V1.91
* Programmer(s) : CL
*                 JDH
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
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <net.h>
#include  <net_phy_def.h>
#include  <net_phy.h>


/*
*********************************************************************************************************
*                                           NetNIC_PhyInit()
*
* Description : Initialize phyter (ethernet link controller).
*               This instance configure the Broadcom 5221 10/100BASE-TX Single-Channel Signi-PHY™ Transceiver.
*
* Argument(s) : p_net_err   error code.
*
* Return(s)   : None.
*
* Caller(s)   : NetNIC_Init.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  NetNIC_PhyInit (NET_ERR  *p_net_err)
{
    CPU_INT16U   phy_reg;


   *p_net_err = NET_PHY_ERR_NONE;
                                                                /* Register 0x00: Control register.                     */
                                                                /* Bit        15: Reset.                                */
                                                                /* Bit        12: Auto-negociation enable.              */
                                                                /* Bit         9: Restart auto-negotiation.             */
                                                                /* - Preserve bit 6-0 values -                          */
    phy_reg  = NetNIC_PhyRegRd(0, 0x00, p_net_err);
    phy_reg &= DEF_BIT_FIELD((6-0) + 1, 0);

    NetNIC_PhyRegWr(0, 0x00, DEF_BIT_15 | DEF_BIT_12 | DEF_BIT_09 | phy_reg, p_net_err);
    BCM5221_DlyReset();

                                                                /* Register 0x1A: Interrupt register.                   */
                                                                /*  Read clears pending interrupts.                     */
    NetNIC_PhyRegRd(0, 0x1A, p_net_err);

                                                                /* Register 0x1A: Interrupt register.                   */
                                                                /* Bit        14: Enable interrupts.                    */
                                                                /* - Preserve bit 7-4 values -                          */
    phy_reg  = NetNIC_PhyRegRd(0, 0x1A, p_net_err);
    phy_reg &= DEF_BIT_FIELD((7-4) + 1, 4);
    NetNIC_PhyRegWr(0, 0x1A, DEF_BIT_14 | phy_reg, p_net_err);

                                                                /* Register 0x1B: Auxilary mode 2 register.             */
                                                                /* Bit         2: ACTLED ON on link, pulses on traffic. */
                                                                /* - Preserve bit 15-12, 4, 3, 0 values -               */
    phy_reg  = NetNIC_PhyRegRd(0, 0x1B, p_net_err);
    phy_reg &= (DEF_BIT_FIELD((15-12) + 1, 12) | DEF_BIT_04 | DEF_BIT_03 | DEF_BIT_00);
    NetNIC_PhyRegWr(0, 0x1B, DEF_BIT_02 | phy_reg, p_net_err);

    NetNIC_PhyIntInit();                                        /* Register module interrupt.                           */
}


/*
*********************************************************************************************************
*                                          NetNIC_PhyAutoNeg()
*
* Description : Wait for link auto-negotiation.
*
* Argument(s) : None.
*
* Return(s)   : None.
*
* Caller(s)   : NetNIC_PhyISR_Handler.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  NetNIC_PhyAutoNeg (void)
{
    CPU_BOOLEAN  link_state;
    CPU_INT16U   i;


    i          = BCM5221_INIT_AUTO_NEG_RETRIES;
    link_state = NetNIC_PhyAutoNegState();

    while ((link_state != DEF_ON) && (i > 0)) {
        BCM5221_DlyAutoNegAck();
        link_state = NetNIC_PhyAutoNegState();
        i--;
    }
}


/*
*********************************************************************************************************
*                                       NetNIC_PhyAutoNegState()
*
* Description : Returns state of auto-negotiation.
*               This instance probe the Intel BCM5221 '3.3V Dual-Speed Fast Ethernet Transceiver'.
*
* Argument(s) : None.
*
* Return(s)   : State of auto-negociation (DEF_OFF = not completed, DEF_ON = completed).
*
* Caller(s)   : NetNIC_PhyAutoNeg.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  NetNIC_PhyAutoNegState (void)
{
    CPU_INT16U   phy_reg;
    NET_ERR      net_err;

                                                                /* Register 0x01: Status register.                      */
                                                                /* Bit         5: Auto-negotiation complete.            */
    phy_reg = NetNIC_PhyRegRd(0, 0x01, &net_err);
    return (DEF_BIT_IS_SET(phy_reg, DEF_BIT_05));
}


/*
*********************************************************************************************************
*                                         NetNIC_PhyLinkState()
*
* Description : Returns state of ethernet link.
*               This instance probe the Intel BCM5221 '3.3V Dual-Speed Fast Ethernet Transceiver'.
*
* Argument(s) : None.
*
* Return(s)   : State of ethernet link (DEF_OFF = link down, DEF_ON = link up).
*
* Caller(s)   : NetNIC_PhyISR_Handler.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  NetNIC_PhyLinkState (void)
{
    CPU_INT16U   phy_reg;
    NET_ERR      net_err;

                                                                /* Register 0x01: Status register.                      */
                                                                /* Bit         2: Link status.                          */
    phy_reg = NetNIC_PhyRegRd(0, 0x01, &net_err);
    return (DEF_BIT_IS_SET(phy_reg, DEF_BIT_02));
}


/*
*********************************************************************************************************
*                                        NetNIC_PhyISR_Handler()
*
* Description : (1) Update NetNIC_ConnStatus according to link state.
*
* Argument(s) : None.
*
* Return(s)   : None.
*
* Caller(s)   : Generic interrupt handler.
*
* Note(s)     : (2) BCM5221 ISR  MUST call NetNIC_PhyISR_Handler() & MUST be developer-implemented in
*
*                       \<Your Product Application>\net_isr.c
*
*                           where
*                                   <Your Product Application>    directory path for Your Product's Application
*
*               (3) This function clears the interrupt source(s) on an external interrupt controller &, if
*                   ENABLED, MUST be developer-implemented in
*
*                       \<Your Product Application>\net_isr.c
*
*                           where
*                                   <Your Product Application>    directory path for Your Product's Application
*********************************************************************************************************
*/

void  NetNIC_PhyISR_Handler (void)
{
    CPU_INT16U   phy_reg;
    NET_ERR      net_err;

                                                                /* Register 0x1A: Interrupt register.                   */
                                                                /*  Read clears pending interrupts.                     */
    phy_reg = NetNIC_PhyRegRd(0, 0x1A, &net_err);
    if (DEF_BIT_IS_SET(phy_reg, DEF_BIT_00)) {                  /* Test if interrupt occured in this module.            */
        NetNIC_PhyAutoNeg();                                    /* Wait for auto-negotiation.                           */
        NetNIC_ConnStatus = NetNIC_PhyLinkState();              /* Set NetNIC_ConnStatus according to link state.       */
        if (NetNIC_ConnStatus == DEF_ON) {                      /* Call BSP feedback functions.                         */
            NetNIC_LinkUp();
        } else {
            NetNIC_LinkDown();
        }
    }

    NetNIC_PhyIntClr();
}
