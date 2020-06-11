
#include "netepoll.h"

/*初始化监听socket*/
void initlistensocket(int ep_fd, char *port)
{
   int         listen_fd;
   int         i;
 
   /* 开启TCP服务 */
   for( listen_fd=-1, i=0; listen_fd<0; i++ )
   {
       (listen_fd = getSrvSocket(HostPort, "tcp", 5)) > 0 ? 0 : sleep( 30 * ( 1 + i / 10) );
   }
   W_LOG("[TCP]==== %s 已启动=[%s], 等待连接中...", ProgName, HostPort);
   //set no block
   fcntl(listen_fd, F_SETFL, O_NONBLOCK); 

 
   /*将listen_fd初始化*/
   eventsetinit(&Ep_events[MAX_EVENTS-1], listen_fd, acceptconnect, &Ep_events[MAX_EVENTS-1]);    
   /*将listen_fd挂上红黑树*/
   eventadd(ep_fd, EPOLLIN, &Ep_events[MAX_EVENTS-1]);

   return ;
}


//获取索引
static inline int getEventIndex()
{
    int i;
    for(i=0; i<MAX_EVENTS; i++)
    {	
        if(Ep_events[i].m_status == 0)
        return i;
    }

    return -1;
}

/*回调函数: 接收连接*/
void acceptconnect(int listen_fd, int event, void *arg)
{
    int			connect_fd;
    int			i;
    int			flag = 0;
    struct sockaddr_in connect_socket_addr;
    socklen_t		connect_socket_len;


    connect_socket_len = sizeof(connect_socket_addr);
    if ( (connect_fd=accept(listen_fd, (struct sockaddr *)&connect_socket_addr, &connect_socket_len)) <0 )
    {
        if (errno != EAGAIN && errno != EINTR)
        {
            //TODO 这里可能的处理是退出进程
     	    E_LOG("accept error:[%d-%s]", errno, strerror(errno));
		    return ;
        }
	    E_LOG("accept error:[%d-%s]", errno, strerror(errno));
        return;
    }
    D_LOG("new connection[IP=%s]", inet_ntoa(connect_socket_addr.sin_addr));

    if((i =  getEventIndex()) < 0)
    {
        E_LOG("get event index error, abandon new connect");
        close(connect_fd); 
        return ;
    }
	/* 设置非阻塞 */
    else if((flag = fcntl(connect_fd, F_SETFL, O_NONBLOCK)) <0)
    {
        E_LOG(" %s: fcntl nonblocking false, %s ", __func__, strerror(errno));
        close(connect_fd); 
        return ;
    }

    memset(&Ep_events[i], 0, sizeof(struct my_events ));
    Ep_events[i].m_addr = connect_socket_addr;
    eventsetinit(&Ep_events[i], connect_fd, recvdata, &Ep_events[i]);
    eventadd(Ep_fd, EPOLLIN, &Ep_events[i]);

    return ;
}
/*接收数据*/

void recvdata(int client_fd, int event, void *arg)
{
    int              len =0 ;
    struct my_events *ev = (struct my_events *)arg;
    struct my_events *nev;
    DATA_RET         ret;
     
    len = recv(client_fd, ev->m_rcv_buf + ev->m_rcv_buf_len,  \
                sizeof(ev->m_rcv_buf) - ev->m_rcv_buf_len, 0);
    if (len > 0)
    {
        ev->m_rcv_buf_len  += len;
	  
        D_LOG("收到新数据:");
	    D_LOG(HEX_FORMAT, ev->m_rcv_buf , ev->m_rcv_buf_len); 

        ret = testpacket(ev);  //判断数据完整性
        if(ret == DATA_COMPLETE)  //数据接收完成
        {
            //分配新结构，原来的结构将释放
            if ((nev = (struct my_events*) malloc(sizeof(struct my_events))) != NULL)
            {
                memcpy(nev, ev, sizeof(struct my_events));
                if (threadpool_add(TaskPool, &mytask, (void*)nev, 0) < 0)
                {
                    free((void*)nev);
                    close(ev->m_fd); //fd在任务中被关掉，                                   
                    E_LOG("任务添加到线程池失败");
                }
                D_LOG("任务添加到线程池成功");
            }
            else
            {
                close(ev->m_fd);
                D_LOG("malloc失败:[%d-%s]", errno, strerror(errno));
            }
            eventdel(Ep_fd, ev);
        }
        else if (ret == DATA_ERROR)
        {
            close(ev->m_fd);
            eventdel(Ep_fd, ev);    //remove read event                                    
        }
        else if(ret == DATA_CONTINUE)
        {
        }
    }
    else if (len == 0)
    {
        close(ev->m_fd);
        eventdel(Ep_fd, ev);
        E_LOG("[Client:%d] disconnection:[ret=%d][%d-%s]", len, ev->m_fd, errno, strerror(errno));
    }
    else
    {
        close(ev->m_fd);
        eventdel(Ep_fd, ev);
        E_LOG("[Client:%d] error:[ret=%d][%d-%s]", len, ev->m_fd, errno, strerror(errno));
    }
 
    return ;
}
/*发送数据*/
void senddata(int client_fd, int event, void *arg)
{
    int              len; 
    struct my_events *ev = (struct my_events *)arg;
 
    len = send(client_fd, ev->m_snd_buf + ev->m_snt_buf_len, ev->m_snd_buf_len - ev->m_snt_buf_len, 0);   //回写

    if (len > 0)
    {
        D_LOG("send[fd=%d], [len=%d] %s ", client_fd, len, ev->m_snd_buf);
        ev->m_snt_buf_len += len;
        if(ev->m_snt_buf_len == ev->m_snd_buf_len)
        {
            D_LOG("date send finish");
            eventdel(Ep_fd, ev);
            close(ev->m_fd);
        }
        //eventsetinit(ev, client_fd, recvdata, ev);
        //eventadd(Ep_fd, EPOLLIN, ev);  
    }
    else
    {
        close(ev->m_fd);
        eventdel(Ep_fd, ev);
        E_LOG("send[fd=%d] error ", client_fd);
    }
    return ;
}

