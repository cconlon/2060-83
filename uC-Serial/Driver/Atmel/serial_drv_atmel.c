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
*
* Filename      : serial_drv_atmel.c
* Version       : V2.00
* Programmer(s) : FGK
*********************************************************************************************************
* Note(s)       : (1) With an appropriate BSP, this device driver will support the UARTs on the Atmel
*                     MCUs & MPUs, including :
*
*                         Atmel AT91SAM7S
*                         Atmel AT91SAM7SE
*                         Atmel AT91SAM7X
*                         Atmel AT91SAM9260
*                         Atmel AT91SAM9261
*                         Atmel AT91SAM9263
*                         Atmel AT91SAM9RL
*                         Atmel AT91SAM9M10
*                         Atmel AT91SAM9G45
*                         Atmel AT91SAM3U
*                         Atmel AT91SAM3S
*                         Atmel AT32UC3A
*                         Atmel AT32UC3B
*                         Atmel AT32AP7000
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <serial.h>
#include  <serial_drv_atmel.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      REGISTER BIT DEFINITIONS
*
* Note(s) : (1) All necessary register bit definitions should be defined within this section.
*********************************************************************************************************
*/

                                                                /* -------------- IxR REGISTER BIT DEFINES ------------ */
#define  IR_RXRDY                                  DEF_BIT_00
#define  IR_TXRDY                                  DEF_BIT_01
#define  IR_RXBRK                                  DEF_BIT_02
#define  IR_OVRE                                   DEF_BIT_05
#define  IR_FRAME                                  DEF_BIT_06
#define  IR_PARE                                   DEF_BIT_07
#define  IR_TIMEOUT                                DEF_BIT_08

                                                                /* -------------- CR REGISTER BIT DEFINES ------------- */
#define  CR_RSTRX                                  DEF_BIT_02
#define  CR_RSTTX                                  DEF_BIT_03
#define  CR_RXEN                                   DEF_BIT_04
#define  CR_RXDIS                                  DEF_BIT_05
#define  CR_TXEN                                   DEF_BIT_06
#define  CR_TXDIS                                  DEF_BIT_07
#define  CR_RSTSTA                                 DEF_BIT_08
#define  CR_RSTIT                                  DEF_BIT_13
#define  CR_RSTNACK                                DEF_BIT_14
#define  CR_DTRDIS                                 DEF_BIT_17
#define  CR_RTSDIS                                 DEF_BIT_19

                                                                /* ------------- CSR REGISTER BIT DEFINES ------------- */
#define  CSR_RXRDY                                 DEF_BIT_00
#define  CSR_TXRDY                                 DEF_BIT_01
#define  CSR_RXBRK                                 DEF_BIT_02
#define  CSR_OVRE                                  DEF_BIT_05
#define  CSR_FRAME                                 DEF_BIT_06
#define  CSR_PARE                                  DEF_BIT_07

                                                                /* -------------- MR REGISTER BIT DEFINES ------------- */
#define  MR_MODE_NORMAL                           (0x00 <<  0)
#define  MR_MODE_HARDWARE                         (0x02 <<  0)

#define  MR_USCLKS_MCK                            (0x00 <<  4)

#define  MR_CHRL_5_BITS                           (0x00 <<  6)
#define  MR_CHRL_6_BITS                           (0x01 <<  6)
#define  MR_CHRL_7_BITS                           (0x02 <<  6)
#define  MR_CHRL_8_BITS                           (0x03 <<  6)

#define  MR_PAR_EVEN                              (0x00 <<  9)
#define  MR_PAR_ODD                               (0x01 <<  9)
#define  MR_PAR_SPACE                             (0x02 <<  9)
#define  MR_PAR_MARK                              (0x03 <<  9)
#define  MR_PAR_NONE                              (0x04 <<  9)

#define  MR_NBSTOP_1_BIT                          (0x00 << 12)
#define  MR_NBSTOP_1_5_BITS                       (0x01 << 12)
#define  MR_NBSTOP_2_BITS                         (0x02 << 12)

#define  MR_OVER                                   DEF_BIT_19


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/

