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
*                                              TIMER MANAGEMENT
*                                              ATMEL AT91SAM9M10
*
* Filename      : csp_tmr.c
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

#define  CSP_TMR_MODULE
#include  <csp.h>

/*
*********************************************************************************************************
*                                    REGISTER & BIT-FIELD DEFINITION
*********************************************************************************************************
*/

                                                                /* ----------- TIMER COUNTER REGISTER DEFINES --------- */
#define  CSP_TMR_ADDR_REG_TC_BASE                (  (CPU_INT32U )0xF8008000)
#define  CSP_TMR_REG_TC_CCRx(tmr_nbr)            ( *(CPU_REG32 *)(CSP_TMR_ADDR_REG_TC_BASE + (tmr_nbr * 0x40) + 0x00))
#define  CSP_TMR_REG_TC_CMRx(tmr_nbr)            ( *(CPU_REG32 *)(CSP_TMR_ADDR_REG_TC_BASE + (tmr_nbr * 0x40) + 0x04))
#define  CSP_TMR_REG_TC_CVx(tmr_nbr)             ( *(CPU_REG32 *)(CSP_TMR_ADDR_REG_TC_BASE + (tmr_nbr * 0x40) + 0x10))
#define  CSP_TMR_REG_TC_RCx(tmr_nbr)             ( *(CPU_REG32 *)(CSP_TMR_ADDR_REG_TC_BASE + (tmr_nbr * 0x40) + 0x1C))
#define  CSP_TMR_REG_TC_SRx(tmr_nbr)             ( *(CPU_REG32 *)(CSP_TMR_ADDR_REG_TC_BASE + (tmr_nbr * 0x40) + 0x20))
#define  CSP_TMR_REG_TC_IERx(tmr_nbr)            ( *(CPU_REG32 *)(CSP_TMR_ADDR_REG_TC_BASE + (tmr_nbr * 0x40) + 0x24))
#define  CSP_TMR_REG_TC_IDRx(tmr_nbr)            ( *(CPU_REG32 *)(CSP_TMR_ADDR_REG_TC_BASE + (tmr_nbr * 0x40) + 0x28))
#define  CSP_TMR_REG_TC_IMRx(tmr_nbr)            ( *(CPU_REG32 *)(CSP_TMR_ADDR_REG_TC_BASE + (tmr_nbr * 0x40) + 0x2C))


                                                                /* ------------- TIMER COUNTER BIT DEFINES ------------ */
#define  CSP_TMR_BIT_TC_CCRx_CLKEN                DEF_BIT_00
#define  CSP_TMR_BIT_TC_CCRx_CLKDIS               DEF_BIT_01
#define  CSP_TMR_BIT_TC_CCRx_SWTRG                DEF_BIT_02

#define  CSP_TMR_MSK_TC_CMRx_TCCLKS               DEF_BIT_FIELD(3u, 0u)
#define  CSP_TMR_BIT_TC_CMRx_WAVSEL_UP_NONE       DEF_BIT_NONE
#define  CSP_TMR_BIT_TC_CMRx_WAVSEL_UP_TRIG       DEF_BIT_MASK(0x02, 13u) 
#define  CSP_TMR_BIT_TC_CMRx_WAVE_EN              DEF_BIT_15

#define  CSP_TMR_BIT_TC_INT_CPCS                  DEF_BIT_04
                                         
                                                                /* ------ PERIODIC INTERVAL TIMER REGISTER DEFINES ----- */
#define  CSP_TMR_ADDR_REG_PIT_BASE               (  (CPU_INT32U )0xFFFFFE30)
#define  CSP_TMR_REG_PIT_MR                      ( *(CPU_REG32 *)(CSP_TMR_ADDR_REG_PIT_BASE + 0x00))         
#define  CSP_TMR_REG_PIT_SR                      ( *(CPU_REG32 *)(CSP_TMR_ADDR_REG_PIT_BASE + 0x04))
#define  CSP_TMR_REG_PIT_PIVR                    ( *(CPU_REG32 *)(CSP_TMR_ADDR_REG_PIT_BASE + 0x08))
#define  CSP_TMR_REG_PIT_PIIR                    ( *(CPU_REG32 *)(CSP_TMR_ADDR_REG_PIT_BASE + 0x0C))

                                                                /* --------- PERIODIC INTERVAL TIMER BIT DEFINES ------- */
#define  CSP_TMR_BIT_PIT_MR_PITEN                 DEF_BIT_24
#define  CSP_TMR_BIT_PIT_MR_PITIEN                DEF_BIT_25


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

typedef  struct  csp_tmr_reg {
    CPU_REG32  IR;
    CPU_REG32  TCR;
    CPU_REG32  TC;
    CPU_REG32  PR;
    CPU_REG32  PC;
    CPU_REG32  MCR;
    CPU_REG32  MRx[4];
    CPU_REG32  CCR;
    CPU_REG32  CCRx[2];
    CPU_REG32  RESERVED0[2];
    CPU_REG32  EMR;
    CPU_REG32  RESERVED1[12];
} CSP_TMR_REG;


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

