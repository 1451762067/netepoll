/* ------------------------------------------------------------------
 * FileName : getCliSocket.c (1.100)
 * Function : Get a Socket connect to the Remote Server 
 *            with service or port ( Tcp or Udp )
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
 * ��  �ܣ� ��ȡָ���ͻ��˵��׽ӿ�
 * ��  �룺 char *host  ������IP��ַ
 *          char *service  �������˿ں�
 *          char *transport  ����Э��
 * ��  ���� ��
 * ����ֵ�� ��0  �׽ӿں�  -1 ʧ��
 */

int getCliSocket(const char *host,const char *service, const char *transport)
{
    struct sockaddr_in  serv_addr,cli_addr;
    int                 s;

    bzero( (char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(host);
    serv_addr.sin_port = htons((u_short)atoi(service));

    s = (memcmp(transport,"udp",3)==0) ?
            socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP) :
            socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( s < 0 )
    {
        return(-__LINE__);
    }

    if(memcmp(transport,"udp",3)==0)
    {
        /*
         * Bind Our local address for us.
         */
        bzero((char *) &cli_addr,sizeof(cli_addr));
        cli_addr.sin_family      = AF_INET;
        cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        cli_addr.sin_port        = htons(0);

        if(bind(s,(struct sockaddr *)&cli_addr,sizeof(cli_addr))<0)
        {
            close(s);
            return(-__LINE__);
        }
    }

    if( connect(s,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    {
        close(s);
        return(-__LINE__);
    }

    return s;
}
