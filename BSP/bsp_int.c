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
*                                     MICRIUM BOARD SUPPORT PACKAGE
*                                          INTERRUPT CONTROLLER
*
* Filename      : bsp_int.c
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_INT_MODULE
#include <bsp.h>


static  void   BSP_IntDummyISR_Handler (void);


/*
*********************************************************************************************************
*********************************************************************************************************
**                                        GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          BSP_IntClr()
*
* Description : This function clears an interrupt
*
* Argument(s) : per_id        The interrupt id
*                             BSP_PER_ID_AIC_FIQ
*                             BSP_PER_ID_SYSC
*                             BSP_PER_ID_PIOA
*                             BSP_PER_ID_PIOB
*                                     .
*                                     .
*                                     .
*                             BSP_PER_ID_AIC
*
* Returns     : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void BSP_IntClr (CPU_INT08U  per_id)
{
                                                                /* -------------- ARGUMENTS CHECKING ---------------- */
    if (per_id > BSP_PER_ID_MAX) { 
        return;
    }

    AIC->AIC_ICCR  = DEF_BIT(per_id);
}


/*
*********************************************************************************************************
*                                             BSP_IntDisAll()
*
* Description : Disable ALL interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application
*
* Argument(s) : none.
*********************************************************************************************************
*/

void  BSP_IntDisAll (void)
{                           
    AIC->AIC_IDCR = 0xFFFFFFFF; 
}


/*
*********************************************************************************************************
*                                          BSP_IntEn()
*
* Description : This function enables an interrupt.
*
* Argument(s) : per_id        The interrupt id
*                             BSP_PER_ID_AIC_FIQ
*                             BSP_PER_ID_SYSC
*                             BSP_PER_ID_PIOA
*                             BSP_PER_ID_PIOB
*                                     .
*                                     .
*                                     .
*                             BSP_PER_ID_AIC
*
* Returns     : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntEn (CPU_INT08U  per_id)
{
                                                                /* -------------- ARGUMENTS CHECKING ---------------- */
    if (per_id > BSP_PER_ID_MAX) { 
        return;
    }

    AIC->AIC_IECR = DEF_BIT(per_id);
}


/*
*********************************************************************************************************
*                                          BSP_IntVectSet()
*
* Description : This function sets the specified interrupt source's ISR handler call back function.
*
* Argument(s) : per_id          The interrupt id
*                                   BSP_PER_ID_AIC_FIQ
*                                   BSP_PER_ID_SYSC
*                                   BSP_PER_ID_PIOA
*                                   BSP_PER_ID_PIOB
*                                     .
*                                     .
*                                     .
*                                   BSP_PER_ID_AIC
*
*               int_prio        Interrupt priority level. 0(lowest) - 7 (highest).
*
*               int_src_type    Interrupt source type.
*                                   BSP_INT_SCR_TYPE_INT_HIGH_LEVEL_SENSITIVE
*                                   BSP_INT_SCR_TYPE_INT_POSITIVE_EDGE_TRIG
*                                   BSP_INT_SCR_TYPE_EXT_LOW_LEVEL_SENSITIVE
*                                   BSP_INT_SCR_TYPE_EXT_HIGH_LEVEL_SENSITIVE
*                                   BSP_INT_SCR_TYPE_EXT_NEGATIVE_EDGE_TRIG
*                                   BSP_INT_SCR_TYPE_EXT_POSITIVE_EDGE_TRIG
*                                                
*               int_isr         ISR handler function in the form of 'void ISR_Handler (void)'.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_IntVectSet (CPU_INT08U     per_id,
                      CPU_INT08U     int_prio,
                      CPU_INT08U     int_src_type,
                      CPU_FNCT_VOID  int_isr_fnct)
{
    CPU_INT08U  aic_scr_type;
                      
                                                                /* ---------------- ARGUMENTS CHECKING -------------- */
    if (int_prio > BSP_INT_PRIO_HIGHEST) {
        return;
    }
    
    if (int_isr_fnct == (CPU_FNCT_VOID)0 ) {
        return;
    }
 
    if (per_id > BSP_PER_ID_MAX) { 
        return;
    }

                                                                
    AIC->AIC_SVR[per_id] = (CPU_INT32U)int_isr_fnct;         /* Set the interrupt vector with the ISR handler      */
    
    switch (int_src_type) {
        case BSP_INT_SCR_TYPE_INT_HIGH_LEVEL_SENSITIVE:
        case BSP_INT_SCR_TYPE_EXT_LOW_LEVEL_SENSITIVE:
             aic_scr_type = 0x00;
             break;
    
        case BSP_INT_SCR_TYPE_INT_POSITIVE_EDGE_TRIG:
        case BSP_INT_SCR_TYPE_EXT_NEGATIVE_EDGE_TRIG:
             aic_scr_type = 0x01;
             break;

        case BSP_INT_SCR_TYPE_EXT_HIGH_LEVEL_SENSITIVE:
             aic_scr_type = 0x02;
             break;

        case BSP_INT_SCR_TYPE_EXT_POSITIVE_EDGE_TRIG:
             aic_scr_type = 0x03;
             break;

        default:
             aic_scr_type = 0x00;
             break;             
    }
     
    AIC->AIC_SMR[per_id] = (int_prio         )               /* Set Interrupt priority and type                    */
                            | (aic_scr_type << 5);
}


