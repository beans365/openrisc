##
## $Id: Makefile.am,v 1.2 2001-09-27 12:02:53 chris Exp $
##

AUTOMAKE_OPTIONS = foreign 1.4
ACLOCAL_AMFLAGS = -I $(RTEMS_TOPdir)/aclocal

bin_PROGRAMS = cklength eolstrip packhex unhex

noinst_PROGRAMS = binpatch

cklength_SOURCES = cklength.c
eolstrip_SOURCES = eolstrip.c
packhex_SOURCES = packhex.c
unhex_SOURCES = unhex.c
binpatch_SOURCES = binpatch.c

bin_SCRIPTS = install-if-change lock-directory unlock-directory

noinst_SCRIPTS = search-id.sh

include $(top_srcdir)/../../automake/host.am
