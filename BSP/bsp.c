/*
*********************************************************************************************************
*                                     MICRIUM BOARD SUPPORT SUPPORT
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
*                                      BOARD SUPPORT PACKAGE (BSP)
*
*                                         ATMEL AT91SAM9G20
*                                               on the
*                                AT91SAM9G20-EK DEVELOPMENT BOARD
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : FT
*                 JM
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define   BSP_MODULE

#include  <bsp.h>
#include  <os_cpu.h>

#include "board.h"
/*
*********************************************************************************************************
*                                             REGISTER & BIT DEFINES
*********************************************************************************************************
*/

#define  BSP_PMC_MOR                            (*(CPU_REG32 *)(0xFFFFFC20))
#define  BSP_PMC_SR                             (*(CPU_REG32 *)(0xFFFFFC68))
#define  BSP_PMC_PLLA                           (*(CPU_REG32 *)(0xFFFFFC28))
#define  BSP_PMC_MCKR                           (*(CPU_REG32 *)(0xFFFFFC30))
#define  BSP_WDTC_WDMR                          (*(CPU_REG32 *)(0xFFFFFE44))
#define  BSP_MATRIX_MRCR                        (*(CPU_REG32 *)(0xFFFFEE00))


/*
*********************************************************************************************************
*                                             GPIO DEFINES
*********************************************************************************************************
*/


#define  BSP_GPIO_PORTB_JOY_UP                  DEF_BIT_16
#define  BSP_GPIO_PORTB_JOY_DOWN                DEF_BIT_17
#define  BSP_GPIO_PORTB_JOY_LEFT                DEF_BIT_14
#define  BSP_GPIO_PORTB_JOY_RIGHT               DEF_BIT_15
#define  BSP_GPIO_PORTB_JOY_CENTER              DEF_BIT_18

#define  BSP_GPIO_PORTB_JOY_GRP                (BSP_GPIO_PORTB_JOY_UP     | \
                                                BSP_GPIO_PORTB_JOY_DOWN   | \
                                                BSP_GPIO_PORTB_JOY_LEFT   | \
                                                BSP_GPIO_PORTB_JOY_RIGHT  | \
                                                BSP_GPIO_PORTB_JOY_CENTER)

#define  BSP_GPIO_PORTA_PB_00                   DEF_BIT_30
#define  BSP_GPIO_PORTA_PB_01                   DEF_BIT_31
#define  BSP_GPIO_PORTA_PB_GRP                 (BSP_GPIO_PORTA_PB_00 | \
                                                BSP_GPIO_PORTA_PB_01)


/*
*********************************************************************************************************
*                                             BSP CONSTANS VALUES
*********************************************************************************************************
*/
                                                                /* Maximum register timeout                            */
#define  BSP_VAL_MAX_TO                         (CPU_INT16U)(0xFFFF)


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             BSP_LowLevelInit()
*
* Description : System Low Level Init (PLLs, External Memories, Boot loaders, etc).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Startup Code.
* 
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LowLevelInit (void)
{

}


/*
*********************************************************************************************************
*                                             BSP_PreInit()
*
* Description : System Pre-Initialization. Initializes all peripherals that don't require OS services (LEDs, PBs)
*               or modules than need to be initialized before the OS (External memories).
*
* Argument(s) : main().
*
* Return(s)   : none.
*
* Note(s)     : (1) Clocks initialization should be ONLY perform when code is NOT executed from DDRAM.
*
*               (2) This funtions configure the clocks as:
*                       
*                       (a) CPU clock    @ 400Mhz.
*                       (b) Master clock @ 133Mhz.
*********************************************************************************************************
*/

