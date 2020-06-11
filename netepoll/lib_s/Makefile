# 
#  Use For Create the Libs for this System 
# 

SUBDIRS:= base $(shell ls -d */)

default:: $(SUBDIRS)

$(SUBDIRS):: 
	@(test ! -f $(@)/Makefile || $(MAKE) -C $(@))

#
# Clean up after ourselves...
#
clean::
	@for i in $(SUBDIRS); do\
		(test ! -d $${i} || $(MAKE) -C $${i} $@); \
	done
	@echo "Make All Clean."

#
# End
#
