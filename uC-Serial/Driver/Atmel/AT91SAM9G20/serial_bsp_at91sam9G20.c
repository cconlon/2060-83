/*
*********************************************************************************************************
*                                     SERIAL (BYTE) COMMUNICATION
*
*                         (c) Copyright 2007-2009; Micrium, Inc.; Weston, FL
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
*                                     SERIAL (BYTE) COMMUNICATION
*                                            DEVICE DRIVER
*
*                                                ATMEL
*                                      BSP FOR ATMEL AT91SAM9G20
*
* Filename      : serial_bsp_at91sam9G20.c
* Version       : V2.00
* Programmer(s) : FT
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <serial_bsp_at91sam9G20.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

                                                                /* ----------- REGISTER BASE SPECIFICATIONS ----------- */
#define  SERIAL_BSP_AT91_USART0_BASE          ((void *)0xFFF8C000)
#define  SERIAL_BSP_AT91_USART1_BASE          ((void *)0xFFF90000)
#define  SERIAL_BSP_AT91_USART2_BASE          ((void *)0xFFF94000)
#define  SERIAL_BSP_AT91_USART3_BASE          ((void *)0xFFF98000)
#define  SERIAL_BSP_AT91_DBGU_BASE            ((void *)0xFFFFEE00)

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void        SerialBSP_AT91_DBGU_ClkEn         (SERIAL_ERR   *perr);
static  void        SerialBSP_AT91_DBGU_ClkDis        (SERIAL_ERR   *perr);
static  void        SerialBSP_AT91_DBGU_CfgGPIO       (CPU_BOOLEAN   flow_ctrl,
                                                       SERIAL_ERR   *perr);
static  void        SerialBSP_AT91_DBGU_CfgInt        (SERIAL_DEV   *pdev,
                                                       SERIAL_ERR   *perr);


static  void        SerialBSP_AT91_USART0_ClkEn       (SERIAL_ERR   *perr);
static  void        SerialBSP_AT91_USART0_ClkDis      (SERIAL_ERR   *perr);
static  void        SerialBSP_AT91_USART0_CfgGPIO     (CPU_BOOLEAN   flow_ctrl,
                                                       SERIAL_ERR   *perr);
static  void        SerialBSP_AT91_USART0_CfgInt      (SERIAL_DEV   *pdev,
                                                       SERIAL_ERR   *perr);

static  void        SerialBSP_AT91_USART1_ClkEn      (SERIAL_ERR   *perr);
static  void        SerialBSP_AT91_USART1_ClkDis     (SERIAL_ERR   *perr);
static  void        SerialBSP_AT91_USART1_CfgGPIO    (CPU_BOOLEAN   flow_ctrl,
                                                      SERIAL_ERR   *perr);
static  void        SerialBSP_AT91_USART1_CfgInt     (SERIAL_DEV   *pdev,
                                                      SERIAL_ERR   *perr);


static  void        SerialBSP_AT91_USART2_ClkEn      (SERIAL_ERR   *perr);
static  void        SerialBSP_AT91_USART2_ClkDis     (SERIAL_ERR   *perr);
static  void        SerialBSP_AT91_USART2_CfgGPIO    (CPU_BOOLEAN   flow_ctrl,
                                                      SERIAL_ERR   *perr);
static  void        SerialBSP_AT91_USART2_CfgInt     (SERIAL_DEV   *pdev,
                                                      SERIAL_ERR   *perr);

static  void        SerialBSP_AT91_USART3_ClkEn      (SERIAL_ERR   *perr);
static  void        SerialBSP_AT91_USART3_ClkDis     (SERIAL_ERR   *perr);
static  void        SerialBSP_AT91_USART3_CfgGPIO    (CPU_BOOLEAN   flow_ctrl,
                                                      SERIAL_ERR   *perr);
static  void        SerialBSP_AT91_USART3_CfgInt     (SERIAL_DEV   *pdev,
                                                      SERIAL_ERR   *perr);

#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)    
static  void        SerialBSP_AT91_USARTx_IntHandler   (void *p_arg);
#else
static  void        SerialBSP_AT91_USART_DbgIntHandler (void *p_arg);
static  void        SerialBSP_AT91_USART0_IntHandler   (void *p_arg);
static  void        SerialBSP_AT91_USART1_IntHandler   (void *p_arg);
static  void        SerialBSP_AT91_USART2_IntHandler   (void *p_arg);
static  void        SerialBSP_AT91_USART3_IntHandler   (void *p_arg);
#endif

