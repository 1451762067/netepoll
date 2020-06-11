/* --------------------------------------------------------------
 * FileName : tcptool.c (1.100)
 * Function : 
 * -------------------------------------------------------------- */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

static int  _Exp_flag;

void _timeout_proc(int Num)
{
    _Exp_flag = 1;
}

/*
 * 功  能： 从指定文件句柄读取定长的数据 
 * 输  入： int Read_fd  文件句柄
 *          int Read_len  读取数据的长度
 *          int Timeout  超时时间
 * 输  出： unsigned char *Buf  读取的数据缓冲区
 * 返回值： 0 成功  -1 对端关闭 -2 超时 其他<0失败
 */
int read_t( int Read_fd, unsigned char *Buf, int Read_len, int Timeout )
{
    int         i;
    int         read_len;

    if( Buf==NULL )
        return (-__LINE__);

    /*
     * 如果超时时间<=0, 则无超时时间, read将一直等待接收足够的数据才返回
     */
    if (Timeout<=0)
        Timeout=0;

    /*
     * 设置超时
     */
    if( Timeout>0 )
    {
#ifdef Linux
        struct sigaction      sa_usr;
        sa_usr.sa_flags = 0; /* SA_RESART */
        sa_usr.sa_handler = _timeout_proc;
        sigaction(SIGALRM, &sa_usr, NULL);
#else
        signal(SIGALRM, _timeout_proc);
#endif
        alarm( Timeout );
        _Exp_flag = 0;
    }

    /*
     * 读取报文
     */
    read_len = 0;
    while(read_len<Read_len)
    {
        i=0;
        if( (i=read( Read_fd, Buf+read_len, Read_len-read_len))<0 )
        {
            if( Timeout>0 )
                alarm(0);
            if( _Exp_flag )
            {
                // E_LOG( "接收网络数据[%d]超时[%d]", Read_len, Timeout);
                return -2;
            }
            if( errno==ECONNRESET ) /* The connect reset by remote */
            {
                // 网络连接被重置(ECONNREST)
                // return (-__LINE__);
                return -1;
            }
            return (-__LINE__);
        }
        else if( i==0 ) /* remote fd closed */
        {
            // D_LOG( "远程主机断开连接" );
            if( Timeout>0 )
                alarm(0);
            return -1;
        }
        else
        {
            read_len+=i;
        }
    }

    if( Timeout>0 )
        alarm(0);

    return 0;
}

/*
 * 功  能： 从指定文件句柄读取定长的数据(线程安全) 
 * 输  入： int Read_fd  文件句柄
 *          int Read_len  读取数据的长度
 *          int Timeout  超时时间
 * 输  出： unsigned char *Buf  读取的数据缓冲区
 * 返回值： 0 成功  -1 对端关闭 -2 超时 其他<0失败
 */
int read_t_r( int Read_fd, unsigned char *Buf, int Read_len, int Timeout )
{
    struct timeval     timeout;
    fd_set             rfdset;
    int                i;
    int                read_len;

    if( Buf==NULL )
        return (-__LINE__);

    /*
     * 如果超时时间<=0, 则无超时时间, read将一直等待接收足够的数据才返回
     */
    if (Timeout<=0)
        Timeout=0;

    /*
     * 读取报文
     */
    read_len = 0;
    while(read_len<Read_len)
    {
        if( Timeout>0 )
        {
            FD_ZERO(&rfdset);
            FD_SET(Read_fd, &rfdset);

            timeout.tv_sec  = Timeout;
            timeout.tv_usec = 0;

            i = select(1+Read_fd, &rfdset, NULL, NULL, &timeout);
            if (i == -1)
            {
                return (-__LINE__);
            }
            else if (i == 0)
            {
                return -2;
            }
        }

        i=0;
        if( (i=read( Read_fd, Buf+read_len, Read_len-read_len))<=0 )
        {
            return ( (i==0||errno==ECONNRESET) ? -1 : -__LINE__ );
        }

        read_len += i;
    }

    return 0;
}

/*
 * 功  能： 从指定句柄accept
 * 输  入： int SrvFd    监听句柄
 *          int Timeout  超时时间
 * 返回值： >=0 成功返回新句柄  -1 失败 -2 超时
 */
int accept_t( int SrvFd, int Timeout )
{
    int                 i;
    struct sockaddr     cli_addr;
    socklen_t           cli_len;

    /*
     * 超时设置
     */
    if (Timeout<=0)  // 超时时间<=0, 则无超时时间, accept将一直等待
        Timeout=0;
    else 
    {
#ifdef Linux
        struct sigaction      sa_usr;
        sa_usr.sa_flags = 0; /* SA_RESART */
        sa_usr.sa_handler = _timeout_proc;
        sigaction(SIGALRM, &sa_usr, NULL);
#else
        signal(SIGALRM, _timeout_proc);
#endif
        alarm( Timeout );
        _Exp_flag = 0;
    }

    cli_len = sizeof(cli_addr);
    if( (i=accept( SrvFd, &cli_addr, &cli_len)) < 0)
    {
        if( Timeout>0 )
            alarm(0);
        return ( _Exp_flag ? -2 : (-__LINE__));
    }

    if( Timeout>0 )
        alarm(0);

    return i;
}

