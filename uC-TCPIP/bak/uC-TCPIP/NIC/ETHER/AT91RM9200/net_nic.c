/*
*********************************************************************************************************
*                                              uC/TCP-IP
*                                      The Embedded TCP/IP Suite
*
*                          (c) Copyright 2003-2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/TCP-IP is provided in source form for FREE evaluation, for educational
*               use or peaceful research.  If you plan on using uC/TCP-IP in a commercial
*               product you need to contact Micrium to properly license its use in your
*               product.  We provide ALL the source code for your convenience and to help
*               you experience uC/TCP-IP.  The fact that the source code is provided does
*               NOT mean that you can use it without paying a licensing fee.
*
*               Network Interface Card (NIC) port files provided, as is, for FREE and do
*               NOT require any additional licensing or licensing fee.
*
*               Knowledge of the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        NETWORK INTERFACE CARD
*
*                                           AT91RM9200 EMAC
*
* Filename      : net_nic.c
* Version       : V1.91
* Programmer(s) : JDH
*********************************************************************************************************
* Note(s)       : (1) Supports EMAC section of Atmel's AT91RM9200 microcontroller as described in
*
*                         Atmel Corporation (ATMEL; http://www.atmel.com).
*                         (a) AT91RM9200    (ATMEL AT91RM9200; Revision 1768B-ATARM-08/03).
*
*                 (2) REQUIREs Ethernet Network Interface Layer located in the following network directory :
*
*                         \<Network Protocol Suite>\IF\Ether\
*
*                              where
*                                      <Network Protocol Suite>    directory path for network protocol suite
*
*                 (3) Since the AT91RM9200 EMAC (NIC) module is integrated into the AT91RM9200 microcontroller, 
*                     the endianness of the registers is the same as the CPU, which is little-endian by default.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define    NET_NIC_MODULE
#include  <net.h>


/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

                                                                        /* ----------- AT91RM9200 EMAC RX BUFFERS ------------ */
                                                                        /* Buffer size MUST be divisible by 16 and MUST be     */
#define  AT91RM9200_EMAC_RX_BUF_SIZE                  1536              /* greater than 1518 or 1522 (VLAN) bytes.             */
#define  AT91RM9200_EMAC_TX_BUF_SIZE                  1536

#define  AT91RM9200_EMAC_TX_NUM_BUFS                     2


/*
*********************************************************************************************************
*                                              DATA TYPES
*********************************************************************************************************
*/

typedef struct {
    CPU_INT32U  RxBufAddr;                                              /* Address of RX buffer.                               */
    CPU_INT32U  RxBufStatus;                                            /* Status of RX buffer.                                */
}  AT91RM9200_EMAC_RX_BUF_HDR;                                          /* See 'AT91RM9200 EMAC RECEIVE BUFFER DESCRIPTOR'.    */


/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

                                                                        /* Array of buffer descriptors.                        */
                                                                        /* MUST be 16-word aligned (AT91RM9200 errata #11).    */
static  volatile  AT91RM9200_EMAC_RX_BUF_HDR  *AT91RM9200_EMAC_RxBufHdrs;

                                                                        /* RX buffers.                                         */
static  volatile  CPU_INT08U                  *AT91RM9200_EMAC_RxBuf;

                                                                        /* TX buffer.                                          */
static  volatile  CPU_INT08U                  *AT91RM9200_EMAC_TxBuf;

                                                                        /* RX buffer currently fetched.                        */
static            CPU_INT16U                   AT91RM9200_EMAC_RxBufNbr;

                                                                        /* TX buffer currently fetched.                        */
static            CPU_INT16U                   AT91RM9200_EMAC_TxBufNbr;


/*
*********************************************************************************************************
*                                       AT91RM9200 EMAC REGISTERS
*
* Note(s) : (1) See ATMEL AT91RM9200, pages 573 - 591 for AT91RM9200 register summary.
*
*           (2) Since the AT91RM9200 EMAC (NIC) module is integrated into the AT91RM9200 microcontroller,
*               the indianness of the registers is the same as the CPU, which is little-endian by default.
*
*           (3) Registers referenced as offsets from a base register address.  The base register address
*               MUST be correctly implemented in product-specific I/O functions NetNIC_Rd16() & NetNIC_Wr16().
*               See ATMEL AT91RM9200, pages 31 - 33 for AT91RM9200 internal memory mapping.
*
*           (4) Registers referenced as offsets from a base register address (see Note #2).  The base register
*               address MUST be correctly implemented in product-specific I/O functions NetNIC_Rd32() &
*               NetNIC_Wr32().  See ATMEL AT91RM9200, pages 573 - 591 for AT91RM9200 register summary.
*
*           (5) To protect AT91RM9200 register accesses during multi-thread operation, registers MUST be
*               accessed in critical sections &/or mutual exclusion.
*
*               (a) For single-register, single-access; critical sections are implemented in
*                   AT91RM9200_EMAC_RegRd & AT91RM9200_EMAC_RegWr().
*
*               (b) For read-modify-write register access OR multiple-register-access sequences; critical
*                   sections &/or mutual exclusion enclosing the entire register access/sequence is REQUIRED.
*********************************************************************************************************
*/

                                                                        /* -------- CONFIGURATION REGISTERS ------------------ */
#define  AT91RM9200_REG_EMAC_CTL_OFFSET         0x00                    /* EMAC Control Register                               */
#define  AT91RM9200_REG_EMAC_CFG_OFFSET         0x04                    /* EMAC Configuration Register                         */
#define  AT91RM9200_REG_EMAC_SR_OFFSET          0x08                    /* EMAC Status Register                                */
#define  AT91RM9200_REG_EMAC_TAR_OFFSET         0x0C                    /* EMAC Transmit Address Register                      */
#define  AT91RM9200_REG_EMAC_TCR_OFFSET         0x10                    /* EMAC Transmit Control Register                      */
#define  AT91RM9200_REG_EMAC_TSR_OFFSET         0x14                    /* EMAC Transmit Status Register                       */
#define  AT91RM9200_REG_EMAC_RBQP_OFFSET        0x18                    /* EMAC Receive Buffer Queue Pointer                   */
#define  AT91RM9200_REG_EMAC_RSR_OFFSET         0x20                    /* EMAC Receive Status Register                        */
#define  AT91RM9200_REG_EMAC_ISR_OFFSET         0x24                    /* EMAC Interrupt Status Register                      */
#define  AT91RM9200_REG_EMAC_IER_OFFSET         0x28                    /* EMAC Interrupt Enable Register                      */
#define  AT91RM9200_REG_EMAC_IDR_OFFSET         0x2C                    /* EMAC Interrupt Disable Register                     */
#define  AT91RM9200_REG_EMAC_IMR_OFFSET         0x30                    /* EMAC Interrupt Mask Register                        */
#define  AT91RM9200_REG_EMAC_MAN_OFFSET         0x34                    /* EMAC PHY Maintenance Register                       */

                                                                        /* -------- STATISTIC REGISTERS ---------------------- */
#define  AT91RM9200_REG_EMAC_FRA_OFFSET         0x40                    /* Frames Transmitted OK Register                      */
#define  AT91RM9200_REG_EMAC_SCOL_OFFSET        0x44                    /* Single Collision Frame Register                     */
#define  AT91RM9200_REG_EMAC_MCOL_OFFSET        0x48                    /* Multiple Collision Frame Register                   */
#define  AT91RM9200_REG_EMAC_OK_OFFSET          0x4C                    /* Frames Received OK Register                         */
#define  AT91RM9200_REG_EMAC_SEQE_OFFSET        0x50                    /* Frame Check Sequence Error Register                 */
#define  AT91RM9200_REG_EMAC_ALE_OFFSET         0x54                    /* Alignment Error Register                            */
#define  AT91RM9200_REG_EMAC_DTE_OFFSET         0x58                    /* Deferred Transmission Frame Register                */
#define  AT91RM9200_REG_EMAC_LCOL_OFFSET        0x5C                    /* Late Collision Register                             */
#define  AT91RM9200_REG_EMAC_ECOL_OFFSET        0x60                    /* Excessive Collision Register                        */
#define  AT91RM9200_REG_EMAC_CSE_OFFSET         0x64                    /* Carrier Sense Error Register                        */
#define  AT91RM9200_REG_EMAC_TUE_OFFSET         0x68                    /* Transmit Underrun Error Register                    */
#define  AT91RM9200_REG_EMAC_CDE_OFFSET         0x6C                    /* Code Error Register                                 */
#define  AT91RM9200_REG_EMAC_ELR_OFFSET         0x70                    /* Excessive Length Error Register                     */
#define  AT91RM9200_REG_EMAC_RJB_OFFSET         0x74                    /* Receive Jabber Register                             */
#define  AT91RM9200_REG_EMAC_USF_OFFSET         0x78                    /* Undersize Frame Register                            */
#define  AT91RM9200_REG_EMAC_SQEE_OFFSET        0x7C                    /* SQE Test Error Register                             */
#define  AT91RM9200_REG_EMAC_DRFC_OFFSET        0x80                    /* Discarded RX Frame Register                         */

                                                                        /* -------- ADDRESS REGISTERS ------------------------ */
