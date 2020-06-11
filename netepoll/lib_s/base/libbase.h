
/* --------------------------------------------------------
 * FileName : libbase.h (1.101)
 * Function : The libbase.a head file
 * -------------------------------------------------------- */

#ifndef _LIBBASE_H_
#define _LIBBASE_H_

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <memory.h>
#include <netinet/tcp.h>

/* msgtool.c�������� */
extern int clr_que( key_t );                            /* ɾ������ */
extern int get_qinfo( key_t , struct msqid_ds * );      /* ȡ����״̬��Ϣ */
extern int recv_qmsg( key_t , char *, long *, int );    /* ���ն�����Ϣ */
extern int send_qmsg( key_t , long , char *, int, int );/* ���Ͷ�����Ϣ */

extern unsigned char lrcBuf(unsigned char *, int ); /* ����LRC */
extern char *space(int);                            /* ���ɿո�, <512���ֽ� */
extern int getDateStr( char *, const char *, int ); /* ȡϵͳ����ʱ�� */
extern int getDate( char *, int );                  /* ȡ YYYYMMDD fmt date */
extern char *deleRhtSpace(char *string);    /* ɾ���ұ߿ո� */
extern char *deleLftSpace(char *string);    /* ɾ����߿ո� */
extern char *deleAllSpace(char *str);       /* ɾ�����пո� */

extern int toBcd(char *buf);
extern int ascToBcd( unsigned char *Bcdstr, unsigned char *AscStr, int AscLen );
extern int bcdToAsc( unsigned char *AscStr, unsigned char *BcdStr, int StrLen );
extern int ascToBcdLeft( unsigned char *Bcd, unsigned char *Asc, int AscLen );

extern int sendtoUdpSrv(char *, char *, unsigned char *, int);
extern int getCliSocket(const char *, const char *, const char *);
extern int getSrvSocket(const char *, const char *, int );

extern int getLoginInfo(char *, char *, char *, char );

/* tcptool.c�������� */
extern int read_t( int , unsigned char *, int , int );
extern int read_t_r( int , unsigned char *, int , int );
extern int accept_t( int , int );
extern int accept_t_r( int , int );
extern void thread_sleep(int);
extern void nop_timeout(int);
extern int setSockNoDelay( int );
extern int setSockTime( int , int );
extern int setSockBuff( int , int );
extern int setSockNonBlock( int );
extern int setSockBlock( int );
extern int tread(int , unsigned char *, int , int );

extern void asc_to_bcd( unsigned char *, unsigned char *, int, unsigned char );
extern void bcd_to_asc( unsigned char *, unsigned char *, int, unsigned char );
extern void XOR( unsigned char *, int , unsigned char *);

extern int expLogFile( char *, char *, int );
extern int openLogFile( char * );
extern int lprintf( char *, ... );
extern int getDebugLevel();
extern int setDebugLevel(int);

extern int expandEnvStr(char *, char *, int );
extern int getEnvVal(char *, char *, int );
extern char *genRandomStr(char*, int);

/* strtool.c�ĺ������� */
extern int isDigit (const char *, const int );
extern int isHex (const char *, const int );
extern int isAlnum (const char *, const int );
extern int isAlpha (const char *, const int );
extern int isAscii (const char *, const int );
extern int isPrint (const char *, const int );
extern int isYear (const int );
extern int isLeapYear (const int );
extern int isMonth (const int );
extern int isDay (const int, const int, const int );
extern int isDate (const char *); 
extern int isTime (const char *); 
extern int isDatetime (const char *); 
extern int isDateFmt (const char *); 
extern int isTimeFmt (const char *); 
extern char *str2upper(char *);
extern char *str2lower(char *);

#define SHOW_PROGRAM_INFO(pname,vstr) {\
        fprintf(stdout,"%s : Release(%s) BulidTime(%s-%s)\n",\
                pname, vstr, __DATE__,__TIME__);\
        exit(0);\
}

#ifndef MAX
    #define MAX(a,b) ((a>b)?(a):(b))
#endif

#ifndef MIN
    #define MIN(a,b) ((a>b)?(b):(a))
#endif

#define SW16(x) ((((short)(x)&0xff00)>>8)|(((short)(x)&0x00ff)<<8))

#define RAW_FORMAT      "RAW_FORMAT"
#define HEX_FORMAT      "HEX_FORMAT"

#define MY_NCPY(dst,sr,len) {\
        memcpy(dst,sr,(len));\
        dst[(len)] = '\0';\
}

/* ��־���� */
#define     DEBUG_LVL       3
#define     WARN_LVL        2
#define     ERROR_LVL       1

#define     _LINE_FMT     "[%s-%d]: "

#define     E_LOG(format, ...) {\
    if (getDebugLevel() >= ERROR_LVL)\
    {\
      memcmp(format,HEX_FORMAT,strlen(HEX_FORMAT))!=0 ? \
      lprintf("[ERROR]"_LINE_FMT format, __FILE__, __LINE__, ##__VA_ARGS__) : \
      (lprintf("[ERROR]"_LINE_FMT,__FILE__,__LINE__),\
      lprintf(format,##__VA_ARGS__));\
    }\
}

#define     W_LOG(format, ...) {\
    if (getDebugLevel() >= WARN_LVL)\
    {\
      memcmp(format,HEX_FORMAT,strlen(HEX_FORMAT))!=0 ? \
      lprintf("[WARN]"_LINE_FMT format, __FILE__, __LINE__, ##__VA_ARGS__) : \
      (lprintf("[WARN]"_LINE_FMT,__FILE__,__LINE__),\
      lprintf(format,##__VA_ARGS__));\
    }\
}

#define     D_LOG(format, ...) {\
    if (getDebugLevel() >= DEBUG_LVL)\
    {\
      memcmp(format,HEX_FORMAT,strlen(HEX_FORMAT))!=0 ? \
      lprintf("[DEBUG]"_LINE_FMT format, __FILE__, __LINE__, ##__VA_ARGS__) : \
      (lprintf("[DEBUG]"_LINE_FMT, __FILE__,__LINE__),\
      lprintf(format,##__VA_ARGS__));\
    }\
}

#endif /* _LIBBASE_H_ */
