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
 * 功  能： 检查字符串是否为数字字符组成
 * 输  入： Str     字符串指针
 *          Len     检查长度
 * 返回值： 1       是 
 *          0       否
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
 * 功  能： 检查字符串是否为十六进制字符组成
 * 输  入： Str     字符串指针
 *          Len     检查长度
 * 返回值： 1       是 
 *          0       否
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
 * 功  能： 检查字符串是否为数字、字母字符组成
 * 输  入： Str     字符串指针
 *          Len     检查长度
 * 返回值： 1       是 
 *          0       否
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
 * 功  能： 检查字符串是否为字母字符组成
 * 输  入： Str     字符串指针
 *          Len     检查长度
 * 返回值： 1       是 
 *          0       否
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
 * 功  能： 检查字符串是否为ASCII字符组成
 * 输  入： Str     字符串指针
 *          Len     检查长度
 * 返回值： 1       是 
 *          0       否
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
 * 功  能： 检查字符串是否为可打印字符组成
 * 输  入： Str     字符串指针
 *          Len     检查长度
 * 返回值： 1       是 
 *          0       否
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
 * 功  能： 检查是否为正确的年份
 * 输  入： Year    年份
 * 返回值： 1       是 
 *          0       否
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
 * 功  能： 检查是否为润年
 * 输  入： Year    年份
 * 返回值： 1       是 
 *          0       否
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
 * 功  能： 检查是为正确的月份
 * 输  入： Month   月份
 * 返回值： 1       是 
 *          0       否
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
 * 功  能： 获取指定年份指定月的最大天数 
 * 输  入： Year    年份 
 *          Month   月份
 * 返回值：>0       指定月的最大天数 
 *          0       失败 
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
 * 功  能： 检查字符串是否为数字字符组成
 * 输  入： Str     字符串指针
 *          Len     检查长度
 * 返回值： 1       是 
 *          0       否
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
 * 功  能： 检查字符串是否为正确的日期格式
 * 输  入： Str     字符串指针
 * 返回值： 1       是 
 *          0       否
 * 说  明： 默认日期格式为-- YYYYMMDD
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
 * 功  能： 检查字符串是否为正确的时间格式
 * 输  入： Str     字符串指针
 * 返回值： 1       是 
 *          0       否
 * 说  明： 默认时间格式为-- hhmmss 
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
 * 功  能： 检查字符串是否为正确的日期时间格式
 * 输  入： Str     字符串指针
 * 返回值： 1       是 
 *          0       否
 * 说  明： 默认日期时间格式为-- YYYYMMDDhhmmss 
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
 * 功  能： 检查字符串是否为正确的带格式的日期格式
 * 输  入： Str     字符串指针
 * 返回值： 1       是 
 *          0       否
 * 说  明： 默认带格式的日期格式为-- YYYY/MM/DD，其中/为分隔符，不限制
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
 * 功  能： 检查字符串是否为正确的带格式的时间格式
 * 输  入： Str     字符串指针
 * 返回值： 1       是 
 *          0       否
 * 说  明： 默认带格式的时间格式为-- hh:mm:ss，其中:为分隔符，不限制
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
 * 功  能： 把字串(可以转换的字符)转成大写
 * 输  入： Str     字符串指针
 * 返回值： 转换后的字符串
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
 * 功  能： 把字串(可以转换的字符)转成小写
 * 输  入： Str     字符串指针
 * 返回值： 转换后的字符串
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
