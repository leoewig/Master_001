##############################################################################
# $Id: ixcp.mk 9922 2013-08-08 08:55:03Z vs7 $
##############################################################################
#
# Makefile fragment for Internal XCP
#
##############################################################################

FEAT_NAME:=ixcp

ifneq ($(VERBOSE),0)
    $(info Including feature $(FEAT_NAME))
endif

ifdef BBLK
    # In a bootloader these files should be linked against RAM libraries, not
    # flash
    RAMSRCFILES += ixcp_prog.c ixcp_slave.c
else
    SRCFILES+=ixcp_prog_allowed.c ixcp_slave.c 
endif
