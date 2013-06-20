/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                              (c) Copyright 2007; Micrium, Inc.; Weston, FL
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
*                                         BOARD SUPPORT PACKAGE
*                                              PERIPHERALS
*
*
* Filename      : bsp_per.h
* Version       : V2.00
* Programmer(s) : FT
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  BSP_PER_PRESENT
#define  BSP_PER_PRESENT


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/


#ifdef   BSP_PER_MODULE
#define  BSP_PER_EXT
#else
#define  BSP_PER_EXT  extern
#endif

/*
*********************************************************************************************************
*                                        DEFAULT CONFIGURATION
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

                                                                /* ----------------- PERIPHRALS ID's ---------------- */
                                                                /* --------- Peripheral Name -------- | -- Ext Int -- */
#define  BSP_PER_ID_AIC_FIQ                          0          /* Advance Interrupt Controller       | FIQ           */
#define  BSP_PER_ID_SYSC                             1          /* System Controller Interrupt        |               */
#define  BSP_PER_ID_PIOA                             2          /* Parallel I/O Controller A          |               */
#define  BSP_PER_ID_PIOB                             3          /* Parallel I/O Controller B          |               */
#define  BSP_PER_ID_PIOC                             4          /* Parallel I/O Controller C          |               */
#define  BSP_PER_ID_PIOD                             4          /* Parallel I/O Controller C          |               */
#define  BSP_PER_ID_PIOE                             4          /* Parallel I/O Controller C          |               */

#define  BSP_PER_ID_US0                              7          /* USART 0                            |               */
#define  BSP_PER_ID_US1                              6          /* USART 1                            |               */
#define  BSP_PER_ID_US2                              7          /* USART 3                            |               */
#define  BSP_PER_ID_MCI0                            10          /* Multimedia Card interface 0        |               */
#define  BSP_PER_ID_MCI1                            11          /* Multimedia Card interface 1        |               */
#define  BSP_PER_ID_CAN                             12          /* CAN controlelr                     |               */
#define  BSP_PER_ID_TWI                             13          /* Two Wire Interface                 |               */
#define  BSP_PER_ID_SPI0                            12          /* Serial Peripheral Interface 1      |               */
#define  BSP_PER_ID_SPI1                            14          /* Serial Peripheral Interface 2      |               */
#define  BSP_PER_ID_SSC0                            15          /* Synchronous Serial Controller 0    |               */
#define  BSP_PER_ID_SSC1                            16          /* Synchronous Serial Controller 1    |               */
#define  BSP_PER_ID_AC97C                           18          /* AC97 Controller                    |               */
#define  BSP_PER_ID_TC0                             19          /* Timer/Counter 0                    |               */
#define  BSP_PER_ID_TC1                             19          /* Timer/Counter 1                    |               */
#define  BSP_PER_ID_TC2                             19          /* Timer/counter 2                    |               */
#define  BSP_PER_ID_PWMC                            20          /* Pulse Width Modulation Controller  |               */
#define  BSP_PER_ID_EMAC                            21          /* Ethernet MAC                       |               */
#define  BSP_PER_ID_2DGE                            23          /* 2D Graphics Engine                 |               */
#define  BSP_PER_ID_UDP                             24          /* USB Device Port                    |               */
#define  BSP_PER_ID_ISI                             25          /* Image Sensor Interface             |               */
#define  BSP_PER_ID_LCDC                            26          /* LCD Controller                     |               */
#define  BSP_PER_ID_DMA                             27          /* DMA controller                     |               */

#define  BSP_PER_ID_UHP                             29          /* USB Host Port                      |               */
#define  BSP_PER_ID_AIC_IRQ0                        30          /* Advance Interrupt Controller       | IRQ0          */
#define  BSP_PER_ID_AIC_IRQ1                        31          /* Advance Interrupt Controller       | IRQ1          */

#define  BSP_PER_ID_MAX                             32

/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void         BSP_PerClkDis        (CPU_INT08U     per_id);
void         BSP_PerClkEn         (CPU_INT08U     per_id);
CPU_BOOLEAN  BSP_PerClkStatusGet  (CPU_INT08U     per_id);

/*
*********************************************************************************************************
*                                              ERROR CHECKING
*********************************************************************************************************
*/

            
/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of module include.                               */