#define  AT91RM9200_REG_EMAC_HSH_OFFSET         0x90                    /* EMAC Hash Address High [63:32]                      */
#define  AT91RM9200_REG_EMAC_HSL_OFFSET         0x94                    /* EMAC Hash Address Low [31:0]                        */
#define  AT91RM9200_REG_EMAC_SA1L_OFFSET        0x98                    /* EMAC Specific Address 1 Low, First 4 bytes          */
#define  AT91RM9200_REG_EMAC_SA1H_OFFSET        0x9C                    /* EMAC Specific Address 1 High, Last 2 bytes          */
#define  AT91RM9200_REG_EMAC_SA2L_OFFSET        0xA0                    /* EMAC Specific Address 2 Low, First 4 bytes          */
#define  AT91RM9200_REG_EMAC_SA2H_OFFSET        0xA4                    /* EMAC Specific Address 2 High, Last 2 bytes          */
#define  AT91RM9200_REG_EMAC_SA3L_OFFSET        0xA8                    /* EMAC Specific Address 3 Low, First 4 bytes          */
#define  AT91RM9200_REG_EMAC_SA3H_OFFSET        0xAC                    /* EMAC Specific Address 3 High, Last 2 bytes          */
#define  AT91RM9200_REG_EMAC_SA4L_OFFSET        0xB0                    /* EMAC Specific Address 4 Low, First 4 bytes          */
#define  AT91RM9200_REG_EMAC_SA4H_OFFSET        0xB4                    /* EMAC Specific Address 4 High, Last 2 bytes          */


/*
*********************************************************************************************************
*                                       AT91RM9200 REGISTER BITS
*********************************************************************************************************
*/

                                                                        /* -------- EMAC CONTROL REGISTER (CTL) BITS ----------------- */
#define  AT91RM9200_REG_EMAC_CTL_LB_LOW         DEF_BIT_NONE            /* 0: Optional. Loopback signal LOW. Default.                  */
#define  AT91RM9200_REG_EMAC_CTL_LB_HIGH        DEF_BIT_00              /* 1: Optional. Loopback signal HIGH.                          */
#define  AT91RM9200_REG_EMAC_CTL_LBL_DIS        DEF_BIT_NONE            /* 0: Loopback Local DISABLE. Default.                         */
#define  AT91RM9200_REG_EMAC_CTL_LBL_EN         DEF_BIT_01              /* 1: Loopback Local ENABLE.                                   */
#define  AT91RM9200_REG_EMAC_CTL_RE_DIS         DEF_BIT_NONE            /* 0: Receive DISABLE. Default.                                */
#define  AT91RM9200_REG_EMAC_CTL_RE_EN          DEF_BIT_02              /* 1: Receive ENABLE.                                          */
#define  AT91RM9200_REG_EMAC_CTL_TE_DIS         DEF_BIT_NONE            /* 0: Transmit DISABLE. Default.                               */
#define  AT91RM9200_REG_EMAC_CTL_TE_EN          DEF_BIT_03              /* 1: Transmit ENABLE.                                         */
#define  AT91RM9200_REG_EMAC_CTL_MPE_DIS        DEF_BIT_NONE            /* 0: Management Port DISABLE. Default.                        */
#define  AT91RM9200_REG_EMAC_CTL_MPE_EN         DEF_BIT_04              /* 1: Management Port ENABLE.                                  */
#define  AT91RM9200_REG_EMAC_CTL_CSR            DEF_BIT_05              /* Write to clear Statistics Registers.                        */
#define  AT91RM9200_REG_EMAC_CTL_ISR            DEF_BIT_06              /* Write to Increment Statistics Registers FOR TEST USE.       */
#define  AT91RM9200_REG_EMAC_CTL_WES_DIS        DEF_BIT_NONE            /* 0: Normal operation of Statistics Registers. Default.       */
#define  AT91RM9200_REG_EMAC_CTL_WES_EN         DEF_BIT_07              /* 1: Enable Write to Statistics Registers FOR TEST USE.       */
#define  AT91RM9200_REG_EMAC_CTL_BP_DIS         DEF_BIT_NONE            /* 0: Normal operation. Default.                               */
#define  AT91RM9200_REG_EMAC_CTL_BP_EN          DEF_BIT_08              /* 1: Force collision on all RX frames in HDX mode.            */

                                                                        /* -------- EMAC CONFIGURATION REGISTER (CFG) BITS ----------- */
#define  AT91RM9200_REG_EMAC_CFG_SPD_10         DEF_BIT_NONE            /* 0: Set 10  MBps Speed (no functional effect). Default.      */
#define  AT91RM9200_REG_EMAC_CFG_SPD_100        DEF_BIT_00              /* 1: Set 100 MBps Speed (no functional effect).               */
#define  AT91RM9200_REG_EMAC_CFG_HD             DEF_BIT_NONE            /* 0: Half Duplex Operation. Default.                          */
#define  AT91RM9200_REG_EMAC_CFG_FD             DEF_BIT_01              /* 1: Full Duplex Operation.                                   */
#define  AT91RM9200_REG_EMAC_CFG_BR             DEF_BIT_NONE            /* Bit Rate (optional: write 0).                               */
#define  AT91RM9200_REG_EMAC_CFG_CAF_DIS        DEF_BIT_NONE            /* 0: Normal operation. Default.                               */
#define  AT91RM9200_REG_EMAC_CFG_CAF_EN         DEF_BIT_04              /* 1: All valid frames are received.                           */
#define  AT91RM9200_REG_EMAC_CFG_NBC_DIS        DEF_BIT_NONE            /* 0: Normal operation. Default.                               */
#define  AT91RM9200_REG_EMAC_CFG_NBC_EN         DEF_BIT_05              /* 1: No broadcast frames are received.                        */
#define  AT91RM9200_REG_EMAC_CFG_MTI_DIS        DEF_BIT_NONE            /* 0: Multicast Hash DISABLE. Default.                         */
#define  AT91RM9200_REG_EMAC_CFG_MTI_EN         DEF_BIT_06              /* 1: Multicast Hash ENABLE.                                   */
#define  AT91RM9200_REG_EMAC_CFG_UNI_DIS        DEF_BIT_NONE            /* 0: Unicast Hash DISABLE. Default.                           */
#define  AT91RM9200_REG_EMAC_CFG_UNI_EN         DEF_BIT_07              /* 1: Unicast Hash ENABLE.                                     */
#define  AT91RM9200_REG_EMAC_CFG_NORMAL         DEF_BIT_NONE            /* 0: Reception of 802.3   1518 byte (NORMAL) frames. Default. */
#define  AT91RM9200_REG_EMAC_CFG_BIG            DEF_BIT_08              /* 1: Reception of 802.3ac 1522 byte (VLAN) frames.            */
#define  AT91RM9200_REG_EMAC_CFG_EAE            DEF_BIT_NONE            /* External Address Match Enable (optional: write 0).          */
#define  AT91RM9200_REG_EMAC_CFG_CLK_8          DEF_BIT_NONE            /* 00: MDC Clock = MCK divided by 8.                           */
#define  AT91RM9200_REG_EMAC_CFG_CLK_16         (0x01 << 10)            /* 01: MDC Clock = MCK divided by 16.                          */
#define  AT91RM9200_REG_EMAC_CFG_CLK_32         (0x02 << 10)            /* 10: MDC Clock = MCK divided by 32. Default.                 */
#define  AT91RM9200_REG_EMAC_CFG_CLK_64         (0x03 << 10)            /* 11: MDC Clock = MCK divided by 64.                          */
#define  AT91RM9200_REG_EMAC_CFG_RTY_DIS        DEF_BIT_NONE            /* 0: Retry Test DISABLE. Default.                             */
#define  AT91RM9200_REG_EMAC_CFG_RTY_EN         DEF_BIT_12              /* 1: Retry Test ENABLE. (FOR TESTS ONLY).                     */
#define  AT91RM9200_REG_EMAC_CFG_RMII_DIS       DEF_BIT_NONE            /* 0:         MII Mode. Default.                               */
#define  AT91RM9200_REG_EMAC_CFG_RMII_EN        DEF_BIT_13              /* 1: Reduced MII Mode.                                        */

                                                                        /* -------- EMAC STATUS REGISTER (SR) BITS ------------------- */
#define  AT91RM9200_REG_EMAC_SR_LINK            DEF_BIT_00              /* Reserved.                                                   */
#define  AT91RM9200_REG_EMAC_SR_MDIO_UNSET      DEF_BIT_NONE            /* 0: MDIO pin NOT set.                                        */
#define  AT91RM9200_REG_EMAC_SR_MDIO_SET        DEF_BIT_01              /* 1: MDIO pin     set. Default.                               */
#define  AT91RM9200_REG_EMAC_SR_IDLE            DEF_BIT_NONE            /* 0: PHY logic is idle.                                       */
#define  AT91RM9200_REG_EMAC_SR_RUNNING         DEF_BIT_02              /* 1: PHY logic running. Default.                              */

                                                                        /* -------- EMAC TRANSMIT CONTROL REGISTER (TCR) BITS -------- */
#define  AT91RM9200_REG_EMAC_TCR_LEN(_x_)       (_x_ & 0x07FF)          /* Transmit Frame Length minus CRC length (if any).            */
#define  AT91RM9200_REG_EMAC_TCR_CRC            DEF_BIT_NONE            /* 0:        Append CRC on Transmit. Default.                  */
#define  AT91RM9200_REG_EMAC_TCR_NCRC           DEF_BIT_15              /* 1: Do NOT append CRC on Transmit.                           */

                                                                        /* -------- EMAC TRANSMIT STATUS REGISTER (TSR) BITS --------- */
                                                                        /* 0: No event.                                                */