static  CPU_INT08U  const  CSP_TmrClkDiv[4] = {
      2u,
      8u,
     32u,
    128u
};

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
*                                        CSP_TmrInit()
*
* Description : Initialize & disable all the timers.
*                   
* Argument(s) : none.
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  CSP_TmrInit (void)
{
    CPU_SR_ALLOC();

    
    CPU_CRITICAL_ENTER();
    DEF_BIT_CLR(CSP_TMR_REG_PIT_MR, DEF_BIT_25 | DEF_BIT_24); 

    CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_TMR_00);

    CSP_TMR_REG_TC_CCRx(0u) = DEF_BIT_01;
    CSP_TMR_REG_TC_IDRx(0u) = DEF_BIT_FIELD(8u, 0u);

    CSP_TMR_REG_TC_CCRx(1u) = DEF_BIT_01;
    CSP_TMR_REG_TC_IDRx(1u) = DEF_BIT_FIELD(8u, 0u);
    
    CSP_TMR_REG_TC_CCRx(2u) = DEF_BIT_01;
    CSP_TMR_REG_TC_IDRx(2u) = DEF_BIT_FIELD(8u, 0u);

    CSP_PM_PerClkDis(CSP_PM_PER_CLK_NBR_TMR_00);

    CPU_CRITICAL_EXIT();
}

/*
*********************************************************************************************************
*                                            CSP_TmrCfg()
*
* Description : Configure periodic or free running timer. 
*                   
* Argument(s) : tmr_nbr    Timer number identifier (CSP_TMR_NBR_xx).
*
*               freq       Periodic timer frequency. (see note #1)
*                          
*                          If 'freq' == 0 the timer is configured as free-running timer
*                          otherwise will be configured to generate a interrupt event specified by
*                          the frequency of the timer.
*
* Return(s)   : DEF_OK,   If the timer was configured correctly,
 *              DEF_FAIL, otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) If 'freq' == 0 Timer is configured as free-running timer.
*                   If 'freq' > 0  Timer will be configured to generate a interrupt event every
*                   (1/freq) seconds.
*
*               (2) If the timer is configured in periodic mode, the interrupt handler needs
*                   to be installed first by the application in the interrupt controller.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_TmrCfg (CSP_DEV_NBR   tmr_nbr,
                         CPU_INT32U    freq)
{
    CPU_BOOLEAN found;
    CPU_INT32U  reload_val;
    CPU_INT32U  clk_freq;
    CPU_INT32U  reg_val;
    CPU_INT08U  i;
    CPU_SR_ALLOC();
    
        
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if ((tmr_nbr   > CSP_TMR_NBR_02      ) &&
        (tmr_nbr  != CSP_TMR_NBR_SYS_TICK)) {
        return (DEF_FAIL);
    }
#endif
        
                 
    reload_val = 0;
    found      = DEF_FALSE;
    clk_freq   = CSP_PM_SysClkFreqGet(CSP_PM_SYS_CLK_NBR_MCK_00);           
        
    switch (tmr_nbr) {
        case CSP_TMR_NBR_00:
        case CSP_TMR_NBR_01:
        case CSP_TMR_NBR_02:
                                                                /* --------- FREE RUNNING TIMER CONFIGURATION --------- */
             if (freq == 0u) {                                  /* Select the maximum frequency (MCLK /2) for the ...   */
                                                                /* ... free-running timer                               */
                 CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_TMR_00);    /* Enable timer clock.                                  */

                 CPU_CRITICAL_ENTER();                          
                 CSP_TMR_REG_TC_CMRx(tmr_nbr) = CSP_TMR_BIT_TC_CMRx_WAVE_EN;          
                 CPU_CRITICAL_EXIT();
             } else  {
                 for (i = 0u; i < 4u; i++) {                    /* ... Get a valid match value, trying diferrent ...    */
                     clk_freq   /= CSP_TmrClkDiv[i];            /* ... clocks (MCK/2, MCK/4, MCK/32 and MCK/128)        */
                     reload_val  = (clk_freq / freq);
                     if ((reload_val < DEF_INT_16U_MAX_VAL) &&  
                         (reload_val != 0u                )) {
                         found   = DEF_TRUE;
                         reg_val = i & CSP_TMR_MSK_TC_CMRx_TCCLKS;
                         break;
                     }
                 }
                          
                 if (found == DEF_FALSE) {                      /* If a valid valiue could not found try with ...       */
                                                                /* ... the Slow Clock                                   */                     
                     reload_val = (CSP_CFG_PM_SLOW_XTAL_FREQ_HZ / freq);
                     if ((reload_val < DEF_INT_16U_MAX_VAL) &&
                        (reload_val != 0u                )) {
                         found   = DEF_TRUE;
                         reg_val = 4u;
                     }
                 }
                 
                 if (found == DEF_FALSE) {
                     return (DEF_FAIL);
                 }
                                                                /* Waveform Mode enable and UP mode with automatic ...  */
                                                                /* ...on RC Compare.                                    */
                 DEF_BIT_SET(reg_val, (CSP_TMR_BIT_TC_CMRx_WAVSEL_UP_TRIG |
                                       CSP_TMR_BIT_TC_CMRx_WAVE_EN));
                 
                 CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_TMR_00);    /* Enable timer clock.                                  */

                 CPU_CRITICAL_ENTER();                 
                 CSP_TMR_REG_TC_CMRx(tmr_nbr) = reg_val;
                 CSP_TMR_REG_TC_RCx(tmr_nbr)  = reload_val;                                     
                 CPU_CRITICAL_EXIT();
                                                               /* Enable the Compare Interrupt                          */
                 CSP_TMR_REG_TC_IERx(tmr_nbr) = CSP_TMR_BIT_TC_INT_CPCS;                             
             }
             
             break;
            
        case CSP_TMR_NBR_SYS_TICK:    
             if (freq == 0u) {
                 reg_val = DEF_BIT_FIELD(20u, 0u)
                         | CSP_TMR_BIT_PIT_MR_PITEN;                   
             } else {
                 reload_val = ((clk_freq) / 16u / freq) - 1u;

                 if ((reload_val  > DEF_BIT_FIELD(20u, 0u)) ||
                     (reload_val == 0u                   )) {
                     return (DEF_FAIL);
                 }
                 
                 reg_val = CSP_TMR_BIT_PIT_MR_PITIEN
                         | reload_val; 
             }
                      
             CPU_CRITICAL_ENTER();
             CSP_TMR_REG_PIT_MR = reg_val;
             reg_val            = CSP_TMR_REG_PIT_PIVR;
             CPU_CRITICAL_EXIT();             
             break;
             
        default:
             return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                            CSP_TmrOutCmpCfg()
