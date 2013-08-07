/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                     APPLICATION CONFIGURATION
*
*                                         ATMEL AT91SAM9G20
*                                               on the
*                                AT91SAM9G20-G45-EK DEVELOPMENT BOARD
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : JM
*********************************************************************************************************
*/

#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT


/*
*********************************************************************************************************
*                                       ADDITIONAL uC/MODULE ENABLES
*********************************************************************************************************
*/

#define  APP_CFG_SERIAL_EN                         DEF_DISABLED


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define NET_OS_CFG_IF_RX_TASK_PRIO                      28u
#define NET_OS_CFG_TMR_TASK_PRIO                        29u

#define SIMU_SEND_DATA_TASK_PRIO                         4u
#define APP_TASK0_TASK_PRIO                              5u
#define APP_CFG_TASK_START_PRIO                          6u

#define PICK_TASK_PRIO					 8u	
#define STATIC_TRANSFER_TASK_PRIO		 9u
#define DYNAMIC_TRANSFER_TASK_PRIO		10u
#define REALDATA_TRANSFER_TASK_PRIO		11u
#define REALVALUE_TRANSFER_TASK_PRIO	12u
#define TRAN_TRANSFER_TASK_PRIO			13u
#define COMMAND_ANALYS_TASK_PRIO		14u
#define RECEIVE_12DATA_TASK_PRIO		15u
#define REAL_TIME_SHOW_TASK_PRIO		16u

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*********************************************************************************************************
*/
#define STATIC_TRANSFER_TASK_SIZE                       1024u
#define DYNAMIC_TRANSFER_TASK_SIZE                  	2048u
#define SIMU_SEND_DATA_TASK_SIZE                        2048u

#define APP_TASK0_TASK_SIZE                            1024u
#define  APP_CFG_TASK_START_STK_SIZE                   1024u
#define  NET_OS_CFG_IF_RX_TASK_STK_SIZE                1024u
#define  NET_OS_CFG_TMR_TASK_STK_SIZE                  1024u

/*
*********************************************************************************************************
*                                               SERIAL
*
* Note(s) : (1) Configure SERIAL_CFG_MAX_NBR_IF to the number of interfaces (i.e., UARTs) that will be
*               present.
*
*           (2) Configure SERIAL_CFG_RD_BUF_EN to enable/disable read buffer functionality.  The serial
*               core stores received data in the read buffer until the user requests it, providing a
*               reliable guarantee against receive overrun.
*
*           (3) Configure SERIAL_CFG_WR_BUF_EN to enable/disable write buffer functionality.  The serial
*               core stores line driver transmit data in the write buffer while the serial interface is
*               transmitting application data.
*
*           (4) Configure SERIAL_CFG_ARG_CHK_EXT_EN to enable/disable extended argument checking
*               functionality.
*
*           (5) Configure SERIAL_CFG_TX_DESC_NBR to allow multiple transmit operations (i.e., Serial_Wr,
*               Serial_WrAsync) to be queued.
*********************************************************************************************************
*/

#define  SERIAL_CFG_MAX_NBR_IF                            2u    /* See Note #1.                                         */

#define  SERIAL_CFG_RD_BUF_EN                    DEF_ENABLED    /* See Note #2.                                         */

#define  SERIAL_CFG_WR_BUF_EN                    DEF_ENABLED    /* See Note #3.                                         */

#define  SERIAL_CFG_ARG_CHK_EXT_EN               DEF_ENABLED    /* See Note #4.                                         */

#define  SERIAL_CFG_TX_DESC_NBR                           1u    /* See Note #5.                                         */


/*
*********************************************************************************************************
*                                    uC/SERIAL APPLICATION CONFIGURATION
*********************************************************************************************************
*/

#define  APP_SERIAL_CFG_TRACE_EN                DEF_ENABLED
#define  APP_SERIAL_CFG_TRACE_PORT_NAME         "DBGU"


/*
*********************************************************************************************************
*                                    uC/OS-II ARM PORT CONFIGURATION
*********************************************************************************************************
*/

//#define  OS_CPU_FPU_EN                             DEF_DISABLED
//#define  OS_CPU_ARM_DCC_EN                         DEF_DISABLED

#define  uC_CFG_OPTIMIZE_ASM_EN                    DEF_ENABLED

/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                0
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                2
#endif

#include <cpu.h>
void  App_SerPrintf (CPU_CHAR  *p_fmt, ...);


#define  APP_CFG_TRACE_LEVEL             TRACE_LEVEL_INFO
#define  APP_CFG_TRACE                   App_SerPrintf

#define  BSP_CFG_TRACE_LEVEL             TRACE_LEVEL_INFO
#define  BSP_CFG_TRACE                   BSP_Ser_Printf               

#define  APP_TRACE_INFO(x)               ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_CFG_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)                ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_CFG_TRACE x) : (void)0)

#define  BSP_TRACE_INFO(x)               ((BSP_CFG_TRACE_LEVEL  >= TRACE_LEVEL_INFO) ? (void)(BSP_CFG_TRACE x) : (void)0)
#define  BSP_TRACE_DBG(x)                ((BSP_CFG_TRACE_LEVEL  >= TRACE_LEVEL_DBG)  ? (void)(BSP_CFG_TRACE x) : (void)0)

/*
*********************************************************************************************************
*                                                  NIC
*********************************************************************************************************
*/
#define  uC_TCPIP_MODULE                 DEF_ENABLED

#endif
