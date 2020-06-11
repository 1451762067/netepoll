
/* -------------------------------------------------------------------
 * FileName : openLogFile.c (1.100)
 * Function : 
 * ------------------------------------------------------------------- */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#ifdef Linux
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#include "libbase.h"


/*
 * ��  �ܣ� չ����־�ļ������ƣ���������������ʱ���ʽ�滻)
 * ��  �룺 char SrcLog[]  ԭʼ��־�ļ�����, int NameLen - DstLog��󳤶�  
 * ��  ���� char DstLog[]  ��չ����־�ļ�����
 * ����ֵ�� 0 �ɹ�  -1 ʧ��
 */
int expLogFile( char SrcLog[], char DstLog[], int NameLen )
{
    char         expand_logfile[512];

    if ( index( SrcLog, '%' ) || index( SrcLog, '$' ) )
    {
        memset( expand_logfile, 0, sizeof(expand_logfile) );
        expandEnvStr( SrcLog, expand_logfile, sizeof(expand_logfile)-1 );
        getDateStr( DstLog, expand_logfile, NameLen );
    }
    else
        strcpy( DstLog, SrcLog );

    return 0;
}

/*
 * ��  �ܣ� ����־�ļ� 
 * ��  �룺 char Logfile[]  ��־�ļ����� 
 * ��  ���� ��
 * ����ֵ�� 0 �ɹ�  -1 ʧ��
 */
int openLogFile( char Logfile[] )
{
    FILE         *fp;
    FILE         *logfp;
    char         log_filename[512];
    char         log_tmp[512];
    char         err_str[256];
    char         dir_str[512];
    char         cmd_str[512];
    struct stat  file_stat;

    /*
     * ����ǵ�һ�δ�Log�ļ��������ʼ�ļ�������
     */
    if ( getenv("__LOGFILE") == NULL )
    {
        setenv("__LOGFILE", Logfile, 1);
    }

    /* չ���ļ���,�滻���еĻ������������ڶ����ʽ */
    memset(log_filename, 0, sizeof(log_filename));
    expLogFile( Logfile, log_filename, sizeof(log_filename)-1 );

    // �����չ���ļ�����ԭʼ�ļ���һ�£�����Ҫ�ļ����л�
    if ( strcmp( log_filename, Logfile ) != 0 )
        setenv("__LOGFILE_NOW", log_filename, 1);
    else
        unsetenv("__LOGFILE_NOW");

    /*
     * �ж���־����Ŀ¼�Ƿ����,����������
     */
    memset( dir_str, 0, sizeof(dir_str) );
    memset( log_tmp, 0, sizeof(log_tmp) );
    strcpy( log_tmp, log_filename );
    strcpy( dir_str, dirname(log_tmp) );
    memset( &file_stat, 0, sizeof(struct stat) );
    if( stat( dir_str, &file_stat )<0 )
    {
        if ( errno==ENOENT ) /* Ŀ¼������ */
        {
            /* ����Ŀ¼ */
            memset( cmd_str, 0, sizeof(cmd_str) );
            sprintf( cmd_str, "mkdir -p %s", dir_str );
            if ( (fp = popen( cmd_str, "r" )) == NULL )
            {
                fprintf( stdout, "[%s-%d] : Open Log file[%s] Error\n",
                         __FILE__, __LINE__, log_filename );
                return 0;
            }
            pclose(fp);
        }
        else
        {
            fprintf( stdout, "[%s-%d] : Open Log file[%s] Error!\n",
                     __FILE__, __LINE__, log_filename );
            return 0;
        }
    }
    /* ����Ŀ¼�򷵻� */
    else if( !S_ISDIR(file_stat.st_mode) )
    {
        fprintf( stdout, "[%s-%d] : Open Log file[%s] Error!\n",
                 __FILE__, __LINE__, log_filename );
        return 0;
    }

    if( (logfp=fopen( log_filename, "a+" )) == NULL )
    {
        fprintf( stdout, "Open Log file[%s] Error\n", log_filename );
        return 0;
    }

#ifdef AIX
    if( dup2(logfp->_file,2)<0 )
    {
        memset(err_str, 0, sizeof(err_str));
        strerror_r(errno, err_str, sizeof(err_str));
        fprintf( stdout, "\ndup2 log Error[%d-%s]\n", errno, err_str );
        fclose( logfp );
        return -1;
    }
#endif

#ifdef SCO_SV
    if( dup2(logfp->__file,2)<0 )
    {
        fprintf( stdout, "\ndup2 log Error[%d-%s]\n", errno, strerror(errno));
        fclose( logfp );
        return -1;
    }
#endif

#ifdef Linux
    if( dup2(fileno(logfp),2)<0 )
    {
        memset(err_str, 0, sizeof(err_str));
        strerror_r(errno, err_str, sizeof(err_str));
        fprintf( stdout, "\ndup2 log Error[%d-%s]\n", errno, err_str );
        fclose( logfp );
        return -1;
    }
#endif

    /* success mv the logfp to the stderr, then close the logfp */
    fclose( logfp );

    return 0;
}

