#ifndef _NETSERVICES_H_
#define _NETSERVICES_H_

#include <board.h>

void AppInit_TCPIP (void);
void NET_Handler( sEmacd *pEmacd );
uint8_t NET_Send( sEmacd *pEmacd,
                    void *pBuffer,
                    uint32_t size,
                    fEmacdTransferCallback fTxCb );
void  Test_Tcp (void  *p_arg);
int channel_open(char *const addr, int port);
int channel_close(int fd);
int channel_write(int fd, void* data, size_t len);
#endif //  _NETSERVICES_H_

