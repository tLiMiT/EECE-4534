#############################################################################
# Makefile: lab4/audio_dma_queue/src				  					            #
#############################################################################
#
# Aug 11, 2011 <agarwal>  Add description.
#	

# set CC to cross compiler 
CC = bfin-elf-gcc

# -- Compile Flags
CFLAGS = -Wall -specs=$(TLL6527M_C_DIR)/common/inc/spec/tll6527m 
# add debug flag 
CFLAGS += -g


# -- Include Path
INC_PATH = -I ../inc

# -- Objects 
OBJS =   main.o \
         audioPlayer.o \
         audioRx.o \
         audioTx.o \
         audioSample.o

# --- Libraries 	
LIBS     = -ltll6527mC   

# --- name of final binary 
TARGET = audio_dma_queue_skel

# Linker file 
LDFLAGS = -T$(TLL6527M_C_DIR)/common/inc/spec/tll6527m_sdram.lds
#LDFLAGS = -T../$(TARGET).lds

# check that TLL6527M_C_DIR variable is defined before starting
ifeq ($(TLL6527M_C_DIR),)
$(error ERROR: TLL6527M_C_DIR environment var not set! Source environment settings first (e.g. source setup.csh))
endif

# --- Compilation 

# default rule
all: $(TARGET)

# link final binary
$(TARGET):$(OBJS) 
	$(CC) $(INC_PATH) $(LIB_PATH) $(CFLAGS) -o $@ $^ $(LIBS) $(LDFLAGS)

# pattern rule for compiling into object files
.c.o:
	$(CC)  $(INC_PATH) -g -c $(CFLAGS)  -o $@ $<  
	
# --- Clean	
clean: 
	rm -rf $(TARGET) $(OBJS)		



