###############################################################################
# $Id: ect.mk 9527 2013-07-16 15:03:22Z pr4 $
###############################################################################
#
# Makefile fragment for Vehicle input/output
#
################################################################################

FEAT_NAME:=ect

ifneq ($(VERBOSE),0)
    $(info Including feature $(FEAT_NAME))
endif

SRCFILES+=ect.c
//A2L_FEATURE_FILES+=ect.a2l

