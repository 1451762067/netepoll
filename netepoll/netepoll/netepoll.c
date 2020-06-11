
#define  _MAIN_

#include "netepoll.h"

int main(int argc, char **argv)
{
    int    i;
    int    n_ready;
    struct epoll_event events[MAX_EVENTS];

    /* 屏蔽信号量 */
    for (i = 0; i < 32; i++)
    {
        signal(i,SIG_IGN);
    }

    /* 读取配置文件 */
    if ( getInit(argc, argv) )
    {
        fprintf(stderr, "getInit error!\n");
        exit(-1);
    }

    if ((TaskPool = threadpool_create(THREAD, QUEUE, 0)) == NULL)
    {
        fprintf(stderr, "threadpool_create error!\n");
        exit(-1);
    }

    //初始化event数据
    memset(Ep_events, 0, sizeof(Ep_events));
    Ep_fd = epoll_create(MAX_EVENTS);                         //创建红黑树,返回给全局变量Ep_fd;
    if (Ep_fd <= 0)
    { 
        D_LOG("create Ep_fd error:[%d-%s]", errno, strerror(errno));
        threadpool_destroy(TaskPool, 0);
        return -1;
    }
	
    /*初始化监听socket*/
    initlistensocket(Ep_fd, HostPort);

    while (1)
    {
        /*监听红黑树,将满足条件的文件描述符加至Ep_events数组*/ 
        n_ready = epoll_wait(Ep_fd, events, MAX_EVENTS, -1); //1秒没事件满足则返回0
        if (n_ready < 0)
        {
            D_LOG("epoll_wait error, exit[%d-%s]", errno, strerror(errno));
            break;
        }

        for (i=0; i<n_ready; i++)
        {
            //根据需要添加多个回调函数
            struct my_events *ev = (struct my_events *)events[i].data.ptr;
            if ((events[i].events & EPOLLIN) && (ev->m_event & EPOLLIN))  //读就绪事件
               ev->call_back(ev->m_fd, events[i].events, ev->m_arg);
            if ((events[i].events & EPOLLOUT) && (ev->m_event & EPOLLOUT))  //读就绪事件
               ev->call_back(ev->m_fd, events[i].events, ev->m_arg);
        }
    }
    threadpool_destroy(TaskPool, 0);

    return 0;
}  
