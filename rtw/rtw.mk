###############################################################################
# $Id: rtw.mk 9527 2013-07-16 15:03:22Z pr4 $
###############################################################################
#
# Makefile fragment for rtw
#
################################################################################

FEAT_NAME:=rtw

ifneq ($(VERBOSE),0)
    $(info Including feature $(FEAT_NAME))
endif

FEAT_DIR:=$(PROTO_ROOT)/$(FEAT_NAME)
ADDITIONAL_INC_DIRS+=$(FEAT_DIR)

SRCFILES+= LDT_TMM_Model.c LDT_TMM_Model_data.c rt_nonfinite.c rtGetInf.c rtGetNaN.c
