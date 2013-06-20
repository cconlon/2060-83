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
*
* Filename      : serial_drv_atmel.h
* Version       : V2.00
* Programmer(s) : FGK
*********************************************************************************************************
* Note(s)       : (1) With an appropriate BSP, this device driver will support the UARTs on the Atmel
*                     MCUs & MPUs, including :
*
*                         Atmel AT91SAM7S
*                         Atmel AT91SAM7SE
*                         Atmel AT91SAM7X
*                         Atmel AT91SAM9260
*                         Atmel AT91SAM9261
*                         Atmel AT91SAM9263
*                         Atmel AT91SAM9RL
*                         Atmel AT91SAM9M10
*                         Atmel AT91SAM9G45
*                         Atmel AT91SAM3U
*                         Atmel AT91SAM3S
*                         Atmel AT32UC3A
*                         Atmel AT32UC3B
*                         Atmel AT32AP7000
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               SERIAL_DRV_ATMEL present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef   SERIAL_DRV_ATMEL_PRESENT
#define   SERIAL_DRV_ATMEL_PRESENT


/*
*********************************************************************************************************
*                                      SERIAL DEVICE DRIVER API
*********************************************************************************************************
*/

extern  SERIAL_DRV_API  SerialDrv_Atmel_API;
extern  SERIAL_DRV_API  SerialDrv_Atmel_DBGU_API;


/*$PAGE*/
/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of SERIAL_DRV_ATMEL module include.              */
