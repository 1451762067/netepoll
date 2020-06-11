
#include "netepoll.h"


/*���ṹ���Ա������ʼ��*/
void eventsetinit(struct my_events *my_ev, int fd, void (*call_back)(int fd, int event, void *arg), void *event_arg)
{
    my_ev->m_fd       = fd;
    my_ev->m_event    = 0;
    my_ev->m_status   = 0;
    my_ev->m_lasttime = time(NULL);
    my_ev->m_arg      = event_arg;
    my_ev->call_back  = call_back;

    return ;
}
/*��������� �ļ��������Ͷ�Ӧ�Ľṹ��*/
void eventadd(int ep_fd, int event, struct my_events *my_ev)
{
    int op;
    struct epoll_event epv;
    epv.data.ptr = my_ev;
    epv.events   = my_ev->m_event = event;
  
    if (my_ev->m_status == 0)
    {
        op = EPOLL_CTL_ADD;
    }
    else
    {
        D_LOG(" add error: already on tree:[%d-%s]", errno, strerror(errno));
        return ;
    }

    if (epoll_ctl(ep_fd, op, my_ev->m_fd, &epv) < 0)
    {
        D_LOG(" event add false [fd= %d] [events= %d] [%d-%s]", my_ev->m_fd, my_ev->m_event, errno, strerror(errno));
    }
    else
    {
        my_ev->m_status = 1;
        D_LOG(" event add ok [fd= %d] [events= %d]", my_ev->m_fd, my_ev->m_event);
    }

    return ;
}
/*�Ӻ������ɾ�� �ļ��������Ͷ�Ӧ�Ľṹ��*/
void eventdel(int ep_fd, struct my_events *ev)
{
    if(ev->m_status != 1)
       return ;

    epoll_ctl(ep_fd, EPOLL_CTL_DEL, ev->m_fd, NULL);
    ev->m_status = 0;
 
    return ;
}
