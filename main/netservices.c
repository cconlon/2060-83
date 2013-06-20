#include <board.h>
#include <string.h>
#include "netservices.h"
#include <includes.h>
/*----------------------------------------------------------------------------
*         Definitions
*----------------------------------------------------------------------------*/
NET_SOCK_ADDR_IP  TestRemoteSockAddr;
NET_SOCK_ID       TestClientSockID;

#if uC_TCPIP_MODULE > 0
NET_IP_ADDR   ip;
NET_IP_ADDR   msk;
NET_IP_ADDR   gateway;
NET_ERR       err;
#endif

/** Error bits for TX */
#define EMAC_TX_ERR_BITS  \
(EMAC_TXD_bmERROR | EMAC_TXD_bmUNDERRUN | EMAC_TXD_bmEXHAUSTED)

#define  REMOTE_HOST_ADDR                 (((NET_IP_ADDR)10u << (3u * DEF_OCTET_NBR_BITS)) | \
((NET_IP_ADDR)217u << (2u * DEF_OCTET_NBR_BITS)) | \
  ((NET_IP_ADDR)  3u << (1u * DEF_OCTET_NBR_BITS)) | \
    ((NET_IP_ADDR)200u << (0u * DEF_OCTET_NBR_BITS)))

extern sEmacd gEmacd;
void  Test_Tcp (void  *p_arg)
{
  NET_ERR            err;
  uint32_t cnt = 0;
  uint8_t buff[1024];
  int32_t len = 0;
  Mem_Set(&TestRemoteSockAddr, (CPU_CHAR)0, NET_SOCK_ADDR_IP_SIZE);
  TestRemoteSockAddr.AddrFamily = NET_SOCK_ADDR_FAMILY_IP_V4;
  TestRemoteSockAddr.Port = NET_UTIL_HOST_TO_NET_16(50001);
  //NetASCII_Str_to_IP("192.168.1.200", &err);//
  TestRemoteSockAddr.Addr = NET_UTIL_HOST_TO_NET_32(REMOTE_HOST_ADDR);
  EMAC_EnableIt(gEmacd.pHw, EMAC_IER_RCOMP | EMAC_IER_ROVR | EMAC_IER_TCOMP);
  TestClientSockID = socket(AF_INET, SOCK_STREAM, NET_SOCK_PROTOCOL_TCP);  //NET_SOCK_ADDR_FAMILY_IP_V4
  connect(TestClientSockID, (struct sockaddr*)&TestRemoteSockAddr, sizeof(struct  sockaddr));
  //NetSock_CfgTimeoutRxQ_Set(TestClientSockID,100000, &err);
  if(TestClientSockID == 0)
    return;
  send(TestClientSockID,"abcdefghij1234567890ABCDEFGHIJabcdefghij1234567890ABCDEFGHIJabcdefghij1234567890ABCDEFGHIJabcdefghij1234567890ABCDEFGHIJabcdefghij1234567890ABCDEFGHIJabcdefghij1234567890ABCDEFGHIJabcdefghij1234567890ABCDEFGHIJabcdefghij1234567890ABCDEFGHIJ",240,NET_SOCK_FLAG_NONE);
  
  while(1) {
    if((len = recv(TestClientSockID, buff, 1024, 0)) > 0)
    {
      send(TestClientSockID,buff,len,NET_SOCK_FLAG_NONE);
    }
    OSTimeDly(100);
    
  }
  close(TestClientSockID);
}

#if uC_TCPIP_MODULE > 0
void AppInit_TCPIP (void)
{
  
  err     = Net_Init();                                               /* Initialize uC/TCP-IP                                     */
  
  ip      = NetASCII_Str_to_IP((CPU_CHAR*)"10.217.3.168",  &err);
  msk     = NetASCII_Str_to_IP((CPU_CHAR*)"255.255.255.0", &err);
  gateway = NetASCII_Str_to_IP((CPU_CHAR*)"10.217.3.1",   &err);
  
  err     = NetIP_CfgAddrThisHost(ip, msk);
  err     = NetIP_CfgAddrDfltGateway(gateway);
}
#endif