typedef  struct  serial_reg {
    CPU_REG32  CR;
    CPU_REG32  MR;
    CPU_REG32  IER;
    CPU_REG32  IDR;
    CPU_REG32  IMR;
    CPU_REG32  CSR;
    CPU_REG32  RHR;
    CPU_REG32  THR;
    CPU_REG32  BRGR;
    CPU_REG32  RTOR;
    CPU_REG32  TTGR;
} SERIAL_REG;


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
                                                                /* Driver initialization.                               */
static  void        SerialDrv_Init       (SERIAL_ERR     *perr);

                                                                /* Driver open.                                         */
static  void        SerialDrv_Open       (SERIAL_DEV     *pdev,
                                          SERIAL_IF_CFG  *pcfg,
                                          SERIAL_ERR     *perr);

static  void        SerialDrv_Open_DBGU  (SERIAL_DEV     *pdev,
                                          SERIAL_IF_CFG  *pcfg,
                                          SERIAL_ERR     *perr);

                                                                /* Driver close.                                        */
static  void        SerialDrv_Close      (SERIAL_DEV     *pdev,
                                          SERIAL_ERR     *perr);

static  void        SerialDrv_Close_DBGU (SERIAL_DEV     *pdev,
                                          SERIAL_ERR     *perr);

                                                                /* Driver receiver start.                               */
static  void        SerialDrv_RxStart    (SERIAL_DEV     *pdev,
                                          SERIAL_ERR     *perr);

                                                                /* Driver receiver stop.                                */
static  void        SerialDrv_RxStop     (SERIAL_DEV     *pdev,
                                          SERIAL_ERR     *perr);

                                                                /* Driver octet receive.                                */
static  void        SerialDrv_RxOctet    (SERIAL_DEV     *pdev,
                                          CPU_INT08U     *pdatum,
                                          SERIAL_ERR     *perr);

                                                                /* Driver transmitter start.                            */
static  void        SerialDrv_TxStart    (SERIAL_DEV     *pdev,
                                          SERIAL_ERR     *perr);

                                                                /* Driver transmitter stop.                             */
static  void        SerialDrv_TxStop     (SERIAL_DEV     *pdev,
                                          SERIAL_ERR     *perr);

                                                                /* Driver octet transmit.                               */
static  void        SerialDrv_TxOctet    (SERIAL_DEV     *pdev,
                                          CPU_INT08U      datum,
                                          SERIAL_ERR     *perr);

                                                                /* Driver ISR handler.                                  */
static  void        SerialDrv_ISR_Handler(SERIAL_DEV     *pdev,
                                          CPU_INT08U      type);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                 SERIAL INTERFACE DEVICE DRIVER API
*********************************************************************************************************
*********************************************************************************************************
*/

SERIAL_DRV_API  SerialDrv_Atmel_API = {
    SerialDrv_Init,
    SerialDrv_Open,
    SerialDrv_Close,
    SerialDrv_RxStart,
    SerialDrv_RxStop,
    SerialDrv_RxOctet,
    SerialDrv_TxStart,
    SerialDrv_TxStop,
    SerialDrv_TxOctet,
    SerialDrv_ISR_Handler
};

SERIAL_DRV_API  SerialDrv_Atmel_DBGU_API = {
    SerialDrv_Init,
    SerialDrv_Open_DBGU,
    SerialDrv_Close_DBGU,
    SerialDrv_RxStart,
    SerialDrv_RxStop,
    SerialDrv_RxOctet,
    SerialDrv_TxStart,
    SerialDrv_TxStop,
    SerialDrv_TxOctet,
    SerialDrv_ISR_Handler
};


