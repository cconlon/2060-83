/*
*********************************************************************************************************
*                                     SERIAL (BYTE) COMMUNICATION
*
*                         (c) Copyright 2007-2009; Micrium, Inc.; Weston, FL
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
*                                     SERIAL (BYTE) COMMUNICATION
*                                            DEVICE DRIVER
*
*                                                ATMEL
*                                      BSP FOR ATMEL AT91SAM9M10
*
* Filename      : serial_bsp_at91sam9m10.h
* Version       : V2.00
* Programmer(s) : FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               SERIAL_BSP_AT91SAM9M10 present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef   SERIAL_BSP_AT91SAM9M10_PRESENT
#define   SERIAL_BSP_AT91SAM9M10_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <csp.h>

#include  <serial.h>
#include  <serial_drv_atmel.h>


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           SERIAL DEVICES
*********************************************************************************************************
*/

extern  SERIAL_DEV_CFG  SerialDev_Atmel_Dbgu;
extern  SERIAL_DEV_CFG  SerialDev_Atmel_UART0;
extern  SERIAL_DEV_CFG  SerialDev_Atmel_UART1;
extern  SERIAL_DEV_CFG  SerialDev_Atmel_UART2;


/*$PAGE*/
/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/


#endif                                                          /* End of SERIAL_BSP_AT91SAM9M10 module include.        */
