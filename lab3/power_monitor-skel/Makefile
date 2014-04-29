#############################################################################
# Makefile: lab3/power_monitor-skel                                           
#############################################################################
#
# Aug 11, 2011 <agarwal>  Add description.
#	
SUBDIRS	=  src

# --- Compilation

all:subdirs

subdirs:
	@set -e ; for d in $(SUBDIRS); do	\
	  $(MAKE) -C $$d ;			\
	done

# --- Maintenance targets

clean:
	@set -e ; for d in $(SUBDIRS); do	\
	  $(MAKE) -C $$d $@ ;			\
	done
