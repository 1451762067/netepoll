
#define  _MAIN_

#include "netepoll.h"

int main(int argc, char **argv)
{
    int    i;
    int    n_ready;
    struct epoll_event events[MAX_EVENTS];

    /* �����ź��� */
    for (i = 0; i < 32; i++)
    {
        signal(i,SIG_IGN);
    }

    /* ��ȡ�����ļ� */
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

    //��ʼ��event����
    memset(Ep_events, 0, sizeof(Ep_events));
    Ep_fd = epoll_create(MAX_EVENTS);                         //���������,���ظ�ȫ�ֱ���Ep_fd;
    if (Ep_fd <= 0)
    { 
        D_LOG("create Ep_fd error:[%d-%s]", errno, strerror(errno));
        threadpool_destroy(TaskPool, 0);
        return -1;
    }
	
    /*��ʼ������socket*/
    initlistensocket(Ep_fd, HostPort);

    while (1)
    {
        /*���������,�������������ļ�����������Ep_events����*/ 
        n_ready = epoll_wait(Ep_fd, events, MAX_EVENTS, -1); //1��û�¼������򷵻�0
        if (n_ready < 0)
        {
            D_LOG("epoll_wait error, exit[%d-%s]", errno, strerror(errno));
            break;
        }

        for (i=0; i<n_ready; i++)
        {
            struct my_events *ev = (struct my_events *)events[i].data.ptr;
            if ((events[i].events & EPOLLIN) && (ev->m_event & EPOLLIN))  //�������¼�
               ev->call_back(ev->m_fd, events[i].events, ev->m_arg);
            if ((events[i].events & EPOLLOUT) && (ev->m_event & EPOLLOUT))  //�������¼�
               ev->call_back(ev->m_fd, events[i].events, ev->m_arg);
        }
    }
    threadpool_destroy(TaskPool, 0);

    return 0;
}  
