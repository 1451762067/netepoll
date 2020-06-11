
/* --------------------------------------------------------
 * FileName : tool.c (1.100)
 * Function : The tool function file
 * -------------------------------------------------------- */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

/*
 * ��  �ܣ� ����LRCУ���� 
 * ��  �룺 unsigned char *Buf  �ַ��� 
 *          int Len  �ַ�������
 * ��  ���� �� 
 * ����ֵ�� ( unsigned char ) LRCУ����
 */
unsigned char lrcBuf(unsigned char *Buf, int Len)
{
    unsigned char   c=0;
    int             i;

    for( i=0; i<Len; i++ )
    {
        c ^= Buf[i];
    }

    return c;
}

/*
 * ��  �ܣ� ���ɿո�����������512�ֽ� 
 * ��  �룺 int number  ���ɿո���ֽ��� 
 * ��  ���� �� 
 * ����ֵ�� ( char *) ��д�ո���ַ��� 
 */
char *space(int number)
{
    static char _space_buffer[513];

    memset(_space_buffer,' ',512);
    *(_space_buffer+512)='\0';
    if(number<=0||number>512)
        return("");
    return(_space_buffer+512-number);
}

/*
 * ��  �ܣ� ��ȡϵͳʱ�䲢����ָ����ʽ�����ַ��� 
 * ��  �룺 char *Format  �ַ�����ʽ 
 *          int  Len      Date���Ƴ���
 * ��  ���� char *Date  ϵͳʱ���ַ��� 
 * ����ֵ�� 0 �ɹ� 
 */
int getDateStr( char *Date, const char *Format, int Len )
{
    time_t         t;
    struct tm      tm_1;

    if ( Format == NULL )
    {
        return -1;
    }

    time(&t);
    /* memcpy( &tm_1, localtime(&t), sizeof(struct tm) ); */
    memset( &tm_1, 0, sizeof(struct tm) );
    localtime_r(&t, &tm_1);
    strftime( Date, Len, Format, &tm_1 );

    return 0;
}

/*
 * ��  �ܣ� ��ȡϵͳ���ڲ���YYYYMDD��ʽ�����ַ���
 * ��  �룺 int Len      Date���Ƴ��� 
 * ��  ���� char *Date   ϵͳ�����ַ���
 * ����ֵ�� 0 �ɹ� 
 */
int getDate( char *Date, int Len )
{
    return( getDateStr(Date,"%Y%m%d", Len ) );
}

/*
 * ��  �ܣ� ��ȡϵͳʱ�䲢��HH:MM:SS��ʽ�����ַ���
 * ��  �룺 int Len     Date���Ƴ��� 
 * ��  ���� char *Time  ϵͳʱ���ַ��� 
 * ����ֵ�� 0 �ɹ� 
 */
int getTime( char *Time, int Len )
{
    return( getDateStr(Time,"%T", Len) );
}

/*
 * ��  �ܣ� ת��ΪBCD�� 
 * ��  �룺 char *buf  �ַ��� 
 * ��  ���� �� 
 * ����ֵ�� (int) ת�����ֵ 
 */
int toBcd(char *buf)
{
    int i,j;

    i = buf[0];
    if (i >= '0' && i <= '9') i -= '0';
    else if (i >= 'a' && i <= 'f') i = i - 'a' + 10;
    else if (i >= 'A' && i <= 'F') i = i - 'A' + 10;
    j = i * 16;
    i = buf[1];
    if (i >= '0' && i <= '9') i -= '0';
    else if (i >= 'a' && i <= 'f') i = i - 'a' + 10;
    else if (i >= 'A' && i <= 'F') i = i - 'A' + 10;
    return(j+i);
}

/*
 * ��  �ܣ� ASCII���ַ���ת��ΪBCD���ַ��� 
 * ��  �룺 unsigned char AscStr[]  ASCII���ַ��� 
 *          int AscLen  ASCII���ַ�������
 * ��  ���� unsigned char Bcdstr[]  BCD���ַ��� 
 * ����ֵ�� 0 �ɹ� 
 */
int ascToBcd( unsigned char Bcdstr[], unsigned char AscStr[], int AscLen )
{
    int i,t;
    unsigned char *tmp=NULL;

    t = (AscLen +1)/2 * 2;
    tmp=(unsigned char *)malloc((t+1)*sizeof(unsigned char));
    if ( tmp != NULL )
    {
        memset(tmp,'0',t);
        memcpy(tmp+t-AscLen,AscStr,AscLen);
        for( i=0 ; i<(AscLen+1)/2; i++ )
        {
            Bcdstr[i] = (unsigned char)toBcd( (char *)(tmp+2*i) );
        }
        free(tmp);
    }

    return(0);
}

/*
 * ��  �ܣ� BCD���ַ���ת��ΪASCII���ַ��� 
 * ��  �룺 unsigned char BcdStr[]  BCD���ַ��� 
 *          int StrLen  BCD���ַ�������
 * ��  ���� unsigned char AscStr[]  ASCII���ַ��� 
 * ����ֵ�� 0 �ɹ� 
 */
int bcdToAsc( unsigned char AscStr[], unsigned char BcdStr[], int StrLen )
{
    int i;
    char *tmp;

    tmp=(char *)malloc((StrLen+2)*sizeof(unsigned char));
    if ( tmp != NULL )
    {
        for( i=0 ; i<(StrLen+1)/2 ; i++ )
            sprintf( tmp + 2*i,"%02X",BcdStr[i] );

        if( (StrLen % 2) == 1 && tmp[0] == '0') i=1;
        else i=0;

        memcpy( AscStr,tmp+i,StrLen );
        AscStr[StrLen]=0;
        free(tmp);
    }

    return(0);
}