#define  AT91RM9200_REG_EMAC_TSR_OVR            DEF_BIT_00              /* 1: Ethernet Transmit Buffer Overrun.                        */
#define  AT91RM9200_REG_EMAC_TSR_OVR_CLR        DEF_BIT_00              /* 1: Clear event.                                             */
#define  AT91RM9200_REG_EMAC_TSR_COL            DEF_BIT_01              /* 1: Collision Occurred.                                      */
#define  AT91RM9200_REG_EMAC_TSR_COL_CLR        DEF_BIT_01              /* 1: Clear event.                                             */
#define  AT91RM9200_REG_EMAC_TSR_RLE            DEF_BIT_02              /* 1: Retry Limit Exceeded.                                    */
#define  AT91RM9200_REG_EMAC_TSR_RLE_CLR        DEF_BIT_02              /* 1: Clear event.                                             */
#define  AT91RM9200_REG_EMAC_TSR_TXIDLE         DEF_BIT_03              /* 1: Transmitter Idle.                                        */
#define  AT91RM9200_REG_EMAC_TSR_TXIDLE_CLR     DEF_BIT_03              /* 1: Clear event.                                             */
#define  AT91RM9200_REG_EMAC_TSR_BNQ            DEF_BIT_04              /* 1: Ethernet Transmit Buffer not Queued.                     */
#define  AT91RM9200_REG_EMAC_TSR_BNQ_CLR        DEF_BIT_04              /* 1: Clear event.                                             */
#define  AT91RM9200_REG_EMAC_TSR_COMP           DEF_BIT_05              /* 1: Transmit Complete.                                       */
#define  AT91RM9200_REG_EMAC_TSR_COMP_CLR       DEF_BIT_05              /* 1: Clear event.                                             */
#define  AT91RM9200_REG_EMAC_TSR_UND            DEF_BIT_06              /* 1: Transmit Underrun.                                       */
#define  AT91RM9200_REG_EMAC_TSR_UND_CLR        DEF_BIT_06              /* 1: Clear event.                                             */

                                                                        /* -------- EMAC RECEIVE STATUS REGISTER (RSR) BITS ---------- */
                                                                        /* 0: No event.                                                */
#define  AT91RM9200_REG_EMAC_RSR_BNA            DEF_BIT_00              /* 1: Buffer Not Available.                                    */
#define  AT91RM9200_REG_EMAC_RSR_BNA_CLR        DEF_BIT_00              /* 1: Clear event.                                             */
#define  AT91RM9200_REG_EMAC_RSR_REC            DEF_BIT_01              /* 1: Frame Received.                                          */
#define  AT91RM9200_REG_EMAC_RSR_REC_CLR        DEF_BIT_01              /* 1: Clear event.                                             */
#define  AT91RM9200_REG_EMAC_RSR_OVR            DEF_BIT_02              /* 1: RX Overrun.                                              */
#define  AT91RM9200_REG_EMAC_RSR_OVR_CLR        DEF_BIT_02              /* 1: Clear event.                                             */

                                                                        /* -------- EMAC INTERRUPT STATUS REGISTER  (ISR) BITS ------- */
                                                                        /* NOTE: Registers are cleared on read.                        */

                                                                        /* -------- EMAC INTERRUPT ENABLE REGISTER  (IER) BITS ------- */
                                                                        /* -------- EMAC INTERRUPT DISABLE REGISTER (IDR) BITS ------- */
                                                                        /* -------- EMAC INTERRUPT MASK REGISTER    (IMR) BITS ------- */
                                                                        /* 0: The corresponding interrupt is enabled.                  */
                                                                        /* 1: The corresponding interrupt is disabled.                 */
                                                                        /* NOTE: The interrupt is disabled when the corresponding bit  */
                                                                        /* is set. This is non-standard for AT91 products as generally */
                                                                        /* a mask bit set enables the interrupt.                       */

#define  AT91RM9200_REG_EMAC_INT_DONE           DEF_BIT_00              /* PHY Management Complete.                                    */
#define  AT91RM9200_REG_EMAC_INT_RCOM           DEF_BIT_01              /* Receive Complete.                                           */
#define  AT91RM9200_REG_EMAC_INT_RBNA           DEF_BIT_02              /* Receive Buffer Not Available.                               */
#define  AT91RM9200_REG_EMAC_INT_TOVR           DEF_BIT_03              /* Transmit Buffer Overrun.                                    */
#define  AT91RM9200_REG_EMAC_INT_TUND           DEF_BIT_04              /* Transmit Buffer Underrun.                                   */
#define  AT91RM9200_REG_EMAC_INT_RTRY           DEF_BIT_05              /* Retry Limit.                                                */
#define  AT91RM9200_REG_EMAC_INT_TBRE           DEF_BIT_06              /* Transmit Buffer Register Empty.                             */
#define  AT91RM9200_REG_EMAC_INT_TCOM           DEF_BIT_07              /* Transmit Complete.                                          */
#define  AT91RM9200_REG_EMAC_INT_TIDLE          DEF_BIT_08              /* Transmit Idle.                                              */
#define  AT91RM9200_REG_EMAC_INT_LINK           DEF_BIT_09              /* Link Pin Changed Value (optional).                          */
#define  AT91RM9200_REG_EMAC_INT_ROVR           DEF_BIT_10              /* RX Overrun.                                                 */
#define  AT91RM9200_REG_EMAC_INT_ABT            DEF_BIT_11              /* Abort on DMA transfer.                                      */

                                                                        /* -------- EMAC PHY MAINTENANCE REGISTER (MAN) BITS --------- */
#define  AT91RM9200_REG_EMAC_MAN_DATA(_x_)      (_x_ & 0xFFFF)          /* PHY Read/Write Data Register.                               */
#define  AT91RM9200_REG_EMAC_MAN_CODE           (0x02 << 16)            /* IEEE Code. MUST have value of 10.                           */
#define  AT91RM9200_REG_EMAC_MAN_REGA(_x_)      ((_x_ & 0x1F) << 18)    /* Specifies the register in the PHY to access.                */
#define  AT91RM9200_REG_EMAC_MAN_PHYA(_x_)      ((_x_ & 0x1F) << 23)    /* PHY address. Normally 0.                                    */
#define  AT91RM9200_REG_EMAC_MAN_WRITE          (0x01 << 28)            /* 01: Transfer is a write.                                    */
#define  AT91RM9200_REG_EMAC_MAN_READ           (0x02 << 28)            /* 10: Transfer is a read.                                     */
#define  AT91RM9200_REG_EMAC_MAN_HIGH           DEF_BIT_30              /* MUST be set to 1.                                           */
#define  AT91RM9200_REG_EMAC_MAN_LOW            DEF_BIT_NONE            /* MUST be set to 0.                                           */


/*
*********************************************************************************************************
*                                 AT91RM9200 RECEIVE BUFFER DESCRIPTOR
*********************************************************************************************************
*/

                                                                        /* -------- WORD 0 - ADDRESS --------------------------------- */
#define  AT91RM9200_EMAC_RXBUF_ADD_MAC_OWNED    DEF_BIT_NONE            /* 0: EMAC owns buffer.                                        */
#define  AT91RM9200_EMAC_RXBUF_ADD_SW_OWNED     DEF_BIT_00              /* 1: Software owns the buffer.                                */
#define  AT91RM9200_EMAC_RXBUF_ADD_NO_WRAP      DEF_BIT_NONE            /* 0: This is NOT the last buffer in the ring.                 */
#define  AT91RM9200_EMAC_RXBUF_ADD_WRAP         DEF_BIT_01              /* 1: This is     the last buffer in the ring.                 */
#define  AT91RM9200_EMAC_RXBUF_ADD_BASE_MASK    0xFFFFFFFC              /* Base address of the receive buffer                          */

                                                                        /* -------- WORD 1 - STATUS ---------------------------------- */
#define  AT91RM9200_EMAC_RXBUF_STAT_LEN_MASK    0x07FF                  /* Length of frame including FCS.                              */
#define  AT91RM9200_EMAC_RXBUF_STAT_LOC4        DEF_BIT_23              /* Local address match (Specific address 4 match).             */
#define  AT91RM9200_EMAC_RXBUF_STAT_LOC3        DEF_BIT_24              /* Local address match (Specific address 3 match).             */
#define  AT91RM9200_EMAC_RXBUF_STAT_LOC2        DEF_BIT_25              /* Local address match (Specific address 2 match).             */
#define  AT91RM9200_EMAC_RXBUF_STAT_LOC1        DEF_BIT_26              /* Local address match (Specific address 1 match).             */
#define  AT91RM9200_EMAC_RXBUF_STAT_UNK         DEF_BIT_27              /* Unknown source address (reserved for future use).           */
#define  AT91RM9200_EMAC_RXBUF_STAT_EXT         DEF_BIT_28              /* External address (optional).                                */
#define  AT91RM9200_EMAC_RXBUF_STAT_UNI         DEF_BIT_29              /* Unicast hash match.                                         */
#define  AT91RM9200_EMAC_RXBUF_STAT_MULTI       DEF_BIT_30              /* Multicast hash match.                                       */
#define  AT91RM9200_EMAC_RXBUF_STAT_BCAST       DEF_BIT_31              /* Global all ones broadcast address detected.                 */


/*
*********************************************************************************************************
*                                       LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void        NetNIC_RxISR_Handler          (void);               /* ISR for RX interrupts.                              */
static  void        NetNIC_TxISR_Handler          (void);               /* ISR for TX interrupts.                              */
static  void        NetNIC_TxPktDiscard           (NET_ERR     *perr);


                                                                        /* -------- AT91RM9200 EMAC FNCTS -------------------- */
static  void        AT91RM9200_EMAC_Init          (void);


                                                                        /* -------- AT91RM9200 EMAC RX FNCTS ----------------- */
static  void        AT91RM9200_EMAC_RxEn          (void);
static  void        AT91RM9200_EMAC_RxIntEn       (void);
static  CPU_INT16U  AT91RM9200_EMAC_RxPktGetSize  (void);
static  void        AT91RM9200_EMAC_RxPkt         (void        *ppkt,
                                                   CPU_INT16U   size,
                                                   NET_ERR     *perr);
static  void        AT91RM9200_EMAC_RxPktDiscard  (CPU_INT16U   size);


                                                                        /* -------- AT91RM9200 EMAC TX FNCTS ----------------- */
