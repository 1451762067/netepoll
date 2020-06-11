# 
#  Use For Create the Programs for this System 
# 

PROJECT_NAME= $(shell basename `pwd`)
VERSION	= 1.0.1
DATE	= `date +%y%m%d`
BAK_LST= $(shell ls)

SUBDIRS	= makeinc lib_s nettest
		  
BIN_LST = 

default:: $(SUBDIRS)

$(SUBDIRS):: 
	@(test ! -d $(@) || $(MAKE) -C $(@))
	@echo ".... Make All Targets OK!..."

#
# Clean up after ourselves...
#
clean::
	@for i in $(SUBDIRS); do\
		(test ! -d $${i} || $(MAKE) -C $${i} $@); \
	done
	@echo "Make All Clean."

#
# Make a compressed package of source code
#
backup:
	@-mkdir -p $(HOME)/bak 1>/dev/null 2>/dev/null
	@tar cvf $(HOME)/bak/$(PROJECT_NAME)-$(VERSION).$(DATE).tar $(BAK_LST)
	@gzip -f $(HOME)/bak/$(PROJECT_NAME)-$(VERSION).$(DATE).tar

#
# Make a compressed package of bin proc
#
brelease:
	@(cd $(HOME);tar zcvf $(HOME)/bin.$(DATE).tgz $(BIN_LST))

#
# End.
#
