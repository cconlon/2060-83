/*
*********************************************************************************************************
*                                              uC/TCP-IP
*                                      The Embedded TCP/IP Suite
*
*                          (c) Copyright 2003-2005; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                NET BOARD SUPPORT PACKAGE (BSP) FUNCTIONS
*
*                               ATMEL AT91SAM9261 USING THE DAVICOM DM9000E
*
* Filename      : net_bsp.h
* Programmer(s) : EHS
* Version       : 1.0
* Date          : 01/25/2007
*********************************************************************************************************
*/

#ifndef __NET_BSP_H__
#define __NET_BSP_H__

#include  <includes.h>

/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef     NET_BSP_MODULE
#define    NET_BSP_EXT
#else
#define    NET_BSP_EXT  extern
#endif

/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  EMAC_CFG_MAC_ADDR_SEL     EMAC_MAC_ADDR_SEL_CFG            /* Configure MAC address set method :                       */
                                                                    /* EMAC_MAC_ADDR_SEL_EEPROM from NIC's EEPROM               */
                                                                    /* EMAC_MAC_ADDR_SEL_CFG    from application                */

#define  DM9000EP_IO_MODE           DM9000EP_IO_16_BITS             /* Configure DM9000EP bus width:                            */
                                                                    /*      DM9000EP_IO_16_BITS  or  DM9000EP_IO_8_BITS         */

#define  SMC_DATA08   (*(volatile CPU_INT08U *)0x30000004L)         /* Address indicating data  access (bit 2 set)              */
#define  SMC_INDX08   (*(volatile CPU_INT08U *)0x30000000L)         /* Address indicating index access (bit 2 clear)            */
#define  SMC_DATA16   (*(volatile CPU_INT16U *)0x30000004L)         /* Address indicating data  access (bit 2 set)              */
#define  SMC_INDX16   (*(volatile CPU_INT16U *)0x30000000L)         /* Address indicating index access (bit 2 clear)            */

/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              MACRO'S
*
* Note(s) : (1) When NET_NIC_CFG_RD_WR_SEL configured to NET_NIC_RD_WR_SEL_MACRO, implement NIC read/write
*               functionality with macro's.
*
*               (a) The developer MUST implement the NIC read/write macro functions with the correct number
*                   & type of arguments necessary to implement the NIC's read/write functionality.
*
*               (b) The developer SHOULD appropriately name the NIC read/write macro functions by appending
*                   the number of bits that the macro functions read/write to the end of the macro function
*                   name :
*
*                           NetNIC_Rd_xx()
*                           NetNIC_Wr_xx()
*
*                   For example, 16-bit read/write macro functions should be named :
*
*                           NetNIC_Rd_16()
*                           NetNIC_Wr_16()
*
*
*               See also 'net_bsp.c  NetNIC_Rd_xx()  Note #1'
*                      & 'net_bsp.c  NetNIC_Wr_xx()  Note #1'.
*********************************************************************************************************
*/

#if (NET_NIC_CFG_RD_WR_SEL != NET_NIC_RD_WR_SEL_FNCT)

#define  NetNIC_WrReg_8(reg, data)      {SMC_INDX16 = (reg);         \
                                         SMC_DATA16 = (data);        }

#define  NetNIC_WrIx_8(ix)              (SMC_INDX16 = (ix))
#define  NetNIC_RdData_16()             (SMC_DATA16)
#define  NetNIC_WrData_16(data)         (SMC_DATA16 = (data))
#define  NetNIC_RdData_8()              (SMC_DATA16)
#define  NetNIC_WrData_8(data)          (SMC_DATA16 = (data))

CPU_INT08U  NetNIC_RdReg_8(CPU_INT08U reg);

#endif

#define  NetBSP_IntEn()                  AT91C_BASE_AIC->AIC_IECR = (1 << AT91C_ID_PIOC)
#define  NetBSP_IntDis()                 AT91C_BASE_AIC->AIC_IDCR = (1 << AT91C_ID_PIOC)

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void         NetBSP_NIC_HW_Init(void);
void         NetBSP_DlyU(CPU_INT16U us);
void         NetBSP_DlyM(CPU_INT16U ms);

void         NetBSP_IntInit(void);
void         NetBSP_IntClr(void);

CPU_INT08U   NetBSP_OS_X_Init(void); 
CPU_INT08U   DM9000_Lock(void); 
CPU_INT08U   DM9000_Unlock(void); 

/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/

#endif