/*$PAGE*/
/*
*********************************************************************************************************
*********************************************************************************************************
*                                     DRIVER INTERFACE FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          SerialDrv_Init()
*
* Description : Initialize serial device driver.
*
* Argument(s) : perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Driver initialized.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_DrvAdd() via 'pdrv_api->Init()'.
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  void  SerialDrv_Init (SERIAL_ERR  *perr)
{
   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                          SerialDrv_Open()
*
* Description : Open a serial device for communication.
*
* Argument(s) : pdev        Pointer to device.
*
*               pcfg        Pointer to interface configuration.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device              opened.
*                               SERIAL_ERR_DRV_OPEN             Device could NOT be opened.
*                               SERIAL_ERR_DRV_INVALID          Device configuration invalid.
*                               SERIAL_ERR_MEM_ALLOC            Memory could NOT be allocated for device
*                                                                   internal data.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Open() via 'pdev->Drv_API->Open()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_Open (SERIAL_DEV     *pdev,
                              SERIAL_IF_CFG  *pcfg,
                              SERIAL_ERR     *perr)
{
    SERIAL_BSP_API  *pdev_bsp;
    SERIAL_REG      *reg;
    CPU_BOOLEAN      flow_ctrl;
    CPU_INT32U       clk_freq;
    CPU_INT32U       baud_rate;
    CPU_INT32U       cd_fp;                                     /* Fractional clk div.                                  */
    CPU_INT32U       mr;                                        /* Mode reg.                                            */


    mr = MR_USCLKS_MCK;                                         /* Use MCK.                                             */

    switch (pcfg->DataBits) {
        case SERIAL_DATABITS_5:
             mr |= MR_CHRL_5_BITS;
             break;


        case SERIAL_DATABITS_6:
             mr |= MR_CHRL_6_BITS;
             break;


        case SERIAL_DATABITS_7:
             mr |= MR_CHRL_7_BITS;
             break;


        case SERIAL_DATABITS_8:
        default:
             mr |= MR_CHRL_8_BITS;
             break;
    }

    switch (pcfg->Parity) {
        case SERIAL_PARITY_ODD:
             mr |= MR_PAR_ODD;
             break;


        case SERIAL_PARITY_EVEN:
             mr |= MR_PAR_EVEN;
             break;


        case SERIAL_PARITY_MARK:
             mr |= MR_PAR_MARK;
             break;


        case SERIAL_PARITY_SPACE:
             mr |= MR_PAR_SPACE;
             break;


        case SERIAL_PARITY_NONE:
        default:
             mr |= MR_PAR_NONE;
             break;
    }

    switch (pcfg->StopBits) {
        case SERIAL_STOPBITS_1:
             mr |= MR_NBSTOP_1_BIT;
             break;


        case SERIAL_STOPBITS_1_5:
             mr |= MR_NBSTOP_1_5_BITS;
             break;


        case SERIAL_STOPBITS_2:
        default:
             mr |= MR_NBSTOP_2_BITS;
             break;
    }

    switch (pcfg->FlowCtrl) {
        case SERIAL_FLOW_CTRL_HARDWARE:
             mr        |= MR_MODE_HARDWARE;
             flow_ctrl  = DEF_ENABLED;
             break;


        case SERIAL_FLOW_CTRL_NONE:
        default:
             mr        |= MR_MODE_NORMAL;
             flow_ctrl  = DEF_DISABLED;
             break;
    }


    pdev_bsp = pdev->Dev_Cfg->BSP_API;

    pdev_bsp->ClkEn(perr);                                      /* En dev-specific HW clk.                              */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }

    reg       = (SERIAL_REG *)pdev->Dev_Cfg->BaseAddr;
    reg->IDR  = IR_RXRDY |                                      /* Dis int.                                             */
                IR_TXRDY |
                IR_RXBRK |
                IR_OVRE  |
                IR_FRAME |
                IR_PARE  |
                IR_TIMEOUT;
    reg->MR   = 0;                                              /* Reset mode             reg.                          */
    reg->RTOR = 0;                                              /* Reset rx'er time-out   reg.                          */
    reg->TTGR = 0;                                              /* Reset tx'er time-guard reg.                          */
    reg->CR   = CR_RSTRX   |
                CR_RSTTX   |
                CR_RSTSTA  |
                CR_RXDIS   |
                CR_TXDIS;

    pdev_bsp->CfgGPIO(flow_ctrl, perr);                         /* Cfg dev-specific GPIO.                               */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }
    pdev_bsp->CfgInt(pdev, perr);                               /* Cfg dev-specific int.                                */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }

    clk_freq  = pdev_bsp->ClkFreqGet();                         /* Get periph clk freq.                                 */
    baud_rate = pcfg->Baudrate;
    if (baud_rate < (clk_freq / 16)) {                          /* 8x  oversampling.                                    */
        mr    |= MR_OVER;                                       /* Set oversampling msk.                                */
        cd_fp  = clk_freq + baud_rate / 2;
        cd_fp /= baud_rate;
    } else {                                                    /* 16x oversampling.                                    */
        mr    &= ~MR_OVER;                                      /* Clr oversampling msk.                                */
        cd_fp  =  clk_freq + baud_rate;
        cd_fp /=  baud_rate * 2;
    }

    reg->CR   =   CR_RXEN |                                     /* En rx'er & tx'er.                                    */
                  CR_TXEN;
    reg->MR   =   mr;
    reg->BRGR = ((cd_fp % 8) << 16) |                           /* Set baud rate.                                       */
                 (cd_fp      >>  3);
    reg->IER  =   IR_OVRE;

   *perr = SERIAL_ERR_NONE;
}


