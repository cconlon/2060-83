/*
*********************************************************************************************************
*                                              uC/CSP
*                                        Chip Support Package
*
*                            (c) Copyright 2010; Micrium, Inc.; Weston, FL
*                         (c) Copyright 2003-2010; Micrium, Inc.; Weston, FL
*
*               All rights reserved. Protected by international copyright laws.
*
*               uC/CSP is provided in source form to registered licensees ONLY.  It is
*               illegal to distribute this source code to any third party unless you receive
*               written permission by an authorized Micrium representative.  Knowledge of
*               the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      FAMILY DEFINITIONS & API.
*                                         ATMEL AT91SAM9G20
*
*
* Filename      : csp_grp.h
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
* Note(s)       : (1) The following products are supported by this family definition.
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  CSP_GRP_MODULE_PRESENT
#define  CSP_GRP_MODULE_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/


#include <csp_types.h>

/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   CSP_GRP_MODULE
#define  CSP_GRP_EXT
#else
#define  CSP_GRP_EXT  extern
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




/*
*********************************************************************************************************
*                                           FAMILY DEFINITION
*********************************************************************************************************
*/

#define  CSP_GRP_AT91SAM9X35
#define  CSP_GRP_NAME                     "AT91SAM9X35"


/*
*********************************************************************************************************
*                               AT91SAM9G20 INTERRUPT CONTROLLER DEVICE NUMBERS
*
* Note(s): (1)  The AT91SAM9G20 uses the Advance Interrupt Controller (AIC). 
*               The AIC driver the nFIQ (Fast interrupt controller) and the nIRQ (standard interrupt request) inputs
*               of an ARM processor. 
* 
*          (2) The system Interrupt ion source 1 is wired-OR of the interrupt signals comming from:
*
*                  - The DDR2/LPDDR controller.
*                  - The Debug Unit.
*                  - The Periodic Interval Timer.
*                  - The Real-Time Timer.
*                  - The Real-Time Clock.
*                  - The Watchdog Timer.
*                  - The Power Management Controller.
*
*
*                         
*                                +-------------+                                       +-------------+
*                                |             |                        src00 -------->|             |
*                 src00 -------->|  Interrupt  |                        src01 -------->|  Interrupt  |
*                 src01 -------->|  Controller |                        src02 -------->|  Controller |
*                 src02 -------->|             |                                   .   |             |
*                 src03 -------->|    (AIC)    |                                   .   |    (AIC)    |
*                            .   |             |  ====                             .   |             |  
*                            .   |             |  ====                  srcn --------->|             |  
*                            .   |             |                                    sys|             |
*                 srcn --------->|             |                                    |->|             |
*                            sys |             |                                    |  +-------------+
*                   +-----+  --->|             |                    +------------+  |
*      sys_src0 --->|     |  |   +-------------|       sys_src0 --->|            |  |
*      sys_src1 --->|     |  |                         sys_src1 --->|            | 
*      sys_src2 --->|     |  |                         sys_src2 --->| Software   |  |
*          .        | OR  |--|                              .       | Interrupt  |--|
*          .        |     |                                 .       | controller |
*          .        |     |                                 .       |            |
*      sys_srcn --->|     |                            sys_srcn --->|            |
*                   +-----+                                         +------------+
*   
*           
*                For example two independent ISR can be installed for the Debug Unit and Periodic interval timer.
*
*                CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_SYS,
*                               (CSP_DEV_NBR   )CSP_INT_USB_SRC_NBR_DBG_UNIT_00,
*                               (CPU_FNCT_PTR  )DbgUnit_Handler,
*                               (CPU_VOID     *)0);
*
*                CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_SYS,
*                               (CSP_DEV_NBR   )CSP_INT_USB_SRC_NBR_PIV,
*                               (CPU_FNCT_PTR  )PIV_Handler,
*                               (CPU_VOID     *)0);
*********************************************************************************************************
*/

#define  CSP_INT_CTRL_NBR_MAIN                  (CSP_DEV_NBR)(0u)    /* NVIC    External Interrupt Controller.           */
#define  CSP_INT_CTRL_NBR_SYS                   (CSP_DEV_NBR)(1u)    /* System  Shared   Interrupt Controller.           */


/*
*********************************************************************************************************
*                                    AT91SAM910 AIC INTERRUPTS SOURCES
*********************************************************************************************************
*/

