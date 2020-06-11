#
# Description: makefile of base libs
#

.SUFFIXES:
.SUFFIXES: .c .o

MAKEINC=../../makeinc/make.inc
ifeq ($(MAKEINC), $(wildcard $(MAKEINC)))
include $(MAKEINC)
else
$(warning "[WARN] : Not Found $(MAKEINC)")
OS_TYPE=`uname`
CFLAGS= -c -D$(OS_TYPE)
endif

INC_DIR = ../../include/
LIB_DIR = ../../lib/

PNAME = $(shell basename `pwd`)
LIBNAME=lib${PNAME}.a
LIBHEAD=lib${PNAME}.h
SONAME=lib${PNAME}.so

SRC_LST := $(wildcard *.c)
OBJ_LIST = ${SRC_LST:.c=.o}

all: $(LIBNAME) 

$(LIBNAME): $(OBJ_LIST) 
	@echo "Compiling the lib $@"
	ar q $@ $(OBJ_LIST) 
	@echo "---$@ loaded."
	@test -d "$(LIB_DIR)" || mkdir $(LIB_DIR)
	@mv $@ $(LIB_DIR)
	@test -d "$(INC_DIR)" || mkdir $(INC_DIR)
	@cp $(LIBHEAD) $(INC_DIR)

$(SONAME) : $(OBJ_LIST) 
	@echo "Compiling the lib $@"
	cc -g -shared -fPIC -o $@ $(OBJ_LIST) 
	@echo "---$@ loaded."
	@test -d "$(LIB_DIR)" || mkdir $(LIB_DIR)
	@mv $@ $(LIB_DIR)
	@test -d "$(INC_DIR)" || mkdir $(INC_DIR)
	@cp $(LIBHEAD) $(INC_DIR)

.c.o:
	$(CC) -Wall -fPIC $(CFLAGS) -I$(INC_DIR) $< 

clean:
	@-rm -f $(OBJ_LIST) $(LIBNAME) $(LIB_DIR)$(LIBNAME)

# End .
