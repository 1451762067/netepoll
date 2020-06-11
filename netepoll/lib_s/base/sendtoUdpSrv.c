
/* -----------------------------------------------------------------
 * FileName : sendtoUdpSrv.c (1.100)
 * Function :
 * ----------------------------------------------------------------- */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>

/*
 * ��  �ܣ� ������Ϣ��ָ���ķ�����
 * ��  �룺 char Ip[]  ������IP��ַ
 *          char Port[]  �������˿ں�
 *          unsigned char *Buf  ���͵�����
 *          int Len  ���ݳ���
 * ��  ���� ��
 * ����ֵ�� 0 �ɹ�  -1 ʧ��
 */
int sendtoUdpSrv( char Ip[], char Port[], unsigned char *Buf, int Len )
{
    int                    sockfd;
    struct sockaddr_in     serv_addr,cli_addr;

    /* open a UDP socket */
    if ((sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0)
    {
        return(-__LINE__);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(Ip);
    serv_addr.sin_port = htons(atoi(Port));

    /* bind our local ip address */
    bzero((char *)&cli_addr,sizeof(cli_addr));
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    cli_addr.sin_port = htons(0);

    if (bind(sockfd,(struct sockaddr *)&cli_addr,sizeof(cli_addr)) < 0)
    {
        close(sockfd);
        return(-__LINE__);
    }

    if ( sendto( sockfd, Buf, Len, 0,
                 (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) < 0 )
    {
        close(sockfd);
        return(-__LINE__);
    }

    close(sockfd);

    return 0;
}