/*
*********************************************************************************************************
*                                        BSP_IntSched()
*
* Description : This function schedules a new interrup service routine (ISR)
*
* Argument(s) : int_type   Interrupt type selection, 
*                              BSP_INT_CTRL_TYPE_IRQ   The interrupt is a standard interrupt request (IRQ)
*                              BSP_INT_CTRL_TYPE_FIQ   The interrupt is a fast     interrupt request (FIQ)
*
* Return(s)   : none.
*
* Caller(s)   : OS Global Interrupt Handler
*
* Note(s)     : none               
*********************************************************************************************************
*/

void  BSP_IntSched(CPU_INT08U  int_type) 
{
    CPU_FNCT_VOID   p_isr;
    
    if (int_type == BSP_INT_CTRL_TYPE_FIQ) {    
        p_isr = (CPU_FNCT_VOID)AIC->AIC_FVR;                 /* Read the interrupt vector from the VIC             */        

        while (p_isr != (CPU_FNCT_VOID)0) {                     /* Make sure we don't have a NULL pointer             */

            (*p_isr)();
            AIC->AIC_EOICR = DEF_BIT_NONE;                   /* End of handler                                     */   
            p_isr = (CPU_FNCT_VOID)AIC->AIC_FVR;             /* Read the interrupt vector from the VIC             */
    }
        AIC->AIC_EOICR = DEF_BIT_NONE;                       /* End of handler                                     */
    }

    if (int_type == BSP_INT_CTRL_TYPE_IRQ) {    
        p_isr = (CPU_FNCT_VOID)AIC->AIC_IVR;                 /* Read the interrupt vector from the VIC             */        

        while (p_isr != (CPU_FNCT_VOID)0) {                     /* Make sure we don't have a NULL pointer             */

            (*p_isr)();
            AIC->AIC_EOICR = DEF_BIT_NONE;                   /* End of handler                                     */   
            p_isr = (CPU_FNCT_VOID)AIC->AIC_IVR;             /* Read the interrupt vector from the VIC             */
    }
        AIC->AIC_EOICR = DEF_BIT_NONE;                       /* End of handler                                     */
    }
}


/*
*********************************************************************************************************
*                                    BSP_IntCtrlInit()
*
* Description : This function initialize the interrupt controller.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Notes       : (1) The spurious interrupt register must be written to 0 in order to ensure
*                  that OS_CPU_ExceptHndlr() terminates after the last pending interrupt
*                  is serviced.
*********************************************************************************************************
*/

void  BSP_IntInit (void)
{
    CPU_INT16U  per_id;


    BSP_IntDummyISR_Handler();
    AIC->AIC_EOICR = DEF_BIT_NONE;                              /* End-of-interrupt command                           */

                                                                /* Initialize all ISR's to the Dummy ISR handler      */
    for (per_id = BSP_PER_ID_AIC_FIQ; per_id < BSP_PER_ID_MAX; per_id++) {     
         BSP_IntVectSet((CPU_INT08U   )per_id,
                        (CPU_INT08U   )BSP_INT_PRIO_LOWEST,
                        (CPU_INT08U   )BSP_INT_SCR_TYPE_INT_HIGH_LEVEL_SENSITIVE,
                        (CPU_FNCT_VOID)BSP_IntDummyISR_Handler);
    }
}

/*
*********************************************************************************************************
*                                        BSP_DummyISR_Handler()
*
* Description : Handle an invalid IRQ.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_IntDummyISR_Handler (void)
{
    CPU_INT32U  irq_id;


    irq_id = (AIC->AIC_ISR & 0x1F);                          /* Retrieve the ID of the interrupting source         */
    BSP_IntClr(DEF_BIT(irq_id));
}