static  void        AT91RM9200_EMAC_TxEn          (void);
static  void        AT91RM9200_EMAC_TxIntEn       (void);
static  void        AT91RM9200_EMAC_TxPkt         (void        *ppkt,
                                                   CPU_INT16U   size,
                                                   NET_ERR     *perr);
static  void        AT91RM9200_EMAC_TxPktPrepare  (void        *ppkt,
                                                   CPU_INT16U   size,
                                                   NET_ERR     *perr);


                                                                        /* -------- PHYTER REG ACCESS FNCTS ------------------ */
static  CPU_INT16U  AT91RM9200_EMAC_PhyRegRd      (CPU_INT08U   phy,
                                                   CPU_INT08U   reg);
static  void        AT91RM9200_EMAC_PhyRegWr      (CPU_INT08U   phy,
                                                   CPU_INT08U   reg,
                                                   CPU_INT16U   val);


                                                                        /* -------- HELPER FUNCTIONS ------------------------- */
static  CPU_INT16U  AT91RM9200_EMAC_RxBufNbrInc     (CPU_INT16U   val);


/*
*********************************************************************************************************
*                                             NetNIC_Init()
*
* Description : (1) Initialize Network Interface Card :
*                   (a) Perform NIC Layer OS initialization
*                   (b) Initialize NIC status
*                   (c) Initialize NIC statistics & error counters
*                   (d) Initialize AT91RM9200
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Net_Init().
*********************************************************************************************************
*/

void  NetNIC_Init (NET_ERR  *perr)
{
                                                                        /* --------------- PERFORM NIC/OS INIT --------------- */
    NetOS_NIC_Init(perr);                                               /* Create NIC objs.                                    */
    if (*perr != NET_OS_ERR_NONE) {
         return;
    }

                                                                        /* ----------------- INIT NIC STATUS ----------------- */
    NetNIC_ConnStatus           =  DEF_OFF;

                                                                        /* ------------- INIT NIC STAT & ERR CTRS ------------ */
#if (NET_CTR_CFG_STAT_EN        == DEF_ENABLED)
    NetNIC_StatRxPktCtr         =  0;
    NetNIC_StatTxPktCtr         =  0;
#endif

#if (NET_CTR_CFG_ERR_EN         == DEF_ENABLED)
    NetNIC_ErrRxPktDiscardedCtr =  0;
    NetNIC_ErrTxPktDiscardedCtr =  0;
#endif

                                                                        /* ----------------- INIT AT91RM9200 ----------------- */
    AT91RM9200_EMAC_Init();

   *perr = NET_NIC_ERR_NONE;
}


/*
*********************************************************************************************************
*                                            NetNIC_IntEn()
*
* Description : Enable NIC interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Net_Init().
*********************************************************************************************************
*/

void  NetNIC_IntEn (NET_ERR  *perr)
{
    AT91RM9200_EMAC_RxIntEn();
    AT91RM9200_EMAC_TxIntEn();

    *perr = NET_NIC_ERR_NONE;
}


/*
*********************************************************************************************************
*                                        NetNIC_ConnStatusGet()
*
* Description : Get NIC's network connection status.
*
*               (1) Obtaining the NIC's network connection status is encapsulated in this function for the
*                   possibility that obtaining a NIC's connection status requires a non-trivial procedure.
*
*
* Argument(s) : none.
*
* Return(s)   : NIC network connection status :
*
*                   DEF_OFF     Network connection DOWN.
*                   DEF_ON      Network connection UP.
*
* Caller(s)   : NetIF_Pkt_Tx().
*********************************************************************************************************
*/

CPU_BOOLEAN  NetNIC_ConnStatusGet (void)
{
    return (NetNIC_ConnStatus);
}


/*
*********************************************************************************************************
*                                         NetNIC_ISR_Handler()
*
* Description : (1) Decode ISR & call appropriate ISR handler :
*
*                   (a) AT91RM9200 Receive Buffer Not Available ISR     NetNIC_RxPktDiscard().
*                   (b) AT91RM9200 Receive  ISR                         NetNIC_RxISR_Handler().
*                   (c) AT91RM9200 Transmit ISR                         NetNIC_TxISR_Handler().
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : NetNIC_ISR() [see Note #2].
*
* Note(s)     : (2) AT91RM9200 ISR  MUST call NetNIC_ISR_Handler() & MUST be developer-implemented in
*
*                       \<Your Product Application>\net_isr*.*
*
*                           where
*                                   <Your Product Application>    directory path for Your Product's Application.
*
*               (3) This function clears the interrupt source(s) on an external interrupt controller &, if
*                   ENABLED, MUST be developer-implemented in
*
*                       \<Your Product Application>\net_isr.c
*
*                           where
*                                   <Your Product Application>    directory path for Your Product's Application.
*********************************************************************************************************
*/

void  NetNIC_ISR_Handler (void)
{
    CPU_INT16U  int_reg;


    int_reg = AT91RM9200_EMAC_RegRd(AT91RM9200_REG_EMAC_ISR_OFFSET);    /* -------------- DETERMINE NIC INT SRC -------------- */
                                                                        /* Interrupts are acknowledged when ISR read.          */
                                                                        /* See 'ATMEL AT91RM9200; Revision 1768B-ATARM–08/03'  */
                                                                        /* p. 586.                                             */

                                                                        /* --------------- HANDLE ENABLED INTS --------------- */
    if (int_reg & AT91RM9200_REG_EMAC_INT_TCOM) {
        NetNIC_TxISR_Handler();
    }

    if (int_reg & (AT91RM9200_REG_EMAC_INT_RBNA |
                   AT91RM9200_REG_EMAC_INT_RCOM |
                   AT91RM9200_REG_EMAC_INT_ROVR)) {
        AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_RSR_OFFSET,           /* Clear all receiver errors.                          */
                             (AT91RM9200_REG_EMAC_RSR_BNA_CLR  |
                              AT91RM9200_REG_EMAC_RSR_REC_CLR  |
                              AT91RM9200_REG_EMAC_RSR_OVR_CLR));
        NetNIC_RxISR_Handler();
    }

#if (NET_NIC_CFG_INT_CTRL_EN == DEF_ENABLED)
    NetNIC_IntClr();                                                    /* Clr int ctrl'r AT91RM9200 int (see Note #3).        */
#endif
}


/*
*********************************************************************************************************
*                                         NetNIC_RxPktGetSize()
*
* Description : Get network packet size from NIC.
*
* Argument(s) : none.
*
* Return(s)   : Size, in octets, of NIC's next network packet.
*
* Caller(s)   : NetIF_RxTaskHandler().
*********************************************************************************************************
*/

CPU_INT16U  NetNIC_RxPktGetSize (void)
{
    CPU_INT16U  size;


    size = AT91RM9200_EMAC_RxPktGetSize();
    return (size);
}


/*
*********************************************************************************************************
*                                            NetNIC_RxPkt()
*
* Description : Read network packet from NIC into buffer.
*
* Argument(s) : ppkt        Pointer to memory buffer to receive NIC packet.
*
*               size        Number of packet frame octets to read into buffer.
*
*               perr        Pointer to variable that will hold the return error code from this function :
*
*                               NET_NIC_ERR_NONE            Packet successfully read.
*                               NET_ERR_INIT_INCOMPLETE     Network initialization NOT complete.
*                               NET_NIC_ERR_NULL_PTR        Argument 'ppkt' passed a NULL pointer.
*                               NET_NIC_ERR_INVALID_SIZE    Invalid size.
*
* Return(s)   : none.
*
* Caller(s)   : NetIF_RxTaskHandler().
*
* Note(s)     : (1) NetNIC_RxPkt() blocked until network initialization completes; perform NO action.
*********************************************************************************************************
*/

void  NetNIC_RxPkt (void        *ppkt,
                    CPU_INT16U   size,
                    NET_ERR     *perr)
{
#if (NET_CTR_CFG_STAT_EN     == DEF_ENABLED)
#if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
    CPU_SR   cpu_sr;
#endif
#endif


    if (Net_InitDone != DEF_YES) {                                      /* If init NOT complete, exit rx (see Note #2).        */
       *perr = NET_ERR_INIT_INCOMPLETE;
        return;
    }

#if (NET_ERR_CFG_ARG_CHK_DBG_EN == DEF_ENABLED)                         /* ------------------- VALIDATE PTR ------------------ */
    if (ppkt == (void *)0) {
       *perr = NET_NIC_ERR_NULL_PTR;
        return;
    }

                                                                        /* ------------------- VALIDATE SIZE ----------------- */
    if (size < NET_IF_FRAME_MIN_SIZE) {
       *perr = NET_NIC_ERR_INVALID_SIZE;
        return;
    }
#endif

    AT91RM9200_EMAC_RxPkt(ppkt, size, perr);                            /* Rd rx pkt from NIC.                                 */
    if (*perr != NET_NIC_ERR_NONE) {
        return;
    }

    NET_CTR_STAT_INC(NetNIC_StatRxPktCtr);

    *perr = NET_NIC_ERR_NONE;
}


/*
*********************************************************************************************************
*                                         NetNIC_RxPktDiscard()
*
* Description : Discard network packet from NIC to free NIC packet frames for new receive packets.
*
* Argument(s) : size        Number of packet frame octets.
*
*               perr        Pointer to variable that will hold the return error code from this function :
*
*                               NET_NIC_ERR_NONE            Packet successfully discarded.
*                               NET_ERR_INIT_INCOMPLETE     Network initialization NOT complete.
*
* Return(s)   : none.
*
* Caller(s)   : NetIF_RxTaskHandler().
*
* Note(s)     : (1) NetNIC_RxPktDiscard() blocked until network initialization completes; perform NO action.
*
*               (2) #### 'perr' may NOT be necessary (remove before product release if unnecessary).
*********************************************************************************************************
*/

