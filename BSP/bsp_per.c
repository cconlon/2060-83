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
*                                              PERIPHERALS
*
*
* Filename      : bsp_per.c
* Version       : V2.00
* Programmer(s) : FT
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define   BSP_PER_MODULE
#include  <bsp.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  BSP_PER_PMC_BASE               ((CPU_INT32U)0xFFFFFC00)

#define  BSP_PER_PMC_PCER           (*(CPU_REG32 *)(BSP_PER_PMC_BASE + 0x0010))
#define  BSP_PER_PMC_PCDR           (*(CPU_REG32 *)(BSP_PER_PMC_BASE + 0x0014))
#define  BSP_PER_PMC_PCSR           (*(CPU_REG32 *)(BSP_PER_PMC_BASE + 0x0018))


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
*                                            BSP_PerClkDis()
*
* Description : Disable the corresponding peripheral clock
*
* Argument(s) : per_id        The peripheral id to be disabled (see note #1)
*                             BSP_PER_ID_PIOA
*                             BSP_PER_ID_PIOB
*                                     .
*                                     .
*                                     .
*                             BSP_PER_ID_AIC
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1). The peripheral clk id are defined in the bsp_per.h file.
*********************************************************************************************************
*/

void  BSP_PerClkDis (CPU_INT08U  per_id)
{
                                                                /* ----------------- ARGUMENTS CHECK ---------------- */
    if ((per_id == BSP_PER_ID_AIC_FIQ) && 
        (per_id == BSP_PER_ID_SYSC   )) {
        return;
    }
    
    if (per_id > BSP_PER_ID_MAX) {
        return;
    }
    
    BSP_PER_PMC_PCDR  = DEF_BIT(per_id);
}

/*
*********************************************************************************************************
*                                            BSP_PerClkEn()
*
* Description : Enable the corresponding peripheral clock
*
* Argument(s) : per_id        The peripheral id to be enabled (see note #1)
*                             BSP_PER_ID_PIOA
*                             BSP_PER_ID_PIOB
*                                     .
*                                     .
*                                     .
*                             BSP_PER_ID_AIC
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1). The peripheral clk id are defined in the bsp_per.h file.
*********************************************************************************************************
*/

void  BSP_PerClkEn (CPU_INT08U per_id)
{
                                                                /* ----------------- ARGUMENTS CHECK ---------------- */
    if ((per_id == BSP_PER_ID_AIC_FIQ) && 
        (per_id == BSP_PER_ID_SYSC   )) {
        return;
    }
    
    if (per_id > BSP_PER_ID_MAX) {
        return;
    }
 
    BSP_PER_PMC_PCER = DEF_BIT(per_id);
}

/*
*********************************************************************************************************
*                                        BSP_PerClkStatusGet()
*
* Description : Get the status of the corresponding peripheral clock.
*
* Argument(s) : per_clk_id    The peripheral clock id to be enabled (see note #1)
*                             BSP_PER_CLK_ID_PIOA
*                             BSP_PER_CLK_ID_PIOB
*                                     .
*                                     .
*                                     .
*                             BSP_PER_ID_AIC
*
* Return(s)   : DEF_TRUE      If the specified clk is enabled 
*               DEF_FALSE     If the specidied clk is disabled.
* 
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/


CPU_BOOLEAN  BSP_PerClkStatusGet  (CPU_INT08U  per_id)
{
    CPU_BOOLEAN status;
   
                                                                  /* ----------------- ARGUMENTS CHECK ---------------- */
    if ((per_id == BSP_PER_ID_AIC_FIQ) && 
        (per_id == BSP_PER_ID_SYSC   )) {
        return (DEF_FALSE);
    }
    
    if (per_id > BSP_PER_ID_MAX) {
        return (DEF_FALSE);
    }
 
    status  = DEF_BIT_IS_SET(BSP_PER_PMC_PCSR, DEF_BIT(per_id));
        
    return (status);
}
