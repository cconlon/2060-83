
#include <includes.h>
#include <stdio.h>
#include <board.h>
#include <bsp.h>
#include <os_csp.h>
#include <stdint.h>
#include <include/trace.h>
//#include <netservices.h>
#include <./simudata/simudata.h>
#include <./83/main83.h>
#include <netservices.h>


// EMAC packet processing offset
#define EMAC_RCV_OFFSET     0

// Delay before a link check
#define EMAC_LINK_CHECK_DELAY       1000000
#define MY_TASK_STK_SIZE 1024

static  OS_STK  App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE];
static  OS_STK  Task0Stk[MY_TASK_STK_SIZE];

#define DEBOUNCE_TIME       50
volatile unsigned int timestamp = 0;
/*
*********************************************************************************************************
*                                         LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  App_TaskStart    (void *p_arg);
static  void  App_ObjCreate    (void);
static  void  App_TaskCreate   (void);
static  void  Task0 (void *p_arg);


int  main (void)
{
#if (OS_TASK_NAME_EN == DEF_ENABLED)
  CPU_INT08U	os_err;
#endif    
#if (CPU_CFG_NAME_EN == DEF_ENABLED) 
  CPU_ERR     cpu_err;
#endif
  
#if (CPU_CFG_NAME_EN == DEF_ENABLED)
  CPU_NameSet((CPU_CHAR *)"AT91SAM9G20 CU-ES2",
              (CPU_ERR  *)&cpu_err);
#endif    
  WDT_Disable(WDT);
  
  BSP_PreInit();                                              /* System pre-initialization.                               */
  BSP_PostInit();
  
  CPU_Init();
  Mem_Init();
  
  TRACE_CONFIGURE(115200, BOARD_MCK);
  printf("ucos");
  printf("ÄãºÃ");
  LED_Toggle(0);
  LED_Toggle(1);
  
  OS_CSP_TickInit();
  OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel"              */
  
  OSTaskCreateExt((void (*)(void *)) App_TaskStart,           /* Create the start task                                    */
                  (void           *) 0,
                  (OS_STK         *)&App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE - 1],
                  (INT8U           ) APP_CFG_TASK_START_PRIO,
                  (INT16U          ) APP_CFG_TASK_START_PRIO,
                  (OS_STK         *)&App_TaskStartStk[0],
                  (INT32U          ) APP_CFG_TASK_START_STK_SIZE,
                  (void           *) 0,
                  (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
  
#if (OS_TASK_NAME_EN == DEF_ENABLED)
  OSTaskNameSet((INT8U  )APP_CFG_TASK_START_PRIO,
                (INT8U *)"Start Task",
                (INT8U *)&os_err);
#endif
  //Simu_Build();
  //Init2060();
  //Start2060();
  OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)       */
}


/*
*********************************************************************************************************
*                                          App_TaskStart()
*
* Description : The startup task.  The uC/OS-II ticker should only be initialize once multitasking starts.
*
* Argument(s) : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  void  App_TaskStart (void *p_arg)
{
  (void)p_arg;
  
  BSP_PostInit();
#if OS_TASK_STAT_EN > 0u
  OSStatInit();                                               /* Determine CPU capacity                                   */
#endif
  
#if (APP_CFG_SERIAL_EN == DEF_ENABLED)
  App_SerialInit();
#endif
  App_ObjCreate();                                            /* Create application objects                               */
  App_TaskCreate();                                           /* Create application tasks                                 */
  
  LED_Clear(0);
  LED_Clear(1);
  AppInit_TCPIP();
  while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.           */
    LED_Toggle(0);
    LED_Toggle(1);
    OSTimeDlyHMSM(0, 0, 1, 0);   
  }
}


/*
*********************************************************************************************************
*                                      App_ObjCreate()
*
* Description : Create applications objects.
*
* Argument(s) : none.
*
* Return(s)   : None.
*
* Caller(s)   : App_TaskStart().
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  void  App_ObjCreate (void)
{
  
}

/*
*********************************************************************************************************
*                                      App_TaskCreate()
*
* Description : Create application Tasks.
*
* Argument(s) : None.
*
* Return(s)   : None.
*
* Caller(s)   : App_TaskStart()
*
* Note(s)     : None.
*********************************************************************************************************
*/
//extern INT32S Init2060();
//extern INT32S Start2060();

static  void  App_TaskCreate (void)
{
  //AppInit_TCPIP();
  //Simu_Build();
  //Init2060();
  //Start2060();
  OSTaskCreate(Task0,
               NULL,
               (OS_STK *)&Task0Stk[MY_TASK_STK_SIZE - 1],
               APP_CFG_TASK_START_PRIO+4);
  
}

static  void  Task0 (void *p_arg)
{
  p_arg = p_arg;
  OSTimeDlyHMSM(0, 0, 1, 0);
  Test_Tcp(NULL);
  while (DEF_TRUE) {                                  /* Task body, always written as an infinite loop.  */
    LED_Toggle(0);
    
    OSTimeDlyHMSM(0, 0, 30, 0);
  }
}
