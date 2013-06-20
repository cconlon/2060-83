#ifndef COMMON_H
#define COMMON_H

#define  TASK_STK_SIZE      2<<20       /* Size of each task's stacks (# of WORDs)            */
#define  N_TASKS               10       /* Number of identical tasks                          */

#define TaskStart_Prio			0

#define Task_prio_Q_test		3
#define Task_prio_Sem_Test 		4
#define Task_prio_Mbox_Test 	5
#define Task_prio_Dly		 	6
#define Task_Prio_Flags			7
#define Task_Prio_Flags2		8
#define Task1_Prio		 		9// for shell

#include <stdint.h>

typedef struct
{
    uint32_t len;
    int32_t *addr;
}dynamic_vec32;

typedef uint8_t    INT8U;                    /* Unsigned  8 bit quantity                           */
typedef int8_t     INT8S;                    /* Signed    8 bit quantity                           */
typedef uint16_t   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef int16_t    INT16S;                   /* Signed   16 bit quantity                           */
typedef uint32_t   INT32U;                   /* Unsigned 32 bit quantity                           */
typedef int32_t    INT32S;                   /* Signed   32 bit quantity                           */
typedef float      FP32;                     /* Single precision floating point                    */
typedef double     FP64;                     /* Double precision floating point                    */

//void _error(char *msg);
//void _log(char *msg);

void sleep_ms(uint32_t ms);

#endif