void  NetNIC_RxPktDiscard (CPU_INT16U   size,
                           NET_ERR     *perr)
{
#if (NET_CTR_CFG_ERR_EN      == DEF_ENABLED)
#if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
    CPU_SR   cpu_sr;
#endif
#endif


    if (Net_InitDone != DEF_YES) {                                      /* If init NOT complete, exit discard (see Note #1).   */
       *perr = NET_ERR_INIT_INCOMPLETE;
        return;
    }

    AT91RM9200_EMAC_RxPktDiscard(size);

    NET_CTR_ERR_INC(NetNIC_ErrRxPktDiscardedCtr);

   *perr = NET_NIC_ERR_NONE;
}


/*
*********************************************************************************************************
*                                         NetNIC_TxPktPrepare()
*
* Description : Prepare to transmit data packets from network driver layer to network interface card.
*
* Argument(s) : ppkt        Pointer to memory buffer to transmit NIC packet.
*
*               size        Number of packet frame octets to write to frame.
*
*               perr        Pointer to variable that will hold the return error code from this function :
*
*                               NET_NIC_ERR_NONE            Packet successfully transmitted.
*                               NET_ERR_INIT_INCOMPLETE     Network initialization NOT complete.
*
*                                                           - RETURNED BY NetNIC_TxPktDiscard() : -
*                               NET_ERR_TX                  Transmit error; packet discarded.
*
* Return(s)   : none.
*
* Caller(s)   : NetIF_Pkt_Tx().
*
* Note(s)     : (1) NetNIC_TxPktPrepare() blocked until network initialization completes; perform NO action.
*********************************************************************************************************
*/

void  NetNIC_TxPktPrepare (void        *ppkt,
                           CPU_INT16U   size,
                           NET_ERR     *perr)
{
    if (Net_InitDone != DEF_YES) {                                      /* If init NOT complete, exit tx (see Note #1).        */
       *perr = NET_ERR_INIT_INCOMPLETE;
        return;
    }

#if (NET_ERR_CFG_ARG_CHK_DBG_EN == DEF_ENABLED)                         /* ------------------- VALIDATE PTR ------------------ */
    if (ppkt == (void *)0) {
        NetNIC_TxPktDiscard(perr);
        return;
    }
#endif

    AT91RM9200_EMAC_TxPktPrepare(ppkt, size, perr);                     /* Preparte to tx pkt to AT91RM9200.                   */

    if (*perr != NET_NIC_ERR_NONE) {
        NetNIC_TxPktDiscard(perr);
        return;
    }

    *perr = NET_NIC_ERR_NONE;
}


/*
*********************************************************************************************************
*                                            NetNIC_TxPkt()
*
* Description : Transmit data packets from network driver layer to network interface card.
*
* Argument(s) : ppkt        Pointer to memory buffer to transmit NIC packet.
*
*               size        Number of packet frame octets to write to frame.
*
*               perr        Pointer to variable that will hold the return error code from this function :
*
*                               NET_NIC_ERR_NONE            Packet successfully transmitted.
*                               NET_ERR_INIT_INCOMPLETE     Network initialization NOT complete.
*
*                                                           - RETURNED BY NetNIC_TxPktDiscard() : -
*                               NET_ERR_TX                  Transmit error; packet discarded.
*
* Return(s)   : none.
*
* Caller(s)   : NetIF_Pkt_Tx().
*
* Note(s)     : (1) NetNIC_TxPkt() blocked until network initialization completes; perform NO action.
*********************************************************************************************************
*/

void  NetNIC_TxPkt (void        *ppkt,
                    CPU_INT16U   size,
                    NET_ERR     *perr)
{
#if (NET_CTR_CFG_STAT_EN     == DEF_ENABLED)
#if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
    CPU_SR   cpu_sr;
#endif
#endif


    if (Net_InitDone != DEF_YES) {                                      /* If init NOT complete, exit tx (see Note #1).        */
       *perr = NET_ERR_INIT_INCOMPLETE;
        return;
    }

#if (NET_ERR_CFG_ARG_CHK_DBG_EN == DEF_ENABLED)                         /* ------------------- VALIDATE PTR ------------------ */
    if (ppkt == (void *)0) {
        NetNIC_TxPktDiscard(perr);
        return;
    }
#endif

    AT91RM9200_EMAC_TxPkt(ppkt, size, perr);                            /* Tx pkt to AT91RM9200.                               */

    if (*perr != NET_NIC_ERR_NONE) {
        NetNIC_TxPktDiscard(perr);
        return;
    }

    NET_CTR_STAT_INC(NetNIC_StatTxPktCtr);

    *perr = NET_NIC_ERR_NONE;
}


/*
*********************************************************************************************************
*                                           NetNIC_PhyRegRd()
*
* Description : Read PHY data value.
*
* Argument(s) : phy         PHY address, normally 0.
*               reg         PHY register.
*
*               perr        Pointer to variable that will hold the return error code from this function :
*
*                               NET_NIC_ERR_NONE                Physical register successfully read.
*
* Return(s)   : Data read from PHY register.
*
* Caller(s)   : NetNIC_PhyInit(),
*               NetNIC_PhyIsLinkUp().
*
* Note(s)     : Register ALWAYS reads 16-bit data values.
*********************************************************************************************************
*/

CPU_INT16U  NetNIC_PhyRegRd (CPU_INT08U   phy,
                             CPU_INT08U   reg,
                             NET_ERR     *perr)
{
    CPU_INT16U  reg_val;


    reg_val = AT91RM9200_EMAC_PhyRegRd(phy, reg);
   *perr    = NET_NIC_ERR_NONE;

    return (reg_val);
}


/*
*********************************************************************************************************
*                                           NetNIC_PhyRegWr()
*
* Description : Write PHY data value.
*
* Argument(s) : phy         PHY address, normally 0.
*               reg         PHY register.
*               val         Data to write to PHY register.
*
*               perr        Pointer to variable that will hold the return error code from this function :
*
*                               NET_NIC_ERR_NONE                Physical register successfully written.
*
* Return(s)   : none.
*
* Caller(s)   : NetNIC_PhyInit(),
*               NetNIC_PhyIsLinkUp().
*
* Note(s)     : Register ALWAYS writes 16-bit data values.
*********************************************************************************************************
*/

