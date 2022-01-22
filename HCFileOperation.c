
#include "HCFileOperation.h"

//创建文件
//形如"X:/123/456.txt" "X:/123/456"表示文件
/*
  函数功能: 在指定目录文件写数据
  参数说明:
            int *fp      文件操作描述符
            char *pcPath 用于保存文件完整路径
            int oflag    文件读写标志位

*/
int HC_fileOpen( int *fp,  const char *pcPath,  int oflag)
{
    unsigned int i = 0;
    unsigned int len=0;
    len = (unsigned int)strlen(pcPath);

    char dirPath[len+1];
    dirPath[len] = '\0';
    strncpy(dirPath, pcPath, len);

    do
    {
        if (dirPath[i] == '/' && i > 0)
        {
            dirPath[i]='\0';
            if(access(dirPath, F_OK) < 0 )
            {
                if (mkdir(dirPath, 0666) < 0 )
                {
                    HC_ERROR("mkdir=%s:msg=%s\n", dirPath, strerror(errno));
                    return ERR_COMMON_FAILED;
                }
            }
            dirPath[i]='/';
        }
        else if(('\0' == dirPath[i]) && ( i > 0))      /*&&('/' != dirPath[i]))*/
        {
            *fp = open(dirPath, oflag, 0666);
            if (NULL == fp)
            {
                HC_ERROR("fopen =%s:msg=%s\n",dirPath, strerror(errno));
                return ERR_COMMON_FAILED;
            }
            return ERR_COMMON_SUCCESS;
        }
    }while(i++ < len);

    return ERR_COMMON_SUCCESS;
}

////rm file ,
int HC_RemoveFile(char *file_path)
{
    char FilePath[256] = {0};
    memcpy(FilePath,file_path,strlen(file_path));
  if(NULL == FilePath){
      HC_PRINT("FilePath NULL");
      return -1;
    }
  if(-1 ==remove(FilePath)){
      HC_ERROR("remove ->%s",FilePath);
      return -1;
    }

  return 0;
}

/*
  函数功能: 在给定目录下查找指定类型的文件是否存在,若存在，则返回文件完整路径
  参数说明:
            char *pcDir 目录名
            char *pcFileName 用于保存文件完整路径
            char *pcType 文件类型(后缀)
*/
int HC_findFile( const char *pcDir,  char *pcFileName,  char *pcType)
{
    char szType[10] = {0};
    DIR *pstDir = NULL;
    char cDir_temp[128] = {0};
    struct dirent *pstDirContent = NULL;

    if(access(pcDir, F_OK) < 0 )
    {
        HC_ERROR("pcDir");
        return ERR_COMMON_FAILED;
    }
    //检查文件类型(后缀名)长度
    if (strlen(pcType) > 6)
    {
        HC_PRINT("文件类型错误,文件类型最多为6个字节!\n");
        return ERR_COMMON_FAILED;
    }

    memcpy(cDir_temp, pcDir, strlen(pcDir));

    //打开目录
    pstDir = opendir(pcDir);
    if (NULL == pstDir)
    {
        HC_ERROR("opendir");
        return ERR_COMMON_FAILED;
    }

    //循环读取目录内容
    while (NULL != (pstDirContent = readdir(pstDir)))
    {
        //文件的d_type值为8, 目录的d_type值为4
        if (4 == pstDirContent->d_type && pstDirContent->d_name[0] != 0x2e)
        {
            memcpy(&cDir_temp[strlen(cDir_temp)], pstDirContent->d_name, strlen(pstDirContent->d_name));
            cDir_temp[strlen(cDir_temp)] = '/';
            closedir(pstDir);
            pstDir = opendir(cDir_temp);
            continue;
        }

        if (8 == pstDirContent->d_type)
        {
            //获取文件类型
            memcpy(szType, &pstDirContent->d_name[strlen(pstDirContent->d_name) - strlen(pcType)], strlen(pcType));
            if (0 == strncmp(szType, pcType, strlen(pcType)))
            {
                memcpy(pcFileName, cDir_temp, strlen(cDir_temp));
                //若给定目录非'/'结尾, 则需手动添加'/'
                if (pcFileName[strlen(pcFileName) - 1] != '/')
                {
                    pcFileName[strlen(pcFileName)] = '/';
                }
                memcpy(&pcFileName[strlen(pcFileName)], pstDirContent->d_name, strlen(pstDirContent->d_name));
                break;
            }
        }
    }
    //关闭目录
    closedir(pstDir);
    //获取指定类型文件失败
    if (pcFileName[0] == 0x00)
    {
        //HC_ERROR("find the special file failed\n");
        fflush(stdout);
        return ERR_COMMON_FAILED;
    }

    return ERR_COMMON_SUCCESS;
}
/*
  函数功能: 在指定目录文件写数据
  参数说明:

            char *pcFileName 用于保存文件完整路径
            char *pcBuf  写入的内容
            int uNbyte   内容大小
            int oflag    文件读写标志位
            成功返回0，失败返回-1
*/
int HC_fileWrite( const char *pcFileName,  const char *pcBuf,  unsigned int uNbyte,IN int oflag)
{
    int writeFd = 0;

    HC_ASSERT(NULL != pcFileName);

    HC_fileOpen(&writeFd, pcFileName, oflag);

    //2. 写入数据
    if (write(writeFd,pcBuf, uNbyte) < 0)
    {
        HC_ERROR("ga_fileWrite");
        close(writeFd);
        fsync(writeFd);
        return ERR_COMMON_FAILED;
    }
    //3. 关闭文件
    fsync(writeFd);
    close(writeFd);

    return ERR_COMMON_SUCCESS;
}
/*
  函数功能: 在指定目录文件读数据
  参数说明:

            char *pcFileName 用于读取文件完整路径
            char *pcBuf  读入的内容
            int uNbyte   内容大小
            int oflag    文件读写标志位
            成功返回读取数据长度，失败返回-1
*/
int HC_fileRead( const char *pcFileName,  const char *pcBuf,  unsigned int uNbyte)
{
    int readFd = 0;
    int readLen = 0;
    int res = 0;

    HC_ASSERT(NULL != pcFileName);

    //1. 打开文件
    res = access(pcFileName, F_OK);
    if (ERR_COMMON_SUCCESS == res)
    {
        readFd = open(pcFileName, O_RDONLY);
        if (ERR_COMMON_FAILED == readFd)
        {
            HC_ERROR("打开文件(%s)失败...\n", pcFileName);
            return ERR_COMMON_FAILED;
        }
    }
    else
        return ERR_COMMON_FAILED;
    //2. 写入数据
    if ((readLen = read(readFd, (void *)pcBuf, uNbyte)) < 0)
    {
        HC_ERROR("ga_fileRead");
        close(readFd);
        return ERR_COMMON_FAILED;
    }
    //3. 关闭文件
    close(readFd);
    return readLen;
}
