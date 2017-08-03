# Makefile fragment for the Flash controller
# 
# NB This feature is included by the bootblock
# 
# $Id: flash.mk 9745 2013-07-31 11:11:40Z vs7 $

FEAT_NAME=flash

ifneq ($(VERBOSE),0)
    $(info Including feature $(FEAT_NAME))
endif

SRCFILES+= \
       flash_eee_bblk_layout.c \
       flash_eee.c

ifdef BBLK
    SRCFILES+= flash_eee_bblk.c \
	       flash_check.c \
	       flash_prog.c
else
ifdef FBL
    SRCFILES+= flash_eee_bblk.c \
	       flash_check.c
else
    # it's an application
    SRCFILES+= flash_fault.c
endif
endif
