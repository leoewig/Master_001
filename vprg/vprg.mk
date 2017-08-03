##############################################################################
# $Id: vprg.mk 8306 2013-04-23 15:33:13Z gvs $
##############################################################################
#
# Makefile fragment for VTBM reprogramming feature
#
##############################################################################

FEAT_NAME:=vprg

ifneq ($(VERBOSE),0)
    $(info Including feature $(FEAT_NAME))
endif

SRCFILES+=vprg_state.c vprg_image.c vprg_comms.c

A2L_FEATURE_FILES+=vprg.a2l
