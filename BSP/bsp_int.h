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
*                                     MICRIUM BOARD SUPPORT PACKAGE
*                                         INTERRUPT CONTROLLER
*
*
* Filename      : bsp_int.h
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP_INT present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  BSP_INT_PRESENT
#define  BSP_INT_PRESENT


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


#ifdef   BSP_INT_MODULE
#define  BSP_INT_EXT
#else
#define  BSP_INT_EXT  extern
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
                                                                /* ------------ INTERRUPT SOURCE TYPE --------------- */
#define  BSP_INT_SCR_TYPE_INT_HIGH_LEVEL_SENSITIVE         0    /* Internal Interrupt High Level Sensitive            */
#define  BSP_INT_SCR_TYPE_INT_POSITIVE_EDGE_TRIG           1    /* Internal Interrupt Positive Edge Trigerred         */
#define  BSP_INT_SCR_TYPE_EXT_LOW_LEVEL_SENSITIVE          2    /* External Interrupt Low Level Sensitive             */
#define  BSP_INT_SCR_TYPE_EXT_HIGH_LEVEL_SENSITIVE         3    /* External Interrupt Negative Edge Trigerred         */
#define  BSP_INT_SCR_TYPE_EXT_NEGATIVE_EDGE_TRIG           4    /* External Interrupt High Level Sensitive            */         
#define  BSP_INT_SCR_TYPE_EXT_POSITIVE_EDGE_TRIG           5    /* External Interrupt Positive Edge triggered         */

                                                                /* ------------ INTERRUPT PRIORITY DEFINES ---------- */
#define  BSP_INT_PRIO_HIGHEST                              7
#define  BSP_INT_PRIO_LOWEST                               0
                         
                                                                /* ---------------- INTERRUPT TYPE DEFINES ---------- */
#define  BSP_INT_CTRL_TYPE_IRQ                             0
#define  BSP_INT_CTRL_TYPE_FIQ                             1

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

void         BSP_IntClr           (CPU_INT08U     per_id);
void         BSP_IntDis           (CPU_INT08U     per_id);
void         BSP_IntDisAll        (void);
void         BSP_IntEn            (CPU_INT08U     per_id);
void         BSP_IntInit          (void);
void         BSP_IntSched         (CPU_INT08U     int_type);
void         BSP_IntVectSet       (CPU_INT08U     per_id,
                                   CPU_INT08U     int_prio,
                                   CPU_INT08U     int_src_type,
                                   CPU_FNCT_VOID  int_isr_fnct);



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
