/*
*********************************************************************************************************
*                                     MICRIUM BOARD SUPPORT SUPPORT
*
*                          (c) Copyright 2003-2009; Micrium, Inc.; Weston, FL
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
*                                    MICRIUM BOARD SUPPORT PACKAGE
*                                             OS LAYER
*
* Filename      : bsp_os.c
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_OS_MODULE
#include <bsp.h>

#if (APP_CFG_PROBE_COM_EN == DEF_ENABLED)
#include <app_probe.h>
#endif

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
                                                                /* ----- PERIODIC INTERVAL TIMER (PIT) REGISTERS ---- */
#define  BSP_OS_REG_PIT_BASE_ADDR                  ((CPU_INT32U)0xFFFFFD30)
#define  BSP_OS_REG_PIT_MR                (*(CPU_REG32 *)(BSP_OS_REG_PIT_BASE_ADDR + 0x00))
#define  BSP_OS_REG_PIT_SR                (*(CPU_REG32 *)(BSP_OS_REG_PIT_BASE_ADDR + 0x04))
#define  BSP_OS_REG_PIT_PIVR              (*(CPU_REG32 *)(BSP_OS_REG_PIT_BASE_ADDR + 0x08))
#define  BSP_OS_REG_PIT_PIIR              (*(CPU_REG32 *)(BSP_OS_REG_PIT_BASE_ADDR + 0x0C))

                                                                /* --------- PIT MODE REGISTER BIT DEFINES ---------- */

#define  BSP_OS_PIT_MR_PIV_VAL_MASK                      DEF_BIT_FIELD(20, 0)
                                                                /* Period interval timer enable                       */
#define  BSP_OS_PIT_MR_PITEN                             DEF_BIT_24

                                                                /* Period interval timer interrupt enable             */
#define  BSP_OS_PIT_MR_PITIEN                            DEF_BIT_25

                                                                /* --------- PIT STATUS REGISTER BIT DEFINES -------- */
#define  BSP_OS_PIT_SR_PITS                              DEF_BIT_00

                                                                /* --------- TIMER COUNTER BASE ADDR DEFINES -------- */
#define  BSP_OS_TC0_BASE_REG              ((BSP_OS_TMR_REG_PTR)(0xFFF7C000))
#define  BSP_OS_TC1_BASE_REG              ((BSP_OS_TMR_REG_PTR)(0xFFF7C040))
#define  BSP_OS_TC2_BASE_REG              ((BSP_OS_TMR_REG_PTR)(0xFFF7C080))

                                                                /* --------- DEBUG MODULE REGISTER DEFINES ---------- */
#define  BSP_OS_DBGU_BASE_ADDR                   ((CPU_INT32U)0xFFFFEE00)
#define  BSP_OS_DBGU_IMR                  (*(CPU_REG32 *)(BSP_OS_DBGU_BASE_ADDR  + 0x0010))
#define  BSP_OS_DBGU_SR                   (*(CPU_REG32 *)(BSP_OS_DBGU_BASE_ADDR  + 0x0014))


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

typedef  struct bsp_os_tmr_reg {
    CPU_REG32  CCR;                                             /* Channel Control Register                           */
    CPU_REG32  CMR;                                             /* Channel Mode Register                              */
    CPU_REG32  RESERVED0[2];
    CPU_REG32  CV;                                              /* Counter Value                                      */
    CPU_REG32  RA;                                              /* Register A                                         */
    CPU_REG32  RB;                                              /* Register B                                         */
    CPU_REG32  RC;                                              /* Register C                                         */
    CPU_REG32  SR;                                              /* Status Register                                    */
    CPU_REG32  IER;                                             /* Interrupt Enable register                          */
    CPU_REG32  IDR;                                             /* Interrupt Disable register                         */
    CPU_REG32  IMR;                                             /* Interrupt Mask Register                            */
} BSP_OS_TMR_REG, *BSP_OS_TMR_REG_PTR;




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

static  void  BSP_OS_SysISR_Handler (void);

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
* Note(s)     : (1) Only OS_CPU_ARM_EXCEPT_FIQ and OS_CPU_ARM_EXCEPT_IRQ exceptions handler are implemented. 
*                   For the rest of the exception a infinite loop is implemented for debuging pruposes. This behavior
*                   should be replaced with another behavior (reboot, etc).
*********************************************************************************************************
*/

