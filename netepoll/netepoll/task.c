
#include "netepoll.h"

void mytask(void* arg) 
{
    struct my_events *ev = (struct my_events *)arg;
    send(ev->m_fd, ev->m_rcv_buf, ev->m_rcv_buf_len, 0);
    D_LOG("send respond end:[len = %d]", ev->m_rcv_buf_len);
    close(ev->m_fd);
    free((void *)ev);

    sleep(1);
}
