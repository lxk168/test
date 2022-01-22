#ifndef HCFILEOPERATION_H
#define HCFILEOPERATION_H
#include "common.h"


extern int HC_RemoveFile(char *file_path);
extern int HC_fileOpen( int *fp,  const char *pcPath,  int oflag);
extern int HC_findFile( const char *pcDir,  char *pcFileName,  char *pcType);
extern int HC_fileWrite( const char *pcFileName,  const char *pcBuf,  unsigned int uNbyte, int oflag);
extern int HC_fileRead( const char *pcFileName,  const char *pcBuf,  unsigned int uNbyte);
#endif
