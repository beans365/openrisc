##
## $Id: Makefile.am,v 1.2 2001-09-27 12:01:28 chris Exp $
##

AUTOMAKE_OPTIONS = foreign 1.4

VPATH = @srcdir@:@srcdir@/../../ppc403/vectors

## FIXME

PGM = ${ARCH}/vectors.rel

## Assembly sources
S_FILES = vectors.S align_h.S

vectors_rel_OBJECTS = $(S_FILES:%.S=${ARCH}/%.o)

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../../../../../automake/lib.am

#
# (OPTIONAL) Add local stuff here using +=
#

$(PGM): $(vectors_rel_OBJECTS)
	$(make-rel)

all-local: ${ARCH} $(PGM)

EXTRA_DIST = vectors.S README

include $(top_srcdir)/../../../../../automake/local.am
