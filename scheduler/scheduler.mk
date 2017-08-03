
FEAT_NAME=scheduler

ifneq ($(VERBOSE),0)
    $(info Including feature $(FEAT_NAME) )
endif


SRCFILES+= scheduler.c
