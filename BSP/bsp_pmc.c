/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                            (c) Copyright 2006-2009; Micrium, Inc.; Weston, FL
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
*                                         (INTERRUPT CONTROLLER)
*
* Filename      : bsp_int.c
* Version       : V2.00
* Programmer(s) : FT
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_PMC_MODULE
#include <bsp.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
                                                                /* -------------- TIMEOUT & CONSTRIANS -------------- */
#define  BSP_PMC_PLL_TO                     0x0000FFFF
#define  BSP_PMC_PLL_MAX_MUL                      2047          /* Maximum PLL Multiplier value                       */
#define  BSP_PMC_MAIN_CLK_STARTUP_TO        0x000000FF          /* Main clk startup timeout                           */
#define  BSP_PMC_MAIN_OSC_MAINF_TO          0x000000FF          /* Main clk ready flag timeout                        */
#define  BSP_PMC_MCLK_TO                    0x000000FF          /* Master clk timeout                                 */


                                                                /* ------- MASTER CLOCK REGISTER BIT DEFINES -------- */
#define  BSP_PMC_MCKR_PRES_2                DEF_BIT_MASK(1, 2)
#define  BSP_PMC_MCKR_PRES_4                DEF_BIT_MASK(2, 2)
#define  BSP_PMC_MCKR_PRES_8                DEF_BIT_MASK(3, 2)
#define  BSP_PMC_MCKR_PRES_16               DEF_BIT_MASK(4, 2)
#define  BSP_PMC_MCKR_PRES_32               DEF_BIT_MASK(5, 2)
#define  BSP_PMC_MCKR_PRES_64               DEF_BIT_MASK(6, 2)

#define  BSP_PMC_MCKR_MDIV_2                DEF_BIT_MASK(1, 8)
#define  BSP_PMC_MCKR_MDIV_4                DEF_BIT_MASK(2, 8)

#define  BSP_PMC_BASE_ADDR                  0xFFFFFC00
#define  BSP_PMC_BASE_REG             ((BSP_PMC_REG_PTR)(BSP_PMC_BASE_ADDR))

                                                                /* ----- CLK GENERATOR PLL REGISTER BIT DEFINES ----- */
#define  BSP_PMC_PLL_DIV_MASK               DEF_BIT_FIELD(8,  0)
#define  BSP_PMC_PLL_CNT_MASK               DEF_BIT_FIELD(6,  8)
#define  BSP_PMC_PLL_OUT_MASK               DEF_BIT_FIELD(2,  14)
#define  BSP_PMC_PLL_MUL_MASK               DEF_BIT_FIELD(11, 16)

#define  BSP_PMC_PLLB_USBDIV_MASK           DEF_BIT_FIELD(2, 28)
#define  BSP_PMC_PLLB_USBDIV_1              DEF_BIT_NONE
#define  BSP_PMC_PLLB_USBDIV_2              DEF_BIT_MASK(1, 28)
#define  BSP_PMC_PLLB_USBDIV_4              DEF_BIT_MASK(2, 28)

