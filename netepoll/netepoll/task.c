
#include "netepoll.h"


DATA_RET testpacket(struct my_events *ev)
{
    int         len = 0;

    len = ev->m_rcv_buf[0] * 256 + ev->m_rcv_buf[1];
    if (len == (ev->m_rcv_buf_len - 2))  //���ݽ��ս���
    {
        D_LOG("���ݽ������");
        return DATA_COMPLETE;
    }
    else if (len < (ev->m_rcv_buf_len - 2))  //���ݴ���
    {
        D_LOG("���ݳ���[%d > %d]", ev->m_rcv_buf_len - 2, len);
        return DATA_ERROR;
    }
    else  //��������
    {
        D_LOG("���ݽ���δ��ɣ�������...");
        return DATA_CONTINUE;
    }
}

void mytask(void* arg) 
{
    struct my_events *ev = (struct my_events *)arg;
    send(ev->m_fd, ev->m_rcv_buf, ev->m_rcv_buf_len, 0);
    D_LOG("send respond end:[len = %d]", ev->m_rcv_buf_len);
    close(ev->m_fd);
    free((void *)ev);

    sleep(1);
}
