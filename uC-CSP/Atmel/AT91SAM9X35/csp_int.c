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
*                                         INTERRUPT MANAGEMENT
*                                          ATMEL  AT91SAM9M10
*
* Filename      : csp_int.c
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

#define    CSP_INT_MODULE
#include   <csp.h>


/*
*********************************************************************************************************
*                                          DEFAULT CONFIGURATION
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
 
#define  CSP_INT_ADDR_AIC                       (CPU_INT32U)(0xFFFFF000)

#define  CSP_INT_REG_AIC_SMRx(src_nbr)          (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + (src_nbr * 4u) + 0x000))
#define  CSP_INT_REG_AIC_SVRx(src_nbr)          (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + (src_nbr * 4u) + 0x080))
#define  CSP_INT_REG_AIC_IVR                    (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + 0x100))
#define  CSP_INT_REG_AIC_FVR                    (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + 0x104))
#define  CSP_INT_REG_AIC_ISR                    (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + 0x108))
#define  CSP_INT_REG_AIC_IPR                    (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + 0x10C))
#define  CSP_INT_REG_AIC_IMR                    (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + 0x110))
#define  CSP_INT_REG_AIC_CISR                   (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + 0x114))
#define  CSP_INT_REG_AIC_IECR                   (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + 0x120))
#define  CSP_INT_REG_AIC_IDCR                   (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + 0x124))
#define  CSP_INT_REG_AIC_ICCR                   (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + 0x128))
#define  CSP_INT_REG_AIC_ISCR                   (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + 0x12C))
#define  CSP_INT_REG_AIC_EOICR                  (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + 0x130))
#define  CSP_INT_REG_AIC_SPU                    (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + 0x134))
#define  CSP_INT_REG_AIC_DCR                    (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + 0x138))
#define  CSP_INT_REG_AIC_FFER                   (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + 0x140))
#define  CSP_INT_REG_AIC_FFDR                   (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + 0x144))
#define  CSP_INT_REG_AIC_FFSR                   (*(CPU_REG32 *)(CSP_INT_ADDR_AIC + 0x148))

#define  CSP_INT_REG_DBGU_IDR                   (*(CPU_REG32 *)(0xFFFFF20C))
#define  CSP_INT_REG_DBGU_IMR                   (*(CPU_REG32 *)(0xFFFFF210))
#define  CSP_INT_REG_DBGU_SR                    (*(CPU_REG32 *)(0xFFFFF214))


//#define  CSP_INT_REG_RTT_MR                     (*(CPU_REG32 *)(0xFFFFFD20))
//#define  CSP_INT_REG_RTT_SR                     (*(CPU_REG32 *)(0xFFFFFD2C))

#define  CSP_INT_REG_PIT_MR                     (*(CPU_REG32 *)(0xFFFFFE30))
#define  CSP_INT_REG_PIT_SR                     (*(CPU_REG32 *)(0xFFFFFE34))
#define  CSP_INT_REG_PIT_PIVR                   (*(CPU_REG32 *)(0xFFFFFE3C))


#define  CSP_INT_REG_RTC_IMR                    (*(CPU_REG32 *)(0xFFFFFED8))
#define  CSP_INT_REG_RTC_SR                     (*(CPU_REG32 *)(0xFFFFFEC8))
#define  CSP_INT_REG_RTC_IDR                    (*(CPU_REG32 *)(0xFFFFFED4))

#define  CSP_INT_REG_WDT_MR                     (*(CPU_REG32 *)(0xFFFFFE40))
#define  CSP_INT_REG_WDT_SR                     (*(CPU_REG32 *)(0xFFFFFE48))

#define  CSP_INT_REG_RSTC_SR                    (*(CPU_REG32 *)(0xFFFFFD40))
#define  CSP_INT_REG_RSTC_MR                    (*(CPU_REG32 *)(0xFFFFFD48))

#define  CSP_INT_REG_PMC_IMR                    (*(CPU_REG32 *)(0xFFFFFC6C))
#define  CSP_INT_REG_PMC_SR                     (*(CPU_REG32 *)(0xFFFFFC68))
#define  CSP_INT_REG_PMC_IDR                    (*(CPU_REG32 *)(0xFFFFFC64))

#if ((CSP_CFG_INT_ISR_ARG_EN       == DEF_ENABLED) || \
    ((CSP_CFG_INT_ISR_EXEC_MEAS_EN == DEF_ENABLED) && \
     (CPU_CFG_TS_EN                == DEF_ENABLED)))
#define CSP_INT_AIC_VECT_EN                     DEF_ENABLED
#else
#define CSP_INT_AIC_VECT_EN                     DEF_DISABLED
#endif


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL DATA TYPES
*********************************************************************************************************
*/