/*
 * ��  �ܣ� д��־�ļ�
 * ��  �룺 char *format ��־��ʽ(RAW_FORMAT��HEX_FROMAT)
 * ��  ���� ��
 * ����ֵ�� 0 �ɹ���-1 ʧ��
 */
#ifndef Linux
int lprintf(format, va_alist)
    char *format;
va_dcl
#else
int lprintf(char *format, ...)
#endif
{
    va_list         arg_prt;
    int             len=0;
    int             i;
    char            *p=NULL;
    char            time_str[50];
    char            tmp_str[50];
    char            space_str[50];
    struct timeval  tv;
    struct stat     file_stat;
    char            src_filename[512];     /* ԭʼ�ļ��� */
    char            exp_filename[512];     /* ��չ���ļ��� */
    int             ret=0;

#ifndef Linux
    va_start (arg_prt);
#else
    va_start (arg_prt, format);
#endif

    /*
     * ��־�ļ��Զ�����
     */
    if ( getenv("__LOGFILE_NOW") != NULL )
    {
        // ȡԭʼ�ļ�����������չ
        memset( src_filename, 0, sizeof(src_filename) );
        strcpy( src_filename, getenv("__LOGFILE") );

        memset( exp_filename, 0, sizeof(exp_filename));
        expLogFile( src_filename, exp_filename, sizeof(exp_filename)-1);

        /*
         * �ж���չ���ļ����͵�ǰ���Ƿ��б仯�������л��ļ�
         */
        if( strcmp( exp_filename, getenv("__LOGFILE_NOW") ) != 0  )
        {
            // ���´���־�ļ����л��ļ���
            openLogFile( src_filename );
        }
        /*
         * �ļ���û�仯������ļ�״̬��״̬�仯�����´��ļ����ļ���ɾ���ȣ�
         */
        else 
        {
            memset( &file_stat, 0, sizeof(struct stat) );
            if ( fstat(2, &file_stat)<0 || access(exp_filename, F_OK) )
            {
                ret = fprintf(stdout, "Got LogFile Stat Error=[%s]!\n",
                              strerror(errno) );
                openLogFile( src_filename );
            }
            /*
             * �л��ļ���ɾ���ļ����������̴򿪣�
             */
            else if ( file_stat.st_nlink==0 )
            {
                ret = fprintf(stdout, "File nlink=0!\n" );
                openLogFile( src_filename );
            }

            // �ļ�״̬OK,�����л��ļ�
        }
    }

    memset(time_str, 0, sizeof(time_str));
    memset(tmp_str, 0, sizeof(tmp_str));
    getDateStr( time_str, "%Y-%m-%d %T", sizeof(time_str) );
    memset(&tv, 0, sizeof(struct timeval));
    gettimeofday(&tv, NULL);
    sprintf( tmp_str, "%s-%06d", time_str, (int)tv.tv_usec );
    strcpy( time_str, tmp_str );

    if( strcmp( format, RAW_FORMAT )==0 )
    {
        p  = va_arg(arg_prt,char *);
        len= va_arg(arg_prt,int);

        ret = fprintf( stderr, "\n[%s - %d]: == RAW_BUF_LEN=[%d] ==",
                       time_str, getpid(), len );

        for( i=0; i<len; i++ )
        {
            if ( !(i%25) )
                ret = fprintf( stderr, "\n   " );
            ret = fprintf( stderr, "%02X ", *(unsigned char *)(p+i) );
        }
        ret = fprintf( stderr, "\n");
    }
    else if ( strcmp( format, HEX_FORMAT )==0 )
    {
        p  = va_arg(arg_prt,char *);
        len= va_arg(arg_prt,int);

        if ( len==0 || p==NULL )
        {
            va_end (arg_prt);
            return 0;
        }

        ret = fprintf(stderr,"\n[%s][%d]: == HEX_BUF_LEN=[%d] ==\n0x00000000  ",
                      time_str, getpid(), len );

        memset(tmp_str, 0, sizeof(tmp_str));
        for( i=0; i<len; i++ )
        {
            if ( !(i%16) && i )
            {
                ret = fprintf( stderr, "   %s\n0x%08X  ", tmp_str, i );
                memset(tmp_str, 0, sizeof(tmp_str));
            }
            ret = fprintf( stderr, "%02X ", *(unsigned char *)(p+i) );
            tmp_str[(i%16)] = isprint(*(unsigned char *)(p+i)) ? *(unsigned char *)(p+i) : '.';
        }

        if ( i%16 )
        {
            memset(space_str, 0, sizeof(space_str));
            sprintf( space_str, "%48s", " " );
            space_str[((16-i%16)*3)] = 0;
            ret = fprintf( stderr, "%s", space_str );
        }
        ret = fprintf( stderr, "   %s\n", tmp_str );
        memset(tmp_str, 0, sizeof(tmp_str));
    }
    else
    {
        ret = fprintf( stderr, "\n[%s][%d]", time_str, getpid() );
        ret = vfprintf( stderr, format, arg_prt );
    }
    va_end (arg_prt);

    if ( ret < 0 )
    {
        setenv("__LOGERR", "1", 1);
    }
    
    return 0;
}
