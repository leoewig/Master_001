# MMC configuration makefile fragment
#
# $Id: mmc_cfg.mk 12602 2015-02-26 10:47:56Z pr4 $

FEAT_NAME:=mmc_cfg

ifneq ($(VERBOSE),0)
    $(info Including feature $(FEAT_NAME))
endif

ifdef PRODDER
# sources used by prodder
SRCFILES+=\
	mmc_danomem.c
else
# sources used by mmc and platform test
SRCFILES+=\
	mmc_dispatch_mmc_utils.c \
	mmc_dio_cfg.c \
	mmc_ican_cfg.c mmc_ican.c \
	mmc_spi_cfg.c mmc_danomem.c \
	mmc_hw_version.c \
	mmc_sensor_cal.c \
	mmc_eol.c

ifndef PLATFORM_TEST
# sources used by mmc only
SRCFILES+=\
	mmc_aps.c \
	mmc_interrupts.c \
	xgmmc_gate_init.c \
	xgmmc_gate_vector.c

# unqualified .dox file names
RAWDOXFILES=mmc_cfg.dox charger.dox isa_control.dox \
	    vehicle.dox vehicle_dbc_hand_edits.dox charger_dbc_hand_edits.dox
# add to path-qualified list of .dox files
# NB += wouldn't work because we need the current FEAT_NAME
DOTDOXFILES:=$(DOTDOXFILES) \
    	$(addprefix $(PROTO_ROOT)/$(FEAT_NAME)/, $(RAWDOXFILES))

# mmc_vp and mmc_prod targets should output fault cal reports
BUILD_TARG_DEPS+=$(FM_CAL_REPORT) $(FM_CAL_SUMMARY)
MISC_TO_CLEAN+=$(FM_CAL_REPORT) $(FM_CAL_SUMMARY)

endif
endif

##############################################################################
# add a rule for generating a textual list of error codes and make the mmc
# depend on it - so we'll always get it for an mmc build
mmc_vp: fm_faults.txt
mmc_prod: fm_faults.txt

# and make sure it gets cleaned up
MISC_TO_CLEAN+=fm_faults.txt

# This means:
# 1. Extract lines starting with optional whitespace and FAULT_TABLE
# 2. Extract from this the word between the first '(' and the first comma
# 	(i.e. the fault name) and everything from the first quote to the last
# 3. Print this text preceded by a counter starting from zero
# 	(by using awk to print one less than the line number)
# 4. Extract lines starting with optional whitespace and FAULT_SUBTYPE
# 5. Extract from this the word between the first '(' and the first ')'
# 	(i.e. the subtype name)
# 6. Print this text preceded by 'subtype' and a counter starting from zero
# 	(by using awk to print one less than the line number)
# (it's all done in subshells so that $(PFX) works)
fm_faults.txt: fm.def
	$(PFX) (egrep ^[[:space:]]*FAULT_TABLE $^ \
	    | sed 's/[^(]*(\([^,]*\)[^"]*\(.*"\).*/\1  \2/' \
	    | awk '{print NR-1,$$0}' > $@)
	$(PFX) (egrep ^[[:space:]]*FAULT_SUBTYPE $^ \
	    | sed 's/[^(]*(\([^)]*\).*/\1/' \
	    | awk '{print "subtype",NR-1,$$0}' >> $@)

##############################################################################

# The MMC has to link in separate banked object files
S12_LNK_FILES=s12_bank_objs-1.blnk s12_bank_objs-2.blnk
# These parameters determine the split of files between the two chunks of
# .ftext in the mmc, the numbers indicate the maximum number of ppages
# that can be allocated there, this needs to match up with the allocated size of
# .ftext regions in the linker file.
# each ppage is 16k of code.
# 
S12_BANK_SPLIT_ARGS=12 s12_bank_objs-1.blnk 14 s12_bank_objs-2.blnk

# The generation of the link files depends on this makefile
$(S12_LNK_FILES): $(PROTO_ROOT)/$(FEAT_NAME)/$(FEAT_NAME).mk

# Linker file used by mmc, platform test, prodder and vrodder
# note ':=' because we need to bind the name before FEAT_NAME gets redefined
LINKER_FILE:=$(PROTO_ROOT)/$(FEAT_NAME)/mmc.lkf

A2L_FEATURE_FILES+=mmc_hw_version.a2l mmc_eol.a2l

#$(info  DEBUGINFO: depth:$(MAKELEVEL) goal:'$(MAKECMDGOALS)' restarts:'$(MAKE_RESTARTS)')

ifdef GTR_BUILD

# If a GTR build is needed, add the relevant symbol to the defs
# If this becomes a more permanent feature, make it into a separate target
# You MUST rebuild obd_ids.o if you change the setting of this in the Makefile
ifneq ($(MAKECMDGOALS),depend)
$(info ************************************************)
$(info Performing GTR build for $(BUILD_TARG) )
$(info ************************************************)
endif # ($(MAKECMDGOALS),depend)

DEFS+=GTR_BUILD

else
# Standard non-GTR build

ifneq ($(MAKECMDGOALS),depend)
$(info ************************************************)
$(info Performing standard build  for $(BUILD_TARG) )
$(info ************************************************)
endif # ($(MAKECMDGOALS),depend)

endif #GTR_BUILD