/*
*********************************************************************************************************
*                                       EXTERN  GLOBAL VARIABLES
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

typedef struct BSP_PMC_REG {
    CPU_REG32  PMC_SCER; 	                                /* System Clock Enable Register                       */ 
    CPU_REG32  PMC_SCDR;                                        /* System Clock Disable Register                      */
    CPU_REG32  PMC_SCSR;                                        /* System Clock Status Register                       */
    CPU_REG32  RESERVED0[1];                                    /*                                                    */
    CPU_REG32  PMC_PCER;                                        /* Peripheral Clock Enable Register                   */
    CPU_REG32  PMC_PCDR;                                        /* Peripheral Clock Disable Register                  */
    CPU_REG32  PMC_PCSR;                                        /* Peripheral Clock Status Register                   */
    CPU_REG32  RESERVED1[1];                                    /*                                                    */
    CPU_REG32  PMC_MOR;                                         /* Main Oscillator Register                           */
    CPU_REG32  PMC_MCFR;                                        /* Main Clock  Frequency Register                     */
    CPU_REG32  PMC_PLLAR;                                       /* PLL A Register                                     */
    CPU_REG32  PMC_PLLBR;                                       /* PLL B Register                                     */
    CPU_REG32  PMC_MCKR;                                        /* Master Clock Register                              */
    CPU_REG32  RESERVED2[3];                                    /*                                                    */
    CPU_REG32  PMC_PCKR[8];                                     /* Programmable Clock Register                        */
    CPU_REG32  PMC_IER;                                         /* Interrupt Enable Register                          */
    CPU_REG32  PMC_IDR;                                         /* Interrupt Disable Register                         */ 
    CPU_REG32  PMC_SR;                                          /* Status Register                                    */
    CPU_REG32  PMC_IMR;                                         /* Interrupt Mask Register                            */
} BSP_PMC_REG, *BSP_PMC_REG_PTR;


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
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*********************************************************************************************************
**                                        GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
**                                        CPU CLK FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         BSP_CPU_ClkFreqGet()
*
* Description : Get the processor clock frequency
*
* Argument(s) : none.
*
* Return(s)   : The processor clock frequency, in Hz.
*
* Caller(s)   : Application
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_PMC_CPU_FreqGet (void)
{
    CPU_INT32U  cpu_css;
    CPU_INT32U  cpu_div;
    CPU_INT32U  cpu_freq;

    
    cpu_css  = (BSP_PMC_BASE_REG->PMC_MCKR     ) & 0x00000003;    /* Get the master clk source                          */
    cpu_div  = (BSP_PMC_BASE_REG->PMC_MCKR >> 2) & 0x00000007;    /* Get the cpu clk preescaler                         */
    cpu_div  = DEF_BIT(cpu_div);                                  /* Convert the Processor clk pres (1, 2, 4, 8, etc)   */
    
    switch (cpu_css) {
        case 0x00:                             
             cpu_freq = BSP_SLOW_XTAL_FREQ_HZ;                    /* Slow clock                                         */
             break;

        case 0x01:                                               
             cpu_freq = BSP_MAIN_XTAL_FREQ_HZ;                    /* Main clock                                         */
             break;

        case 0x02:                                                /* PLL A clock                                        */
             cpu_freq = BSP_PMC_PLL_FreqGet(BSP_PMC_PLL_ID_PLLA);
             break;

        case 0x03:                                                /* PLL B clock                                        */
             cpu_freq = BSP_PMC_PLL_FreqGet(BSP_PMC_PLL_ID_PLLB);
             break;

        default:
             cpu_freq = 0;
             break;             
    }

    cpu_freq = cpu_freq / cpu_div;

    return (cpu_freq);
}

/*
*********************************************************************************************************
*********************************************************************************************************
**                                       MAIN OSCILLATOR FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           BSP_PMC_MainOscDis()
*
* Description : Disable the Main Oscillator.
*
* Argument(s) : none.
*
* Return(s)   : none.
*                               
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void BSP_PMC_MainOscDis (void)
{
    DEF_BIT_CLR(BSP_PMC_BASE_REG->PMC_MOR, DEF_BIT_00);
}            


/*
*********************************************************************************************************
*                                           BSP_PMC_MainOscEn()
*
* Description : Enable the Main Oscillator.
*
* Argument(s) : start_time    Specifies the number of slow clock cycles multiply by 8 
*                             for the main oscillator start-up time.
*
* Return(s)   : DEF_TRUE      If the main oscillator is enabled and stabilized
*               DEF_FALSE     If the main oscillator could not be stabilized during a period of time
*                             specified by BSP_MAIN_CLK_STARTUP_TO
* 
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_PMC_MainOscEn (CPU_INT08U  start_time)
{
    CPU_INT32U  main_osc_to;
    
   
    BSP_PMC_BASE_REG->PMC_MOR = DEF_BIT_00                        /* Enable the main oscillator                         */
                              | (start_time << 8);

    main_osc_to               = BSP_PMC_MAIN_CLK_STARTUP_TO;
    
    while ((main_osc_to > 0) &&                                 /* Wait until the main oscillator is stablibilized    */
           (DEF_BIT_IS_CLR(BSP_PMC_BASE_REG->PMC_SR, DEF_BIT_00))) {
        main_osc_to--;       
    }
           
    if (main_osc_to == 0) {                                     /* If a timeout ocurred return a error                */
        return (DEF_TRUE);
    } else {
        return (DEF_FALSE);
    }        
}
                        
/*
*********************************************************************************************************
*                                        BSP_PMC_MainOscFreqGet()
*
* Description : Return the Frequency of the main oscillator.
*
* Argument(s) : none
*               
*
* Return(s)   : The frequency of the Main Oscillator.
*                               
* Caller(s)   : Application.
*
* Note(s)     : (1) The Main Oscillator features a Main Clock frequency counter that provides the quartz frequency
*                   connected to the Main Oscillator. Generally, this value is known by the system designer; however,
*                   it is useful for the boot program to configure the device with the correct clock speed,
*                   independently of the application.
*
*               (2) This functions should not be used  unless the current value of the slow clock frequency is know.
*                   Since th RC Internal oscillator characteristics may vary (capacitancy value, etc).
*********************************************************************************************************
*/

