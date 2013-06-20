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
*                                      MICRIUM BOARD SUPPORT PACKAGE
*                                         SERIAL (UART) INTERFACE
*
* Filename      : bsp_ser.c
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_SER_MODULE
#include <bsp.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
                                                                /* ------- USART CONTROL REGISTER BIT DEFINES ------- */
#define  BSP_SER_US_CR_RSTRX                  DEF_BIT_02        /* Reset receiver                                     */
#define  BSP_SER_US_CR_RSTTX                  DEF_BIT_03        /* Reset Transmitter                                  */ 
#define  BSP_SER_US_CR_RXEN                   DEF_BIT_04        /* Receiver Enable                                    */
#define  BSP_SER_US_CR_RXDIS                  DEF_BIT_05        /* Receiver Disable                                   */
#define  BSP_SER_US_CR_TXEN                   DEF_BIT_06        /* Transmitter Enable                                 */
#define  BSP_SER_US_CR_TXDIS                  DEF_BIT_07        /* Transmitter Disable                                */
#define  BSP_SER_US_CR_RSTSTA                 DEF_BIT_08        /* Reset Status Bits                                  */
#define  BSP_SER_US_CR_STTBRK                 DEF_BIT_09        /* Start Break                                        */
#define  BSP_SER_US_CR_STPBRK                 DEF_BIT_10        /* Stop Break                                         */
#define  BSP_SER_US_CR_STTTO                  DEF_BIT_11        /* Start Time-out                                     */
#define  BSP_SER_US_CR_SENDA                  DEF_BIT_12        /* Send Address                                       */
#define  BSP_SER_US_CR_RSTIT                  DEF_BIT_13        /* Reset Iterations                                   */
#define  BSP_SER_US_CR_RSTNACK                DEF_BIT_14        /* Reset Non Acknowledge                              */
#define  BSP_SER_US_CR_RETTO                  DEF_BIT_15        /* Rearm Time-out                                     */
#define  BSP_SER_US_CR_RTSEN                  DEF_BIT_18        /* Request to Send Enable                             */
#define  BSP_SER_US_CR_RTSDIS                 DEF_BIT_19        /* Request to Send Disable                            */

                                                                /* --------- USART MODE REGISTER BIT DEFINES -------- */
#define  BSP_SER_US_MR_MODE_MASK              DEF_BIT_FIELD(4, 0)
#define  BSP_SER_US_MR_MODE_NORMAL            DEF_BIT_NONE
                                                                /* Clock source select mask                           */
#define  BSP_SER_US_MR_USCLKS_MASK            DEF_BIT_FIELD(2, 4)
#define  BSP_SER_US_MR_USCLKS_MCK             DEF_BIT_NONE
#define  BSP_SER_US_MR_USCLKS_MCK_DIV         DEF_BIT_MASK(1, 4)
#define  BSP_SER_US_MR_USCLKS_SCK             DEF_BIT_MASK(3, 4)
                                                                /* Character Length Mask                              */
#define  BSP_SER_US_MR_CHRL_MASK              DEF_BIT_FIELD(2, 6)
#define  BSP_SER_US_MR_CHRL_5                 DEF_BIT_NONE    
#define  BSP_SER_US_MR_CHRL_6                 DEF_BIT_MASK(1, 6)
#define  BSP_SER_US_MR_CHRL_7                 DEF_BIT_MASK(2, 6)
#define  BSP_SER_US_MR_CHRL_8                 DEF_BIT_MASK(3, 6)
                                                                /* Parity Mask                                        */
#define  BSP_SER_US_MR_PAR_MASK               DEF_BIT_FIELD(3, 9)
#define  BSP_SER_US_MR_PAR_EVEN               DEF_BIT_NONE     
#define  BSP_SER_US_MR_PAR_ODD                DEF_BIT_MASK(1, 9)
#define  BSP_SER_US_MR_PAR_FORCED_0           DEF_BIT_MASK(2, 9)
#define  BSP_SER_US_MR_PAR_FORCED_1           DEF_BIT_MASK(3, 9)
#define  BSP_SER_US_MR_PAR_NONE               DEF_BIT_MASK(4, 9)
#define  BSP_SER_US_MR_PAR_MULTI_DROP         DEF_BIT_MASK(5, 9)
                                                                /* Number of stop bits                                */
