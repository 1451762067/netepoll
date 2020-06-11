/* -------------------------------------------------------
 * FileName  : netepoll.h (1.40)
 * Function  :
 * CreateInfo: zhangzh @ 2020.06.09 10:31:06
 * ------------------------------------------------------- */
 
#ifndef _NETEPOLL_H_
#define _NETEPOLL_H_
 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <signal.h>
#include <libgen.h>
#include <time.h>
#include <malloc.h>
#include <math.h>
#include <errno.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <setjmp.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <arpa/inet.h>

#include "libbase.h"
#include "threadpool.h"

#define BUFF_SIZE   1024    /* Buffer Size of UDP/IP */ 
#define MAX_EVENTS  1024

#define HEADER_LEN  2

#define THREAD 32
#define QUEUE  256

typedef enum {
    DATA_COMPLETE,
    DATA_ERROR,
    DATA_CONTINUE
}DATA_RET;

struct my_events {
  
    int        m_fd;                                         //监听的文件描述符
    int        m_event;                                      //监听的事件
    int        m_status;                                     //是否在红黑树上, 1->在, 0->不在
    time_t     m_lasttime;                                   //最后放入红黑树的时间  
    void       *m_arg;                                       //泛型参数
    void       (*call_back)(int fd, int event, void *arg);   //回调函数
    char       m_rcv_buf[BUFSIZ];
    int        m_rcv_buf_len;
    char       m_snd_buf[BUFSIZ];
    int        m_snd_buf_len;
    int        m_snt_buf_len;
    struct     sockaddr_in m_addr;
};



#ifdef _MAIN_
    int                     Debug;
    char                    HostPort[40];
    int                     TimeOut;
    char                    ProgName[30];
    char                    IniFile[256];
    char                    CfgFile[256];
    threadpool_t* TaskPool;

    int                     Ep_fd;                                //红黑树根
    struct my_events        Ep_events[MAX_EVENTS];   
    
#else
    extern int              Debug;
    extern char             HostPort[];
    extern int              TimeOut;
    extern char             ProgName[];
    extern char             IniFile[];
    extern char             CfgFile[];
    extern threadpool_t* TaskPool;

    extern int              Ep_fd;                                //红黑树根
    extern struct my_events Ep_events[];  
   
#endif

// 函数声明
extern int getInit(int, char **);
/*初始化监听socket*/
void initlistensocket(int ep_fd, char * port);
/*将结构体成员变量初始化*/
void eventsetinit(struct my_events *my_ev, int fd, void (*call_back)(int fd, int event, void *arg), void *event_arg);
/*向红黑树添加 文件描述符和对应的结构体*/
void eventadd(int ep_fd, int event, struct my_events *my_ev);
/*从红黑树上删除 文件描述符和对应的结构体*/
void eventdel(int ep_fd, struct my_events *ev);
/*发送数据*/
void senddata(int client_fd, int event, void *arg);
/*接收数据*/
void recvdata(int client_fd, int event, void *arg);
/*回调函数: 接收连接*/
void acceptconnect(int listen_fd, int event, void *arg);
/* 任务函数 */
void mytask(void* arg);
/* 数据完成性判断 */
DATA_RET testpacket(struct my_events* ev);

#endif /*  _NETEPOLL_H_ */
