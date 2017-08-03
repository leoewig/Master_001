##############################################################################
# $Id: mdm.mk 5399 2012-11-28 14:46:26Z pr4 $
##############################################################################
#
# Makefile fragment for Measurement data manager
#
##############################################################################

FEAT_NAME:=mdm

ifneq ($(VERBOSE),0)
    $(info Including feature $(FEAT_NAME))
endif

SRCFILES+=mdm.c mdm_ee.c

#SRCFILES+=mdm.c mdm_ee.c mdm_fp_util.c