*
* Description : Configure a timer for compare mode. 
*                   
* Argument(s) : tmr_nbr    Timer number identifier (CSP_TMR_NBR_xx).
*
*               pin_nbr    Pin number.
*
*               pin_action  Output compare pin action
*
*                              CSP_TMR_OPT_PIN_OUT_NONE      Do nothing.
*                              CSP_TMR_OPT_PIN_OUT_CLR       Clear  the correspoding  external pin for output compare.
*                              CSP_TMR_OPT_PIN_OUT_SET       Set    the correspondin  external pin for output compare.                                                       
*                              CSP_TMR_OPT_PIN_OUT_TOGGLE    Toggle the corresponding external pin for output compare.
*
* Return(s)   : DEF_OK,   If the timer was configured correctly in compare mode,
*               DEF_FAIL, otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_TmrOutCmpCfg (CSP_DEV_NBR    tmr_nbr,
                               CSP_DEV_NBR    pin,
                               CSP_OPT        pin_action,
                               CPU_INT32U     freq)

{
    (void)tmr_nbr;
    (void)pin;
    (void)pin_action;
    (void)freq;
        
    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                           CSP_TmrIntClr()
*
* Description : Clear a periodic timer interrupt.
*                   
* Argument(s) : tmr_nbr    Timer number identifier (CSP_TMR_NBR_xx).
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  CSP_TmrIntClr (CSP_DEV_NBR tmr_nbr)
{
    CPU_INT32U status = 0;

#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if ((tmr_nbr   > CSP_TMR_NBR_02      ) &&
        (tmr_nbr  != CSP_TMR_NBR_SYS_TICK)) {
        return status;
    }
#endif
    
   switch (tmr_nbr) {
        case CSP_TMR_NBR_00:
        case CSP_TMR_NBR_01:
        case CSP_TMR_NBR_02:
             status = CSP_TMR_REG_TC_SRx(tmr_nbr);
             break;
            
        case CSP_TMR_NBR_SYS_TICK:
             status = CSP_TMR_REG_PIT_PIVR;
             break;
            
        default:
             break;
    }

    return status;
}

