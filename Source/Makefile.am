## Process this file with automake to generate a Makefile.in
## To build all programs with GTK+ uncomment these lines.
##AM_CPPFLAGS = @GTK_CFLAGS@
##AM_LDADD = @GTK_LIBS@

AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = zhavam
zhavam_SOURCES = jsmn.c \
		jsmnRipper.c \
		list.c \
		zhavam.c  \
		zhavam_acrcloud.c  \
		zhavam_alsa.c  \
		zhavam_pulse.c  \
		zhavam_devices.c  \
		zhavam_errtra.c  \
		zhavam_config.c  \
		zhavam_glade.c	\
		zhavam_jsonparser.c

## Only our GUI program uses GTK+ at the moment.
zhavam_CPPFLAGS = @GTK_CFLAGS@
zhavam_LDADD = @GTK_LIBS@

CLEANFILES = *~