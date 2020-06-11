/* ------------------------------------------------------------------
 * FileName : getSrvSocket.c (1.100)
 * Function : get a socket in the service or port ( Tcp or Udp )
 * ------------------------------------------------------------------ */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

/*
 * 功  能： 开启指定的服务并获取服务的套接口
 * 输  入： char *SrvAddr  服务端口
 *          char *SockType  传输协议
 *          int Qlen  端口号的长度
 * 输  出： 无
 * 返回值： 非0  套接口号  -1 失败
 */
int getSrvSocket(const char *SrvAddr, const char *SockType, int Qlen)
{
    struct sockaddr_in  serv_addr;
    int                 s, reuse;
    char                service[128];
    char                *srv_port;

    bzero( (char *)&serv_addr, sizeof(serv_addr));
    memset( service, 0, sizeof(service) );
    strcpy( service, SrvAddr );
    serv_addr.sin_family      = AF_INET;

    if ( ( srv_port = strstr( service, ":" ) ) == NULL )
    {
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_addr.sin_port = htons((u_short)atoi(service));
    }
    else 
    {
        serv_addr.sin_port = htons((u_short)atoi(srv_port+1));
        *srv_port = 0;
        serv_addr.sin_addr.s_addr = inet_addr(service);
    }

    s = (memcmp(SockType,"udp",3)==0) ? 
            socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP) :
            socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( s < 0 )
    {
        return(-__LINE__);
    }

#ifdef Linux
    reuse = 5;
    if( setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse))<0 )
    {
        close(s);
        return(-__LINE__);
    }
#endif

    if (bind(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        close(s);
        return(-__LINE__);
    }

    if ( memcmp(SockType,"udp",3)!=0 && listen(s, Qlen) <0 )
    {
        close(s);
        return(-__LINE__);
    }

    return s;
}
