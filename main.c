#include <stdio.h>
#include "common.h"

#include"HCFileOperation.h"
#include"hcopencv.h"
#include "zmq/czmq.h"
#include"HCJson.h"
typedef  struct
{
    char idbuf[512];
}ST_IDNumber;
extern ST_IDNumber IDNumber[3];
static int init(void);
static int HC_PthreadCreate(pthread_t ThreadID,void *Process_Thread);
static void *pictruedrawprocess(void);
static void *zmqget(void);
static int idflag=ERR_COMMON_FAILED;
static int HCEventVideoCJsonAnalysis(char *cjsonbuf);
int init(void)
{
    char cjsonbuf[512]={0};
    memset(cjsonbuf,0x0,sizeof (cjsonbuf));
    for (int i=0;i<3;i++) {
         memset(&IDNumber[i],0x0,sizeof (ST_IDNumber));
    }


    pthread_t t0=0,t1=0;
    HC_PthreadCreate(t0,zmqget);
    HC_PthreadCreate(t1,pictruedrawprocess);

    return ERR_COMMON_SUCCESS;
}
void *zmqget(void)
{
     zsock_t *pull = zsock_new_pull ("inproc://drawpicture1");
    while (1) {
          char * string = zstr_recv (pull);
          if(HCEventVideoCJsonAnalysis(string)!=ERR_COMMON_FAILED)
           {
              idflag=ERR_COMMON_SUCCESS;
           }
          else {
              idflag=ERR_COMMON_FAILED;
          }
           zstr_free (&string);
        usleep(200000);
    }
}
int HCEventVideoCJsonAnalysis(char *cjsonbuf)
{



    //第一步打包JSON字符串
    cJSON* cjson = cJSON_Parse(cjsonbuf);
    //判断是否打包成功
    if(cjson == NULL){
        HC_PRINT("cjson error…\n");
        return ERR_COMMON_FAILED;
    }
    char *json_data = NULL;
    json_data = cJSON_Print(cJSON_GetObjectItem(cjson,"one"));
    if(json_data!=NULL)
    {
        sprintf(IDNumber[0].idbuf,"%s",json_data);

    }
    free(json_data);
    json_data = cJSON_Print(cJSON_GetObjectItem(cjson,"two"));
    if(json_data!=NULL)
    {
        sprintf(IDNumber[1].idbuf,"%s",json_data);

    }
    free(json_data);
    json_data = cJSON_Print(cJSON_GetObjectItem(cjson,"three"));
    if(json_data!=NULL)
    {
        sprintf(IDNumber[2].idbuf,"%s",json_data);

    }
    free(json_data);
    //这里只需要释放cjson即可，因为其它的都指向它
    cJSON_Delete(cjson);

    return ERR_COMMON_SUCCESS;
}
int HC_PthreadCreate(pthread_t ThreadID,void *Process_Thread)
{
    int res = ERR_COMMON_SUCCESS;
    //创建线程
    if(Process_Thread == NULL)
    { }
    else
    {
        res = pthread_create(&ThreadID, NULL, Process_Thread, NULL);
        if (0 != res)
        {
            HC_PRINT("创建线程失败");
            return ERR_COMMON_FAILED;
        }
        pthread_detach(ThreadID);
    }
    return res;
}
void *pictruedrawprocess(void)
{
    char Send_task_info[512]={0};
    char filename[512]={0};
     zsock_t* push = zsock_new_push("inproc://example");
    sprintf(Send_task_info,\
            "{\"filename\":\"%s\"}",\
            filename);

    HC_PRINT("json: %s\n", Send_task_info);

    zstr_send(push, Send_task_info);
    memset(Send_task_info,0x0,sizeof (Send_task_info));
    while (1) {

        usleep(300000);
    }
}
//FTP数据上传
 char * HCPictureDraw( const char *ID,const char *pDataRootPath)
{
    int res = ERR_COMMON_SUCCESS;
    char szFileName[128] = {0};
    char *ret="";
    char pathload[100]={0};
    int PathLength=0;
    int  ReadFd=0;
    int  WriteFd=0;
    strcat(pathload,pDataRootPath);
    strcat(pathload,"/");
    strcat(pathload,ID);
    strcat(pathload,"/");
    HC_ASSERT(pathload);
    ReadFd = open(pathload, O_RDWR);
    if (ReadFd < 0)
    {
        if(HC_fileOpen(&WriteFd, pathload, O_RDWR | O_CREAT)==ERR_COMMON_FAILED){
            close(ReadFd);
            close(WriteFd);
            return ret;
        }
    }
    PathLength=(int)strlen(pathload);
    res = HC_findFile(pathload, szFileName, ".jpg");
    if (ERR_COMMON_SUCCESS == res)
    {
        if(HCOpencvDraw(szFileName,szFileName,"open",100,100,100,100)==ERR_COMMON_SUCCESS)
        {

                HC_RemoveFile(szFileName);

        }
        else
        {
            close(ReadFd);
            close(WriteFd);
            return ret;
        }

    }
    else
    {
        close(ReadFd);
        close(WriteFd);
        return ret;
    }
    close(ReadFd);
    close(WriteFd);
    return ERR_COMMON_SUCCESS;
}
int main()
{
    init();
    while (1)
    {
        sleep(1);
    }
}