void  NetNIC_PhyRegWr (CPU_INT08U   phy,
                       CPU_INT08U   reg,
                       CPU_INT16U   val,
                       NET_ERR     *perr)
{
    AT91RM9200_EMAC_PhyRegWr(phy, reg, val);
   *perr = NET_NIC_ERR_NONE;
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        NetNIC_RxISR_Handler()
*
* Description : Signal Network Interface Receive Task that a receive packet is available.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : NetNIC_ISR_Handler().
*
* Note(s)     : (1) NetNIC_ISR_Handler() decodes AT91RM9200 Receive ISR & calls NetNIC_RxISR_Handler().
*********************************************************************************************************
*/

static  void  NetNIC_RxISR_Handler (void)
{
    NET_ERR  err;


                                                                        /* Interrupts are acknowledged when ISR read.          */
                                                                        /* See 'ATMEL AT91RM9200; Revision 1768B-ATARM–08/03'  */
                                                                        /* p. 586.                                             */
                                                                        /* ISR are previously read in NetNIC_ISR_Handler().    */

                                                                        /* ------------- POST RX COMPLETE SIGNAL ------------- */
    NetOS_IF_RxTaskSignal(&err);                                        /* Signal Net IF Rx Task of NIC rx pkt.                */
}


/*
*********************************************************************************************************
*                                        NetNIC_TxISR_Handler()
*
* Description : (1) Clear transmit interrupt &/or transmit errors :
*
*                   (a) Acknowledge transmit interrupt.
*                   (b) Post transmit FIFO empty signal.
*
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : NetNIC_ISR_Handler().
*
* Note(s)     : (2) NetNIC_ISR_Handler() decodes AT91RM9200 Transmit ISR & calls NetNIC_TxISR_Handler().
*********************************************************************************************************
*/

static  void  NetNIC_TxISR_Handler (void)
{
                                                                        /* Interrupts are acknowledged when ISR read.          */
                                                                        /* See 'ATMEL AT91RM9200; Revision 1768B-ATARM–08/03'  */
                                                                        /* p. 586.                                             */
                                                                        /* ISR are previously read in NetNIC_ISR_Handler().    */

                                                                        /* ------------- POST TX COMPLETE SIGNAL ------------- */
    NetOS_NIC_TxRdySignal();
}


/*
*********************************************************************************************************
*                                         NetNIC_TxPktDiscard()
*
* Description : On any Transmit errors, set error.
*
* Argument(s) : pbuf        Pointer to network buffer.
*
*               perr        Pointer to variable that will hold the return error code from this function :
*
*                               NET_ERR_TX                  Transmit error; packet discarded.
*
* Return(s)   : none.
*
* Caller(s)   : NetNIC_TxPkt().
*********************************************************************************************************
*/

static  void  NetNIC_TxPktDiscard (NET_ERR  *perr)
{
#if (NET_CTR_CFG_ERR_EN      == DEF_ENABLED)
#if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
    CPU_SR   cpu_sr;
#endif
#endif


    NET_CTR_ERR_INC(NetNIC_ErrTxPktDiscardedCtr);

   *perr = NET_ERR_TX;
}


/*
*********************************************************************************************************
*                                        AT91RM9200_EMAC_Init()
*
* Description : (1) Initialize & start AT91RM9200 :
*
*                   (a) Initialize Interrupts.
*                   (b) Initialize Registers.
*                   (c) Initialize MAC Address.
*                   (d) Initialize RX Buffer descriptors.
*                   (e) Initialize Auto Negotiation.
*                   (f) Enable     Receiver/Transmitter.
*                   (g) Initialize External Interrupt Controller.  See Note #4.
*
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : NetNIC_Init().
*
* Note(s)     : (2) See this 'net_nic.c  AT91RM9200 REGISTER BITS' for register bit summary.
*
*               (3) (a) Assumes MAC address to set has previously been initialized by
*
*                       (1) AT91RM9200's EEPROM             for AT91RM9200_EMAC_MAC_ADDR_SEL_EEPROM
*                       (2) Application code                for AT91RM9200_EMAC_MAC_ADDR_SEL_CFG
*
*                   (b) AT91RM9200 NIC module allow configuration of four MAC addresses that will be
*                       used to accept or reject an ethernet packet.  This driver uses one of these.
*                       The method of configuring MAC addresses in AT91RM9200 defined in ATMEL document
*                       is erroneous / unclear.  Here is some more information determinated by
*                       experimentation:
*
*                       The register AT91RM9200_REG_EMAC_SA1L will contain the first four bytes of the
*                       MAC address and the register AT91RM9200_REG_EMAC_SA1H the two remaining bytes,
*                       organized in this way:
*
*                       Note: IEEE 802 standard defines MAC bytes 0:2 to the manufactured ID and bytes
*                       3:5 to the unit ID.
*
*                       AT91RM9200_REG_EMAC_SA1L register
*                           -------------------------------------
*                       MSB | Byte 3 | Byte 2 | Byte 1 | Byte 0 | LSB
*                           -------------------------------------
*
*                       AT91RM9200_REG_EMAC_SA1H register
*                           -------------------------------------
*                       MSB |    0   |    0   | Byte 5 | Byte 4 | LSB
*                           -------------------------------------
*
*                   (c) Since MAC address register configuration accesses two registers in sequence,
*                       ATMEL has added hardware protection mechanism in these registers.  So you MUST
*                       write to the AT91RM9200_REG_EMAC_SA1L register first and AT91RM9200_REG_EMAC_SA1H
*                       then. See 'ATMEL AT91RM9200; Revision 1768B-ATARM–08/03', p. 574 for more details.
*
*               (4) Interrupts MUST be enabled ONLY after ALL network initialization is complete (see also
*                   'net.c  Net_Init()  Note #4c').
*
*               (5) This function initializes an external interrupt controller (if present) &, if ENABLED,
*                   MUST be developer-implemented in
*
*                       \<Your Product Application>\net_isr.c
*
*                           where
*                                   <Your Product Application>    directory path for Your Product's Application.
*********************************************************************************************************
*/

static  void  AT91RM9200_EMAC_Init (void)
{
#if (AT91RM9200_EMAC_CFG_MAC_ADDR_SEL == AT91RM9200_EMAC_MAC_ADDR_SEL_EEPROM)
    CPU_INT32U  reg_val;
#endif
    CPU_INT08U  i;
    NET_ERR     err;


                                                                        /* --------------- DISABLE ALL INTERRUPTS ------------ */
    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_IDR_OFFSET,
                    (AT91RM9200_REG_EMAC_INT_DONE     |
                     AT91RM9200_REG_EMAC_INT_RCOM     |
                     AT91RM9200_REG_EMAC_INT_RBNA     |
                     AT91RM9200_REG_EMAC_INT_TOVR     |
                     AT91RM9200_REG_EMAC_INT_TUND     |
                     AT91RM9200_REG_EMAC_INT_RTRY     |
                     AT91RM9200_REG_EMAC_INT_TBRE     |
                     AT91RM9200_REG_EMAC_INT_TCOM     |
                     AT91RM9200_REG_EMAC_INT_TIDLE    |
                     AT91RM9200_REG_EMAC_INT_LINK     |
                     AT91RM9200_REG_EMAC_INT_ROVR     |
                     AT91RM9200_REG_EMAC_INT_ABT));

                                                                        /* --------------------- CLEAR ISR ------------------- */
    AT91RM9200_EMAC_RegRd(AT91RM9200_REG_EMAC_ISR_OFFSET);

                                                                        /* ------------------- INIT CTL REG ------------------ */
    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_CTL_OFFSET,               /* Enable Received & Transmitter, reset Statistics.    */
                    (AT91RM9200_REG_EMAC_CTL_LB_LOW   |
                     AT91RM9200_REG_EMAC_CTL_LBL_DIS  |
                     AT91RM9200_REG_EMAC_CTL_RE_DIS   |                 /* Enabled at the end of initialization.               */
                     AT91RM9200_REG_EMAC_CTL_TE_DIS   |                 /* Enabled at the end of initialization.               */
                     AT91RM9200_REG_EMAC_CTL_MPE_EN   |
                     AT91RM9200_REG_EMAC_CTL_CSR      |
                     AT91RM9200_REG_EMAC_CTL_WES_DIS  |
                     AT91RM9200_REG_EMAC_CTL_BP_DIS));

                                                                        /* ------------------- INIT CFG REG ------------------ */
    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_CFG_OFFSET,
                    (AT91RM9200_REG_EMAC_CFG_SPD_100  |
                     AT91RM9200_REG_EMAC_CFG_HD       |
                     AT91RM9200_REG_EMAC_CFG_CAF_DIS  |
                     AT91RM9200_REG_EMAC_CFG_NBC_DIS  |
                     AT91RM9200_REG_EMAC_CFG_MTI_DIS  |
                     AT91RM9200_REG_EMAC_CFG_UNI_DIS  |
                     AT91RM9200_REG_EMAC_CFG_NORMAL   |
                     AT91RM9200_REG_EMAC_CFG_CLK_32   |
                     AT91RM9200_REG_EMAC_CFG_RTY_DIS  |
                     AT91RM9200_REG_EMAC_CFG_RMII_DIS));

                                                                        /* --------------- CLEAR TX STATUS BITS -------------- */
    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_TSR_OFFSET,
                    (AT91RM9200_REG_EMAC_TSR_OVR_CLR  |
                     AT91RM9200_REG_EMAC_TSR_COL_CLR  |
                     AT91RM9200_REG_EMAC_TSR_RLE_CLR  |
                     AT91RM9200_REG_EMAC_TSR_COMP_CLR |
                     AT91RM9200_REG_EMAC_TSR_UND_CLR));

                                                                        /* --------------- CLEAR RX STATUS BITS -------------- */
    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_RSR_OFFSET,
                    (AT91RM9200_REG_EMAC_RSR_BNA_CLR  |
                     AT91RM9200_REG_EMAC_RSR_REC_CLR  |
                     AT91RM9200_REG_EMAC_RSR_OVR_CLR));

                                                                        /* ----------------- INIT MAC REG/ADDR --------------- */
                                                                        /* See Note #3.                                        */

                                                                        /* MAC address is fetched from EMAC EEPROM.            */
#if (AT91RM9200_EMAC_CFG_MAC_ADDR_SEL == AT91RM9200_EMAC_MAC_ADDR_SEL_EEPROM)
    reg_val = AT91RM9200_EMAC_RegRd(AT91RM9200_REG_EMAC_SA1L_OFFSET);

    NetIF_MAC_Addr[0] = ((reg_val >>  0) & 0xFF);
    NetIF_MAC_Addr[1] = ((reg_val >>  8) & 0xFF);
    NetIF_MAC_Addr[2] = ((reg_val >> 16) & 0xFF);
    NetIF_MAC_Addr[3] = ((reg_val >> 24) & 0xFF);

    reg_val = AT91RM9200_EMAC_RegRd(AT91RM9200_REG_EMAC_SA1H_OFFSET);

    NetIF_MAC_Addr[4] = ((reg_val >>  0) & 0xFF);
    NetIF_MAC_Addr[5] = ((reg_val >>  8) & 0xFF);
#else                                                                   /* MAC address is application-configured.              */
    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_SA1L_OFFSET,
        (NetIF_MAC_Addr[3] << 24) |
        (NetIF_MAC_Addr[2] << 16) |
        (NetIF_MAC_Addr[1] <<  8) |
         NetIF_MAC_Addr[0]);

    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_SA1H_OFFSET,
        (NetIF_MAC_Addr[5] <<  8) |
         NetIF_MAC_Addr[4]);
#endif

    NetIF_MAC_AddrValid = DEF_YES;

                                                                        /* ------------ INIT RX BUFFER DESCRIPTORS ----------- */
    AT91RM9200_EMAC_RxBufHdrs = (volatile AT91RM9200_EMAC_RX_BUF_HDR *)AT91RM9200_EMAC_RX_BUF_HDRS_ADDR;
    AT91RM9200_EMAC_RxBuf     = (volatile CPU_INT08U *)AT91RM9200_EMAC_RX_BUF_ADDR;
    AT91RM9200_EMAC_TxBuf     = (volatile CPU_INT08U *)AT91RM9200_EMAC_TX_BUF_ADDR;
    AT91RM9200_EMAC_RxBufNbr  = 0;
    AT91RM9200_EMAC_TxBufNbr  = 0;

    for (i = 0; i < AT91RM9200_EMAC_RX_NUM_BUFS; i++) {

        AT91RM9200_EMAC_RxBufHdrs[i].RxBufAddr   =
            ((CPU_INT32U)(AT91RM9200_EMAC_RxBuf + i * AT91RM9200_EMAC_RX_BUF_SIZE) & AT91RM9200_EMAC_RXBUF_ADD_BASE_MASK);
        AT91RM9200_EMAC_RxBufHdrs[i].RxBufStatus = 0x00000000;

        if (i == (AT91RM9200_EMAC_RX_NUM_BUFS - 1)) {
                                                                        /* Last buffer MUST have its WRAP bit SET.             */
                                                                        /* See 'ATMEL AT91RM9200; Revision 1768B-ATARM–08/03'  */
                                                                        /* p. 573.                                             */
            AT91RM9200_EMAC_RxBufHdrs[i].RxBufAddr |= AT91RM9200_EMAC_RXBUF_ADD_WRAP;
        }
    }

                                                                        /* ------- CONFIGURE BUFFER DESCRIPTORS IN NIC ------- */
    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_RBQP_OFFSET, (CPU_INT32U)AT91RM9200_EMAC_RxBufHdrs);

                                                                        /* ------------------- CONFIGURE PHY ----------------- */
    NetNIC_PhyInit(&err);

                                                                        /* --------------------- EN RX/TX -------------------- */
    AT91RM9200_EMAC_RxEn();
    AT91RM9200_EMAC_TxEn();

                                                                        /* ----------------- INIT INT CTRL'R ----------------- */
