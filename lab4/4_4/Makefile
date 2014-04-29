##############################################################################
# Makefile: lab4/audio_dma_queue_skel                                           
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
