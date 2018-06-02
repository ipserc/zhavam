#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = gcc

# define any compile-time flags
#------ CFLAGS += -Wall -fopenmp -g $(shell pkg-config --cflags gtk+-3.0 gmodule-2.0 glib-2.0 libxml-2.0)  
CFLAGS += -Wall -fopenmp -g $(shell pkg-config --cflags gtk+-3.0 glib-2.0 libxml-2.0 libconfig) 
#$(shell xml2-config --cflags)

# define any directories containing header files other than /usr/include
#
# INCLUDES = -I/home/newhall/include  -I../include
INCLUDES += -I/opt/include
#INCLUDES += -include jsmnRipper.h -include jsmn.h

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
# LFLAGS = -L/home/newhall/lib  -L../lib
# Removed cause gmodule is dumping core LFLAGS += $(shell pkg-config --libs gtk+-3.0 gmodule-2.0 glib-2.0 libxml-2.0)
LFLAGS += $(shell pkg-config --libs gtk+-3.0 glib-2.0 libxml-2.0 libconfig)
LFLAGS += -L/home/ipserc/workspace_oxigen/zhavam/lib -L/opt/lib

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
# LIBS = -lmylib -lm -ljsmnRipper -llist
LIBS += -lz -lm -lasound -lacrcloud_recognizer

# define the C source files
# SRCS = emitter.c error.c init.c lexer.c main.c symbol.c parser.c

SRCS =  jsmn.c \
		jsmnRipper.c \
		list.c \
		zhavam.c  \
		zhavam_acrcloud.c  \
		zhavam_alsa.c  \
		zhavam_devices.c  \
		zhavam_errtra.c  \
		zhavam_config.c  \
		zhavam_glade.c	\
		zhavam_jsonparser.c

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.c=.o)

# define the executable file 
MAIN = zhavam

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:	$(MAIN)
	@echo
	@echo ===========================================
	@echo  $(MAIN) has been compiled... 
	@echo  Compilation:`date`
	@echo ===========================================
	@echo

glade:
	./gladec.sh ../config/zhavam.glade
	make
	
$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.c.o:
	./gladec.sh ../config/zhavam.glade
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^
# DO NOT DELETE