#if (NET_NIC_CFG_INT_CTRL_EN == DEF_ENABLED)
    NetNIC_IntInit();                                                   /* See Note #5.                                        */
#endif
}


/*
*********************************************************************************************************
*                                        AT91RM9200_EMAC_RxEn()
*
* Description : Enable AT91RM9200 EMAC Receiver.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : AT91RM9200_EMAC_Init().
*********************************************************************************************************
*/

static  void  AT91RM9200_EMAC_RxEn (void)
{
#if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
    CPU_SR      cpu_sr;
#endif
    CPU_INT32U  reg_val;


    CPU_CRITICAL_ENTER();                                               /* See 'AT91RM9200 REGISTERS  Note #5b'.               */

    reg_val  = AT91RM9200_EMAC_RegRd(AT91RM9200_REG_EMAC_CTL_OFFSET);
    reg_val |= AT91RM9200_REG_EMAC_CTL_RE_EN;                           /* Enable Receiver.                                    */

    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_CTL_OFFSET, reg_val);

    CPU_CRITICAL_EXIT();
}


/*
*********************************************************************************************************
*                                       AT91RM9200_EMAC_RxIntEn()
*
* Description : Enable AT91RM9200 EMAC Receiver Interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : NetNIC_IntEn().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AT91RM9200_EMAC_RxIntEn (void)
{
#if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
    CPU_SR      cpu_sr;
#endif
    CPU_INT32U  reg_val;


    CPU_CRITICAL_ENTER();                                               /* See 'AT91RM9200 REGISTERS  Note #5b'.               */

    reg_val  = AT91RM9200_EMAC_RegRd(AT91RM9200_REG_EMAC_IER_OFFSET);
    reg_val |= (AT91RM9200_REG_EMAC_INT_RCOM |                          /* Enable 'Reception complete' interrupt.              */
                AT91RM9200_REG_EMAC_INT_RBNA |                          /* Enable 'Reception buffer not available' interrupt.  */
                AT91RM9200_REG_EMAC_INT_ROVR);                          /* Enable 'Receiver overrun' interrupt.                */

    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_IER_OFFSET, reg_val);

    CPU_CRITICAL_EXIT();
}


/*
*********************************************************************************************************
*                                    AT91RM9200_EMAC_RxPktGetSize()
*
* Description : (1) Get network packet size from NIC :
*                   (a) Read packet frame size.
*                   (b) Read packet frame's control octet to check for odd packet frame size.
*
* Argument(s) : none.
*
* Return(s)   : Size, in octets, of NIC's next network packet.
*
* Caller(s)   : NetNIC_RxPktGetSize().
*
* Note(s)     : (1) See 'AT91RM9200 EMAC RECEIVE BUFFER DESCRIPTOR' for packet frame format.
*********************************************************************************************************
*/

static  CPU_INT16U  AT91RM9200_EMAC_RxPktGetSize (void)
{
    CPU_INT16U  size;
    CPU_INT08U  i;


    size = 0;
    for (i = 0; i < AT91RM9200_EMAC_RX_NUM_BUFS; i++) {
        if (AT91RM9200_EMAC_RxBufHdrs[AT91RM9200_EMAC_RxBufNbrInc(i)].RxBufAddr & AT91RM9200_EMAC_RXBUF_ADD_SW_OWNED) {
            AT91RM9200_EMAC_RxBufNbr = AT91RM9200_EMAC_RxBufNbrInc(i);

                                                                        /* NIC returns size of frame including FCS.            */
                                                                        /* (Frame Check Sequence) which has 4 bytes.           */
                                                                        /* So we must substract 4 bytes from the value.        */
            size = (AT91RM9200_EMAC_RxBufHdrs[AT91RM9200_EMAC_RxBufNbr].RxBufStatus & AT91RM9200_EMAC_RXBUF_STAT_LEN_MASK) - 4;
            break;
        }
    }
    return (size);
}


/*
*********************************************************************************************************
*                                        AT91RM9200_EMAC_RxPkt()
*
* Description : (1) Read network packet from NIC into buffer :
*                   (a) Clear all receiver errors.
*                   (b) Find the first buffer marked 'software-owned'.
*                       'software owned' means that the NIC has copied a packet frame into it.
*                   (c) Read received packet frame from AT91RM9200.
*                   (d) Release received packet.
*
* Argument(s) : ppkt        Pointer to memory buffer to receive NIC packet.
*               ----        Argument checked in NetNIC_RxPkt().
*
*               size        Number of packet frame octets to read into buffer.
*               ----        Argument checked in NetNIC_RxPkt().
*
*               perr        Pointer to variable that will hold the return error code from this function :
*
*                               NET_NIC_ERR_NONE                Packet successfully transmitted.
*                               AT91RM9200_EMAC_ERR_RX_BUSY     Receiver not ready.
*
* Return(s)   : none.
*
* Caller(s)   : NetNIC_RxPkt().
*
* Note(s)     : (1) See 'AT91RM9200 EMAC RECEIVE BUFFER DESCRIPTOR' for packet frame format.
*********************************************************************************************************
*/

static  void  AT91RM9200_EMAC_RxPkt (void        *ppkt,
                                     CPU_INT16U   size,
                                     NET_ERR     *perr)
{
    CPU_INT32U  i;


    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_RSR_OFFSET,               /* Clear all receiver errors.                          */
                         (AT91RM9200_REG_EMAC_RSR_BNA_CLR |
                          AT91RM9200_REG_EMAC_RSR_REC_CLR |
                          AT91RM9200_REG_EMAC_RSR_OVR_CLR));

    for (i = 0; i < AT91RM9200_EMAC_RX_NUM_BUFS; i++) {
        if (AT91RM9200_EMAC_RxBufHdrs[AT91RM9200_EMAC_RxBufNbrInc(i)].RxBufAddr & AT91RM9200_EMAC_RXBUF_ADD_SW_OWNED) {
            AT91RM9200_EMAC_RxBufNbr = AT91RM9200_EMAC_RxBufNbrInc(i);

                                                                        /* NIC returns size of frame including FCS.            */
                                                                        /* (Frame Check Sequence) which has 4 bytes.           */
                                                                        /* So we must substract 4 bytes from the value.        */
            size =                 (AT91RM9200_EMAC_RxBufHdrs[AT91RM9200_EMAC_RxBufNbr].RxBufStatus & AT91RM9200_EMAC_RXBUF_STAT_LEN_MASK) - 4;
            Mem_Copy(ppkt, (void *)(AT91RM9200_EMAC_RxBufHdrs[AT91RM9200_EMAC_RxBufNbr].RxBufAddr   & AT91RM9200_EMAC_RXBUF_ADD_BASE_MASK), size);

            AT91RM9200_EMAC_RxPktDiscard(size);
            break;
        }
    }

    *perr = NET_NIC_ERR_NONE;
    if (i == AT91RM9200_EMAC_RX_NUM_BUFS) {
                                                                        /* No buffer marked 'software-owned'.                  */
        *perr = AT91RM9200_EMAC_ERR_RX_BUSY;
    } else {
                                                                        /* Network bursts can lead to missed NIC interrupts.   */
                                                                        /* If this happens, packets may be stuck into the NIC  */
                                                                        /* buffers because IF layer will not be informed of    */
                                                                        /* their presence. If next buffer is marked "software  */
                                                                        /* owned", it can be the case. So we notice IF layer.  */
                                                                        /* If there is more IF layer notice that the number of */
                                                                        /* packet received at one time, IF layer will receive  */
                                                                        /* non-fatal "AT91RM9200_EMAC_ERR_RX_BUSY" error msg.  */
        if (AT91RM9200_EMAC_RxBufHdrs[AT91RM9200_EMAC_RxBufNbr].RxBufAddr & AT91RM9200_EMAC_RXBUF_ADD_SW_OWNED) {
            NetOS_IF_RxTaskSignal(perr);                                /* Signal Net IF Rx Task of NIC rx pkt.                */
            *perr = NET_NIC_ERR_NONE;
        }
    }
}


/*
*********************************************************************************************************
*                                    AT91RM9200_EMAC_RxPktDiscard()
*
* Description : Discard network packet from NIC to free NIC packet frames for new receive packets.
*
* Argument(s) : size        Number of packet frame octets.
*
* Return(s)   : none.
*
* Caller(s)   : NetNIC_RxPktDiscard().
*********************************************************************************************************
*/

static  void  AT91RM9200_EMAC_RxPktDiscard (CPU_INT16U  size)
{
    if (size == 0) {
        return;
    }

                                                                        /* Release buffer back to the NIC.                     */
    AT91RM9200_EMAC_RxBufHdrs[AT91RM9200_EMAC_RxBufNbr].RxBufAddr &= ~AT91RM9200_EMAC_RXBUF_ADD_SW_OWNED;

                                                                        /* Set current RX buffer pointer to next buffer.       */
    AT91RM9200_EMAC_RxBufNbr = AT91RM9200_EMAC_RxBufNbrInc(1);
}


/*
*********************************************************************************************************
*                                        AT91RM9200_EMAC_TxEn()
*
* Description : Enable AT91RM9200 EMAC Transmitter.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : AT91RM9200_EMAC_Init().
*********************************************************************************************************
*/