/*
*********************************************************************************************************
*                                            CSP_TmrRst()
*
* Description : Resets a timer.
*                   
* Argument(s) : tmr_nbr    Timer number identifier (CSP_TMR_NBR_xx).
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  CSP_TmrRst (CSP_DEV_NBR  tmr_nbr)
{
    
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if ((tmr_nbr   > CSP_TMR_NBR_02      ) &&
        (tmr_nbr  != CSP_TMR_NBR_SYS_TICK)) {
        return;
    }
#endif
    
    switch (tmr_nbr) {
        case CSP_TMR_NBR_00:
        case CSP_TMR_NBR_01:
        case CSP_TMR_NBR_02:
             CSP_TMR_REG_TC_CCRx(tmr_nbr) = CSP_TMR_BIT_TC_CCRx_SWTRG;
             break;
            
        case CSP_TMR_NBR_SYS_TICK:
        default:
             break;
    }     
}


/*
*********************************************************************************************************
*                                             CSP_TmrRd()
*
* Description : Read the current value of a timer.
*                   
* Argument(s) : tmr_nbr    Timer number identifier (CSP_TMR_NBR_xx).
*
* Return(s)   : The current value of the timer.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CSP_TMR_VAL  CSP_TmrRd (CSP_DEV_NBR  tmr_nbr)
{
    CSP_TMR_VAL  tmr_val;

    
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if ((tmr_nbr   > CSP_TMR_NBR_02      ) &&
        (tmr_nbr  != CSP_TMR_NBR_SYS_TICK)) {
        return (DEF_FAIL);
    }
#endif

    tmr_val = 0u;
    
    switch (tmr_nbr) {
        case CSP_TMR_NBR_00:
        case CSP_TMR_NBR_01:
        case CSP_TMR_NBR_02:
             tmr_val = CSP_TMR_REG_TC_CVx(tmr_nbr) & DEF_BIT_FIELD(16u, 0u);
             break;
            
        case CSP_TMR_NBR_SYS_TICK:
             tmr_val = CSP_TMR_REG_PIT_PIIR & DEF_BIT_FIELD(20u, 0u);        
             break;
             
        default:
             break;
    }
    
    return (tmr_val);
}

/*
*********************************************************************************************************
*                                     CSP_TmrStart()
*
* Description : Start a timer.
*                   
* Argument(s) : tmr_nbr    Timer number identifier (CSP_TMR_NBR_xx).
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  CSP_TmrStart (CSP_DEV_NBR  tmr_nbr)
{
    CPU_SR_ALLOC();


#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if ((tmr_nbr   > CSP_TMR_NBR_02      ) &&
        (tmr_nbr  != CSP_TMR_NBR_SYS_TICK)) {
        return;
    }
#endif
    
    switch (tmr_nbr) {
        case CSP_TMR_NBR_00:
        case CSP_TMR_NBR_01:
        case CSP_TMR_NBR_02:
             CSP_TMR_REG_TC_CCRx(tmr_nbr) = CSP_TMR_BIT_TC_CCRx_CLKEN;
             CSP_TMR_REG_TC_CCRx(tmr_nbr) = CSP_TMR_BIT_TC_CCRx_SWTRG;
             break;
            
        case CSP_TMR_NBR_SYS_TICK:
             CPU_CRITICAL_ENTER();
             DEF_BIT_SET(CSP_TMR_REG_PIT_MR, CSP_TMR_BIT_PIT_MR_PITEN);
             CPU_CRITICAL_EXIT();
             break;
             
        default:
             break;
    }  
}


/*
*********************************************************************************************************
*                                            CSP_TmrStop()
*
* Description : Stop a timer.
*                   
* Argument(s) : tmr_nbr    Timer number identifier (CSP_TMR_NBR_xx).
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  CSP_TmrStop (CSP_DEV_NBR  tmr_nbr)
{
    CPU_SR_ALLOC();

    
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if ((tmr_nbr   > CSP_TMR_NBR_02      ) &&
        (tmr_nbr  != CSP_TMR_NBR_SYS_TICK)) {
        return;
    }
#endif
                
    switch (tmr_nbr) {
        case CSP_TMR_NBR_00:
        case CSP_TMR_NBR_01:
        case CSP_TMR_NBR_02:
             CSP_TMR_REG_TC_CCRx(tmr_nbr) = CSP_TMR_BIT_TC_CCRx_CLKDIS;
             break;
            
        case CSP_TMR_NBR_SYS_TICK:
             CPU_CRITICAL_ENTER();
             DEF_BIT_CLR(CSP_TMR_REG_PIT_MR, CSP_TMR_BIT_PIT_MR_PITEN);
             CPU_CRITICAL_EXIT();
             break;
             
        default:
             break;
    }
}


/*
*********************************************************************************************************
*                                         CSP_TmrWr()
*
* Description : Write a value to the timer.
*                   
* Argument(s) : tmr_nbr    Timer number identifier.
*
*               tmr_val    value to write.
*
* Return(s)   : none
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  CSP_TmrWr (CSP_DEV_NBR  tmr_nbr,
                 CSP_TMR_VAL  tmr_val)
{
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if ((tmr_nbr   > CSP_TMR_NBR_02      ) &&
        (tmr_nbr  != CSP_TMR_NBR_SYS_TICK)) {
        return;
    }
#endif

    (void)tmr_nbr;
    (void)tmr_val;    
}