static  CPU_INT32U  SerialBSP_AT91_ClkFreqGet        (void);


/*
*********************************************************************************************************
*                                   SERIAL INTERFACE DEVICE BSP API
*********************************************************************************************************
*/

static  SERIAL_BSP_API  SerialBSP_AT91_DBGU = {
    SerialBSP_AT91_DBGU_ClkEn,
    SerialBSP_AT91_DBGU_ClkDis,
    SerialBSP_AT91_DBGU_CfgGPIO,
    SerialBSP_AT91_DBGU_CfgInt,
    SerialBSP_AT91_ClkFreqGet
};

static  SERIAL_BSP_API  SerialBSP_AT91_USART0 = {
    SerialBSP_AT91_USART0_ClkEn,
    SerialBSP_AT91_USART0_ClkDis,
    SerialBSP_AT91_USART0_CfgGPIO,
    SerialBSP_AT91_USART0_CfgInt,
    SerialBSP_AT91_ClkFreqGet
};

static  SERIAL_BSP_API  SerialBSP_AT91_USART1 = {
    SerialBSP_AT91_USART1_ClkEn,
    SerialBSP_AT91_USART1_ClkDis,
    SerialBSP_AT91_USART1_CfgGPIO,
    SerialBSP_AT91_USART1_CfgInt,
    SerialBSP_AT91_ClkFreqGet
};

static  SERIAL_BSP_API  SerialBSP_AT91_USART2 = {
    SerialBSP_AT91_USART2_ClkEn,
    SerialBSP_AT91_USART2_ClkDis,
    SerialBSP_AT91_USART2_CfgGPIO,
    SerialBSP_AT91_USART2_CfgInt,
    SerialBSP_AT91_ClkFreqGet
};


static  SERIAL_BSP_API  SerialBSP_AT91_USART3 = {
    SerialBSP_AT91_USART3_ClkEn,
    SerialBSP_AT91_USART3_ClkDis,
    SerialBSP_AT91_USART3_CfgGPIO,
    SerialBSP_AT91_USART3_CfgInt,
    SerialBSP_AT91_ClkFreqGet
};

/*
*********************************************************************************************************
*********************************************************************************************************
*                                      SERIAL INTERFACE DEVICES
*********************************************************************************************************
*********************************************************************************************************
*/

SERIAL_DEV_CFG  SerialDev_Atmel_Dbgu = {
     SERIAL_BSP_AT91_DBGU_BASE,
    &SerialDrv_Atmel_DBGU_API,
    &SerialBSP_AT91_DBGU
};

SERIAL_DEV_CFG  SerialDev_Atmel_UART0 = {
     SERIAL_BSP_AT91_USART0_BASE,
    &SerialDrv_Atmel_API,
    &SerialBSP_AT91_USART0
};

SERIAL_DEV_CFG  SerialDev_Atmel_UART1 = {
     SERIAL_BSP_AT91_USART1_BASE,
    &SerialDrv_Atmel_API,
    &SerialBSP_AT91_USART1
};

SERIAL_DEV_CFG  SerialDev_Atmel_UART2 = {
     SERIAL_BSP_AT91_USART2_BASE,
    &SerialDrv_Atmel_API,
    &SerialBSP_AT91_USART2
};


SERIAL_DEV_CFG  SerialDev_Atmel_UART3 = {
     SERIAL_BSP_AT91_USART3_BASE,
    &SerialDrv_Atmel_API,
    &SerialBSP_AT91_USART3
};

/*
*********************************************************************************************************
*********************************************************************************************************
*                                      LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*********************************************************************************************************
*/

#if (CSP_CFG_INT_ISR_ARG_EN == DEF_DISABLED)    
static  SERIAL_DEV  *SerialBSP_AT91_DBGU_DevPtr;
static  SERIAL_DEV  *SerialBSP_AT91_USART0_DevPtr;
static  SERIAL_DEV  *SerialBSP_AT91_USART1_DevPtr;
static  SERIAL_DEV  *SerialBSP_AT91_USART2_DevPtr;
static  SERIAL_DEV  *SerialBSP_AT91_USART3_DevPtr;
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                        LOCAL ERRORS
*********************************************************************************************************
*********************************************************************************************************
*/


