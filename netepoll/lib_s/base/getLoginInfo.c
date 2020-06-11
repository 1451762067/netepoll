
/* ------------------------------------------------------------------------ 
 * FileName : getLoginInfo.c (1.100)
 * Function : Read the DB Login config file And return the Login Info 
 * ------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libbase.h"

/*
 * 功  能： 从配置文件中读取DB相关登陆信息 
 * 输  入： char *DB_name  服务器名称
 *          char *User_name  用户名
 *          char *Passwd  密码
 *          char Option  配置文件选项
 * 输  出： 无
 * 返回值： 0 成功  -1 失败
 */

int getLoginInfo(char *DB_name, char *User_name, char *Passwd, char Option)
{
    char     cfg_filename[512];
    FILE     *fp;
    char     buf[81];
    int      found_flag=0;

    memset(cfg_filename, 0 ,sizeof(cfg_filename));
    sprintf( cfg_filename, "%s/etc/dblogin.ini", getenv("HOME") );
    if ( (fp=fopen( cfg_filename, "r" ))==NULL )
    {
        return (-__LINE__);
    }

    while(!found_flag)
    {
        memset(buf, 0, sizeof(buf));
        if( fgets( buf, 80, fp )==NULL )
        {
            fclose(fp);
            return (-__LINE__);
        }

        if ( buf[0]=='#' || buf[0]==' ' || buf[0]=='\t' || buf[0]=='\n')
        {
           continue;
        }

        if( buf[1]==Option )
        {
            // 用 : 分隔 
            if ( buf[3] == ':' )
                sscanf( buf+4, "%[^:]:%[^:]:%s", DB_name, User_name, Passwd );
            else // 用空格分隔
                sscanf( buf+4, "%s%s%s", DB_name, User_name, Passwd );
            found_flag=1;
        }
    }

    deleRhtSpace(DB_name);
    deleRhtSpace(User_name);
    deleRhtSpace(Passwd);

    fclose(fp);

    return 0;
}
