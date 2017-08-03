###############################################################################
# $Id: vio.mk 9527 2013-07-16 15:03:22Z pr4 $
###############################################################################
#
# Makefile fragment for Vehicle input/output
#
################################################################################

FEAT_NAME:=vio

ifneq ($(VERBOSE),0)
    $(info Including feature $(FEAT_NAME))
endif

SRCFILES+=vio.c vio_read.c vio_write.c
A2L_FEATURE_FILES+=vio.a2l

