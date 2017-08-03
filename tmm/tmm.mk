###############################################################################
# $Id: tmm.mk 9527 2013-07-16 15:03:22Z pr4 $
###############################################################################
#
# Makefile fragment for Vehicle input/output
#
################################################################################

FEAT_NAME:=tmm

ifneq ($(VERBOSE),0)
    $(info Including feature $(FEAT_NAME))
endif

SRCFILES+=tmm.c smm.c motor.c pid.c tmm_fm.c dac.c tmm_seq.c