/*$PAGE*/
/*
*********************************************************************************************************
*                                          SerialBSP_ClkEn()
*
* Description : Enable device/peripheral clock.
*
* Argument(s) : perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device/peripheral clock enabled successfully.
*
* Return(s)   : none.
*
* Caller(s)   : SerialDrv_Open() via 'pdev->BSP_API->ClkEn()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  SerialBSP_AT91_DBGU_ClkEn (SERIAL_ERR  *perr)
{        
   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_AT91_USART0_ClkEn (SERIAL_ERR  *perr)
{
    CSP_IntDis(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_UART_00);
    CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_UART_00);

   *perr = SERIAL_ERR_NONE;
}


static  void  SerialBSP_AT91_USART1_ClkEn (SERIAL_ERR  *perr)
{
    CSP_IntDis(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_UART_01);
    CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_UART_01);

   *perr = SERIAL_ERR_NONE;
}


static  void  SerialBSP_AT91_USART2_ClkEn (SERIAL_ERR  *perr)
{
    CSP_IntDis(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_UART_02);
    CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_UART_02);

   *perr = SERIAL_ERR_NONE;
}


static  void  SerialBSP_AT91_USART3_ClkEn (SERIAL_ERR  *perr)
{
    CSP_IntDis(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_UART_03);
    CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_UART_03);

   *perr = SERIAL_ERR_NONE;
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialBSP_ClkDis()
*
* Description : Disable device/peripheral clock.
*
* Argument(s) : perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device/peripheral clock disabled successfully.
*
* Return(s)   : none.
*
* Caller(s)   : SerialDrv_Close() via 'pdev->BSP_API->ClkDis()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  SerialBSP_AT91_DBGU_ClkDis (SERIAL_ERR  *perr)
{

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_AT91_USART0_ClkDis (SERIAL_ERR  *perr)
{
    CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_UART_00);

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_AT91_USART1_ClkDis (SERIAL_ERR  *perr)
{
    CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_UART_01);

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_AT91_USART2_ClkDis (SERIAL_ERR  *perr)
{
    CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_UART_02);

   *perr = SERIAL_ERR_NONE;
}


static  void  SerialBSP_AT91_USART3_ClkDis (SERIAL_ERR  *perr)
{
    CSP_PM_PerClkEn(CSP_PM_PER_CLK_NBR_UART_03);

   *perr = SERIAL_ERR_NONE;
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialBSP_CfgGPIO()
*
* Description : Configure device/peripheral input/output pins.
*
* Argument(s) : flow_ctrl   Flow control enabled.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device/peripheral GPIO configured successfully.
*                               SERIAL_ERR_DRV_INVALID          Device configuration invalid.
*
* Return(s)   : none.
*
* Caller(s)   : SerialDrv_Open() via 'pdev->BSP_API->CfgGPIO()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  SerialBSP_AT91_DBGU_CfgGPIO (CPU_BOOLEAN   flow_ctrl,
                                           SERIAL_ERR   *perr)
{
    CSP_GPIO_Cfg(CSP_GPIO_PORT_NBR_B,                           /* ... Port B.                                          */
                (DEF_BIT_12 | DEF_BIT_13),
                 CSP_GPIO_DIR_IN,                               /* ... Pin configured as input.                         */
                 CSP_GPIO_FLAG_MODE_NONE,                       /* ... Neither  pull-up nor pull-down resistor.         */
                 DEF_NO,                                        /* ... Interrupt disabled.                              */
                 DEF_BIT_NONE,
                 CSP_GPIO_FNCT_01);                             /* ... Peripheral A function.                           */
    
   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_AT91_USART0_CfgGPIO (CPU_BOOLEAN   flow_ctrl,
                                             SERIAL_ERR   *perr)
{
    CSP_GPIO_Cfg(CSP_GPIO_PORT_NBR_A,                           /* ... Port B.                                          */
                (DEF_BIT_18 | DEF_BIT_19),
                 CSP_GPIO_DIR_IN,                               /* ... Pin configured as input.                         */
                 CSP_GPIO_FLAG_MODE_NONE,                       /* ... Neither  pull-up nor pull-down resistor.         */
                 DEF_NO,                                        /* ... Interrupt disabled.                              */
                 DEF_BIT_NONE,
                 CSP_GPIO_FNCT_01);                             /* ... Peripheral A function.                           */

    if (flow_ctrl == DEF_ENABLED) {
        CSP_GPIO_Cfg(CSP_GPIO_PORT_NBR_A,                       /* ... Port B.                                          */
                    (DEF_BIT_15 | DEF_BIT_17),
                     CSP_GPIO_DIR_IN,                           /* ... Pin configured as input.                         */
                     CSP_GPIO_FLAG_MODE_NONE,                   /* ... Neither  pull-up nor pull-down resistor.         */
                     DEF_NO,                                    /* ... Interrupt disabled.                              */
                     DEF_BIT_NONE,
                     CSP_GPIO_FNCT_02);                         /* ... Peripheral B function.                           */
    }
 
   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_AT91_USART1_CfgGPIO (CPU_BOOLEAN   flow_ctrl,
                                             SERIAL_ERR   *perr)
{
    CSP_GPIO_Cfg(CSP_GPIO_PORT_NBR_B,                           /* ... Port B.                                          */
                (DEF_BIT_04 | DEF_BIT_05),
                 DEF_BIT_NONE,
                 CSP_GPIO_FLAG_MODE_NONE,                       /* ... Neither  pull-up nor pull-down resistor.         */
                 DEF_NO,                                        /* ... Interrupt disabled.                              */
                 DEF_BIT_NONE,
                 CSP_GPIO_FNCT_01);                             /* ... Peripheral A function.                           */        
    
    if (flow_ctrl == DEF_ENABLED) {
        CSP_GPIO_Cfg(CSP_GPIO_PORT_NBR_D,                       /* ... Port D.                                          */
                    (DEF_BIT_16 | DEF_BIT_17),
                     DEF_BIT_NONE,
                     CSP_GPIO_FLAG_MODE_NONE,                   /* ... Neither  pull-up nor pull-down resistor.         */
                     DEF_NO,                                    /* ... Interrupt disabled.                              */
                     DEF_BIT_NONE,
                     CSP_GPIO_FNCT_01);                         /* ... Peripheral A function.                           */
    }

   *perr = SERIAL_ERR_NONE;
}


static  void  SerialBSP_AT91_USART2_CfgGPIO (CPU_BOOLEAN   flow_ctrl,
                                             SERIAL_ERR   *perr)
{
    CSP_GPIO_Cfg(CSP_GPIO_PORT_NBR_B,                           /* ... Port B.                                          */
                (DEF_BIT_07 | DEF_BIT_06),
                 DEF_BIT_NONE,
                 CSP_GPIO_FLAG_MODE_NONE,                       /* ... Neither  pull-up nor pull-down resistor.         */
                 DEF_NO,                                        /* ... Interrupt disabled.                              */
                 DEF_BIT_NONE,
                 CSP_GPIO_FNCT_01);                             /* ... Peripheral A function.                           */        
    
    if (flow_ctrl == DEF_ENABLED) {
        CSP_GPIO_Cfg(CSP_GPIO_PORT_NBR_C,                       /* ... Port C.                                          */
                    (DEF_BIT_09 | DEF_BIT_11),
                     DEF_BIT_NONE,
                     CSP_GPIO_FLAG_MODE_NONE,                   /* ... Neither  pull-up nor pull-down resistor.         */
                     DEF_NO,                                    /* ... Interrupt disabled.                              */
                     DEF_BIT_NONE,
                     CSP_GPIO_FNCT_01);                         /* ... Peripheral A function.                           */
    }

    *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_AT91_USART3_CfgGPIO (CPU_BOOLEAN   flow_ctrl,
                                             SERIAL_ERR   *perr)
{
    CSP_GPIO_Cfg(CSP_GPIO_PORT_NBR_B,                           /* ... Port B.                                          */
                (DEF_BIT_08 | DEF_BIT_09),
                 DEF_BIT_NONE,
                 CSP_GPIO_FLAG_MODE_NONE,                       /* ... Neither  pull-up nor pull-down resistor.         */
                 DEF_NO,                                        /* ... Interrupt disabled.                              */
                 DEF_BIT_NONE,
                 CSP_GPIO_FNCT_01);                             /* ... Peripheral A function.                           */        
    
    if (flow_ctrl == DEF_ENABLED) {
        CSP_GPIO_Cfg(CSP_GPIO_PORT_NBR_A,                       /* ... Port A.                                          */
                    (DEF_BIT_23 | DEF_BIT_24),
                     DEF_BIT_NONE,
                     CSP_GPIO_FLAG_MODE_NONE,                   /* ... Neither  pull-up nor pull-down resistor.         */
                     DEF_NO,                                    /* ... Interrupt disabled.                              */
                     DEF_BIT_NONE,
                     CSP_GPIO_FNCT_02);                         /* ... Peripheral B function.                           */
    }

    *perr = SERIAL_ERR_NONE;
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialBSP_CfgInt()
*
* Description : Configure device/peripheral interrupt.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device/peripheral interrupt configured successfully.
*
* Return(s)   : none.
*
* Caller(s)   : SerialDrv_Open() via 'pdev->BSP_API->CfgInt()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  SerialBSP_AT91_DBGU_CfgInt (SERIAL_DEV  *pdev,
                                            SERIAL_ERR  *perr)
{
#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)    
   CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_SYS, 
                  (CSP_DEV_NBR   )CSP_INT_SYS_SRC_NBR_DBG,
                  (CPU_FNCT_PTR  )SerialBSP_AT91_USARTx_IntHandler,
                  (void         *)pdev);
#else
   SerialBSP_AT91_DBGU_DevPtr = pdev;

   CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_SYS, 
                  (CSP_DEV_NBR   )CSP_INT_SYS_SRC_NBR_DBG,
                  (CPU_FNCT_PTR  )SerialBSP_AT91_USART_DbgIntHandler,
                  (void         *)pdev);   
#endif   
    
   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_AT91_USART0_CfgInt (SERIAL_DEV  *pdev,
                                            SERIAL_ERR  *perr)
{
    CSP_IntSrcCfg(CSP_INT_CTRL_NBR_MAIN,
                  CSP_INT_SRC_NBR_UART_00,
                  0u,
                  CSP_INT_POL_LEVEL_HIGH);

#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)    
    CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN, 
                   (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_00,
                   (CPU_FNCT_PTR  )SerialBSP_AT91_USARTx_IntHandler,
                   (void         *)pdev);
#else
    SerialBSP_AT91_USART0_DevPtr = pdev;

    CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN, 
                   (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_00,
                   (CPU_FNCT_PTR  )SerialBSP_AT91_USART0_IntHandler,
                   (void         *)pdev);
#endif    
        
    CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_UART_00);
    

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_AT91_USART1_CfgInt (SERIAL_DEV  *pdev,
                                            SERIAL_ERR  *perr)
{
    CSP_IntSrcCfg(CSP_INT_CTRL_NBR_MAIN,
                  CSP_INT_SRC_NBR_UART_01,
                  0u,
                  CSP_INT_POL_LEVEL_HIGH);

#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)    
    CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN, 
                   (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_01,
                   (CPU_FNCT_PTR  )SerialBSP_AT91_USARTx_IntHandler,
                   (void         *)pdev);
#else
    SerialBSP_AT91_USART1_DevPtr = pdev;

    CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN, 
                   (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_01,
                   (CPU_FNCT_PTR  )SerialBSP_AT91_USART1_IntHandler,
                   (void         *)pdev);
#endif    

    CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_UART_01);

    *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_AT91_USART2_CfgInt (SERIAL_DEV  *pdev,
                                            SERIAL_ERR  *perr)
{
    CSP_IntSrcCfg(CSP_INT_CTRL_NBR_MAIN,
                  CSP_INT_SRC_NBR_UART_02,
                  0u,
                  CSP_INT_POL_LEVEL_HIGH);

#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)    
    CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN, 
                   (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_02,
                   (CPU_FNCT_PTR  )SerialBSP_AT91_USARTx_IntHandler,
                   (void         *)pdev);
#else
     SerialBSP_AT91_USART2_DevPtr = pdev;
     
     CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN, 
                    (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_02,
                    (CPU_FNCT_PTR  )SerialBSP_AT91_USART2_IntHandler,
                    (void         *)pdev);
#endif   
    
    CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_UART_02);

   *perr = SERIAL_ERR_NONE;
}