void  OS_CPU_ExceptHndlr (CPU_INT32U  except_id)
{
   CPU_INT32U    *p_sp;
   
   
    switch (except_id) {
        case OS_CPU_ARM_EXCEPT_FIQ:   
             BSP_IntSched(BSP_INT_CTRL_TYPE_FIQ);
             break;
             
       
       case OS_CPU_ARM_EXCEPT_IRQ:    
             BSP_IntSched(BSP_INT_CTRL_TYPE_IRQ);
             break;
                        
 
        case OS_CPU_ARM_EXCEPT_RESET:
             /* $$$$ Insert code to handle a Reset exception               */
        
        case OS_CPU_ARM_EXCEPT_UNDEF_INSTR:
            /* $$$$ Insert code to handle a Undefine Instruction exception */ 

        case OS_CPU_ARM_EXCEPT_SWI:               
            /* $$$$ Insert code to handle a Software exception             */ 
        
        case OS_CPU_ARM_EXCEPT_PREFETCH_ABORT:
            /* $$$$ Insert code to handle a Prefetch Abort exception       */ 
        
        case OS_CPU_ARM_EXCEPT_DATA_ABORT:        
            /* $$$$ Insert code to handle a Data Abort exception           */ 
        
        case OS_CPU_ARM_EXCEPT_ADDR_ABORT:        
            /* $$$$ Insert code to handle a Address Abort exception        */ 
        default:        
            if (OSIntNesting == 1) {
                p_sp = (CPU_INT32U *)OS_CPU_ExceptStkPtr;
            } else {
                p_sp = (CPU_INT32U *)OSTCBCur->OSTCBStkPtr;
            }
            
            /*
            TRACE_INFO(("\nCPU_ARM_EXCEPTION #%d trapped.\n", except_id));            
            TRACE_INFO(("R0  : 0x%08x\n", *(p_sp + 0x01)));
            TRACE_INFO(("R1  : 0x%08x\n", *(p_sp + 0x02)));
            TRACE_INFO(("R2  : 0x%08x\n", *(p_sp + 0x03)));
            TRACE_INFO(("R3  : 0x%08x\n", *(p_sp + 0x04)));
            TRACE_INFO(("R4  : 0x%08x\n", *(p_sp + 0x05)));
            TRACE_INFO(("R5  : 0x%08x\n", *(p_sp + 0x06)));
            TRACE_INFO(("R6  : 0x%08x\n", *(p_sp + 0x07)));
            TRACE_INFO(("R7  : 0x%08x\n", *(p_sp + 0x08)));
            TRACE_INFO(("R8  : 0x%08x\n", *(p_sp + 0x09)));
            TRACE_INFO(("R9  : 0x%08x\n", *(p_sp + 0x0A)));
            TRACE_INFO(("R10 : 0x%08x\n", *(p_sp + 0x0B)));
            TRACE_INFO(("R11 : 0x%08x\n", *(p_sp + 0x0C)));
            TRACE_INFO(("R12 : 0x%08x\n", *(p_sp + 0x0D)));
            TRACE_INFO(("SP  : 0x%08x\n",   p_sp));
            TRACE_INFO(("LR  : 0x%08x\n", *(p_sp + 0x0E)));
            TRACE_INFO(("PC  : 0x%08x\n", *(p_sp + 0x0F)));
            TRACE_INFO(("CPSR: 0x%08x\n", *(p_sp + 0x00)));
            */
            while (DEF_TRUE) {                                  /* Infinite loop on other exceptions. (see note #1)          */
                ;                                               
            }
    }      
}

