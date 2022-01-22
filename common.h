#ifndef COMMON_H
#define COMMON_H


#include "string.h"
#include "stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "errno.h"
#include <assert.h>
#include <dirent.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <dirent.h>
#include <time.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <semaphore.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <linux/ethtool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include "sys/stat.h"
#include "sys/types.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include <time.h>
#include <zmq/zmq.h>
#include <math.h>
#define ERR_COMMON_SUCCESS 0
#define ERR_COMMON_FAILED  -1

#define   IN
#define   OUT
#define   INOUT

#define  bool	_Bool
#define  false  0
#define  true   1

#define  __u8       unsigned char
#define  __u16     unsigned short
#define  __u32     unsigned int
#define LocalPath   "/userdata/usr/local/"   //本地目录
#define SdcardPath  "/mnt/sdcard/"  // sd卡目录
#define PicturePath   SdcardPath   "picture/"
#define VideoPath      SdcardPath   "video/"
#define HTTPUploadPath  SdcardPath "HTTP/"
#define FTPUploadPath   SdcardPath  "FTP/"
#define HC_ASSERT(bResult) assert(bResult)



#define HC_ERROR(format, args...)	do {fprintf(stderr, " [HC_ERROR]%s %s(Line %d): (errno=%d) ",  __FILE__, __func__, __LINE__, errno);perror("reason");} while(0)

#define HC_DEBUG(format, args...)	do {fprintf(stdout, " [HC_DEBUG]%s (Line %d):", __FILE__, __LINE__);fprintf(stdout, format, ##args);fprintf(stdout, "\n");} while(0)

#define HC_PRINT(format, args...) do {fprintf(stdout, " [HC_PRINT]%s (Line %d):", __FILE__, __LINE__);fprintf(stdout, format, ##args);fprintf(stdout, "\n");fflush(stdout);} while(0)
extern int PVFlag;
extern int SendFlag;
#endif
