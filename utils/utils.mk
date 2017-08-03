# Utils makefile fragment
#
# The list of source files depends on whether the target is a bootblock
# (BBLK_BUILD is defined) or not
#
# $Id: utils.mk 11333 2013-11-18 14:16:07Z pr4 $
#
FEAT_NAME=utils

ifneq ($(VERBOSE),0)
    $(info Including feature $(FEAT_NAME))
endif

ifdef FBL
SRCFILES+= watchdog.c
else
ifdef BBLK
RAMSRCFILES += \
   cosmic_pointers.c \
   crc.c \

# watchdog.c contains both flash and ram code, so it is allocated
# to SRCFILES, but with a ramcode section
SRCFILES+= \
    watchdog.c
else
SRCFILES+= \
	watchdog.c \
	pit.c \
	isr_utils.c \
	interp.c \
	interp2d.c \
	fixedpoint.c \
	shift_helpers.c \
	shifts.c \
	crc.c \
	cosmic_pointers.c \
	strings.c \
	eol.c

ifdef PRODDER
SRCFILES+= \
	adc_conversion.c
endif

ifndef VTBM
SRCFILES+= \
	multiplication.c \
	division.c \
	casting.c \
	sensor_cal.c \
	stm.c

ifndef VRODDER
SRCFILES+= \
	timing_monitor.c
endif
endif
endif
endif

# include basic conversions file (it lives in include/ but we need to include
# it from a .mk somewhere, so why not here?)
A2L_FEATURE_FILES+=mmc_conversions.a2l
A2L_FEATURE_FILES+=timing_monitor.a2l


# note: don't use append operator because need to get $FEAT_NAME with
# its current value
DOTDOXFILES:=$(DOTDOXFILES) $(PROTO_ROOT)/$(FEAT_NAME)/utils.dox
