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
*                                       POWER & CLOCK MANAGEMENT
*                                           ATMEL AT91SAM9M10
*
* Filename      : csp_pm.c
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
* Note(s)       :
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    CSP_PM_MODULE
#include   <csp.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


#define  CSP_REG_PMC_BASE_ADDR                  ( (CPU_INT32U )(0xFFFFFC00))

#define  CSP_PM_REG_PMC_SCER                    (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x00))
#define  CSP_PM_REG_PMC_SCDR                    (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x04))
#define  CSP_PM_REG_PMC_SCSR                    (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x08))

#define  CSP_PM_REG_PMC_PCER                    (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x10))
#define  CSP_PM_REG_PMC_PCDR                    (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x14))
#define  CSP_PM_REG_PMC_PCSR                    (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x18))

#define  CSP_PM_REG_PMC_CKGR_UCKR               (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x1C))
#define  CSP_PM_REG_PMC_CKGR_MOR                (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x20))
#define  CSP_PM_REG_PMC_CKGR_MCFR               (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x24))
#define  CSP_PM_REG_PMC_CKGR_PLLAR              (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x28))

#define  CSP_PM_REG_PMC_MCKR                    (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x30))
#define  CSP_PM_REG_PMC_USB                     (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x38))
#define  CSP_PM_REG_PMC_PCK0                    (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x40))
#define  CSP_PM_REG_PMC_PCK1                    (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x44))

#define  CSP_PM_REG_PMC_IER                     (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x60))
#define  CSP_PM_REG_PMC_IDR                     (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x64))
#define  CSP_PM_REG_PMC_SR                      (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x68))
#define  CSP_PM_REG_PMC_IMR                     (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x6C))

#define  CSP_PM_REG_PMC_SCKCR                   (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x150))

#define  CSP_PM_REG_PMC_PLLICPR                 (*(CPU_REG32 *)(CSP_REG_PMC_BASE_ADDR + 0x80))

#define  CSP_PM_VAL_RC_SLOW_CRYSTAL                        32000u
#define  CSP_PM_VAL_UTMI_MUL                                  40u

#define  CSP_PM_REG_PMC_SR_LOCKU                 DEF_BIT_06

#define  CSP_PM_BIT_PMC_SCKCR_RCEN               DEF_BIT_00
#define  CSP_PM_BIT_PMC_SCKCR_OSC32EN            DEF_BIT_01
#define  CSP_PM_BIT_PMC_SCKCR_OSC32BYP           DEF_BIT_02
#define  CSP_PM_BIT_PMC_SCKCR_OSCSEL             DEF_BIT_03

#define  CSP_PM_BIT_PMC_MCKR_PLLADIV2            DEF_BIT_12

#define  CSP_PM_REG_PMC_BIT_PCKx_SLCLKMCK        DEF_BIT_08

#define  CSP_PM_BIT_PMC_USB_USBS                 DEF_BIT_00

#define  CSP_PM_MSK_PMC_MDIV                     DEF_BIT_MASK(3u, 8u)

#define  CSP_PM_VAL_REG_TO                                 0xFFFu

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
*                                            LOCAL MACRO's
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
*                                           LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            CSP_PM_Init()
*
* Description : Initialize power management module.
*
* Argument(s) : none.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  CSP_PM_Init (void)
{

}


/*
*********************************************************************************************************
*                                         CSP_PM_CPU_ClkFreqGet()
*
* Description : Return the CPU clock frequency.
*
* Argument(s) : none
*
* Return(s)   : The CPU frequency in Hertz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  CSP_PM_CPU_ClkFreqGet (void)
{
    CPU_INT32U  cpu_freq;
    CPU_INT32U  reg_val;


    cpu_freq = CSP_PM_SysClkFreqGet(CSP_PM_SYS_CLK_NBR_MCK_00); /* Calculate the master clock frequency.                */

    reg_val  = CSP_PM_REG_PMC_MCKR;
    reg_val  = (reg_val >> 8u) & DEF_BIT_FIELD(2u, 0u);

    switch (reg_val) {
        case 1u:
             cpu_freq *= 2u;
             break;

        case 2u:
             cpu_freq *= 4u;
             break;

        case 3u:
             cpu_freq *= 3u;
             break;

        default:
             break;
    }

    return (cpu_freq);
}


