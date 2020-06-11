
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
 * 功  能： 生成LRC校验码 
 * 输  入： unsigned char *Buf  字符串 
 *          int Len  字符串长度
 * 输  出： 无 
 * 返回值： ( unsigned char ) LRC校验码
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
 * 功  能： 生成空格，数量不大于512字节 
 * 输  入： int number  生成空格的字节数 
 * 输  出： 无 
 * 返回值： ( char *) 填写空格的字符串 
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
 * 功  能： 获取系统时间并按照指定格式生成字符串 
 * 输  入： char *Format  字符串格式 
 *          int  Len      Date限制长度
 * 输  出： char *Date  系统时间字符串 
 * 返回值： 0 成功 
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
 * 功  能： 获取系统日期并以YYYYMDD格式生成字符串
 * 输  入： int Len      Date限制长度 
 * 输  出： char *Date   系统日期字符串
 * 返回值： 0 成功 
 */
int getDate( char *Date, int Len )
{
    return( getDateStr(Date,"%Y%m%d", Len ) );
}

/*
 * 功  能： 获取系统时间并以HH:MM:SS格式生成字符串
 * 输  入： int Len     Date限制长度 
 * 输  出： char *Time  系统时间字符串 
 * 返回值： 0 成功 
 */
int getTime( char *Time, int Len )
{
    return( getDateStr(Time,"%T", Len) );
}

/*
 * 功  能： 转换为BCD码 
 * 输  入： char *buf  字符串 
 * 输  出： 无 
 * 返回值： (int) 转换后的值 
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
 * 功  能： ASCII码字符串转换为BCD码字符串 
 * 输  入： unsigned char AscStr[]  ASCII码字符串 
 *          int AscLen  ASCII码字符串长度
 * 输  出： unsigned char Bcdstr[]  BCD码字符串 
 * 返回值： 0 成功 
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
 * 功  能： BCD码字符串转换为ASCII码字符串 
 * 输  入： unsigned char BcdStr[]  BCD码字符串 
 *          int StrLen  BCD码字符串长度
 * 输  出： unsigned char AscStr[]  ASCII码字符串 
 * 返回值： 0 成功 
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
 * 功  能： ASCII码字符串转换为BCD码字符串(当AscLen为奇数时，转换成BCD码字符串右边补零) 
 * 输  入： unsigned char AscStr[]  ASCII码字符串 
 *          int AscLen  ASCII码字符串长度
 * 输  出： unsigned char Bcdstr[]  BCD码字符串 
 * 返回值： 0 成功 
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
 * 功  能： 删除字符串右边的空格 
 * 输  入： char *string  字符串 
 * 输  出： 无 
 * 返回值： ( char * )去除右边空格后的字符串 
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
 * 功  能： 删除字符串左边的空格 
 * 输  入： char *string  字符串 
 * 输  出： 无 
 * 返回值： ( char * )去除左边空格后的字符串
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
 * 功  能： 删除字符串中所有的空格
 * 输  入： char *string  字符串 
 * 输  出： 无 
 * 返回值： ( char * )去除所有空格后的字符串 
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
 * 将输入字符串按每8个字节分组，最后不足8位补数字0，然后从第1组开始逐组异或
 * ( A ^ ( A + 8 ) ^ ( A + 16 ) ... ) 
 * 
 * 输入参数:	szInData
 * 		nLen
 * 输出参数:	szOutData
 */

void XOR( unsigned char *szInData, int nLen, unsigned char *szOutData )
{
	int 	i, j;
	char 	tmp[20];

	memset( szOutData, 0, 8 );

	for ( i = 0; i < nLen; i += 8 ) 
	{
		/* 不足8位右补 0x00 */
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