/*
 * ��  �ܣ� ASCII���ַ���ת��ΪBCD���ַ���(��AscLenΪ����ʱ��ת����BCD���ַ����ұ߲���) 
 * ��  �룺 unsigned char AscStr[]  ASCII���ַ��� 
 *          int AscLen  ASCII���ַ�������
 * ��  ���� unsigned char Bcdstr[]  BCD���ַ��� 
 * ����ֵ�� 0 �ɹ� 
 */
int ascToBcdLeft( unsigned char Bcdstr[], unsigned char AscStr[], int AscLen )
{
    int i,t;
    unsigned char *tmp;

    t = (AscLen +1)/2 * 2;
    tmp=(unsigned char *)malloc((t+1)*sizeof(unsigned char));
    if ( tmp != NULL )
    {
        memset(tmp,'0',t);
        memcpy(tmp,AscStr,AscLen);
        for( i=0 ; i<(AscLen+1)/2; i++ )
            Bcdstr[i] = (unsigned char)toBcd( (char *)(tmp+2*i) );
        free((char *)tmp);
    }

    return(0);
}

/*
 * ��  �ܣ� ɾ���ַ����ұߵĿո� 
 * ��  �룺 char *string  �ַ��� 
 * ��  ���� �� 
 * ����ֵ�� ( char * )ȥ���ұ߿ո����ַ��� 
 */
char *deleRhtSpace(char *string)
{
    int l;

    l=strlen(string)-1;
    while(l>=0&&*(string+l)==' ')
            l--;
    l++;
    *(string+l)='\0';
    return(string);
}

/*
 * ��  �ܣ� ɾ���ַ�����ߵĿո� 
 * ��  �룺 char *string  �ַ��� 
 * ��  ���� �� 
 * ����ֵ�� ( char * )ȥ����߿ո����ַ���
 */
char *deleLftSpace(char *string)
{
    int l,i;
    char *ptr,str[512];

    l=strlen(string);
    ptr=string;
    for(i=0;i<l;i++,ptr++)
    {
        if(*(string+i)!=' ')
            break;
    }
    memset(str, 0, sizeof(str));
    strncpy(str, ptr, sizeof(str)-1);
    strcpy(string,str);

    return(string);
}

/*
 * ��  �ܣ� ɾ���ַ��������еĿո�
 * ��  �룺 char *string  �ַ��� 
 * ��  ���� �� 
 * ����ֵ�� ( char * )ȥ�����пո����ַ��� 
 */
char *deleAllSpace(char *str)
{
    char string[512],*ptr;
    int l,n;

    memset(string, 0, sizeof(string));
    strncpy(string, str, sizeof(string)-1);
    l=strlen(string);
    ptr=string;
    for(n=0;n<l;n++)
    {
        if(*(string+n)!=' ')
            *ptr++=*(string+n);
    }
    *ptr='\0';
    strcpy(str,string);
    return(str);
}

/*
 * exchange the ascii code to bcd code
 */
void asc_to_bcd ( unsigned char *bcd_buf, unsigned char *ascii_buf, 
                  int conv_len, unsigned char type )
{
    int         cnt;
    char        ch, ch1;

    if ( conv_len & 0x01 && type )
        ch1 = 0;
    else
        ch1 = 0x55;

    for ( cnt = 0; cnt < conv_len; ascii_buf ++, cnt ++ )
    {
        if ( * ascii_buf >= 'a' )
            ch = * ascii_buf - 'a' + 10;
        else if ( *ascii_buf >= 'A' )
            ch = * ascii_buf - 'A' + 10;
        else if ( * ascii_buf >= '0' )
            ch = * ascii_buf - '0';
        else
            ch = 0;

        if ( ch1 == 0x55 )
            ch1 = ch;
        else
        {
            * bcd_buf ++ = ( ch1 << 4 ) | ch;
            ch1 = 0x55;
        }
    }

    if ( ch1 != 0x55 )
        * bcd_buf = ch1 << 4;

    return;
}

/*
 * exchange bcd code to ascii code 
 */
void bcd_to_asc ( unsigned char *ascii_buf, unsigned char *bcd_buf,
                  int conv_len, unsigned char type )
{
    int         cnt;

    if ( conv_len & 0x01 && type ) 
    {
        cnt = 1;
        conv_len ++;
    }
    else
        cnt = 0;
    for ( ; cnt < conv_len; cnt ++, ascii_buf ++ ) 
    {
        *ascii_buf = ( (cnt&0x01) ? ( *bcd_buf++ & 0x0f ) : ( *bcd_buf >> 4 ) );
        *ascii_buf += ( ( *ascii_buf > 9 ) ? ( 'A' - 10 ) : '0' );
    }

    return;
}

/*
 * �������ַ�����ÿ8���ֽڷ��飬�����8λ������0��Ȼ��ӵ�1�鿪ʼ�������
 * ( A ^ ( A + 8 ) ^ ( A + 16 ) ... ) 
 * 
 * �������:	szInData
 * 		nLen
 * �������:	szOutData
 */

void XOR( unsigned char *szInData, int nLen, unsigned char *szOutData )
{
	int 	i, j;
	char 	tmp[20];

	memset( szOutData, 0, 8 );

	for ( i = 0; i < nLen; i += 8 ) 
	{
		/* ����8λ�Ҳ� 0x00 */
		if ( ( nLen - i ) < 8 ) 
		{
			memset ( tmp, '\0', 8 );
			memcpy ( tmp, szInData+i, nLen-i );
			for ( j = 0; j < 8; j ++ ) 
			{
				szOutData[j] ^= tmp[j];
			}
		} 
		else 
		{
			for ( j = 0; j < 8; j ++ ) 
			{
				szOutData[j] ^= szInData[i+j];
			}
		}
	}

	return;
}