/*
*********************************************************************************************************
*                                          CSP_PM_PerClkDivCfg()
*
* Description : Configure peripheral clock divider.
*
* Argument(s) : clk_nbr     Peripheral clock id.
*
*               clk_div     Peripheral Clock divider.
*
* Return(s)   : DEF_OK,   If the clock was configured correctly.
*               DEF_FAIL, Otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Peripherals clk cannot be configured independently.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_PM_PerClkDivCfg (CSP_DEV_NBR  clk_nbr,
                                  CPU_SIZE_T   clk_div)

{
    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                         CSP_PM_PerClkEn()
*
* Description : Enable a peripheral clock.
*
* Argument(s) : clk_nbr     Peripheral clock id.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  CSP_PM_PerClkEn (CSP_DEV_NBR clk_nbr)
{
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (clk_nbr > (CSP_PM_PER_CLK_NBR_MAX_NBR - 1u)) {
        return;
    }
#endif

    CSP_PM_REG_PMC_PCER = DEF_BIT(clk_nbr);
}


/*
*********************************************************************************************************
*                                         CSP_PM_PerClkDis()
*
* Description : Disable a peripheral clock.
*
* Argument(s) : clk_nbr   Peripheral clock id.
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  CSP_PM_PerClkDis (CSP_DEV_NBR clk_nbr)
{
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (clk_nbr > (CSP_PM_PER_CLK_NBR_MAX_NBR - 1u)) {
        return;
    }
#endif

    CSP_PM_REG_PMC_PCDR = DEF_BIT(clk_nbr);
}


/*
*********************************************************************************************************
*                                        CSP_PM_PerClkFreqGet()
*
* Description : Return the Peripheral clock
*
* Argument(s) : clk_nbr     peripheral clock id.
*
* Return(s)   : The peripheral clock frequency.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  CSP_PM_PerClkFreqGet  (CSP_DEV_NBR  clk_nbr)
{
    CPU_INT32U  clk_freq;


    clk_freq  = CSP_PM_SysClkFreqGet(CSP_PM_SYS_CLK_NBR_MCK_00);

    return (clk_freq);
}


/*
*********************************************************************************************************
*                                        CSP_PM_SysClkEn()
*
* Description : Enable a System Clock.
*
* Argument(s) : clk_nbr   System clock number (CSP_PM_SYS_CLK_NBR_XX).
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) System clock definition can be found in the 'csp_grp.h' header file.
*********************************************************************************************************
*/

void  CSP_PM_SysClkEn (CSP_DEV_NBR  clk_nbr)
{
    CPU_INT32U  reg_val;
    CPU_INT16U  reg_to;

#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (clk_nbr > (CSP_PM_SYS_CLK_NBR_MAX - 1u)) {
        return;
    }
#endif

    switch (clk_nbr) {
        case CSP_PM_SYS_CLK_NBR_PCK_00:
             CSP_PM_REG_PMC_SCER = DEF_BIT_08;
             break;

        case CSP_PM_SYS_CLK_NBR_PCK_01:
             CSP_PM_REG_PMC_SCER = DEF_BIT_09;
             break;

        case CSP_PM_SYS_CLK_NBR_DDRCK_00:
             CSP_PM_REG_PMC_SCER = DEF_BIT_02;
             break;

        case CSP_PM_SYS_CLK_NBR_UHP12_00:
        case CSP_PM_SYS_CLK_NBR_UHP48_00:
             CSP_PM_REG_PMC_SCER = DEF_BIT_06;
             break;

        case CSP_PM_SYS_CLK_NBR_UPLL_00:
             CSP_PM_REG_PMC_CKGR_UCKR = (0xF << 28)             /* UTMI Boas Start-up time.                             */
                                      | (0xF << 20);            /* UTMI PLL Start-up time.                              */

             DEF_BIT_SET(CSP_PM_REG_PMC_CKGR_UCKR, DEF_BIT_16); /* UTMO PLL en.                                         */

             reg_val = CSP_PM_REG_PMC_SR;
             reg_to  = CSP_PM_VAL_REG_TO;

             while ((reg_to > 0u) &&
                    (DEF_BIT_IS_CLR(reg_val, CSP_PM_REG_PMC_SR_LOCKU))) {
                reg_to--;
                reg_val = CSP_PM_REG_PMC_SR;
             }

             CSP_PM_REG_PMC_CKGR_UCKR |= DEF_BIT_24;            /* UTMI Bias en.                                        */
             break;

        case CSP_PM_SYS_CLK_NBR_MCK_00:
        case CSP_PM_SYS_CLK_NBR_SYSCLKDDR_00:
        default:
             break;
    }
}