void  BSP_PreInit (void)
{
#ifndef APP_CFG_DDRAM_EN
    CPU_INT32U  reg_to;
    CPU_INT32U  reg_val;
#endif
    
    
#ifndef APP_CFG_DDRAM_EN                                        /* See note #1.                                         */
    BSP_PMC_MOR = DEF_BIT_00                                    /* Enable main oscillator.                              */
                | (64u << 8u);
     
    reg_to  = BSP_VAL_MAX_TO;
    reg_val = BSP_PMC_SR;    
    while ((reg_to > 0u) &&
           (DEF_BIT_IS_CLR(reg_val, DEF_BIT_00))) {
        reg_to--;
        reg_val = BSP_PMC_SR;
    }

    if (reg_to == 0u) {
        return;
    }
                                                                /* Initialize PLLA  for 800 Mhz.                        */
    BSP_PMC_PLLA =  DEF_BIT_29                                  /* ... Bit 29 must always set to 1 CKGR_PLLAR           */
                 |  0x3F       << 8u                            /* ... Set the PLLA counter.                            */
                 | (200u - 1u) << 16u                           /* ... Set the PLL multiplier  = 200                    */
                 |  3u;                                         /* ... Set the PLL divider     = 3                      */
    
    reg_to  = BSP_VAL_MAX_TO;                                   /* ... Wait for the PLL setup.                          */
    reg_val = BSP_PMC_SR;    
    while ((reg_to > 0u) &&
           (DEF_BIT_IS_CLR(reg_val, DEF_BIT_01))) {
        reg_to--;
        reg_val = BSP_PMC_SR;
    }

    if (reg_to == 0u) {
        return;
    }

    reg_to  = BSP_VAL_MAX_TO;                                   /* Wait for master clock.                                */
    reg_val = BSP_PMC_SR;    
    while ((reg_to > 0u) &&
           (DEF_BIT_IS_CLR(reg_val, DEF_BIT_03))) {
        reg_to--;
        reg_val = BSP_PMC_SR;
    }

    if (reg_to == 0u) {
        return;
    }    
    
                                                                /* Switch to master clock.                              */
    BSP_PMC_MCKR = (3u  << 8u)                                  /* ... Mclk    = CPUclk  / 3                            */
                 | DEF_BIT_12;                                  /* ... PLLDIV2 = PLLA / 2                               */

    reg_to  = BSP_VAL_MAX_TO;                                   /* Wait for master clock.                               */
    reg_val = BSP_PMC_SR;    
    while ((reg_to > 0u) &&
           (DEF_BIT_IS_CLR(reg_val, DEF_BIT_03))) {
        reg_to--;
        reg_val = BSP_PMC_SR;
    }

    if (reg_to == 0u) {
        return;
    }    

    BSP_PMC_MCKR |= 2u;
    
    
    reg_to  = BSP_VAL_MAX_TO;                                   /* Wait for master clock.                               */
    reg_val = BSP_PMC_SR;    
    while ((reg_to > 0u) &&
           (DEF_BIT_IS_CLR(reg_val, DEF_BIT_03))) {
        reg_to--;
        reg_val = BSP_PMC_SR;
    }

    if (reg_to == 0u) {
        return;
    }    
#endif
    
#if 0
    BSP_WDTC_WDMR   = DEF_BIT_15;                               /* Disable Watchdog timer.                              */
    BSP_MATRIX_MRCR = DEF_BIT_00                                /* Remamp Internal RAM.                                 */
                    | DEF_BIT_01;
#endif    
    BOARD_RemapRam();
    OS_CPU_InitExceptVect(); 
    CSP_IntInit();                                              /* Initialize interrupt controller module.              */
}


/*
*********************************************************************************************************
*                                             BSP_PostInit()
*
* Description : Initialize all the peripherals that required OS services (OS initialized)
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/


void  BSP_PostInit (void)
{   
    //CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_GPIO_A);
    //CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_GPIO_B);
    //CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_GPIO_D);
    //PMC_EnablePeripheral(ID_PIOA);
    //PMC_EnablePeripheral(ID_PIOB);
    //PMC_EnablePeripheral(ID_PIOD);
    LED_Configure(0);
    LED_Configure(1);

#if 0 
                                                                /* ------------------ LEDs GPIO INIT ------------------ */
    
    CSP_GPIO_Cfg(CSP_GPIO_PORT_NBR_B,         
                 BSP_GPIO_PORTB_LED_GRP,
                 CSP_GPIO_DIR_OUT,
                 CSP_GPIO_FLAG_MODE_NONE,
                 DEF_NO,
                 DEF_BIT_NONE,
                 CSP_GPIO_FNCT_00);
                                                           /* ------------------- PBs GPIO INIT ------------------ */
    CSP_GPIO_Cfg(CSP_GPIO_PORT_NBR_A,
                 BSP_GPIO_PORTA_PB_GRP,
                 CSP_GPIO_DIR_IN,
                 CSP_GPIO_FLAG_MODE_NONE,
                 DEF_NO,
                 DEF_BIT_NONE,
                 CSP_GPIO_FNCT_00);
   
                                                                /* ---------------- JOYSTICK GPIO INIT ---------------- */
    CSP_GPIO_Cfg(CSP_GPIO_PORT_NBR_B,
                 BSP_GPIO_PORTB_JOY_GRP,
                 CSP_GPIO_DIR_IN,
                 CSP_GPIO_FLAG_MODE_NONE,
                 DEF_NO,
                 DEF_BIT_NONE,
                 CSP_GPIO_FNCT_00);  
#endif
}



/*
*********************************************************************************************************
*                                    OS_CSP_BSP_ExceptHandler()
*
* Description : Handles ARM exceptions.
*
* Argument(s) : Exception type.
*
*                   CPU_ARM_EXCEPT_RST              Reset exception.
*                   CPU_ARM_EXCEPT_UND              Undefined instruction.
*                   CPU_ARM_EXCEPT_SWI              Software interrupt.
*                   CPU_ARM_EXCEPT_ABORT_PREFETCH   Prefetch Abort.
*                   CPU_ARM_EXCEPT_ABORT_DATA       Data Abort.
*
* Return(s)   : none.
*
* Caller(s)   : OS_CPU_IntHandler().
*
* Note(s)     : (1) This exception handler is implemented with an infinite loop for 
*                   debugging porpuses only. 
*********************************************************************************************************
*/

void OS_CSP_BSP_ExceptHandler  (CPU_INT08U  except_type)
{
    switch (except_type) {
        case CPU_ARM_EXCEPT_RST:
        case CPU_ARM_EXCEPT_UND:
        case CPU_ARM_EXCEPT_SWI:
        case CPU_ARM_EXCEPT_ABORT_PREFETCH:
        case CPU_ARM_EXCEPT_ABORT_DATA:
             while (DEF_TRUE) {
                 ;
             }
    }
}
