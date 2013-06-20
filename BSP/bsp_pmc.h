/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                              (c) Copyright 2009; Micrium, Inc.; Weston, FL
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
*                                       POWER MANAGMENT CONTROLLER
*
*
* Filename      : bsp_int.h
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

#ifndef  BSP_PMC_PRESENT
#define  BSP_PMC_PRESENT


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


#ifdef   BSP_PMC_MODULE
#define  BSP_PMC_EXT
#else
#define  BSP_PMC_EXT  extern
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

                                                                /* ------------ SLOW CLK GENERATOR DEFINES ---------- */
#define  BSP_PMC_SLOW_CLK_SRC_RC_INT                       1    /* Slow Clk source RC internal                        */
#define  BSP_PCM_SLOW_CLK_SRC_OSC32                        2    /* Slow clk source 32768 Hz crystal                   */
#define  BSP_PMC_SLOW_CLK_SRC_RC_INT_FREQ_HZ           32000    /* RC internal frequency (hz)                         */
#define  BSP_PMC_SLOW_CLK_SRC_OSC32_FREQ_HZ            32768    /* 32768 slow clk oscilator                           */

                                                                /* ---------------- MAIN XTAL DEFINES --------------- */
#define  BSP_PMC_MAIN_XTAL_FREQ_HZ                 12000000L    /* Onboard crystal frequency (hz).                    */


                                                                /* ------------------- CLK SOURCES ------------------ */
#define  BSP_PMC_CLK_SRC_SLOW                              0    /* Slow  clock                                        */
#define  BSP_PMC_CLK_SRC_MAIN                              1    /* Main  clock                                        */
#define  BSP_PMC_CLK_SRC_PLLA                              2    /* PLL   clock                                        */ 
#define  BSP_PMC_CLK_SRC_PLLB                              3    /* PLL   clock                                        */ 

                                                                /* -------------- CLOCK PREESCALERS  ---------------- */
#define  BSP_PMC_CLK_PRE1                                  0    /* Selected clock                                     */           
#define  BSP_PMC_CLK_PRE2                                  1    /* Selected clock divided by  2                       */
#define  BSP_PMC_CLK_PRE4                                  2    /* Selected clock divided by  4                       */
#define  BSP_PMC_CLK_PRE8                                  3    /* Selected clock divided by  8                       */
#define  BSP_PMC_CLK_PRE16                                 4    /* Selected clock divided by 16                       */
#define  BSP_PMC_CLK_PRE32                                 5    /* Selected clock divided by 32                       */
#define  BSP_PMC_CLK_PRE64                                 6    /* Selected clock divided by 64                       */

                                                                /* ------------------- USB DIVIDERS  ---------------- */
#define  BSP_PMC_USB_CLK_DIV1                              0    /* Divider output is PLL clock output                 */           
#define  BSP_PMC_USB_CLK_DIV2                              1    /* Divider output is PLL clock output divided by 2    */
#define  BSP_PMC_USB_CLK_DIV4                              2    /* Divider output is PLL clock output divided by 2    */

                                                                /* -------------------- PLL IDs --------------------- */
#define  BSP_PMC_PLL_ID_PLLA                               0 
#define  BSP_PMC_PLL_ID_PLLB                               1
                                                                /* -------------------- SYS CLK ID ------------------ */

#define  BSP_PMC_SYS_CLK_ID_UHP                             0
#define  BSP_PMC_SYS_CLK_ID_UDP                             1
#define  BSP_PMC_SYS_CLK_ID_PCK0                            2
#define  BSP_PMC_SYS_CLK_ID_PCK1                            3
#define  BSP_PMC_SYS_CLK_ID_PCK2                            4
#define  BSP_PMC_SYS_CLK_ID_PCK3                            5
#define  BSP_PMC_SYS_CLK_ID_HCK0                            6
#define  BSP_PMC_SYS_CLK_ID_HCK1                            7


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

CPU_INT32U   BSP_PMC_CPU_FreqGet      (void);

void         BSP_PMC_MainOscDis       (void);
CPU_BOOLEAN  BSP_PMC_MainOscEn        (CPU_INT08U  start_time);
CPU_INT32U   BSP_PMC_MainOscFreqGet   (void);

CPU_BOOLEAN  BSP_PMC_MclkCfg          (CPU_INT08U  cpu_clk_src,
                                       CPU_INT08U  cpu_clk_pre,
                                       CPU_INT08U  mclk_pre);
CPU_INT32U   BSP_PMC_MclkFreqGet      (void);


CPU_INT32U   BSP_PMC_PLL_FreqGet      (CPU_INT08U  pll_id);
CPU_BOOLEAN  BSP_PMC_PLL_Cfg          (CPU_INT08U  pll_id,
                                       CPU_INT16U  pll_mul,
                                       CPU_INT08U  pll_div,
                                       CPU_BOOLEAN pll_out,
                                       CPU_INT08U  pll_ctr);

void         BSP_PMC_SysClkDis        (CPU_INT08U  sys_clk_id);
void         BSP_PMC_SysClkEn         (CPU_INT08U  sys_clk_id);

void         BSP_PMC_USB_ClkCfg       (CPU_INT08U  usb_div);


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