static  void  AT91RM9200_EMAC_TxEn (void)
{
#if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
    CPU_SR      cpu_sr;
#endif
    CPU_INT32U  reg_val;


    CPU_CRITICAL_ENTER();                                               /* See 'AT91RM9200 REGISTERS  Note #5b'.               */

    reg_val  = AT91RM9200_EMAC_RegRd(AT91RM9200_REG_EMAC_CTL_OFFSET);
    reg_val |= AT91RM9200_REG_EMAC_CTL_TE_EN;                           /* Enable transmitter.                                 */

    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_CTL_OFFSET, reg_val);

    CPU_CRITICAL_EXIT();
}


/*
*********************************************************************************************************
*                                       AT91RM9200_EMAC_TxIntEn()
*
* Description : Enable AT91RM9200 EMAC Transmit Interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : NetNIC_IntEn().
*********************************************************************************************************
*/

static  void  AT91RM9200_EMAC_TxIntEn (void)
{
#if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
    CPU_SR      cpu_sr;
#endif
    CPU_INT32U  reg_val;


    CPU_CRITICAL_ENTER();                                               /* See 'AT91RM9200 REGISTERS  Note #5b'.               */

    reg_val  = AT91RM9200_EMAC_RegRd(AT91RM9200_REG_EMAC_IER_OFFSET);
    reg_val |= AT91RM9200_REG_EMAC_INT_TCOM;                            /* Enable 'Transmission complete' interrupt.           */

    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_IER_OFFSET, reg_val);

    CPU_CRITICAL_EXIT();
}


/*
*********************************************************************************************************
*                                    AT91RM9200_EMAC_TxPktPrepare()
*
* Description : (1) Copy packet into local NIC TX buffer prior to the end of the transmission of the last
                    packet.
*
* Argument(s) : ppkt        Pointer to memory buffer to transmit NIC packet.
*               ----        Argument checked in NetNIC_TxPkt().
*
*               size        Number of packet frame octets to write to frame.
*
*               perr        Pointer to variable that will hold the return error code from this function :
*
*                               NET_NIC_ERR_NONE                Packet successfully transmitted.
* Return(s)   : none.
*
* Caller(s)   : NetNIC_TxPktPrepare().
*********************************************************************************************************
*/

static void  AT91RM9200_EMAC_TxPktPrepare (void        *ppkt,
                                           CPU_INT16U   size,
                                           NET_ERR     *perr)
{
              CPU_INT32U   tx_ix;
    volatile  CPU_INT08U  *pbuf;


    tx_ix  = AT91RM9200_EMAC_TxBufNbr * AT91RM9200_EMAC_TX_BUF_SIZE;
    tx_ix += NET_IF_RX_IX;

    pbuf   = AT91RM9200_EMAC_TxBuf;
    pbuf  += tx_ix;

    Mem_Copy((void *)pbuf, ppkt, size);

   *perr = NET_NIC_ERR_NONE;
}


/*
*********************************************************************************************************
*                                        AT91RM9200_EMAC_TxPkt()
*
* Description : (1) Instruct AT91RM9200 EMAC to send network packet :
*                   (a) Check is transmitter ready.
*                   (b) Clear all transmitter errors.
*                   (c) Inform transmitter about buffer address and size.
*                       This starts actual transmission of the packet.
*
* Argument(s) : ppkt        Pointer to memory buffer to transmit NIC packet.
*               ----        Argument checked in NetNIC_TxPkt().
*
*               size        Number of packet frame octets to write to frame.
*
*               perr        Pointer to variable that will hold the return error code from this function :
*
*                               NET_NIC_ERR_NONE                Packet successfully transmitted.
*                               AT91RM9200_EMAC_ERR_TX_BUSY     Transmitter not ready.
* Return(s)   : none.
*
* Caller(s)   : NetNIC_TxPkt().
*********************************************************************************************************
*/

static  void  AT91RM9200_EMAC_TxPkt (void        *ppkt,
                                     CPU_INT16U   size,
                                     NET_ERR     *perr)
{
              CPU_INT32U   reg_val;
              CPU_INT32U   tx_addr;
    volatile  CPU_INT08U  *paddr;



                                                                        /* Check is transmitter ready.                         */
    reg_val = AT91RM9200_EMAC_RegRd(AT91RM9200_REG_EMAC_TSR_OFFSET);
    if ((reg_val & AT91RM9200_REG_EMAC_TSR_BNQ) == 0) {
        *perr = AT91RM9200_EMAC_ERR_TX_BUSY;
        return;
    }

                                                                        /* Clear all receiver errors.                          */
    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_RSR_OFFSET,
                         (AT91RM9200_REG_EMAC_RSR_BNA_CLR  |
                          AT91RM9200_REG_EMAC_RSR_REC_CLR  |
                          AT91RM9200_REG_EMAC_RSR_OVR_CLR));

                                                                        /* Clear all transmitter errors.                       */
    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_TSR_OFFSET,
                         (AT91RM9200_REG_EMAC_TSR_OVR_CLR  |
                          AT91RM9200_REG_EMAC_TSR_COL_CLR  |
                          AT91RM9200_REG_EMAC_TSR_RLE_CLR  |
                          AT91RM9200_REG_EMAC_TSR_COMP_CLR |
                          AT91RM9200_REG_EMAC_TSR_UND_CLR));

                                                                        /* Inform transmitter about frame address and size.    */
                                                                        /* This starts actual transmission of the packet.      */
    tx_addr  = AT91RM9200_EMAC_TxBufNbr * AT91RM9200_EMAC_TX_BUF_SIZE;
    tx_addr += NET_IF_RX_IX;

    paddr    = AT91RM9200_EMAC_TxBuf;
    paddr   += tx_addr;

    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_TAR_OFFSET, (CPU_INT32U)paddr);
    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_TCR_OFFSET, size);
    AT91RM9200_EMAC_TxBufNbr = ((AT91RM9200_EMAC_TxBufNbr + 1) % AT91RM9200_EMAC_TX_NUM_BUFS);
    *perr = NET_NIC_ERR_NONE;
}


/*
*********************************************************************************************************
*                                      AT91RM9200_EMAC_PhyRegRd()
*
* Description : Read PHY data value via AT91RM9200 EMAC PHY maintenance register.
*
* Argument(s) : phy             PHY address, normally 0.
*               reg             PHY register.
*
* Return(s)   : Data read from PHY register.
*
* Caller(s)   : NetNIC_PhyRegRd.
*
* Note(s)     : Register ALWAYS reads 16-bit data values.
*********************************************************************************************************
*/

static  CPU_INT16U  AT91RM9200_EMAC_PhyRegRd (CPU_INT08U  phy,
                                              CPU_INT08U  reg)
{
    CPU_INT32U  reg_val;


    AT91RM9200_EMAC_PhyRegDly();
    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_MAN_OFFSET,
                         (AT91RM9200_REG_EMAC_MAN_LOW       |
                          AT91RM9200_REG_EMAC_MAN_HIGH      |
                          AT91RM9200_REG_EMAC_MAN_READ      |
                          AT91RM9200_REG_EMAC_MAN_PHYA(phy) |
                          AT91RM9200_REG_EMAC_MAN_REGA(reg) |
                          AT91RM9200_REG_EMAC_MAN_CODE      |
                          AT91RM9200_REG_EMAC_MAN_DATA(0)));

    AT91RM9200_EMAC_PhyRegDly();
    reg_val = AT91RM9200_EMAC_RegRd(AT91RM9200_REG_EMAC_MAN_OFFSET);
    reg_val = AT91RM9200_REG_EMAC_MAN_DATA(reg_val);
    return (reg_val);
}


/*
*********************************************************************************************************
*                                      AT91RM9200_EMAC_PhyRegWr()
*
* Description : Write PHY data value via AT91RM9200 EMAC PHY maintenance register.
*
* Argument(s) : phy             PHY address, normally 0.
*               reg             PHY register.
*               val             Data to write to PHY register.
*
* Return(s)   : none.
*
* Caller(s)   : NetNIC_PhyRegWr.
*
* Note(s)     : Register ALWAYS writes 16-bit data values.
*********************************************************************************************************
*/

static  void  AT91RM9200_EMAC_PhyRegWr (CPU_INT08U  phy,
                                        CPU_INT08U  reg,
                                        CPU_INT16U  val)
{
    AT91RM9200_EMAC_PhyRegDly();
    AT91RM9200_EMAC_RegWr(AT91RM9200_REG_EMAC_MAN_OFFSET,
                         (AT91RM9200_REG_EMAC_MAN_LOW       |
                          AT91RM9200_REG_EMAC_MAN_HIGH      |
                          AT91RM9200_REG_EMAC_MAN_WRITE     |
                          AT91RM9200_REG_EMAC_MAN_PHYA(phy) |
                          AT91RM9200_REG_EMAC_MAN_REGA(reg) |
                          AT91RM9200_REG_EMAC_MAN_CODE      |
                          AT91RM9200_REG_EMAC_MAN_DATA(val)));
}


/*
*********************************************************************************************************
*                                     AT91RM9200_EMAC_RxBufNbrInc()
*
* Description : Return incremented value of AT91RM9200_EMAC_RxBufNbr (AT91RM9200_EMAC_RxBuf is a ring-buffer).
*
* Argument(s) : val             Value to add to AT91RM9200_EMAC_RxBufNbr.
*
* Return(s)   : new value of AT91RM9200_EMAC_RxBufNbr.
*
* Caller(s)   : Various.
*********************************************************************************************************
*/

static  CPU_INT16U  AT91RM9200_EMAC_RxBufNbrInc (CPU_INT16U  val)
{
    return ((AT91RM9200_EMAC_RxBufNbr + val) % AT91RM9200_EMAC_RX_NUM_BUFS);
}
