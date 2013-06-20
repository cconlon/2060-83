
#ifndef __NET_BSP_H__
#define __NET_BSP_H__

#include  <includes.h>

/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef     NET_BSP_MODULE
#define    NET_BSP_EXT
#else
#define    NET_BSP_EXT  extern
#endif

/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  EMAC_CFG_MAC_ADDR_SEL     EMAC_MAC_ADDR_SEL_CFG            /* Configure MAC address set method :                       */


#define  NetBSP_IntEn()                  AT91C_BASE_AIC->AIC_IECR = (1 << AT91C_ID_PIOC)
#define  NetBSP_IntDis()                 AT91C_BASE_AIC->AIC_IDCR = (1 << AT91C_ID_PIOC)

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void         NetBSP_NIC_HW_Init(void);
void         NetBSP_DlyU(CPU_INT16U us);
void         NetBSP_DlyM(CPU_INT16U ms);

void         NetBSP_IntClr(void);

CPU_INT08U   NetBSP_OS_X_Init(void); 
int NET_EMAC_PHY_Init(NET_ERR     *perr);

void NET_EMAC_IntInit(pHandler pISR_Handler);
uint16_t NET_EMAC_RXD_GetSize(void);
void NET_EMAC_ISR(void);

uint8_t NET_EMAC_Poll(void *ppkt);

void  NET_EMAC_TxPktPrepare (void        *ppkt,
                             uint16_t   size,
                             NET_ERR     *perr);
void NET_EMAC_Send(uint16_t size, NET_ERR *perr);
void NET_EMAC_EMAC_RxPktDiscard(void);
void NET_EMAC_IntEn(void);

#endif
