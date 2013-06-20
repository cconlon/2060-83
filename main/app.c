/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                                 (c) Copyright 2004, Micrium, Weston, FL
*                                          All Rights Reserved
*
*                                              AT91SAM9261
*                                              Sample code
* File : APP.C
* By   : Eric Shufro
*********************************************************************************************************
*/

#include <includes.h>

/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

static  OS_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

#if uC_TCPIP_MODULE > 0
        NET_IP_ADDR   ip;
        NET_IP_ADDR   msk;
        NET_IP_ADDR   gateway;
        NET_ERR       err;
#endif

/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void    AppTaskStart(void *p_arg);

#if OS_VIEW_MODULE > 0
static  void  AppTerminalRx(CPU_INT08U rx_data);
#endif

#if uC_TCPIP_MODULE > 0
static  void  AppInit_TCPIP(void);
#endif

/*
*********************************************************************************************************
*                                             C ENTRY POINT
*********************************************************************************************************
*/

int  main (void)
{
    CPU_INT08U err;


    BSP_IntDisAll();                            /* Disable ALL interrupts to the interrupt controller  */

    OSInit();                                   /* Initialize uC/OS-II                                 */

                                                /* Create start task                                   */
    OSTaskCreateExt(AppTaskStart,
                    NULL,
                    (OS_STK *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1],
                    APP_TASK_START_PRIO,
                    APP_TASK_START_PRIO,
                    (OS_STK *)&AppTaskStartStk[0],
                    APP_TASK_START_STK_SIZE,
                    NULL,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

                                                /* Assign names to created tasks                       */
#if OS_TASK_NAME_SIZE > 11
    OSTaskNameSet(APP_TASK_START_PRIO, (INT8U*)("Start Task"), &err);
#endif

    OSStart();                                  /* Start uC/OS-II                                      */
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                              STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
* Arguments   : p_arg is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*               2) Interrupts are enabled once the task start because the I-bit of the CCR register was
*                  set to 0 by 'OSTaskCreate()'.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{

    (void)p_arg;                                    /* Prevent compiler warning                        */

    BSP_Init();                                     /* Initialize the BSP                              */

#if OS_TASK_STAT_EN > 0
    OSStatInit();                                   /* Start stats task                                */
#endif

#if OS_VIEW_MODULE > 0
    OSView_Init(38400);                             /* OSView Init, baud rate = 38400                  */
    OSView_TerminalRxSetCallback(AppTerminalRx);
    OSView_RxIntEn();                               /* Enable Rx Interrupts                            */
#endif

#if uC_TCPIP_MODULE > 0
    AppInit_TCPIP();                                /* Initialize uC/TCP-IP and associated appliations     */
#endif

    LED_Off(0);                                     /* Turn off ALL the LEDs                           */

    while (TRUE) {                                  /* Task body, always written as an infinite loop.  */
        LED_Toggle(1);
        LED_Toggle(2);
        OSTimeDlyHMSM(0, 0, 0, 100);
    }
}

/*
*********************************************************************************************************
*                                     uC/OS-View TERMINAL WINDOW CALLBACK
*********************************************************************************************************
*/

#if OS_VIEW_MODULE > 0
static  void  AppTerminalRx (CPU_INT08U rx_data)
{
}
#endif

/*
*********************************************************************************************************
*                                      AppInit_TCPIP()
*
* Description : This function is called by AppTaskStart() and is responsible for initializing uC/TCP-IP
*               uC/HTTPs, uC/TFTPs and uC/DHCPc if enabled.
*********************************************************************************************************
*/
#if uC_TCPIP_MODULE > 0
static void AppInit_TCPIP (void)
{
#if EMAC_CFG_MAC_ADDR_SEL == EMAC_MAC_ADDR_SEL_CFG
    NetIF_MAC_Addr[0] = 0x00;
    NetIF_MAC_Addr[1] = 0x50;
    NetIF_MAC_Addr[2] = 0xC2;
    NetIF_MAC_Addr[3] = 0x25;
    NetIF_MAC_Addr[4] = 0x60;
    NetIF_MAC_Addr[5] = 0x01;
#endif

    err     = Net_Init();                                               /* Initialize uC/TCP-IP                                     */

#if 1                                                                   /* Selection for LAN IP address depending on network used   */
    ip      = NetASCII_Str_to_IP((INT8S*)"192.168.1.60",  &err);
    msk     = NetASCII_Str_to_IP((INT8S*)"255.255.255.0", &err);
    gateway = NetASCII_Str_to_IP((INT8S*)"192.168.1.1",   &err);
#else
    ip      = NetASCII_Str_to_IP((INT8S*)"10.10.1.116",   &err);
    msk     = NetASCII_Str_to_IP((INT8S*)"255.255.255.0", &err);
    gateway = NetASCII_Str_to_IP((INT8S*)"10.10.1.1",     &err);
#endif    
    
    err     = NetIP_CfgAddrThisHost(ip, msk);
    err     = NetIP_CfgAddrDfltGateway(gateway);
}
#endif