#if (CSP_INT_AIC_VECT_EN == DEF_ENABLED)
typedef  struct  csp_int_aic_vect {    
#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)    
    void          *ArgPtr;                                      /* ISR function Argument pointer.                       */
#endif    
#if (CSP_CFG_INT_ISR_EXEC_MEAS_EN == DEF_ENABLED) && \
    (CPU_CFG_TS_EN                == DEF_ENABLED)
    CPU_TS32       TimeMax;                                     /* ISR maximum execution time.                          */
    CPU_TS32       TimeCur;                                     /* ISR current execution time.                          */
#endif    
} CSP_INT_AIC_VECT;
#endif

/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     INTERRUPT VECTOR TABLE
*
* Note(s) (1) 'CSP_MainVectTbl[]' stores all the interrupts handlers for the AIC interrupt.
*
*         (2) 'CSP_SysVectTbl[]' stores all the interrupts handlers for the shared system interrupt handlers
*********************************************************************************************************
*/

#if (CSP_INT_AIC_VECT_EN == DEF_ENABLED)
static  CSP_INT_AIC_VECT  CSP_MainVectTbl[CSP_INT_SRC_NBR_MAX];     /* AIC interrupt vector table                        */
#endif

static  CSP_INT_VECT      CSP_SysVectTbl[CSP_INT_SYS_SRC_NBR_MAX];  /* System interrupt vector table.                    */


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