#define  BSP_SER_US_MR_NBSTOP_MASK            DEF_BIT_FIELD(2, 12)
#define  BSP_SER_US_MR_NBSTOP_1               DEF_BIT_NONE     
#define  BSP_SER_US_MR_NBSTOP_1_5             DEF_BIT_MASK(1, 12)
#define  BSP_SER_US_MR_NBSTOP_2               DEF_BIT_MASK(2, 12)


                                                                /* ---------- USART INTERRUPTS BIT DEFINES --------- */
#define  BSP_SER_US_INT_RXRDY                 DEF_BIT_00        /* RXRDY Interrupt Enable                            */
#define  BSP_SER_US_INT_TXRDY                 DEF_BIT_01        /* TXRDY Interrupt Enable                            */
#define  BSP_SER_US_INT_RXBRK                 DEF_BIT_02        /* Receiver Break Interrupt Enable                   */
#define  BSP_SER_US_INT_ENDRX                 DEF_BIT_03        /* End of Receive Transfer Interrupt Enable          */
#define  BSP_SER_US_INT_ENDTX                 DEF_BIT_04        /* End of Transmit Interrupt Enable                  */
#define  BSP_SER_US_INT_OVRE                  DEF_BIT_05        /* Overrun Error Interrupt Enable                    */
#define  BSP_SER_US_INT_FRAME                 DEF_BIT_06        /* Framing Error Interrupt Enable                    */
#define  BSP_SER_US_INT_PARE                  DEF_BIT_07        /* Parity Error Interrupt Enable                     */
#define  BSP_SER_US_INT_TIMEOUT               DEF_BIT_08        /* Time-out Interrupt Enable                         */ 
#define  BSP_SER_US_INT_TXEMPTY               DEF_BIT_09        /* TXEMPTY Interrupt Enable                          */
#define  BSP_SER_US_INT_ITERATION             DEF_BIT_10        /* Iteration Interrupt Enable                        */
#define  BSP_SER_US_INT_TXBUFE                DEF_BIT_11        /* Buffer Empty Interrupt Enable                     */
#define  BSP_SER_US_INT_RXBUFF                DEF_BIT_12        /* Buffer Full Interrupt Enable                      */
#define  BSP_SER_US_INT_NACK                  DEF_BIT_13        /* Non Acknowledge Interrupt Enable                  */
#define  BSP_SER_US_INT_CTSIC                 DEF_BIT_19        /* Clear to Send Input Change Interrupt Enable       */
#define  BSP_SER_US_INT_ALL                  (DEF_BIT_FIELD(14, 0) | \
                                              BSP_SER_US_INT_CTSIC)

                                                                 /* --------------- USART I/O DEFINES -------------- */
#define  BSP_SER_UART_00_PINS                (DEF_BIT_26 | \
                                              DEF_BIT_27 | \
                                              DEF_BIT_28 | \
                                              DEF_BIT_29)

#define  BSP_SER_UART_DBG_PINS               (DEF_BIT_30 | \
                                              DEF_BIT_31)

                                                                 /* ----------- USART BASE ADDRESS DEFINES --------- */

#define  BSP_SER_DBG_BASE_ADDR                   0xFFFFEE00
#define  BSP_SER_DBG_BASE_REG              ((BSP_SER_REG_PTR)(BSP_SER_DBG_BASE_ADDR))

#define  BSP_SER_US0_BASE_ADDR                   0xFFFF8C00
#define  BSP_SER_US0_BASE_REG              ((BSP_SER_REG_PTR)(BSP_SER_US0_BASE_ADDR))

#define  BSP_SER_REG_TO                          0x0000FFFF



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

typedef struct BSP_SER_REG {
    CPU_REG32	 US_CR; 	                                /* Control Register                                   */
    CPU_REG32	 US_MR;                                         /* Mode Register                                      */
    CPU_REG32	 US_IER;                                        /* Interrupt Enable Register                          */
    CPU_REG32	 US_IDR;                                        /* Interrupt Disable Register                         */
    CPU_REG32	 US_IMR;                                        /* Output Disable Registerr                           */
    CPU_REG32	 US_CSR;                                        /* Output Status Register                             */
    CPU_REG32	 US_RHR;                                        /* Receiver Holding Register                          */
    CPU_REG32	 US_THR;                                        /* Transmit Holding Register                          */
    CPU_REG32	 US_BRGR;                                       /* Baud Rate Generator Register                       */
}  BSP_SER_REG, * BSP_SER_REG_PTR;


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