#define  CSP_INT_SRC_NBR_EXT_FIQ                (CSP_DEV_NBR)( 0u)    /* External FIQ Interrupt.                          */
#define  CSP_INT_SRC_NBR_SYS                    (CSP_DEV_NBR)( 1u)    /* System Interrupt.                                */
#define  CSP_INT_SRC_NBR_GPIO_A                 (CSP_DEV_NBR)( 2u)    /* Parallel I/O Controller A.                       */
#define  CSP_INT_SRC_NBR_GPIO_B                 (CSP_DEV_NBR)( 3u)    /* Parallel I/O Controller B.                       */
#define  CSP_INT_SRC_NBR_GPIO_C                 (CSP_DEV_NBR)( 4u)    /* Parallel I/O Controller C.                       */
#define  CSP_INT_SRC_NBR_GPIO_D                 (CSP_DEV_NBR)( 5u)    /* Parallel I/O Controller D.                       */
#define  CSP_INT_SRC_NBR_GPIO_E                 (CSP_DEV_NBR)( 5u)    /* Parallel I/O Controller E.                       */
#define  CSP_INT_SRC_NBR_RNG_00                 (CSP_DEV_NBR)( 6u)    /* True Random Number Genrator.                     */
#define  CSP_INT_SRC_NBR_UART_00                (CSP_DEV_NBR)( 7u)    /* USART 0.                                         */
#define  CSP_INT_SRC_NBR_UART_01                (CSP_DEV_NBR)( 8u)    /* USART 1.                                         */
#define  CSP_INT_SRC_NBR_UART_02                (CSP_DEV_NBR)( 9u)    /* USART 2.                                         */
#define  CSP_INT_SRC_NBR_UART_03                (CSP_DEV_NBR)(10u)    /* USART 3.                                         */
#define  CSP_INT_SRC_NBR_MCI_00                 (CSP_DEV_NBR)(11u)    /* High Speed Multimedia Card Interface 0.          */
#define  CSP_INT_SRC_NBR_TWI_00                 (CSP_DEV_NBR)(12u)    /* Two-Wire Interface 0.                            */
#define  CSP_INT_SRC_NBR_TWI_01                 (CSP_DEV_NBR)(13u)    /* Two-Wire Interface 1.                            */
#define  CSP_INT_SRC_NBR_SPI_00                 (CSP_DEV_NBR)(14u)    /* Serial Peripheral Interface 0.                   */
#define  CSP_INT_SRC_NBR_SPI_01                 (CSP_DEV_NBR)(15u)    /* Serial Peripheral Interface 1.                   */
#define  CSP_INT_SRC_NBR_SSC_00                 (CSP_DEV_NBR)(16u)    /* Synchronous Serial Controller 0.                 */
#define  CSP_INT_SRC_NBR_SSC_01                 (CSP_DEV_NBR)(17u)    /* Synchronous Serial Controller 1.                 */
#define  CSP_INT_SRC_NBR_TMR_xx                 (CSP_DEV_NBR)(18u)    /* Timer Counter 0, 1, 2, 3, 4, 5                   */
#define  CSP_INT_SRC_NBR_PWM_00                 (CSP_DEV_NBR)(19u)    /* Pulse width modulation controller.               */
#define  CSP_INT_SRC_NBR_TSC_00                 (CSP_DEV_NBR)(20u)    /* Touch Screen ADC Controller.                     */
#define  CSP_INT_SRC_NBR_DMA_00                 (CSP_DEV_NBR)(21u)    /* DMA Controller.                                  */
#define  CSP_INT_SRC_NBR_USB_HOST_00            (CSP_DEV_NBR)(22u)    /* USB host high speed.                             */
#define  CSP_INT_SRC_NBR_LCD_00                 (CSP_DEV_NBR)(23u)    /* USB host high speed.                             */
#define  CSP_INT_SRC_NBR_AC97_00                (CSP_DEV_NBR)(24u)    /* AC97 Controller.                                 */
#define  CSP_INT_SRC_NBR_ETHER_00               (CSP_DEV_NBR)(25u)    /* Ethernet MAC.                                    */
#define  CSP_INT_SRC_NBR_ISI_00                 (CSP_DEV_NBR)(26u)    /* Image Sensor Interface.                          */
#define  CSP_INT_SRC_NBR_USB_DEV_00             (CSP_DEV_NBR)(27u)    /* USB Device port.                                 */
#define  CSP_INT_SRC_NBR_MCI_01                 (CSP_DEV_NBR)(29u)    /* High Speed Multimedia Card Interface 1.          */
#define  CSP_INT_SRC_NBR_VDEC_00                (CSP_DEV_NBR)(30u)    /* Video Decoder.                                   */
#define  CSP_INT_SRC_NBR_IRQ                    (CSP_DEV_NBR)(31u)    /* External interrupt IRQ                           */
#define  CSP_INT_SRC_NBR_MAX                    (CSP_DEV_NBR)(32u)    /* Total number of interrupt sources.               */