static  void  CSP_IntSysHandler (void *p_arg);


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
*                                            CSP_IntClr()
*
* Description : Clear an interrupt source on a specific interrupt controller.
*
* Argument(s) : int_ctrl   Interrupt controller number (see note #1).
*
*               src_nbr    Interrupt source number (see note #2). 
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Interrupt controllers numbers identifiers can be found in 'csp_grp.h' 
* 
*                       CSP_INT_CTRL_NBR_XX'  where 'XX" is the name of the interrupt controller.
*
*               (2) Interrupt source identifiers can be found in 'csp_grp.h' as CSP_INT
*
*                       CSP_INT_SRC_NBR_XX      where XX is the peripheral or interrupt source name.
*                                               Main interrupt controller only.
* 
*                       CSP_INT_XX_SRC_NBR_YY   where xx is the name of the interrupt controller or shared 
*                                               handler and YY is the peripheral or source name.
*
*               (3) Two interrupts controllers are defined for this family definition:
*
*                       CSP_INT_CTRL_NBR_MAIN   AIC interrupt controller.
*                       CSP_INT_CTRL_NBR_SYS    Shared system interrupt handler.
*********************************************************************************************************
*/

void  CSP_IntClr (CSP_DEV_NBR  int_ctrl,
                  CSP_DEV_NBR  src_nbr)                  
{
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (int_ctrl != CSP_INT_CTRL_NBR_MAIN) {
        return;
    }
    
    if (src_nbr > (CSP_INT_SRC_NBR_MAX - 1u)) {
        return;
    }
#endif

    CSP_INT_REG_AIC_ICCR = DEF_BIT(src_nbr);
}


/*
*********************************************************************************************************
*                                          CSP_IntDis()
*
* Description : Disable an interrupt source on a specific interrupt controller.
*
* Argument(s) : int_ctrl   Interrupt controller number. (see 'CSP_IntClr()' note #1).
*
*               src_nbr    Interrupt source number. (see 'CSP_IntClr()' note #2).
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  CSP_IntDis (CSP_DEV_NBR  int_ctrl,
                  CSP_DEV_NBR  src_nbr)                   
{ 
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (int_ctrl != CSP_INT_CTRL_NBR_MAIN) {
        return;
    }
    
    if (src_nbr > (CSP_INT_SRC_NBR_MAX - 1u)) {
        return;
    }
#endif
    
    CSP_INT_REG_AIC_IDCR = DEF_BIT(src_nbr);
}


/*
*********************************************************************************************************
*                                          CSP_IntDisAll()
*
* Description : Disable all interrupts on a specific interrupt controller.
*
* Argument(s) : int_ctrl   Interrupt controller number. (see 'CSP_IntClr()' note #1).
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  CSP_IntDisAll (CSP_DEV_NBR  int_ctrl)

{
    if (int_ctrl == CSP_INT_CTRL_NBR_MAIN) {
        CSP_INT_REG_AIC_IDCR = DEF_BIT_FIELD(32u, 0u);
    }
}


/*
*********************************************************************************************************
*                                          CSP_IntEn()
*
* Description : Enable an interrupt source on a specific interrupt controller.
*
* Argument(s) : int_ctrl   Interrupt controller number. (see 'CSP_IntClr()' note #1).
*
*               src_nbr    Interrupt source number. (see 'CSP_IntClr()' note #2).
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  CSP_IntEn (CSP_DEV_NBR  int_ctrl,
                 CSP_DEV_NBR  src_nbr)                 
{
#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (int_ctrl != CSP_INT_CTRL_NBR_MAIN) {
        return;
    }
    
    if (src_nbr > (CSP_INT_SRC_NBR_MAX - 1u)) {
        return;
    }
#endif
    
    CSP_INT_REG_AIC_IECR = DEF_BIT(src_nbr);
}


/*
*********************************************************************************************************
*                                          CSP_IntInit()
*
* Description : Initiailze all interrupt controllers.
*                   (1) Disable all interrupts.
*                   (2) Clear all pending interrupts.
*                   (3) Initialize all vector interrupt tables.
*                   (4) Install all shared interrupt handlers. 
*
* Argument(s) : None.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  CSP_IntInit  (void)
{
    CPU_INT08U         src_nbr;
    CSP_INT_VECT      *p_int_vect;
#if (CSP_INT_AIC_VECT_EN == DEF_ENABLED)
    CSP_INT_AIC_VECT  *p_int_aic_vect;
#endif
    CPU_SR_ALLOC();
    
    
    CSP_INT_REG_AIC_IDCR  = DEF_BIT_FIELD(32u, 0u);             /* Disable all interrupts.                              */
    CSP_INT_REG_AIC_ICCR  = DEF_BIT_FIELD(32u, 0u);             /* Clear all pending interrupts.                        */
    CSP_INT_REG_AIC_SPU   = 0u;                                 /* Set spurios interrupt vector to NULL                 */
    CSP_INT_REG_AIC_EOICR = 0u;                                 /* Set end of interrupt signal.                         */
    
                                                                /* Disable & Clear all pennding system interrupts.      */
    CSP_INT_REG_DBGU_IDR  = DEF_BIT_FIELD(32u, 0u);             /* ... Disable debug interrupt.                         */
    CSP_INT_REG_RTC_IDR   = DEF_BIT_FIELD(32u, 0u);             /* ... Disable real-time clock interrupt.               */
    //DEF_BIT_CLR(CSP_INT_REG_RTT_MR, DEF_BIT_16 | DEF_BIT_17);   /* ... Disable real-time timer interrupt.               */

    DEF_BIT_CLR(CSP_INT_REG_PIT_MR, DEF_BIT_25);                /* ... Disable PIT interrupt.                           */    
    (void)CSP_INT_REG_PIT_PIVR;                                 /* ... Clear pending interrupts.                        */
    
    DEF_BIT_CLR(CSP_INT_REG_RSTC_MR, 0xA5000000 | 
                                     DEF_BIT_04);               /* ... Disable user reset interrupt.                    */
    DEF_BIT_CLR(CSP_INT_REG_WDT_MR, DEF_BIT_12);
    CSP_INT_REG_PMC_IDR = DEF_BIT_FIELD(32u, 0u);               /* ... Disable power managment controller interrupt.    */
    
#if (CSP_INT_AIC_VECT_EN == DEF_ENABLED)
    p_int_aic_vect = &CSP_MainVectTbl[0];
#endif
    
    for (src_nbr = 0u; src_nbr < CSP_INT_SRC_NBR_MAX; src_nbr++) {
        CPU_CRITICAL_ENTER();
        CSP_INT_REG_AIC_SMRx(src_nbr) = DEF_BIT_NONE;
        CSP_INT_REG_AIC_SVRx(src_nbr) = 0u;                     /* Inialize all interrupt vectors to NULL.              */
                
#if (CSP_INT_AIC_VECT_EN == DEF_ENABLED)

#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)
        p_int_aic_vect->ArgPtr  = (void *)0;
#endif        

#if (CSP_CFG_INT_ISR_EXEC_MEAS_EN == DEF_ENABLED) && \
    (CPU_CFG_TS_EN                == DEF_ENABLED)
        p_int_aic_vect->TimeMax = (CPU_TS )0;
        p_int_aic_vect->TimeCur = (CPU_TS )0;         
#endif        
        p_int_aic_vect++;
#endif        
        CPU_CRITICAL_EXIT();     
    }
 
    for (src_nbr = 0u; src_nbr < CSP_INT_SYS_SRC_NBR_MAX; src_nbr++ ) {        
        p_int_vect = &CSP_SysVectTbl[src_nbr];
        CPU_CRITICAL_ENTER();                                   /* Initialize system shared handler the vector table.   */
        CSP_IntVectClr(p_int_vect);
        CPU_CRITICAL_EXIT();
    }
                                                                /* Install system interrupt shared handler.             */
    CSP_INT_REG_AIC_SMRx(CSP_INT_SRC_NBR_SYS) = 0u;              
    CSP_INT_REG_AIC_SVRx(CSP_INT_SRC_NBR_SYS) = (CPU_INT32U)CSP_IntSysHandler;
    CSP_INT_REG_AIC_ICCR                      = DEF_BIT(CSP_INT_SRC_NBR_SYS);
    CSP_INT_REG_AIC_IECR                      = DEF_BIT(CSP_INT_SRC_NBR_SYS);    
    CSP_INT_REG_AIC_EOICR                     = DEF_BIT_NONE;     
}

/*
*********************************************************************************************************
*                                            CSP_IntSysHandler()
*
* Description : System interrupt handler.            
*
* Argument(s) : p_arg    Interrupt ISR argument.
*
* Return(s)   : None.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : None.               
*********************************************************************************************************
*/

static  void  CSP_IntSysHandler (void  *p_arg)
{
    CPU_INT32U     int_stat;
    CPU_INT32U     int_msk;    
    CSP_INT_VECT  *p_int_vect;

    
                                                                /* Periodic Interval Timer status.                      */
    if ((DEF_BIT_IS_SET(CSP_INT_REG_PIT_MR, DEF_BIT_24)) &&
        (DEF_BIT_IS_SET(CSP_INT_REG_PIT_SR, DEF_BIT_00))) {
        p_int_vect = &CSP_SysVectTbl[CSP_INT_SYS_SRC_NBR_PIT];
        CSP_IntVectDeref(p_int_vect);
    }
    
    int_msk = CSP_INT_REG_DBGU_IMR;                             /* Debug unit interrupt.                                */
    if (int_msk != DEF_BIT_NONE) {
        int_stat  = CSP_INT_REG_DBGU_SR;
        int_stat &= int_msk;
        
        if (int_stat != DEF_BIT_NONE) {
            p_int_vect = &CSP_SysVectTbl[CSP_INT_SYS_SRC_NBR_DBG];
            CSP_IntVectDeref(p_int_vect);
        }
    }
#if 0
    int_msk = CSP_INT_REG_RTT_MR;                               /* Real-Time timer.                                     */
    int_msk = (int_msk >> 16u) & DEF_BIT_FIELD(2u, 0u);
    
    if (int_msk != DEF_BIT_NONE) {
        int_stat  = CSP_INT_REG_RTT_SR;
        int_stat &= int_msk;
            
        if (int_stat != DEF_BIT_NONE) {
            p_int_vect = &CSP_SysVectTbl[CSP_INT_SYS_SRC_NBR_RTT];
            CSP_IntVectDeref(p_int_vect);
        }
    }
#endif    
    int_msk = CSP_INT_REG_RTC_IMR;                              /* Real-Time clock.                                      */
    if (int_msk != DEF_BIT_NONE) {
        int_stat  = CSP_INT_REG_RTC_SR;
        int_stat &= int_msk;
        
        if (int_stat != DEF_BIT_NONE) {
            p_int_vect = &CSP_SysVectTbl[CSP_INT_SYS_SRC_NBR_RTC];
            CSP_IntVectDeref(p_int_vect);
        }
    }
                                         
    int_msk  = CSP_INT_REG_WDT_MR;                              /* Watchdog timer.                                       */
    int_stat = CSP_INT_REG_WDT_SR;
    
    if ((DEF_BIT_IS_SET(int_msk, DEF_BIT_12)) &&
        (DEF_BIT_IS_SET_ANY(int_stat, (DEF_BIT_01 | DEF_BIT_00)))) {        
        p_int_vect = &CSP_SysVectTbl[CSP_INT_SYS_SRC_NBR_WDT];        
        CSP_IntVectDeref(p_int_vect);
    }
                                                                
    int_msk = CSP_INT_REG_RSTC_MR;                              /* Reset controller.                                    */
    if (int_msk != DEF_BIT_NONE) {
        int_stat  = CSP_INT_REG_RSTC_SR;
        int_stat &= int_msk;
    
        if (int_stat != DEF_BIT_NONE) {
            p_int_vect = &CSP_SysVectTbl[CSP_INT_SYS_SRC_NBR_RST_CTRL];
            CSP_IntVectDeref(p_int_vect);
        }
    }           
                                                                /* Power management controller.                         */
    int_msk = CSP_INT_REG_PMC_IMR;
    if (int_msk != DEF_BIT_NONE) {
        int_stat  = CSP_INT_REG_PMC_SR;
        int_stat &= int_msk;
        
        if (int_stat != DEF_BIT_NONE) {
            p_int_vect = &CSP_SysVectTbl[CSP_INT_SYS_SRC_NBR_PMC];
            CSP_IntVectDeref(p_int_vect);
        }    
    }
}


/*
*********************************************************************************************************
*                                          CSP_IntSrcCfg()
*
* Description : Configure an interrupt source.
*
* Argument(s) : int_ctrl   Interrupt controller number. (see 'CSP_IntClr()' note #1).
*
*               src_nbr    Interrupt source number. (see 'CSP_IntClr()' note #2).
*
*               src_prio   Interrupt source priority.
*
*               src_pol    Interrupt source polarity:
*                              CSP_INT_POL_LEVEL_HIGH           Interrupt is activated on a high level.
*                              CSP_INT_POL_LEVEL_LOW            Interrupt is activated on a low level.
* 
*                              CSP_INT_POL_EDGE_RISING          Interrupt is activated on the rising edge.
*                              CSP_INT_POL_EDGE_FALLING         Interrupt is activated on the falling edge.
*                              CSP_INT_POL_EDGE_BOTH            Interrupt is activated on both edges.       
*
* Return(s)   : DEF_OK,    if the interrupt source was configured
*               DEF_FAIL,  otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Interrupt source priority is determined by the interrupt controller implementation.
*                   The maximum and minimum values are defined in the family definition file 'csp_grp.h'
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_IntSrcCfg (CSP_DEV_NBR  int_ctrl,
                            CSP_DEV_NBR  src_nbr,
                            CSP_OPT      src_prio,
                            CSP_OPT      src_pol)

{
    CPU_INT32U  reg_val;
    CPU_SR_ALLOC();

#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (int_ctrl != CSP_INT_CTRL_NBR_MAIN) {
        return (DEF_FAIL);
    }
    
    if (src_nbr > CSP_INT_SRC_NBR_MAX - 1u) {
        return (DEF_FAIL);
    }
    
    if (src_prio > 7u) {
        return (DEF_FAIL);
    }
#endif

    reg_val = 0u;
    
    switch (src_pol) {
        case CSP_INT_POL_LEVEL_HIGH:
             reg_val = DEF_BIT_MASK(2u, 5u);
             break;
             
        case CSP_INT_POL_EDGE_RISING:
             reg_val = DEF_BIT_MASK(3u, 5u);
             break;

        case CSP_INT_POL_EDGE_FALLING:
             reg_val = DEF_BIT_MASK(1u, 5u); 
             break;
             
        default:
             break;        
    }

    reg_val |= src_prio & DEF_BIT_FIELD(3u, 0u);
    
    CPU_CRITICAL_ENTER();
    CSP_INT_REG_AIC_SMRx(src_nbr) = reg_val;
    CPU_CRITICAL_EXIT();
    
    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                          CSP_IntVectReg()
*
* Description : Register an interrupt service routine handler for a specific interrupt controller.
*
* Argument(s) : int_ctrl    Interrupt controller number. (see 'CSP_IntClr()' note #1).
*
*               src_nbr     Interrupt source number. (see 'CSP_IntClr()' note #2).
*
*               isr_fnct    Pointer to the interrupt service routine (ISR).
*
*               p_arg       Pointer to the argument of the interrupt service routine (ISR).
*
* Return(s)   : DEF_OK,    If the vector was registered successfully.
*               DEF_FAIL,  otherwise.
*               
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_IntVectReg (CSP_DEV_NBR    int_ctrl,
                             CSP_DEV_NBR    src_nbr,
                             CPU_FNCT_PTR   isr_fnct,
                             void          *p_arg)
{
    CSP_INT_VECT  *p_int_vect;
    CPU_SR_ALLOC();

#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)    
    if (isr_fnct == (CPU_FNCT_PTR)0) {
        return (DEF_FAIL);
    }
    
    if ((int_ctrl == CSP_INT_CTRL_NBR_MAIN) &&
        (src_nbr   > CSP_INT_SRC_NBR_MAX - 1u)) {
        return (DEF_FAIL);
    }

    if ((int_ctrl == CSP_INT_CTRL_NBR_SYS     ) &&
        (src_nbr   > CSP_INT_SYS_SRC_NBR_MAX - 1u )) {
        return (DEF_FAIL);
    }
#endif    
    
    switch (int_ctrl) {
        case CSP_INT_CTRL_NBR_MAIN:                             /* Install interrupt ISR in the NVIC's vector table.    */
             CSP_INT_REG_AIC_SVRx(src_nbr) = (CPU_INT32U)isr_fnct;
             CPU_CRITICAL_ENTER();
#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)
             CSP_MainVectTbl[src_nbr].ArgPtr = p_arg;
#endif   

#if (CSP_CFG_INT_ISR_EXEC_MEAS_EN == DEF_ENABLED) && \
    (CPU_CFG_TS_EN                == DEF_ENABLED)
             CSP_MainVectTbl[src_nbr].TimeMax = (CPU_TS )0;
             CSP_MainVectTbl[src_nbr].TimeCur = (CPU_TS )0;         
#endif                     
             CPU_CRITICAL_EXIT();
             break;

        case CSP_INT_CTRL_NBR_SYS:                              /* Install interrupt ISR in the USB's vector table.     */
             p_int_vect = &CSP_SysVectTbl[src_nbr];
             CPU_CRITICAL_ENTER();

             CSP_IntVectSet((CSP_INT_VECT *)p_int_vect,
                            (CPU_FNCT_PTR  )isr_fnct,
                            (void *        )p_arg);

             CPU_CRITICAL_EXIT();

             break;
 
        default:
             return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                          CSP_IntVectUnreg()
*
* Description : Un-register an interrupt vector.
*
* Argument(s) : int_ctrl   Interrupt controller number. (see 'CSP_IntClr()' note #1).
*
*               src_nbr    Interrupt source number. (see 'CSP_IntClr()' note #2).
*
* Return(s)   : DEF_OK,     If the vector was unregistered successfully.
*               DEF_FAIL,   otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  CSP_IntVectUnreg (CSP_DEV_NBR  int_ctrl,
                               CSP_DEV_NBR  src_nbr)                         
{
    CSP_INT_VECT  *p_int_vect;
    CPU_SR_ALLOC();


#if (CSP_CFG_ARG_CHK_EN == DEF_ENABLED)    
    if ((int_ctrl == CSP_INT_CTRL_NBR_MAIN) &&
        (src_nbr   > CSP_INT_SRC_NBR_MAX - 1u)) {
        return (DEF_FAIL);
    }

    if ((int_ctrl == CSP_INT_CTRL_NBR_SYS     ) &&
        (src_nbr   > CSP_INT_SYS_SRC_NBR_MAX - 1u )) {
        return (DEF_FAIL);
    }
#endif     
    
    switch (int_ctrl) {
        case CSP_INT_CTRL_NBR_MAIN:                             /* Install interrupt ISR in the NVIC's vector table.    */
             CSP_INT_REG_AIC_SVRx(src_nbr) = (CPU_INT32U)0;

             CPU_CRITICAL_ENTER();
#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)
             CSP_MainVectTbl[src_nbr].ArgPtr = (void *)0;
#endif   

#if (CSP_CFG_INT_ISR_EXEC_MEAS_EN == DEF_ENABLED) && \
    (CPU_CFG_TS_EN                == DEF_ENABLED)
             CSP_MainVectTbl[src_nbr].TimeMax = (CPU_TS )0;
             CSP_MainVectTbl[src_nbr].TimeCur = (CPU_TS )0;         
#endif                     
             CPU_CRITICAL_EXIT();
             break;

        case CSP_INT_CTRL_NBR_SYS:                              /* Install interrupt ISR in the USB's vector table.     */
             p_int_vect = &CSP_SysVectTbl[src_nbr];

             CPU_CRITICAL_ENTER();
             CSP_IntVectClr(p_int_vect); 
             CPU_CRITICAL_EXIT();
             break;
 
        default:
             return (DEF_FAIL);
    }
    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                          CSP_IntHandler()
*
* Description : Global interrupt handler.
*
* Argument(s) : None.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function is not used.
*********************************************************************************************************
*/

void  CSP_IntHandler (void)
{
}

/*
*********************************************************************************************************
*                                          CSP_IntHandlerSrc()
*
* Description : Global interrupt controller handler (dispatcher).
*
* Argument(s) : src_nbr     ARM Interrupt number
*                               CPU_ARM_EXCEPT_IRQ   Interrupt request.
*                               CPU_ARM_EXCEPT_FIQ   Fast interrupt request.          
*
* Return(s)   : none.
*
* Caller(s)   : Global CPU Interrupt handler.
*
* Note(s)     : (1) This funtion should be called from a GENERIC Interrupt handler.
*********************************************************************************************************
*/

void  CSP_IntHandlerSrc  (CSP_DEV_NBR  src_nbr)
{
    CPU_FNCT_PTR       isr_fnct;
    CSP_INT_VECT       vect;
#if( CSP_INT_AIC_VECT_EN == DEF_ENABLED)
    CPU_INT08U         int_src;
    CSP_INT_AIC_VECT  *p_aic_vect;
#endif    
    
    
    switch (src_nbr) {        
        case CPU_ARM_EXCEPT_IRQ:
             isr_fnct = (CPU_FNCT_PTR)CSP_INT_REG_AIC_IVR;
#if( CSP_INT_AIC_VECT_EN == DEF_ENABLED)
             int_src  = CSP_INT_REG_AIC_ISR & DEF_BIT_FIELD(5u, 0u) ;
#endif             
             while (isr_fnct != (CPU_FNCT_PTR)0) {
#if( CSP_INT_AIC_VECT_EN == DEF_ENABLED)
                 p_aic_vect   = &CSP_MainVectTbl[int_src];
#endif                 
                 vect.FnctPtr =  isr_fnct;

#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)
                 vect.ArgPtr = p_aic_vect->ArgPtr;
#endif   

#if (CSP_CFG_INT_ISR_EXEC_MEAS_EN == DEF_ENABLED) && \
    (CPU_CFG_TS_EN                == DEF_ENABLED)
                 vect.TimeMax = p_aic_vect->TimeMax;
                 vect.TimeCur = p_aic_vect->TimeCur;  
#endif                     
                 CSP_IntVectDeref(&vect);
#if (CSP_CFG_INT_ISR_EXEC_MEAS_EN == DEF_ENABLED) && \
    (CPU_CFG_TS_EN                == DEF_ENABLED)
                 p_aic_vect->TimeMax = vect.TimeMax;
                 p_aic_vect->TimeCur = vect.TimeCur;
#endif
                 CSP_INT_REG_AIC_EOICR = DEF_BIT_NONE;
                 isr_fnct              = (CPU_FNCT_PTR)CSP_INT_REG_AIC_IVR;
#if( CSP_INT_AIC_VECT_EN == DEF_ENABLED)
                 int_src               = CSP_INT_REG_AIC_ISR & DEF_BIT_FIELD(5u, 0u) ;
#endif                 
             }
             CSP_INT_REG_AIC_EOICR = DEF_BIT_NONE;        
             break;
             
        case CPU_ARM_EXCEPT_FIQ:
             isr_fnct = (CPU_FNCT_PTR)CSP_INT_REG_AIC_FVR;
             while (isr_fnct != (CPU_FNCT_PTR)0) {
                 vect.FnctPtr = isr_fnct;
#if( CSP_INT_AIC_VECT_EN == DEF_ENABLED)
                 p_aic_vect   = &CSP_MainVectTbl[CSP_INT_SRC_NBR_EXT_FIQ];
#endif

#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)
                 vect.ArgPtr = p_aic_vect->ArgPtr;
#endif   

#if (CSP_CFG_INT_ISR_EXEC_MEAS_EN == DEF_ENABLED) && \
    (CPU_CFG_TS_EN                == DEF_ENABLED)
                 vect.TimeMax = p_aic_vect->TimeMax;
                 vect.TimeCur = p_aic_vect->TimeCur;  
#endif                     
                 CSP_IntVectDeref(&vect);
#if (CSP_CFG_INT_ISR_EXEC_MEAS_EN == DEF_ENABLED) && \
    (CPU_CFG_TS_EN                == DEF_ENABLED)
                 p_aic_vect->TimeMax = vect.TimeMax;
                 p_aic_vect->TimeCur = vect.TimeCur;
#endif
                 CSP_INT_REG_AIC_EOICR = DEF_BIT_NONE;
                 isr_fnct              = (CPU_FNCT_PTR)CSP_INT_REG_AIC_FVR;
             }
             CSP_INT_REG_AIC_EOICR = DEF_BIT_NONE;                    
             break;
             
        default:
             break;
    }
}
    