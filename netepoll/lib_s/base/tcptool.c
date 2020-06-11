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
 * ��  �ܣ� ��ָ���ļ������ȡ���������� 
 * ��  �룺 int Read_fd  �ļ����
 *          int Read_len  ��ȡ���ݵĳ���
 *          int Timeout  ��ʱʱ��
 * ��  ���� unsigned char *Buf  ��ȡ�����ݻ�����
 * ����ֵ�� 0 �ɹ�  -1 �Զ˹ر� -2 ��ʱ ����<0ʧ��
 */
int read_t( int Read_fd, unsigned char *Buf, int Read_len, int Timeout )
{
    int         i;
    int         read_len;

    if( Buf==NULL )
        return (-__LINE__);

    /*
     * �����ʱʱ��<=0, ���޳�ʱʱ��, read��һֱ�ȴ������㹻�����ݲŷ���
     */
    if (Timeout<=0)
        Timeout=0;

    /*
     * ���ó�ʱ
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
     * ��ȡ����
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
                // E_LOG( "������������[%d]��ʱ[%d]", Read_len, Timeout);
                return -2;
            }
            if( errno==ECONNRESET ) /* The connect reset by remote */
            {
                // �������ӱ�����(ECONNREST)
                // return (-__LINE__);
                return -1;
            }
            return (-__LINE__);
        }
        else if( i==0 ) /* remote fd closed */
        {
            // D_LOG( "Զ�������Ͽ�����" );
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
 * ��  �ܣ� ��ָ���ļ������ȡ����������(�̰߳�ȫ) 
 * ��  �룺 int Read_fd  �ļ����
 *          int Read_len  ��ȡ���ݵĳ���
 *          int Timeout  ��ʱʱ��
 * ��  ���� unsigned char *Buf  ��ȡ�����ݻ�����
 * ����ֵ�� 0 �ɹ�  -1 �Զ˹ر� -2 ��ʱ ����<0ʧ��
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
     * �����ʱʱ��<=0, ���޳�ʱʱ��, read��һֱ�ȴ������㹻�����ݲŷ���
     */
    if (Timeout<=0)
        Timeout=0;

    /*
     * ��ȡ����
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
 * ��  �ܣ� ��ָ�����accept
 * ��  �룺 int SrvFd    �������
 *          int Timeout  ��ʱʱ��
 * ����ֵ�� >=0 �ɹ������¾��  -1 ʧ�� -2 ��ʱ
 */
int accept_t( int SrvFd, int Timeout )
{
    int                 i;
    struct sockaddr     cli_addr;
    socklen_t           cli_len;

    /*
     * ��ʱ����
     */
    if (Timeout<=0)  // ��ʱʱ��<=0, ���޳�ʱʱ��, accept��һֱ�ȴ�
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
 * ��  �ܣ� ��ָ�����accept
 * ��  �룺 int SrvFd    �������
 *          int Timeout  ��ʱʱ��
 * ����ֵ�� >=0 �ɹ������¾��  -1 ʧ�� -2 ��ʱ
 */
int accept_t_r( int SrvFd, int Timeout )
{
    struct timeval      timeout;
    fd_set              afdset;
    int                 i;
    struct sockaddr     cli_addr;
    socklen_t           cli_len;

    /*
     * ��ʱ����
     */
    if (Timeout<=0)  // ��ʱʱ��<=0, ���޳�ʱʱ��, accept��һֱ�ȴ�
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
            if ( errno == EINTR )  // �źŴ�ϼ���
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
 * ��  �ܣ� �趨socketͨѶ��ʱʱ�� 
 * ��  �룺 int Sock     �������
 *          int Timeout  ��ʱʱ��
 * ����ֵ�� 0-- �ɹ�      -1-- ʧ�� 
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
 * ��  �ܣ� �趨socketͨѶ��ʱʱ�� 
 * ��  �룺 int Sock      �������
 *          int Times     ������buff����
 * ����ֵ�� 0-- �ɹ�    -1-- ʧ�� 
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

// ����SockΪNODELAY
//
int setSockNoDelay(int Fd)
{
    int enable = 1;

    if ( setsockopt(Fd, IPPROTO_TCP, TCP_NODELAY, (void *)&enable,
                    sizeof(enable)) < 0 )
        return -1;
    return 0;
}

// ����SockΪ������
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

// ����SockΪ����
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
 * tread ��ȡ����ʱ�ṩ��ʱ����
 */
int tread(int SockFd, unsigned char *Buf, int Len, int TimeOut)
{
    struct timeval     timeout;
    fd_set             rfdset;
    int                i;

    // �޳�ʱ���ƣ���ֱ�ӵ��� read
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


