#include <board.h>
#include <string.h>
#include "netservices.h"
#include <net.h>

/*----------------------------------------------------------------------------
*         Definitions
*----------------------------------------------------------------------------*/


int channel_open(char *const addr, int port)
{
  int ret;
  struct sockaddr_in  ServerAddr;
//  struct sockaddr_in  ClientAddr;
//  int       ServerID;
  int       ClientID;
  Mem_Set(&ServerAddr, (CPU_CHAR)0, sizeof(ServerAddr) );
  ServerAddr.sin_family = AF_INET;
  ServerAddr.sin_port = htons(port);
  ServerAddr.sin_addr.s_addr = inet_addr (addr);
  ClientID = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(-1 == ClientID)
    return -1;
  ret = connect(ClientID, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
  if(-1 == ret)
    return -1;
  else
    return ClientID;
}

int channel_write(int fd, void* data, size_t len) {
  return send(fd, data, len, NET_SOCK_FLAG_NONE);
}

void  Test_Tcp (void  *p_arg)
{
  struct sockaddr_in  ServerAddr;
  struct sockaddr_in  ClientAddr;
  int       ServerID;
  int       ClientID;
  uint8_t buff[1024];
  int32_t len = 0;
  socklen_t client_len = sizeof(ClientAddr);
  Mem_Set(&ServerAddr, (CPU_CHAR)0, sizeof(ServerAddr) );
  ServerAddr.sin_family = AF_INET;
  ServerAddr.sin_port = htons(50002);
  ServerAddr.sin_addr.s_addr = inet_addr ("10.217.3.168");
  
  ServerID = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
  
  //connect(TestClientSockID, (struct sockaddr*)&TestRemoteSockAddr, sizeof(struct  sockaddr));
  bind(ServerID, (struct sockaddr*)&ServerAddr, sizeof(struct  sockaddr));
  listen(ServerID, 10);
  ClientID = accept(ServerID, (struct sockaddr*)&ClientAddr, &client_len);
  
  assert(ClientID != -1);
  send(ClientID,"abcdefghijklmnopqrstuvwxyz", 26, NET_SOCK_FLAG_NONE);
  
  while(1) {
    
    len = recv(ClientID, buff, 1024, 0);
    
    if(len > 0)
    {
      //OSTimeDlyHMSM(0,0,1,0);
      send(ClientID,buff,len,NET_SOCK_FLAG_NONE);
    }
    if(len == 1)
    {
      break;
    }
    
    OSTimeDlyHMSM(0,0,1,0);
  }
  //close(TestClientSockID);
}

void AppInit_TCPIP (void)
{
  NET_IP_ADDR   ip;
  NET_IP_ADDR   msk;
  NET_IP_ADDR   gateway;
  NET_ERR       err;
  
  err     = Net_Init();                                               /* Initialize uC/TCP-IP                                     */
  
  ip      = NetASCII_Str_to_IP((CPU_CHAR*)"10.217.3.168",  &err);
  msk     = NetASCII_Str_to_IP((CPU_CHAR*)"255.255.255.0", &err);
  gateway = NetASCII_Str_to_IP((CPU_CHAR*)"10.217.3.1",   &err);
  
  err     = NetIP_CfgAddrThisHost(ip, msk);
  err     = NetIP_CfgAddrDfltGateway(gateway);
}