static  void  SerialBSP_AT91_USART3_CfgInt (SERIAL_DEV  *pdev,
                                            SERIAL_ERR  *perr)
{
    CSP_IntSrcCfg(CSP_INT_CTRL_NBR_MAIN,
                  CSP_INT_SRC_NBR_UART_03,
                  0u,
                  CSP_INT_POL_LEVEL_HIGH);

#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)        
    CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN, 
                   (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_03,
                   (CPU_FNCT_PTR  )SerialBSP_AT91_USARTx_IntHandler,
                   (void         *)pdev);
#else
     SerialBSP_AT91_USART3_DevPtr = pdev;
     
     CSP_IntVectReg((CSP_DEV_NBR   )CSP_INT_CTRL_NBR_MAIN, 
                    (CSP_DEV_NBR   )CSP_INT_SRC_NBR_UART_03,
                    (CPU_FNCT_PTR  )SerialBSP_AT91_USART3_IntHandler,
                    (void         *)pdev);
#endif    

    CSP_IntEn(CSP_INT_CTRL_NBR_MAIN, CSP_INT_SRC_NBR_UART_03);

   *perr = SERIAL_ERR_NONE;
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                       SerialBSP_ClkFreqGet()
*
* Description : Get peripheral clock frequency.
*
* Argument(s) : none.
*
* Return(s)   : Peripheral clock frequency, in Hz.
*
* Caller(s)   : SerialDrv_Open() via 'pdev->BSP_API->ClkFreqGet()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_INT32U  SerialBSP_AT91_ClkFreqGet (void)
{
    CPU_INT32U  clk_freq;


    clk_freq =CSP_PM_SysClkFreqGet(CSP_PM_SYS_CLK_NBR_MCK_00);
    
    return (clk_freq);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                       SerialBSP_IntHandler()
*
* Description : Handle serial port interrupt.
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

#if (CSP_CFG_INT_ISR_ARG_EN == DEF_ENABLED)
void  SerialBSP_AT91_USARTx_IntHandler (void  *p_arg)
{
    SERIAL_DEV      *p_dev;
    SERIAL_DEV_CFG  *p_cfg;

    p_dev  = (SERIAL_DEV *)p_arg;
    p_cfg  = p_dev->Dev_Cfg;    
    p_cfg->Drv_API->ISR_Handler(p_dev, SERIAL_ISR_TYPE_UNKNOWN);
}
#else

static  void  SerialBSP_AT91_USART_DbgIntHandler (void *p_arg)
{
    SERIAL_DEV      *p_dev;
    SERIAL_DEV_CFG  *p_cfg;

    p_dev  = (SERIAL_DEV *)SerialBSP_AT91_DBGU_DevPtr;
    p_cfg  = p_dev->Dev_Cfg;    
    p_cfg->Drv_API->ISR_Handler(p_dev, SERIAL_ISR_TYPE_UNKNOWN);

}

static  void  SerialBSP_AT91_USART0_IntHandler  (void *p_arg)
{
    SERIAL_DEV      *p_dev;
    SERIAL_DEV_CFG  *p_cfg;

    
    p_dev  = (SERIAL_DEV *)SerialBSP_AT91_USART0_DevPtr;
    p_cfg  = p_dev->Dev_Cfg;    
    p_cfg->Drv_API->ISR_Handler(p_dev, SERIAL_ISR_TYPE_UNKNOWN);

}

static  void  SerialBSP_AT91_USART1_IntHandler  (void *p_arg)
{
    SERIAL_DEV      *p_dev;
    SERIAL_DEV_CFG  *p_cfg;

    
    p_dev  = (SERIAL_DEV *)SerialBSP_AT91_USART1_DevPtr;
    p_cfg  = p_dev->Dev_Cfg;    
    p_cfg->Drv_API->ISR_Handler(p_dev, SERIAL_ISR_TYPE_UNKNOWN);
}


static  void  SerialBSP_AT91_USART2_IntHandler  (void *p_arg)
{
    SERIAL_DEV      *p_dev;
    SERIAL_DEV_CFG  *p_cfg;

    
    p_dev  = (SERIAL_DEV *)SerialBSP_AT91_USART2_DevPtr;
    p_cfg  = p_dev->Dev_Cfg;    
    p_cfg->Drv_API->ISR_Handler(p_dev, SERIAL_ISR_TYPE_UNKNOWN);
}


static  void  SerialBSP_AT91_USART3_IntHandler  (void *p_arg)
{
    SERIAL_DEV      *p_dev;
    SERIAL_DEV_CFG  *p_cfg;

    
    p_dev  = (SERIAL_DEV *)SerialBSP_AT91_USART3_DevPtr;
    p_cfg  = p_dev->Dev_Cfg;    
    p_cfg->Drv_API->ISR_Handler(p_dev, SERIAL_ISR_TYPE_UNKNOWN);
}
#endif