static  void  SerialDrv_Open_DBGU (SERIAL_DEV     *pdev,
                                   SERIAL_IF_CFG  *pcfg,
                                   SERIAL_ERR     *perr)
{
    SERIAL_BSP_API  *pdev_bsp;
    SERIAL_REG      *reg;
    CPU_INT32U       clk_freq;
    CPU_INT32U       baud_rate;
    CPU_INT32U       cd;                                        /* Clk div.                                             */
    CPU_INT32U       mr;                                        /* Mode reg.                                            */


    mr = MR_USCLKS_MCK;                                         /* Use MCK.                                             */

    switch (pcfg->DataBits) {
        case SERIAL_DATABITS_8:
             mr |= MR_CHRL_8_BITS;
             break;


        default:
            *perr = SERIAL_ERR_DRV_INVALID;
             return;
    }

    switch (pcfg->Parity) {
        case SERIAL_PARITY_ODD:
             mr |= MR_PAR_ODD;
             break;


        case SERIAL_PARITY_EVEN:
             mr |= MR_PAR_EVEN;
             break;


        case SERIAL_PARITY_MARK:
             mr |= MR_PAR_MARK;
             break;


        case SERIAL_PARITY_SPACE:
             mr |= MR_PAR_SPACE;
             break;


        case SERIAL_PARITY_NONE:
        default:
             mr |= MR_PAR_NONE;
             break;
    }

    switch (pcfg->StopBits) {
        case SERIAL_STOPBITS_1:
             mr |= MR_NBSTOP_1_BIT;
             break;


        default:
            *perr = SERIAL_ERR_DRV_INVALID;
             break;
    }

    switch (pcfg->FlowCtrl) {
        case SERIAL_FLOW_CTRL_NONE:
             mr |= MR_MODE_NORMAL;
             break;


        default:
            *perr = SERIAL_ERR_DRV_INVALID;
             break;
    }

    pdev_bsp = pdev->Dev_Cfg->BSP_API;

    pdev_bsp->ClkEn(perr);                                      /* En dev-specific HW clk.                              */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }

    reg       = (SERIAL_REG *)pdev->Dev_Cfg->BaseAddr;
    reg->IDR  = IR_RXRDY |                                      /* Dis int.                                             */
                IR_TXRDY |
                IR_RXBRK |
                IR_OVRE  |
                IR_FRAME |
                IR_PARE  |
                IR_TIMEOUT;
    reg->MR   = 0;                                              /* Reset mode reg.                                      */
    reg->CR   = CR_RSTRX   |
                CR_RSTTX   |
                CR_RSTSTA  |
                CR_RXDIS   |
                CR_TXDIS;

    pdev_bsp->CfgGPIO(DEF_DISABLED, perr);                      /* Cfg dev-specific GPIO.                               */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }
    pdev_bsp->CfgInt(pdev, perr);                               /* Cfg dev-specific int.                                */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }

    clk_freq   =  pdev_bsp->ClkFreqGet();                       /* Get periph clk freq.                                 */
    baud_rate  =  pcfg->Baudrate;
    cd         =  clk_freq + (baud_rate * 8);
    cd        /=  baud_rate * 16;

    reg->CR    =  CR_RXEN |                                     /* En rx'er & tx'er.                                    */
                  CR_TXEN;
    reg->MR    =  mr;
    reg->BRGR  =  cd;                                           /* Set baud rate.                                       */
    reg->IER   =  IR_OVRE;

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                          SerialDrv_Close()
*
* Description : Close a serial device for communication.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device closed.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Close() via 'pdev->Drv_API->Close()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_Close (SERIAL_DEV  *pdev,
                               SERIAL_ERR  *perr)
{
    SERIAL_REG  *reg;

                                                                /* -------------------- RESET USART ------------------- */
    reg       = (SERIAL_REG *)pdev->Dev_Cfg->BaseAddr;
    reg->IDR  = IR_RXRDY |                                      /* Dis int.                                             */
                IR_TXRDY |
                IR_RXBRK |
                IR_OVRE  |
                IR_FRAME |
                IR_PARE  |
                IR_TIMEOUT;
    reg->MR   = 0;                                              /* Reset mode             reg.                          */
    reg->RTOR = 0;                                              /* Reset rx'er time-out   reg.                          */
    reg->TTGR = 0;                                              /* Reset tx'er time-guard reg.                          */
    reg->CR   = CR_RSTRX   |
                CR_RSTTX   |
                CR_RSTSTA  |
                CR_RXDIS   |
                CR_TXDIS;

    pdev->Dev_Cfg->BSP_API->ClkDis(perr);                       /* Dis dev-specific HW clk.                             */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialDrv_Close_DBGU (SERIAL_DEV  *pdev,
                                    SERIAL_ERR  *perr)
{
    SERIAL_REG  *reg;

                                                                /* -------------------- RESET USART ------------------- */
    reg       = (SERIAL_REG *)pdev->Dev_Cfg->BaseAddr;
    reg->IDR  = IR_RXRDY |                                      /* Dis int.                                             */
                IR_TXRDY |
                IR_RXBRK |
                IR_OVRE  |
                IR_FRAME |
                IR_PARE  |
                IR_TIMEOUT;
    reg->MR   = 0;                                              /* Reset mode reg.                                      */
    reg->CR   = CR_RSTRX   |
                CR_RSTTX   |
                CR_RSTSTA  |
                CR_RXDIS   |
                CR_TXDIS;

    pdev->Dev_Cfg->BSP_API->ClkDis(perr);                       /* Dis dev-specific HW clk.                             */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_RxStart()
*
* Description : Start receiving data & getting data received interrupts.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device reception started.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Open(),
*               Serial_IF_RxStart() via 'pdev->Drv_API->RxStart()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_RxStart (SERIAL_DEV  *pdev,
                                 SERIAL_ERR  *perr)
{
    SERIAL_REG  *reg;


    reg      = (SERIAL_REG *)pdev->Dev_Cfg->BaseAddr;
    reg->IER =  IR_RXRDY  |                                       /* En rx rdy int.                                       */
                IR_OVRE;

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_RxStop()
*
* Description : Stop receiving data and data received interrupts.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device reception stopped.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Close(),
*               Serial_IF_Rx(),
*               Serial_IF_RxStop() via 'pdev->Drv_API->RxStop()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_RxStop (SERIAL_DEV  *pdev,
                                SERIAL_ERR  *perr)
{
    SERIAL_REG  *reg;


    reg      = (SERIAL_REG *)pdev->Dev_Cfg->BaseAddr;
    reg->IDR =  IR_RXRDY;                                       /* Dis rx rdy int.                                      */

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_RxOctet()
*
* Description : Receive data octet.
*
* Argument(s) : pdev        Pointer to device.
*
*               pdatum      Pointer to variable that will receive the received data octet.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device octet     transmitted,
*                               SERIAL_ERR_DRV_IO_ERR           Device octet NOT transmitted.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_IF_Rx() via 'pdrv_api->RxOctet()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_RxOctet (SERIAL_DEV  *pdev,
                                 CPU_INT08U  *pdatum,
                                 SERIAL_ERR  *perr)
{
    SERIAL_REG  *reg;


    reg = (SERIAL_REG *)pdev->Dev_Cfg->BaseAddr;
    if ((reg->CSR & CSR_RXRDY) == 0) {
       *perr = SERIAL_ERR_DRV_IO_ERR;
        return;
    }

   *pdatum = reg->RHR & 0xFF;
   *perr   = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_TxStart()
*
* Description : Start transmitting data & getting data transmitted interrupts.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device transmission started.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_IF_Tx(),
*               Serial_WrStart() via 'pdev->Drv_API->TxStart()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_TxStart (SERIAL_DEV  *pdev,
                                 SERIAL_ERR  *perr)
{
    SERIAL_REG  *reg;


    reg      = (SERIAL_REG *)pdev->Dev_Cfg->BaseAddr;
    reg->IER =  IR_TXRDY;                                       /* En tx rdy int.                                       */

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_TxStop()
*
* Description : Stop transmitting data & getting data transmitted interrupts.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device transmission stopped.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Close(),
*               Serial_IF_Tx(),
*               Serial_WrStartNext() via 'pdev->Drv_API->TxStop()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_TxStop (SERIAL_DEV  *pdev,
                                SERIAL_ERR  *perr)
{
    SERIAL_REG  *reg;


    reg      = (SERIAL_REG *)pdev->Dev_Cfg->BaseAddr;
    reg->IDR =  IR_TXRDY;                                       /* Dis tx rdy int.                                      */

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_TxOctet()
*
* Description : Transmit data octet.
*
* Argument(s) : pdev        Pointer to device.
*
*               datum       Data octet to transmit.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device octet     transmitted,
*                               SERIAL_ERR_DRV_IO_ERR           Device octet NOT transmitted.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_IF_Tx()   via      'pdrv_api->TxOctet()',
*               Serial_WrStart() via 'pdev->Drv_API->TxOctet()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_TxOctet (SERIAL_DEV  *pdev,
                                 CPU_INT08U   datum,
                                 SERIAL_ERR  *perr)
{
    SERIAL_REG  *reg;


    reg = (SERIAL_REG *)pdev->Dev_Cfg->BaseAddr;
    if ((reg->CSR & CSR_TXRDY) == 0) {
       *perr = SERIAL_ERR_DRV_IO_ERR;
        return;
    }

    reg->THR = datum;

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                       SerialDrv_ISR_Handler()
*
* Description : Handle serial interface's device interrupt service routine (ISR) function(s).
*
* Argument(s) : pdev        Pointer to device.
*
*               type        Device  interrupt type(s) to handle :
*
*                               SERIAL_ISR_TYPE_UNKNOWN         Handle unknown device           ISR(s).
*                               SERIAL_ISR_TYPE_RX              Handle device receive           ISR(s).
*                               SERIAL_ISR_TYPE_RX_OVERRUN      Handle device receive  overrun  ISR(s).
*                               SERIAL_ISR_TYPE_TX_RDY          Handle device transmit ready    ISR(s).
*                               SERIAL_ISR_TYPE_TX_COMPLETE     Handle device transmit complete ISR(s).
*                               SERIAL_ISR_TYPE_ERR             Handle device error             ISR(s).
*
* Return(s)   : None.
*
* Caller(s)   : Device driver(s)' Board Support Package (BSP) Interrupt Service Routine (ISR) handler(s).
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  void  SerialDrv_ISR_Handler (SERIAL_DEV  *pdev,
                                     CPU_INT08U   type)
{
    SERIAL_REG  *reg;
    CPU_INT32U   csr;
    CPU_INT32U   imr;
    CPU_INT08U   datum;


   (void)&type;

    reg  = (SERIAL_REG *)pdev->Dev_Cfg->BaseAddr;
    csr  =  reg->CSR;
    imr  =  reg->IMR;

    csr &=  imr;


                                                                /* --------------- HANDLE RD COMPLETION --------------- */
    if (DEF_BIT_IS_SET(csr, CSR_RXRDY) == DEF_YES) {
        datum = reg->RHR & 0xFF;
        SerialIF_Rx(pdev, datum);                               /* Inform serial core of rx completion.                 */
    }



                                                                /* --------------- HANDLE WR COMPLETION --------------- */
    if (DEF_BIT_IS_SET(csr, CSR_TXRDY) == DEF_YES) {
        SerialIF_Tx(pdev);                                      /* Inform serial core of wr completion.                 */
    }



                                                                /* -------------------- HANDLE ERRS ------------------- */
    if (DEF_BIT_IS_SET(csr, CSR_OVRE) == DEF_YES) {             /* Overrun err.                                         */
        reg->CR = CR_RSTSTA;                                    /* If an overrun occurs, reset status flag.             */
    }
}