/*
*********************************************************************************************************
*                                       AT91SAM910 AIC INTERRUPTS SOURCES
*********************************************************************************************************
*/

#define  CSP_INT_SYS_SRC_NBR_DDR2               (CSP_DEV_NBR)(0u)
#define  CSP_INT_SYS_SRC_NBR_DBG                (CSP_DEV_NBR)(1u)
#define  CSP_INT_SYS_SRC_NBR_PIT                (CSP_DEV_NBR)(2u)
#define  CSP_INT_SYS_SRC_NBR_RTT                (CSP_DEV_NBR)(3u)
#define  CSP_INT_SYS_SRC_NBR_RTC                (CSP_DEV_NBR)(4u)
#define  CSP_INT_SYS_SRC_NBR_WDT                (CSP_DEV_NBR)(5u)
#define  CSP_INT_SYS_SRC_NBR_RST_CTRL           (CSP_DEV_NBR)(6u)
#define  CSP_INT_SYS_SRC_NBR_PMC                (CSP_DEV_NBR)(7u)
#define  CSP_INT_SYS_SRC_NBR_MAX                (CSP_DEV_NBR)(8u)


/*
*********************************************************************************************************
*                                   POWER MANAGMENT PERIPHERALS CLOCK NUMBER
*********************************************************************************************************
*/


#define  CSP_PM_PER_CLK_NBR_RSVD_00                (CSP_DEV_NBR)( 0u)    /* FIQ Interrupt.                                   */
#define  CSP_PM_PER_CLK_NBR_SYS                    (CSP_DEV_NBR)( 1u)    /* System Interrupt.                                */
#define  CSP_PM_PER_CLK_NBR_GPIO_A                 (CSP_DEV_NBR)( 2u)    /* Parallel I/O Controller A.                       */
#define  CSP_PM_PER_CLK_NBR_GPIO_B                 (CSP_DEV_NBR)( 2u)    /* Parallel I/O Controller B.                       */
#define  CSP_PM_PER_CLK_NBR_GPIO_C                 (CSP_DEV_NBR)( 3u)    /* Parallel I/O Controller C.                       */
#define  CSP_PM_PER_CLK_NBR_GPIO_D                 (CSP_DEV_NBR)( 3u)    /* Parallel I/O Controller D.                       */
//#define  CSP_PM_PER_CLK_NBR_GPIO_E                 (CSP_DEV_NBR)( 5u)    /* Parallel I/O Controller E.                       */
//#define  CSP_PM_PER_CLK_NBR_RNG_00                 (CSP_DEV_NBR)( 6u)    /* True Random Number Genrator.                     */
#define  CSP_PM_PER_CLK_NBR_UART_00                (CSP_DEV_NBR)( 5u)    /* USART 0.                                         */
#define  CSP_PM_PER_CLK_NBR_UART_01                (CSP_DEV_NBR)( 6u)    /* USART 1.                                         */
#define  CSP_PM_PER_CLK_NBR_UART_02                (CSP_DEV_NBR)( 7u)    /* USART 2.                                         */
//#define  CSP_PM_PER_CLK_NBR_UART_03                (CSP_DEV_NBR)(10u)    /* USART 3.                                         */
//#define  CSP_PM_PER_CLK_NBR_MCI_00                 (CSP_DEV_NBR)(11u)    /* High Speed Multimedia Card Interface 0.          */
#define  CSP_PM_PER_CLK_NBR_TWI_00                 (CSP_DEV_NBR)(09u)    /* Two-Wire Interface 0                             */
#define  CSP_PM_PER_CLK_NBR_TWI_01                 (CSP_DEV_NBR)(10u)    /* Two-Wire Interface 1                             */
#define  CSP_PM_PER_CLK_NBR_TWI_02                 (CSP_DEV_NBR)(11u)    /* Two-Wire Interface 2                             */

