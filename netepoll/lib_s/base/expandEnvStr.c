
/* -------------------------------------------------------
 * FileName  : expandEnvStr.c (1.100)
 * Function  :
 * ------------------------------------------------------- */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define __ENV_NAME_SIZE   100
#define __ENV_VALUE_SIZE  256
#define __ENV_CHR         "_0123456789"\
                          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

// 获取环境变量对应值
int getEnvVal(char *EnvStr, char *EnvVal, int Size)
{
    char    env_name[__ENV_NAME_SIZE+1];
    char    *env_val;
    int     name_len;

    name_len = strspn(EnvStr, __ENV_CHR);
    if (name_len > __ENV_NAME_SIZE)
    {
        return -1;
    }

    memset(env_name, 0, sizeof(env_name));
    memcpy(env_name, EnvStr, name_len);

    /* eg.: ${VAR} */
    if (name_len == strlen(EnvStr))
    {
        env_val = getenv(env_name);
        if (env_val != NULL)
        {
            strncpy(EnvVal, env_val, Size);
        }
    }
    /*
     * 当变量未定义或者值为空时,返回值为word的内容,否则返回变量的值
     * eg.: ${VAR:-word}
     */
    else if (memcmp(EnvStr+name_len, ":-", 2) == 0)
    {
        env_val = getenv(env_name);
        if (env_val == NULL)
        {
            strncpy(EnvVal, EnvStr+name_len+2, Size);
        }
        else
        {
            strncpy(EnvVal, env_val, Size);
        }
    }
    /*
     * 若变量已赋值的话,其值才用word替换,否则不进行任何替换
     * eg.: ${VAR:+word}
     */
    else if (memcmp(EnvStr+name_len, ":+", 2) == 0)
    {
        env_val = getenv(env_name);
        if (env_val != NULL)
        {
            strncpy(EnvVal, EnvStr+name_len+2, Size);
        }
    }
    /*
     * 无法识别字符，取环境变量
     */
    else
    {
        env_val = getenv(env_name);
        if (env_val != NULL)
        {
            strncpy(EnvVal, env_val, Size);
        }
    }

    return strlen(EnvVal);
}

int expandEnvStr(char *InBuf, char *OutBuf, int BufSize)
{
    int     i;
    int     len;
    int     pos;
    char    env_name[__ENV_NAME_SIZE+1];
    char    env_val[__ENV_VALUE_SIZE+1];
    int     name_len;
    int     val_len;
    char    *tmp;

    len = strlen(InBuf);
    i = pos = 0;

    while (i < len && pos < BufSize)
    {
        if (InBuf[i] != '$')
        {
            OutBuf[pos] = InBuf[i];
            i++;
            pos++;
        }
        /* eg.:$VAR或${VAR} */
        else
        {
            i++; // skip '$' 
            if (InBuf[i] != '{')   // $VAR
            {
                name_len = strspn(InBuf+i, __ENV_CHR);
                if (name_len > __ENV_NAME_SIZE)
                {
                    return -1;
                }

                memset(env_name, 0, sizeof(env_name));
                memcpy(env_name, InBuf+i, name_len);
                i += name_len;
            }
            else                 // ${VAR}
            {
                /* {}左右未对称 */
                tmp = index(InBuf+i, '}');
                if (tmp == NULL)
                {
                    break;
                }

                name_len = tmp-InBuf-i-1;
                if (name_len > __ENV_NAME_SIZE)
                {
                    return -1;
                }

                memset(env_name, 0, sizeof(env_name));
                memcpy(env_name, InBuf+i+1, name_len);
                i += name_len+2;
            }

            memset(env_val, 0, sizeof(env_val));
            if ((val_len=getEnvVal(env_name, env_val, __ENV_VALUE_SIZE)) < 0)
            {
                return -1;
            }

            /* 超过输出BUF空间，丢弃后面字符串 */
            if ((pos+val_len) >= BufSize)
            {
                break;
            }

            memcpy(OutBuf+pos, env_val, val_len);
            pos += val_len;
        }
    }

    return 0;
}
