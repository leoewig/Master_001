##############################################################################
# $Id: cal.mk 8311 2013-04-23 16:08:54Z pr4 $
##############################################################################
#
# Makefile fragment for Calibration
#
##############################################################################

FEAT_NAME:=cal

ifneq ($(VERBOSE),0)
    $(info Including feature $(FEAT_NAME))
endif

#SRCFILES+=cal.c cal_cosmic.c cal_offline.c
SRCFILES+=cal.c cal_cosmic.c

A2L_FEATURE_FILES+=cal_offline.a2l

