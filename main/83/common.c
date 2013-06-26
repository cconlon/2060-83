#include "ucos_ii.h"

OS_EVENT				*g_EventStatic;
OS_EVENT				*g_EventDynamic;
//OS_EVENT				*g_EventTran;
OS_EVENT				*g_EventRTData;

#if 0
void _error(char *msg)
{

  FILE *fp;
  char time_buffer[16];
  OS_ERR err;
  OSSchedLock(&err);
  sprintf(time_buffer,"%6d ",OSTimeGet(&err));
  fp = fopen("error.txt", "a");
  fwrite(time_buffer, sizeof(char), strlen(time_buffer), fp);
  fwrite(msg, sizeof(char), strlen(msg), fp);
  fclose(fp);
  OSSchedUnlock(&err);

}

void _log(char *msg)
{

  FILE *fp;
  char time_buffer[16];
  OS_ERR err;
  OSSchedLock(&err);
  sprintf(time_buffer,"%6d ",OSTimeGet(&err));
  fp = fopen("log.txt", "a");
  fwrite(time_buffer, sizeof(char), strlen(time_buffer), fp);
  fwrite(msg, sizeof(char), strlen(msg), fp);
  fclose(fp);
  OSSchedUnlock(&err);
}
#endif
