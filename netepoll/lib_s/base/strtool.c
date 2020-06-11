/* --------------------------------------------------------
 * FileName : strtool.c (1.101)
 * Function : check string format, change stirng format
 * -------------------------------------------------------- */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

static int mxday_month(const int, const int);

/*
 * ��  �ܣ� ����ַ����Ƿ�Ϊ�����ַ����
 * ��  �룺 Str     �ַ���ָ��
 *          Len     ��鳤��
 * ����ֵ�� 1       �� 
 *          0       ��
 */
int isDigit (const char *Str, const int Len)
{
    int i;
    for (i=0; i<Len; i++)
    {
        if (isdigit(Str[i])==0)
        {
            return 0;
        }
    }
    return 1;
}

/*
 * ��  �ܣ� ����ַ����Ƿ�Ϊʮ�������ַ����
 * ��  �룺 Str     �ַ���ָ��
 *          Len     ��鳤��
 * ����ֵ�� 1       �� 
 *          0       ��
 */
int isHex (const char *Str, const int Len)
{
    int  i;
    for (i=0; i<Len; i++)
    {
        if (isxdigit(Str[i])==0)
        {
            return 0;
        }
    }
    return 1;
}

/*
 * ��  �ܣ� ����ַ����Ƿ�Ϊ���֡���ĸ�ַ����
 * ��  �룺 Str     �ַ���ָ��
 *          Len     ��鳤��
 * ����ֵ�� 1       �� 
 *          0       ��
 */
int isAlnum (const char *Str, const int Len)
{
    int  i;
    for (i=0; i<Len; i++)
    {
        if (isalnum(Str[i])==0)
        {
            return 0;
        }
    }
    return 1;
}

/*
 * ��  �ܣ� ����ַ����Ƿ�Ϊ��ĸ�ַ����
 * ��  �룺 Str     �ַ���ָ��
 *          Len     ��鳤��
 * ����ֵ�� 1       �� 
 *          0       ��
 */
int isAlpha (const char *Str, const int Len)
{
    int  i;
    for (i=0; i<Len; i++)
    {
        if (isalpha(Str[i])==0)
        {
            return 0;
        }
    }
    return 1;
}

/*
 * ��  �ܣ� ����ַ����Ƿ�ΪASCII�ַ����
 * ��  �룺 Str     �ַ���ָ��
 *          Len     ��鳤��
 * ����ֵ�� 1       �� 
 *          0       ��
 */
int isAscii (const char *Str, const int Len)
{
    int  i;
    for (i=0; i<Len; i++)
    {
        if (isascii(Str[i])==0)
        {
            return 0;
        }
    }
    return 1;
}

/*
 * ��  �ܣ� ����ַ����Ƿ�Ϊ�ɴ�ӡ�ַ����
 * ��  �룺 Str     �ַ���ָ��
 *          Len     ��鳤��
 * ����ֵ�� 1       �� 
 *          0       ��
 */
int isPrint (const char *Str, const int Len)
{
    int  i;
    for (i=0; i<Len; i++)
    {
        if (isprint(Str[i])==0)
        {
            return 0;
        }
    }
    return 1;
}

/*
 * ��  �ܣ� ����Ƿ�Ϊ��ȷ�����
 * ��  �룺 Year    ���
 * ����ֵ�� 1       �� 
 *          0       ��
 */
int isYear (const int Year)
{
    if (Year<1700 || Year>3000)
    {
        return 0;
    }
    return 1;
}

/*
 * ��  �ܣ� ����Ƿ�Ϊ����
 * ��  �룺 Year    ���
 * ����ֵ�� 1       �� 
 *          0       ��
 */
int isLeapYear (const int Year)
{
    if ((Year%4==0&&Year%100!=0) || Year%400==0 || Year%1000==0)
    {
        return 1;
    }
    return 0;
}

/*
 * ��  �ܣ� �����Ϊ��ȷ���·�
 * ��  �룺 Month   �·�
 * ����ֵ�� 1       �� 
 *          0       ��
 */
int isMonth (const int Month)
{
    if (Month>12 || Month<1)
    {
        return 0;
    }
    return 1;
}

/*
 * ��  �ܣ� ��ȡָ�����ָ���µ�������� 
 * ��  �룺 Year    ��� 
 *          Month   �·�
 * ����ֵ��>0       ָ���µ�������� 
 *          0       ʧ�� 
 */