static  BSP_OS_SEM   BSP_SerTxWait;
static  BSP_OS_SEM   BSP_SerRxWait;
static  BSP_OS_SEM   BSP_SerLockByte;
static  BSP_OS_SEM   BSP_SerLockStr;
static  CPU_INT08U   BSP_SerRxData;

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/



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
*                                         BSP_Ser_ISR_Handler()
*
* Description : Serial ISR
*
* Argument(s) : none
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_Ser_ISR_Handler (void)
{
    CPU_INT32U  us_imr;
    CPU_INT32U  us_csr;
    
    us_csr = BSP_SER_DBG_BASE_REG->US_CSR;
    us_imr = BSP_SER_DBG_BASE_REG->US_IMR;
    
    us_csr &= us_imr;
    
#if (BSP_CFG_SER_COMM_SEL == BSP_SER_COMM_UART_DBG)    
    if (DEF_BIT_IS_SET(us_csr, BSP_SER_US_INT_RXRDY)) {
        BSP_SerRxData                = (CPU_INT08U)(BSP_SER_DBG_BASE_REG->US_RHR & 0x00FF);
        BSP_SER_DBG_BASE_REG->US_IDR = BSP_SER_US_INT_RXRDY;                
        BSP_OS_SemPost(BSP_SerRxWait);
    }
    
    if (DEF_BIT_IS_SET(BSP_SER_DBG_BASE_REG->US_CSR, BSP_SER_US_INT_TXEMPTY)) {
        BSP_SER_DBG_BASE_REG->US_IDR = BSP_SER_US_INT_TXEMPTY;                 
        BSP_OS_SemPost(BSP_SerTxWait);    
    }
    
    BSP_IntClr(BSP_PER_ID_SYSC);                                /* Clear the DBG interrupt                            */
#endif

#if (BSP_CFG_SER_COMM_SEL == BSP_SER_COMM_UART_00)    
    if (DEF_BIT_IS_SET(us_csr, BSP_SER_US_INT_RXRDY)) {
        BSP_SerRxData                = (CPU_INT08U)(BSP_SER_US0_BASE_REG->US_RHR & 0x00FF);
        BSP_SER_US0_BASE_REG->US_IDR = BSP_SER_US_INT_RXRDY;                
        BSP_OS_SemPost(BSP_SerRxWait);
    }
    
    if (DEF_BIT_IS_SET(BSP_SER_US0_BASE_REG->US_CSR, BSP_SER_US_INT_TXEMPTY)) {
        BSP_SER_US0_BASE_REG->US_IDR = BSP_SER_US_INT_TXEMPTY;                 
        BSP_OS_SemPost(BSP_SerTxWait);    
    }
    
    BSP_IntClr(BSP_PER_ID_US0);                                 /* Clear the DBG interrupt                            */
#endif

}


/*
*********************************************************************************************************
*                                          BSP_Ser_Init()
*
* Description : Initialize a serial port for communication.
*
* Argument(s) : baud_rate           The desire RS232 baud rate.
*
* Return(s)   : none.
*
* Caller(s)   : Application
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_Ser_Init (CPU_INT32U  baud_rate)
{    
    CPU_INT32U     mclk_freq;

  
    BSP_OS_SemCreate(&BSP_SerTxWait,   0, "Serial Tx Wait");
    BSP_OS_SemCreate(&BSP_SerRxWait,   0, "Serial Rx Wait");        
    BSP_OS_SemCreate(&BSP_SerLockByte, 1, "Serial Byte Lock");    
    BSP_OS_SemCreate(&BSP_SerLockStr,  1, "Serial Str  Lock");    

 
    mclk_freq   = BSP_CPU_MclkFreq();                           /* Get peripheral clock frequency                     */

