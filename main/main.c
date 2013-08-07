#include <ucos_ii.h>
#include <board.h>
#include <bsp.h>
#include <os_csp.h>
#include <lib_mem.h>
#include <stdint.h>
#include <include/trace.h>
#include "simudata/simudata.h"
#include "83/main83.h"
#include "netservices.h"
#include "app_cfg.h"

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

#define ICACHE_MASK	(INT32U)(1 << 12)
#define DCACHE_MASK	(INT32U)(1 << 2)

INT32U  cp15_rd (void);
void cache_enable(INT32U bit);
void cache_disable(INT32U bit);

INT32U  cp15_rd (void)
{
    __asm("mrc p15, 0, r0, c1, c0, 0");
}

void cache_enable(INT32U bit)
{
  __asm("mrc p15, 0, r1, c1, c0, 0 \n"
        "orr r1, r1, r0 \n"
        "mcr p15, 0, r1, c1, c0, 0");
}

void cache_disable(INT32U bit)
{
  __asm("mrc p15, 0, r1, c1, c0, 0 \n"
        "bic r1, r1, r0 \n"
        "mcr p15, 0, r1, c1, c0, 0");
}
int tmd2(int a,int b);
int tmd3(int a,int b,int c);
int tmd4(int a,int b,int c,int d);

int tmd2(int a,int b)
{
  int sum = 0;
 
  int A[] = {1,2,3,10,20,30,100,200,300};
  for(int k=0;k<9;k++)
  {
    sum += A[k];
  }
  sum -= a;
  sum -= b;
  return sum;
}
int tmd3(int a,int b,int c)
{
  return a+b+c;
}
int tmd4(int a,int b, int c, int d)
{
  int tmp1 = 0;
  int tmp2 = 0;
  int tmp;
  tmp1 = a*a+b*b;
  tmp2 = c*c+d*d;
  tmp += tmd2(tmp1, tmp2);
  return tmp;
}
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
  tmd4(5, 10, 20, 30);
  INT32U cp15_value = cp15_rd();
  cache_enable(ICACHE_MASK);
  cache_disable(ICACHE_MASK);
  cache_enable(ICACHE_MASK);
  
  cache_enable(DCACHE_MASK);
  cache_disable(DCACHE_MASK);
  cache_enable(DCACHE_MASK);
  
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
  AppInit_TCPIP();
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
  Simu_Build();
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
//extern WEAK signed int putchar( signed int c )
//{
//  DBGU_PutChar( c ) ;
//  return c ;
//}
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
  int i=0;
  char buff[16];
  while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.           */
    LED_Toggle(0);
    LED_Toggle(1);
    sprintf(buff,"i=%d ",i++);
    printf(buff);
    OSTimeDlyHMSM(0, 0, 5, 0);   
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

static  void  App_TaskCreate (void)
{
  OSTaskCreate(Task0,
               NULL,
               (OS_STK *)&Task0Stk[MY_TASK_STK_SIZE - 1],
               APP_TASK0_TASK_PRIO);
  
}

static  void  Task0 (void *p_arg)
{
  p_arg = p_arg;
  OSTimeDlyHMSM(0, 0, 5, 0);
  //  AppInit_TCPIP();
  //  Test_Tcp(NULL);

  while (DEF_TRUE) {                                  /* Task body, always written as an infinite loop.  */
    LED_Toggle(0);
    OSTimeDlyHMSM(0, 0, 2, 0);
  }
}
