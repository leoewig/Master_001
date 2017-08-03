# $Id: platform.mk 11357 2013-11-21 10:43:53Z pr4 $
# Makefile fragment for platform code

FEAT_NAME=platform

ifneq ($(VERBOSE),0)
    $(info Including feature $(FEAT_NAME))
endif

# core list of modules used by all MMC-based applications
# These will be linked into prodder, plat_test, mmc and vrodder

PLATFORM_CORE_SRC=crtsi3.s \
      static_initialisation.s \
      lmlsw.s\
      main_isr.c \
      gate.c \
      ai.c \
	  LookUp_real32_T_real32_T_ZERO.c \
      dio.c \
	  tmi_tables.c \
      spi_init.c spimaster.c \
      ican.c ican_init.c \
      regs.c \
      ts.c \
      i2c.c \
      xgate_error_handler.c \
      xgnop.c \
      xgate_icm.c \
      danomem.c
ifndef VRODDER
# vrodder has no eeprom
PLATFORM_CORE_SRC+= \
     nvs_cfg.c \
     nvs.c \
     eeprom.c
endif

# These are the sources shared for both plat_test and mmc
PLATFORM_SHARED_SRC =\
     fm.c \
     om.c \
     rtc.c \
     smg.c \
     platform.c \
     wus.c

    # core list of modules used by all mmc based applications
    # These will be linked into prodder, plat_test and mmc
    PLATFORM_SRC=$(PLATFORM_CORE_SRC)

    ifndef PRODDER
    ifndef VRODDER
	# These sources are shared between both plat_test and mmc targets
	PLATFORM_SRC+=$(PLATFORM_SHARED_SRC)
    endif
    endif

    ifdef PLATFORM_TEST
	# These are plat_test only
    endif


SRCFILES+=$(PLATFORM_SRC)

A2L_FEATURE_FILES+=platform.a2l mmc_conversions.a2l fm.a2l