CPU_INT32U  BSP_PMC_MainOscFreqGet (void)
{
    CPU_INT32U  mclk_to;
    CPU_INT16U  main_clk_ctr;
    CPU_INT32U  main_clk_freq;
    
    
    mclk_to = BSP_PMC_MAIN_OSC_MAINF_TO;
    
    while ((mclk_to > 0) && 
           (DEF_BIT_IS_CLR(BSP_PMC_BASE_REG->PMC_MCFR, DEF_BIT_16))) {
        mclk_to--;       
    }

    if (mclk_to == 0) {
        main_clk_freq = 0;
    } else {
        main_clk_ctr  = (BSP_PMC_BASE_REG->PMC_MCFR & 0xFFFF);
        main_clk_freq = (main_clk_ctr * BSP_SLOW_XTAL_FREQ_HZ) / 16;
    }    
    
    return (main_clk_freq);
}


/*
*********************************************************************************************************
*                                            BSP_PMC_MclkCfg()
*
* Description : This function configure the Master clk and the Processor clk
*
* Argument(s) : cpu_clk_scr   Selects the CPU clk source:
*                                 BSP_PMC_CLK_SRC_SLOW      
*                                 BSP_PMC_CLK_SRC_MAIN      
*                                 BSP_PMC_CLK_SRC_PLLA
*                                 BSP_PMC_CLK_SRC_PLLB
*   
*
*               cpu_clk_pre   The processor clk preescaler
*                                 BSP_PMC_CLK_PRE1   Selected clock
*                                 BSP_PMC_CLK_PRE2   Selected clock divided by  2
*                                 BSP_PMC_CLK_PRE4   Selected clock divided by  4
*                                 BSP_PMC_CLK_PRE8   Selected clock divided by  8
*                                 BSP_PMC_CLK_PRE16  Selected clock divided by 16
*                                 BSP_PMC_CLK_PRE32  Selected clock divided by 32
*                                 BSP_PMC_CLK_PRE64  Selected clock divided by 64

*               mclk_pre      The master clk preescaler/divider.
*                                 BSP_PMC_CLK_PRE1   Selected clock
*                                 BSP_PMC_CLK_PRE2   Selected clock divided by  2
*                                 BSP_PMC_CLK_PRE4   Selected clock divided by  4
*
*   
* Return(s)   : DEF_TRUE      If the master clk is ready
*               DEF_FALSE     If the main oscillator could not be set in BSP_MCLK_MAX_TIMEOUT cycles
*                             or if a the paramater are incorrect
*                             
* Return(s)   : none. 
*                               
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_PMC_MclkCfg (CPU_INT08U  cpu_clk_src,
                              CPU_INT08U  cpu_clk_pre,
                              CPU_INT08U  mclk_pre)

{
    CPU_INT32U  mclk_to;
    CPU_INT32U  reg_val;
                                                                /* --------------- ARGUMENTS CHECKING --------------- */
 
    reg_val = DEF_BIT_NONE;
    
    switch (cpu_clk_src) {
        case BSP_PMC_CLK_SRC_SLOW:     
             break;
       
        case BSP_PMC_CLK_SRC_MAIN:     
             reg_val = 0x01;
             break;
             
        case BSP_PMC_CLK_SRC_PLLA:
             reg_val = 0x02;
             break;
                 
        case BSP_PMC_CLK_SRC_PLLB:
             reg_val = 0x03;        
             break;
 
        default:
            return (DEF_FALSE);                 
    }
    
    switch (cpu_clk_pre) {             
        case BSP_PMC_CLK_PRE1:
             break;
        
        case BSP_PMC_CLK_PRE2:
             DEF_BIT_SET(reg_val, BSP_PMC_MCKR_PRES_2);
             break;
                 
        case BSP_PMC_CLK_PRE8:
             DEF_BIT_SET(reg_val, BSP_PMC_MCKR_PRES_8);
             break;

        case BSP_PMC_CLK_PRE16:
             DEF_BIT_SET(reg_val, BSP_PMC_MCKR_PRES_16);
             break;
                 
        case BSP_PMC_CLK_PRE32:
             DEF_BIT_SET(reg_val, BSP_PMC_MCKR_PRES_32);
             break;

        case BSP_PMC_CLK_PRE64:
             DEF_BIT_SET(reg_val, BSP_PMC_MCKR_PRES_64);
             break;
             
             
        default:
            return (DEF_FALSE);
    }
    
     switch (mclk_pre) {
        case BSP_PMC_CLK_PRE1:               
             break;
             
        case BSP_PMC_CLK_PRE2:
             DEF_BIT_SET(reg_val, BSP_PMC_MCKR_MDIV_2);
             break;
                 
        case BSP_PMC_CLK_PRE4:
             DEF_BIT_SET(reg_val, BSP_PMC_MCKR_MDIV_4);
             break;
             
             
        default:
             return (DEF_FALSE);
    }

    
    BSP_PMC_BASE_REG->PMC_MCKR = reg_val;
    
    mclk_to = BSP_PMC_MCLK_TO;

    while ((mclk_to > 0) && 
          (DEF_BIT_IS_CLR(BSP_PMC_BASE_REG->PMC_SR, DEF_BIT_03))) {
        mclk_to--;       
    }
        
    if (mclk_to == 0) {
        return (DEF_FALSE);
    } else {
        return (DEF_TRUE);  
    }

}

