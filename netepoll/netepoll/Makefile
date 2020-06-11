#
# Make the Proc netepoll
#

.SUFFIXES:
.SUFFIXES: .c .o

MAKEINC=../makeinc/make.inc
ifeq ($(MAKEINC), $(wildcard $(MAKEINC)))
include $(MAKEINC)
else
$(warning "[WARN] : Not Found $(MAKEINC)")
OS_TYPE=`uname`
CFLAGS= -c -D$(OS_TYPE)
endif

USER_LIB = -lbase -lpthread

INC_DIR = -I. -I../include
LIB_DIR = -L../lib
LIB_LIST = $(UNIXLIB) $(NETLIB) $(USER_LIB)

BINDIR  = $(HOME)/bin

PNAME = $(shell basename `pwd`)
SRC_LST := $(shell ls *.c)
OBJS = ${SRC_LST:.c=.o}

${PNAME}: $(OBJS)
	$(CC) $(OFLAGS) -g -o $@ ${OBJS} ${INC_DIR} ${LIB_DIR} ${LIB_LIST}
	@size $@
	@test -d "$(BINDIR)" || mkdir $(BINDIR)
	@mv $@ $(BINDIR)
	@rm ${OBJS}
	@echo "make $@ ok!"

.c.o:
	${CC} -Wall $(CFLAGS) -g ${INC_DIR} $<

clean :
	@-rm -f ${OBJS}

#
# End.
#
