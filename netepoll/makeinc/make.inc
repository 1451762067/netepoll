#
# This file is the Makefile include file
# Usage : Set the flags for OS Linux
#

DB = POSTGRESQL
OS_TYPE=`uname`
CC=cc
CFLAGS= -c -D$(OS_TYPE) 
OFLAGS= -O2 
SOFLAGS= -shared 
UNIXLIB= -ldl 
NETLIB=
OPENSSL_INCPATH=/usr/openssl-1.1.0h/include/
OPENSSL_LIBPATH=/usr/openssl-1.1.0h/lib/
#
# Default DB = Oracle 
#
PRECOMPILER=$(ORACLE_HOME)/bin/proc
PRECOMPFLAGS= lines=yes UNSAFE_NULL=YES DBMS=V8
DB_LIBS= -lclntsh 
DATABASE_INCPATH=$(ORACLE_HOME)/precomp/public
DATABASE_LIBPATH=$(ORACLE_HOME)/lib
OCI_INCPATH=$(ORACLE_HOME)/rdbms/public

ifeq ($(DB), DB2)
	PRECOMPILER=$(DB2_HOME)/sqllib/bin/db2
	PRECOMPFLAGS= 
	DB_NAME=aip_m35
	DB_USER=aip_m35
	DB_PW=aip_m35
	DB_LIBS= -ldb2 -lm
	DATABASE_INCPATH=$(DB2_HOME)/sqllib/include
	DATABASE_LIBPATH=$(DB2_HOME)/sqllib/lib
	BND_DIR= ~/bind
endif

ifeq ($(DB), POSTGRESQL)
    PRECOMPILER=$(PG_HOME)/bin/ecpg
    PRECOMPFLAGS=
    DB_LIBS= -lecpg
    DATABASE_INCPATH=$(PG_HOME)/include/
    DATABASE_LIBPATH=$(PG_HOME)/lib
endif

#
# End.
#