/*
*********************************************************************************************************
*                                         BSP_PMC_MclkFreqGet()
*
* Description : Get the master clock frequency (MCK).
*
* Argument(s) : none.
*
* Return(s)   : The master clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_PMC_MclkFreqGet (void)
{
    CPU_INT32U  mclk_freq;
    CPU_INT08U  mclk_div;

    
    mclk_freq = BSP_PMC_CPU_FreqGet();
    mclk_div  = ((BSP_PMC_BASE_REG->PMC_MCKR >> 8) & 0x03);     /* Read the Master Clock divider                      */
    mclk_div  = DEF_BIT(mclk_div);
    mclk_freq /= mclk_div;

    return (mclk_freq);
}




/*
*********************************************************************************************************
*********************************************************************************************************
**                                            PLL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            BSP_PMC_PLL_Set()
*
* Description : This funciton enables and configures the PLL
*
* Argument(s) : pll_id     The pll identifier
*                              BSP_PMC_PLL_A
*                              BSP_PMC_PLL_B
*
*               pll_mul    The pll Multipler.   
*
*               pll_div    The pll divider.
*
*               pll_ctr    The number of slow clock cycles before the LOCKA bit ise set in PMC_SR.
*
*               pll_out    : PLL frequency optimization parameter (see note #1)
*                            DEF_FALSE  Output frequency between  80-200
*                            DEF_TRUE   Output frequency between 190-240
*
* Return(s)   : DEF_TRUE   If the PLL is enabled and stabilized
*               DEF_FALSE  If the PLL could not be stabilized during a period of time
*                          specified by BSP_PLL_MAX_TIMEOUT
*                               
* Caller(s)   : Application.
*
* Note(s)     : (1) From the AT91SAM9RL64 datasheet 
*                   (http://www.atmel.com/dyn/resources/prod_documents/doc6289.pdf)
*                   The PLLA has the following characteristics.
*
*                 ||------------------|---------------------------------|-------|-------|--------||
*                 || Parameter        |   Condition                     |  MIN  |  MAX  |  Unit  ||
*                 ||------------------|---------------------------------|-------|-------|--------||
*                 || Output Frequency | If OUT field in CKGR_PLL is 00  |   80  |  200  |  Mhz   ||
*                 ||------------------|---------------------------------|-------|-------|--------||
*                 || Output Frequency | If OUT field in CKGR_PLL is 01  |   190 |  240  |  Mhz   ||
*                 ||-----------------------------------------------------------------------------||

*********************************************************************************************************
*/
            