jsmn.o: jsmn.h
list.o: list.h /usr/include/stdio.h /usr/include/libio.h
list.o: /usr/include/_G_config.h /usr/include/stdlib.h /usr/include/string.h
zhavam.o: /usr/include/errno.h /usr/include/features.h
zhavam.o: /usr/include/stdc-predef.h /usr/include/libconfig.h
zhavam.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
zhavam.o: /usr/include/stdlib.h /usr/include/string.h jsmn.h list.h zhavam.h
zhavam.o: zhavam_acrcloud.h acrcloud_recognizer.h zhavam_alsa.h
zhavam.o: /usr/include/alsa/asoundlib.h /usr/include/unistd.h
zhavam.o: /usr/include/fcntl.h /usr/include/assert.h /usr/include/endian.h
zhavam.o: /usr/include/alsa/asoundef.h /usr/include/alsa/version.h
zhavam.o: /usr/include/alsa/global.h /usr/include/time.h
zhavam.o: /usr/include/alsa/input.h /usr/include/alsa/output.h
zhavam.o: /usr/include/alsa/error.h /usr/include/alsa/conf.h
zhavam.o: /usr/include/alsa/pcm.h /usr/include/alsa/rawmidi.h
zhavam.o: /usr/include/alsa/timer.h /usr/include/alsa/hwdep.h
zhavam.o: /usr/include/alsa/control.h /usr/include/alsa/mixer.h
zhavam.o: /usr/include/alsa/seq_event.h /usr/include/alsa/seq.h
zhavam.o: /usr/include/alsa/seqmid.h /usr/include/alsa/seq_midi_event.h
zhavam.o: zhavam_config.h zhavam_devices.h zhavam_errtra.h
zhavam.o: zhavam_jsonparser.h
zhavam_acrcloud.o: /usr/include/string.h jsmn.h list.h zhavam.h
zhavam_acrcloud.o: zhavam_acrcloud.h acrcloud_recognizer.h zhavam_alsa.h
zhavam_acrcloud.o: /usr/include/alsa/asoundlib.h /usr/include/unistd.h
zhavam_acrcloud.o: /usr/include/features.h /usr/include/stdc-predef.h
zhavam_acrcloud.o: /usr/include/stdio.h /usr/include/libio.h
zhavam_acrcloud.o: /usr/include/_G_config.h /usr/include/stdlib.h
zhavam_acrcloud.o: /usr/include/fcntl.h /usr/include/assert.h
zhavam_acrcloud.o: /usr/include/errno.h /usr/include/endian.h
zhavam_acrcloud.o: /usr/include/alsa/asoundef.h /usr/include/alsa/version.h
zhavam_acrcloud.o: /usr/include/alsa/global.h /usr/include/time.h
zhavam_acrcloud.o: /usr/include/alsa/input.h /usr/include/alsa/output.h
zhavam_acrcloud.o: /usr/include/alsa/error.h /usr/include/alsa/conf.h
zhavam_acrcloud.o: /usr/include/alsa/pcm.h /usr/include/alsa/rawmidi.h
zhavam_acrcloud.o: /usr/include/alsa/timer.h /usr/include/alsa/hwdep.h
zhavam_acrcloud.o: /usr/include/alsa/control.h /usr/include/alsa/mixer.h
zhavam_acrcloud.o: /usr/include/alsa/seq_event.h /usr/include/alsa/seq.h
zhavam_acrcloud.o: /usr/include/alsa/seqmid.h
zhavam_acrcloud.o: /usr/include/alsa/seq_midi_event.h zhavam_config.h
zhavam_acrcloud.o: zhavam_devices.h zhavam_errtra.h zhavam_jsonparser.h
zhavam_alsa.o: /usr/include/errno.h /usr/include/features.h
zhavam_alsa.o: /usr/include/stdc-predef.h /usr/include/stdio.h
zhavam_alsa.o: /usr/include/libio.h /usr/include/_G_config.h
zhavam_alsa.o: /usr/include/stdlib.h /usr/include/string.h jsmn.h list.h
zhavam_alsa.o: zhavam.h zhavam_acrcloud.h acrcloud_recognizer.h zhavam_alsa.h
zhavam_alsa.o: /usr/include/alsa/asoundlib.h /usr/include/unistd.h
zhavam_alsa.o: /usr/include/fcntl.h /usr/include/assert.h
zhavam_alsa.o: /usr/include/endian.h /usr/include/alsa/asoundef.h
zhavam_alsa.o: /usr/include/alsa/version.h /usr/include/alsa/global.h
zhavam_alsa.o: /usr/include/time.h /usr/include/alsa/input.h
zhavam_alsa.o: /usr/include/alsa/output.h /usr/include/alsa/error.h
zhavam_alsa.o: /usr/include/alsa/conf.h /usr/include/alsa/pcm.h
zhavam_alsa.o: /usr/include/alsa/rawmidi.h /usr/include/alsa/timer.h
zhavam_alsa.o: /usr/include/alsa/hwdep.h /usr/include/alsa/control.h
zhavam_alsa.o: /usr/include/alsa/mixer.h /usr/include/alsa/seq_event.h
zhavam_alsa.o: /usr/include/alsa/seq.h /usr/include/alsa/seqmid.h
zhavam_alsa.o: /usr/include/alsa/seq_midi_event.h zhavam_config.h
zhavam_alsa.o: zhavam_devices.h zhavam_errtra.h zhavam_jsonparser.h
zhavam_devices.o: /usr/include/stdio.h /usr/include/libio.h
zhavam_devices.o: /usr/include/_G_config.h /usr/include/string.h jsmn.h
zhavam_devices.o: list.h acrcloud_recognizer.h zhavam.h zhavam_acrcloud.h
zhavam_devices.o: zhavam_alsa.h /usr/include/alsa/asoundlib.h
zhavam_devices.o: /usr/include/unistd.h /usr/include/features.h
zhavam_devices.o: /usr/include/stdc-predef.h /usr/include/stdlib.h
zhavam_devices.o: /usr/include/fcntl.h /usr/include/assert.h
zhavam_devices.o: /usr/include/errno.h /usr/include/endian.h
zhavam_devices.o: /usr/include/alsa/asoundef.h /usr/include/alsa/version.h
zhavam_devices.o: /usr/include/alsa/global.h /usr/include/time.h
zhavam_devices.o: /usr/include/alsa/input.h /usr/include/alsa/output.h
zhavam_devices.o: /usr/include/alsa/error.h /usr/include/alsa/conf.h
zhavam_devices.o: /usr/include/alsa/pcm.h /usr/include/alsa/rawmidi.h
zhavam_devices.o: /usr/include/alsa/timer.h /usr/include/alsa/hwdep.h
zhavam_devices.o: /usr/include/alsa/control.h /usr/include/alsa/mixer.h
zhavam_devices.o: /usr/include/alsa/seq_event.h /usr/include/alsa/seq.h
zhavam_devices.o: /usr/include/alsa/seqmid.h
zhavam_devices.o: /usr/include/alsa/seq_midi_event.h zhavam_config.h
zhavam_devices.o: zhavam_devices.h zhavam_errtra.h zhavam_jsonparser.h
zhavam_errtra.o: /usr/include/errno.h /usr/include/features.h
zhavam_errtra.o: /usr/include/stdc-predef.h /usr/include/stdio.h
zhavam_errtra.o: /usr/include/libio.h /usr/include/_G_config.h
zhavam_errtra.o: /usr/include/stdlib.h /usr/include/time.h
zhavam_errtra.o: acrcloud_recognizer.h jsmn.h list.h zhavam.h
zhavam_errtra.o: zhavam_acrcloud.h zhavam_alsa.h
zhavam_errtra.o: /usr/include/alsa/asoundlib.h /usr/include/unistd.h
zhavam_errtra.o: /usr/include/string.h /usr/include/fcntl.h
zhavam_errtra.o: /usr/include/assert.h /usr/include/endian.h
zhavam_errtra.o: /usr/include/alsa/asoundef.h /usr/include/alsa/version.h
zhavam_errtra.o: /usr/include/alsa/global.h /usr/include/alsa/input.h
zhavam_errtra.o: /usr/include/alsa/output.h /usr/include/alsa/error.h
zhavam_errtra.o: /usr/include/alsa/conf.h /usr/include/alsa/pcm.h
zhavam_errtra.o: /usr/include/alsa/rawmidi.h /usr/include/alsa/timer.h
zhavam_errtra.o: /usr/include/alsa/hwdep.h /usr/include/alsa/control.h
zhavam_errtra.o: /usr/include/alsa/mixer.h /usr/include/alsa/seq_event.h
zhavam_errtra.o: /usr/include/alsa/seq.h /usr/include/alsa/seqmid.h
zhavam_errtra.o: /usr/include/alsa/seq_midi_event.h zhavam_config.h
zhavam_errtra.o: zhavam_devices.h zhavam_errtra.h zhavam_jsonparser.h
zhavam_config.o: /usr/include/stdio.h /usr/include/libio.h
zhavam_config.o: /usr/include/_G_config.h jsmn.h list.h zhavam.h
zhavam_config.o: zhavam_acrcloud.h acrcloud_recognizer.h zhavam_alsa.h
zhavam_config.o: /usr/include/alsa/asoundlib.h /usr/include/unistd.h
zhavam_config.o: /usr/include/features.h /usr/include/stdc-predef.h
zhavam_config.o: /usr/include/stdlib.h /usr/include/string.h
zhavam_config.o: /usr/include/fcntl.h /usr/include/assert.h
zhavam_config.o: /usr/include/errno.h /usr/include/endian.h
zhavam_config.o: /usr/include/alsa/asoundef.h /usr/include/alsa/version.h
zhavam_config.o: /usr/include/alsa/global.h /usr/include/time.h
zhavam_config.o: /usr/include/alsa/input.h /usr/include/alsa/output.h
zhavam_config.o: /usr/include/alsa/error.h /usr/include/alsa/conf.h
zhavam_config.o: /usr/include/alsa/pcm.h /usr/include/alsa/rawmidi.h
zhavam_config.o: /usr/include/alsa/timer.h /usr/include/alsa/hwdep.h
zhavam_config.o: /usr/include/alsa/control.h /usr/include/alsa/mixer.h
zhavam_config.o: /usr/include/alsa/seq_event.h /usr/include/alsa/seq.h
zhavam_config.o: /usr/include/alsa/seqmid.h
zhavam_config.o: /usr/include/alsa/seq_midi_event.h zhavam_config.h
zhavam_config.o: zhavam_devices.h zhavam_errtra.h zhavam_jsonparser.h
zhavam_jsonparser.o: /usr/include/stdio.h /usr/include/libio.h
zhavam_jsonparser.o: /usr/include/_G_config.h /usr/include/stdlib.h
zhavam_jsonparser.o: /usr/include/string.h acrcloud_recognizer.h jsmn.h
zhavam_jsonparser.o: list.h zhavam.h zhavam_acrcloud.h zhavam_alsa.h
zhavam_jsonparser.o: /usr/include/alsa/asoundlib.h /usr/include/unistd.h
zhavam_jsonparser.o: /usr/include/features.h /usr/include/stdc-predef.h
zhavam_jsonparser.o: /usr/include/fcntl.h /usr/include/assert.h
zhavam_jsonparser.o: /usr/include/errno.h /usr/include/endian.h
zhavam_jsonparser.o: /usr/include/alsa/asoundef.h /usr/include/alsa/version.h
zhavam_jsonparser.o: /usr/include/alsa/global.h /usr/include/time.h
zhavam_jsonparser.o: /usr/include/alsa/input.h /usr/include/alsa/output.h
zhavam_jsonparser.o: /usr/include/alsa/error.h /usr/include/alsa/conf.h
zhavam_jsonparser.o: /usr/include/alsa/pcm.h /usr/include/alsa/rawmidi.h
zhavam_jsonparser.o: /usr/include/alsa/timer.h /usr/include/alsa/hwdep.h
zhavam_jsonparser.o: /usr/include/alsa/control.h /usr/include/alsa/mixer.h
zhavam_jsonparser.o: /usr/include/alsa/seq_event.h /usr/include/alsa/seq.h
zhavam_jsonparser.o: /usr/include/alsa/seqmid.h
zhavam_jsonparser.o: /usr/include/alsa/seq_midi_event.h zhavam_config.h
zhavam_jsonparser.o: zhavam_devices.h zhavam_errtra.h zhavam_jsonparser.h