int mxday_month (const int Year, const int Month)
{
    int  mxday;
    const int  day[13]={0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if ( !isYear(Year) || !isMonth(Month) )
    {
        return 0;
    }

    mxday = day[Month];
    if ( isLeapYear(Year) && Month==2)
    {
        mxday += 1;
    }

    return mxday;
}

/*
 * ��  �ܣ� ����ַ����Ƿ�Ϊ�����ַ����
 * ��  �룺 Str     �ַ���ָ��
 *          Len     ��鳤��
 * ����ֵ�� 1       �� 
 *          0       ��
 */
int isDay (const int Year, const int Month, const int Day)
{
    if (Day<1 || Day>mxday_month(Year, Month))
    {
        return 0;
    }
    return 1;
}

/*
 * ��  �ܣ� ����ַ����Ƿ�Ϊ��ȷ�����ڸ�ʽ
 * ��  �룺 Str     �ַ���ָ��
 * ����ֵ�� 1       �� 
 *          0       ��
 * ˵  ���� Ĭ�����ڸ�ʽΪ-- YYYYMMDD
 */
int isDate (const char *Str)
{
    int  year;
    int  month;
    int  day;

    if ( !isDigit(Str, 8) || strlen(Str)!= 8)
    {
        return 0;
    }

    year= (Str[0]-'0')*1000 + (Str[1]-'0')*100 + (Str[2]-'0')*10 + (Str[3]-'0');
    month= (Str[4]-'0')*10 + (Str[5]-'0');
    day= (Str[6]-'0')*10 + (Str[7]-'0');
    if ( !isDay(year, month, day) )
    {
        return 0;
    }

    return 1;
}

/*
 * ��  �ܣ� ����ַ����Ƿ�Ϊ��ȷ��ʱ���ʽ
 * ��  �룺 Str     �ַ���ָ��
 * ����ֵ�� 1       �� 
 *          0       ��
 * ˵  ���� Ĭ��ʱ���ʽΪ-- hhmmss 
 */
int isTime (const char *Str)
{
    int  hour;
    int  minute;
    int  second;

    if ( !isDigit(Str, 6) || strlen(Str)!= 6)
    {
        return 0;
    }
    
    hour = (Str[0]-'0')*10 + (Str[1]-'0');
    minute = (Str[2]-'0')*10 + (Str[3]-'0');
    second = (Str[4]-'0')*10 + (Str[5]-'0');
    if ((hour>23||hour<0) || (minute>59||minute<0) || (second>59||second<0))
    {
        return 0;
    }

    return 1;
}

/*
 * ��  �ܣ� ����ַ����Ƿ�Ϊ��ȷ������ʱ���ʽ
 * ��  �룺 Str     �ַ���ָ��
 * ����ֵ�� 1       �� 
 *          0       ��
 * ˵  ���� Ĭ������ʱ���ʽΪ-- YYYYMMDDhhmmss 
 */
int isDatetime (const char *Str)
{
    char date[8+1];
    char time[6+1];

    memset (date, 0, sizeof(date));
    memset (time, 0, sizeof(time));

    memcpy (date, Str, 8);
    memcpy (time, Str+8, 6);

    if (!isDate(date) || !isTime(time))
    {
        return 0;
    }
    return 1;
}

/*
 * ��  �ܣ� ����ַ����Ƿ�Ϊ��ȷ�Ĵ���ʽ�����ڸ�ʽ
 * ��  �룺 Str     �ַ���ָ��
 * ����ֵ�� 1       �� 
 *          0       ��
 * ˵  ���� Ĭ�ϴ���ʽ�����ڸ�ʽΪ-- YYYY/MM/DD������/Ϊ�ָ�����������
 */
int isDateFmt (const char *Str)
{
    char date[8+1];

    if (strlen(Str) != 10)
    {
        return 0;
    }

    memset (date, 0, sizeof(date));

    memcpy (date, Str, 4);
    memcpy (date+4, Str+4+1, 2);
    memcpy (date+4+2, Str+4+1+2+1, 2);

    if (!isDate(date))
    {
        return 0;
    }

    return 1;
}

/*
 * ��  �ܣ� ����ַ����Ƿ�Ϊ��ȷ�Ĵ���ʽ��ʱ���ʽ
 * ��  �룺 Str     �ַ���ָ��
 * ����ֵ�� 1       �� 
 *          0       ��
 * ˵  ���� Ĭ�ϴ���ʽ��ʱ���ʽΪ-- hh:mm:ss������:Ϊ�ָ�����������
 */
int isTimeFmt (const char *Str)
{
    char time[6+1];

    if (strlen(Str) != 8)
    {
        return 0;
    }

    memset (time, 0, sizeof(time));

    memcpy (time, Str, 2);
    memcpy (time+2, Str+2+1, 2);
    memcpy (time+2+2, Str+2+1+2+1, 2);

    if (!isTime(time))
    {
        return 0;
    }
    return 1;
}

/*
 * ��  �ܣ� ���ִ�(����ת�����ַ�)ת�ɴ�д
 * ��  �룺 Str     �ַ���ָ��
 * ����ֵ�� ת������ַ���
 */
char *str2upper(char *Str)
{
    char *ptr;
    int  cunt, len;

    len = strlen(Str);
    ptr = Str;
    for (cunt=0; cunt<len; cunt++)
    {
        *ptr = toupper(*ptr);
        ptr++;
    }

    return (Str);
}

/*
 * ��  �ܣ� ���ִ�(����ת�����ַ�)ת��Сд
 * ��  �룺 Str     �ַ���ָ��
 * ����ֵ�� ת������ַ���
 */
char *str2lower(char *Str)
{
    char *ptr;
    int  cunt, len;

    len = strlen(Str);
    ptr = Str;
    for (cunt=0; cunt<len; cunt++)
    {
        *ptr = tolower(*ptr);
        ptr++;
    }

    return (Str);
}


char  *genRandomStr(char* output, int length)
{
    int flag, i;
    struct timeval tpstart;
    gettimeofday(&tpstart, NULL);
    srand(tpstart.tv_usec);

    for (i = 0; i < length; i++)
    {
        flag = rand() % 3;
        switch (flag)
	{
	case 0:
	    output[i] = 'A' + rand() % 26;
	    break;
	case 1:
	    output[i] = 'a' + rand() % 26;
	    break;
	case 2:
	    output[i] = '0' + rand() % 10;
	    break;
	default:
	    output[i] = 'x';
	    break;
        }
    }
    return (char *)output;
}