/*
*********************************************************************************************************
*                                        CSP_PM_SysClkDis()
*
* Description : Disable a System Clock
*
* Argument(s) : clk_nbr   System clock number (CSP_PM_SYS_CLK_NBR_XX).
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) See 'CSP_PM_SysClkEn()' note #1
*********************************************************************************************************
*/

void  CSP_PM_SysClkDis (CSP_DEV_NBR  clk_nbr)
{

    CPU_INT32U  reg_val;
    CPU_INT16U  reg_to;


#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (clk_nbr > (CSP_PM_SYS_CLK_NBR_MAX - 1u)) {
        return;
    }
#endif

    switch (clk_nbr) {
        case CSP_PM_SYS_CLK_NBR_PCK_00:
             CSP_PM_REG_PMC_SCDR = DEF_BIT_08;
             break;

        case CSP_PM_SYS_CLK_NBR_PCK_01:
             CSP_PM_REG_PMC_SCDR = DEF_BIT_09;
             break;

        case CSP_PM_SYS_CLK_NBR_DDRCK_00:
             CSP_PM_REG_PMC_SCDR = DEF_BIT_02;
             break;

        case CSP_PM_SYS_CLK_NBR_UHP12_00:
        case CSP_PM_SYS_CLK_NBR_UHP48_00:
             CSP_PM_REG_PMC_SCDR = DEF_BIT_06;
             break;

        case CSP_PM_SYS_CLK_NBR_UPLL_00:
             DEF_BIT_CLR(CSP_PM_REG_PMC_CKGR_UCKR, (DEF_BIT_16 | DEF_BIT_24));

             reg_val = CSP_PM_REG_PMC_SR;
             reg_to  = CSP_PM_VAL_REG_TO;

             while ((reg_to > 0u) &&
                    (DEF_BIT_IS_SET(reg_val,CSP_PM_REG_PMC_SR_LOCKU))) {
                reg_to--;
                reg_val = CSP_PM_REG_PMC_SR;
             }
             break;

        case CSP_PM_SYS_CLK_NBR_MCK_00:
        case CSP_PM_SYS_CLK_NBR_SYSCLKDDR_00:
        default:
             break;
    }
}


/*
*********************************************************************************************************
*                                        CSP_PM_SysClkFreqGet()
*
* Description : Return the frequency for a specific system clock.
*
* Argument(s) : clk_nbr   System clock number (CSP_PM_SYS_CLK_NBR_XX).
*
* Return(s)   : The system clock frequency.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) See 'CSP_PM_SysClkEn()' note #1
*********************************************************************************************************
*/

