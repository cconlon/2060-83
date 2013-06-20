#ifndef _NETSERVICES_H_
#define _NETSERVICES_H_

#include <board.h>



void NET_Handler( sEmacd *pEmacd );
uint8_t NET_Send( sEmacd *pEmacd,
                    void *pBuffer,
                    uint32_t size,
                    fEmacdTransferCallback fTxCb );
void  Test_Tcp (void  *p_arg);
void AppInit_TCPIP (void);
#endif //  _NETSERVICES_H_

