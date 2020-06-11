
/* -------------------------------------------------------
 * FileName  : procDebugLevel.c (1.100)
 * Function  :
 * ------------------------------------------------------- */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int setDebugLevel( int Level )
{
    char     tmp_str[80];

    memset( tmp_str, 0, sizeof(tmp_str) );
    sprintf( tmp_str, "%d", Level );
    setenv("__DEBUGLVL", tmp_str, 1);

    return 0;
}

int getDebugLevel( )
{
    int      debug_level;
    char     tmp_str[80];
    
    if ( getenv("__DEBUGLVL") == NULL )
        debug_level = 0;
    else
    {
        memset( tmp_str, 0, sizeof(tmp_str) );
        strcpy( tmp_str, getenv("__DEBUGLVL") );
        debug_level = atoi(tmp_str);
    }

    return debug_level;
}
