##############################################################################
# $Id: ms.mk 4215 2012-10-09 13:44:56Z tg10 $
##############################################################################
#
# Makefile fragment for main state machine feature
#
##############################################################################


FEAT_NAME:=ms

ifneq ($(VERBOSE),0)
    $(info Including feature $(FEAT_NAME))
endif

SRCFILES+=ms.c

A2L_FEATURE_FILES+=ms.a2l