/*
 * 功  能： 从指定句柄accept
 * 输  入： int SrvFd    监听句柄
 *          int Timeout  超时时间
 * 返回值： >=0 成功返回新句柄  -1 失败 -2 超时
 */
int accept_t_r( int SrvFd, int Timeout )
{
    struct timeval      timeout;
    fd_set              afdset;
    int                 i;
    struct sockaddr     cli_addr;
    socklen_t           cli_len;

    /*
     * 超时设置
     */
    if (Timeout<=0)  // 超时时间<=0, 则无超时时间, accept将一直等待
        Timeout=0;

    timeout.tv_sec  = Timeout;
    timeout.tv_usec = 0;

    while(1)
    {
        FD_ZERO(&afdset);
        FD_SET(SrvFd, &afdset);

        i = select(SrvFd+1, &afdset, NULL, NULL, &timeout);
        if (i == -1)
        {
            if ( errno == EINTR )  // 信号打断继续
                continue;
            return (-__LINE__);
        }
        else if (i == 0)
        {
            return -2;
        }
        break;
    }

    cli_len = sizeof(cli_addr);
    if( (i=accept( SrvFd, &cli_addr, &cli_len)) < 0)
    {
        return (-__LINE__);
    }

    return i;
}

void thread_sleep(int Second)
{
    struct timespec     rqtp;
    struct timespec     rem;

    rqtp.tv_sec  = Second;
    rqtp.tv_nsec = 0;
    memset(&rem,0,sizeof(rem));
    nanosleep(&rqtp, &rem);

    return;
}

void nop_timeout(int Num)
{
    return;
}

/*
 * 功  能： 设定socket通讯超时时间 
 * 输  入： int Sock     监听句柄
 *          int Timeout  超时时间
 * 返回值： 0-- 成功      -1-- 失败 
 */
int setSockTime( int Sock, int Timeout)
{
    struct timeval tm;
    socklen_t l = sizeof(tm);

    memset(&tm, 0, sizeof(tm));
    tm.tv_sec = Timeout;

    if (setsockopt( Sock, SOL_SOCKET, SO_RCVTIMEO, &tm, l ) < 0)
    {
        return -1;
    }

    if (setsockopt( Sock, SOL_SOCKET, SO_SNDTIMEO, &tm, l ) < 0)
    {
        return -1;
    }

    return 0;
}


/*
 * 功  能： 设定socket通讯超时时间 
 * 输  入： int Sock      监听句柄
 *          int Times     调整的buff倍数
 * 返回值： 0-- 成功    -1-- 失败 
 */
int setSockBuff(int Sock, int Times)
{
    int        rcv_len, snd_len;
    socklen_t  len;

    len = sizeof(int);
    if ( getsockopt(Sock, SOL_SOCKET, SO_RCVBUF, (char *)&rcv_len, &len) < 0 )
    {
        return -1;
    }

    if ( getsockopt(Sock, SOL_SOCKET, SO_SNDBUF, (char *)&snd_len, &len) < 0 )
    {
        return -1;
    }

    rcv_len *= Times;
    snd_len *= Times;

    if ( setsockopt(Sock, SOL_SOCKET, SO_RCVBUF, (char *)&rcv_len, sizeof(int)) < 0 )
    {
        return -2;
    }

    if ( setsockopt(Sock, SOL_SOCKET, SO_SNDBUF, (char *)&snd_len, sizeof(int)) < 0 )
    {
        return -2;
    }

    return 0;
}

// 设置Sock为NODELAY
//
int setSockNoDelay(int Fd)
{
    int enable = 1;

    if ( setsockopt(Fd, IPPROTO_TCP, TCP_NODELAY, (void *)&enable,
                    sizeof(enable)) < 0 )
        return -1;
    return 0;
}

// 设置Sock为非阻塞
//
int setSockNonBlock(int Fd)
{
    int flags;

    if ( ( flags = fcntl(Fd, F_GETFL) ) < 0 )
        return flags;
    if (fcntl(Fd, F_SETFL, (flags | O_NONBLOCK) ) < 0)
        return -1;
    return 0;
}

// 设置Sock为阻塞
//
int setSockBlock(int Fd)
{
    int flags;

    if ( ( flags = fcntl(Fd, F_GETFL) ) < 0 )
        return flags;

    flags &= ~O_NONBLOCK;
    if(fcntl(Fd, F_SETFL, flags)<0)
        return -1;
    return 0;
}

/*
 * tread 读取数据时提供超时控制
 */
int tread(int SockFd, unsigned char *Buf, int Len, int TimeOut)
{
    struct timeval     timeout;
    fd_set             rfdset;
    int                i;

    // 无超时控制，则直接调用 read
    if (TimeOut<=0)
        return read( SockFd, Buf, Len );

    timeout.tv_sec  = TimeOut;
    timeout.tv_usec = 0;
    FD_ZERO(&rfdset);
    FD_SET(SockFd, &rfdset);
    i = select(1+SockFd, &rfdset, NULL, NULL, &timeout);
    if (i== -1)
    {
        return (-__LINE__);
    }
    else if (i == 0)
    {
        return -2;
    }

    return read( SockFd, Buf, Len );
}