/*
*********************************************************************************************************
*********************************************************************************************************
*                             uC/Probe PLUG-IN FOR uC/OS-II FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       OSProbe_TmrInit()
*
* Description : Select & initialize a timer for use with the uC/Probe Plug-In for uC/OS-II.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

#if (APP_CFG_PROBE_OS_PLUGIN_EN == DEF_ENABLED) && \
    (OS_PROBE_HOOKS_EN          == 1          )
void  OSProbe_TmrInit (void)
{
#if (OS_PROBE_TIMER_SEL == 0)
    BSP_PerClkEn(BSP_PER_ID_TC0);                               /* Enable the peripheral clk                          */
    BSP_OS_TC0_BASE_REG->CCR = DEF_BIT_01;

    DEF_BIT_CLR(BSP_OS_TC0_BASE_REG->CMR, DEF_BIT_FIELD(3, 0)); /* TIMER_CLOCK1 is input clk                          */
                
    BSP_OS_TC0_BASE_REG->CCR = DEF_BIT_00;                      /* TC0 timer enabled                                  */ 
    BSP_OS_TC0_BASE_REG->CCR = DEF_BIT_02;                      /* SWTRG to reset and start                           */
#endif

#if (OS_PROBE_TIMER_SEL == 1)
    BSP_PerClkEn(BSP_PER_ID_TC1);                               /* Enable the peripheral clk                          */
    BSP_OS_TC1_BASE_REG->CCR = DEF_BIT_01;

    DEF_BIT_CLR(BSP_OS_TC1_BASE_REG->CMR, DEF_BIT_FIELD(3, 0)); /* TIMER_CLOCK1 is input clk                          */
                
    BSP_OS_TC1_BASE_REG->CCR = DEF_BIT_00;                      /* TC1 timer enabled                                  */ 
#endif

#if (OS_PROBE_TIMER_SEL == 2)
    BSP_PerClkEn(BSP_PER_ID_TC2);                               /* Enable the peripheral clk                          */
    BSP_OS_TC2_BASE_REG->CCR = DEF_BIT_01;

    DEF_BIT_CLR(BSP_OS_TC2_BASE_REG->CMR, DEF_BIT_FIELD(3, 0)); /* TIMER_CLOCK1 is input clk                          */
                
    BSP_OS_TC2_BASE_REG->CCR = DEF_BIT_00;                      /* TC2 timer enabled                                  */ 
#endif

#if (OS_PROBE_TIMER_SEL == 3)
    BSP_PerClkEn(BSP_PER_ID_TC0);                               /* Enable the peripheral clk                          */
    BSP_OS_TC3_BASE_REG->CCR = DEF_BIT_01;

    DEF_BIT_CLR(BSP_OS_TC3_BASE_REG->CMR, DEF_BIT_FIELD(3, 0)); /* TIMER_CLOCK1 is input clk                          */
                
    BSP_OS_TC3_BASE_REG->CCR = DEF_BIT_00;                      /* TC3 timer enabled                                  */ 
#endif

#if (OS_PROBE_TIMER_SEL == 4)
    BSP_PerClkEn(BSP_PER_ID_TC4);                               /* Enable the peripheral clk                          */
    BSP_OS_TC4_BASE_REG->CCR = DEF_BIT_01;

    DEF_BIT_CLR(BSP_OS_TC4_BASE_REG->CMR, DEF_BIT_FIELD(3, 0)); /* TIMER_CLOCK1 is input clk                          */
                
    BSP_OS_TC4_BASE_REG->CCR = DEF_BIT_00;                      /* TC4 timer enabled                                  */ 
#endif

#if (OS_PROBE_TIMER_SEL == 5)
    BSP_PerClkEn(BSP_PER_ID_TC5);                               /* Enable the peripheral clk                          */
    BSP_OS_TC5_BASE_REG->CCR = DEF_BIT_01;

    DEF_BIT_CLR(BSP_OS_TC5_BASE_REG->CMR, DEF_BIT_FIELD(3, 0)); /* TIMER_CLOCK1 is input clk                          */
                
    BSP_OS_TC5_BASE_REG->CCR = DEF_BIT_00;                      /* TC5 timer enabled                                  */ 
#endif
}
#endif


/*
*********************************************************************************************************
*                                        OSProbe_TmrRd()
*
* Description : Read the current counts of a 16-bit free running timer.
*
* Argument(s) : none.
*
* Return(s)   : The 16 bit counts (in a 32 bit variable) of the timer.
*********************************************************************************************************
*/

#if (APP_CFG_PROBE_OS_PLUGIN_EN == DEF_ENABLED) && \
    (OS_PROBE_HOOKS_EN          == 1          )
