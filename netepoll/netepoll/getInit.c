
/* -------------------------------------------------------
 * FileName  : getInit.c (1.40)
 * Function  :
 * CreateInfo: tangyx @ 2014.12.17 11:06:34
 * ------------------------------------------------------- */
 
#include "netepoll.h"

int getInit(int argc, char **argv)
{
    FILE *fp;
    char tmp[256];
    char log_file[512];
    int  opt;

    memset(tmp, 0, sizeof(tmp));
    strcpy(tmp, argv[0]);
    memset(ProgName, 0, 30);
    strcpy(ProgName, basename(tmp));

    /* ����ȱʡ�����ļ� */
    sprintf(IniFile, "%s/etc/%s.ini", getenv("HOME"), ProgName);
    sprintf(CfgFile, "%s/etc/%s.cfg", getenv("HOME"), ProgName);

    /* �����������Ƿ��д��Զ��������ļ� */
    while ((opt = getopt(argc, argv, "vI:F:")) != -1)
    {
        switch (opt)
        {
            case 'v':
                SHOW_PROGRAM_INFO(ProgName,"1.40");
                break;
            case 'I':
                strcpy(IniFile, optarg);
                break;
            case 'F':
                strcpy(CfgFile, optarg);
                break;
            default: /* '?' */
                fprintf(stderr, "%s : unkown param [-%c]\n", ProgName, opt);
                return -1;
        }
    }

    // ��ini�ļ���ȡ������Ϣ
    if ( (fp=fopen( IniFile, "r" )) == NULL )
    {
        fprintf(stderr, "�������ļ�[%s]����!\n", IniFile);
        return -1;
    }

    fscanf(fp, "%s%d", tmp, &Debug);
    fscanf(fp, "%s%s", tmp, log_file);
    fscanf(fp, "%s%s", tmp, HostPort);
    fscanf(fp, "%s%d", tmp, &TimeOut);

    fclose(fp);
        
    setDebugLevel(Debug);
    if (Debug)
    {
        openLogFile(log_file);
    }

    /*
     * ����Ϊ��̨����
     */
    switch( fork( ) )
    {
        case 0  :  break;
        case -1 :  exit ( -1 );
        default :  exit ( 0 );
    }

    return 0;
}