#if (BSP_CFG_SER_COMM_SEL == BSP_SER_COMM_UART_DBG)
                                                                /* Set GPIOA pins 21 and 22 as DBGU UART pins         */
    BSP_GPIO_Cfg(BSP_GPIO_PORT_C,
                 (BSP_SER_UART_DBG_PINS),            
                  BSP_GPIO_OPT_PER_SEL_A); 

    BSP_SER_DBG_BASE_REG->US_IDR = BSP_SER_US_INT_ALL;  

    BSP_SER_DBG_BASE_REG->US_CR  = BSP_SER_US_CR_RXEN           /* Enable the receiver                                */
                                 | BSP_SER_US_CR_TXEN;          /* Enable the transmitter                             */

    BSP_SER_DBG_BASE_REG->US_MR  = BSP_SER_US_MR_MODE_NORMAL    /* RS232C mode selected                               */
                                 | BSP_SER_US_MR_USCLKS_MCK     /* USART input CLK is MCK                             */
                                 | BSP_SER_US_MR_CHRL_8         /* 8 bit data to be sent                              */
                                 | BSP_SER_US_MR_PAR_NONE       /* No parity bit selected                             */
                                 | BSP_SER_US_MR_NBSTOP_1;      /* 1 stop bit selected                                */

    BSP_SER_DBG_BASE_REG->US_BRGR = (CPU_INT16U)((mclk_freq / baud_rate) / 16);

    
    BSP_PerClkEn(BSP_PER_ID_SYSC);                              /* Enable the DBGU peripheral clock                   */
#endif
    
#if (BSP_CFG_SER_COMM_SEL == BSP_SER_COMM_UART_00)
                                                                /* Set GPIOA pins 06 and 07 as US0 UART pins          */
    BSP_GPIO_Cfg(BSP_GPIO_PORT_A,
                 (BSP_SER_UART_US0_PINS),            
                  BSP_GPIO_OPT_PER_SEL_A); 

    BSP_SER_US0_BASE_REG->US_IDR = BSP_SER_US_INT_ALL;  

    BSP_SER_US0_BASE_REG->US_CR  = BSP_SER_US_CR_RXEN           /* Enable the receiver                                */
                                 | BSP_SER_US_CR_TXEN;          /* Enable the transmitter                             */

    BSP_SER_US0_BASE_REG->US_MR  = BSP_SER_US_MR_MODE_NORMAL    /* RS232C mode selected                               */
                                 | BSP_SER_US_MR_USCLKS_MCK     /* USART input CLK is MCK                             */
                                 | BSP_SER_US_MR_CHRL_8         /* 8 bit data to be sent                              */
                                 | BSP_SER_US_MR_PAR_NONE       /* No parity bit selected                             */
                                 | BSP_SER_US_MR_NBSTOP_1;      /* 1 stop bit selected                                */

    BSP_SER_US0_BASE_REG->US_BRGR = (CPU_INT16U)((mclk_freq / baud_rate) / 16);

    
    BSP_PerClkEn(BSP_PER_ID_US0);                               /* Enable the DBGU peripheral clock                   */

#endif
    
}

/*
*********************************************************************************************************
*                                                Ser_WrByte()
*
* Description : Writes a single byte to a serial port.
*
* Argument(s) : tx_byte     The character to output.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function blocks until room is available in the UART for the byte to be sent.
*********************************************************************************************************
*/

void  BSP_Ser_WrByte (CPU_INT08U tx_byte)
{        
    CPU_BOOLEAN  err;


    err = BSP_OS_SemWait(BSP_SerLockByte, 0);
    
    if (err == DEF_FAIL) {
        return;
    }
    
#if (BSP_CFG_SER_COMM_SEL == BSP_SER_COMM_UART_DBG)        
    BSP_SER_DBG_BASE_REG->US_THR = tx_byte;
    BSP_SER_DBG_BASE_REG->US_IER = BSP_SER_US_INT_TXEMPTY;
    err                          = BSP_OS_SemWait(BSP_SerTxWait, 0);

    if (err == DEF_FAIL) {
        return;
    }

    BSP_SER_DBG_BASE_REG->US_IDR = BSP_SER_US_INT_TXEMPTY;
#endif

#if (BSP_CFG_SER_COMM_SEL == BSP_SER_COMM_UART_00)        
    BSP_SER_US0_BASE_REG->US_THR = tx_byte;
    BSP_SER_US0_BASE_REG->US_IER = BSP_SER_US_INT_TXEMPTY;

    BSP_OS_SemWait(BSP_SerTxWait, 0);

    BSP_SER_US0_BASE_REG->US_IDR = BSP_SER_US_INT_TXEMPTY;
    if (err == DEF_FAIL) {
        return;
    }
#endif    
    BSP_OS_SemPost(BSP_SerLockByte);   
}