CPU_INT32U  OSProbe_TmrRd (void)
{
    CPU_INT32U  cnts;


#if (OS_PROBE_TIMER_SEL == 0)
    cnts = (CPU_INT32U)(BSP_OS_TC0_BASE_REG->CV & 0x0000FFFF);  /* Read timer 0                                       */
#endif

#if (OS_PROBE_TIMER_SEL == 1)
    cnts = (CPU_INT32U)(BSP_OS_TC1_BASE_REG->CV & 0x0000FFFF);  /* Read timer 1                                       */
#endif

#if (OS_PROBE_TIMER_SEL == 2)
    cnts = (CPU_INT32U)(BSP_OS_TC2_BASE_REG->CV & 0x0000FFFF);  /* Read timer 2                                       */
#endif

#if (OS_PROBE_TIMER_SEL == 3)
    cnts = (CPU_INT32U)(BSP_OS_TC3_BASE_REG->CV & 0x0000FFFF);  /* Read timer 3                                       */
#endif

#if (OS_PROBE_TIMER_SEL == 4)
    cnts = (CPU_INT32U)(BSP_OS_TC4_BASE_REG->CV & 0x0000FFFF);  /* Read timer 4                                       */
#endif

#if (OS_PROBE_TIMER_SEL == 5)
    cnts = (CPU_INT32U)(BSP_OS_TC5_BASE_REG->CV & 0x0000FFFF);   /* Read timer 5                                       */
#endif

    return (cnts);
}
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
**                                     BSP OS LOCKS FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      BSP_OS_SemCreate()
*
* Description : Creates a sempahore to lock/unlock
*
* Argument(s) : p_sem        Pointer to a BSP_OS_SEM structure
*  
*               sem_val      Initial value of the semaphore.
*
*               p_sem_name   Pointer to the semaphore name.
*
* Return(s)   : DEF_OK       if the semaphore was created.
*               DEF_FAIL     if the sempahore could not be created.
*     
* Caller(s)   : Application.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemCreate (BSP_OS_SEM       *p_sem,
                               BSP_OS_SEM_VAL    sem_val,
                               CPU_CHAR         *p_sem_name)
{
    OS_EVENT    *p_event;

#if (OS_EVENT_NAME_EN > 0)
    CPU_INT08U  err;
#endif
    
    p_event = OSSemCreate(sem_val);
    
    if (p_event == (OS_EVENT *)0) {
        return (DEF_FAIL);    
    }
    
    *p_sem = (BSP_OS_SEM)(p_event);
    
#if (OS_EVENT_NAME_EN > 0)
    OSEventNameSet((OS_EVENT *)p_event, 
                   (INT8U    *)p_sem_name, 
                   (INT8U    *)&err);
#endif
    

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                     BSP_OS_SemWait()
*
* Description : Wait on a semaphore to become available
*
* Argument(s) : sem          sempahore handler
*  
*               dly_ms       delay in miliseconds to wait on the semaphore
*
* Return(s)   : DEF_OK       if the semaphore was acquire
*               DEF_FAIL     if the sempahore could not be acquire
*     
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemWait (BSP_OS_SEM  sem,
                             CPU_INT32U  dly_ms)
{
    CPU_INT08U  err;
    CPU_INT32U  dly_ticks;
    
    dly_ticks  = ((dly_ms * DEF_TIME_NBR_mS_PER_SEC) / OS_TICKS_PER_SEC);
    
    OSSemPend((OS_EVENT   *)sem,
              (CPU_INT32U  )dly_ticks,
              (CPU_INT08U *)&err);

    if (err != OS_ERR_NONE) {
       return (DEF_FAIL);
    }

    return (DEF_OK);
}

/*
*********************************************************************************************************
*                                      BSP_OS_SemPost()
*
* Description : Post a semaphore
*
* Argument(s) : sem          Semaphore handler
*  
* Return(s)   : DEF_OK     if the semaphore was posted.
*               DEF_FAIL      if the sempahore could not be posted.
*     
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemPost (BSP_OS_SEM  sem)
{
    CPU_INT08U  err;
        
    err = OSSemPost((OS_EVENT *)sem);
    
    if (err != OS_ERR_NONE) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*********************************************************************************************************
**                                     uC/OS-II TIMER FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                    BSP_TimeDlyMs()
*
* Description : This function delay the exceution for specifi amount of miliseconds
*
* Argument(s) : dly_ms       Delay in miliseconds
*
* Return(s)   : none.
*
* Caller(s)   : Application
*
* Note(s)     : none.
*               
*********************************************************************************************************
*/

void   BSP_OS_TimeDlyMs (CPU_INT32U  dly_ms)
{
    CPU_INT16U  os_ms;
    CPU_INT08U  os_sec;
    

    if (dly_ms > 10000) {                                       /* Limit delays to 10 seconds.                        */
        dly_ms = 10000;
    }

    os_sec = dly_ms / 1000;
    os_ms  = dly_ms % 1000;

    OSTimeDlyHMSM(0, 0, os_sec, os_ms);    
}


/*
*********************************************************************************************************
*                                            BSP_OS_TmrTickInit()
*
* Description : Initialize uC/OS-II's tick source; for the AT91SAM, the Periodic Interval Timer (PIT).
*
* Argument(s) : ticks_per_sec              Number of ticks per second.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) PIT Interrupt frequency:
*
*                             MCLK        1
*                   Freq =    ---- * -----------
*                              16     (PIV + 1)
*
*
*                             MCLK      1
*                   PIV  =  ( ---- * ------ ) - 1
*                              16     Freq
*
*                   Where:
*
*                       MCLK = Master Clock
*                       Freq = Desired frequency.
*********************************************************************************************************
*/

void  BSP_OS_TmrTickInit (CPU_INT32U  tick_per_sec)
{
    CPU_INT32U  cnts;
    CPU_INT32U  mclk_freq;

                                                                /* Determine the number of counts per tick.           */
    mclk_freq = BSP_CPU_MclkFreq();

    cnts      = ((mclk_freq) / 16 / tick_per_sec) - 1;

                                                                /* Set the vector address for PIT.                    */
    BSP_IntClr(BSP_PER_ID_SYSC);

    BSP_IntVectSet((CPU_INT08U   )BSP_PER_ID_SYSC,
                   (CPU_INT08U   )BSP_INT_PRIO_LOWEST,
                   (CPU_INT08U   )BSP_INT_SCR_TYPE_INT_HIGH_LEVEL_SENSITIVE,
                   (CPU_FNCT_VOID)BSP_OS_SysISR_Handler );

    BSP_IntEn(BSP_PER_ID_SYSC);

                                                                /* Enable the PIT with the correct compare value.     */
    BSP_OS_REG_PIT_MR = BSP_OS_PIT_MR_PITEN
                      | BSP_OS_PIT_MR_PITIEN
                      | cnts;
}


/*
*********************************************************************************************************
*                                           BSP_TmrTickISR_Handler()
*
* Description : This function handles the PIT interrupt that is used to generate TICKs for uC/OS-II.
*
* Argument(s) : none
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Sys_Int_Handler()
*
* Note(s)     : none.              
*********************************************************************************************************
*/

static  void  BSP_OS_TmrTickISR_Handler (void)
{
    CPU_INT32U  status;
    
                                                                /* Clear the PIT interrupt                            */
    status = BSP_OS_REG_PIT_PIVR;

    (void)status;                                               /* Prevent a compiler warning that status was ...     */
                                                                /* ... never used                                     */

    OSTimeTick();                                               /* Tell uC/OS-II about clock tick                     */

}


/*
*********************************************************************************************************
*                                         BSP_OS_SysISR_Handler()
*
* Description : Handle SYS peripheral interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : (1) The interrupts from several devices are multiplexed for the SYS interrupt.  This
*                   handler currently handles the DBGU and PIT interrupts.
*
*               (2) AIC interrupts are cleared in this function; each handler called by this function
*                   need only clear its local source.
*********************************************************************************************************
*/

static  void  BSP_OS_SysISR_Handler (void)
{
    if (DEF_BIT_IS_SET(BSP_OS_REG_PIT_SR, BSP_OS_PIT_SR_PITS)) {
        BSP_OS_TmrTickISR_Handler();
    }
        
    BSP_IntClr(BSP_PER_ID_SYSC);
}
