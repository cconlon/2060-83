/*
************************************************************************************************************************
*                                                      uC/OS-II
*                                                The Real-Time Kernel
*
*
*                                      (c) Copyright 2009, Micrium, Weston, FL
*                                                 All Rights Reserved
*
*                                            ATMEL AT91SAM9xxx OS CHIP SUPPORT
*
* File      : OS_CSP.C
* Version   : V2.92
* By        : FT
*
* For       : ARM7 or ARM9
* Mode      : ARM or Thumb
************************************************************************************************************************
*/

#define  OS_CSP_GLOBALS
#include <ucos_ii.h>
#include <csp.h>
#include <os_csp.h>

#include <include/pio_it.h>
#include <include/pit.h>
/*$PAGE*/
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
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

static  void  OS_CSP_TickHandler (void *p_arg);


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/



/*$PAGE*/
/*
************************************************************************************************************************
*                                                INITIALIZE TICKER INTERRUPT
*
* Description: Initialize the Periodic Interval Timer (PIT)
*
* Arguments  : none.
*
* Note(s)    : 1) This function MUST be called after OSStart() & after processor initialization.
************************************************************************************************************************
*/

void  OS_CSP_TickInit (void)
{
	(void)CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_SYS,
                       (CSP_DEV_NBR   )CSP_INT_SYS_SRC_NBR_PIT,
                       (CPU_FNCT_PTR  )OS_CSP_TickHandler,
                       (void         *)0);
   
	(void)CSP_TmrCfg(CSP_TMR_NBR_SYS_TICK, OS_TICKS_PER_SEC);    
   
	CSP_TmrStart(CSP_TMR_NBR_SYS_TICK);
}


/*$PAGE*/
/*
************************************************************************************************************************
*                                                  TICKER INTERRUPT HANDLER
*
* Description: Handle the PIT interrupt.
*
* Arguments  : p_arg   Pointer to the ISR argument.
*
* Note(s)    : none.
************************************************************************************************************************
*/

void  OS_CSP_TickHandler (void *p_arg)
{
  extern volatile unsigned int timestamp;
  unsigned int status;
  //CPU_INT32U val = CSP_TmrIntClr(CSP_TMR_NBR_SYS_TICK);
  status = PIT_GetStatus() & PIT_SR_PITS;
  if(status) {
    timestamp += (PIT_GetPIVR() >> 20);
    
    OSTimeTick();                                               /* Tell uC/OS-III about clock tick                     */
  }
}


/*$PAGE*/
/*
********************************************************************************************************
*                                          OS_CPU_ExceptHndlr()
*
* Description : Handle any exceptions.
*
* Argument(s) : except_id     ARM exception type:
*
*                                  OS_CPU_ARM_EXCEPT_RESET             0x00
*                                  OS_CPU_ARM_EXCEPT_UNDEF_INSTR       0x01
*                                  OS_CPU_ARM_EXCEPT_SWI               0x02
*                                  OS_CPU_ARM_EXCEPT_PREFETCH_ABORT    0x03
*                                  OS_CPU_ARM_EXCEPT_DATA_ABORT        0x04
*                                  OS_CPU_ARM_EXCEPT_ADDR_ABORT        0x05
*                                  OS_CPU_ARM_EXCEPT_IRQ               0x06
*                                  OS_CPU_ARM_EXCEPT_FIQ               0x07
*
* Return(s)   : none.
*
* Caller(s)   : OS_CPU_ARM_EXCEPT_HANDLER(), which is declared in os_cpu_a.s.
*
* Note(s)     : (1) These function implements the hadnlers for the ARM interrupt request (IRQ) and 
*                   Fast-Interrupt request (FIQ) for the AT91SAM9xxx series which use the Advance Interrupt Controller (AIC).
*
*               (2) The behavior of the other ARM exeptions (RESET, UNDEF, SWI, PREFETCH_ABORT, DATA_ABORT and ADDR_ABORT)
*                   are application dependent and it should be implemented in the Board Support Package.
*
*               (3) The OS_BSP_ExceptHandler() must be implemented in the Board support package                  
*********************************************************************************************************
*/

void  OS_CPU_ExceptHndlr (CPU_INT32U  except_id)
{
  
  switch (except_id) {
  case OS_CPU_ARM_EXCEPT_IRQ:
  case OS_CPU_ARM_EXCEPT_FIQ:
    CSP_IntHandlerSrc((CSP_DEV_NBR)except_id);
    break;
    
  default:
  case OS_CPU_ARM_EXCEPT_RESET:
  case OS_CPU_ARM_EXCEPT_UNDEF_INSTR:
  case OS_CPU_ARM_EXCEPT_SWI:
  case OS_CPU_ARM_EXCEPT_PREFETCH_ABORT:
  case OS_CPU_ARM_EXCEPT_DATA_ABORT:
    OS_CSP_BSP_ExceptHandler((CPU_INT08U)except_id);
    break;
  }
}