CPU_INT32U  CSP_PM_SysClkFreqGet (CSP_DEV_NBR  clk_nbr)
{
    CPU_INT32U  mclk_freq;
    CPU_INT32U  plla_clk_freq;
    CPU_INT32U  plla_clk_div2_freq;
    CPU_INT32U  upll_clk_freq;
    CPU_INT32U  slow_clk_freq;
    CPU_INT32U  sys_clk_freq;
    CPU_INT16U  clk_div;
    CPU_INT08U  clk_src;
    CPU_INT16U  pll_mul;
    CPU_INT08U  pll_div;
    CPU_INT32U  reg_val;

                                                                /* ------- CALCULATE INITIAL FREQUENCY VALUES --------- */
    sys_clk_freq  = 0u;
                                                                /* Calculate UPLL frequency.                            */
    upll_clk_freq = (CSP_CFG_PM_MAIN_XTAL_FREQ_HZ * CSP_PM_VAL_UTMI_MUL);

                                                                /* Calculate Slow clock frequency.                      */
    if (DEF_BIT_IS_SET(CSP_PM_REG_PMC_SCKCR, CSP_PM_BIT_PMC_SCKCR_OSCSEL)) {
        slow_clk_freq = CSP_CFG_PM_SLOW_XTAL_FREQ_HZ;
    } else {
        slow_clk_freq = CSP_PM_VAL_RC_SLOW_CRYSTAL;
    }
                                                                /* Calculate PLLA frequency.                            */
    //plla_clk_freq = (CSP_CFG_PM_MAIN_XTAL_FREQ_HZ / DEF_TIME_NBR_uS_PER_SEC);
    plla_clk_freq = (CSP_CFG_PM_MAIN_XTAL_FREQ_HZ >> 10);
                                                                /* ... Get PLL multiplier and divider.                  */
    pll_mul       = (CSP_PM_REG_PMC_CKGR_PLLAR >> 16u) & DEF_BIT_FIELD(8u, 0u);
    pll_div       = (CSP_PM_REG_PMC_CKGR_PLLAR       ) & DEF_BIT_FIELD(8u, 0u);
    if (pll_div != 0) {
        plla_clk_freq  = plla_clk_freq * (pll_mul + 1u) / pll_div;
        //plla_clk_freq *= DEF_TIME_NBR_uS_PER_SEC;
        plla_clk_freq <<= 10;
    } else {
        plla_clk_freq = 0u;
    }

    plla_clk_div2_freq = plla_clk_freq;
                                                                /* ... Determine if PLLADIV2 signal value.              */
    if (DEF_BIT_IS_SET(CSP_PM_REG_PMC_MCKR, CSP_PM_BIT_PMC_MCKR_PLLADIV2)) {
        plla_clk_div2_freq = (plla_clk_freq / 2u);
    }

    reg_val = CSP_PM_REG_PMC_MCKR;                              /* Calculate Master clock frequency.                    */
    clk_src = (CPU_INT08U)(reg_val & DEF_BIT_FIELD(2u, 0u));
    clk_div = (reg_val >> 2u) & DEF_BIT_FIELD(3u, 0u);
    clk_div = DEF_BIT(clk_div);

    switch (clk_src) {                                          /* ... Get master clock source.                         */
        case 0u:
             mclk_freq = slow_clk_freq;                         /* ... Slow clock.                                      */
             break;

        case 1u:
             mclk_freq = CSP_CFG_PM_MAIN_XTAL_FREQ_HZ;          /* ... Main Osicllator.                                 */
             break;

        case 2u:
             mclk_freq = plla_clk_div2_freq;                    /* ... PLLA output/PLLA output divided by 2.            */
             break;

        case 3u:
             mclk_freq = upll_clk_freq;                         /* ... UPLL                                             */
             break;

        default:
             break;
    }

    mclk_freq /= clk_div;                                       /* ... Get Master clock division.                       */
    clk_div    = (reg_val >> 8u) & DEF_BIT_FIELD(2u, 0u);
    switch (clk_div) {
        case 1u:
             mclk_freq /= 2u;
             break;

        case 2u:
             mclk_freq /= 4u;
             break;

        case 3u:
             mclk_freq /= 3u;
             break;

        default:
             break;
    }
                                                                /* ------- CALCULATE SYSTEM CLOCK FREQUENCIES  -------- */
    switch (clk_nbr) {
        case CSP_PM_SYS_CLK_NBR_PCK_00:                         /* ... Programable clock.                               */
        case CSP_PM_SYS_CLK_NBR_PCK_01:

             if (clk_nbr == CSP_PM_SYS_CLK_NBR_PCK_00) {
                 reg_val = CSP_PM_REG_PMC_PCK0;
             } else {
                 reg_val = CSP_PM_REG_PMC_PCK1;
             }
             clk_src = reg_val & DEF_BIT_FIELD(2u, 0u);         /* ... Determine clk source selection.                  */
             switch (clk_src) {
                 case 0u:
                      if (DEF_BIT_IS_CLR(reg_val, CSP_PM_REG_PMC_BIT_PCKx_SLCLKMCK)) {
                          sys_clk_freq = slow_clk_freq;         /* ... Slow clock.                                      */
                      } else {
                          sys_clk_freq = mclk_freq;             /* ... Master clock.                                    */
                      }
                      break;

                 case 1u:                                       /* ... Main clock.                                      */
                      sys_clk_freq = CSP_CFG_PM_MAIN_XTAL_FREQ_HZ;
                      break;

                 case 2u:                                       /* ... PLLA/PLLADIV2.                                   */
                      sys_clk_freq = plla_clk_div2_freq;
                      break;

                 case 3u:                                       /* ... UPLL.                                            */
                      sys_clk_freq = upll_clk_freq;
                      break;

                 default:
                      break;
             }
             clk_div   = (reg_val >> 2u) & DEF_BIT_FIELD(3u, 0u);
             clk_div   = DEF_BIT(clk_div);
             sys_clk_freq /= clk_div;
             break;

        case CSP_PM_SYS_CLK_NBR_DDRCK_00:                       /* ... DDR clcok.                                       */
             reg_val = CSP_PM_REG_PMC_MCKR;
             if (DEF_BIT_IS_CLR(reg_val, CSP_PM_MSK_PMC_MDIV) == DEF_NO) {
                 sys_clk_freq = mclk_freq;                      /* ... DDR clock = Mclk when Mclk != Processor clk.     */
             }
             break;

        case CSP_PM_SYS_CLK_NBR_SYSCLKDDR_00:
             reg_val = CSP_PM_REG_PMC_MCKR;
             if (DEF_BIT_IS_CLR(reg_val, CSP_PM_MSK_PMC_MDIV) == DEF_NO) {
                 sys_clk_freq = 2u * mclk_freq;                 /* ... SysClk DDR = 2xMclk when Mclk != Processor clk.  */
             }
             break;

        case CSP_PM_SYS_CLK_NBR_UPLL_00:                        /* ... USB PLL.                                         */
             sys_clk_freq = upll_clk_freq;
             break;

        case CSP_PM_SYS_CLK_NBR_UHP48_00:                       /* ... OHCI 48 and 12 clocks.                           */
        case CSP_PM_SYS_CLK_NBR_UHP12_00:
             reg_val = CSP_PM_REG_PMC_USB;
             if (DEF_BIT_IS_SET(reg_val, CSP_PM_BIT_PMC_USB_USBS)) {
                 sys_clk_freq = plla_clk_freq;
             } else {
                 sys_clk_freq = upll_clk_freq;
             }
             clk_div = (reg_val >> 8u) & DEF_BIT_FIELD(4u, 0u);
             clk_div++;
             sys_clk_freq /= clk_div;

             if (clk_nbr == CSP_PM_SYS_CLK_NBR_UHP12_00) {
                 sys_clk_freq /= 4u;
             }
             break;

        case CSP_PM_SYS_CLK_NBR_MCK_00:                         /* Master clock.                                        */
             sys_clk_freq = mclk_freq;
             break;

        default:
             break;
    }

    return (sys_clk_freq);
}


/*
*********************************************************************************************************
*                                         CSP_PM_SysClkDivCfg()
*
* Description : Configure peripheral clock divider.
*
* Argument(s) : clk_nbr     System clock number (see 'CSP_PM_SysClkEn()' note #1)
*
*               clk_div     System clock divider.
*
* Return(s)   : DEF_OK,    If the clock was configured,
*               DEF_FAIL,  otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_PM_SysClkDivCfg (CSP_DEV_NBR  clk_nbr,
                                  CPU_SIZE_T   clk_div)
{
    (void)&clk_nbr;
    (void)&clk_div;

    return (DEF_OK);
}