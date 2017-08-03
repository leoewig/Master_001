# $Id: exc.mk 7205 2013-02-20 12:09:47Z gvs $
#
# Makefile fragment for External CAN driver library (CANbedded interface)

FEAT_NAME:=exc

ifneq ($(VERBOSE),0)
    $(info Including feature $(FEAT_NAME))
endif

EXC_SRC=exc.c exc_canbedded_callbacks.c exc_integrity.c
FORCE_FAR_SRC+=$(EXC_SRC)
SRCFILES+=$(EXC_SRC)

# CHECK_EXCLUDE+=exc_canbedded_callbacks.c exc.c