#define  CSP_PM_PER_CLK_NBR_SPI_00                 (CSP_DEV_NBR)(14u)    /* Serial Peripheral Interface 0.                   */
#define  CSP_PM_PER_CLK_NBR_SPI_01                 (CSP_DEV_NBR)(15u)    /* Serial Peripheral Interface 1.                   */
#define  CSP_PM_PER_CLK_NBR_SSC_00                 (CSP_DEV_NBR)(16u)    /* Synchronous Serial Controller 0.                 */
//#define  CSP_PM_PER_CLK_NBR_SSC_01                 (CSP_DEV_NBR)(17u)    /* Synchronous Serial Controller 1.                 */
//#define  CSP_PM_PER_CLK_NBR_TMR_xx                 (CSP_DEV_NBR)(18u)    /* Timer Counter 0, 1, 2, 3, 4, 5                   */
#define  CSP_PM_PER_CLK_NBR_TMR_00                 (CSP_DEV_NBR)(17u)

//#define  CSP_PM_PER_CLK_NBR_PWM_00                 (CSP_DEV_NBR)(19u)    /* Pulse width modulation controller.               */
//#define  CSP_PM_PER_CLK_NBR_TSC_00                 (CSP_DEV_NBR)(20u)    /* Touch Screen ADC Controller.                     */
//#define  CSP_PM_PER_CLK_NBR_DMA_00                 (CSP_DEV_NBR)(21u)    /* DMA Controller.                                  */
//#define  CSP_PM_PER_CLK_NBR_USB_HOST_00            (CSP_DEV_NBR)(22u)    /* USB host high speed.                             */
//#define  CSP_PM_PER_CLK_NBR_LCD_00                 (CSP_DEV_NBR)(23u)    /* USB host high speed.                             */
//#define  CSP_PM_PER_CLK_NBR_AC97_00                (CSP_DEV_NBR)(24u)    /* AC97 Controller.                                 */
//#define  CSP_PM_PER_CLK_NBR_ETHER_00               (CSP_DEV_NBR)(25u)    /* Ethernet MAC.                                    */
//#define  CSP_PM_PER_CLK_NBR_ISI_00                 (CSP_DEV_NBR)(26u)    /* Image Sensor Interface                           */
//#define  CSP_PM_PER_CLK_NBR_USB_DEV_00             (CSP_DEV_NBR)(27u)    /* Image Sensor Interface                           */
//#define  CSP_PM_PER_CLK_NBR_MCI_01                 (CSP_DEV_NBR)(29u)    /* High Speed Multimedia Card Interface 1.          */
#define  CSP_PM_PER_CLK_NBR_MAX_NBR                (CSP_DEV_NBR)(32u)    /* Total number of interrupt sources                */


/*
*********************************************************************************************************
*                                      POWER MANAGMENT SYSTEM CLOCK NUMBER
*********************************************************************************************************
*/
                                                                         
#define  CSP_PM_SYS_CLK_NBR_PCK_00                 (CSP_DEV_NBR)( 0u)                                                                             
#define  CSP_PM_SYS_CLK_NBR_PCK_01                 (CSP_DEV_NBR)( 1u)    
#define  CSP_PM_SYS_CLK_NBR_DDRCK_00               (CSP_DEV_NBR)( 2u)
#define  CSP_PM_SYS_CLK_NBR_SYSCLKDDR_00           (CSP_DEV_NBR)( 3u)
#define  CSP_PM_SYS_CLK_NBR_UPLL_00                (CSP_DEV_NBR)( 4u)
#define  CSP_PM_SYS_CLK_NBR_UHP48_00               (CSP_DEV_NBR)( 5u)
#define  CSP_PM_SYS_CLK_NBR_UHP12_00               (CSP_DEV_NBR)( 6u)
#define  CSP_PM_SYS_CLK_NBR_MCK_00                 (CSP_DEV_NBR)( 7u)

#define  CSP_PM_SYS_CLK_NBR_MAX                    (CSP_DEV_NBR)( 8u)


/*
*********************************************************************************************************
*                                            DATA TYPES
*********************************************************************************************************
*/

typedef  CPU_INT32U  CSP_GPIO_MSK;                              /* GPIO port size.                                      */
typedef  CPU_INT16U  CSP_TMR_VAL;                               /* Timer value size.                                    */

/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                               MCU's FAMILY DEPENDENT API (EXTENDED API)
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of CSP_GRP module include.                      */