/*
*********************************************************************************************************
*                                                BSP_Ser_WrStr()
*
* Description : Transmits a string.
*
* Argument(s) : tx_str      The string that will be transmitted.
*
* Return(s)   : none.
*
* Caller(s)   : Application
*
* Note(s)     : none.
*********************************************************************************************************
*/


void  BSP_Ser_WrStr (CPU_CHAR * tx_str)
{
    CPU_BOOLEAN  err;
    
    
    err = BSP_OS_SemWait(BSP_SerLockStr, 0);
    
    if (err == DEF_FAIL) {
        return;
    }

    while ((*tx_str) != 0) {
        BSP_Ser_WrByte(*tx_str++);
    }
    
    BSP_OS_SemPost(BSP_SerLockStr);
}

/*
*********************************************************************************************************
*                                                BSP_Ser_RdByte()
*
* Description : Receive a single byte.
*
* Argument(s) : none.
*
* Return(s)   : The received byte
*
* Caller(s)   : Application
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT08U  BSP_Ser_RdByte (void)
{
      
    CPU_INT08U   rx_byte;
    

#if (BSP_CFG_SER_COMM_SEL == BSP_SER_COMM_UART_DBG)    
    BSP_SER_DBG_BASE_REG->US_IER = BSP_SER_US_INT_RXRDY; 
    
    BSP_OS_SemWait(BSP_SerRxWait, 0);
    
    BSP_SER_DBG_BASE_REG->US_IDR = BSP_SER_US_INT_RXRDY;         
#endif

#if (BSP_CFG_SER_COMM_SEL == BSP_SER_COMM_UART_00)    
    BSP_SER_US0_BASE_REG->US_IER = BSP_SER_US_INT_RXRDY; 
    
    BSP_OS_SemWait(BSP_SerRxWait, 0);
        
    BSP_SER_US0_BASE_REG->US_IDR = BSP_SER_US_INT_RXRDY; 
#endif

    rx_byte = BSP_SerRxData;

    return (rx_byte);
    
}

/*
*********************************************************************************************************
*                                                BSP_Ser_RdStr()
*
* Description : This function reads a string from a UART.
*
* Argument(s) : rx_str      A pointer to a buffer at which the string can be stored.
*
*               len         The size of the string that will be read.
*
* Return(s)   : none.
*
* Caller(s)   : Application
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_Ser_RdStr (CPU_CHAR    *rx_str,
                     CPU_INT32U   len)
{
    CPU_CHAR     input;
    CPU_CHAR     input_ix;


    input_ix  = 0;
    rx_str[0] = 0;

    
    while (DEF_TRUE)
    {    
        input = BSP_Ser_RdByte();
             
        if ((input == ASCII_CHAR_CARRIAGE_RETURN ) ||
            (input == ASCII_CHAR_LINE_FEED)) {
            BSP_Ser_Printf("\n");
            rx_str[input_ix] = 0;
            break;
        }

        if (input == ASCII_CHAR_BACKSPACE) {
            if (input_ix > 0) {
                BSP_Ser_Printf("\b \b");
                input_ix--;
                rx_str[input_ix] = 0;
            }
        }

        if (ASCII_IsPrint(input)) {
            BSP_Ser_Printf("%c", input);
            rx_str[input_ix] = input;
            input_ix++;
            if (input_ix >= len) {
               input_ix = len;
            }
        }
    }
}

/*
*********************************************************************************************************
*                                                BSP_Ser_Printf()
*
* Description : Formatted outout to the serial port.
*               This funcion reads a string from a serial port. This call blocks until a
*               character appears at the port and the last character is a Carriage
*               Return (0x0D).
*
* Argument(s) : Format string follwing the C format convention.
*
* Return(s)   : none.
*
* Caller(s)   : Application
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_Ser_Printf (CPU_CHAR *format, ...)
{
    static  CPU_CHAR  buffer[80 + 1];
            va_list   vArgs;


    va_start(vArgs, format);
    vsprintf((char *)buffer, (char const *)format, vArgs);
    va_end(vArgs);

    BSP_Ser_WrStr((CPU_CHAR*) buffer);
}