CPU_BOOLEAN  BSP_PMC_PLL_Cfg (CPU_INT08U  pll_id,
                              CPU_INT16U  pll_mul,
                              CPU_INT08U  pll_div,
                              CPU_BOOLEAN pll_out,
                              CPU_INT08U  pll_ctr)
{
    CPU_INT32U  pll_to;
    CPU_INT32U  reg_val;
    CPU_INT08U  pll_status_mask;
    
                                                                /* ---------------- ARGUMENTS CHECKING -------------- */
    if (pll_mul > BSP_PMC_PLL_MAX_MUL) {
        return (DEF_FALSE);
    }
        
    if ((pll_id != BSP_PMC_PLL_ID_PLLA) &&
        (pll_id != BSP_PMC_PLL_ID_PLLB)) {
        return (DEF_FALSE);
    }
    
    reg_val =  ((pll_div      << 0 ) & BSP_PMC_PLL_DIV_MASK)
            |  ((pll_ctr      << 8 ) & BSP_PMC_PLL_CNT_MASK) 
            |  ((pll_out      << 14) & BSP_PMC_PLL_OUT_MASK)
            | (((pll_mul - 1) << 16) & BSP_PMC_PLL_MUL_MASK);
    
    if (pll_id == BSP_PMC_PLL_ID_PLLA) {
        BSP_PMC_BASE_REG->PMC_PLLAR  = reg_val
                                     | DEF_BIT_29;        
        pll_status_mask              = DEF_BIT_01;
    } 
    
    if (pll_id == BSP_PMC_PLL_ID_PLLB) {
        BSP_PMC_BASE_REG->PMC_PLLBR  = reg_val;    
        pll_status_mask              = DEF_BIT_02;
    }
                                 
    pll_to = BSP_PMC_PLL_TO;

    while ((pll_to > 0) && 
          (DEF_BIT_IS_CLR(BSP_PMC_BASE_REG->PMC_SR, pll_status_mask))) {
        pll_to--;       
    }
        
    if (pll_to == 0) {
        return (DEF_FALSE);
    } else {
        return (DEF_TRUE);  
    }
                 
}


/*
*********************************************************************************************************
*                                          BSP_PMC_PLL_FreqGet()
*
* Description : This function return the frequency of the PLL
*
* Argument(s) : none.
*
* Return(s)   : The PLL frequency. (If the PLL is disabled this function returns 0)
*                                              
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/
            
CPU_INT32U  BSP_PMC_PLL_FreqGet (CPU_INT08U  pll_id)
{
    CPU_INT08U  pll_div;
    CPU_INT16U  pll_mul;
    CPU_INT32U  pll_freq;
  
                                                                /* ---------------- ARGUMENTS CHECKING -------------- */    
    if ((pll_id != BSP_PMC_PLL_ID_PLLA) &&
        (pll_id != BSP_PMC_PLL_ID_PLLB)) {
        return (DEF_FALSE);
    }
    
    if (pll_id == BSP_PMC_PLL_ID_PLLA) {
        pll_div = ((BSP_PMC_BASE_REG->PMC_PLLAR >>  0) & 0x0FF);
        pll_mul = ((BSP_PMC_BASE_REG->PMC_PLLAR >> 16) & 0x7FF);       
    }     
    if (pll_id == BSP_PMC_PLL_ID_PLLB) {
        pll_div = ((BSP_PMC_BASE_REG->PMC_PLLBR >>  0) & 0x0FF);
        pll_mul = ((BSP_PMC_BASE_REG->PMC_PLLBR >> 16) & 0x7FF);       
    }
               
    if (pll_div != 0) {
        pll_freq  = ((BSP_MAIN_XTAL_FREQ_HZ / 100) * (pll_mul + 1)) / pll_div;    
        pll_freq *= 100;
    }
    
    return (pll_freq);        
}



/*
*********************************************************************************************************
*                                           BSP_PMC_SysClk_En()
*
* Description : This function enables a specic system clk
*
* Argument(s) : sys_clk_id          The system clk id
*                                       BSP_PMC_SYS_CLK_ID_UHP
*                                       BSP_PMC_SYS_CLK_ID_UDP 
*                                       BSP_PMC_SYS_CLK_ID_PCK0   
*                                       BSP_PMC_SYS_CLK_ID_PCK1  
*                                       BSP_PMC_SYS_CLK_ID_PCK2
*                                       BSP_PMC_SYS_CLK_ID_PCK3   
*                                       BSP_PMC_SYS_CLK_ID_HCK0
*                                       BSP_PMC_SYS_CLK_ID_HCK1
*
*
* Return  (s) : none.
*
* Caller  (s) : Application
*
* Note(s)     : none.              
*********************************************************************************************************
*/

