

#include "netepoll.h"

/*********************************************************************************
  *Function:    //testpacket
  *Description：//接收数据的完成性判断，解决拆包粘包问题，目前的设置的报文是 2bytes长度够+数据
  *Input:       //struct my_events
  *Output:      //无
  *Return:      //说明完整性，DATA_COMPLETE 完成可进入任务处理，DATA_ERROR 格式错误 应清理连接, DATA_CONTINUE 无动作
  *Others:      //无
**********************************************************************************/
DATA_RET testpacket(struct my_events *ev)
{
    int         len = 0;

    len = ev->m_rcv_buf[0] * 256 + ev->m_rcv_buf[1];
    if (len == (ev->m_rcv_buf_len - 2))  //数据接收结束
    {
        D_LOG("数据接收完成");
        return DATA_COMPLETE;
    }
    else if (len < (ev->m_rcv_buf_len - 2))  //数据错误
    {
        D_LOG("数据超长[%d > %d]", ev->m_rcv_buf_len - 2, len);
        return DATA_ERROR;
    }
    else  //继续接收
    {
        D_LOG("数据接收未完成，将继续...");
        return DATA_CONTINUE;
    }
}

/*********************************************************************************
  *Function:    //mytask 线程函数
  *Description：//任务处理函数，根据业务需要改写此代码 
  *Input:       //arg，使用者进入任务处理后应根据实参类型做强制转换
  *Output:      //无
  *Return:      //无
  *Others:      //无
**********************************************************************************/
void mytask(void* arg) 
{
    //TODO
    struct my_events *ev = (struct my_events *)arg;
    send(ev->m_fd, ev->m_rcv_buf, ev->m_rcv_buf_len, 0);
    D_LOG("send respond end:[len = %d]", ev->m_rcv_buf_len);
    close(ev->m_fd);
    free((void *)ev);

    sleep(1);
}