void  BSP_PMC_SysClkEn (CPU_INT08U  sys_clk_id)
{

    CPU_INT32U  reg_val;
    
    
    reg_val = DEF_BIT_NONE;   

    switch (sys_clk_id) {        
        case  BSP_PMC_SYS_CLK_ID_UHP:
              reg_val = DEF_BIT_06;
              break;

        case  BSP_PMC_SYS_CLK_ID_UDP:
              reg_val = DEF_BIT_07;
              break;              

        case  BSP_PMC_SYS_CLK_ID_PCK0:
              reg_val = DEF_BIT_08;
              break;              

        case  BSP_PMC_SYS_CLK_ID_PCK1:
              reg_val = DEF_BIT_09;
              break;              

        case  BSP_PMC_SYS_CLK_ID_PCK2:
              reg_val = DEF_BIT_10;
              break;              

        case  BSP_PMC_SYS_CLK_ID_PCK3:
              reg_val = DEF_BIT_11;
              break;              

        case  BSP_PMC_SYS_CLK_ID_HCK0:
              reg_val = DEF_BIT_16;
              break;

        case  BSP_PMC_SYS_CLK_ID_HCK1:
              reg_val = DEF_BIT_17;
              break;
        
        default:
              break;

    }
    
    DEF_BIT_SET(BSP_PMC_BASE_REG->PMC_SCER, reg_val);
}


/*
*********************************************************************************************************
*                                           BSP_PMC_SysClk_En()
*
* Description : This function enables a specic system clk
*
* Argument(s) : sys_clk_id          The system clk id
*                                       BSP_PMC_SYS_CLK_ID_UHP
*                                       BSP_PMC_SYS_CLK_ID_UDP 
*                                       BSP_PMC_SYS_CLK_ID_PCK0   
*                                       BSP_PMC_SYS_CLK_ID_PCK1  
*                                       BSP_PMC_SYS_CLK_ID_PCK2
*                                       BSP_PMC_SYS_CLK_ID_PCK3   
*                                       BSP_PMC_SYS_CLK_ID_HCK0
*                                       BSP_PMC_SYS_CLK_ID_HCK1
*
* Return  (s) : none.
*
* Caller  (s) : Application
*
* Note(s)     : none.              
*********************************************************************************************************
*/

void  BSP_PMC_SysClkDis (CPU_INT08U  sys_clk_id)
{

    CPU_INT32U  reg_val;
    
    
    reg_val = DEF_BIT_NONE;   

    switch (sys_clk_id) {        
        case  BSP_PMC_SYS_CLK_ID_UHP:
              reg_val = DEF_BIT_06;
              break;

        case  BSP_PMC_SYS_CLK_ID_UDP:
              reg_val = DEF_BIT_07;
              break;

        case  BSP_PMC_SYS_CLK_ID_PCK0:
              reg_val = DEF_BIT_08;
              break;

        case  BSP_PMC_SYS_CLK_ID_PCK1:
              reg_val = DEF_BIT_09;
              break;

        case  BSP_PMC_SYS_CLK_ID_PCK2:
              reg_val = DEF_BIT_10;
              break;

        case  BSP_PMC_SYS_CLK_ID_PCK3:
              reg_val = DEF_BIT_11;
              break;

        case  BSP_PMC_SYS_CLK_ID_HCK0:
              reg_val = DEF_BIT_16;
              break;

        case  BSP_PMC_SYS_CLK_ID_HCK1:
              reg_val = DEF_BIT_17;
              break;
        
        default:
              break;

    }
    
    DEF_BIT_CLR(BSP_PMC_BASE_REG->PMC_SCER, reg_val);
}


/*
*********************************************************************************************************
*                                            BSP_PMC_PLL_Set()
*
* Description : This function sets the USB divider in the PLLB.
*
* Argument(s) : usb_div           The divider for the USB clock
*                                     BSP_PMC_CLK_PRE1   Selected clock divided by  1
*                                     BSP_PMC_CLK_PRE2   Selected clock divided by  2
*                                     BSP_PMC_CLK_PRE4   Selected clock divided by  4

*
* Return(s)   : none.
*                               
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_PMC_USB_ClkCfg  (CPU_INT08U  usb_div)
{
    CPU_INT32U  reg_val;
    
    switch (usb_div) {
        case BSP_PMC_CLK_PRE1:
             reg_val = BSP_PMC_PLLB_USBDIV_1;
             break;
    
        case BSP_PMC_CLK_PRE2:
             reg_val = BSP_PMC_PLLB_USBDIV_2;
             break;
             
        case BSP_PMC_CLK_PRE4:
             reg_val = BSP_PMC_PLLB_USBDIV_4;
             break;

        default:
             return;
    }
    
    DEF_BIT_CLR(BSP_PMC_BASE_REG->PMC_PLLBR, BSP_PMC_PLLB_USBDIV_MASK);
    DEF_BIT_SET(BSP_PMC_BASE_REG->PMC_PLLBR, reg_val);
}